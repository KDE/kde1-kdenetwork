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
#include "pap.h"
#include "pppdata.h"


bool PAP_UsePAP() {
  return (bool)(gpppdata.authMethod() == AUTH_PAP);
}

QString PAP_AuthFile() {
  QString s = QDir::homeDirPath() + "/";
  s += PAP_AUTH_FILE;
  return s;
}

bool PAP_CreateAuthFile() {  
  // Create the pap authentication file. PPPD requires that the file is
  // owned by the real user. If we are running SETUID root, simply make
  // a chown on it
  QString fname = PAP_AuthFile();
  int fd = open(fname.data(), 
		O_CREAT|O_TRUNC|O_WRONLY,
		S_IRUSR|S_IWUSR);
  if(fd > 0) {
    bool err = false;
    QString s = gpppdata.storedUsername();
    s += "\n";
    err |= (write(fd, s.data(), s.length()) != (int)s.length());

    s = gpppdata.password;
    s += "\n";
    err |= (write(fd, s.data(), s.length()) != (int)s.length());
    
    // for SETUID root
    if(getuid() != geteuid() && geteuid() == 0)
      err |= (fchown(fd, getuid(), (gid_t)-1) != 0);

    err |= (close(fd) != 0);
    return !err;
  }

  return FALSE;
}

bool PAP_RemoveAuthFile() {
  if(!PAP_UsePAP())
    return FALSE;

  QString fname = PAP_AuthFile();
  if(access(fname.data(), F_OK) == 0)
    return (bool)(unlink(fname.data()) != -1);
  else
    return FALSE;
}
