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

#include <qlayout.h>
#include <qslider.h>
#include <kbuttonbox.h>
#include "modemcmds.h"
#include <stdlib.h>

#define ADJUSTLABEL(l) l->setMinimumSize(l->sizeHint()); l->setAlignment(AlignLeft|AlignVCenter);

#define ADJUSTEDIT(e) e->setText("XXXXXXXXqy"); e->setMinimumSize(e->sizeHint()); e->setFixedHeight(e->sizeHint().height()); e->setText(""); e->setMaxLength(MODEMSTR_SIZE);

ModemCommands::ModemCommands(QWidget *parent, const char *name)
  : QDialog(parent, name, TRUE, WStyle_Customize|WStyle_NormalBorder)
{
  setCaption(i18n("Edit Modem Commands"));

  const int GRIDROWS = 18;

  // toplevel layout
  QVBoxLayout *tl = new QVBoxLayout(this, 10, 4);
  
  // add grid + frame
  QGridLayout *l1 = new QGridLayout(GRIDROWS, 4);
  tl->addLayout(l1);
  box = new QGroupBox(this, "box");
  l1->addMultiCellWidget(box, 0, GRIDROWS-1, 0, 3);

  initstr = new QLineEdit(this);
  label1 = new QLabel(i18n("Initialization String:"), this);
  ADJUSTEDIT(initstr);
  ADJUSTLABEL(label1);
  l1->addWidget(label1, 1, 1);
  l1->addWidget(initstr, 1, 2);

  initresp = new QLineEdit(this);
  label2 = new QLabel(i18n("Init Response:"), this);
  ADJUSTEDIT(initresp);
  ADJUSTLABEL(label2);
  l1->addWidget(label2, 2, 1);
  l1->addWidget(initresp, 2, 2);

  QHBoxLayout *l2 = new QHBoxLayout;
  l1->addLayout(l2, 3, 2);
  linitslider = new QLabel("MMMM", this);

  linitslider->setFixedWidth(linitslider->sizeHint().width());
  linitslider->setFixedHeight(initstr->sizeHint().height());
  linitslider->setAlignment(AlignCenter);
  linitslider->setFrameStyle(QFrame::WinPanel | QFrame::Sunken);
  linitslider->setLineWidth(2);
  QSlider *initslider = new QSlider(1, 300, 1, 0,
				QSlider::Horizontal, this);
  initslider->setFixedHeight(initslider->sizeHint().height());
  connect(initslider, SIGNAL(valueChanged(int)),
	  linitslider, SLOT(setNum(int)));
  initslider->setValue(gpppdata.modemInitDelay());
  linitslider->setNum(gpppdata.modemInitDelay());
  l2->addWidget(linitslider, 0);
  l2->addWidget(initslider, 1);
  
  label3 = new QLabel(i18n("Init Delay (sec/100):"), this);
  ADJUSTLABEL(label3);  
  l1->addWidget(label3, 3, 1);

  dialstr = new QLineEdit(this);
  label4 = new QLabel(i18n("Dial String:"),this);
  ADJUSTEDIT(dialstr);
  ADJUSTLABEL(label4);
  l1->addWidget(label4, 4, 1);
  l1->addWidget(dialstr, 4, 2);

  connectresp = new QLineEdit(this);
  label5 = new QLabel(i18n("Connect Response:"), this);
  ADJUSTEDIT(connectresp);
  ADJUSTLABEL(label5);
  l1->addWidget(label5, 5, 1);
  l1->addWidget(connectresp, 5, 2);

  busyresp = new QLineEdit(this);
  label6 = new QLabel(i18n("Busy Response:"), this);
  ADJUSTEDIT(busyresp);
  ADJUSTLABEL(label6);
  l1->addWidget(label6, 6, 1);
  l1->addWidget(busyresp, 6, 2);

  nocarrierresp = new QLineEdit(this);
  label7 = new QLabel(i18n("No Carrier Resonse:"), this);
  ADJUSTEDIT(nocarrierresp);
  ADJUSTLABEL(label7);
  l1->addWidget(label7, 7, 1);
  l1->addWidget(nocarrierresp, 7, 2);

  nodialtoneresp = new QLineEdit(this);
  label8 = new QLabel(i18n("No Dialtone Response:"), this);
  ADJUSTEDIT(nodialtoneresp);
  ADJUSTLABEL(label8);
  l1->addWidget(label8, 8, 1);
  l1->addWidget(nodialtoneresp, 8, 2);

  hangupstr = new QLineEdit(this);
  label9 = new QLabel(i18n("Hangup String:"), this);
  ADJUSTEDIT(hangupstr);
  ADJUSTLABEL(label9);
  l1->addWidget(label9, 9, 1);
  l1->addWidget(hangupstr, 9, 2);

  hangupresp = new QLineEdit(this);
  label10 = new QLabel(i18n("Hangup Response:"), this);
  ADJUSTEDIT(hangupresp);
  ADJUSTLABEL(label10);
  l1->addWidget(label10, 10, 1);
  l1->addWidget(hangupresp, 10, 2);

  answerstr = new QLineEdit(this);
  label11 = new QLabel(i18n("Answer String:"), this);
  ADJUSTEDIT(answerstr);
  ADJUSTLABEL(label11);
  l1->addWidget(label11, 11, 1);
  l1->addWidget(answerstr, 11, 2);

  ringresp = new QLineEdit(this);
  label12 = new QLabel(i18n("Ring Response:"), this);
  ADJUSTEDIT(ringresp);
  ADJUSTLABEL(label12);
  l1->addWidget(label12, 12, 1);
  l1->addWidget(ringresp, 12, 2);

  answerresp = new QLineEdit(this);
  label13 = new QLabel(i18n("Answer Response:"), this);
  ADJUSTEDIT(answerresp);
  ADJUSTLABEL(label13);
  l1->addWidget(label13, 13, 1);
  l1->addWidget(answerresp, 13, 2);

  escapestr = new QLineEdit(this);
  label14 = new QLabel(i18n("Escape String:"), this);
  ADJUSTEDIT(escapestr);
  ADJUSTLABEL(label14);
  l1->addWidget(label14, 14, 1);
  l1->addWidget(escapestr, 14, 2);

  escaperesp = new QLineEdit(this);
  label15 = new QLabel(i18n("Escape Response:"), this);
  ADJUSTEDIT(escaperesp);
  ADJUSTLABEL(label15);
  l1->addWidget(label15, 15, 1);
  l1->addWidget(escaperesp, 15, 2);

  // put slider and label into a separate H-Box
  QHBoxLayout *l3 = new QHBoxLayout;
  l1->addLayout(l3, 16, 2);
  lslider = new QLabel("MMMM", this);

  // a little trick to make the label look like a disabled lineedit
  lslider->setFixedWidth(lslider->sizeHint().width());
  lslider->setFixedHeight(escapestr->sizeHint().height());
  lslider->setAlignment(AlignCenter);
  lslider->setFrameStyle(QFrame::WinPanel | QFrame::Sunken);
  lslider->setLineWidth(2);
  QSlider *slider = new QSlider(0, 255, 1, 0,
				QSlider::Horizontal, this);
  slider->setFixedHeight(slider->sizeHint().height());
  connect(slider, SIGNAL(valueChanged(int)),
	  lslider, SLOT(setNum(int)));
  slider->setValue(gpppdata.modemEscapeGuardTime());
  lslider->setNum(gpppdata.modemEscapeGuardTime());
  l3->addWidget(lslider, 0);
  l3->addWidget(slider, 1);
  
  label16 = new QLabel(i18n("Guard Time (sec/50):"), this);
  ADJUSTLABEL(label16);  
  l1->addWidget(label16, 16, 1);

  KButtonBox *bbox = new KButtonBox(this);
  bbox->addStretch();
  ok = bbox->addButton(i18n("OK"));
  ok->setDefault(TRUE);
  cancel = bbox->addButton(i18n("Cancel"));
  
  connect(ok, SIGNAL(clicked()), SLOT(okbutton()));
  connect(cancel, SIGNAL(clicked()), SLOT(cancelbutton()));

  bbox->layout();
  tl->addWidget(bbox);
  
  initstr->setFocus();
  
  l1->addColSpacing(0, 10);
  l1->addColSpacing(3, 10);
  l1->addRowSpacing(0, 5);
  l1->addRowSpacing(GRIDROWS-1, 5);
  tl->freeze();

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

}


void ModemCommands::okbutton() {
  gpppdata.setModemInitStr(initstr->text());
  gpppdata.setModemInitResp(initresp->text());
  gpppdata.setModemInitDelay(atoi(linitslider->text()));

  gpppdata.setModemDialStr(dialstr->text());
  gpppdata.setModemConnectResp(connectresp->text());
  gpppdata.setModemBusyResp(busyresp->text());
  gpppdata.setModemNoCarrierResp(nocarrierresp->text());
  gpppdata.setModemNoDialtoneResp(nodialtoneresp->text());

  gpppdata.setModemEscapeStr(escapestr->text());
  gpppdata.setModemEscapeResp(escaperesp->text());
  gpppdata.setModemEscapeGuardTime(atoi(lslider->text()));
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

#include "modemcmds.moc"
