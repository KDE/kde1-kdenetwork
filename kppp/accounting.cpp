/* -*- C++ -*-
 *
 *            kPPP: A pppd front end for the KDE project
 *
 * $Id$
 * 
 *            Copyright (C) 1997 Bernd Johannes Wuebben 
 *                   wuebben@math.cornell.edu
 *
 * This file contributed by: Mario Weilguni, <mweilguni@arctica.sime.com>
 * Thanks Mario!
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

#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <qdir.h>
#include <qfile.h>
#include <qdatetm.h>
#include <qregexp.h>
#include <qlabel.h>

#include <kapp.h>
#include <kconfig.h>
#include <kprogress.h>

#include "accounting.h"
#include "homedir.h"
#include "config.h"
#include "pppdata.h"

#ifdef MY_DEBUG
#include <stdio.h>
#endif

// defines the maximum duration until the current costs
// are saved again (to prevent loss due to "kill")
// specifying -1 disables the features
#define UPDATE_TIME    (5*60*1000)

// maximum size of a logfile
// #define LOGFILE_MAXSIZE	1000000


extern PPPData gpppdata;

Accounting::Accounting(QObject *parent) : QObject(parent) {

  _total = 0;
  _session = 0;
  acct_timer_id = 0;
  update_timer_id = 0;

}


Accounting::~Accounting() {

  if(running())
    slotStop();

}


bool Accounting::running() {

  return (bool)(acct_timer_id != 0);

}


void Accounting::timerEvent(QTimerEvent *t) {

  if(t->timerId() == acct_timer_id) {

    double newCosts;
    double newLen;

    rules.getActiveRule(QDateTime::currentDateTime(), newCosts, newLen);
    if(newLen < 9) {
      slotStop();
      return; // no default rule found
    }
    
    // check if we have a new rule. If yes,
    // kill the timer and restart it with new
    // duration
    if((newCosts != _lastcosts) || (newLen != _lastlen)) {

#ifdef MY_DEBUG
      printf("SWITCHING RULES, new costs = %0.2f, new len = %0.2f\n",
	     newCosts, newLen);
#endif


      killTimer(acct_timer_id);
      if(newLen > 0)
	acct_timer_id = startTimer((int)(newLen * 1000.0));

      _lastlen = newLen;
      _lastcosts = newCosts;
    }

    // emit changed() signal if necessary
    if(newCosts != 0) {
      _session += newCosts;
       emit changed(rules.currencyString(total()),
		    rules.currencyString(session()));


    }
  } // if(t->timerId() == acct_timer_id)...

  if(t->timerId() == update_timer_id) {
    // just to be sure, save the current costs
    // every n seconds (see UPDATE_TIME)

    saveCosts();
  }
  
}


void Accounting::slotStart() {

  if(!running()) {
    loadCosts();
    _lastcosts = 0.0;
    _lastlen   = 0.0;
    _session = rules.perConnectionCosts();
    acct_timer_id = startTimer(1);
     if(UPDATE_TIME > 0)
       update_timer_id = startTimer(UPDATE_TIME);

    QDate date;
    LogFileName = date.monthName(QDate::currentDate().month());
    LogFileName += "-";
    QString yearstr;
    yearstr.setNum(QDate::currentDate().year());
    LogFileName += yearstr;
    QString str;
    str.sprintf("started PPP connection:%s:%s",
		gpppdata.accname(),gpppdata.phonenumber());
    logMessage(str.data());
  }
}


void Accounting::slotStop() {

  if(running()) {
    killTimer(acct_timer_id);
    if(update_timer_id != 0)
      killTimer(update_timer_id);
    acct_timer_id = 0;
    update_timer_id = 0;
    QString s = "terminated PPP connection\nCOST OF SESSION: ";

    s += rules.currencyString(session()) \
      + "\nSUBTOTAL FOR:" \
      + gpppdata.accname() \
      + ":"\
      + rules.currencyString(total());
    logMessage(s);
    saveCosts();
  }
}


bool Accounting::loadRuleSet(const char *name) {

  if(strlen(name) == 0) {
    rules.load(""); // delete old rules
    return TRUE;
  }

  QString d;
  // load from home directory if file is found there
  d = getHomeDir();
  d += ACCOUNTING_PATH ;
  d += "/Rules/";
  d += name;


  QFileInfo fl(d.data());
   if(fl.exists()) {
     int ret = rules.load(d.data());
     return (bool)(ret == 0);
   }
  
  // load from KDE directory if file is found there
  d = KApplication::kdedir();
  d += "/share/apps/kppp/Rules/";
  d += name;

  QFileInfo fg(d.data());
   if(fg.exists()) {
     int ret = rules.load(d.data());
     return (bool)(ret == 0);
   }

 return FALSE;
}


void Accounting::logMessage(const char *s) {
  logMessage(QString(s));
}


void Accounting::logMessage(QString s) {

  QString fname = getHomeDir();
  fname += ACCOUNTING_PATH;
  fname += "/Log/";
  fname += LogFileName;

  QFile f(fname.data());

  if(f.open(IO_WriteOnly | IO_Append)) {

    QString date = QDateTime::currentDateTime().toString() + ": ";
    bool doreplace = TRUE;

    while(s.length() > 0) {

      f.writeBlock(date.data(), date.length());
      int len;

      if((len = s.find("\n")) == -1)
	len = s.length();
	
      f.writeBlock(s.left(len).data(), len);
      s = s.mid(len+1, s.length()-len+1);
      f.writeBlock("\n", 1);

      if(doreplace) {
	date = date.replace(QRegExp("."), " ");
	doreplace = FALSE;
      }

    }

    f.close();
    chown(fname.data(),getuid(),getgid());
    chmod(fname.data(),S_IRUSR | S_IWUSR);
  }
}



double Accounting::total() {
  if(rules.minimumCosts() <= _session)
    return _total + _session;
  else
    return _total + rules.minimumCosts();
 }

 

double Accounting::session() {

  if(rules.minimumCosts() <= _session)
    return _session;
  else
    return rules.minimumCosts();

}

// set costs back to zero ( typically once per month)
bool Accounting::resetCosts(const char *accountname){

    QString s = getHomeDir();
    s += ACCOUNTING_PATH;
    s += "Costs";

    QString str;
    str = accountname;
    str.stripWhiteSpace();
    if(str.isEmpty())
      return FALSE;

    QFileInfo f(s.data());
    if(!f.exists())
      return FALSE;
    
    KConfig conf(s);
    
    conf.setGroup(str);
    conf.writeEntry(QString("TotalCosts"), "");

    conf.sync();

    chown(s.data(),getuid(),getgid());
    chmod(s.data(),S_IRUSR | S_IWUSR);    
    return TRUE;

}


bool Accounting::saveCosts() {

  if(!rules.name().isNull() && (rules.name().length() > 0)) {

    QString s = getHomeDir();
    s += ACCOUNTING_PATH;
    s += "Costs";

    QFileInfo f(s.data());

    if(!f.exists()){
      // doesn't exist yet -- let's create it.
      QFile file(s.data());

      if(!file.open( IO_ReadWrite )){
	QString string;
	string.sprintf("Cannot create\n%s\nYour online costs will not be saved.",s.data());
	QMessageBox::warning(0,"Sorry",string.data());
	return FALSE;
      }
      file.close();
    }

    KConfig conf(s);
    QString val;
    val.setNum(total());

    conf.setGroup(QString(gpppdata.accname()));
    conf.writeEntry(QString("TotalCosts"), val);

    conf.sync();

    chown(s.data(),getuid(),getgid());
    chmod(s.data(),S_IRUSR | S_IWUSR);    
    return TRUE;

  } else
    
    return FALSE;
}


bool Accounting::loadCosts() {

  QString s = getHomeDir();
  s += ACCOUNTING_PATH;
  s += "Costs";

  QFileInfo f(s.data());
  if(!f.exists())
    return FALSE;

  KConfig conf(s);
  QString val;

  /*conf.setGroup(QString(rules.name()));*/
  conf.setGroup(QString(gpppdata.accname()));
  val = conf.readEntry(QString("TotalCosts"));

  if(val.isNull()) // QString will segfault if isnull and toDouble called
    _total = 0.0;
  else {
    bool ok;
    _total = val.toDouble(&ok);
    if(!ok)
      _total = 0.0;
  }

  return TRUE;

}


