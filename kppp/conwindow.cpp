/*
 *            kPPP: A pppd front end for the KDE project
 *
 * $Id$
 * 
 *            Copyright (C) 1997 Bernd Johannes Wuebben 
 *                   wuebben@math.cornell.edu
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
#include "conwindow.h"
#include "main.h"


extern XPPPWidget *p_xppp;
extern KApplication *app;
extern PPPData gpppdata;

ConWindow::ConWindow(QWidget *parent, const char *name,QWidget *mainwidget)
  : QWidget(parent, name,0 )
{


  main = mainwidget;
  minutes = 0;
  seconds = 0;
  hours = 0;
  days = 0;


  info1 = new QLabel(this,"infolabel1");
  info1->setGeometry(40,10,150,25);
  info1->setText("Connected at:");

  info2 = new QLabel(this,"infolabel");
  info2->setGeometry(150,10,150,25);
  info2->setText("");
 
  timelabel1 = new QLabel(this,"timelabel1");
  timelabel1->setGeometry(40,30,200,25);
  timelabel1->setText("Time connected:");

  timelabel2 = new QLabel(this,"timelabel");
  timelabel2->setGeometry(150,30,200,25);
  timelabel2->setText("000:00:00");

  // now the stuff for accounting
  session_bill_l = new QLabel("Session Bill:", this);
  session_bill_l->setGeometry(40, 30+20, 200, 25);
  session_bill = new QLabel("", this);
  session_bill->setGeometry(150, 30+20, 200, 25);
  total_bill_l = new QLabel("Total Bill:", this);
  total_bill_l->setGeometry(40, 30+20+20, 200, 25);
  total_bill = new QLabel("", this);
  total_bill->setGeometry(150, 30+20+20, 200, 25);

  this->setCaption("kppp");

  cancelbutton = new QPushButton(this,"cancelbutton");

  cancelbutton->setText("Disconnect");
  connect(cancelbutton, SIGNAL(clicked()), main,SLOT(disconnect()));
  cancelbutton->setGeometry(225,75+40,80,25);

  statsbutton = new QPushButton(this,"statsbutton");
  statsbutton->setGeometry(225,40,80,25);
  statsbutton->setText("Details");
  statsbutton->setFocus();
  connect(statsbutton, SIGNAL(clicked()), this,SLOT(stats()));



  /*  fline = new QFrame(this,"line");
  fline->setFrameStyle(QFrame::HLine |QFrame::Sunken);
  fline->setGeometry(10,65+50,300,5);
  */

  clocktimer = new QTimer(this);
  connect(clocktimer, SIGNAL(timeout()), SLOT(timeclick()));
}


ConWindow::~ConWindow() {

 this->stopClock();
  
}



void ConWindow::accounting(bool on){

  if(on){
    cancelbutton->move(225,70);
    cancelbutton->show();
    statsbutton->move(225,40);
    timelabel1->move(40,30);
    timelabel2->move(150,30);
    statsbutton->show();
    session_bill->show();
    session_bill_l->show();
    total_bill_l->show();
    total_bill->show();
    setFixedSize(320, 110);
    setGeometry(QApplication::desktop()->width()/2-160,
		QApplication::desktop()->height()/2-55,
		320,110);
  }
  else{
      cancelbutton->move(225,62);
      statsbutton->move(225,32);
      session_bill->hide();
      session_bill_l->hide();
      total_bill_l->hide();
      total_bill->hide();
      timelabel1->move(40,33);
      timelabel2->move(150,33);
      setFixedSize(320, 97);
      setGeometry(QApplication::desktop()->width()/2-160,
		  QApplication::desktop()->height()/2-47,
		  320,97);
  }

}



void ConWindow::stats(){

  p_xppp->stats->show();

}


void ConWindow::startClock() {

  minutes = 0;
  seconds = 0;
  hours = 0;
  QString title ;

  title = gpppdata.accname();

  if(gpppdata.get_show_clock_on_caption()){
    title += " 00:00" ; 
  }
  this->setCaption(title);

  timelabel2->setText("00:00:00");
  clocktimer->start(1000);

}

void ConWindow::setConnectionSpeed(){

  if(p_xppp)
    info2->setText(p_xppp->con_speed);

}

void ConWindow::stopClock() {
  
  clocktimer->stop();

}
  
void ConWindow::timeclick() {
  seconds++;
  
  if(seconds >= 60 ) {
    minutes ++;
    seconds = 0;
  }

  if (minutes >= 60){
    minutes = 0;
    hours ++;
  }

  if( hours >= 24){
    days ++;
    hours = 0;
  }
  
  time_string = "";
  

  time_string.sprintf("%02d:%02d",hours,minutes);

  time_string2 = "";
  if (days){
      time_string2.sprintf("%d d %02d:%02d:%02d",
			   days,hours,minutes,seconds);

  }
  else{
    time_string2.sprintf("%02d:%02d:%02d",hours,minutes,seconds);
  }

  caption_string = gpppdata.accname();
  caption_string += " ";
  caption_string += time_string;
  

  timelabel2->setText(time_string2);

  if(gpppdata.get_show_clock_on_caption() && (seconds == 1)){
    // we update the Caption only once per minute not every second
    // otherwise I get a flickering icon 
    this->setCaption(caption_string);
  }

}

void ConWindow::closeEvent( QCloseEvent *e ){

        e->ignore();                            
	
	// we don't want to lose the
	// conwindow since this is our last connection kppp. 
	// if we lost it we could only kill the program by hand to get on with life.

}


void ConWindow::slotAccounting(QString total, QString session) {
  total_bill->setText(total.data());
  session_bill->setText(session.data());
}
