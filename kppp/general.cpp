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
#include <kapp.h>
#include <qlayout.h>
#include <kintegerline.h>
#include <kquickhelp.h>
#include "general.h"
#include "version.h"
#include "macros.h"
#include "log.h"

GeneralWidget::GeneralWidget( QWidget *parent, const char *name)
  : QWidget(parent, name)
{
  QGridLayout *tl = new QGridLayout(this, 10, 4, 10, 10);
  tl->addRowSpacing(0, fontMetrics().lineSpacing() - 10); // magic
  box = new QGroupBox(i18n("kppp Setup"), this,"box");
  tl->addMultiCellWidget(box, 0, 9, 0, 3);
  
  label6 = new QLabel(this,"timeout");
  label6->setText(i18n("pppd Timeout:"));
  label6->setMinimumSize(label6->sizeHint());
  tl->addWidget(label6, 1, 1);
  
  QHBoxLayout *l1 = new QHBoxLayout;
  tl->addLayout(l1, 1, 2);
  pppdtimeout = new KIntegerLine(this, "pppdtimeout");
  pppdtimeout->setFixedHeight(pppdtimeout->sizeHint().height());
  pppdtimeout->setMaxLength(TIMEOUT_SIZE);
  pppdtimeout->setMinimumWidth(pppdtimeout->sizeHint().width()/4);
  pppdtimeout->setText(gpppdata.pppdTimeout());
  connect(pppdtimeout, SIGNAL(textChanged(const char*)),
	  SLOT(pppdtimeoutchanged(const char*)));
  l1->addWidget(pppdtimeout, 1);

  labeltmp = new QLabel(this,"seconds");
  labeltmp->setText(i18n("Seconds"));
  labeltmp->setMinimumSize(labeltmp->sizeHint());
  l1->addWidget(labeltmp, 2);

  tl->addRowSpacing(2, 5);

  chkbox6 = new QCheckBox(i18n("Dock into Panel on Connect"),this,"dockingbox");
  KQuickHelp::add(chkbox6, "<+><bold>Dock into Panel on Connect<-><bold>\n\nAfter a <red>connection<black> is established,\nthe window is \"docked\" into the panel.");
  MIN_HEIGHT(chkbox6);
  chkbox6->setChecked(gpppdata.get_dock_into_panel());
  connect(chkbox6,SIGNAL(toggled(bool)),this,SLOT(docking_toggled(bool)));
  tl->addMultiCellWidget(chkbox6, 3, 3, 1, 2);

  chkbox2 = new QCheckBox(i18n("Automatic Redial on Disconnect"),
			  this,"redialbox");
  MIN_HEIGHT(chkbox2);
  chkbox2->setChecked(gpppdata.automatic_redial());
  connect(chkbox2,SIGNAL(toggled(bool)),this,SLOT(redial_toggled(bool)));
  tl->addMultiCellWidget(chkbox2, 4, 4, 1, 2);

  chkbox3 = new QCheckBox(i18n("Show Clock on Caption"),
			  this,"captionbox");
  MIN_HEIGHT(chkbox3);
  chkbox3->setChecked(gpppdata.get_show_clock_on_caption());
  connect(chkbox3,SIGNAL(toggled(bool)),this,SLOT(caption_toggled(bool)));
  tl->addMultiCellWidget(chkbox3, 5, 5, 1, 2);

  chkbox4 = new QCheckBox(i18n("Disconnect on X-server shutdown"),this,"captionbox");
  MIN_HEIGHT(chkbox4);
  chkbox4->setChecked(gpppdata.get_xserver_exit_disconnect());
  connect(chkbox4,SIGNAL(toggled(bool)),this,SLOT(xserver_toggled(bool)));
  tl->addMultiCellWidget(chkbox4, 6, 6, 1, 2);

  chkbox7 = new QCheckBox(i18n("Quit on Disconnect"),this,"quitbox");
  MIN_HEIGHT(chkbox7);
  chkbox7->setChecked(gpppdata.quit_on_disconnect());
  connect(chkbox7,SIGNAL(toggled(bool)),this,SLOT(quit_toggled(bool)));
  tl->addMultiCellWidget(chkbox7, 7, 7, 1, 2);

  chkbox5 = new QCheckBox(i18n("Minimize Window on Connect"),this,"iconifybox");
  MIN_HEIGHT(chkbox5);
  chkbox5->setChecked(gpppdata.get_iconify_on_connect());
  connect(chkbox5,SIGNAL(toggled(bool)),this,SLOT(iconify_toggled(bool)));
  tl->addMultiCellWidget(chkbox5, 8, 8, 1, 2);

  tl->activate();
}


