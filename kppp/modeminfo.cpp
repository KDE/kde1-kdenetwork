/*
 *            kPPP: A front end for pppd for the KDE project
 *
 * $Id$
 * 
 * Copyright (C) 1997 Bernd Johannes Wuebben 
 *                    wuebben@math.cornell.edu
 *
 * This file contributed by: Markus Wuebben, mwuebben@fiwi02.wiwi.uni-tuebingen.de
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
#include <qregexp.h> 
#include "main.h"
#include "modeminfo.h"
#include "connect.h"
#include <kmsgbox.h>

#ifdef NO_USLEEP
extern int usleep( long usec );
#endif 

extern XPPPWidget *p_xppp;
extern KApplication *app;
extern QString ati_query_strings[NUM_OF_ATI];


ModemTransfer::ModemTransfer(QWidget *parent=0, const char *name=0)
  : QDialog(parent, name,TRUE, WStyle_Customize|WStyle_NormalBorder)
{

  setCaption("ATI Query");
  
  setMaximumSize(250,130);
  setMinimumSize(250,130);
  
  progressBar = new Progress(0,8,0,Progress::Horizontal,this,"bar");
  progressBar->setGeometry(20,20,210,25);

  statusBar = new QLabel(this,"sBar");
  statusBar->setFrameStyle(QFrame::Panel|QFrame::Sunken);
  statusBar->setGeometry(20,60,210,25);
  statusBar->setAlignment(AlignCenter);
  statusBar->setText("Looking for Modem ...");

  cancel = new QPushButton("Cancel", this);
  cancel->setGeometry(160, 95, 70, 30);
  cancel->setFocus();
  connect(cancel, SIGNAL(clicked()), SLOT(cancelbutton()));

  connect(this, SIGNAL(ati_done()),SLOT(ati_done_slot()));

  modemfd = -1;
  expecting = false;
  step = 0;
  main_timer_ID = -1;
  ati_counter = 1;

  ////////////////////////////////////////////////

  inittimer = new QTimer(this);
  connect(inittimer, SIGNAL(timeout()), SLOT(init()));

  timeout_timer = new QTimer(this);
  connect(timeout_timer, SIGNAL(timeout()), SLOT(time_out_slot()));

  readtimer = new QTimer(this);
  connect(readtimer, SIGNAL(timeout()), SLOT(readtty()));

  scripttimer = new QTimer(this);
  connect(scripttimer, SIGNAL(timeout()), SLOT(do_script()));

  timeout_timer->start(15000,TRUE); // 15 secs singel shot
  inittimer->start(500);

}  



void ModemTransfer::ati_done_slot(){

  readtimer->stop();
  scripttimer->stop();
  timeout_timer->stop();
  closetty();
  unlockdevice();
  accept();

}

void ModemTransfer::time_out_slot(){

  timeout_timer->stop();
  readtimer->stop();
  scripttimer->stop();

  QMessageBox::warning(this, "Error","Modem Query timed out.");
  reject();  

}
	  


void ModemTransfer::init() {

  inittimer->stop();
  expecting = false;

  app->processEvents();

  if (!lockdevice()){
    
    statusBar->setText("Sorry, modem device is locked");
    return;
  }

  if(opentty()){

    if(modemfd >= 0) {
      writeline(gpppdata.modemHangupStr());
      usleep(100000);  // wait 0.1 secs
      hangup();
      usleep(100000);  // wait 0.1 secs
      writeline("ATE0Q1V1"); // E0 don't echo the commands I send ...
    }

    statusBar->setText("Modem Ready");
    app->processEvents();

    app->processEvents();
    //    readtimer->start(1); 		// this one read from the modem
    scripttimer->start(1000);	 	// this one does the ati query

  }
  else {
    // commeted out the next line because we let opentty set the status appropriately
    //    statusBar->setText("Can't open modem device");
    step = 99; // wait until cancel is pressed
    unlockdevice();
  }
}                  




void ModemTransfer::do_script() {

  //  if(!expecting){
    if(step == 0) {
      readtty();
      statusBar->setText("ATI ...");
      progressBar->advance(1);
      writeline("ATI");
      //usleep(10000); // wait 0.01 secs
      //      setExpect("\r\n\r\nOK");
      step ++;
      return;
    }

    if(step == 1) {
      readtty();
      statusBar->setText("ATI 1 ...");
      progressBar->advance(1);
      writeline("ATI1");
      //usleep(10000); // wait 0.01 secs
      //      setExpect("\r\n\r\nOK");
      return;
    }

    if(step == 2) {
      readtty();
      statusBar->setText("ATI 2 ...");
      progressBar->advance(1);
      writeline("ATI2");
      //usleep(10000); // wait 0.01 secs
      //      setExpect("\r\n\r\nOK");
      return;
    }

    if(step == 3) {
      readtty();
      statusBar->setText("ATI 3 ...");
      progressBar->advance(1);
      writeline("ATI3");
      //      usleep(10000); // wait 0.01 secs
      //setExpect("\r\n\r\nOK");
      return;
    }

    if(step == 4) {
      readtty();
      statusBar->setText("ATI 4 ...");
      progressBar->advance(1);
      writeline("ATI4");
      //      usleep(10000); // wait 0.01 secs
      //setExpect("\r\n\r\nOK");
      return;
    }

    if(step == 5) {
      readtty();
      statusBar->setText("ATI 5 ...");
      progressBar->advance(1);
      writeline("ATI5");
      //      usleep(10000); // wait 0.01 secs
      //setExpect("\r\n\r\nOK");
      return;
    }

    if(step == 6) {
      readtty();
      statusBar->setText("ATI 6 ...");
      progressBar->advance(1);
      writeline("ATI6");
      //usleep(10000); // wait 0.01 secs
      //setExpect("\r\n\r\nOK");
      return;
    }

    if(step == 7) {
      readtty();
      statusBar->setText("ATI 7 ...");
      progressBar->advance(1);
      writeline("ATI7");
      //usleep(10000); // wait 0.01 secs
      //setExpect("\r\n\r\nOK");
      return;
    }
    readtty();
    emit ati_done();
    //  }
}


void ModemTransfer::readtty() {

  //  char c;
  char buffer[255];

  memset(buffer,'\0',254);
  read(modemfd, buffer, 255);

  if (step == 0)
    return;
  //    c = ((int)c & 0x7F);
  //  readbuffer += c;
    //    printf("%x %c||",c,c);
  //  }

  readbuffer = buffer;

  /*  if(expecting) {

    if(readbuffer.contains(expectstr)) {
      //      printf("Read-buffer:%s\n",readbuffer.data());
      expecting = false;

      QString number;
      number.sprintf("ATI%d",ati_counter);
      
      //      readbuffer.replace(number.data(),""); // remove the echoed ATI command
      readbuffer.replace("\r\n\r\nOK","");  // remove the very last OK
      */
  
  //    if(readbuffer.length() >4)
  //  readbuffer = readbuffer.left(readbuffer.length() - 4);
    
    readbuffer.replace("\n"," ");         // remove stray \n
    readbuffer.replace("\r","");          // remove stray \r
    readbuffer = readbuffer.stripWhiteSpace(); // strip of leading or trailing white
                                                 // space

