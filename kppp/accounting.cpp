/* -*- C++ -*-
 *
 *            kPPP: A pppd front end for the KDE project
 *
 * $Id$
 * 
 *            Copyright (C) 1997 Bernd Johannes Wuebben 
 *                   wuebben@math.cornell.edu
 *
 * This file contributed by: Mario Weilguni, <mweilguni@sime.com>
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
#include <kprogress.h>
#include <time.h>

#include "accounting.h"
#include "kpppconfig.h"
#include "pppdata.h"

#ifdef MY_DEBUG
#include <stdio.h>
#endif

// defines the maximum duration until the current costs
// are saved again (to prevent loss due to "kill")
// specifying -1 disables the features
#define UPDATE_TIME    (5*60*1000)

extern PPPData gpppdata;
extern int ibytes;
extern int obytes;

Accounting::Accounting(QObject *parent) : QObject(parent) {

  _total = 0;
  _session = 0;
  acct_timer_id = 0;
  update_timer_id = 0;

  QDate dt = QDate::currentDate();
  LogFileName.sprintf("%s-%4d.log",
		      dt.monthName(dt.month()),
		      dt.year());

  QString fname = QDir::homeDirPath() + "/";
  fname += ACCOUNTING_PATH;
  fname += "/Log/";
  fname += LogFileName;

  LogFileName = fname;
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
    if(newLen < 1) { // changed to < 1     
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


QString timet2qstring(time_t t) {
  QString s;

  s.sprintf("%u", t);
  return s;
}


void Accounting::slotStart() {

  if(!running()) {
    loadCosts();
    _lastcosts = 0.0;
    _lastlen   = 0.0;
    _session = rules.perConnectionCosts();
    rules.setStartTime(QDateTime::currentDateTime());
    acct_timer_id = startTimer(1);
     if(UPDATE_TIME > 0)
       update_timer_id = startTimer(UPDATE_TIME);

    QString s;
    s = timet2qstring(time(0));
    s += ":";
    s += gpppdata.accname();
    s += ":";
    s += rules.currencySymbol();

    logMessage(s, TRUE);
  }
}


void Accounting::slotStop() {

  if(running()) {
    killTimer(acct_timer_id);
    if(update_timer_id != 0)
      killTimer(update_timer_id);
    acct_timer_id = 0;
    update_timer_id = 0;
    
    QString s;
    s.sprintf(":%s:%0.4e:%0.4e:%u:%u\n",
	      timet2qstring(time(0)).data(),
	      session(),
	      total(),
	      ibytes,
	      obytes);

    logMessage(s, FALSE);
    saveCosts();
  }
}


bool Accounting::loadRuleSet(const char *name) {

  if(strlen(name) == 0) {
    rules.load(""); // delete old rules
    return TRUE;
  }

  // load from home directory if file is found there
  QString d = QDir::homeDirPath() + "/";
  d += ACCOUNTING_PATH ;
  d += "/Rules/";
  d += name;


  QFileInfo fl(d.data());
   if(fl.exists()) {
     int ret = rules.load(d.data());
     return (bool)(ret == 0);
   }
  
  // load from KDE directory if file is found there
  d = KApplication::kde_datadir().copy();
  d += "/kppp/Rules/";
  d += name;

  QFileInfo fg(d.data());
   if(fg.exists()) {
     int ret = rules.load(d.data());
     return (bool)(ret == 0);
   }

 return FALSE;
}


void Accounting::logMessage(QString s, bool newline) {
  QFile f(LogFileName.data());

  bool result = f.open(IO_ReadWrite);
  if(result) {
    if(newline) {
      f.at(f.size() - 1);
      char c = 0;
      f.readBlock(&c, 1);
      if(c != '\n')
	f.writeBlock("\n", 1);
    } else
      f.at(f.size() - 1);

    f.writeBlock(s.data(), s.length());
		 
    f.close();
    chown(LogFileName.data(),getuid(),getgid());
    chmod(LogFileName.data(),S_IRUSR | S_IWUSR);
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

  QString prev_account = gpppdata.accname();

  gpppdata.setAccount(accountname);
  gpppdata.setTotalCosts("");

  gpppdata.setAccount(prev_account);

  return TRUE;

}


bool Accounting::saveCosts() {

  if(!rules.name().isNull() && (rules.name().length() > 0)) {
    QString val;
    val.setNum(total());

    gpppdata.setTotalCosts(val);
    gpppdata.save();

    return TRUE;

  } else
    
    return FALSE;
}


bool Accounting::loadCosts() {

  QString val = gpppdata.totalCosts();

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

  QString prev_account = gpppdata.accname();

  gpppdata.setAccount(accountname);
  QString val = gpppdata.totalCosts();

  gpppdata.setAccount(prev_account);
  
  return val;

}

#include "accounting.moc"
