/*
 *
 *            kPPP: A front end for pppd for the KDE project
 *
 * $Id$
 * 
 * Copyright (C) 1997 Bernd Johannes Wuebben 
 * wuebben@math.cornell.edu
 *
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
 *
 */


#ifndef _MODEMINFO_H_
#define _MODEMINFO_H_

#include <qgrpbox.h> 
#include <qdialog.h>
#include <qlined.h>
#include <qpushbt.h>
#include <qpainter.h>
#include <qlabel.h>
#include <qevent.h>
#include <qtimer.h>
#include <qsignal.h>
#include <qregexp.h> 
#include <qframe.h>
#include <qmsgbox.h>
#include <qrangect.h>
#include <qfont.h>

#include <unistd.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "pppdata.h"
#include "modem.h"
#include <kprogress.h>

#define NUM_OF_ATI 8

class ModemTransfer : public QDialog, public Modem {
  Q_OBJECT
public:
  ModemTransfer(QWidget *parent=0, const char *name=0);
  
  QTimer *readModemTimer;
  QTimer *initTimer;

  QString answer;
  KProgress *progressBar;
  QLabel *statusBar;
  QPushButton *cancel;

  void  setExpect(const char *n);
  void closeEvent( QCloseEvent *e);

signals:
  void ati_done();

public slots:
  void init();
  void readtty();
  void do_script();
  void time_out_slot();
  void ati_done_slot();
  void cancelbutton();

protected:
  bool  expecting;
  int 	step;
  int   main_timer_ID;
  int   ati_counter;
  QString readbuffer;
  QString expectstr;

  QTimer *inittimer;
  QTimer *readtimer;
  QTimer *timeout_timer;
  QTimer *scripttimer;

};


class ModemInfo : public QDialog {
  Q_OBJECT
public:
  ModemInfo(QWidget *parent=0, const char *name=0);
  ~ModemInfo() {}

private:
  QLabel *ati_label[NUM_OF_ATI];
  QLineEdit *ati_label_result[NUM_OF_ATI];
};

#endif
