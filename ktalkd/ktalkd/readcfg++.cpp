/* This file is part of ktalkd

    Copyright (C) 1997 David Faure (faure@kde.org)

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
#include <qstring.h>

/* Ktalkd includes */
#include "readconf.h"
#include "defs.h"
#include <config.h>

#include "print.h"

/** Converts a string s into a boolean. Handles 0/1, on/off, true/false. 
 * (case insensitive) */
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

/*  User configuration file, ktalkdrc in localconfigdir(). */
KConfig * cfg = 0;

/*  Initiate user-config-file reading. */
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
      // check if it exists, 'cause otherwise it would be created empty with
      // root as owner !
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
    message("User config file ok");
    return 1;
  } else {
      message("No user config file %s !",(const char*)aFileName);
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

        if (Options::debug_mode) syslog(LOG_DEBUG,"User option %s : %s", key, result);
        return 1;
    }
    else 
    {
        if (Options::debug_mode) syslog(LOG_DEBUG,"User option %s NOT found", key);
        return 0;
    }
}

int read_bool_user_config(char * key, int * result)
{
    char msgtmpl[S_CFGLINE];
    int ret = read_user_config(key, msgtmpl, S_CFGLINE); // ret=1 if found
    if (ret!=0) *result = booleanresult(msgtmpl);
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
    Options::answmach=booleanresult(result); 
    message("AnswMach : %d",Options::answmach);}
  
  if (found("XAnnounce")) {
    Options::XAnnounce=booleanresult(result); 
    message("XAnnounce : %d",Options::XAnnounce); }
  
  if (found("Time")) { 
    Options::time_before_answmach=atoi(result); 
    message("Time : %d",Options::time_before_answmach); }
  
  if (found("Sound")) { 
    Options::sound=booleanresult(result);
    message("Sound : %d",Options::sound); }
  
  if (found("SoundFile")) { 
    qstrncpy(Options::soundfile,result,S_CFGLINE);
    message("SoundFile = %s",Options::soundfile); }
  
  if (found("SoundPlayer")) { 
    qstrncpy(Options::soundplayer,result,S_CFGLINE); 
    message("SoundPlayer = %s",Options::soundplayer); }
  
  if (found("SoundPlayerOpt")) { 
    qstrncpy(Options::soundplayeropt,result,S_CFGLINE);
    message("SoundPlayerOpt = %s",Options::soundplayeropt); }
  
  if (found("MailProg")) { 
    qstrncpy(Options::mailprog,result,S_CFGLINE);
    message("Mail prog = %s",Options::mailprog); }
  
  /* text based announcement */
  if (found("Announce1")) { qstrncpy(Options::announce1,result,S_CFGLINE); }
  if (found("Announce2")) { qstrncpy(Options::announce2,result,S_CFGLINE); }
  if (found("Announce3")) { qstrncpy(Options::announce3,result,S_CFGLINE); }

  if (found("NEUUser"))   { 
      qstrncpy(Options::NEU_user,result,S_INVITE_LINES); 
      message(Options::NEU_user); 
  }
  if (found("NEUBehaviour")) {
      Options::NEU_behaviour=atoi(result); 
      message("NEUBehaviour : %d",Options::NEU_behaviour); 
  }
  
  if (found("ExtPrg")) { 
    qstrncpy(Options::extprg,result,S_CFGLINE);
    message("Ext prg = %s",Options::extprg); }
  else {   /* has to work even without config file at all */
      char buffer [250];
      get_kdebindir(buffer, 250);
      snprintf(Options::extprg,S_CFGLINE,"%s/ktalkdlg",buffer);
  }

  delete syscfg;
  
  return 1;
}


