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

#include <config.h>

#include <qdir.h>
#include <qregexp.h> 
#include <kmsgbox.h>

#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/ioctl.h>

#include <errno.h>

#ifdef HAVE_SYS_PARAM_H
#include <sys/param.h>
#endif

#include "pap.h"
#include "chap.h"
#include "connect.h"
#include "main.h"
#include "kpppconfig.h"
#include "macros.h"
#include "docking.h"
#include "loginterm.h"
#include "log.h"
#include <qsocknot.h>

#define READ_TIMER 2

const int MAX_ARGS = 100;

void parseargs(char* buf, char** args);

extern KPPPWidget *p_kppp;
extern DockWidget *dock_widget;
extern int if_is_up();
extern bool pppd_has_died;
extern QString old_hostname;
extern QString local_ip_address;
extern bool quit_on_disconnect;

bool modified_hostname;

LoginTerm *termwindow = 0L;

extern int totalbytes;

ConnectWidget::ConnectWidget(QWidget *parent, const char *name)
  : QWidget(parent, name),
    // initialize some important variables
    myreadbuffer(""),
    main_timer_ID(0),
    vmain(0),  
    scriptindex(0),
    loopnest(0),
    loopend(false),
    semaphore(false),
    sn(0),
    expecting(false),
    readbuffer(""),
    scanvar(""),
    scanning(false),
    pausing(false),
    modem_in_connect_state(false),
    dialnumber(0)
{
  modemfd = -1;
  modified_hostname = false;
  
  QVBoxLayout *tl = new QVBoxLayout(this, 8, 10);
  QString tit = i18n("Connecting to: ");
  setCaption(tit);

  QHBoxLayout *l0 = new QHBoxLayout(10);
  tl->addLayout(l0);
  l0->addSpacing(10);
  messg = new QLabel(this, "messg");
  messg->setFrameStyle(QFrame::Panel|QFrame::Sunken);
  messg->setAlignment(AlignCenter);
  messg->setText(i18n("Sorry, can't create modem lock file."));  
  messg->setMinimumHeight(messg->sizeHint().height() + 5);
  int messw = (messg->sizeHint().width() * 12) / 10;
  messw = QMAX(messw,280);
  messg->setMinimumWidth(messw);
  messg->setText(i18n("Looking for Modem ..."));
  l0->addSpacing(10);
  l0->addWidget(messg);
  l0->addSpacing(10);

  QHBoxLayout *l1 = new QHBoxLayout(10);
  tl->addLayout(l1);
  l1->addStretch(1);
  
  debug = new QPushButton(i18n("Log"), this);
  FIXED_HEIGHT(debug);
  connect(debug, SIGNAL(clicked()), SLOT(debugbutton()));

  cancel = new QPushButton(i18n("Cancel"), this);
  FIXED_HEIGHT(cancel);
  cancel->setFocus();
  connect(cancel, SIGNAL(clicked()), SLOT(cancelbutton()));

  int maxw = QMAX(cancel->sizeHint().width(),
		 debug->sizeHint().width());
  maxw = QMAX(maxw,65);
  debug->setFixedWidth(maxw);
  cancel->setFixedWidth(maxw);
  l1->addWidget(debug);
  l1->addWidget(cancel);

  tl->freeze();
  
  pausetimer = new QTimer(this);
  connect(pausetimer, SIGNAL(timeout()), SLOT(pause()));

  kapp->processEvents();

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
  delete prompt;
}


void ConnectWidget::preinit() {
  // this is all just to keep the GUI nice and snappy ....
  // you have to see to believe ...
  messg->setText(i18n("Looking for Modem ..."));
  inittimer->start(100);
}