QString Accounting::getCosts(const char* accountname) {


  QString s = getHomeDir();
  s += ACCOUNTING_PATH;
  s += "Costs";


  QFileInfo f(s.data());
  if(!f.exists())
    return FALSE;

  KConfig conf(s);
  QString val;

  conf.setGroup(QString(accountname));
  val = conf.readEntry(QString("TotalCosts"));

  val.stripWhiteSpace();

  return val;

}


/*
void Accounting::truncateLogFile() {
  QString fname = getHomeDir();
  fname += ACCOUNTING_PATH;
  fname += "/LOG";

  QFile f(fname.data());
  if(f.exists() && (f.size() > LOGFILE_MAXSIZE)) {
    // rename the file to LOG~
    QDir d;
    if(!d.rename(fname.data(), (fname + "~").data()))
      return; // ERROR

    f.setName((fname + "~").data());
    if(!f.open(IO_ReadOnly)) {
      d.rename((fname + "~").data(), fname.data());
      return; // ERROR
    }

    QFile fnew(fname.data());
    if(!fnew.open(IO_WriteOnly)) {     
      f.close();
      d.rename((fname + "~").data(), fname.data());
      return; // ERROR
    }

    // create a progress indicator
    QWidget *w = new QWidget(0, "KPPP: Truncating logfile");
    KProgress *prg = new KProgress(f.size()/2, f.size(), 0, 
				   KProgress::Horizontal, w);
    prg->setGeometry(30, 30, 300-2*30, 24);
    w->setCaption("KPPP: Truncating logfile");
    w->setGeometry((kapp->desktop()->width() - 300)/2, 
		   (kapp->desktop()->height() - 82)/2,
		   300, 84);
    w->show();      

    // copy approx. one half of the logfile
    char buffer[2048];
    f.at(f.size() / 2);
    f.readLine(buffer, sizeof(buffer));
    
    while(!f.atEnd()) {
      prg->setValue(f.at());
      int br = f.readBlock(buffer, sizeof(buffer));
      fnew.writeBlock(buffer, br);
    }
    
    // done
    f.close();
    fnew.close();

    // delete LOG~
    d.remove((fname + "~").data());

    delete w;
  }
}
*/
