/*
 *
 *              kPPP: A pppd Front End for the KDE project
 *              Copyright (C) 1997 Bernd Johannes Wuebben
 *                      wuebben@math.cornell.edu
 * 
 * This file was contributed by Mario Weilguni <mweilguni@sime.com>
 * Thanks Mario!
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this program; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <qdir.h>
#include <unistd.h>
#include <qregexp.h>
#include "pap.h"
#include "chap.h"
#include "pppdata.h"


bool PAP_UsePAP() {
  return (bool)(gpppdata.authMethod() == AUTH_PAP);
}

bool PAP_CreateAuthFile(char *authfile) {
  QFile fin, fout;
  QString fname = authfile;
  fname += ".new";
  
  // copy to new file pap-secrets  
  fout.setName(fname.data());
  if(fout.open(IO_WriteOnly)) {
    QString user = gpppdata.storedUsername();
    QString pass = gpppdata.password;
    //    QRegExp r_user((user + "[ \t]").data());
    QRegExp r_user("\\s*" + user + "[ \t]");		
    QRegExp r_user2("\\s*[\"\']" + user + "[\"\']");

    // copy old file
    fin.setName(authfile);
    if(fin.open(IO_ReadOnly)) {
      QTextStream t(&fin);
      QString line;
      
      while(!t.eof()) {
	line = t.readLine();
	if(line.find(r_user) == 0 || line.find(r_user2) == 0)
	  continue;

	fout.writeBlock(line.data(), line.length());
	fout.writeBlock("\n", 1);
      }

      fin.close();    
    }

    // append user/pass pair
    QString line = "\"" + user + "\"\t*\t\"" + pass + "\"\n";
    fout.writeBlock(line.data(), line.length());

    fout.close();
  }

  QDir d;
  QString oldName = authfile;
  oldName += ".old";

  // delete old file if any
  d.remove(oldName.data());

  d.rename(authfile, oldName.data());
  d.rename(fname.data(), authfile);
  chmod(authfile, 0600);

  return TRUE;
}

bool PAP_RemoveAuthFile(char *authfile) {
  if(!PAP_UsePAP() && strcmp(authfile, PAP_AUTH_FILE) == 0)
    return FALSE;

  if(!CHAP_UseCHAP() && strcmp(authfile, CHAP_AUTH_FILE) == 0)
    return FALSE;

  QString oldName = authfile;
  oldName += ".old";

  QDir d;
  if(d.exists(oldName.data())) {
    d.remove(authfile);
    return d.rename(oldName.data(), authfile);
  } else
    return FALSE;
}
