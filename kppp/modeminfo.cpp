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

#include <qdir.h>

#include <unistd.h>
#include <qregexp.h>
#include <kapp.h> 
#include "modeminfo.h"
#include "macros.h"

extern QString ati_query_strings[NUM_OF_ATI];


ModemTransfer::ModemTransfer(QWidget *parent, const char *name)
  : QDialog(parent, name,TRUE, WStyle_Customize|WStyle_NormalBorder)
{
  setCaption(i18n("ATI Query"));

  QVBoxLayout *tl = new QVBoxLayout(this, 10, 10);
  
  progressBar = new KProgress(0, 8, 0, KProgress::Horizontal, this, "bar");
  progressBar->setBarStyle(KProgress::Blocked);
  
  statusBar = new QLabel(this,"sBar");
  statusBar->setFrameStyle(QFrame::Panel|QFrame::Sunken);
  statusBar->setAlignment(AlignCenter);

  // This is a rather complicated case. Since we do not know which
  // message is the widest in the national language, we'd to
  // search all these messages. This is a little overkill, so I take
  // the longest english message, translate it and give it additional
  // 20 percent space. Hope this is enough.
  statusBar->setText(i18n("Sorry, can't create modem lock file."));
  statusBar->setMinimumWidth((statusBar->sizeHint().width() * 12) / 10);
  statusBar->setMinimumHeight(statusBar->sizeHint().height() + 4);

  // set original text
  statusBar->setText(i18n("Looking for Modem ..."));
  progressBar->setFixedHeight(statusBar->minimumSize().height());
  tl->addWidget(progressBar);
  tl->addWidget(statusBar);

  cancel = new QPushButton(i18n("Cancel"), this);
  cancel->setFocus();
  connect(cancel, SIGNAL(clicked()), SLOT(cancelbutton()));
  FIXED_SIZE(cancel);

  QHBoxLayout *l1 = new QHBoxLayout;
  tl->addLayout(l1);
  l1->addStretch(1);
  l1->addWidget(cancel);

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

  timeout_timer->start(15000,TRUE); // 15 secs single shot
  inittimer->start(500);

  tl->freeze();
}


void ModemTransfer::ati_done_slot(){

  readtimer->stop();
  scripttimer->stop();
  timeout_timer->stop();
  closetty();
  unlockdevice();
  accept();

}

void ModemTransfer::time_out_slot() {

  timeout_timer->stop();
  readtimer->stop();
  scripttimer->stop();

  QMessageBox::warning(this, 
		       i18n("Error"),
		       i18n("Modem Query timed out."));
  reject();
}

void ModemTransfer::init() {

  inittimer->stop();
  expecting = false;

  kapp->processEvents();

  int lock = lockdevice();
  if (lock == 1){
    
    statusBar->setText(i18n("Sorry, modem device is locked."));
    return;
  }
  if (lock == -1){
    
    statusBar->setText(i18n("Sorry, can't create modem lock file."));
    return;
  }


  if(opentty()){

    writeline(gpppdata.modemHangupStr());
    usleep(100000);  // wait 0.1 secs
    if(hangup()) {
      usleep(100000);  // wait 0.1 secs
      writeline("ATE0Q1V1"); // E0 don't echo the commands I send ...

      statusBar->setText(i18n("Modem Ready"));
      kapp->processEvents();

      kapp->processEvents();
      scripttimer->start(1000);	 	// this one does the ati query
    }
  }
  
  // opentty() or hangup() failed 
  statusBar->setText(modemMessage());
  step = 99; // wait until cancel is pressed
  unlockdevice();
  
}                  