void GeneralWidget::docking_toggled(bool on){
  gpppdata.set_dock_into_panel(on);
}


void GeneralWidget::iconify_toggled(bool on){
  gpppdata.set_iconify_on_connect(on);
}

 
void GeneralWidget::caption_toggled(bool on){
  gpppdata.set_show_clock_on_caption(on);
}


void GeneralWidget::redial_toggled(bool on){
  gpppdata.set_automatic_redial(on);
}


void GeneralWidget::xserver_toggled(bool on){
  gpppdata.set_xserver_exit_disconnect(on);
}


void GeneralWidget::quit_toggled(bool on){
  gpppdata.set_quit_on_disconnect(on);
}


void GeneralWidget::pppdtimeoutchanged(const char *n) {
  gpppdata.setpppdTimeout(n);

}


AboutWidget::AboutWidget( QWidget *parent, const char *name)
  : QWidget(parent, name)
{
  QGridLayout *tl = new QGridLayout(this, 3, 3, 10, 10);
  tl->addRowSpacing(0, fontMetrics().lineSpacing() - 10); // magic
  box = new QGroupBox(this,"box");
  box->setTitle(i18n("About kppp"));
  tl->addMultiCellWidget(box, 0, 2, 0, 2);

  label1 = new QLabel(this, "About");
  label1->setAlignment(AlignLeft|ExpandTabs);

  QString string;
  string = "kppp "KPPPVERSION;
  string += i18n("\nA dialer and front-end to pppd\n\n"
		 "(c) 1997, 1998\n"
		 "    Bernd Johannes Wuebben <wuebben@kde.org>\n"
		 "    Harri Porten <porten@kde.org>\n"
		 "    Mario Weilguni <mweilguni@kde.org>\n\n"
		 "Currently maintained by Harri Porten and Mario\n"
		 "Weilguni. Please send all bug reports to the\n"
		 "current maintainer.\n\n"
		 "This program is distributed under the GNU GPL\n"
		 "(GNU General Public License)."
		 );
  label1->setText(string);
  label1->setMinimumSize(label1->sizeHint());
  tl->addWidget(label1, 1, 1);

  QString pixdir = KApplication::kde_datadir() +"/kppp/pics/";  
  
  tl->activate();
}



