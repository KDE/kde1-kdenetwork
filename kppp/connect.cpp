/*
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

#include <qdir.h>
#include <qregexp.h> 
#include <kmsgbox.h>

#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <errno.h>

#include "connect.h"
#include "main.h"
#include "config.h"
#include "macros.h"

#ifdef NO_USLEEP
int usleep( long usec );
#endif 

void parseargs(char* buf, char** args);

extern XPPPWidget *p_xppp;
extern KApplication *app;
extern int if_is_up();
extern bool pppd_has_died;
extern bool reconnect_on_disconnect;
extern QString old_hostname;
extern QString local_ip_address;
extern bool quit_on_disconnect;

bool modified_hostname;

ConnectWidget::ConnectWidget(QWidget *parent, const char *name)
  : QWidget(parent, name)
{

  QVBoxLayout *tl = new QVBoxLayout(this, 8, 10);

  // initalize some important varibles

  vmain = 0;
  expecting = false;

  loopnest = 0;
  loopend = false;

  pausing = false;
  modem_in_connect_state = false;
  scriptindex = 0;
  myreadbuffer = "";
  main_timer_ID = 0;
  modemfd = -1;
  semaphore = false;
  modified_hostname = FALSE;

  QString tit = klocale->translate("Connecting to: ");
  setCaption(tit);

  QHBoxLayout *l0 = new QHBoxLayout(10);
  tl->addLayout(l0);
  l0->addSpacing(10);
  messg = new QLabel(this, "messg");
  messg->setFrameStyle(QFrame::Panel|QFrame::Sunken);
  messg->setAlignment(AlignCenter);
  messg->setText(klocale->translate("Sorry, can't create modem lock file."));  
  messg->setMinimumHeight(messg->sizeHint().height() + 5);
  int messw = (messg->sizeHint().width() * 12) / 10;
  messw = QMAX(messw,280);
  messg->setMinimumWidth(messw);
  messg->setText(klocale->translate("Looking for Modem ..."));
  l0->addSpacing(10);
  l0->addWidget(messg);
  l0->addSpacing(10);

  QHBoxLayout *l1 = new QHBoxLayout(10);
  tl->addLayout(l1);
  l1->addStretch(1);
  
  debug = new QPushButton(klocale->translate("Log"), this);
  FIXED_HEIGHT(debug);
  connect(debug, SIGNAL(clicked()), SLOT(debugbutton()));

  cancel = new QPushButton(klocale->translate("Cancel"), this);
  FIXED_HEIGHT(cancel);
  cancel->setFocus();
  connect(cancel, SIGNAL(clicked()), SLOT(cancelbutton()));

  int maxw = MAX(cancel->sizeHint().width(),
		 debug->sizeHint().width());
  maxw = QMAX(maxw,65);
  debug->setFixedWidth(maxw);
  cancel->setFixedWidth(maxw);
  l1->addWidget(debug);
  l1->addWidget(cancel);

  tl->freeze();
  
  pausetimer = new QTimer(this);
  connect(pausetimer, SIGNAL(timeout()), SLOT(pause()));

  app->processEvents();

  readtimer = new QTimer(this);
  connect(readtimer, SIGNAL(timeout()), SLOT(readtty()));

  timeout_timer = new QTimer(this);
  connect(timeout_timer, SIGNAL(timeout()), SLOT(script_timed_out()));
  
  inittimer = new QTimer(this);
  connect(inittimer, SIGNAL(timeout()), SLOT(init()));

  if_timeout_timer = new QTimer(this);
  connect(if_timeout_timer, SIGNAL(timeout()), SLOT(if_waiting_timed_out()));
  
  connect(this,SIGNAL(if_waiting_signal()),this,SLOT(if_waiting_slot()));

  prompt = new PWEntry( this, "pw" );         
  if_timer = new QTimer(this);
  connect(if_timer,SIGNAL(timeout()),SLOT(if_waiting_slot()));
}


ConnectWidget::~ConnectWidget() {

}

void ConnectWidget::preinit() {
  
  // this is all just to keep the GUI nice and snappy ....
  // you have to see to believe ...

  messg->setText(klocale->translate("Looking for Modem ..."));
  inittimer->start(500);

}


void ConnectWidget::init() {

  pppd_has_died = false;
  inittimer->stop();
  vmain = 0;
  expecting = false;
  pausing = false;
  scriptindex = 0;
  myreadbuffer = "";
  firstrunID = true;
  firstrunPW = true;

  p_xppp->con_speed = "";
  
  reconnect_on_disconnect = gpppdata.get_automatic_redial();

  QString tit = klocale->translate("Connecting to: ");
  tit += gpppdata.accname();
  setCaption(tit);

  app->processEvents();

  int lock = lockdevice();
  if (lock == 1){
    
    messg->setText(klocale->translate("Sorry, modem device is locked."));
    vmain = 20; // wait until cancel is pressed
    return;
  }
  if (lock == -1){
    
    messg->setText(klocale->translate("Sorry, can't create modem lock file."));
    vmain = 20; // wait until cancel is pressed
    return;
  }

  if(opentty()){
    messg->setText(klocale->translate("Modem Ready"));
    app->processEvents();

    hangup();
    
    app->processEvents();

    // this timer reads from the modem
    semaphore = false;
    readtimer->start(1);
    
    // if we are stuck anywhere we will time out
    timeout_timer->start(atoi(gpppdata.modemTimeout())*1000); 
      
    // this timer will run the script etc.
    main_timer_ID = startTimer(1);
  }
  else { 
    vmain = 20; // wait until cancel is pressed
    unlockdevice();
  }
}                  


void ConnectWidget::timerEvent(QTimerEvent *t) {

  (void) t;

  if (semaphore || pausing)
    return;

  if(vmain == 0) {
    messg->setText(klocale->translate("Initializing Modem..."));
    p_xppp->debugwindow->statusLabel(klocale->translate("Initializing Modem..."));

    writeline(gpppdata.modemInitStr());

    if(gpppdata.FastModemInit())
      usleep(10000); // 0.01 sec
    else
      usleep(1000000); // 1 sec


    setExpect(gpppdata.modemInitResp());
    vmain = 1;

    return;
  }

  // dial the number and wait to connect

  if(vmain == 1) {
    if(!expecting) {

      timeout_timer->stop();
      timeout_timer->start(atoi(gpppdata.modemTimeout())*1000);

      QString bm = klocale->translate("Dialing");;
      bm += " "; 
      bm += gpppdata.phonenumber();
      messg->setText(bm);
      p_xppp->debugwindow->statusLabel(bm);

      QString pn = gpppdata.modemDialStr();
      pn += gpppdata.phonenumber();
      writeline(pn);
      
      setExpect(gpppdata.modemConnectResp());
      vmain = 100;  
      return;
    }
  }

  // wait for connect, but redial if BUSY or wait for user cancel
  // if NO CARRIER or NO DIALTONE

  if(vmain == 100) {
    if(!expecting) {
      setExpect("\n");
      vmain = 101;
      return;
    }

    if(readbuffer.contains(gpppdata.modemBusyResp())) {      
      timeout_timer->stop();
      timeout_timer->start(atoi(gpppdata.modemTimeout())*1000);

      messg->setText(klocale->translate("Line Busy. Hanging up ..."));
      p_xppp->debugwindow->readchar('\n');
      hangup();

      if(gpppdata.busyWait() > 0){

	QString bm = klocale->translate("Line Busy. Waiting: ");
	bm += gpppdata.busyWait();
	bm += klocale->translate(" seconds");
	messg->setText(bm);
	p_xppp->debugwindow->statusLabel(bm);
      
	pausing = true;
      
	pausetimer->start(atoi(gpppdata.busyWait())*1000, true);
	timeout_timer->stop();
      }

      vmain = 0;
      return;
    }

    if(readbuffer.contains(gpppdata.modemNoDialtoneResp())) {

      timeout_timer->stop();

      messg->setText(klocale->translate("No Dialtone"));
      vmain = 20;
      unlockdevice();
      return;
    }

    if(readbuffer.contains(gpppdata.modemNoCarrierResp())) {

      timeout_timer->stop();

      messg->setText(klocale->translate("No Carrier"));
      vmain = 20;
      unlockdevice();
      return;
    }
  }

  // wait for newline after CONNECT response (so we get the speed)
  if(vmain == 101) {
    if(!expecting) {
      modem_in_connect_state=true; // modem will no longer respond to AT commands

      p_xppp->startAccounting();
      p_xppp->con_win->startClock();

      vmain = 2;
      scriptTimeout=atoi(gpppdata.modemTimeout())*1000;
      return;
    }
  }

  // execute the script

  if(vmain == 2) {
    if(!expecting && !pausing) {

      timeout_timer->stop();
      timeout_timer->start(scriptTimeout);

      if(!gpppdata.scriptType(scriptindex)) {
	vmain = 10;
        return;
      }

      if(strcmp(gpppdata.scriptType(scriptindex), "Send") == 0) {
	QString bm = klocale->translate("Sending ");
	bm += gpppdata.script(scriptindex);
	messg->setText(bm);
	p_xppp->debugwindow->statusLabel(bm);

	writeline(gpppdata.script(scriptindex));
	scriptindex++;
        return;
      }


      if(strcmp(gpppdata.scriptType(scriptindex), "Expect") == 0) {
        QString bm = klocale->translate("Expecting ");
        bm += gpppdata.script(scriptindex);
	messg->setText(bm);
	p_xppp->debugwindow->statusLabel(bm);

        setExpect(gpppdata.script(scriptindex));
	scriptindex++;
        return;
      }


      if(strcmp(gpppdata.scriptType(scriptindex), "Pause") == 0) {
	QString bm = klocale->translate("Pause ");
	bm += gpppdata.script(scriptindex);
	bm += klocale->translate(" seconds");
	messg->setText(bm);
	p_xppp->debugwindow->statusLabel(bm);
	
	pausing = true;
	
	pausetimer->start(atoi(gpppdata.script(scriptindex))*1000, true);
	timeout_timer->stop();
	
	scriptindex++;
	return;
      }

      if(strcmp(gpppdata.scriptType(scriptindex), "Timeout") == 0) {

	timeout_timer->stop();

	QString bm = klocale->translate("Timeout ");
	bm += gpppdata.script(scriptindex);
	bm += klocale->translate(" seconds");
	messg->setText(bm);
	p_xppp->debugwindow->statusLabel(bm);
	
	scriptTimeout=atoi(gpppdata.script(scriptindex))*1000;
        timeout_timer->start(scriptTimeout);
	
	scriptindex++;
	return;
      }

      if(strcmp(gpppdata.scriptType(scriptindex), "Hangup") == 0) {
	messg->setText(klocale->translate("Hangup"));
	p_xppp->debugwindow->statusLabel(klocale->translate("Hangup"));

	writeline(gpppdata.modemHangupStr());
	setExpect(gpppdata.modemHangupResp());
	
	scriptindex++;
	return;
      }

      if(strcmp(gpppdata.scriptType(scriptindex), "Answer") == 0) {
	
	timeout_timer->stop();

	messg->setText(klocale->translate("Answer"));
	p_xppp->debugwindow->statusLabel(klocale->translate("Answer"));

	setExpect(gpppdata.modemRingResp());
	vmain = 150;
	return;
      }

      if(strcmp(gpppdata.scriptType(scriptindex), "ID") == 0) {
	QString bm = klocale->translate("ID ");
	bm += gpppdata.script(scriptindex);
	messg->setText(bm);
	p_xppp->debugwindow->statusLabel(bm);

	QString idstring = gpppdata.Id();
	
	if(!idstring.isEmpty() && firstrunID){
	  // the user entered an Id on the main kppp dialog
	  writeline(idstring.data());
	  firstrunID = false;
	  scriptindex++;
	}
	else{
	  // the user didn't enter and Id on the main kppp dialog
	  // let's query for an ID
	     /* if not around yet, then post window... */
	     if (prompt->Consumed()) {
	       if (!(prompt->isVisible())) {
		 prompt->setPrompt(gpppdata.script(scriptindex));
		 prompt->setEchoModeNormal();
		 prompt->show();
	       }
	     } else {
	       /* if prompt withdrawn ... then, */
	       if(!(prompt->isVisible())) {
		 writeline(prompt->text());
		 prompt->setConsumed();
		 scriptindex++;
		 return;
	       }
	       /* replace timeout value */
	     }
	}
      }

      if(strcmp(gpppdata.scriptType(scriptindex), "Password") == 0) {
	QString bm = klocale->translate("Password ");
	bm += gpppdata.script(scriptindex);
	messg->setText(bm);
	p_xppp->debugwindow->statusLabel(bm);

	QString pwstring = gpppdata.Password();
	
	if(!pwstring.isEmpty() && firstrunPW){
	  // the user entered an Id on the main kppp dialog
	  writeline(pwstring.data());
	  firstrunPW = false;
	  scriptindex++;
	}
	else{
	  // the user didn't enter and Id on the main kppp dialog
	  // let's query for an ID
	     /* if not around yet, then post window... */
	     if (prompt->Consumed()) {
	       if (!(prompt->isVisible())) {
		 prompt->setPrompt(gpppdata.script(scriptindex));
		 prompt->setEchoModePassword();
		 prompt->show();
	       }
	     } else {
	       /* if prompt withdrawn ... then, */
	       if(!(prompt->isVisible())) {
		 writeline(prompt->text());
		 prompt->setConsumed();
		 scriptindex++;
		 return;
	       }
	       /* replace timeout value */
	     }
	}
      }
 
      if(strcmp(gpppdata.scriptType(scriptindex), "Prompt") == 0) {
	QString bm = klocale->translate("Prompting ");
	bm += gpppdata.script(scriptindex);
	messg->setText(bm);
	p_xppp->debugwindow->statusLabel(bm);

	/* if not around yet, then post window... */
	if (prompt->Consumed()) {
	   if (!(prompt->isVisible())) {
		prompt->setPrompt(gpppdata.script(scriptindex));
		prompt->setEchoModeNormal();
	        prompt->show();
	   }
	} else {
	    /* if prompt withdrawn ... then, */
	    if (!(prompt->isVisible())) {
	    	writeline(prompt->text());
	        prompt->setConsumed();
	        scriptindex++;
        	return;
	    }
	    /* replace timeout value */
	}
      }

      if(strcmp(gpppdata.scriptType(scriptindex), "PWPrompt") == 0) {
	QString bm = klocale->translate("PW prompt ");
	bm += gpppdata.script(scriptindex);
	messg->setText(bm);
	p_xppp->debugwindow->statusLabel(bm);

	/* if not around yet, then post window... */
	if (prompt->Consumed()) {
	   if (!(prompt->isVisible())) {
		prompt->setPrompt(gpppdata.script(scriptindex));
		prompt->setEchoModePassword();
	        prompt->show();
	   }
	} else {
	    /* if prompt withdrawn ... then, */
	    if (!(prompt->isVisible())) {
	    	writeline(prompt->text());
	        prompt->setConsumed();
	        scriptindex++;
        	return;
	    }
	    /* replace timeout value */
	}
      }

      if(strcmp(gpppdata.scriptType(scriptindex), "LoopStart") == 0) {

        QString bm = klocale->translate("LoopStart ");
        bm += gpppdata.script(scriptindex);

	if ( loopnest > (MAXLOOPNEST-2) ) {
		bm += klocale->translate("ERROR: Nested too deep, ignored.");
		vmain=20;
		scriptindex++;
		cancelbutton();
	        QMessageBox::warning( 0, klocale->translate("Error"),
				      klocale->translate("Loops nested too deeply!"));
	} else {
        	setExpect(gpppdata.script(scriptindex));
		loopstartindex[loopnest] = scriptindex + 1;
		loopstr[loopnest] = gpppdata.script(scriptindex);
		loopend = false;
		loopnest++;
	}
	messg->setText(bm);
	p_xppp->debugwindow->statusLabel(bm);

	scriptindex++;
      }

      if(strcmp(gpppdata.scriptType(scriptindex), "LoopEnd") == 0) {
        QString bm = "LoopEnd ";
        bm += gpppdata.script(scriptindex);
	if ( loopnest <= 0 ) {
		bm = klocale->translate("LoopEnd without mathing Start! Line: ") + bm ;
		vmain=20;
		scriptindex++;
		cancelbutton();
	        QMessageBox::warning( 0, klocale->translate("Error"), bm );
		return;
	} else {
        	setExpect(gpppdata.script(scriptindex));
		loopnest--;
		loopend = true;
	}
	messg->setText(bm);
	p_xppp->debugwindow->statusLabel(bm);

	scriptindex++;
      }
   }
  }

  // this is a subroutine for the "Answer" script option

  if(vmain == 150) {
    if(!expecting) {
      writeline(gpppdata.modemAnswerStr());
      setExpect(gpppdata.modemAnswerResp());

      vmain = 2;
      scriptindex++;
      return;
    }
  }


  if(vmain == 10) {   // was 12
    if(!expecting) {

      timeout_timer->stop();
      if_timeout_timer->stop(); // better be sure.

      if_timeout_timer->start(atoi(gpppdata.pppdTimeout())*1000);

#ifdef MY_DEBUG
 printf("started if timeout timer with %d\n",atoi(gpppdata.pppdTimeout())*1000);
#endif

     
      readtimer->stop();
      killTimer( main_timer_ID );
      
      int result;
      app->flushX();
      semaphore = true;
      result = execppp();

      p_xppp->debugwindow->statusLabel(klocale->translate("Starting pppd ..."));

#ifdef MY_DEBUG
      printf("execppp() returned with return-code %d\n", result);
#endif

      if(result) {
	adddns(); 

	// O.K we are done here, let's change over to the if_waiting loop
	// where we wait for the ppp if (interface) to come up.

	emit if_waiting_signal();
      }
      else {

	// starting pppd wasn't successful. Error messages were 
	// handled by execppp();

	if_timeout_timer->stop();
	this->hide();
	messg->setText("");
	p_xppp->quit_b->setFocus();
	p_xppp->show();
	app->processEvents();
	hangup();
	p_xppp->stopAccounting();
	p_xppp->con_win->stopClock();
	closetty();
        unlockdevice();

      }

      return;
    }
  }

  // this is a "wait until cancel" entry

  if(vmain == 20) {
  }

}


