/* -*- C++ -*-
 *
 *            kPPP: A pppd front end for the KDE project
 *
 * $Id$
 * 
 *            Copyright (C) 1997 Bernd Johannes Wuebben 
 *                   wuebben@math.cornell.edu
 *
 * based on EzPPP:
 * Copyright (C) 1997  Jay Painter
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

#ifndef _CONNECT_H_
#define _CONNECT_H_

#include<sys/types.h>
#include<sys/wait.h>
#include<signal.h>
#include<fcntl.h>
#include<unistd.h>
#include<termios.h>
#include<stdlib.h>
#include<string.h>


#include<qapp.h>
#include<qdialog.h>
#include<qtimer.h>
#include<qpushbt.h>
#include<qlabel.h>
#include<qevent.h>

#include "kpppconfig.h"
#include "debug.h"
#include "pppdata.h"
#include "pwentry.h"
#include "docking.h"

extern QApplication *qApp;

#define MAXLOOPNEST (MAX_SCRIPT_ENTRIES/2)

class ConnectWidget : public QWidget {

  Q_OBJECT

public:
  
  ConnectWidget(QWidget *parent=0, const char *name=0);
  ~ConnectWidget();
  


public:

void  set_con_speed_string();

protected:
  void timerEvent(QTimerEvent *);
  void closeEvent( QCloseEvent *e );  

private slots:
    void readtty();
    void pause();
    void cancelbutton();
    void debugbutton();
    void if_waiting_slot();

public slots:
    void init();
    void preinit();
    bool closetty();
    void script_timed_out();
    void if_waiting_timed_out();

signals:
    void if_waiting_signal();

public:
    QString myreadbuffer;  // we want to keep every thing in order to fish for the 
                           // connection speed later on
    QPushButton *debug;
    int main_timer_ID;

private:
  int vmain;
  int scriptindex;

//  static const int maxloopnest=(MAX_SCRIPT_ENTRIES/2);
  int loopnest;
  int loopstartindex[MAXLOOPNEST];
  bool loopend;
  QString loopstr[MAXLOOPNEST];

  bool semaphore;
  QTimer *inittimer;
  QTimer *timeout_timer;
  void escape_to_command_mode();
  void hangup();
  speed_t modemspeed();
  bool opentty();
  bool execppp();

  void setExpect(const char *);
  bool expecting;
  QString expectstr;

  bool writeline(const char *);
  QTimer *readtimer;
  QString readbuffer;

  bool pausing;
  PWEntry *prompt;

  int scriptTimeout;
  QTimer *pausetimer;
  QTimer *if_timer;
  QTimer *if_timeout_timer;

  QLabel *messg;
  QPushButton *cancel;

  bool firstrunID;
  bool firstrunPW;

  bool modem_in_connect_state; 
  int modemfd;
  struct termios initial_tty;
  struct termios tty;

};


// non-member function to kill&wait on the pppd child process
void killppp();
void adddns();
void removedns();
void add_domain(const char* newdomain);
void auto_hostname();

extern bool modem_is_locked;
int lockdevice();
void unlockdevice();

#endif
