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
#include "main.h"
#include <sys/ioctl.h>
#include <termios.h>
#include <sys/types.h>
#include "miniterm.h"
#include "finishpic.h"
#include "resetpic.h"
#include "optionspic.h"
#include "helppic.h"
#include <kmsgbox.h>
#include "connect.h"

#define TOOLBAR_HEIGHT 26
#define TOOLBAR_X_OFFSET 10
#define TOOLBAR_Y_OFFSET 5
#define TOOLBAR_BUTTON_HEIGHT 28
#define TOOLBAR_BUTTON_WIDTH 28

#define BUTTON_HEIGHT		25
#define BUTTON_WIDTH		25
#define BUTTON_SEPARATION	6


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

  setCaption("kppp mini terminal");

  m_file = new QPopupMenu;
  m_file->insertItem( "&Quit",this, SLOT(cancelbutton()) );
  m_edit = new QPopupMenu;
  m_options = new QPopupMenu;
  m_options->insertItem("&Options");
  m_help = new QPopupMenu;
  m_help->insertItem( "&Help",this, SLOT(help()) );
  
  menubar = new QMenuBar( this );
  menubar->insertItem( "&File", m_file );
  //  menubar->insertItem( "&Edit", m_edit );
  menubar->insertItem( "&Options", m_options );
  menubar->insertItem( "&Help", m_help);
  
  statusbar = new QLabel(this);
  statusbar->setFrameStyle(QFrame::Panel | QFrame::Sunken);

  statusbar2 = new QLabel(this);
  statusbar2->setFrameStyle(QFrame::Panel | QFrame::Sunken);

  terminal = new MyTerm(this,"term");
  connect(terminal,SIGNAL(got_a_line()),this,SLOT(process_line()));

  pb1 = new QPushButton(this);
  QToolTip::add(pb1,"Options");
  //connect( pb1, SIGNAL( clicked() ), SLOT( options() ) );

  if ( !pb1_pixmap.loadFromData(options_xpm_data, options_xpm_len) ) {
    QMessageBox::warning(this, "Error", "Could not load option.xpm");
  }

  pb1->setPixmap( pb1_pixmap );

  pb2 = new QPushButton(this);
  QToolTip::add(pb2,"Quit Mini-Terminal");
  connect( pb2, SIGNAL( clicked() ), SLOT( cancelbutton() ) );

  if ( !pb2_pixmap.loadFromData(finish_xpm_data, finish_xpm_len) ) {
   QMessageBox::warning(this, "Error", "Could not load finish.xpm");
  }
  pb2->setPixmap( pb2_pixmap );

  pb3 = new QPushButton( this);
  QToolTip::add(pb3,"Reset Modem");

  connect( pb3, SIGNAL( clicked() ), SLOT( resetModem() ) );

  if ( !pb3_pixmap.loadFromData(reset_xpm_data, reset_xpm_len) ) {
    QMessageBox::warning(this, "Error", "Could not load reset.xpm");
  }
  pb3->setPixmap( pb3_pixmap );

  pb4 = new QPushButton( this);
  QToolTip::add(pb4,"Help");
  connect( pb4, SIGNAL( clicked() ), SLOT( help() ) );

  if ( !pb4_pixmap.loadFromData(help_xpm_data, help_xpm_len) ) {
    QMessageBox::warning(this, "Error", "Could not load help.xpm");
  }
  pb4->setPixmap( pb4_pixmap );