void ConnectWidget::set_con_speed_string(){

  // Here we are trying to determine the speed at which we are connected.
  // Usually the modem responds after connect with something like
  // CONNECT 115200, so all we need to do is find the number after CONNECT
  // or whatever the modemConnectResp() is.

  QString p,t;

  t = gpppdata.modemConnectResp();
  t = t.stripWhiteSpace();

  // if ConnectResp is empty I don't know how to find the speed..

  if (t.isEmpty()){
    p_xppp->con_speed = klocale->translate("unknown speed");
    return; 
  }

  //  t += "[ ]*[a-z0-9\x2F\x20]+[\n\r]";  // this one might be too ambitious
  //  t += "[ ]*[a-z0-9\x2F]+[\n\r]";      // try this one in case of trouble
  t += "[ ]*[a-zA-Z0-9/]+[\n\r]";
  
  // t should now contain something like
  // "CONNECT[ ]*[a-z0-9]*[\n\r]"
  // \x20 is a space ans \x2F is a forward slash /

  QRegExp r(t.data());              
   
  int len = 0;
  int start = 0;

  start = r.match(myreadbuffer, 0, &len);
  if (start != -1){
    p = myreadbuffer.mid(start + strlen(gpppdata.modemConnectResp()) ,
			 len - strlen(gpppdata.modemConnectResp()));      
    p = p.stripWhiteSpace();
    p_xppp->con_speed = p.copy();

  }
  else{
    p_xppp->con_speed = klocale->translate("unknown speed");
  }
}



