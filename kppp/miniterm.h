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


#ifndef _MINITERM_H_
#define _MINITERM_H_

#include <qpixmap.h>
#include <qmenubar.h> 
#include <qpopmenu.h> 
#include <qgrpbox.h> 
#include <qdialog.h>
#include <qlined.h>
#include <qmlined.h>
#include <qpushbt.h>
#include <qlabel.h>
#include <qevent.h>
#include <qtimer.h>
#include <qsignal.h>
#include <qframe.h>
#include <qmsgbox.h>
#include <qrangect.h>
#include <qmlined.h> 
#include <qtooltip.h> 

#include <unistd.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>

#include <ktoolbar.h>
#include <kmenubar.h>
#include <kiconloader.h>

#include "pppdata.h"

class MiniTerm;

class MyTerm : public QMultiLineEdit {

  Q_OBJECT

public:

  MyTerm(QWidget *parent=0, const char *name=0);
  ~MyTerm() {}

  void keyPressEvent (QKeyEvent*);
  void insertChar(char c);
  void newLine();
  void backspace();
  void del();
  void myreturn();
  void mynewline();
  
signals:

  void got_a_line();
private:
  bool return_pressed;  // was the last key pressed a "return" ?
  MiniTerm *p_parent;


  
};


class MiniTerm : public QDialog {

  Q_OBJECT

public:

  MiniTerm(QWidget *parent=0, const char *name=0);


void closeEvent( QCloseEvent *e );
void resizeEvent(QResizeEvent *e);

public slots:

  void cancelbutton();
  void init();
  bool opentty();
  bool writeline(const char *buf);
  void readtty();
  void hangup();
  bool closetty();
  void process_line();
  void help();
  void resetModem();
  bool writeChar(char c);

protected:

  void setupToolbar();

  QPushButton *cancel;
  MyTerm *terminal;
  QTimer *inittimer;
  QTimer *readtimer;

  struct termios tty;
  struct termios initial_tty;
  KMenuBar    * menubar;
  KToolBar     * toolbar;
  QPopupMenu  * m_file;
  QPopupMenu  * m_edit;
  QPopupMenu  * m_options;
  QPopupMenu  * m_help;
  QLabel      * statusbar;
  QLabel      * statusbar2;
  QPushButton *pb1;
  QPushButton *pb2;
  QPushButton *pb3;
  QPushButton *pb4;

  QPixmap pb1_pixmap;
  QPixmap pb2_pixmap;
  QPixmap pb3_pixmap;
  QPixmap pb4_pixmap;



public:
  int 	modemfd;
  int line;
  int col;
  int line_start;
  int col_start;
};


#endif





