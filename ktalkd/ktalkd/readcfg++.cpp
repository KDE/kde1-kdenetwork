/* This file is part of ktalkd

    Copyright (C) 1997 David Faure (david.faure@ifhamy.insa-lyon.fr)

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
    Boston, MA 02111-1307, USA. */

/*
 * Routines for reading configuration from KDE configuration
 *  for ktalkd.
 *
 * by David Faure <David.Faure@ifhamy.insa-lyon.fr>
 */

/* Unix includes */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <syslog.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <pwd.h>

/* KDE & Qt includes */
#include <kapp.h>
#include <qlabel.h>
#include <qstring.h>

/* Ktalkd includes */
#include "readcfg++.h"
#include "defs.h"
#include <config.h>

/* C interface */
extern "C" {
#include "print.h"
extern int debug_mode;
}

int booleanresult(const char * s)
{
     if (strlen(s)==1)
	  { return atoi(s); }
     else if ((!strncasecmp(s,"on",2))||(!strncasecmp(s,"true",4))) {return 1;}
     else if ((!strncasecmp(s,"off",3))||(!strncasecmp(s,"false",5))){return 0;}
     else {
	  syslog(LOG_ERR,"Wrong boolean value %s in ktalkdrc",s);
	  return 0;
     }
}

/*  User configuration file, ktalkdrc in localconfigdir().

It can contain one or more of : Mail, Subj, Head, Msg1..Msg3, Sound, SoundFile, Answmach
In the future, it could contain also : XAnnounce, Time...

Fisrt implementation : used only within io.c => ok for mail, subj, head, msg1..3
Second : added Sound*, then Answmach, open/close moved to answmach.c */

KConfig * cfg = 0;

/* 
 *   Initiate user-config-file reading.
 */
int init_user_config(char * l_name)
{
  struct passwd * pw = getpwnam(l_name);
  struct stat buf;
  QString aFileName;

  if (!pw) return 0;
  else {
#ifdef HAVE_FUNC_SETENV
    setenv("HOME",pw->pw_dir,1 /* overwrite */); 
/* Set $HOME, because localconfigdir() calls QDir::homeDirPath() */
#else
    QString envvar = QString("HOME=")+pw->pw_dir;
    putenv(envvar);
#endif
    aFileName = KApplication::localconfigdir()+QString ("/ktalkdrc");
    endpwent();
  }
  if (stat(aFileName,&buf)!=-1)
 // check if it exists, otherwise it will be created with root as owner !
  {
    cfg = new KConfig( aFileName );

  // debug code, to print locale found :
#if 0
  const char *g_lang = getenv("LANG");
  char syscmd[200];
  if (!g_lang)
    sprintf ( syscmd, "echo LANG : not set... >/tmp/readcfg++_debug");
  else
    sprintf ( syscmd, "echo LANG : %s >/tmp/readcfg++_debug", g_lang);
  system(syscmd);
  sprintf ( syscmd, "echo language : %s >>/tmp/readcfg++_debug",
                  (const char *) kapp->getLocale()->language() );
  system(syscmd);
#endif 
    cfg -> setGroup("ktalkd");
    if (debug_mode) syslog(LOG_DEBUG,"User config file ok");
    return 1;
  } else {
      if (debug_mode) syslog(LOG_DEBUG,"No user config file %s !",(const char*)aFileName);
      return 0;
    }
}

/*
 * Read one entry in user-config-file
 */

int read_user_config(char * key, char * result, int max)
{
    if (!cfg) syslog(LOG_ERR,"PROGRAM ERROR, init_user_config NOT CALLED");

    QString Qresult;
    if (!(Qresult = cfg -> readEntry(key)).isEmpty())
    {
        qstrncpy( result, Qresult, max);

        if (debug_mode) syslog(LOG_DEBUG,"User option %s : %s", key, result);
        return 1;
    }
    else 
    {
        if (debug_mode) syslog(LOG_DEBUG,"User option %s NOT found", key);
        return 0;
    }
}

int read_bool_user_config(char * key, int * result)
{
    char msgtmpl[S_CFGLINE];
    int ret = read_user_config(key, msgtmpl, S_CFGLINE);
    
    if (ret!=0) ret = booleanresult( msgtmpl );
    
    if (result!=NULL) *result = ret;
    
    
    return ret;
}

// Close user-config-file and destroys objects used.

