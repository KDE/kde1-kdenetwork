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

#ifndef _MODEM_H_
#define _MODEM_H_

#include <qdir.h>

#include <sys/types.h>
#include <signal.h>
#include <termios.h>
#include <unistd.h>

#include <config.h>

int     lockdevice();
void    unlockdevice();
void    alarm_handler(int);

class Modem {

public:
  Modem();
  ~Modem() {}

public:
  bool    opentty();
  bool    closetty();
  speed_t modemspeed();
  bool    writeline(const char *);
  bool    hangup();
  void    escape_to_command_mode();
  char    *modemMessage();
  // private:
  bool    modem_in_connect_state; 
  int     modemfd;

private:
  QString errmsg;
  struct termios initial_tty;
  struct termios tty;

};

#ifndef HAVE_USLEEP
void usleep (long);
#endif

#endif