void ConnectWidget::init() {
  pppd_has_died = false;
  gpppdata.setpppdError(0);
  inittimer->stop();
  vmain = 0;
  expecting = false;
  pausing = false;
  scriptindex = 0;
  myreadbuffer = "";
  scanning = false;
  scanvar = "";
  firstrunID = true;
  firstrunPW = true;
  totalbytes = 0;
  dialnumber = 0;

  p_kppp->con_speed = "";

  quit_on_disconnect = quit_on_disconnect || gpppdata.quit_on_disconnect(); 

  QString tit = i18n("Connecting to: ");
  tit += gpppdata.accname();
  setCaption(tit);

  kapp->processEvents();

  int lock = lockdevice();

  if (lock == 1) {
    
    messg->setText(i18n("Sorry, modem device is locked."));
    vmain = 20; // wait until cancel is pressed
    return;
  }
  if (lock == -1) {
    
    messg->setText(i18n("Sorry, can't create modem lock file."));
    vmain = 20; // wait until cancel is pressed
    return;
  }

  if(opentty()) {
    messg->setText(modemMessage());
    kapp->processEvents();
    if(hangup()) {

      kapp->processEvents();

      // this timer reads from the modem
      semaphore = false;

      if(sn) {
	delete sn;
	Debug("QSocketNotifier stopped!");
      }
      sn = new QSocketNotifier(modemfd, QSocketNotifier::Read, this);
      connect(sn, SIGNAL(activated(int)),
	      this, SLOT(readtty(int)));
      
      // if we are stuck anywhere we will time out
      timeout_timer->start(atoi(gpppdata.modemTimeout())*1000); 
      
      // this timer will run the script etc.
      main_timer_ID = startTimer(10);

      return;
    }
  }

  // initialization failed
  messg->setText(modemMessage());
  vmain = 20; // wait until cancel is pressed
  unlockdevice();
}                  