void ConnectWidget::readtty() {

  char c;

  if(semaphore)
    return;

  if(read(modemfd, &c, 1) == 1) {
    c = ((int)c & 0x7F);
    readbuffer += c;
    myreadbuffer += c;
    p_xppp->debugwindow->readchar(c); 

  }

  if(expecting) {

    if(readbuffer.contains(expectstr)) {
      expecting = false;
      readbuffer = "";

      QString ts = klocale->translate("Found: ");
      ts += expectstr;
      p_xppp->debugwindow->statusLabel(ts);
      if (loopend) {
	loopend=false;
      }
      return;
    }
    if (loopend && readbuffer.contains(loopstr[loopnest])) {
      expecting = false;
      readbuffer = "";
      QString ts = klocale->translate("Looping: ");
      ts += loopstr[loopnest];
      p_xppp->debugwindow->statusLabel(ts);
      scriptindex = loopstartindex[loopnest];
      loopend = false;
      loopnest++;
    }
  }
  
}



void ConnectWidget::pause() {
  pausing = false;
  pausetimer->stop();
}



void ConnectWidget::cancelbutton() {

  readtimer->stop();
  killTimer(main_timer_ID);
  timeout_timer->stop();

  messg->setText(klocale->translate("One Moment Please ..."));
  
#ifdef MY_DEBUG
printf( "ConnectWidget::cancelbutton() \n" );
#endif
  app->processEvents();
  
  hangup();

  this->hide();
  messg->setText("");
  p_xppp->quit_b->setFocus();
  p_xppp->show();
  p_xppp->stopAccounting();	// just to be sure
  p_xppp->con_win->stopClock();
  closetty();
  unlockdevice();

  //abort prompt window...
  if (prompt->isVisible()) {
  	prompt->hide();
  }
  prompt->setConsumed();

  if(quit_on_disconnect) 
    kapp->exit(0);
}


