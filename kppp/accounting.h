/* -*- C++ -*-
 *            kPPP: A pppd front end for the KDE project
 *
 * $Id$
 *
 *            Copyright (C) 1997 Bernd Johannes Wuebben
 *                   wuebben@math.cornell.edu
 * This file contributed by: Mario Weilguni, <mweilguni@sime.com>
 *
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

#ifndef __ACCOUNTING__H__
#define __ACCOUNTING__H__

#include <qobject.h>
#include <qmsgbox.h>
#include <qsocketnotifier.h>
#include "ruleset.h"

class Accounting : public QObject {
  Q_OBJECT
public:

  Accounting(QObject *parent = 0);
  ~Accounting();

  bool running();
  bool loadRuleSet(const char *name);
  double total();
  double session();

protected:
  void timerEvent(QTimerEvent *);
  void logMessage(QString, bool = FALSE);
  bool saveCosts();
  bool loadCosts();

signals:
  void changed(QString total, QString session);

public slots:
  void resetCosts(const char *accountname);
  void slotStart();
  void slotStop();

private:
  RuleSet rules;
  QString LogFileName;
  double _total, _session;
  double _lastcosts;
  double _lastlen;
  int acct_timer_id, update_timer_id;

  // static members
public:
  static QString getCosts(const char* accountname);  
};

#endif

