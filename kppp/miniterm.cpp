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


#include <unistd.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <sys/types.h>

#include <kmsgbox.h>


#include "main.h"
#include "connect.h"
#include "miniterm.h"

#define T_WIDTH 550
#define T_HEIGHT 400

#ifdef NO_USLEEP
extern int usleep( long usec );
#endif 

extern XPPPWidget *p_xppp;
extern KApplication *app;
extern PPPData gpppdata;

MiniTerm::MiniTerm(QWidget *parent=0, const char *name=0)
  : QDialog(parent, name,TRUE, WStyle_Customize|WStyle_NormalBorder)
{

  col = line = col_start = line_start = 0;
  modemfd = -1;

  setCaption("Kppp Mini-Terminal");

  m_file = new QPopupMenu;
  m_file->insertItem( "&Quit",this, SLOT(cancelbutton()) );
  m_edit = new QPopupMenu;
  m_options = new QPopupMenu;
  m_options->insertItem("&Reset Modem",this,SLOT(resetModem()));
  m_help = new QPopupMenu;
  m_help->insertItem( "&Help",this, SLOT(help()) );
  
  menubar = new KMenuBar( this );
  menubar->insertItem( "&File", m_file );
  menubar->insertItem( "&Modem", m_options );
  menubar->insertItem( "&Help", m_help);
  
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


void MiniTerm::setupToolbar(){

  toolbar = new KToolBar( this );

  KIconLoader *loader = kapp->getIconLoader();

  QPixmap pixmap;

  pixmap = loader->loadIcon("exit.xpm");
  toolbar->insertButton(pixmap, 0,
		      SIGNAL(clicked()), this,
		      SLOT(cancelbutton()), TRUE, klocale->translate("Quit MiniTerm"));


  pixmap = loader->loadIcon("back.xpm");
  toolbar->insertButton(pixmap, 0,
		      SIGNAL(clicked()), this,
		      SLOT(resetModem()), TRUE, klocale->translate("Reset Modem"));

  pixmap = loader->loadIcon("help.xpm");
  toolbar->insertButton(pixmap, 0,
		      SIGNAL(clicked()), this,
		      SLOT(help()), TRUE, klocale->translate("Help"));

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
  statusbar->setText("Initializing Modem");
  app->processEvents();

  int lock = lockdevice();
  if (lock == 1){
    
    statusbar->setText("Sorry, modem device is locked.");
    return;
  }
  if (lock == -1){
    
    statusbar->setText("Sorry, can't create modem lock file.");
    return;
  }

  if(opentty()){

    if(modemfd >= 0) {
      writeline(gpppdata.modemHangupStr());
      usleep(100000);  // wait 0.1 secs
      hangup();
      writeline(gpppdata.modemInitStr());
      usleep(100000);
    }

    statusbar->setText("Modem Ready");
    terminal->setFocus();

    app->processEvents();
    app->processEvents();
    readtimer->start(1);
  }
  else {// commmented out since this will now be set by the opentty() better.
        // statusbar->setText("Can't open modem device");
    unlockdevice();
  }
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
  statusbar->setText("Hanging up ...");
  app->processEvents();
  app->flushX();

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
 
  statusbar->setText("Resetting Modem");
  terminal->newLine();
  app->processEvents();
  app->flushX();

  if(modemfd >= 0) {
    writeline(gpppdata.modemHangupStr());
    usleep(100000); // 0.1 sec
    hangup();
  }
  statusbar->setText("Modem Ready");
}


bool MiniTerm::closetty(){

  if(modemfd > 0)

    if(tcsetattr(modemfd, TCSANOW, &initial_tty) < 0){
      statusbar->setText("Can't restore tty settings: tcsetattr()\n");
    }

    ::close(modemfd);
    modemfd = -1;
  return TRUE;
}


bool MiniTerm::opentty() {

 memset(&initial_tty,'\0',sizeof(initial_tty)); 
 
 if((modemfd = open(gpppdata.modemDevice(), O_RDWR|O_NDELAY)) < 0){

    statusbar->setText("Can't open Modem");
    return FALSE;
  }
 
  if(tcgetattr(modemfd, &tty) < 0){

    statusbar->setText("tcgetattr() failed\n");
    return FALSE;
  }

  initial_tty = tty; // save a copy

  tty.c_cc[VMIN] = 0; // nonblocking 
  tty.c_cc[VTIME] = 0;
  tty.c_oflag = 0;
  tty.c_lflag = 0;

  // clearing CLOCAL as below ensures we observe the modem status lines
  //  tty.c_cflag &= ~(CSIZE | CSTOPB | PARENB | CLOCAL);  
 
 
  tty.c_cflag &= ~(CSIZE | CSTOPB | PARENB);
  tty.c_cflag |= CLOCAL ; //ignore modem satus lines
  tty.c_oflag &= ~OPOST; //no outline processing -- transparent output
 
  tty.c_cflag |= CS8 | CREAD;       
  tty.c_iflag = IGNBRK | IGNPAR | ISTRIP;       // added ISTRIP
  tty.c_lflag &= ~ICANON;  			// non-canonical mode
  tty.c_lflag &= ~(ECHO|ECHOE|ECHOK|ECHOKE);

 
  if(strcmp(gpppdata.flowcontrol(), "None") != 0) {
    if(strcmp(gpppdata.flowcontrol(), "CRTSCTS") == 0) {
      tty.c_cflag |= CRTSCTS;
      tty.c_iflag &= ~IXOFF;
    }
    else {
      tty.c_cflag &= ~CRTSCTS;
      tty.c_iflag |= IXOFF;
      tty.c_cc[VSTOP]  = 0x13; // DC3 = XOFF = ^S 
      tty.c_cc[VSTART] = 0x11; // DC1 = XON  = ^Q 
    }
  }
  else {
    tty.c_cflag &= ~CRTSCTS;
    tty.c_iflag &= ~IXOFF;
  }

  cfsetospeed(&tty, modemspeed());
  cfsetispeed(&tty, modemspeed());

  if(tcsetattr(modemfd, TCSANOW, &tty) < 0){

    statusbar->setText("tcsetattr() failed\n");
    return FALSE;
  }

  return TRUE;
}
		


speed_t MiniTerm::modemspeed() {

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



void MiniTerm::hangup() {

  struct termios temptty;

  if(modemfd >= 0) {

    // Properly bracketed escape code  
    tcflush(modemfd,TCOFLUSH);
    // +3 because quiet time must be greater than guard time.
    usleep((gpppdata.modemEscapeGuardTime()+3)*20000);

    write(modemfd, gpppdata.modemEscapeStr(), strlen(gpppdata.modemEscapeStr()) );  

    tcflush(modemfd,TCOFLUSH);
    usleep((gpppdata.modemEscapeGuardTime()+3)*20000);

    // Then hangup command
    writeline(gpppdata.modemHangupStr());
    
    if(gpppdata.FastModemInit())
      usleep(10000); // 0.01 sec
    else
      usleep(1000000); // 1 sec

    tcsendbreak(modemfd, 0);

    tcgetattr(modemfd, &temptty);
    cfsetospeed(&temptty, B0);
    cfsetispeed(&temptty, B0);
    tcsetattr(modemfd, TCSAFLUSH, &temptty);

    usleep(10000); // wait 0.01 secs 

    cfsetospeed(&temptty, modemspeed());
    cfsetispeed(&temptty, modemspeed());
    tcsetattr(modemfd, TCSAFLUSH, &temptty);
   
  }


}


bool MiniTerm::writeChar(char c){

  write(modemfd,&c,1);
  return true;

}

bool MiniTerm::writeline(const char *buf) {


  write(modemfd, buf, strlen(buf));

  if(strcmp(gpppdata.enter(), "CR/LF") == 0)
    write(modemfd, "\r\n", 2);
 
  if(strcmp(gpppdata.enter(), "LF") == 0)
    write(modemfd, "\n", 1);
 
  if(strcmp(gpppdata.enter(), "CR") == 0)
    write(modemfd, "\r", 1);

  return true;
}



void MiniTerm::closeEvent( QCloseEvent *e ){

  e->ignore();     // don't let the user close the window

}

void MiniTerm::help(){

  app->invokeHTMLHelp("kppp/kppp.html","");

}


MyTerm::MyTerm(QWidget *parent=0 ,const char* name=0)
  : QMultiLineEdit(parent, name)
{
   p_parent =   (MiniTerm*)  parent;
   this->setFont(QFont("courier",12,QFont::Normal));
  
}

void MyTerm::keyPressEvent(QKeyEvent *k) {


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




