ModemWidget::ModemWidget( QWidget *parent, const char *name)
  : QWidget(parent, name)
{
  int k;

  QGridLayout *tl = new QGridLayout(this, 11, 4, 10, 10);
  tl->addRowSpacing(0, fontMetrics().lineSpacing() - 10); // magic

  box = new QGroupBox(i18n("Serial device"), this,"box");
  tl->addMultiCellWidget(box, 0, 9, 0, 3);

  label1 = new QLabel(i18n("Modem Device:"), this,"modem");
  label1->setMinimumSize(label1->sizeHint());
  tl->addWidget(label1, 1, 1);
  
  modemdevice = new QComboBox(false,this, "modemdevice");

  static char *devices[] = {
#ifdef __FreeBSD__
  "/dev/cuaa0",
  "/dev/cuaa1",
  "/dev/cuaa2",
  "/dev/cuaa3",
#else
  "/dev/modem",
  "/dev/cua0",
  "/dev/cua1",
  "/dev/cua2",
  "/dev/cua3",
  "/dev/ttyS0",
  "/dev/ttyS1",
  "/dev/ttyS2",
  "/dev/ttyS3",

#ifndef NOISDNSUPPORT
  "/dev/ttyI0",
  "/dev/ttyI1",
  "/dev/ttyI2",
  "/dev/ttyI3",
#endif
#endif
  0};

  for(k = 0; devices[k]; k++)
    modemdevice->insertItem(devices[k]);

  modemdevice->setMinimumWidth(modemdevice->sizeHint().width());
  modemdevice->setFixedHeight(modemdevice->sizeHint().height());
  tl->addWidget(modemdevice, 1, 2);
   
  connect(modemdevice, SIGNAL(activated(int)), SLOT(setmodemdc(int)));

  label2 = new QLabel(i18n("Flow Control:"), this,"Flow");
  label2->setMinimumSize(label2->sizeHint());
  tl->addWidget(label2, 2, 1);

  flowcontrol = new QComboBox(false,this);
  flowcontrol->insertItem("CRTSCTS");
  flowcontrol->insertItem("XON/XOFF");
  flowcontrol->insertItem(i18n("None"));
  flowcontrol->setMinimumWidth(flowcontrol->sizeHint().width());
  flowcontrol->setFixedHeight(flowcontrol->sizeHint().height());
  tl->addWidget(flowcontrol, 2, 2);

  connect(flowcontrol, SIGNAL(activated(int)), SLOT(setflowcontrol(int)));


  labelenter = new QLabel(i18n("Line Termination:"), 
			  this,"enter");
  labelenter->setMinimumSize(labelenter->sizeHint());
  tl->addWidget(labelenter, 3, 1);

  enter = new QComboBox(false,this);
  enter->insertItem("CR");
  enter->insertItem("LF");
  enter->insertItem("CR/LF");
  enter->setMinimumWidth(enter->sizeHint().width());
  enter->setFixedHeight(enter->sizeHint().height());
  tl->addWidget(enter, 3, 2);
  connect(enter, SIGNAL(activated(int)), SLOT(setenter(int)));

  baud_label = new QLabel(this);
  baud_label->setText(i18n("Connection Speed:"));
  MIN_SIZE(baud_label);
  tl->addWidget(baud_label, 4, 1);
  
  QHBoxLayout *l1 = new QHBoxLayout;
  tl->addLayout(l1, 4, 2);
  baud_c = new QComboBox(this, "baud_c");

  static char *baudrates[] = {
    
#ifdef B460800 
    "460800",
#endif

#ifdef B230400
    "230400",
#endif

#ifdef B115200
    "115200",
#endif

#ifdef B57600
    "57600",
#endif

    "38400",
    "19200",
    "9600",
    "2400",
    0};

  for(k = 0; baudrates[k]; k++)
    baud_c->insertItem(baudrates[k]);
  
  baud_c->setCurrentItem(3);
  connect(baud_c, SIGNAL(activated(int)),
	  this, SLOT(speed_selection(int)));
  FIXED_HEIGHT(baud_c);
  MIN_WIDTH(baud_c);
  l1->addWidget(baud_c);
  l1->addStretch(1);

  for(int i=0; i <= enter->count()-1; i++) {
    if(strcmp(gpppdata.enter(), enter->text(i)) == 0)
      enter->setCurrentItem(i);
  }

  //Modem Lock File
  label4 = new QLabel(i18n("Lock File Directory:"),
		      this,"modemlockdirlabel");
  label4->setMinimumSize(label4->sizeHint());
  tl->addWidget(label4, 6, 1);

  modemlockdir = new QLineEdit(this, "modemlockdir");
  modemlockdir->setMaxLength(PATH_SIZE);
  modemlockdir->setText("XXXXXXXXXXXX");
  modemlockdir->setMinimumWidth(modemlockdir->sizeHint().width());
  modemlockdir->setFixedHeight(modemlockdir->sizeHint().height());
  modemlockdir->setText(gpppdata.modemLockDir());
  connect(modemlockdir, SIGNAL(textChanged(const char*)),
	  SLOT(modemlockdirchanged(const char*)));
  tl->addWidget(modemlockdir, 6, 2);

  //Modem Timeout Line Edit Box
  label3 = new QLabel(this,"modemtimeoutlabel");
  label3->setText(i18n("Modem Timeout:"));
  label3->setMinimumSize(label3->sizeHint());
  tl->addWidget(label3, 7, 1);

  QHBoxLayout *l2 = new QHBoxLayout;
  tl->addLayout(l2, 7, 2);

  modemtimeout = new KIntegerLine(this, "modemtimeout");
  modemtimeout->setFixedHeight(modemtimeout->sizeHint().height());
  modemtimeout->setMaxLength(TIMEOUT_SIZE);
  modemtimeout->setText(gpppdata.modemTimeout());
  connect(modemtimeout, SIGNAL(textChanged(const char*)),
	  SLOT(modemtimeoutchanged(const char*)));  
  l2->addWidget(modemtimeout, 1);

  labeltmp = new QLabel(i18n("Seconds"), this,"seconds");
  labeltmp->setMinimumSize(labeltmp->sizeHint());
  l2->addWidget(labeltmp, 2);

  //set stuff from gpppdata
  for(int i=0; i <= modemdevice->count()-1; i++) {
    if(strcmp(gpppdata.modemDevice(), modemdevice->text(i)) == 0)
      modemdevice->setCurrentItem(i);
  }

  for(int i=0; i <= flowcontrol->count()-1; i++) {
    if(strcmp(gpppdata.flowcontrol(), flowcontrol->text(i)) == 0)
      flowcontrol->setCurrentItem(i);
  }     

  //set the modem speed
  for(int i=0; i < baud_c->count(); i++)
    if(strcmp(baud_c->text(i), gpppdata.speed()) == 0)
      baud_c->setCurrentItem(i);

  tl->activate();
}


