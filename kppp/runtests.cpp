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

QString findFileInPath( const char *fname, const char *extraPath = 0 ) {
  QString f;  

  if(access(fname, F_OK) == 0)
    return QString(fname);

  // strip arguments
  QString _fname = fname;
  if(_fname.find(' ') != -1)
    _fname = _fname.left(_fname.find(' '));

  char path[2048];

  // for absolute path
  strcpy(path, ":");

  if(getenv("PATH") != NULL)
    strncat(path, getenv("PATH"), sizeof(path)-128);

  if(extraPath != 0)
    strcat(path, extraPath);

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

int runTests() {
  int warning = 0;

  // Test 1: search the pppd binary
  QString f = gpppdata.pppdPath();
  bool pppdFound = FALSE;
  if(access(f.data(), F_OK) == 0)
    pppdFound = TRUE;
  else {
    f = findFileInPath("pppd", ":/usr/sbin:/sbin:/usr/local/sbin");
    pppdFound = f.length() > 0;

    // save the new location
    if(pppdFound) {
      gpppdata.setpppdPath(f.data());
      gpppdata.save();
    }
  }

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

  if(warning == 0)
    return TEST_OK;
  else
    return TEST_WARNING;
}
