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

/*
 * process.c handles the requests, which can be of three types:
 *	ANNOUNCE - announce to a user that a talk is wanted
 *	LEAVE_INVITE - insert the request into the table
 *	LOOK_UP - look up to see if a request is waiting in
 *		  in the table for the local user
 *	DELETE - delete invitation
 */

#include "includ.h"
#include <sys/param.h>
#ifdef HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#include <netdb.h>
#include <syslog.h>
#include <stdio.h>
#include <string.h>
#include <utmp.h>
#include <pwd.h>
#include <stdlib.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <fcntl.h>

#include "process.h"
#include "print.h"
#include "table.h"
#include "announce.h"
#include "find_user.h"
#include "defs.h"

int  process_request(register CTL_MSG *mp, register CTL_RESPONSE *rp)
{
	register CTL_MSG *ptr;

	rp->vers = TALK_VERSION;
	rp->type = mp->type;
	rp->id_num = htonl(0);
	if (mp->vers != TALK_VERSION) {
		syslog(LOG_WARNING, "Bad protocol version %d", mp->vers);
		rp->answer = BADVERSION;
		return PROC_REQ_ERR;
	}
	mp->id_num = ntohl(mp->id_num);
	mp->addr.sa_family = ntohs(mp->addr.sa_family);
	if (mp->addr.sa_family != AF_INET) {
		syslog(LOG_WARNING, "Bad address, family %d",
		    mp->addr.sa_family);
		rp->answer = BADADDR;
		return PROC_REQ_ERR;
	}
	mp->ctl_addr.sa_family = ntohs(mp->ctl_addr.sa_family);
	if (mp->ctl_addr.sa_family != AF_INET) {
		syslog(LOG_WARNING, "Bad control address, family %d",
		    mp->ctl_addr.sa_family);
		rp->answer = BADCTLADDR;
		return PROC_REQ_ERR;
	}
	mp->pid = ntohl(mp->pid);
	if (debug_mode)
		print_request("process_request", mp);
	switch (mp->type) {

	case ANNOUNCE:
		return do_announce(mp, rp);

	case LEAVE_INVITE:
		ptr = find_request(mp);
		if (ptr != (CTL_MSG *)0) {
			rp->id_num = htonl(ptr->id_num);
			rp->answer = SUCCESS;
		} else
			insert_table(mp, rp);
		break;

	case LOOK_UP:
		ptr = find_match(mp);
		if (ptr != (CTL_MSG *)0) {
			rp->id_num = htonl(ptr->id_num);
			rp->addr = ptr->addr;
			rp->addr.sa_family = htons(ptr->addr.sa_family);
			rp->answer = SUCCESS;
		} else
			rp->answer = NOT_HERE;
		break;

	case DELETE:
		rp->answer = delete_invite(mp->id_num);
		break;

	default:
		rp->answer = UNKNOWN_REQUEST;
		break;
	}
        return PROC_REQ_OK;
}

int do_announce(register CTL_MSG *mp,CTL_RESPONSE *rp)
{
	struct hostent *hp;
	CTL_MSG *ptr;
	int result;
	char disp[DISPLAYS_LIST_MAX];

        /* see if the user is logged */
        strncpy(callee_name, mp->r_name, NAME_SIZE);
	result = find_user(mp->r_name, mp->r_tty, disp);
        message2("find_user : result = %d",result);
        
	if (result != SUCCESS) {
            message("Couldn t find user ...");
            if (result == NOT_HERE)
            { /* Not here ?  -> Start answering machine ! */
                if (getpwnam(mp->r_name)) /* Does the user exist ? */
                { /* Yes ! -> SUCCESS. */
                    message("Not logged.");
                    rp->answer = SUCCESS;
                    endpwent();
                    return PROC_REQ_ANSWMACH_NOT_LOGGED; /* answer machine. */
                } else 
                { /* Non-existent user ... */
                    endpwent();                    
                    /* output an error into the logs */

                    syslog(LOG_ERR,"User unknown : %s.",mp->r_name);
                    syslog(LOG_ERR,"The caller is : %s.",mp->l_name);

                    switch (OPTNEU_behaviour) {
		    case 2: /* Paranoid setting. Do nothing. */
                        message("Paranoid setting. Do nothing.");
	                rp->answer = NOT_HERE;
			return PROC_REQ_ERR;
		    case 0: /* Launch answering machine. */
                        message("Not here.");
                        rp->answer = SUCCESS;
                        return PROC_REQ_ANSWMACH_NOT_HERE;
		    case 1: /* NEU_user will take the talk. */
                        message("Not here. I ll take the talk.");

                        /* Is he here ? */
                        result = find_user(OPTNEU_user, mp->r_tty, disp);
                        message2("find_user again : result = %d",result);
                        if ((result == NOT_HERE) || (*OPTNEU_user=='\0'))
                        { /* He isn't here => proc_req_answmach */
                            rp->answer = SUCCESS;
                            return PROC_REQ_ANSWMACH_NOT_HERE; /* answer machine. */
                        } else
                        { /* He's here => ok, proceed */
                          strncpy(callee_name, OPTNEU_user, NAME_SIZE);
                        }
                    } /* switch */
                } /* getpwnam */
            } /* result */
            else {
                message("not SUCCESS, nor NOT_HERE");
                rp->answer = result; /* not SUCCESS, nor NOT_HERE*/
                return PROC_REQ_ERR;
            }
	}

#define	satosin(sa)	((struct sockaddr_in *)(sa))
	hp = gethostbyaddr((char *)&satosin(&mp->ctl_addr)->sin_addr,
		sizeof (struct in_addr), AF_INET);
	if (hp == (struct hostent *)0) {
		rp->answer = MACHINE_UNKNOWN;
		return PROC_REQ_ERR;
	}
	ptr = find_request(mp);

	if (ptr == (CTL_MSG *) 0) {
		rp->answer = announce(mp, hp->h_name, disp);
		if (rp->answer != PERMISSION_DENIED) insert_table(mp, rp);
		if (debug_mode) print_response("Announce done", rp);
		return PROC_REQ_OK;
	}

	if ((mp->id_num > ptr->id_num) &&  (mp->id_num != ~0x0L)) {
	  /*
	   * This is an explicit re-announce, so update the id_num
	   * field to avoid duplicates and re-announce the talk.
	   */

            syslog(LOG_WARNING, "reannounce %d", mp->id_num);
            ptr->id_num = new_id();
            rp->id_num = htonl(ptr->id_num);
            rp->answer = announce(mp, hp->h_name, disp);
            return PROC_REQ_ANSWMACH;

        } else {
            /* a duplicated request, so ignore it */
            syslog(LOG_WARNING, "dupannounce %d", mp->id_num);
            rp->id_num = htonl(ptr->id_num);
            rp->answer = SUCCESS;
            return PROC_REQ_OK;
	}
}