//    printf("%s\n",readbuffer.data());
    if(step < NUM_OF_ATI + 1){
	ati_query_strings[step-1] = readbuffer.copy();
//	printf("step:%d ::%s\n",step,readbuffer.data());
    }
      readbuffer = "";
      ati_counter ++;
      step ++;

      //      if(step > (NUM_OF_ATI - 1))
      //	emit ati_done();
      // }
      //}
}


void ModemTransfer::cancelbutton() {

  scripttimer->stop();
  readtimer->stop();
  timeout_timer->stop();

  statusBar->setText("One Moment Please ...");
  app->processEvents();
  
  if(modemfd >= 0) {
    writeline(gpppdata.modemHangupStr());
    usleep(10000); // 0.01 sec
    hangup();
  }

  closetty();
  unlockdevice();
  reject();
}


void ModemTransfer::setExpect(const char *n) {

  expectstr = n;
  expecting = true;

}


bool ModemTransfer::closetty(){

  if(modemfd > 0)
    tcsetattr(modemfd, TCSANOW, &initial_tty);
    ::close(modemfd);
  return TRUE;
}


bool ModemTransfer::opentty() {

  if((modemfd = open(gpppdata.modemDevice(), O_RDWR|O_NDELAY)) < 0){

    statusBar->setText("Can't open Modem");
    return FALSE;
  }

  if(tcgetattr(modemfd, &tty) < 0){

    statusBar->setText("Sorry, the modem is busy.");
    return FALSE;
  }

  memset(&initial_tty,'\0',sizeof(initial_tty));

  initial_tty = tty;

  tty.c_cc[VMIN] = 0; // nonblocking 
  tty.c_cc[VTIME] = 0;
  tty.c_oflag = 0;
  tty.c_lflag = 0;

  // clearing CLOCAL as below ensures we observe the modem status lines
  tty.c_cflag &= ~(CSIZE | CSTOPB | PARENB | CLOCAL);  
  tty.c_cflag |= CS8 | CREAD;       
  tty.c_iflag = IGNBRK | IGNPAR | ISTRIP;  // added ISTRIP
  tty.c_lflag &= ~ICANON;
  tty.c_lflag &= ~(ECHO|ECHOE|ECHOK|ECHOKE);

  // flow control 
  if(strcmp(gpppdata.flowcontrol(), "None") != 0) {
    if(strcmp(gpppdata.flowcontrol(), "CRTSCTS") == 0) {
      tty.c_cflag |= CRTSCTS;
    }
    else {
      tty.c_iflag |= IXOFF;
      tty.c_cc[VSTOP]  = 0x13; /* DC3 = XOFF = ^S */
      tty.c_cc[VSTART] = 0x11; /* DC1 = XON  = ^Q */
    }
  }
  else {
    tty.c_cflag &= ~CRTSCTS;
    tty.c_iflag &= ~IXOFF;
  }

  cfsetospeed(&tty, modemspeed());
  cfsetispeed(&tty, modemspeed());

  if(tcsetattr(modemfd, TCSANOW, &tty) < 0){
    statusBar->setText("Sorry, the modem is busy");
    return FALSE;
  }

  return TRUE;
}
		