void ConnectWidget::timerEvent(QTimerEvent *) {
  if (semaphore || pausing)
    return;

  if(vmain == 0 || vmain == 3) {
    messg->setText(i18n("Initializing Modem..."));
    emit debugMessage(i18n("Initializing Modem..."));

    // TODO
    // carriage return and then wait a second so that the modem will
    // let us issue commands.
    //writeline("");
    //usleep(100000); 

    setExpect(gpppdata.modemInitResp());
    if(vmain == 0) {
      writeline(gpppdata.modemInitStr());
      usleep(gpppdata.modemInitDelay() * 10000); // 0.01 - 3.0 sec       
      vmain = 3;
    } else {
      QString vol("AT");
      vol += gpppdata.volumeInitString();
      writeline(vol.data());
      usleep(gpppdata.modemInitDelay() * 10000); // 0.01 - 3.0 sec 
      vmain = 1;
    }
      

    return;
  }

  // dial the number and wait to connect
  if(vmain == 1) {
    if(!expecting) {

      timeout_timer->stop();
      timeout_timer->start(atoi(gpppdata.modemTimeout())*1000);

      QString bm = i18n("Dialing");
      QStrList *plist = &gpppdata.phonenumbers();
      bm += " ";
      bm += plist->at(dialnumber);
      messg->setText(bm);
      emit debugMessage(bm);

      QString pn = gpppdata.modemDialStr();
      pn += plist->at(dialnumber);
      if(++dialnumber >= plist->count())
        dialnumber = 0;
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

      messg->setText(i18n("Line Busy. Hanging up ..."));
      emit debugPutChar('\n');
      hangup();

      if(gpppdata.busyWait() > 0) {

	QString bm = i18n("Line Busy. Waiting: ");
	bm += gpppdata.busyWait();
	bm += i18n(" seconds");
	messg->setText(bm);
	emit debugMessage(bm);
      
	pausing = true;
      
	pausetimer->start(atoi(gpppdata.busyWait())*1000, true);
	timeout_timer->stop();
      }
      modem_in_connect_state=false; 
      vmain = 0;
      return;
    }

    if(readbuffer.contains(gpppdata.modemNoDialtoneResp())) {
      timeout_timer->stop();

      messg->setText(i18n("No Dialtone"));
      vmain = 20;
      unlockdevice();
      return;
    }

    if(readbuffer.contains(gpppdata.modemNoCarrierResp())) {
      timeout_timer->stop();

      messg->setText(i18n("No Carrier"));
      vmain = 20;
      unlockdevice();
      return;
    }
  }

  // wait for newline after CONNECT response (so we get the speed)
  if(vmain == 101) {
    if(!expecting) {
      modem_in_connect_state=true; // modem will no longer respond to AT commands

      emit startAccounting();
      p_kppp->con_win->startClock();

      vmain = 2;
      scriptTimeout=atoi(gpppdata.modemTimeout())*1000;
      return;
    }
  }

  // execute the script

  if(vmain == 2) {
    if(!expecting && !pausing && !scanning) {

      timeout_timer->stop();
      timeout_timer->start(scriptTimeout);

      if(!gpppdata.scriptType(scriptindex) || !gpppdata.script(scriptindex)) {
	vmain = 10;
        return;
      }

      if(strcmp(gpppdata.scriptType(scriptindex), "Scan") == 0) {
	QString bm = i18n("Scanning ");
	bm += gpppdata.script(scriptindex);
	messg->setText(bm);
	emit debugMessage(bm);

        setScan(gpppdata.script(scriptindex));
	scriptindex++;
        return;
      }

      if(strcmp(gpppdata.scriptType(scriptindex), "Save") == 0) {
	QString bm = i18n("Saving ");
	bm += gpppdata.script(scriptindex);
	messg->setText(bm);
	emit debugMessage(bm);

	if(stricmp(gpppdata.script(scriptindex), "password") == 0) {
	  gpppdata.setPassword(scanvar.data());
	  p_kppp->setPW_Edit(scanvar.data());
	  if(gpppdata.storePassword())
	    gpppdata.setStoredPassword(scanvar.data());
	  firstrunPW = true;
	}

	scriptindex++;
        return;
      }


      if(strcmp(gpppdata.scriptType(scriptindex), "Send") == 0) {
	QString bm = i18n("Sending ");
	bm += gpppdata.script(scriptindex);
	messg->setText(bm);
	emit debugMessage(bm);

	writeline(gpppdata.script(scriptindex));
	scriptindex++;
        return;
      }

      if(strcmp(gpppdata.scriptType(scriptindex), "Expect") == 0) {
        QString bm = i18n("Expecting ");
        bm += gpppdata.script(scriptindex);
	messg->setText(bm);
	emit debugMessage(bm);

        setExpect(gpppdata.script(scriptindex));
	scriptindex++;
        return;
      }


      if(strcmp(gpppdata.scriptType(scriptindex), "Pause") == 0) {
	QString bm = i18n("Pause ");
	bm += gpppdata.script(scriptindex);
	bm += i18n(" seconds");
	messg->setText(bm);
	emit debugMessage(bm);
	
	pausing = true;
	
	pausetimer->start(atoi(gpppdata.script(scriptindex))*1000, true);
	timeout_timer->stop();
	
	scriptindex++;
	return;
      }

      if(strcmp(gpppdata.scriptType(scriptindex), "Timeout") == 0) {

	timeout_timer->stop();

	QString bm = i18n("Timeout ");
	bm += gpppdata.script(scriptindex);
	bm += i18n(" seconds");
	messg->setText(bm);
	emit debugMessage(bm);
	
	scriptTimeout=atoi(gpppdata.script(scriptindex))*1000;
        timeout_timer->start(scriptTimeout);
	
	scriptindex++;
	return;
      }

      if(strcmp(gpppdata.scriptType(scriptindex), "Hangup") == 0) {
	messg->setText(i18n("Hangup"));
	emit debugMessage(i18n("Hangup"));

	writeline(gpppdata.modemHangupStr());
	setExpect(gpppdata.modemHangupResp());
	
	scriptindex++;
	return;
      }

      if(strcmp(gpppdata.scriptType(scriptindex), "Answer") == 0) {
	
	timeout_timer->stop();

	messg->setText(i18n("Answer"));
	emit debugMessage(i18n("Answer"));

	setExpect(gpppdata.modemRingResp());
	vmain = 150;
	return;
      }

      if(strcmp(gpppdata.scriptType(scriptindex), "ID") == 0) {
	QString bm = i18n("ID ");
	bm += gpppdata.script(scriptindex);
	messg->setText(bm);
	emit debugMessage(bm);

	QString idstring = gpppdata.storedUsername();
	
	if(!idstring.isEmpty() && firstrunID) {
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
	QString bm = i18n("Password ");
	bm += gpppdata.script(scriptindex);
	messg->setText(bm);
	emit debugMessage(bm);

	QString pwstring = gpppdata.Password();
	
	if(!pwstring.isEmpty() && firstrunPW) {
	  // the user entered a password on the main kppp dialog
	  writeline(pwstring.data());
	  firstrunPW = false;
	  scriptindex++;
	}
	else{
	  // the user didn't enter a password on the main kppp dialog
	  // let's query for a password
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
		 p_kppp->setPW_Edit(prompt->text());
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
	QString bm = i18n("Prompting ");

        // if the scriptindex (aka the prompt text) includes a ## marker 
        // this marker should get substituted with the contents of our stored 
        // variable (from the subsequent scan).
	
	QString ts = gpppdata.script(scriptindex);
	int vstart = ts.find( "##" );
	if( vstart != -1 ) {
		ts.remove( vstart, 2 );
		ts.insert( vstart, scanvar );
	}

	bm += ts;
	messg->setText(bm);
	emit debugMessage(bm);

	/* if not around yet, then post window... */
	if (prompt->Consumed()) {
	   if (!(prompt->isVisible())) {
		prompt->setPrompt( ts );
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
	QString bm = i18n("PW Prompt ");
	bm += gpppdata.script(scriptindex);
	messg->setText(bm);
	emit debugMessage(bm);

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

        QString bm = i18n("Loop Start ");
        bm += gpppdata.script(scriptindex);

	if ( loopnest > (MAXLOOPNEST-2) ) {
		bm += i18n("ERROR: Nested too deep, ignored.");
		vmain=20;
		scriptindex++;
		cancelbutton();
	        QMessageBox::warning( 0, i18n("Error"),
				      i18n("Loops nested too deeply!"));
	} else {
        	setExpect(gpppdata.script(scriptindex));
		loopstartindex[loopnest] = scriptindex + 1;
		loopstr[loopnest] = gpppdata.script(scriptindex);
		loopend = false;
		loopnest++;
	}
	messg->setText(bm);
	emit debugMessage(bm);

	scriptindex++;
      }

      if(strcmp(gpppdata.scriptType(scriptindex), "LoopEnd") == 0) {
        QString bm = "Loop End ";
        bm += gpppdata.script(scriptindex);
	if ( loopnest <= 0 ) {
		bm = i18n("LoopEnd without mathing Start! Line: ") + bm ;
		vmain=20;
		scriptindex++;
		cancelbutton();
	        QMessageBox::warning( 0, i18n("Error"), bm );
		return;
	} else {
        	setExpect(gpppdata.script(scriptindex));
		loopnest--;
		loopend = true;
	}
	messg->setText(bm);
	emit debugMessage(bm);

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

  if(vmain == 30) {
    if (termwindow->isVisible()) return;
    if (termwindow->pressedContinue())
      vmain = 10;
    else
      cancelbutton();
  }

  if(vmain == 10) { 
    if(!expecting) {

      int result;

      timeout_timer->stop();
      if_timeout_timer->stop(); // better be sure.

      // stop reading of data
      if(sn) {
	delete sn;
	sn = 0;
	Debug("QSocketNotifier stopped!");
      }

      if(gpppdata.authMethod() == AUTH_TERMINAL) {
	if (termwindow) {
	  delete termwindow;
	  termwindow = 0L;
	  this->show();
	} else {
	  termwindow = new LoginTerm(0L, 0L, modemfd);
	  hide();
	  termwindow->show();
	  vmain = 30;
	  return;
	}
      }
      
      killTimer( main_timer_ID );

      if_timeout_timer->start(atoi(gpppdata.pppdTimeout())*1000);
      Debug("started if timeout timer with %d\n",atoi(gpppdata.pppdTimeout())*1000);

      kapp->flushX();
      semaphore = true;
      result = execppp();

      emit debugMessage(i18n("Starting pppd ..."));
      Debug("execppp() returned with return-code %d\n", result);

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
	p_kppp->quit_b->setFocus();
	p_kppp->show();
	kapp->processEvents();
	hangup();
	emit stopAccounting();
	p_kppp->con_win->stopClock();
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


void ConnectWidget::set_con_speed_string() {
  // Here we are trying to determine the speed at which we are connected.
  // Usually the modem responds after connect with something like
  // CONNECT 115200, so all we need to do is find the number after CONNECT
  // or whatever the modemConnectResp() is.
  QString t, r;

  t = gpppdata.modemConnectResp();
  t = t.stripWhiteSpace();
  r = Modem::parseModemSpeed(t);
  p_kppp->con_speed = r.data();
}



void ConnectWidget::readtty(int) {
  if(semaphore)
    return;

  char c = 0;

  if(read(modemfd, &c, 1) == 1) {
    c = ((int)c & 0x7F);
    readbuffer += c;
    myreadbuffer += c;
    
    // While in scanning mode store each char to the scan buffer
    // for use in the prompt command
    if( scanning )
       scanbuffer += c;
 
    // add to debug window
    emit debugPutChar(c);
  }

  // Let's check if we are finished with scanning:
  // The scanstring have to be in the buffer and the latest character
  // was a carriage return or an linefeed (depending on modem setup)
  if( scanning && scanbuffer.contains(scanstr) && (c == '\n' || c == '\r') ) {
      scanning = false;

      int vstart = scanbuffer.find( scanstr ) + scanstr.length();
      scanvar = scanbuffer.mid( vstart, scanbuffer.length() - vstart);
      scanvar = scanvar.stripWhiteSpace();

      // Show the Variabel content in the debug window
      QString sv = i18n("Scan Var: ");
      sv += scanvar;
      emit debugMessage(sv);
  }

  if(expecting) {
    if(readbuffer.contains(expectstr)) {
      expecting = false;
      readbuffer = "";

      QString ts = i18n("Found: ");
      ts += expectstr;
      emit debugMessage(ts);

      if (loopend) {
	loopend=false;
      }
      return;
    }

    if (loopend && readbuffer.contains(loopstr[loopnest])) {
      expecting = false;
      readbuffer = "";
      QString ts = i18n("Looping: ");
      ts += loopstr[loopnest];
      emit debugMessage(ts);
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
  if(sn) {
    delete sn;
    sn = 0;
    Debug("QSocketNotifier stopped!");
  }

  killTimer(main_timer_ID);
  timeout_timer->stop();

  if (termwindow) {
    delete termwindow;
    termwindow = 0L;
    this->show();
  }

  messg->setText(i18n("One Moment Please ..."));

  // just to be sure
  PAP_RemoveAuthFile();
  CHAP_RemoveAuthFile();
  
  kapp->processEvents();
  
  hangup();

  this->hide();
  messg->setText("");
  p_kppp->quit_b->setFocus();
  p_kppp->show();
  emit stopAccounting();	// just to be sure
  p_kppp->con_win->stopClock();
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


void ConnectWidget::script_timed_out() {
  if(vmain == 20) { // we are in the 'wait for the user to cancel' state
    timeout_timer->stop();
    emit stopAccounting();
    p_kppp->con_win->stopClock();
    return;
  }

  if (prompt->isVisible())
    prompt->hide();
  
  prompt->setConsumed();
  messg->setText(i18n("Script timed out!"));
  hangup();
  emit stopAccounting();
  p_kppp->con_win->stopClock();

  vmain = 0; // let's try again.
}


void ConnectWidget::debugbutton() {
  emit toggleDebugWindow();
}


void ConnectWidget::setScan(const char *n) {
  scanning = true;
  scanstr = n;
  scanbuffer = "";

  QString ts = i18n("Scanning: ");
  ts += n;
  emit debugMessage(ts);
}


void ConnectWidget::setExpect(const char *n) {
  expecting = true;
  expectstr = n;

  QString ts = i18n("Expecting: ");
  ts += n;
  emit debugMessage(ts);
}


void ConnectWidget::if_waiting_timed_out() {
  if_timer->stop();
  if_timeout_timer->stop();
  Debug("if_waiting_timed_out()\n");
  
  gpppdata.setpppdError(E_IF_TIMEOUT);

  // let's kill the stuck pppd
  killpppd();

  emit stopAccounting();
  p_kppp->con_win->stopClock();

  
  // killing ppp will generate a SIGCHLD which will be caught in pppdie()
  // in main.cpp what happens next will depend on the boolean 
  // reconnect_on_disconnect which is set in ConnectWidget::init();
}


void ConnectWidget::if_waiting_slot() {
  messg->setText(i18n("Logging on to Network ..."));

  if(!if_is_up()) {

    if(pppd_has_died) { // we are here if pppd died immediately after starting it.

      if_timer->stop();
      // error message handled in main.cpp: die_ppp()
      return;
    }

    if_timer->start(100, TRUE); // single shot 
    return;
  }

  // O.K the ppp interface is up and running
  // give it a few time to come up completly (0.2 seconds)
  if_timeout_timer->stop(); 
  if_timer->stop();
  usleep(200000);

  p_kppp->stats->take_stats(); // start taking ppp statistics
  auto_hostname();

  if(strcmp(gpppdata.command_on_connect(), "") != 0) {
    messg->setText(i18n("Running Startup Command ..."));

    // make sure that we don't get any async errors
    kapp->flushX(); 
    execute_command(gpppdata.command_on_connect());
    messg->setText(i18n("Done"));
  }

  // remove the authentication file
  PAP_RemoveAuthFile();
  CHAP_RemoveAuthFile();

  emit debugMessage(i18n("Done"));
  set_con_speed_string();

  p_kppp->con_win->setConnectionSpeed();
  this->hide();
  messg->setText("");

  // prepare the con_win so as to have the right size for
  // accounting / non-accounting mode
  p_kppp->con_win->accounting(p_kppp->accounting.running());

  if (gpppdata.get_dock_into_panel()) {
    dock_widget->dock();
    dock_widget->take_stats();
    this->hide();
  } 
  else {
    p_kppp->con_win->show();
    
    if(gpppdata.get_iconify_on_connect()) {
      p_kppp->con_win->iconify();
    }
  }

  closetty();
}


bool ConnectWidget::execppp() {
  pid_t id;

  QString command;
  const unsigned int MAX_CMDLEN = 2024;
  char buf[MAX_CMDLEN];
  char *args[MAX_ARGS];


  command = "pppd";

  command += " ";
  command += gpppdata.modemDevice();

  command += " " ;
  command += gpppdata.speed();

  command += " -detach";

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

  // PAP settings
  if(gpppdata.authMethod() == AUTH_PAP) {
    command += " -chap user ";
    command = command + "\"" + gpppdata.storedUsername() + "\"";
  }

  // CHAP settings
  if(gpppdata.authMethod() == AUTH_CHAP) {
    command += " -pap user ";
    command = command + "\"" + gpppdata.storedUsername() + "\"";
  }

  if (command.length() > MAX_CMDLEN) {
    QMessageBox::warning(this, 
			 i18n("Error"), 
			 i18n(
			      "pppd command + command-line arguments exeed\n"
			      "2024 characters in length. What are you doing?"
			      )
			 );	
    return false; // nonsensically long command which would bust my buffer buf.
  }
  
  // check for debug
  if(gpppdata.getPPPDebug())
    command += " debug";
  
  strcpy(buf,command.data());

  // let's parse the arguments the user supplied into UNIX suitable form
  // that is a list of pointers each pointing to exactly one word

  parseargs(buf,args);

  kapp->flushX();

  if((id = fork()) < 0)
    { 
      fprintf(stderr,"In parent: fork() failed\n");
      return false;
    }

  if(id != 0) {
    Debug("In parent: fork() %d\n",id);
    
    gpppdata.setpppdpid(id);
    Debug("pppd pid %d\n",id);
    return true;
  }

  if(id == 0) {
    /*    printf("In child: fork() %d\n",id);*/
    /*    close(modemfd);*/
    Debug("%s \n",command.data());

#ifdef BSD
    setpgrp(0,0);    
#else
    setpgrp();
#endif
    
    execve(gpppdata.pppdPath(), args, '\0');
    _exit(0);
  }

  return true;
}


void ConnectWidget::closeEvent( QCloseEvent *e ) {
  // We don't want to lose the conwindow since this is our last 
  // connection to kppp. If we lost it we would have to kill the 
  // program by hand to get on with life.
  e->ignore();                            
}

void ConnectWidget::setMsg(const char* msg) {
  messg->setText(msg);
}


// Set the hostname and domain from DNS Server
void auto_hostname() {

  struct in_addr local_ip;
  struct hostent *hostname_entry;
  QString new_hostname;
  int    dot;
  char   tmp_str[100];

  gethostname(tmp_str, 100);
  old_hostname=tmp_str;

  if (!local_ip_address.isEmpty() && gpppdata.autoname()) {
    local_ip.s_addr=inet_addr((const char*)local_ip_address);
    hostname_entry=gethostbyaddr((const char *)&local_ip,sizeof(in_addr),AF_INET);

    if (hostname_entry != NULL) {
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

  if (domain == NULL || ! strcmp(domain, "")) 
    return;

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
	  write(fd, resolv[j].data(), resolv[j].length());
          write(fd, " \t#entry disabled by kppp\n", 26);
	}
	else {
	  write(fd, resolv[j].data(), resolv[j].length());
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
  QString resolv[MAX_RESOLVCONF_LINES];
  extern QString old_hostname;

  if((fd = open("/etc/resolv.conf", O_RDONLY)) >= 0) {

    int i=0;
    while(read(fd, &c, 1) == 1 && i < MAX_RESOLVCONF_LINES) {
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
	if(resolv[j].contains("#entry disabled by kppp")) {
	  write(fd, resolv[j].data()+2, resolv[j].length() - 27);
	  write(fd, "\n", 1);
	}
	else{
	  write(fd, resolv[j].data(), resolv[j].length());
	  write(fd, "\n", 1);
	}
      }
    }
    close(fd);

  }

  if (  modified_hostname ) {
    sethostname ((const char*)old_hostname,old_hostname.length());
    modified_hostname = FALSE;
  }

}  


void parseargs(char* buf, char** args) {
  int nargs = 0;
  int quotes;

  while(nargs < MAX_ARGS -1 && *buf != '\0') {
    
    quotes = 0;
    
    // Strip whitespace. Use nulls, so that the previous argument is
    // terminated automatically.
     
    while ((*buf == ' ' ) || (*buf == '\t' ) || (*buf == '\n' ) )
      *buf++ ='\0';
    
    // detect begin of quoted argument
    if (*buf == '"' || *buf == '\'') {
      quotes = *buf;
      *buf++ ='\0';
    }

    // save the argument
    if(*buf != '\0') {
      *args++ = buf;
      nargs++;
    }
    
    if (!quotes)
      while ((*buf != '\0') && (*buf != '\n') &&
	     (*buf != '\t') && (*buf != ' '))
	buf++;
    else {
      while ((*buf != '\0') && (*buf != quotes))
	buf++;
      *buf++ = '\0';
    } 
  }
 
  *args = 0L;
}

#include "connect.moc"