void ModemWidget::speed_selection(int) {
  gpppdata.setSpeed(baud_c->text(baud_c->currentItem()));
}


void ModemWidget::setenter(int ) {
  gpppdata.setEnter(enter->text(enter->currentItem()));
}


void ModemWidget::setmodemdc(int i) {
  gpppdata.setModemDevice(modemdevice->text(i));
}


void ModemWidget::setflowcontrol(int i) {
  gpppdata.setFlowcontrol(flowcontrol->text(i));
}


void ModemWidget::modemlockdirchanged(const char *n) {
  gpppdata.setModemLockDir(n);
}


void ModemWidget::modemtimeoutchanged(const char *n) {
  gpppdata.setModemTimeout(n);
}


ModemWidget2::ModemWidget2( QWidget *parent, const char *name)
  : QWidget(parent, name)
{
  QGridLayout *tl = new QGridLayout(this, 3, 3, 10, 10);
  tl->addRowSpacing(0, fontMetrics().lineSpacing() - 10); // magic

  box = new QGroupBox(i18n("Modem"), this,"box");
  tl->addMultiCellWidget(box, 0, 2, 0, 2);

  QVBoxLayout *l1 = new QVBoxLayout;
  tl->addLayout(l1, 1, 1);
  tl->setColStretch(1, 1);
  tl->setRowStretch(1, 1);
  l1->addStretch(1);

  QHBoxLayout *l10 = new QHBoxLayout;
  l1->addLayout(l10);
  label4 = new QLabel(this,"busywaitlabel");
  label4->setText(i18n("Busy Wait:"));
  label4->setMinimumSize(label4->sizeHint());
  l10->addStretch(1);
  l10->addWidget(label4);

  busywait = new KIntegerLine(this, "busywait");
  busywait->setFixedHeight(busywait->sizeHint().height());
  busywait->setMaxLength(TIMEOUT_SIZE);
  busywait->setText(gpppdata.busyWait());
  busywait->setMinimumWidth(busywait->sizeHint().width()/3);
  connect(busywait, SIGNAL(textChanged(const char*)),
	  SLOT(busywaitchanged(const char*)));
  l10->addWidget(busywait);

  labeltmp = new QLabel(this,"seconds");
  labeltmp->setText(i18n("Seconds"));
  labeltmp->setMinimumSize(labeltmp->sizeHint());
  l10->addWidget(labeltmp, 1);
  l10->addStretch(1);

  // the checkboxes
  l1->addSpacing(10);
  l1->addStretch(1);

  QHBoxLayout *hbl = new QHBoxLayout;
  l1->addLayout(hbl);
  QLabel *volumeLabel = new QLabel(i18n("Modem volume"), this);
  volumeLabel->setAlignment(AlignVCenter|AlignRight);
  MIN_SIZE(volumeLabel);
  hbl->addStretch(1);
  hbl->addWidget(volumeLabel);
  volume = new KSlider(0, 2, 1, gpppdata.volume(), KSlider::Horizontal, this);
  volume->setFixedSize(120, 25);  
  hbl->addWidget(volume);
  hbl->addStretch(1);
  connect(volume, SIGNAL(valueChanged(int)),
	  this, SLOT(volumeChanged(int)));

  QHBoxLayout *l12 = new QHBoxLayout;
  l1->addLayout(l12);
  l12->addStretch(1);
  chkbox1 = 
    new QCheckBox(i18n("Modem Asserts CD Line."), 
		  this, "assertCD");
  chkbox1->setMinimumSize(chkbox1->sizeHint());
  chkbox1->setChecked(gpppdata.UseCDLine());
  connect(chkbox1,SIGNAL(toggled(bool)),this,SLOT(use_cdline_toggled(bool)));
  l12->addWidget(chkbox1);
  l12->addStretch(1);
  l1->addStretch(1);


  // add the buttons 
  QHBoxLayout *l11 = new QHBoxLayout;
  l1->addLayout(l11); 
  l11->addStretch(1);
  QVBoxLayout *l111 = new QVBoxLayout;
  l11->addLayout(l111);
  modemcmds = new QPushButton(i18n("Modem Commands"), this);
  modeminfo_button = new QPushButton(i18n("Query Modem"), this);
  terminal_button = new QPushButton(i18n("Terminal"), this);
  modemcmds->setMinimumWidth(modemcmds->sizeHint().width());
  modemcmds->setFixedHeight(modemcmds->sizeHint().height());
  modeminfo_button->setMinimumWidth(modeminfo_button->sizeHint().width());
  modeminfo_button->setFixedHeight(modeminfo_button->sizeHint().height());
  terminal_button->setMinimumWidth(terminal_button->sizeHint().width());
  terminal_button->setFixedHeight(terminal_button->sizeHint().height());
  l111->addWidget(modemcmds);
  l111->addWidget(modeminfo_button);
  l111->addWidget(terminal_button);
  l11->addStretch(1);
  l1->addStretch(1);

  tl->activate();

  connect(modemcmds, SIGNAL(clicked()), SLOT(modemcmdsbutton()));
  connect(modeminfo_button, SIGNAL(clicked()), SLOT(query_modem()));
  connect(terminal_button, SIGNAL(clicked()), SLOT(terminal()));
}


void ModemWidget2::modemcmdsbutton() {
  ModemCommands mc(this);
  mc.exec();
}


void ModemWidget2::query_modem() {  
  modemtrans = new ModemTransfer(this);
  modemtrans->exec();
  delete modemtrans;
}


void ModemWidget2::terminal() {
    MiniTerm terminal(NULL,NULL);
  terminal.exec();
}


void ModemWidget2::use_cdline_toggled(bool on) {
    gpppdata.setUseCDLine(on);
}


void ModemWidget2::busywaitchanged(const char *n) {
  gpppdata.setbusyWait(n);
}

void ModemWidget2::volumeChanged(int v) {
  gpppdata.setVolume(v);
}


#include "general.moc"
