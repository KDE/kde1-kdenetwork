/*
 *            kPPP: A pppd front end for the KDE project
 *
 * $Id$
 * 
 *            Copyright (C) 1997 Bernd Johannes Wuebben 
 *                   wuebben@math.cornell.edu
 * 
 * This file was contributed by Mario Weilguni <mweilguni@sime.com>
 * Thanks Mario !
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this program; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <qdir.h>
#include "runtests.h"
#include <kapp.h>
#include <kmsgbox.h>
#include <unistd.h>
#include <qmsgbox.h>
#include <qregexp.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pwd.h>

QString findFileInPath( const char *fname, const char *extraPath) {
  QString f;  

  if(access(fname, F_OK) == 0)
    return QString(fname);

  // strip arguments
  QString _fname = fname;
  if(_fname.find(' ') != -1)
    _fname = _fname.left(_fname.find(' '));

  char path[4096] = "";
  if(extraPath != 0)
    strcpy(path, extraPath);

  // for absolute path
  strcat(path, ":");

  if(getenv("PATH") != NULL)
    strncat(path, getenv("PATH"), sizeof(path)-512);

  char *p = strtok(path, ":");
  while(p != NULL) {
    f = p;
    f += "/";
    f += _fname;
    if(access(f.data(), F_OK) == 0) {
      return f;
    } else
      p = strtok(NULL, ":");
  }

  f = "";
  return f;
}

int uidFromName(const char *uname) {
  struct passwd *pw;

  setpwent();
  while((pw = getpwent()) != NULL) {
    if(strcmp(uname, pw->pw_name) == 0) {
      int uid = pw->pw_uid;
      endpwent();
      return uid;
    }
  }

  endpwent();
  return -1;
}

int runTests() {
  int warning = 0;

  // Test pre-1: check if the user is allowed to dial-out
  if(access("/etc/kppp.allow", R_OK) == 0 && getuid() != 0) {
    bool access = FALSE;
    FILE *f;
    if((f = fopen("/etc/kppp.allow", "r")) != NULL) {
      char buf[2048];
      while(!feof(f)) {
	if(fgets(buf, sizeof(buf), f) != NULL) {
	  QString s(buf, sizeof(buf));
	  
	  s = s.stripWhiteSpace();
	  if(s[0] == '#' || s.length() == 0)
	    continue;

	  if(uidFromName(s.data()) == getuid()) {
	    access = TRUE;
	    fclose(f);
	  }
	}
      }
      fclose(f);
    }

    if(!access) {
      QMessageBox::warning(0, 
		 klocale->translate("Error"),
		 klocale->translate("You´re not allowed to dial out with "
				    "kppp.\nContact your system administrator."
				    ));
      exit(1);
    }
  }
  
  // Test 1: search the pppd binary
  QString f = gpppdata.pppdPath();
  bool pppdFound = FALSE;
  if(access(f.data(), F_OK) == 0)
    pppdFound = TRUE;

  if(!pppdFound) {
    QMessageBox::warning(0, 
		 klocale->translate("Error"),
		 klocale->translate("Cannot find the pppd-daemon!\n\n"
				    "Make sure that pppd is installed and\n"
				    "you have entered the correct path.\n"
				    ));
    warning++;
  }
    
  // Test 2: check access to the pppd binary
  if(pppdFound) {
    if(access(f.data(), X_OK) != 0 && geteuid() != 0) {
      QMessageBox::critical(0, 
		   klocale->translate("Error"),
		   klocale->translate("You do not have the permission\n"
				      "to start pppd!\n\n"
				      "Contact your system administrator\n"
				      "and ask to get access to pppd."));
      return TEST_CRITICAL;
    } else {
      struct stat st;
      stat(f.data(), &st);
      if(st.st_mode & S_ISUID == 0 && getuid() != 0) {
	QMessageBox::warning(0, 
		     klocale->translate("Error"),
		     klocale->translate("pppd is not properly installed!\n\n"
					"The pppd binary must be installed\n"
					"with the SUID bit set. Contact your\n"
					"system administrator."));
	warning++;
      }
    }

    if(geteuid() == 0) {
      struct stat st;
      stat(f.data(), &st);

      if(st.st_uid != 0) { // pppd not owned by root
	QMessageBox::warning(0, 
		     klocale->translate("Error"),
		     klocale->translate("pppd is not properly installed!\n\n"
					"The pppd binary must be installed\n"
					"with the SUID bit set. Contact your\n"
					"system administrator."));
	return TEST_CRITICAL;
      }
    }
  }

  // Test 3: search the logviewer
  if(strlen(gpppdata.logViewer()) > 0) {
    f = findFileInPath( gpppdata.logViewer() );
    if( f.length() == 0 || access(f.data(), X_OK) != 0 ) {
      QString s;
      QString dflt;

      // search default
      dflt = findFileInPath( "kedit" );
      if( dflt.length() == 0 || access(dflt.data(), X_OK) != 0 )
	dflt = "kvt -e less";

      s.sprintf(klocale->translate("The logviewer \"%s\" was not found!\n\n"
				   "Setting default back to \"%s\"."),
		gpppdata.logViewer(), dflt.data());
      QMessageBox::information(0,
			       klocale->translate("Information"),
			       s.data());
      gpppdata.setlogViewer(dflt.data());
      gpppdata.save();
    }
  }

  // Test 4: check for undesired 'lock' option in /etc/ppp/options
  int fd;
  if ((fd = open(SYSOPTIONS, O_RDONLY)) >= 0) {
    QString str;
    char c;
    while (str.length() < 50 && read(fd, &c, 1) == 1) 
      str+=c;
    QRegExp r1("^lock");
    QRegExp r2("\\slock$");   // \s matches white space (9,10,11,12,13,32) 
    QRegExp r3("\\slock\\s");
    if (r1.match(str) >= 0 || r2.match(str) >= 0 || r3.match(str) >= 0) {
      QMessageBox::warning(0,
		   klocale->translate("Error"),
		   klocale->translate("kppp has detected a 'lock' option in "
				      "/etc/ppp/options.\n\nThis option has "
				      "to be removed since kppp takes care "
				      "of device locking itself.\n"
				      "Contact your system administrator."));
      warning++;
    }
    close(fd);
  } 

  // Test 5: check for existence of /etc/resolv.conf
  
  if ((fd = open("/etc/resolv.conf", O_RDONLY)) >= 0)
    close(fd);
  else {
    if (geteuid() == 0) {
      if ((fd = open("/etc/resolv.conf", O_WRONLY|O_CREAT)) >= 0) {
	// file will be owned by root and world readible
	fchown(fd, 0, 0);
	fchmod(fd, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	close (fd);
      }
    } else {
      QMessageBox::warning
	(0, klocale->translate("Error"),
	 klocale->translate("/etc/resolv.conf is missing!\n\n"
			    "Ask your system administrator to create\n"
			    "a non-empty file that has appropriate\n"
			    "read and write permissions."));
      warning ++;
    }
  }
      
  if(warning == 0)
    return TEST_OK;
  else
    return TEST_WARNING;
}