#define T_WIDTH 550
#define T_HEIGHT 400
#define B_XOFFSET 3
#define B_YOFFSET 2


  menubar->setGeometry(0,0,T_WIDTH,30);
  pb1->setGeometry( B_XOFFSET, menubar->height() + B_YOFFSET, 
		    BUTTON_WIDTH, BUTTON_HEIGHT );
  pb2->setGeometry( 2*B_XOFFSET + BUTTON_WIDTH, menubar->height() + B_YOFFSET, 
		    BUTTON_WIDTH, BUTTON_HEIGHT );
  pb3->setGeometry( 3*B_XOFFSET + 2*BUTTON_WIDTH, menubar->height() + B_YOFFSET, 
		    BUTTON_WIDTH, BUTTON_HEIGHT );
  pb4->setGeometry( 4*B_XOFFSET + 3*BUTTON_WIDTH, menubar->height() + B_YOFFSET, 
		    BUTTON_WIDTH, BUTTON_HEIGHT );

  terminal->setGeometry(0, menubar->height() + 2*B_YOFFSET + BUTTON_HEIGHT , 
   T_WIDTH,  T_HEIGHT - menubar->height() - 22 - 2*B_YOFFSET - BUTTON_HEIGHT);
  statusbar->setGeometry(0, T_HEIGHT - 20, T_WIDTH - 70, 20);
  statusbar2->setGeometry(T_WIDTH - 70, T_HEIGHT - 20, 70, 20);

  readtimer = new QTimer(this);
  connect(readtimer,SIGNAL(timeout()),this,SLOT(readtty()));

  inittimer = new QTimer(this);
  connect(inittimer,SIGNAL(timeout()),this,SLOT(init()));
  inittimer->start(500);

}  



void MiniTerm::process_line(){

  QString newline;
  newline = terminal->textLine(line);
  newline = newline.remove(0,col_start);
  newline = newline.stripWhiteSpace();

  //  printf("write:%s\n",newline.data());
  writeline(newline.data());

}

void MiniTerm::resizeEvent(QResizeEvent *e){

  (void) e;

  menubar->setGeometry(0,0,width(),30);
  pb1->setGeometry( B_XOFFSET, menubar->height() + B_YOFFSET, 
		    BUTTON_WIDTH, BUTTON_HEIGHT );
  pb2->setGeometry( 2*B_XOFFSET + BUTTON_WIDTH, menubar->height() + B_YOFFSET, 
		    BUTTON_WIDTH, BUTTON_HEIGHT );
  pb3->setGeometry( 3*B_XOFFSET + 2*BUTTON_WIDTH, menubar->height() + B_YOFFSET, 
		    BUTTON_WIDTH, BUTTON_HEIGHT );
  pb4->setGeometry( 4*B_XOFFSET + 3*BUTTON_WIDTH, menubar->height() + B_YOFFSET, 
		    BUTTON_WIDTH, BUTTON_HEIGHT );

  terminal->setGeometry(0, menubar->height() + 2*B_YOFFSET + BUTTON_HEIGHT , 
     width(),  height() - menubar->height() - 22 - 2*B_YOFFSET - BUTTON_HEIGHT);
  statusbar->setGeometry(0, height() - 20, width() - 70, 20);
  statusbar2->setGeometry(width() - 70, height() - 20, 70, 20);

}