void ConnectWidget::script_timed_out(){

  if(vmain == 20){ // we are in the 'wait for the user to cancel' state
    timeout_timer->stop();
      p_xppp->stopAccounting();
      p_xppp->con_win->stopClock();
    return;
  }

  if (prompt->isVisible()) {
  	prompt->hide();
  }
  prompt->setConsumed();

  messg->setText(klocale->translate("Script timed out!"));
  
  hangup();
  p_xppp->stopAccounting();
  p_xppp->con_win->stopClock();

  vmain = 0; // let's try again.

}

void ConnectWidget::debugbutton() {

  if(!p_xppp->debugwindow->isVisible()) { 
    p_xppp->debugwindow->show();
  }
  else {
    p_xppp->debugwindow->hide();
  }

}



void ConnectWidget::setExpect(const char *n) {
  expecting = true;
  expectstr = n;

  QString ts = klocale->translate("Expecting: ");
  ts += n;
  p_xppp->debugwindow->statusLabel(ts);
}

void ConnectWidget::if_waiting_timed_out(){


  if_timer->stop();
  if_timeout_timer->stop();

#ifdef MY_DEBUG
printf("if_waiting_timed_out()\n");
#endif

  // let's kill the stuck pppd
  killppp();

  p_xppp->stopAccounting();
  p_xppp->con_win->stopClock();

  
  // killing ppp will generate a SIGCHLD which will be caught in pppdie()
  // in main.cpp what happens next will depend on the boolean 
  // reconnect_on_disconnect which is set in ConnectWidget::init();

}


