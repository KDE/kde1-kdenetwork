/*
 * Copyright (c) 1983 Regents of the University of California, (c) 1997 David Faure.
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

/* From: @(#)io.c	5.6 (Berkeley) 3/1/91 */

/*
 * This file contains the I/O handling and the exchange of 
 * edit characters. This connection itself is established in
 * ctl.c
 */

#include "talk.h"
#include <pwd.h>
#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#ifdef TIME_WITH_SYS_TIME
#include <time.h>
#endif
#else
#include <time.h>
#endif
#include <sys/ioctl.h>
#ifdef HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <syslog.h>
#include <netdb.h>
#include "talk_ctl.h"
#include "../defs.h"  /* for S_* */
#include "../print.h"
#include "../readconf.h"

#define A_LONG_TIME 10000000  /* seconds before timeout */

char char_erase; /* set by init_disp.c, in set_edit_chars */

extern int errno;

static void write_banner(char * banner);
static int read_message(FILE * fd);   // message to mail
static void write_headers(FILE * fd, struct hostent * hp, char *
                          messg_myaddr, char * NEUperson, int mode, int usercfg);  // mail headers

void talk(int mode, int usercfg, char* NEUperson/* set by answmach.c,
                                                   if NEU */ )
{
     char command[S_COMMAND];
     char messg_myaddr [S_MESSG];
     struct hostent *hp;
     FILE * fd = 0;      /* file descriptor, to write the message */
     char customline[S_CFGLINE];
     
#ifndef OLD_POPEN_METHOD   // never defined
     char fname[256];
     int fildes;
     int oldumask = umask(066);
       /* set permissions for temp file to rw- --- --- */
     int emptymail; /* 1 if empty mail allowed */
#endif
     int something_entered;
     
     hp = gethostbyaddr((char *)&his_machine_addr, sizeof (struct in_addr), AF_INET);
     if (hp == (struct hostent *)0) 
	  p_error("Answering machine : Remote machine unknown.");

#ifdef USER_SETS_EMAIL
     if ((!usercfg) || (!read_user_config("Mail",messg_myaddr,S_MESSG-1)))
#endif
       strcpy(messg_myaddr,callee_name);

#ifdef OLD_POPEN_METHOD   // never defined
     snprintf(command,S_COMMAND,"%s %s",OPTmailprog,messg_myaddr);

    fd = popen(command,"w");
    if (!fd) 
      {
	snprintf(customline,S_CFGLINE,"Unable to open a pipe towards %s.",OPTmailprog);
	p_error(customline);
      }
#else

    sprintf(fname, _PATH_TMP"ktalkdXXXXXX");
    if ((fildes = mkstemp(fname)) == -1 || (fd = fdopen(fildes, "w+")) == 0) {
        p_error("Unable to open temporary file");
    }

    umask(oldumask);
    
    message(fname); // debugging purpose
    
#endif
    
    write_headers(fd, hp, messg_myaddr, NEUperson, mode, usercfg);

    /* read other options before setting usercfg to 0, below. */
    if ((!usercfg) || (!read_bool_user_config("EmptyMail",&emptymail)))
      /* try from user config file, otherwise default : */
      emptymail = 1;

    message2("usercfg : %d",usercfg);
    message2("EmptyMail : %d",emptymail);
    
    message("Connection established");

    if (usercfg) {
      if (!read_user_config("Msg1",customline,S_CFGLINE-1)) 
	{ message("Error reading Msg1"); end_user_config(); usercfg=0; }
      else { message(customline); strcat(customline,"\n"); }
    }

    /* No user-config'ed banner */
    if (!usercfg)
    { /* => Display OPTinvitelines* */
         write_banner(OPTinvitelines); write(sockt,"\n",1);
    }
    else if (mode==PROC_REQ_ANSWMACH_NOT_HERE)
    { /* => Display OPTNEUBanner* */
         write_banner(OPTNEUBanner1); write(sockt,"\n",1);
         write_banner(OPTNEUBanner2); write(sockt,"\n",1);
         write_banner(OPTNEUBanner3); write(sockt,"\n",1);
    } else {
	 int linenr = 1; 
	 /* number of the Msg[1-*] line. is set to 0 after displaying banner*/
	 char m[]="Msg1"; /* used as key to read configuration */
	 
	 while (linenr) /* still something to write to the caller */
	      {
   		  write_banner(customline);
                   
		   /* read next line in custom file. */
		   m[3]=(++linenr)+'0';
		   if (!read_user_config(m,customline,S_CFGLINE-1))
			linenr=0; /* end of message */			
		   else
			strcat(customline,"\n");
	      }
    }
    /* Banner displayed. Let's take the message. */
    something_entered = read_message(fd);
#ifdef OLD_POPEN_METHOD   // never defined
    pclose(fd);
#else
    fclose(fd);

    if (something_entered || emptymail)
    { /* Don't send empty message, except if 'EmptyMail' has been set */
        int retcode;
	snprintf(command,S_COMMAND,"cat %s | %s %s",fname,OPTmailprog,messg_myaddr);
        retcode = system(command);
        if ((retcode==127) || (retcode==-1))
          syslog(LOG_ERR,"system() error : %m");
        else if (retcode!=0)
          syslog(LOG_WARNING,"cat %s | %s %s : %m", fname, OPTmailprog, messg_myaddr);
    }
    (void)unlink(fname);
#endif
}

