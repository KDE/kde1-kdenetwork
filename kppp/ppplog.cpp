/*
 *
 *            kPPP: A pppd front end for the KDE project
 *
 * $Id$
 *
 * (c) 1998 Mario Weilguni <mweilguni@kde.org>
 *
 *            Copyright (C) 1997 Bernd Johannes Wuebben
 *                   wuebben@math.cornell.edu
 *
 * based on EzPPP:
 * Copyright (C) 1997  Jay Painter
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
#include <qglobal.h>
#include <qstring.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <qstrlist.h>
#include <qdialog.h>
#include <kbuttonbox.h>
// use short form for compatibility to Qt 1.33
#include <qmlined.h>
// #include <qmultilinedit.h>
#include <qlayout.h>
#include <kapp.h>
#include <kmsgbox.h>
#include "macros.h"
#include "pppdata.h"

bool PPPL_is_pppd_line(const char *s) {
  char *p;

  if(s == 0)
    return FALSE;

  p = (char *)strstr(s, "pppd[");
  if(p == 0)
    return FALSE;
  p += strlen("pppd[");
  while(isdigit(*p))
    p++;

  if(*p == ']')
    return TRUE;

  return FALSE;
}

const char *PPPL_findLogFile() {
  FILE *f;

  // access() cannot be used here, because access also
  // test the uid of the current user. So I use fopen()
  // here as long as nothing better is found
  if((f = fopen("/var/log/messages", "r")) != NULL) {
    fclose(f);
    return "/var/log/messages";
  }

  if((f = fopen("/var/log/syslog.ppp", "r")) == 0) {
    fclose(f);
    return "/var/log/syslog.ppp";
  }

  return 0;
}

int PPPL_MakeLog(QStrList &list) {
  FILE *f;
  int pid, newpid;
  char buffer[1024], *p;

  const char *fname = PPPL_findLogFile();
  if(fname == 0) {
    fprintf(stderr, "Cannot find logfile!\n");
    return 1;
  }

  f = fopen(fname, "r");
  if(f == 0) {
    fprintf(stderr, "Cannot open logfile!\n");
    return 1;
  }

  while(fgets(buffer, sizeof(buffer)-1, f) != 0) {
    if(!PPPL_is_pppd_line(buffer))
      continue;    

    if(strlen(buffer) && buffer[strlen(buffer)-1] == '\n')
      buffer[strlen(buffer)-1] = '\0';
    list.append(buffer);
  }
  fclose(f);

  if(list.count() == 0)
    return 2;

  /* find out pid of pppd */
  p = (char *)strchr(list.last(), '[');
  if(p == 0)
    return 3;

  p++;
  sscanf(p, "%d", &pid);

  // position at EOL
  list.last();

  while(list.prev()) {
    p = (char *)strchr(list.current(), '[');
    if(p != 0) {
      p++;
      sscanf(p, "%d", &newpid);

      /* truncate list */
      if(newpid != pid) {
	int cnt = list.at();

	for(int i = 0; i <= cnt; i++)
	  list.removeFirst();
      }
    }
  }

  /* clear security related info */
  list.first();

  char *keyword[] = {"name = \"",
		    "user=\"",
		    "password=\"",
		    0};

  while(list.current() != 0) {
    for(int j = 0; keyword[j] != 0; j++) {
      char *p;

      if( (p = (char *)strstr(list.current(), keyword[j])) != 0) {
	p += strlen(keyword[j]);
	while(*p && *p != '"')
	  *p++ = 'X';
      }
    }

    list.next();
  }

  return 0;
}

void PPPL_ShowLog() {
  QStrList sl;
  int i;
  PPPL_MakeLog(sl);

//   if(ret != 0 || sl.count() == 0) {
//     KMsgBox::message(0,
// 		     i18n("Error"),
// 		     i18n("KPPP is not able to generate a PPP log.\n\nPossible reasons:\n   * the \"debug\" option has not been used\n   * an unusual syslogd configuration\n   * a KPPP bug"),
// 		     KMsgBox::STOP);
//     return;
//   }

  bool foundLCP = gpppdata.getPPPDebug();
  for(i = 0; !foundLCP && i < (int)sl.count(); i++) {
    fprintf(stderr, "LINE=%s\n", sl.at(i));
    if(strstr("[LCP", sl.at(i)) == NULL)
      foundLCP = TRUE;
}

  if(!foundLCP) {
    int result = KMsgBox::yesNo(0,
				i18n("Warning"),
				i18n("
KPPP could not prepare a PPP log. It´s very likely
that pppd was started without the \"debug\" option.\n
Without this option it´s difficult to find out PPP
problems, so you should turn on the debug option.\n
Shall I turn it on now?"),
				KMsgBox::QUESTION);

    if(result != 0) {
      gpppdata.setPPPDebug(TRUE);
      KMsgBox::message(0,
		       i18n("Information"),
		       i18n("
The \"debug\" option has been added. You
should now try to reconnect. If that fails
again, you will get a PPP log that may help
you to track down the connection problem."),
		       KMsgBox::INFORMATION);
    }
    
    return;
  }

  // scan for remote messages
  const char *rmsg = "Remote message: ";
  for(i = sl.count()-1; i >=0 ; i--) {
    char *p = strstr(sl.at(i), rmsg);

    if(p) {
      p += strlen(rmsg);
      if(strlen(p)) {
        // found a remote message
        QString msg(2048);
        msg.sprintf(i18n("
The remote system system has sent the following message:

\"%s\"

This may give you a hint why the connection has failed."), p);
      
        KMsgBox::message(0,
                         i18n("Error"),
                         msg.data(),
                         KMsgBox::STOP);
      }
    }
  }

  QDialog *dlg = new QDialog(0, "", TRUE);

  dlg->setCaption(i18n("PPP log"));
  QVBoxLayout *tl = new QVBoxLayout(dlg, 10, 10);
  QMultiLineEdit *edit = new QMultiLineEdit(dlg);
  edit->setReadOnly(TRUE);
  KButtonBox *bbox = new KButtonBox(dlg);
  bbox->addStretch(1);
  QPushButton *write = bbox->addButton(i18n("Write to file"));
  QPushButton *close = bbox->addButton(i18n("Close"));
  bbox->layout();
  edit->setMinimumSize(600, 250);

  tl->addWidget(edit, 1);
  tl->addWidget(bbox);
  tl->freeze();

  for(i = 0; i < (int)sl.count(); i++)
    edit->append(sl.at(i));
  
  dlg->connect(close, SIGNAL(clicked()),
	       dlg, SLOT(reject()));
  dlg->connect(write, SIGNAL(clicked()),
	       dlg, SLOT(accept()));

  if(dlg->exec()) {
    QDir d = QDir::home();
    QString s = d.absPath() + "/PPP-logfile";
    FILE *f = fopen(s.data(), "w");
    for(i = 0; i < (int)sl.count(); i++)
      fprintf(f, "%s\n", sl.at(i));
    fclose(f);

    QString msg;
    msg.sprintf("The PPP log has been saved\nas \"%s\"!\n\nIf you want to send a bug report or have\nproblems connecting to the internet, please\nattach this file. It will help the maintainers\nto find the bug and to improve KPPP", s.data());
    KMsgBox::message(0,
		     i18n("Information"),
		     msg.data(),
		     KMsgBox::INFORMATION);
  }
  delete dlg;
}