void ConnectWidget::if_waiting_slot(){


  messg->setText(klocale->translate("Logging on to Network ..."));

  if(!if_is_up()){

    if(pppd_has_died){ // we are here if pppd died immediately after starting it.

      if_timer->stop();
      // error message handled in main.cpp: die_ppp()
      return;
    }

    if_timer->start(500,TRUE); // single shot 
    return;
  }

  // O.K the ppp interface is up and running

  if_timeout_timer->stop(); 
  if_timer->stop();

  p_xppp->stats->take_stats(); // start taking ppp statistics
  auto_hostname();

  if(!gpppdata.command()) {
    
    // let's fish the connection speed out of the read buffer.
    // so that we can say Connected at 115200 or similar.
    p_xppp->debugwindow->statusLabel(klocale->translate("Done"));
    
       
  }
  else { // need to run a command
    
    pid_t id;
    messg->setText(klocale->translate("Running Startup Command ..."));

    app->flushX(); /* make sure that we don't get any asyn errors*/

    if((id = fork()) == 0) {
      setuid(getuid());
      system(gpppdata.command());
      exit(0);
    }	 
    
    messg->setText(klocale->translate("Done"));
    p_xppp->debugwindow->statusLabel(klocale->translate("Done"));
    
      
  }

  set_con_speed_string();

  p_xppp->con_win->setConnectionSpeed();
  this->hide();
  messg->setText("");

  // prepare the con_win so as to have the right size for
  // accounting / non-accounting mode
  p_xppp->con_win->accounting(p_xppp->accounting.running());

  p_xppp->con_win->show();
  closetty();
	
}



