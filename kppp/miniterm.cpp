/*
 *            kPPP: A front end for pppd for the KDE project
 *
 * $Id$
 * 
 * Copyright (C) 1997 Bernd Johannes Wuebben 
 *                    wuebben@math.cornell.edu
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
 */

#include <qdir.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <sys/types.h>


#include "pppdata.h"
#include "miniterm.h"

#define T_WIDTH 550
#define T_HEIGHT 400

extern PPPData gpppdata;

MiniTerm::MiniTerm(QWidget *parent, const char *name)
  : QDialog(parent, name,TRUE, WStyle_Customize|WStyle_NormalBorder)
{

  col = line = col_start = line_start = 0;
  modemfd = -1;

  setCaption(i18n("Kppp Mini-Terminal"));

  m_file = new QPopupMenu;
  m_file->insertItem( i18n("&Quit"),this, SLOT(cancelbutton()) );
  m_options = new QPopupMenu;
  m_options->insertItem(i18n("&Reset Modem"),this,SLOT(resetModem()));
  m_help = new QPopupMenu;
  m_help->insertItem( i18n("&Help"),this, SLOT(help()) );
  
  menubar = new QMenuBar( this );
  menubar->insertItem( i18n("&File"), m_file );
  menubar->insertItem( i18n("&Modem"), m_options );
  menubar->insertItem( i18n("&Help"), m_help);
  
  statusbar = new QLabel(this);
  statusbar->setFrameStyle(QFrame::Panel | QFrame::Sunken);

  statusbar2 = new QLabel(this);
  statusbar2->setFrameStyle(QFrame::Panel | QFrame::Sunken);

  terminal = new MyTerm(this,"term");
  connect(terminal,SIGNAL(got_a_line()),this,SLOT(process_line()));

  setupToolbar();

  statusbar->setGeometry(0, T_HEIGHT - 20, T_WIDTH - 70, 20);
  statusbar2->setGeometry(T_WIDTH - 70, T_HEIGHT - 20, 70, 20);

  menubar->setGeometry(0,0,T_WIDTH,30);

  terminal->setGeometry(0, menubar->height() + toolbar->height() , 
   T_WIDTH,  T_HEIGHT - menubar->height() - toolbar->height() - statusbar->height());
 
  readtimer = new QTimer(this);
  connect(readtimer,SIGNAL(timeout()),this,SLOT(readtty()));

  inittimer = new QTimer(this);
  connect(inittimer,SIGNAL(timeout()),this,SLOT(init()));
  inittimer->start(500);

}  

MiniTerm::~MiniTerm() {

  delete toolbar;
  delete statusbar;
  delete statusbar2;

}

void MiniTerm::setupToolbar(){

  toolbar = new KToolBar( this );

  KIconLoader *loader = kapp->getIconLoader();

  QPixmap pixmap;

  pixmap = loader->loadIcon("exit.xpm");
  toolbar->insertButton(pixmap, 0,
		      SIGNAL(clicked()), this,
		      SLOT(cancelbutton()), TRUE, i18n("Quit MiniTerm"));

  pixmap = loader->loadIcon("back.xpm");
  toolbar->insertButton(pixmap, 0,
		      SIGNAL(clicked()), this,
		      SLOT(resetModem()), TRUE, i18n("Reset Modem"));

  pixmap = loader->loadIcon("help.xpm");
  toolbar->insertButton(pixmap, 0,
		      SIGNAL(clicked()), this,
		      SLOT(help()), TRUE, i18n("Help"));

  toolbar->setBarPos( KToolBar::Top );

}

void MiniTerm::process_line(){

  QString newline;
  newline = terminal->textLine(line);
  newline = newline.remove(0,col_start);
  newline = newline.stripWhiteSpace();
  writeline(newline.data());

}

void MiniTerm::resizeEvent(QResizeEvent*){

  menubar->setGeometry(0,0,width(),30);

  toolbar->setGeometry(0,menubar->height(),width(),toolbar->height());

  terminal->setGeometry(0, menubar->height() + toolbar->height() , 
   width(),  height() - menubar->height() - toolbar->height() - statusbar->height());

  statusbar->setGeometry(0, height() - 20, width() - 70, 20);
  statusbar2->setGeometry(width() - 70, height() - 20, 70, 20);

}

