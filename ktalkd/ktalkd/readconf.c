/*
 *              readconf.c
 *
 * Routines for reading talkd.conf and .talkdrc
 *
 * by David Faure <David.Faure@ifhamy.insa-lyon.fr
 */

#include <pwd.h>
#include <sys/types.h> 
#include <fcntl.h>
#include <syslog.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include "print.h"
#include "readconf.h"
#include "defs.h"

FILE * fd;

int booleanresult(char * s)
{
     if (strlen(s)==1)
	  { return atoi(s); }
     else if ((!strncasecmp(s,"on",2))||(!strncasecmp(s,"true",4))) {return 1;}
     else if ((!strncasecmp(s,"off",3))||(!strncasecmp(s,"false",5))){return 0;}
     else {
	  syslog(LOG_ERR,"Wrong boolean value %s in %s",s,TALKD_CONF);
	  return 0;
     }
}

/* obsolete routine, but still used for .talkdrc */
int read_user_config(char * key, char * result, int max)
{
     char * value;
     char * buff = (char *) malloc (max);
     char * ret;
     fseek(fd,0,SEEK_SET);
     do {
	  ret = fgets(buff,max,fd);
     } while ((ret) && (strncasecmp(buff,key,strlen(key))));
     if (ret) {
	  value = strchr(buff,':')+1;
	  while (isspace(*value)) value++; /* get rid of spaces, tabs... */
	  strncpy(result,value,max);
	  result[strlen(result)-1]='\0'; /* get rid of \n */
     }
     free(buff);
     return (ret) ? 1 : 0;
}

int read_bool_user_config(char * key, int * result)
{
    char msgtmpl[S_CFGLINE];
    int ret = read_user_config(key, msgtmpl, S_CFGLINE);
    if (ret) *result = booleanresult(msgtmpl);
    return ret;
}

int init_user_config(char * l_name)
{
#define S_MSGPATH 50
     char msgpath[S_MSGPATH];
     struct passwd * pw = getpwnam(l_name);
     if (!pw) return 0;
     else {
       snprintf(msgpath, S_MSGPATH, "%s/.talkdrc", pw->pw_dir);
       endpwent();
       fd=fopen(msgpath, "r");
       return (fd != 0);
     }
}

void end_user_config()
{
  fclose(fd);
}

void setenv_kdebindir(void)
{ // nothing to do, not in KDE.
}

void get_kdebindir(char * buffer, int max)
{ // nothing at all, never used outside KDE.
}

/* routine for reading talkd.conf */

int process_config_file(void)
{
     FILE * fd = fopen(TALKD_CONF,"r");
     char * ret;
     char buff[S_CFGLINE];
     char * result;

#define found(k) (!strncasecmp(buff,k,strlen(k)))

     if (!fd) { free(buff); return 0; }
     do {
	  ret = fgets(buff,S_CFGLINE,fd);
	  if ((ret) && (*buff!='#') && ((result = strchr(buff,':')))) {
	       result++;
	       while (isspace(*result)) 
		    result++; /* get rid of spaces, tabs... */
	       result[strlen(result)-1]='\0'; /* get rid of \n */
		 
	       if (found("AnswMach:")) {
		    OPTanswmach=booleanresult(result); 
		    message2("AnswMach : %d",OPTanswmach);}
		 
	       if (found("XAnnounce:")) {
		    OPTXAnnounce=booleanresult(result); 
		    message2("XAnnounce : %d",OPTXAnnounce); }
	    
	       if (found("Time:")) { 
		    OPTtime_before_answmach=atoi(result); 
		    message2("Time : %d",OPTtime_before_answmach); }

	       if (found("Sound:")) { 
		    OPTsound=booleanresult(result);
		    message2("Sound : %d",OPTsound); }

	       if (found("SoundFile:")) { 
		    strncpy(OPTsoundfile,result,S_CFGLINE);
		    message("SoundFile =");message(OPTsoundfile); }

	       if (found("SoundPlayer:")) { 
                   strncpy(OPTsoundplayer,result,S_CFGLINE);
		    message("SoundPlayer ="); message(result); }

	       if (found("SoundPlayerOpt:")) { 
                    strncpy(OPTsoundplayeropt,result,S_CFGLINE); 
		    message("SoundPlayerOpt ="); message(result); }

	       if (found("MailProg:")) { 
		    strncpy(OPTmailprog,result,S_CFGLINE);
		    message("Mail prog ="); message(result); }

               /* text based announcement */
               if (found("Announce1")) { strncpy(OPTannounce1,result,S_CFGLINE); }
               if (found("Announce2")) { strncpy(OPTannounce2,result,S_CFGLINE); }
               if (found("Announce3")) { strncpy(OPTannounce3,result,S_CFGLINE); }
               
               if (found("NEUUser"))   { strncpy(OPTNEU_user,result,S_INVITE_LINES); }
               if (found("NEUBehaviour")) { OPTNEU_behaviour=booleanresult(result); }
               if (found("NEUBanner1")) { strncpy(OPTNEUBanner1,result,S_CFGLINE); }
               if (found("NEUBanner2")) { strncpy(OPTNEUBanner2,result,S_CFGLINE); }
               if (found("NEUBanner3")) { strncpy(OPTNEUBanner3,result,S_CFGLINE); }
	  }
     } while (ret);
     fclose(fd);
     return 1; 
}