speed_t ModemTransfer::modemspeed() {

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



void ModemTransfer::hangup() {

  struct termios temptty;

  // TODO:
  // does temptty need to be memset with zeroes ?????


  tcsendbreak(modemfd, 0);
  tcgetattr(modemfd, &temptty);
  cfsetospeed(&temptty, B0);
  cfsetispeed(&temptty, B0);
  tcsetattr(modemfd, TCSAFLUSH, &temptty);

  usleep(100000); // wait 0.01 secs 

  cfsetospeed(&temptty, modemspeed());
  cfsetispeed(&temptty, modemspeed());
  tcsetattr(modemfd, TCSAFLUSH, &temptty);

}



bool ModemTransfer::writeline(const char *buf) {


  write(modemfd, buf, strlen(buf));

  if(strcmp(gpppdata.enter(), "CR/LF") == 0)
    write(modemfd, "\r\n", 2);
 
  if(strcmp(gpppdata.enter(), "LF") == 0)
    write(modemfd, "\n", 1);
 
  if(strcmp(gpppdata.enter(), "CR") == 0)
    write(modemfd, "\r", 1);
 
  return true;
}



void ModemTransfer::closeEvent( QCloseEvent *e ){

  e->ignore();     // don't let the user close the window

}


ModemInfo::ModemInfo(QWidget *parent=0 ,const char* name=0)
  : QDialog(parent, name, TRUE, WStyle_Customize|WStyle_NormalBorder)
{
  QString label_text;

  setCaption("Modem Query Results");

  box = new QGroupBox(this, "box");
  box->setGeometry(5,5,330,360);

  ok = new QPushButton("OK", this);
  ok->setGeometry(260, 375, 70, 30);
  connect(ok, SIGNAL(clicked()), SLOT(okbutton()));
  
  for(int  i = 0 ; i< NUM_OF_ATI ; i++){

    label_text = "";
    if ( i == 0)
      label_text.sprintf("ATI :");
    else
      label_text.sprintf("ATI %d:",i );

    ati_label[i] = new QLabel(label_text.data(),this);
    ati_label[i]->setGeometry(20,30 + i* 40,50,25);

    ati_label_result[i] =  new QLineEdit(this);
    ati_label_result[i]->setGeometry(70,30 + i* 40 ,240,25);
    //    ati_label_result[i]->setFrameStyle(QFrame::WinPanel | QFrame::Sunken);
    //    ati_label_result[i]->setBackgroundColor(white);
    ati_label_result[i]->setText(ati_query_strings[i]);
  
  }

}

void ModemInfo::okbutton() {

  accept();  

}




























