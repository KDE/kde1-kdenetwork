/*
 *
 *            kPPP: A pppd front end for the KDE project
 *
 * $Id$
 * 
 *            Copyright (C) 1997 Bernd Johannes Wuebben 
 *                   wuebben@math.cornell.edu
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

#ifndef _GENERAL_H_
#define _GENERAL_H_

#include <string.h>

#include <qwidget.h>
#include <qpainter.h>
#include <qcombo.h>
#include <qlined.h>
#include <qlabel.h>
#include <qchkbox.h>
#include <qradiobt.h>
#include <qchkbox.h>

#include "modemcmds.h"
#include "pppdata.h"
#include "modeminfo.h"
#include "miniterm.h"

class GeneralWidget : public QWidget {
  
  Q_OBJECT

public:
  
  GeneralWidget( QWidget *parent=0, const char *name=0 );

private slots:
  
//  void 	pppdpathchanged(const char*);
  void 	pppdtimeoutchanged(const char *n);
  //  void 	logviewerchanged(const char*);
  void 	caption_toggled(bool);
  void  iconify_toggled(bool on); 
  void 	redial_toggled(bool on);
  void 	xserver_toggled(bool on);
  void  quit_toggled(bool);
  void  docking_toggled(bool on);


private:

  QGroupBox 	*box;
  //  QLabel 	*label1;

  QLabel 	*label3;
  QLabel 	*label4;
  QLabel 	*label5;
  QLabel 	*label6;
  QLabel 	*labeltmp;

  QCheckBox 	*chkbox1;
  QCheckBox 	*chkbox2;
  QCheckBox 	*chkbox3;
  QCheckBox 	*chkbox4;
  QCheckBox 	*chkbox5;
  QCheckBox 	*chkbox6;
  QCheckBox 	*chkbox7;

  QLineEdit 	*pppdtimeout;
  //  QLineEdit 	*logviewer;
  //  QLabel    	*logviewerlabel;
  //  QLineEdit 	*pppdpath;


};


class ModemWidget : public QWidget {

  Q_OBJECT

public:

  ModemWidget( QWidget *parent=0, const char *name=0 );


private slots:

  void 	setmodemdc(int);
  void 	setflowcontrol(int);
  void 	modemtimeoutchanged(const char*);
  void 	modemlockfilechanged(const char*);
  void 	setenter(int);
  void  speed_selection(int);

private:

  QComboBox 	*enter;
  QGroupBox 	*box;
  QLabel 	*label1;
  QLabel 	*label2;
  QLabel 	*label3;
  QLabel 	*label4;
  QLabel 	*labeltmp;
  QLabel 	*labelenter;
  QComboBox 	*modemdevice;
  QComboBox 	*flowcontrol;
  
  QComboBox *baud_c;
  QLabel *baud_label;

  QLineEdit 	*modemtimeout;
  QLineEdit 	*modemlockfile;
};


class ModemWidget2 : public QWidget {

  Q_OBJECT

public:

  ModemWidget2( QWidget *parent=0, const char *name=0 );


private slots:
  void 	busywaitchanged(const char*);
  void 	use_cdline_toggled(bool);
  void 	modemcmdsbutton();
  void 	terminal();
  void 	query_modem();
  void 	query_done();

private:

  ModemTransfer *modemtrans;
  QLabel 	*label4;
  QLabel 	*labeltmp;
  QGroupBox 	*box;
  QPushButton 	*modemcmds;
  QPushButton 	*modeminfo_button;
  QPushButton 	*terminal_button;
  QFrame 	*fline;
  QLineEdit 	*busywait;
  QCheckBox 	*chkbox1;
};

class AboutWidget : public QWidget {

  Q_OBJECT

public:
  
  AboutWidget( QWidget *parent=0, const char *name=0 );

private:
  
  QGroupBox 	*box;
  QLabel 	*label1;
  QLabel 	*label2;
  QLabel 	*label3;


};


#endif
