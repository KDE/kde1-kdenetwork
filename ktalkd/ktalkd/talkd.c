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

#if defined(LIBM_SCCS) && !defined(lint)
char copyright[] =
"@(#) Copyright (c) 1983 Regents of the University of California.\n\
 All rights reserved.\n";
#endif /* not lint */

/*
 * The top level of the daemon, the format is heavily borrowed
 * from rwhod.c. Basically: find out who and where you are; 
 * disconnect all descriptors and ttys, and then endless
 * loop on waiting for and processing requests
 */

#include "includ.h"

#ifdef HAVE_SYS_PARAM_H
#include <sys/param.h>
#endif
#ifndef MAXHOSTNAMELEN
#define MAXHOSTNAMELEN  256             /* max hostname size */
#endif

#include <sys/wait.h>
#include <signal.h>
#include <syslog.h>
#include <time.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "process.h"
#include "print.h"
#include "readconf.h"
#include "defs.h"

#define TIMEOUT 20
 /* TIMEOUT was 30, but has been reduced to remove the
    zombie process (answering machine) as soon as possible */
#define MAXIDLE 120
 /* #define MAXIDLE 30     for debugging purposes */
#define NB_MAX_CHILD 10

int	debug_mode = 0; /* set to one to have verbose reports from ktalkd */

long	lastmsgtime;
char	hostname[MAXHOSTNAMELEN];
int	nb_child_to_wait=0;

void	timeout(int dummy);
/* in answmach.c : */
void launch_ans_mach(CTL_MSG msginfo, int mode);

/* the following copies defaults values to 'OPT*' variables so that
 * configuration file can overwrite them */

int OPTanswmach = 1 ; /* used here */
int OPTtime_before_answmach = 20; /* in answmach/answmach.c */
/* used in announce.c : */
int OPTsound = 0;
int OPTXAnnounce = 1;
char OPTsoundfile [S_CFGLINE];
char OPTsoundplayer [S_CFGLINE];
char OPTsoundplayeropt [S_CFGLINE];
char OPTannounce1 [S_CFGLINE] = ANNOUNCE1;
char OPTannounce2 [S_CFGLINE] = ANNOUNCE2;
char OPTannounce3 [S_CFGLINE] = ANNOUNCE3;
char OPTinvitelines [S_INVITE_LINES] = INVITE_LINES;/* used in answmach/io.c */
char OPTmailprog [S_CFGLINE] = "mail.local"; /* used in answmach/io.c */
int OPTNEU_behaviour = 2; /* default non-existent-user behaviour */
char OPTNEU_user[S_CFGLINE];
char OPTNEUBanner1 [S_CFGLINE] = NEU_BANNER1;
char OPTNEUBanner2 [S_CFGLINE] = NEU_BANNER2;
char OPTNEUBanner3 [S_CFGLINE] = NEU_BANNER3;
char OPTextprg [S_CFGLINE];
char		OPTNEU_set_user_name [S_CFGLINE] = "";
char		callee_name[NAME_SIZE];

extern int errno;

int main(int argc, char *argv[])
{
        CTL_MSG		request;
        CTL_RESPONSE	response;
        CTL_MSG *mp = &request;
        int  ret_value = PROC_REQ_OK; /* return value from process_request */
	int cc;
        int talkd_sockt = 0;

	if (getuid()) {
		fprintf(stderr, "%s: getuid: not super-user", argv[0]);
		exit(1);
	}
#ifdef LOG_PERROR
	openlog("ktalkd", LOG_PID || LOG_PERROR, LOG_DAEMON);
#else
	openlog("ktalkd", LOG_PID, LOG_DAEMON);
#endif
	if (gethostname(hostname, sizeof (hostname) - 1) < 0) {
		syslog(LOG_ERR, "gethostname: %m");
		_exit(1);
	}
	if (chdir(_PATH_DEV) < 0) {
		syslog(LOG_ERR, "chdir: %s: %m", _PATH_DEV);
		_exit(1);
	}
	if (argc > 1 && strcmp(argv[1], "-d") == 0)
		debug_mode = 1;

        signal(SIGALRM, timeout);
        alarm(TIMEOUT);

	process_config_file(); /* read configuration */

	for (;;) {

                if (OPTanswmach && (ret_value>=PROC_REQ_MIN_A) && 
                     (ret_value<=PROC_REQ_MAX_A)
                     && (nb_child_to_wait<NB_MAX_CHILD))
                {
                    message("Launch answer machine.");
                    launch_ans_mach(request, ret_value);
                    nb_child_to_wait++;
                }
                
                cc = recv(talkd_sockt, (char *)mp, sizeof (*mp), 0);
		if (cc != sizeof (*mp)) {
			if (cc < 0 && errno != EINTR)
				syslog(LOG_WARNING, "recv: %m");
			continue;
		}
		lastmsgtime = time(0);
		ret_value = process_request(mp, &response);
                if (debug_mode) print_response("=> response", &response);
		/* can block here, is this what I want? */
		cc = sendto(talkd_sockt, (char *)&response,
		    sizeof (response), 0, (struct sockaddr *)&mp->ctl_addr,
		    sizeof (mp->ctl_addr));
		if (cc != sizeof (response))
			syslog(LOG_WARNING, "sendto: %m");
        }
}

void timeout(int dummy)
{
    (void)dummy; // to avoid warning
    if (nb_child_to_wait>0)
    {
        int pid;
        pid = waitpid(-1,0,WNOHANG);
        if (pid==-1)
            syslog(LOG_ERR,"Timeout. Error waiting for answering machine.");
        if ((pid!=0) & (nb_child_to_wait>0)) nb_child_to_wait--;
    }
    else if (time(0) - lastmsgtime >= MAXIDLE)
        _exit(0);
    alarm(TIMEOUT);
}
