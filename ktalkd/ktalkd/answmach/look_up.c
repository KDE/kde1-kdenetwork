/*
 * Copyright (c) 1983 Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/* From: @(#)look_up.c	5.7 (Berkeley) 3/1/91 */

#include "talk.h"
#include <syslog.h>
#include <unistd.h>
#include <errno.h>
#include "talk_ctl.h"

int look_for_invite(CTL_RESPONSE *rp);


#if 0
/* print_addr is a debug print routine */
void print_addr(struct sockaddr_in * addr)
{
	int i;

	syslog(LOG_DEBUG,"addr = %x, port = %o, family = %o zero = ",
		addr->sin_addr.s_addr, addr->sin_port, addr->sin_family);
	for (i = 0; i<8;i++)
	  syslog(LOG_DEBUG,"%o ", (int)addr->sin_zero[i]);

}
#endif

/*
 * See if the local daemon has an invitation for us.
 * Hopefully it has : we called the answering for that reason !
 */
int check_local(void)
{
	CTL_RESPONSE response;
	register CTL_RESPONSE *rp = &response;

	/* the rest of msg was set up in launch_ans_mach */
	msg.ctl_addr = *(struct sockaddr *)&ctl_addr;
	msg.ctl_addr.sa_family = htons(ctl_addr.sin_family);
	msg.id_num = 0;

#if 0
	print_addr((struct sockaddr_in *)&msg.addr);
	print_addr((struct sockaddr_in *)&msg.ctl_addr);
#endif

	/* must be initiating a talk. Send a LOOKUP msg. */
	if (!look_for_invite(rp))
		return 0;
	/*
	 * There was an invitation waiting for us, 
	 * so connect with the other (hopefully waiting) party 
	 */
	message("Waiting to connect with caller");
	do {
		if (rp->addr.sa_family != AF_INET)
			p_error("Response uses invalid network address");
		errno = 0;
		if (connect(sockt,
		    (struct sockaddr *)&rp->addr, sizeof (rp->addr)) != -1)
			return 1;
	} while (errno == EINTR);
	if (errno == ECONNREFUSED) {
		/*
		 * The caller gave up, but his invitation somehow
		 * was not cleared. Clear it and initiate an 
		 * invitation. (We know there are no newer invitations,
		 * the talkd works LIFO.)
		 */
		ctl_transact(his_machine_addr, msg, DELETE, rp);
		close(sockt);
		open_sockt();
		return 0;
	}
	p_error("Unable to connect with initiator");
	/*NOTREACHED*/
	return 0;
}

/*
 * Look for an invitation on 'machine'
 */
int look_for_invite(CTL_RESPONSE *rp)
{
  /* Checking for invitation on caller's machine"; */
	ctl_transact(his_machine_addr, msg, LOOK_UP, rp);
	/* the switch is for later options, such as multiple invitations */
	switch (rp->answer) {

	case SUCCESS:
		msg.id_num = htonl(rp->id_num);
		return (1);

	default:
		/* there wasn't an invitation waiting for us */
		return (0);
	}
}
