/*
 *            kPPP: A front end for pppd for the KDE project
 *
 * $Id$
 * 
 * Copyright (C) 1997 Bernd Johannes Wuebben 
 * wuebben@math.cornell.edu
 *
 * based on EzPPP:
 * Copyright (C) 1997  Jay Painter
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

#include "modemcmds.h"

ModemCommands::ModemCommands(QWidget *parent=0, const char *name=0)
  : QDialog(parent, name, TRUE, WStyle_Customize|WStyle_NormalBorder)
{
  setCaption("Edit Modem Commands");

  box = new QGroupBox(this, "box");
  box->setGeometry(5,5,330,465);

  ok = new QPushButton("OK", this);
  ok->setGeometry(10, 475, 70, 25);
  connect(ok, SIGNAL(clicked()), SLOT(okbutton()));
  
  cancel = new QPushButton("Cancel", this);
  cancel->setGeometry(90, 475, 70, 25);
  cancel->setFocus();
  connect(cancel, SIGNAL(clicked()), SLOT(cancelbutton()));


  initstr = new QLineEdit(this);
  initstr->setGeometry(160, 15, 150, 25);
  initstr->setMaxLength(MODEMSTR_SIZE);

  label1 = new QLabel(this);
  label1->setGeometry(20,15,135,25);
  label1->setText("Initialization String:");

  initresp = new QLineEdit(this);
  initresp->setGeometry(160, 45, 150, 25);
  initresp->setMaxLength(MODEMSTR_SIZE);

  label2 = new QLabel(this);
  label2->setGeometry(20,45,135,25);
  label2->setText("Init Response:");

  dialstr = new QLineEdit(this);
  dialstr->setGeometry(160, 75, 150, 25);
  dialstr->setMaxLength(MODEMSTR_SIZE);

  label3 = new QLabel(this);
  label3->setGeometry(20,75,135,25);
  label3->setText("Dial String:");

  connectresp = new QLineEdit(this);
  connectresp->setGeometry(160, 105, 150, 25);
  connectresp->setMaxLength(MODEMSTR_SIZE);

  label4 = new QLabel(this);
  label4->setGeometry(20,105,135,25);
  label4->setText("Connect Response:");

  busyresp = new QLineEdit(this);
  busyresp->setGeometry(160, 135, 150, 25);
  busyresp->setMaxLength(MODEMSTR_SIZE);


  label5 = new QLabel(this);
  label5->setGeometry(20,135,135,25);
  label5->setText("Busy Response:");

  nocarrierresp = new QLineEdit(this);
  nocarrierresp->setGeometry(160, 165, 150, 25);
  nocarrierresp->setMaxLength(MODEMSTR_SIZE);

  label6 = new QLabel(this);
  label6->setGeometry(20,165,135,25);
  label6->setText("No Carrier Resonse:");

  nodialtoneresp = new QLineEdit(this);
  nodialtoneresp->setGeometry(160, 195, 150, 25);
  nodialtoneresp->setMaxLength(MODEMSTR_SIZE);

  label7 = new QLabel(this);
  label7->setGeometry(20,195,135,25);
  label7->setText("No Dialtone Response:");

  hangupstr = new QLineEdit(this);
  hangupstr->setGeometry(160, 225, 150, 25);
  hangupstr->setMaxLength(MODEMSTR_SIZE);

  label8 = new QLabel(this);
  label8->setGeometry(20,225,135,25);
  label8->setText("Hangup String:");

  hangupresp = new QLineEdit(this);
  hangupresp->setGeometry(160, 255, 150, 25);
  hangupresp->setMaxLength(MODEMSTR_SIZE);

  label9 = new QLabel(this);
  label9->setGeometry(20,255,135,25);
  label9->setText("Hangup Response:");

  answerstr = new QLineEdit(this);
  answerstr->setGeometry(160, 285, 150, 25);
  answerstr->setMaxLength(MODEMSTR_SIZE);

  label10 = new QLabel(this);
  label10->setGeometry(20,285,135,25);
  label10->setText("Answer String:");

  ringresp = new QLineEdit(this);
  ringresp->setGeometry(160, 315, 150, 25);
  ringresp->setMaxLength(MODEMSTR_SIZE);

  label11 = new QLabel(this);
  label11->setGeometry(20,315,135,25);
  label11->setText("Ring Response:");

  answerresp = new QLineEdit(this);
  answerresp->setGeometry(160, 345, 150, 25);
  answerresp->setMaxLength(MODEMSTR_SIZE);

  label12 = new QLabel(this);
  label12->setGeometry(20,345,135,25);
  label12->setText("Answer Response:");

  escapestr = new QLineEdit(this);
  escapestr->setGeometry(160, 375, 150, 25);
  escapestr->setMaxLength(MODEMSTR_SIZE);

  label13 = new QLabel(this);
  label13->setGeometry(20,375,135,25);
  label13->setText("Escape String:");

  escaperesp = new QLineEdit(this);
  escaperesp->setGeometry(160, 405, 150, 25);
  escaperesp->setMaxLength(MODEMSTR_SIZE);

  label14 = new QLabel(this);
  label14->setGeometry(20,405,135,25);
  label14->setText("Escape Response:");

  escapeguardtime = new QScrollBar( 0, 255, 1, 10, 
			gpppdata.modemEscapeGuardTime(),   // initial value.
			QScrollBar::Horizontal,
			this, "escapeguardtimesb");
  escapeguardtime->setGeometry(204, 435, 108, 25);

  escapeguardtimelcd = new QLCDNumber( 3, this, "lcd" ); 
  connect( escapeguardtime, SIGNAL(valueChanged(int)), 
	    escapeguardtimelcd, SLOT(display(int)) ); 
  escapeguardtimelcd->setGeometry(158, 435, 45, 25);
  escapeguardtimelcd->display(gpppdata.modemEscapeGuardTime());

  label14 = new QLabel(this);
  label14->setGeometry(20,435,135,25);
  label14->setText("Guard Time (sec/50):");

  //set stuff from gpppdata
  initstr->setText(gpppdata.modemInitStr());
  initresp->setText(gpppdata.modemInitResp());

  dialstr->setText(gpppdata.modemDialStr());
  connectresp->setText(gpppdata.modemConnectResp());
  busyresp->setText(gpppdata.modemBusyResp());
  nocarrierresp->setText(gpppdata.modemNoCarrierResp());
  nodialtoneresp->setText(gpppdata.modemNoDialtoneResp());

  escapestr->setText(gpppdata.modemEscapeStr());
  escaperesp->setText(gpppdata.modemEscapeResp());

  hangupstr->setText(gpppdata.modemHangupStr());
  hangupresp->setText(gpppdata.modemHangupResp());

  answerstr->setText(gpppdata.modemAnswerStr());
  ringresp->setText(gpppdata.modemRingResp());
  answerresp->setText(gpppdata.modemAnswerResp());

  this->setFixedSize(340, 505);

}


void ModemCommands::okbutton() {
  gpppdata.setModemInitStr(initstr->text());
  gpppdata.setModemInitResp(initresp->text());

  gpppdata.setModemDialStr(dialstr->text());
  gpppdata.setModemConnectResp(connectresp->text());
  gpppdata.setModemBusyResp(busyresp->text());
  gpppdata.setModemNoCarrierResp(nocarrierresp->text());
  gpppdata.setModemNoDialtoneResp(nodialtoneresp->text());

  gpppdata.setModemEscapeStr(escapestr->text());
  gpppdata.setModemEscapeResp(escaperesp->text());
  gpppdata.setModemEscapeGuardTime(escapeguardtime->value());
  gpppdata.setModemHangupStr(hangupstr->text());
  gpppdata.setModemHangupResp(hangupresp->text());

  gpppdata.setModemAnswerStr(answerstr->text());
  gpppdata.setModemRingResp(ringresp->text());
  gpppdata.setModemAnswerResp(answerresp->text());

  gpppdata.save();
  accept();
}


void ModemCommands::cancelbutton() {
  reject();
}





