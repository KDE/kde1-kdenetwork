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
#include <sys/ioctl.h>
#include <setjmp.h>

#include "modem.h"
#include "pppdata.h"

QString lockfile;
bool    modem_is_locked = false;
bool    expect_alarm = false;
static jmp_buf jmp_buffer;

Modem::Modem() {

  modemfd = -1;
  modem_in_connect_state = false;

}


speed_t Modem::modemspeed() {

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


bool Modem::opentty() {

  int flags;

  if((modemfd = open(gpppdata.modemDevice(), O_RDWR|O_NDELAY)) < 0){
    errmsg = i18n("Sorry, can't open modem.");
    return false;
  }
  
  if(gpppdata.UseCDLine()) {
    ioctl( modemfd, TIOCMGET, &flags ); 
    if ((flags&TIOCM_CD)==0) {
      errmsg = i18n("Sorry, the modem is not ready.");
      ::close(modemfd);
      modemfd=-1;
      return false;
    }
  }
	
  if(tcgetattr(modemfd, &tty) < 0){
    errmsg = i18n("Sorry, the modem is busy.");
    ::close(modemfd);
    modemfd = -1;
    return false;
  }

  memset(&initial_tty,'\0',sizeof(initial_tty));

  initial_tty = tty;

  tty.c_cc[VMIN] = 0; // nonblocking 
  tty.c_cc[VTIME] = 0;
  tty.c_oflag = 0;
  tty.c_lflag = 0;

  tty.c_cflag &= ~(CSIZE | CSTOPB | PARENB);  
  tty.c_cflag |= CS8 | CREAD;
  tty.c_cflag |= CLOCAL;                   // ignore modem status lines      
  tty.c_iflag = IGNBRK | IGNPAR | ISTRIP;  // added ISTRIP
  tty.c_lflag &= ~ICANON;                  // non-canonical mode
  tty.c_lflag &= ~(ECHO|ECHOE|ECHOK|ECHOKE);


  if(strcmp(gpppdata.flowcontrol(), "None") != 0) {
    if(strcmp(gpppdata.flowcontrol(), "CRTSCTS") == 0) {
      tty.c_cflag |= CRTSCTS;
    }
    else {
      tty.c_iflag |= IXON | IXOFF;
      tty.c_cc[VSTOP]  = 0x13; /* DC3 = XOFF = ^S */
      tty.c_cc[VSTART] = 0x11; /* DC1 = XON  = ^Q */
    }
  }
  else {
    tty.c_cflag &= ~CRTSCTS;
    tty.c_iflag &= ~(IXON | IXOFF);
  }

  cfsetospeed(&tty, modemspeed());
  cfsetispeed(&tty, modemspeed());

  if(tcsetattr(modemfd, TCSANOW, &tty) < 0){
    errmsg = i18n("Sorry, the modem is busy.");
    ::close(modemfd);
    modemfd=-1;
    return false;
  }

  errmsg = i18n("Modem Ready.");
  return true;
}


bool Modem::closetty(){

  if(modemfd >=0 ){

    /* discard data not read or transmitted */
    tcflush(modemfd, TCIOFLUSH);
    
    if(tcsetattr(modemfd, TCSANOW, &initial_tty) < 0){
      errmsg = i18n("Can't restore tty settings: tcsetattr()\n");
      return false;
    }
    ::close(modemfd);
  }

  return true;
  
}

bool Modem::writeline(const char *buf) {

  // TODO check return code and think out how to proceed
  // in case of trouble.

   write(modemfd, buf, strlen(buf));

  // Let's send an "enter"
  // which enter we send depends on what the user has selected
  // I haven't seen this on other dialers but this seems to be
  // necessary. I have tested this with two different modems and 
  // one needed an CR the other a CR/LF. Am i hallucinating ?
  // If you know what the scoop is on this please let me know. 

  if(strcmp(gpppdata.enter(), "CR/LF") == 0)
    write(modemfd, "\r\n", 2);
  else if(strcmp(gpppdata.enter(), "LF") == 0)
    write(modemfd, "\n", 1);
  else if(strcmp(gpppdata.enter(), "CR") == 0)
    write(modemfd, "\r", 1);
 
  return true;
}


bool Modem::hangup() {

  // this should really get the modem to hang up and go into command mode
  // If anyone sees a fault in the following please let me know, since
  // this is probably the most imporant snippet of code in the whole of
  // kppp. If people complain about kppp being stuck, this piece of code
  // is most likely the reason.

  struct termios temptty;

  if(modemfd >= 0) {

    // is this Escape & HangupStr stuff really necessary ? (Harri)

    if ( modem_in_connect_state ) escape_to_command_mode(); 

    // Then hangup command
    writeline(gpppdata.modemHangupStr());
    
    usleep(gpppdata.modemInitDelay() * 10000); // 0.01 - 3.0 sec 

    if (setjmp(jmp_buffer) == 0) {
      // set alarm in case tcsendbreak() hangs 
      signal(SIGALRM, alarm_handler);
      alarm(2);
      expect_alarm = true;
      
      tcsendbreak(modemfd, 0);
      
      expect_alarm = false;
      alarm(0);
      signal(SIGALRM, SIG_IGN);
    } else {
      // we reach this point if the alarm handler got called
      errmsg = i18n("Sorry, the modem doesn't respond.");
      return false;
    }

    tcgetattr(modemfd, &temptty);
    cfsetospeed(&temptty, B0);
    cfsetispeed(&temptty, B0);
    tcsetattr(modemfd, TCSAFLUSH, &temptty);

    usleep(gpppdata.modemInitDelay() * 10000); // 0.01 - 3.0 secs 

    cfsetospeed(&temptty, modemspeed());
    cfsetispeed(&temptty, modemspeed());
    tcsetattr(modemfd, TCSAFLUSH, &temptty);
   
    return true;

  } else
    return false;

}


void Modem::escape_to_command_mode() {

// Send Properly bracketed escape code to put the modem back into command state.
// A modem will accept AT commands only when it is in command state.
// When a modem sends the host the CONNECT string, that signals
// that the modem is now in the connect state (no long accepts AT commands.)
// Need to send properly timed escape sequence to put modem in command state.
// Escape codes and guard times are controlled by S2 and S12 values.
// 

  tcflush(modemfd,TCOFLUSH);
  // +3 because quiet time must be greater than guard time.
  usleep((gpppdata.modemEscapeGuardTime()+3)*20000);
  write(modemfd, gpppdata.modemEscapeStr(), strlen(gpppdata.modemEscapeStr()) );  
  tcflush(modemfd,TCOFLUSH);
  usleep((gpppdata.modemEscapeGuardTime()+3)*20000);

  modem_in_connect_state = false;   // side-effect?

}

char *Modem::modemMessage() {

  return errmsg.data();

}

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

  // bail out if we encounter anything else than /dev/xxx
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

void alarm_handler(int) {

#ifdef MY_DEBUG
  printf("alarm_handler(): Received SIGALRM\n");
#endif

  // jump 
  if (expect_alarm)
    longjmp(jmp_buffer, 1);

}


#ifndef HAVE_USLEEP

// usleep for those of you out there who don't have a BSD 4.2 style usleep

extern int select();

void usleep(long usec){
  
  struct struct {
    long tv_sec;
    long tv_usec;
  } tval;

  tval.tv_sec = usec / 1000000;
  tval.tv_usec = usec % 1000000;
  select(0, (long *)0, (long *)0, (long *)0, &tval);
  return;

}

#endif /* HAVE_USLEEP */