bool ConnectWidget::closetty(){

  if(modemfd){
    
    if(tcsetattr(modemfd, TCSANOW, &initial_tty) < 0){
    }
    ::close(modemfd);
  }

  return TRUE;
  
}

bool ConnectWidget::opentty() {


  if((modemfd = open(gpppdata.modemDevice(), O_RDWR|O_NDELAY)) < 0){
    
    messg->setText(klocale->translate("Sorry, can't open modem."));
    return FALSE;
  }

  if(tcgetattr(modemfd, &tty) < 0){
    
    messg->setText(klocale->translate("Sorry, the modem is busy."));
    return FALSE;
  }

  memset(&initial_tty,'\0',sizeof(initial_tty));

  initial_tty = tty;

  tty.c_cc[VMIN] = 0; // nonblocking 
  tty.c_cc[VTIME] = 0;
  tty.c_oflag = 0;
  tty.c_lflag = 0;

  tty.c_cflag &= ~(CSIZE | CSTOPB | PARENB);  
  tty.c_cflag |= CS8 | CREAD | CLOCAL;       
  tty.c_iflag = IGNBRK | IGNPAR | ISTRIP;  // added ISTRIP
  tty.c_lflag &= ~ICANON;
  tty.c_lflag &= ~(ECHO|ECHOE|ECHOK|ECHOKE);


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
    messg->setText(klocale->translate("Sorry, the modem is busy."));
    return FALSE;
  }

  return TRUE;
}


