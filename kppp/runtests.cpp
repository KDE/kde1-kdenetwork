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
#include <sys/stat.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

QString findFileInPath( const char *fname, const char *extraPath = 0 ) {
  QString f;

  char path[2048];
  if(getenv("PATH") != NULL)
    strncpy(path, getenv("PATH"), sizeof(path)-128);
  else
    strcpy(path, "");

  if(extraPath != 0)
    strcat(path, extraPath);

  char *p = strtok(path, ":");
  while(p != NULL) {
    f = p;
    f += "/";
    f += fname;
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
    if(pppdFound)
      gpppdata.setpppdPath(f.data());
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
      gpppdata.setlogViewer("kvt -e vi");
    }
  }

  

  if(warning == 0)
    return TEST_OK;
  else
    return TEST_WARNING;
}