void end_user_config()
{
  delete cfg;
  cfg = 0;
}

// set KDEBINDIR
void setenv_kdebindir(void)
{
#ifdef HAVE_FUNC_SETENV
  setenv("KDEBINDIR", KApplication::kde_bindir (), 0/*don't overwrite*/);
#else
  QString env = QString("KDEBINDIR=") + KApplication::kde_bindir ();
  putenv(env);
#endif
}

// get KDE bin dir
void get_kdebindir(char * buffer, int max)
{
  qstrncpy( buffer, KApplication::kde_bindir(), max);
}

// System configuration file

int process_config_file(void)
{ 
  setenv_kdebindir(); // Has to be done, for any $KDEBINDIR in ktalkdrc.

  QString aFileName = KApplication::kde_configdir()+QString ("/ktalkdrc");
  KConfig * syscfg = new KConfig( aFileName );

  syscfg -> setGroup("ktalkd");

  QString result;
    
#define found(k) (!(result = syscfg -> readEntry(k)).isEmpty())
  //    QString cfgStr = cfgStr0.stripWhiteSpace();
  
  if (found("AnswMach")) {
    OPTanswmach=booleanresult(result); 
    message2("AnswMach : %d",OPTanswmach);}
  
  if (found("XAnnounce")) {
    OPTXAnnounce=booleanresult(result); 
    message2("XAnnounce : %d",OPTXAnnounce); }
  
  if (found("Time")) { 
    OPTtime_before_answmach=atoi(result); 
    message2("Time : %d",OPTtime_before_answmach); }
  
  if (found("Sound")) { 
    OPTsound=booleanresult(result);
    message2("Sound : %d",OPTsound); }
  
  if (found("SoundFile")) { 
    qstrncpy(OPTsoundfile,result,S_CFGLINE);
    message_s("SoundFile = %s",OPTsoundfile); }
  
  if (found("SoundPlayer")) { 
    qstrncpy(OPTsoundplayer,result,S_CFGLINE); 
    message_s("SoundPlayer = %s",OPTsoundplayer); }
  
  if (found("SoundPlayerOpt")) { 
    qstrncpy(OPTsoundplayeropt,result,S_CFGLINE);
    message_s("SoundPlayerOpt = %s",OPTsoundplayeropt); }
  
  if (found("MailProg")) { 
    qstrncpy(OPTmailprog,result,S_CFGLINE);
    message_s("Mail prog = %s",OPTmailprog); }
  
  /* text based announcement */
  if (found("Announce1")) { qstrncpy(OPTannounce1,result,S_CFGLINE); }
  if (found("Announce2")) { qstrncpy(OPTannounce2,result,S_CFGLINE); }
  if (found("Announce3")) { qstrncpy(OPTannounce3,result,S_CFGLINE); }

  if (found("NEUUser"))   { 
      qstrncpy(OPTNEU_user,result,S_INVITE_LINES); 
      message(OPTNEU_user); 
  }
  if (found("NEUBehaviour")) {
      OPTNEU_behaviour=atoi(result); 
      message2("NEUBehaviour : %d",OPTNEU_behaviour); 
  }
  
  if (found("NEUSetUserName"))
  {
    qstrncpy(OPTNEU_set_user_name,result,S_CFGLINE);
    message_s("NEUSetUserName: %s", OPTNEU_set_user_name);
  }
  
  if (found("NEUBanner1")) { qstrncpy(OPTNEUBanner1,result,S_CFGLINE); }
  if (found("NEUBanner2")) { qstrncpy(OPTNEUBanner2,result,S_CFGLINE); }
  if (found("NEUBanner3")) { qstrncpy(OPTNEUBanner3,result,S_CFGLINE); }

  if (found("ExtPrg")) { 
    qstrncpy(OPTextprg,result,S_CFGLINE);
    message_s("Ext prg = %s",OPTextprg); }
  else {   /* has to work even without config file at all */
      char buffer [250];
      get_kdebindir(buffer, 250);
      snprintf(OPTextprg,S_CFGLINE,"%s/ktalkdlg",buffer);
  }

  if ( OPTNEU_set_user_name[0]=='\000' && OPTNEU_behaviour==1 )
    OPTNEU_behaviour = 0; // OPTNEU_set_user_name has to be set !

  delete syscfg;
  
  return 1;
}