void MiniTerm::init() {

  inittimer->stop();
  statusbar->setText("Initializing Modem");
  app->processEvents();

  if (!lockdevice()){
    
    statusbar->setText("Sorry, modem device is locked");
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
  //  statusBar->setText("One Moment Please ...");
  app->processEvents();
  
  if(modemfd >= 0) {
    writeline(gpppdata.modemHangupStr());
    usleep(100000); // 0.1 sec
    hangup();
  }

  closetty();
  unlockdevice();
  //  delete terminal;
  reject();
}



void MiniTerm::resetModem(){
 
  statusbar->setText("Resetting Modem");
  app->processEvents();
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
  return TRUE;
}


bool MiniTerm::opentty() {

  // just in case we get a modemfd
  // but can't tcgetattr and then call closetty()

  memset(&initial_tty,'\0',sizeof(initial_tty)); 


 if((modemfd = open(gpppdata.modemDevice(), O_RDWR|O_NDELAY)) < 0){

    statusbar->setText("Can't open Modem");
    return FALSE;
  }
 
 /* n = fcntl(modemfd, F_GETFL, 0);
 (void) fcntl(modemfd, F_SETFL, n & ~O_NDELAY);*/

  if(tcgetattr(modemfd, &tty) < 0){

    statusbar->setText("Can't tcgetattr()\n");
    return FALSE;
  }

  initial_tty = tty; // save a copy

  tty.c_cc[VMIN] = 0; // nonblocking 
  tty.c_cc[VTIME] = 0;
  tty.c_oflag = 0;
  tty.c_lflag = 0;

  // clearing CLOCAL as below ensures we observe the modem status lines
  //  tty.c_cflag &= ~(CSIZE | CSTOPB | PARENB | CLOCAL);  
 
  //experimental
  tty.c_cflag &= ~(CSIZE | CSTOPB | PARENB);
  tty.c_cflag |= CLOCAL ; //ignore modem satus lines
  tty.c_oflag &= ~OPOST; //no outline processing -- transparent output
  //end exerimental

  tty.c_cflag |= CS8 | CREAD;       
  tty.c_iflag = IGNBRK | IGNPAR | ISTRIP;       // added ISTRIP
  tty.c_lflag &= ~ICANON;  			// non-canonical mode
  tty.c_lflag &= ~(ECHO|ECHOE|ECHOK|ECHOKE);

  // flow control 
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

    statusbar->setText("Can't tcsetattr()\n");
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

  // TODO:
  // does temptty need to be memset with zeroes ?????


  tcsendbreak(modemfd, 0);
  tcgetattr(modemfd, &temptty);
  cfsetospeed(&temptty, B0);
  cfsetispeed(&temptty, B0);
  tcsetattr(modemfd, TCSAFLUSH, &temptty);

  usleep(100000); // wait 0.1 secs 

  cfsetospeed(&temptty, modemspeed());
  cfsetispeed(&temptty, modemspeed());
  tcsetattr(modemfd, TCSAFLUSH, &temptty);

}


bool MiniTerm::writeChar(char c){

  write(modemfd,&c,1);
  //  printf("%x\n",c);
  return true;
}

bool MiniTerm::writeline(const char *buf) {

  /*  struct termios tty;
  tcgetattr(modemfd, &tty);
  tty.c_lflag &= ~(ECHO);
  tcsetattr(modemfd,TCSANOW,&tty);
  */

  write(modemfd, buf, strlen(buf));

  if(strcmp(gpppdata.enter(), "CR/LF") == 0)
    write(modemfd, "\r\n", 2);
 
  if(strcmp(gpppdata.enter(), "LF") == 0)
    write(modemfd, "\n", 1);
 
  if(strcmp(gpppdata.enter(), "CR") == 0)
    write(modemfd, "\r", 1);

  //  ioctl(modemfd, TIOCFLUSH, (void *)0);
  //  ioctl(modemfd, TCFLSH, 2);
  return true;
}



void MiniTerm::closeEvent( QCloseEvent *e ){

  e->ignore();     // don't let the user close the window

}

void MiniTerm::help(){

  app->invokeHTMLHelp("kppp/kppp.html","");
  /*  if ( fork() == 0 )
        {
                QString path = DOCS_PATH;
                path += "/kppp.html";
                execlp( "kdehelp", "kdehelp", path.data(), 0 );
                ::exit( 1 );
        }
	*/
}


MyTerm::MyTerm(QWidget *parent=0 ,const char* name=0)
  : QMultiLineEdit(parent, name)
{
   p_parent =   (MiniTerm*)  parent;
   this->setFont(QFont("fixed",10,QFont::Normal));
  
}

void MyTerm::keyPressEvent(QKeyEvent *k) {


  if(k->ascii() == 13){
    myreturn();
    p_parent->writeChar((char) k->ascii());
    return;
  }

  if((k->ascii() ==8) || k->ascii() == 127){ //delete and backspace
    //    return;
  }
  p_parent->writeChar((char) k->ascii());

  //  QMultiLineEdit::keyPressEvent(k);

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
    //  setAutoUpdate(FALSE);
    getCursorPosition(&line,&column);
    for (int i = 0; i < column;i++)
      QMultiLineEdit::cursorLeft();
    //  setAutoUpdate(TRUE);
}

void MyTerm::mynewline() {
  
  //  setAutoUpdate(FALSE);
    QMultiLineEdit::end(FALSE);
    QMultiLineEdit::newLine();
    //setAutoUpdate(TRUE);

}




