void MiniTerm::init() {

  inittimer->stop();
  statusbar->setText(i18n("Initializing Modem"));
  kapp->processEvents();

  int lock = lockdevice();
  if (lock == 1){
    
    statusbar->setText(i18n("Sorry, modem device is locked."));
    return;
  }
  if (lock == -1){
    
    statusbar->setText(i18n("Sorry, can't create modem lock file."));
    return;
  }

  if(opentty()){

    writeline(gpppdata.modemHangupStr());
    usleep(100000);  // wait 0.1 secs
    if(hangup()) {
      writeline(gpppdata.modemInitStr());
      usleep(100000);
      
      statusbar->setText(i18n("Modem Ready"));
      terminal->setFocus();
      
      kapp->processEvents();
      kapp->processEvents();
      readtimer->start(1);

      return;
    }
  }
  
  // opentty() or hangup() failed 
  statusbar->setText(modemMessage());
  unlockdevice();
  
}                  



void MiniTerm::readtty() {

  char c;

  if(read(modemfd, &c, 1) == 1) {
    c = ((int)c & 0x7F);
    // printf("read:%x %c\n",c,c);
    
    // TODO sort this shit out

    if(((int)c != 13)&& ((int)c != 10)&&((int)c != 8))
      terminal->insertChar( c );

    if((int)c == 8)
      terminal->backspace();
    if((int)c == 127)
      terminal->backspace();

    if((int)c == 10)
      terminal->mynewline();

    if((int)c == 13)
      terminal->myreturn();
  }
  
}


void MiniTerm::cancelbutton() {


  readtimer->stop();
  statusbar->setText(i18n("Hanging up ..."));
  kapp->processEvents();
  kapp->flushX();

  if(modemfd >= 0) {
    writeline(gpppdata.modemHangupStr());
    usleep(100000); // 0.1 sec
    hangup();
  }

  closetty();
  unlockdevice();

  reject();
}



void MiniTerm::resetModem(){
 
  statusbar->setText(i18n("Resetting Modem"));
  terminal->newLine();
  kapp->processEvents();
  kapp->flushX();

  if(modemfd >= 0) {
    writeline(gpppdata.modemHangupStr());
    usleep(100000); // 0.1 sec
    hangup();
  }
  statusbar->setText(i18n("Modem Ready"));
}


bool MiniTerm::writeChar(char c){

  write(modemfd,&c,1);
  return true;

}


void MiniTerm::closeEvent( QCloseEvent *e ){

  e->ignore();     // don't let the user close the window

}

void MiniTerm::help(){

  kapp->invokeHTMLHelp("kppp/kppp.html","");

}


MyTerm::MyTerm(QWidget *parent, const char* name)
  : QMultiLineEdit(parent, name)
{
   p_parent = (MiniTerm*)parent;
   this->setFont(QFont("courier",12,QFont::Normal));
  
}

void MyTerm::keyPressEvent(QKeyEvent *k) {

  // ignore meta keys
  if (k->ascii() == 0) return;

  if(k->ascii() == 13){
    myreturn();
    p_parent->writeChar((char) k->ascii());
    return;
  }


  p_parent->writeChar((char) k->ascii());

}

void MyTerm::insertChar(char c) {
  
  QMultiLineEdit::insertChar(c);

}

void MyTerm::newLine() {
  
  QMultiLineEdit::newLine();

}

void MyTerm::del() {
  
  QMultiLineEdit::del();

}

void MyTerm::backspace() {
  
  //  QMultiLineEdit::cursorLeft();
  QMultiLineEdit::backspace();

}

void MyTerm::myreturn() {
  
    int column;
    int line;

    getCursorPosition(&line,&column);
    for (int i = 0; i < column;i++)
      QMultiLineEdit::cursorLeft();

}

void MyTerm::mynewline() {
  

    QMultiLineEdit::end(FALSE);
    QMultiLineEdit::newLine();

}

#include "miniterm.moc"
