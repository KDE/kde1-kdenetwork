/*
 *              kPPP: A pppd Front End for the KDE project
 *
 * $Id$
 *
 *              Copyright (C) 1997 Bernd Johannes Wuebben
 *                      wuebben@math.cornell.edu
 *
 * This file was added by Harri Porten <porten@tu-harburg.de> 
 *
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

#include <errno.h>

#include "modem.h"
#include "pppdata.h"

QString lockfile;
bool 	modem_is_locked = false;

// Lock modem device. Returns 0 on success 1 if the modem is locked and -1 if
// a lock file can't be created ( permission problem )

int lockdevice() {

  int fd;
  char newlock[80]="";

  QDir lockdir(gpppdata.modemLockDir());

  if(lockdir == ".") {
#ifdef MY_DEBUG
  printf("gpppdata.modemLockDir is empty ..."\
	   "assuming the user doesn't want a lockfile.\n");
#endif
    return 0;
  }

  QString device = gpppdata.modemDevice();

  // bail out if we encounter anything else than /dev/*
  if (device.left(5) != "/dev/" || device.findRev('/') != 4)
    return -1;

  lockfile = lockdir.absPath();

  lockfile += "/LCK.."; 
  lockfile += device.right(device.length() - 5);

  if (modem_is_locked) 
    return 1;

  struct stat st;
  if(stat(lockfile.data(), &st) == -1) {
    if(errno == EBADF)
      return -1;
  } else {
    // make sure that this is a file, not a special file
    if(!S_ISREG(st.st_mode)) 
      return -1;
  }


  if ((fd = open(lockfile.data(), O_RDONLY)) >= 0) {

    // Mario: it's not necessary to read more than lets say 32 bytes. If
    // file has more than 32 bytes, skip the rest
    char oldlock[33];
    int sz = read(fd, &oldlock, 32);
    close (fd);
    if (sz <= 0)
      return 1;
    oldlock[sz] = '\0';
      
#ifdef MY_DEBUG
    printf("Device is locked by: %s\n", &oldlock);
#endif
      
    int oldpid;
    int match = sscanf(oldlock, "%d", &oldpid);

    // found a pid in lockfile ?
    if (match < 1 || oldpid <= 0)
      return 1;
    
    // check if process exists
    if (kill((pid_t)oldpid, 0) == 0)
      return 1;
    if (errno != ESRCH)
      return 1;
      
#ifdef MY_DEBUG
    printf("lockfile is stale\n");
#endif
  }

  if((fd = open(lockfile.data(), O_WRONLY|O_TRUNC|O_CREAT,0644)) >= 0) {
    sprintf(newlock,"%05d %s %s\n", getpid(), "kppp", "user" );

#ifdef MY_DEBUG
    printf("Locking Device: %s\n",newlock);
#endif

    write(fd, newlock, strlen(newlock));
    close(fd);
    modem_is_locked=true;

    return 0;
  }

  return -1;

}
  

// UnLock modem device

void unlockdevice() {

  if (modem_is_locked) {
    unlink(lockfile);
    modem_is_locked=false;

#ifdef MY_DEBUG
   printf("UnLocking Modem Device\n");
#endif

  }

}  


speed_t modemspeed() {

  int i;

  // convert the string modem speed int the gpppdata object to a t_speed type
  // to set the modem.  The constants here should all be ifdef'd because
  // other systems may not have them

  i = atoi(gpppdata.speed())/100;

  switch(i) {
  case 24:
    return B2400;
    break;
  case 96:
    return B9600;
    break;
  case 192:
    return B19200;
    break;
  case 384:
    return B38400;
    break;
#ifdef B57600
  case 576:
    return B57600;
    break;
#endif

#ifdef B115200
  case 1152:
    return B115200;
    break;
#endif

#ifdef B230400
  case 2304:
    return B230400;
    break;
#endif

#ifdef B460800 
  case 4608:
    return 4608;
    break;
#endif

  default:            
    return B9600;
    break;
  }
}