speed_t ConnectWidget::modemspeed() {

  int i;

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



void ConnectWidget::escape_to_command_mode() {

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


void ConnectWidget::hangup() {


  // this should really get the modem to hang up and go into command mode
  // If anyone sees a fault in the following please let me know, since
  // this is probably the most imporant snippet of code in the whole of
  // kppp. If people complain about kppp being stuck, this piece of code
  // is most likely the reason.

  struct termios temptty;

  if(modemfd >= 0) {

    if ( modem_in_connect_state ) escape_to_command_mode(); 

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


bool ConnectWidget::writeline(const char *buf) {

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
 
  if(strcmp(gpppdata.enter(), "LF") == 0)
    write(modemfd, "\n", 1);
 
  if(strcmp(gpppdata.enter(), "CR") == 0)
    write(modemfd, "\r", 1);
 
  return true;
}


bool ConnectWidget::execppp() {

  pid_t id;

  QString command;
  char buf[2024];
  char *args[100];


  command = "pppd";

  command += " ";
  command += gpppdata.modemDevice();

  command += " " ;
  command += gpppdata.speed();


  if(strcmp(gpppdata.ipaddr(), "0.0.0.0") != 0 ||
     strcmp(gpppdata.gateway(), "0.0.0.0") != 0) {
    if(strcmp(gpppdata.ipaddr(), "0.0.0.0") != 0) {

      command += " "; 
      command += gpppdata.ipaddr();
      command +=  ":";
    }
    else {

      command += " ";
      command += ":";
    }

    if(strcmp(gpppdata.gateway(), "0.0.0.0") != 0)

      command += gpppdata.gateway();
  }


  if(strcmp(gpppdata.subnetmask(), "0.0.0.0") != 0) {

    command += " ";
    command += "netmask";
    command += " ";
    command += gpppdata.subnetmask();

  }


  if(strcmp(gpppdata.flowcontrol(), "None") != 0) {
    if(strcmp(gpppdata.flowcontrol(), "CRTSCTS") == 0) {

      command += " ";
      command +=  "crtscts";
    }
    else {

      command += " ";
      command += "xonxoff";
    }
  }

  if(gpppdata.defaultroute()) {
    command += " ";
    command +=  "defaultroute";
  }

  for(int i=0; gpppdata.pppdArgument(i); i++) {
    command += " ";
    command += gpppdata.pppdArgument(i);
  }


  if (command.length() > 2023){
    QMessageBox::warning(this, klocale->translate("Error"), 
		     klocale->translate("pppd command + command-line arguments exeed\n"
					"2024 characters in length. What are you doing?"));	

    return false; // nonsensically long command which would bust my buffer buf.
  }
  
  strcpy(buf,command.data());

  // let's parse the arguments the user supplied into UNIX suitable form
  // that is a list of pointers each pointing to exactly one word

  parseargs(buf,args);

  app->flushX();

  if((id = fork()) < 0)
    { 
#ifdef MY_DEBUG
      fprintf(stderr,"In parent: fork() failed\n");
#endif
      return false;
    }

  if(id != 0)
    {
#ifdef MY_DEBUG
      fprintf(stderr,"In parent: fork() %d\n",id);
#endif
       gpppdata.setpppdpid(id);
    
#ifdef MY_DEBUG
      printf("pppd pid %d\n",id);
#endif 
      return true;
    }

  if(id == 0) 
    {

      /*    printf("In child: fork() %d\n",id);*/
      /*    close(modemfd);*/

#ifdef MY_DEBUG
      printf("%s \n",command.data());
#endif

#if defined(__FreeBSD__) || defined(__NetBSD__) 
      setpgrp(0,0);    
#else
      setpgrp();
#endif
      execve(gpppdata.pppdPath(), args, '\0');
      exit(0);
  }

  return true;

}	



void ConnectWidget::closeEvent( QCloseEvent *e ){

        e->ignore();                            
	
	// We don't want to lose the conwindow since this is our last 
	// connection to kppp. If we lost it we would have to kill the 
	// program by hand to get on with life.
}


void killppp() {
  
#ifdef MY_DEBUG
printf("In killppp(): I will attempt to kill pppd\n");
#endif MY_DEBUG
 

  if(gpppdata.pppdpid() >= 0) {

#ifdef MY_DEBUG
printf("Sending SIGTERM to %d\n",gpppdata.pppdpid());
#endif MY_DEBUG

    if(kill(gpppdata.pppdpid(), SIGTERM) < 0)
#ifdef MY_DEBUG
printf("Error killing %d\n",gpppdata.pppdpid());
#endif MY_DEBUG
      qApp->beep();

  }



}



// Set the hostname and domain from DNS Server

void auto_hostname(){

  struct in_addr local_ip;
  struct hostent *hostname_entry;
  QString new_hostname;
  int    dot;
  char   tmp_str[100];

  gethostname(tmp_str, 100);
  old_hostname=tmp_str;

  if (!local_ip_address.isEmpty() && gpppdata.autoname()){
    local_ip.s_addr=inet_addr((const char*)local_ip_address);
    hostname_entry=gethostbyaddr((const char *)&local_ip,sizeof(in_addr),AF_INET);

    if (hostname_entry != NULL){
      new_hostname=hostname_entry->h_name;
      dot=new_hostname.find('.');
      new_hostname=new_hostname.remove(dot,new_hostname.length()-dot);
      sethostname (new_hostname,new_hostname.length());
      modified_hostname = TRUE;

      new_hostname=hostname_entry->h_name;
      new_hostname.remove(0,dot+1);
      add_domain(new_hostname);
    }
  }

}  

// Replace the DNS domain entry in the /etc/resolv.conf file and
// disable the nameserver entries if option is enabled
void add_domain(const char *domain) {

  int fd;
  char c;
  QString resolv[MAX_RESOLVCONF_LINES];

  if (domain == NULL || ! strcmp(domain, "")) return;

  if((fd = open("/etc/resolv.conf", O_RDONLY)) >= 0) {

    int i=0;
    while((read(fd, &c, 1) == 1) && (i < MAX_RESOLVCONF_LINES)) {
      if(c == '\n') {
	i++;
      }
      else {
	resolv[i] += c;
      }
    }
    close(fd);
    if ((c != '\n') && (i < MAX_RESOLVCONF_LINES)) i++;

    if((fd = open("/etc/resolv.conf", O_WRONLY|O_TRUNC)) >= 0) {

      write(fd, "domain ", 7);
      write(fd, domain, strlen(domain));
      write(fd, " \t\t#kppp temp entry\n", 20);

      for(int j=0; j < i; j++) {
	if((resolv[j].contains("domain") ||
	      ( resolv[j].contains("nameserver") 
		&& !resolv[j].contains("#kppp temp entry") 
		&& gpppdata.exDNSDisabled())) 
	        && !resolv[j].contains("#entry disabled by kppp")) {

          write(fd, "# ", 2);
	  write(fd, resolv[j], resolv[j].length());
          write(fd, " \t#entry disabled by kppp\n", 26);
	}
	else {
	  write(fd, resolv[j], resolv[j].length());
	  write(fd, "\n", 1);
	}
      }
    }
    close(fd);
  }
}  

// adds the DNS entries in the /etc/resolv.conf file
void adddns() {

  int fd;

  if((fd = open("/etc/resolv.conf", O_WRONLY|O_APPEND)) >= 0) {

    for(int i=0; gpppdata.dns(i); i++) {
      write(fd, "nameserver ", 11);
      write(fd, gpppdata.dns(i), strlen(gpppdata.dns(i)));
      write(fd, " \t#kppp temp entry\n", 19);
    }
    close(fd);
  }
  add_domain(gpppdata.domain());
}  


// remove the dns entries from the /etc/resolv.conf file
void removedns() {

  int fd;
  char c;
  QString resolv[30];
  extern QString old_hostname;

  if((fd = open("/etc/resolv.conf", O_RDONLY)) >= 0) {

    int i=0;
    while(read(fd, &c, 1) == 1) {
      if(c == '\n') {
	i++;
      }
      else {
	resolv[i] += c;
      }
    }
    close(fd);

    if((fd = open("/etc/resolv.conf", O_WRONLY|O_TRUNC)) >= 0) {
      for(int j=0; j < i; j++) {
	if(resolv[j].contains("#kppp temp entry")) continue;
	if(resolv[j].contains("#entry disabled by kppp")){
	  write(fd, ((const char*)resolv[j])+2, resolv[j].length() - 27);
	  write(fd, "\n", 1);
	}
	else{
	  write(fd, resolv[j], resolv[j].length());
	  write(fd, "\n", 1);
	}
      }
    }
    close(fd);

  }

  if (  modified_hostname ){
    sethostname ((const char*)old_hostname,old_hostname.length());
    modified_hostname = FALSE;
  }

}  

#ifdef NO_USLEEP

#include <sys/types.h>
#include <sys/time.h>

// usleep for those of you out there who don't have a BSD 4.2 style usleep

extern int select();

int usleep( long usec ){
  
  struct timeval tval;

  tval.tv_sec = microsecs/ 1000000;
  tval.tv_usec= microsecs% 1000000;
  return  select(0, NULL, NULL, NULL, &tval);

}

#endif /* NO_USLEPP */



void parseargs(char* buf, char** args){

  while(*buf != '\0'){
    
    // Strip whitespace. Use nulls, so that the previous argument is terminated 
    // automatically.
     
    while ((*buf == ' ' ) || (*buf == '\t' ) || (*buf == '\n' ) )
      *buf++ ='\0';
    
    // save the argument
    if(*buf != '\0')
    *args++ = buf;
    
    while ((*buf != '\0') && (*buf != '\n') && (*buf != '\t') && (*buf != ' '))
      buf++;
    
  }
 
  *args ='\0';;

}

// Lock modem device. Returns 0 on success 1 if the modem is locked and -1 if
// a lock file can't be created ( permission problem )

int lockdevice() {

  int fd;
  char c;
  QString oldlock="";
  QString procpid=PROC_DIR; /* "/proc" */
  char newlock[80]="";
  int start, len;
  QRegExp r("[1-9]+[0-9]*");

  QString lockfile;
  lockfile = gpppdata.modemLockFile();
  lockfile = lockfile.stripWhiteSpace();

  if(lockfile.isEmpty()){
#ifdef MY_DEBUG
    printf("gpppdata.modemLockFile is empty ..."\
	   "assuming the user doesn't want a lockfile.\n");
#endif
    return 0;
  }


  if (modem_is_locked) return 1;

  if ((fd = open(gpppdata.modemLockFile(), O_RDONLY)) >= 0) {

    while (read(fd, &c, 1) == 1)  oldlock+=c;
#ifdef MY_DEBUG
    printf("Device is locked by: %s\n",(const char*)oldlock);
#endif

#ifdef linux /* we will use /proc only on the Linux platform */

    oldlock.stripWhiteSpace();
    start=r.match(oldlock,0,&len);
    procpid+="/";
    procpid+=oldlock.mid(start,len);
    procpid.stripWhiteSpace();
    close(fd);

    if ((fd = open((const char*)procpid, O_RDONLY)) >= 0) {
      close(fd);
    }

    if ((errno != ENOENT) && (atoi(oldlock.mid(start,len))!=getpid()) ) return 1;

#else
    return 1;
#endif

  }

  if((fd = open(gpppdata.modemLockFile(), O_WRONLY|O_TRUNC|O_CREAT,0644)) >= 0) {
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

    unlink(gpppdata.modemLockFile());
    modem_is_locked=false;

#ifdef MY_DEBUG
   printf("UnLocking Modem Device\n");
#endif

  }

}  


#include "connect.moc"
