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

#ifdef HAVE_NETINET_IN_H
#include <netinet/in.h>
#endif

#ifdef HAVE_RESOLV_H
#include <resolv.h>
#endif

#ifndef _PATH_RESCONF
#define _PATH_RESCONF "/etc/resolv.conf"
#endif

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
      while(f != NULL && !feof(f)) {
	if(fgets(buf, sizeof(buf), f) != NULL) {
	  QString s(buf, sizeof(buf));

	  s = s.stripWhiteSpace();
	  if(s[0] == '#' || s.length() == 0)
	    continue;

	  if((uid_t)uidFromName(s.data()) == getuid()) {
	    access = TRUE;
	    fclose(f);
	    f = NULL;
	  }
	}
      }
      if(f)
	fclose(f);
    }

    if(!access) {
      QMessageBox::warning(0,
		 i18n("Error"),
		 i18n("You´re not allowed to dial out with "
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
		 i18n("Error"),
		 i18n("Cannot find the pppd-daemon!\n\n"
				    "Make sure that pppd is installed and\n"
				    "you have entered the correct path.\n"
				    ));
    warning++;
  }

  // Test 2: check access to the pppd binary
  if(pppdFound) {
    if(access(f.data(), X_OK) != 0 && geteuid() != 0) {
      QMessageBox::critical(0,
		   i18n("Error"),
		   i18n("You do not have the permission\n"
				      "to start pppd!\n\n"
				      "Contact your system administrator\n"
				      "and ask to get access to pppd."));
      return TEST_CRITICAL;
    } else {
      struct stat st;
      stat(f.data(), &st);
      if(st.st_mode & S_ISUID == 0 && getuid() != 0) {
	QMessageBox::warning(0,
		     i18n("Error"),
		     i18n("pppd is not properly installed!\n\n"
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
		     i18n("Error"),
		     i18n("pppd is not properly installed!\n\n"
					"The pppd binary must be installed\n"
					"with the SUID bit set. Contact your\n"
					"system administrator."));
	return TEST_CRITICAL;
      }
    }
  }

  // Test 4: check for undesired 'lock' option in /etc/ppp/options
  QFile opt(SYSOPTIONS);
  if (opt.open(IO_ReadOnly)) {
    QTextStream t(&opt);
    QRegExp r1("^lock");
    QRegExp r2("\\slock$");   // \s matches white space (9,10,11,12,13,32)
    QRegExp r3("\\slock\\s");
    QString s;
    int lines = 0;
    bool match = false;
    
    while (!t.eof() && lines++ < 100) {
      s = t.readLine();
      
      // truncate comments
      if (s.find('#') >= 0)
        s.truncate(s.find('#'));

      if (r1.match(s) >= 0 || r2.match(s) >= 0 || r3.match(s) >= 0)
        match = true;
    }
    opt.close();
    if (match) {
      QMessageBox::warning(0,
                           i18n("Error"),
                           i18n("kppp has detected a 'lock' option in "
                                "/etc/ppp/options.\n\nThis option has "
                                "to be removed since kppp takes care "
                                "of device locking itself.\n"
                                "Contact your system administrator."));
      warning++;
    }
  } 

  // Test 5: check for existence of /etc/resolv.conf
  int fd;
  if ((fd = open(_PATH_RESCONF, O_RDONLY)) >= 0)
    close(fd);
  else {
    if (geteuid() == 0) {
      if ((fd = open(_PATH_RESCONF, O_WRONLY|O_CREAT)) >= 0) {
	// file will be owned by root and world readible
	fchown(fd, 0, 0);
	fchmod(fd, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	close (fd);
      }
    } else {
      QString msgstr;
      msgstr = _PATH_RESCONF" ";
      msgstr += i18n("is missing!\n\n"
		     "Ask your system administrator to create\n"
		     "a non-empty file that has appropriate\n"
		     "read and write permissions.");
      QMessageBox::warning
	(0, i18n("Error"), msgstr);
      warning ++;
    }
  }

  if(warning == 0)
    return TEST_OK;
  else
    return TEST_WARNING;
}