/*
 * p_error prints the system error message in the log
 * and then exits. 
 */
void p_error(const char *string) 
{
    syslog(LOG_ERR,string);
    quit();
}

static void write_headers(FILE * fd, struct hostent * hp, char *
                          messg_myaddr, char * NEUperson, int mode, int usercfg)
{
    int ismaillocal; /* true if mail.local is used */
    char messg [S_MESSG];
    char messg_tmpl [S_MESSG];

    /* if using mail.local, set 'Date:' and 'From:', because they will be missing otherwise */
    ismaillocal = (strstr(OPTmailprog,"mail.local")!=NULL);
    if (ismaillocal)
            /* should we check only the end of the name ? */
      {
	time_t tmp = time(0);
	snprintf(messg,S_MESSG,"Date: %s",ctime(&tmp)); /* \n is included in ctime */
	fwrite(messg,strlen(messg),1,fd); /* Date */

	snprintf(messg,S_MESSG,"From: %s@%s\n",msg.r_name,hp->h_name);
	fwrite(messg,strlen(messg),1,fd); /* From */
      }
	
    snprintf(messg,S_MESSG,"To: %s\n",messg_myaddr);
    fwrite(messg,strlen(messg),1,fd); /* To */

    if ((!usercfg) || (!read_user_config("Subj",messg_tmpl,S_CFGLINE)))
      /* try from user config file, otherwise default subject: */
      strcpy(messg_tmpl,"Message from %s");
    snprintf(messg,S_MESSG,messg_tmpl,msg.r_name);
    fwrite("Subject: ",9,1,fd);
    fwrite(messg,strlen(messg),1,fd); /* Subject */
    fwrite("\n",1,1,fd);

    if (!ismaillocal) 
      {  /* No need to set Reply-To if From has been set correctly */
	snprintf(messg,S_MESSG,"Reply-To: %s@%s\n",msg.r_name,hp->h_name);
	fwrite(messg,strlen(messg),1,fd); /* Reply-To */
      }

    fwrite("\n",1,1,fd); /* empty line -> end of headers */

    if ((!usercfg) || (!read_user_config("Head",messg_tmpl,S_CFGLINE)))
      /* try from user config file, otherwise default headline: */
      strcpy(messg_tmpl,"Message left in the answering machine, by %s@%s");
    snprintf(messg,S_MESSG,messg_tmpl,msg.r_name,hp->h_name);
    
    
    if (mode==PROC_REQ_ANSWMACH_NOT_HERE) 
    {
        strcat(messg," => '");
        strcat(messg,NEUperson);
    }
    else if ( strcmp(msg.l_name,callee_name)!=0 )
    {
      strcat(messg," => '");
      strcat(messg,msg.l_name);
      strcat(messg,"'");
    }
    fwrite(messg,strlen(messg),1,fd); /* First line of the message */
    fwrite("\n\n",2,1,fd);
}

static void write_banner(char * banner)
{ /* writes the message 'banner', null-terminated */
    int count = strlen(banner);
    int nbsent;
    char * str = banner;
    message("write_banner");
    /*    message2("Count : %d.",count); */
    while (count>0) {
        /* let's send 16 -bytes-max packets */
         if (count>=16) nbsent = write(sockt,str,16);
                  else nbsent = write(sockt,str,count);
        count -= nbsent;
        str += nbsent;
        /*        message2("Count rest: %d.",count);
                  message2("  [ Nbsent : %d.",nbsent); */
        /* sleep(1); ONLY to debug !! */
        fsync(sockt);
    }
}
 
static int read_message(FILE * fd) // returns 1 if something has been entered
{
     register int read_template, sockt_mask;
     int read_set, nb;
     int pos = 0;       // position on the line. left=0.
     int something = 0; // nothing entered by caller up to now.
     struct timeval wait;
     char buff[BUFSIZ];
     char line[80] = ""; // buffer for current line

     sockt_mask = (1<<sockt);
     read_template = sockt_mask;

     for (;;) {
	  read_set = read_template;
	  wait.tv_sec = A_LONG_TIME;
	  wait.tv_usec = 0;

	  nb = select(32, (fd_set *) &read_set, 0, 0, &wait);
	  if (nb <= 0) {
	       if (errno == EINTR) {
		    read_set = read_template;
		    continue;
	       } /* panic, we don't know what happened */
	       p_error("Unexpected error from select");
	  }
	  if (read_set & sockt_mask) { 
	       int i;
	       /* There is data on sockt */
	       nb = read(sockt, buff, sizeof buff);
	       if (nb <= 0) {
		    message("Connection closed. Exiting");
		    break;
	       }
               something = 1;
	       for (i=0; i<nb; i++ ) {
		    if ((buff[i]==char_erase) && (pos>0)) /* backspace */
			 pos--;
		    else {
			 if (pos == 79) {
			      fwrite(line,pos,1,fd);
			      pos = 0;
			 }
			 line[pos++]=buff[i];
			 if (buff[i]=='\n') {
			      fwrite(line,pos,1,fd);
			      pos = 0;
			 }
		    }
	       }
	  } /* if read_set ... */
     }
     if (pos>0) { line[pos++]='\n'; fwrite(line,pos,1,fd); }
                                    /* last line */
     return something; // 1 if something entered.
}