void ModemTransfer::do_script() {

    if(step == 0) {
      readtty();
      statusBar->setText("ATI ...");
      progressBar->advance(1);
      writeline("ATI");
      step ++;
      return;
    }

    if(step == 1) {
      readtty();
      statusBar->setText("ATI 1 ...");
      progressBar->advance(1);
      writeline("ATI1");
      return;
    }

    if(step == 2) {
      readtty();
      statusBar->setText("ATI 2 ...");
      progressBar->advance(1);
      writeline("ATI2");
      return;
    }

    if(step == 3) {
      readtty();
      statusBar->setText("ATI 3 ...");
      progressBar->advance(1);
      writeline("ATI3");
      return;
    }

    if(step == 4) {
      readtty();
      statusBar->setText("ATI 4 ...");
      progressBar->advance(1);
      writeline("ATI4");
      return;
    }

    if(step == 5) {
      readtty();
      statusBar->setText("ATI 5 ...");
      progressBar->advance(1);
      writeline("ATI5");
      return;
    }

    if(step == 6) {
      readtty();
      statusBar->setText("ATI 6 ...");
      progressBar->advance(1);
      writeline("ATI6");
      return;
    }

    if(step == 7) {
      readtty();
      statusBar->setText("ATI 7 ...");
      progressBar->advance(1);
      writeline("ATI7");
      return;
    }
    readtty();
    emit ati_done();

}


void ModemTransfer::readtty() {

  char buffer[255];

  memset(buffer,'\0',255);
  read(modemfd, buffer, 254);

  if (step == 0)
    return;

  readbuffer = buffer;

    
  readbuffer.replace("\n"," ");         // remove stray \n
  readbuffer.replace("\r","");          // remove stray \r
  readbuffer = readbuffer.stripWhiteSpace(); // strip of leading or trailing white
                                                 // space

  if(step < NUM_OF_ATI + 1){
    ati_query_strings[step-1] = readbuffer.copy();
  }
  readbuffer = "";
  ati_counter ++;
  step ++;

}


void ModemTransfer::cancelbutton() {

  scripttimer->stop();
  readtimer->stop();
  timeout_timer->stop();

  statusBar->setText(i18n("One Moment Please ..."));
  kapp->processEvents();
  
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


void ModemTransfer::closeEvent( QCloseEvent *e ){

  e->ignore();     // don't let the user close the window

}


ModemInfo::ModemInfo(QWidget *parent, const char* name)
  : QDialog(parent, name, TRUE, WStyle_Customize|WStyle_NormalBorder)
{
  QString label_text;

  setCaption(i18n("Modem Query Results"));

  QVBoxLayout *tl = new QVBoxLayout(this, 10, 10);

  QGridLayout *l1 = new QGridLayout(NUM_OF_ATI, 2, 5);
  tl->addLayout(l1, 1);
  for(int  i = 0 ; i < NUM_OF_ATI ; i++) {

    label_text = "";
    if ( i == 0)
      label_text.sprintf("ATI :");
    else
      label_text.sprintf("ATI %d:", i );

    ati_label[i] = new QLabel(label_text.data(), this);
    MIN_SIZE(ati_label[i]);
    l1->addWidget(ati_label[i], i, 0);

    ati_label_result[i] =  new QLineEdit(this);    
    ati_label_result[i]->setText(ati_query_strings[i]);
    MIN_SIZE(ati_label_result[i]);
    FIXED_HEIGHT(ati_label_result[i]);
    l1->addWidget(ati_label_result[i], i, 1); 
  }
  //tl->addSpacing(1);

  QHBoxLayout *l2 = new QHBoxLayout;
  ok = new QPushButton(i18n("Close"), this);
  ok->setDefault(TRUE);
  ok->setFocus();

  // Motif default buttons + Qt layout do not work tight together
  if(ok->style() == MotifStyle) {
    ok->setFixedWidth(ok->sizeHint().width() + 10);
    ok->setFixedHeight(ok->sizeHint().height() + 10);
    tl->addSpacing(8);
  } else
    FIXED_SIZE(ok);

  tl->addLayout(l2);
  l2->addStretch(1);

  connect(ok, SIGNAL(clicked()), SLOT(okbutton()));
  l2->addWidget(ok);
  
  tl->freeze();
}

void ModemInfo::okbutton() {

  accept();  

}

#include "modeminfo.moc"
