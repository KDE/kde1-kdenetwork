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
#include <qmultilinedit.h>
#include <qlayout.h>
#include <kapp.h>
#include "macros.h"

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
  if(access("/var/log/messages", R_OK) == 0)
    return "/var/log/messages";

  if(access("/var/log/syslog.ppp", R_OK) == 0)
    return "/var/log/syslog.ppp";

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
  list.at(list.count() - 1);

  do {
    list.prev();
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
  } while(list.prev() != 0);

  /* clear security related info */
  list.at(0);

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

  PPPL_MakeLog(sl);
  QDialog *dlg = new QDialog(0, "", TRUE);

  dlg->setCaption(i18n("PPP log"));
  QVBoxLayout *tl = new QVBoxLayout(dlg, 10, 10);
  QMultiLineEdit *edit = new QMultiLineEdit(dlg);
  edit->setReadOnly(TRUE);
  KButtonBox *bbox = new KButtonBox(dlg);
  bbox->addStretch(1);
  QPushButton *close = bbox->addButton(i18n("Close"));
  bbox->addStretch(1);
  bbox->layout();
  edit->setMinimumSize(600, 250);

  tl->addWidget(edit, 1);
  tl->addWidget(bbox);
  tl->freeze();

  for(unsigned i = 0; i < sl.count(); i++)
    edit->append(sl.at(i));
  
  dlg->connect(close, SIGNAL(clicked()),
	       dlg, SLOT(accept()));

  dlg->exec();  
  delete dlg;
}
