/*
 * Copyright (c) 1983  Regents of the University of California, (c) 1997 David Faure
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
 *
 */

#include "talk.h"
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include "talk_ctl.h"
#include "../defs.h"
#include "../readconf.h"

/*
 *   Launch (by forking) the answering machine ....
 */

void launch_ans_mach(CTL_MSG msginfo, int mode)
{
  
  if ((fork()) == 0) /* let's fork to let the daemon process other messages */
    {
      char buff[50];
      struct servent *sp;
      struct hostent *hp;
      int usercfg;    /* set if a user config file exists */
      int LaunchIt=1;   /* Does the local user want the answering machine to
                         be launched ? */

      char NEUperson[NAME_SIZE]; /* the inexistent user, for log purposes */

      CTL_MSG mpcopy;
      CTL_MSG * mp = &mpcopy;

      struct sockaddr_in sin;

      memcpy(&mpcopy, &msginfo, sizeof(CTL_MSG));

#ifdef FvK
      /* I'm not responsible for this patch. I include it because some people
         asked me to. D.F. */
      /* <FvK> patch : (used to find the correct interface, when other than eth0). */
      struct sockaddr_in foo;
      int sock, i;
      /* If socket fails, code will see it. */
      sock = socket(AF_INET, SOCK_DGRAM, 0);
      /* </FvK> */
#endif

#define	satosin(sa)	((struct sockaddr_in *)(sa))
      his_machine_addr = (satosin(&mp->ctl_addr))->sin_addr;

      /* look up the address of the local host */
      hp = gethostbyname(hostname);
      if (!hp) {
	   syslog(LOG_ERR, "GetHostByName failed.");
	   exit(-1);
      }
      memcpy(&my_machine_addr, hp->h_addr, hp->h_length);

      /* find the server's port */
      sp = getservbyname("ntalk", "udp");
      if (sp == 0) {
	   sprintf(buff, "talk: %s/%s: service is not registered.\n",
		         "ntalk", "udp");
	   p_error(buff); /* quit */
      }
      daemon_port = sp->s_port;
      sin.sin_family = AF_INET;
      sin.sin_addr.s_addr = his_machine_addr.s_addr;
      sin.sin_port = sp->s_port;

#ifdef FvK
      /* <FvK> Now here is the trick.  We connect to the other side. */
      if ((sock >= 0) &&
	  (connect(sock, (struct sockaddr *) &sin, sizeof(sin)) == 0)) {
	   /* Bingo.  Now fetch the address. */
	   foo = sin;
	   i = sizeof(foo);
	   if (getsockname(sock, (struct sockaddr *) &foo, &i) == 0) {
		my_machine_addr = foo.sin_addr;
	   }
      }
      /* Loose the socket. */
      close(sock);
      /* </FvK> */
#endif

      /* in a normal talk client, this is done at the end of get_names : */

      /* memcpy mp to msg, in order to copy
         'vers','addr.port, ctl_addr.port','ctl_addr.sa_family' */
      memcpy(&msg,mp,sizeof(CTL_MSG));
      msg.id_num = htonl(0);

      /* change family byte order */
      msg.addr.sa_family = htons(mp->addr.sa_family);

      /* Only wait if somebody could possibly answer.
         If NEU/not logged, start at once. */
      if (mode==PROC_REQ_ANSWMACH) {
          /* Wait a little. The 'ringing your party again' has been displayed,
             but it's probably still a bit early to launch the answering machine. */
              sleep(OPTtime_before_answmach);
      }
      
      strncpy(msg.l_name,mp->r_name,NAME_SIZE); /* exchange names */
      strncpy(msg.r_name,mp->l_name,NAME_SIZE);

      if (mode==PROC_REQ_ANSWMACH_NOT_HERE)
          usercfg = init_user_config(OPTNEU_user);
      else
          usercfg = init_user_config(msg.l_name);

      if (usercfg)
          if (!read_bool_user_config("Answmach",&LaunchIt))
              LaunchIt=1;

      if (LaunchIt)
      {
          open_ctl();
          open_sockt();
          if (check_local())  /* otherwise, either the caller gave up before we 
                                 started or the callee answered ... */
          {
              /* init_disp.c : send the first 3 chars, machine dependent */
              set_edit_chars();
              
              if (mode==PROC_REQ_ANSWMACH_NOT_HERE)
              {
              /* The caller is trying to talk to somebody this system doesn't know.
                 We can display a NEU banner (non-existent user) and take a message
                 for OPTNEU_user (root?). */                  
                  strncpy(NEUperson,msg.l_name,NAME_SIZE); /* keep it for log purposes */
                  strncpy(msg.l_name,OPTNEU_user,NAME_SIZE); /* for mail address, config file... */
              } else *NEUperson='\0';
              /* Do the talking */
              talk(mode, usercfg, NEUperson);
          }
      } else message("Not launched. Option 'answmach' set to 0.");
      if (usercfg) end_user_config();
      
      exit(-1);
    }
}
