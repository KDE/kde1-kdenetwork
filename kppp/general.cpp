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

#include <kapp.h>
#include <qlayout.h>
#include "general.h"
#include "version.h"
#include <kintegerline.h>
#include "macros.h"

QString ati_query_strings[NUM_OF_ATI];
extern KApplication*	app;

GeneralWidget::GeneralWidget( QWidget *parent, const char *name)
  : QWidget(parent, name)
{
  QGridLayout *tl = new QGridLayout(this, 10, 4, 10, 10);
  tl->addRowSpacing(0, fontMetrics().lineSpacing() - 10); // magic
  box = new QGroupBox(klocale->translate("kppp Setup"), this,"box");
  tl->addMultiCellWidget(box, 0, 9, 0, 3);
  
//   label1 = new QLabel(this,"path");
//   label1->setText(klocale->translate("pppd Path:"));
//   label1->setMinimumSize(label1->sizeHint());
//   tl->addWidget(label1, 1, 1);

//   //pppd Path Line Edit Box
//   pppdpath = new QLineEdit(this, "pppdpath");
//   pppdpath->setMinimumWidth(pppdpath->sizeHint().width());
//   pppdpath->setFixedHeight(pppdpath->sizeHint().height());
//   pppdpath->setMaxLength(PATH_SIZE);
//   pppdpath->setText(gpppdata.pppdPath());
//   connect(pppdpath, SIGNAL(textChanged(const char*)),
// 	  SLOT(pppdpathchanged(const char*)));
//   tl->addWidget(pppdpath, 1, 2);

  //pppd Timeout Line Edit Box

  label6 = new QLabel(this,"timeout");
  label6->setText(klocale->translate("pppd Timeout:"));
  label6->setMinimumSize(label6->sizeHint());
  tl->addWidget(label6, 1, 1);
  
  QHBoxLayout *l1 = new QHBoxLayout;
  tl->addLayout(l1, 1, 2);
  pppdtimeout = new KIntegerLine(this, "pppdtimeout");
  pppdtimeout->setFixedHeight(pppdtimeout->sizeHint().height());
  pppdtimeout->setMaxLength(TIMEOUT_SIZE);
  pppdtimeout->setText(gpppdata.pppdTimeout());
  connect(pppdtimeout, SIGNAL(textChanged(const char*)),
	  SLOT(pppdtimeoutchanged(const char*)));
  l1->addWidget(pppdtimeout, 1);

  labeltmp = new QLabel(this,"seconds");
  labeltmp->setText(klocale->translate("Seconds"));
  labeltmp->setMinimumSize(labeltmp->sizeHint());
  l1->addWidget(labeltmp, 2);

  logviewerlabel = new QLabel(this,"logviewerlabel");
  logviewerlabel->setText(klocale->translate("Log Viewer:"));
  logviewerlabel->setMinimumSize(logviewerlabel->sizeHint());
  tl->addWidget(logviewerlabel, 2, 1);

  logviewer = new QLineEdit(this, "logvieweredit");
  logviewer->setMinimumWidth(logviewer->sizeHint().width());
  logviewer->setFixedHeight(logviewer->sizeHint().height());
  logviewer->setMaxLength(PATH_SIZE);
  logviewer->setText(gpppdata.logViewer());
  connect(logviewer, SIGNAL(textChanged(const char*)),
	  SLOT(logviewerchanged(const char*)));
  tl->addWidget(logviewer, 2, 2);

  tl->addRowSpacing(3, 5);

  chkbox6 = new QCheckBox(klocale->translate("Dock into Panel on Connect"),this,"dockingbox");
  chkbox6->setMinimumSize(chkbox6->sizeHint());
  chkbox6->setChecked(gpppdata.get_dock_into_panel());
  connect(chkbox6,SIGNAL(toggled(bool)),this,SLOT(docking_toggled(bool)));
  tl->addMultiCellWidget(chkbox6, 4, 4, 1, 2);

  chkbox2 = new QCheckBox(klocale->translate("Automatic Redial on Disconnect"),
			  this,"redialbox");
  chkbox2->setMinimumSize(chkbox2->sizeHint());
  chkbox2->setChecked(gpppdata.get_automatic_redial());
  connect(chkbox2,SIGNAL(toggled(bool)),this,SLOT(redial_toggled(bool)));
  tl->addMultiCellWidget(chkbox2, 5, 5, 1, 2);

  chkbox3 = new QCheckBox(klocale->translate("Show Clock on Caption"),
			  this,"captionbox");
  chkbox3->setMinimumSize(chkbox2->sizeHint());
  chkbox3->setChecked(gpppdata.get_show_clock_on_caption());
  connect(chkbox3,SIGNAL(toggled(bool)),this,SLOT(caption_toggled(bool)));
  tl->addMultiCellWidget(chkbox3, 6, 6, 1, 2);

  chkbox4 = new QCheckBox(klocale->translate("Disconnect on X-server shutdown"),this,"captionbox");
  chkbox4->setMinimumSize(chkbox2->sizeHint());
  chkbox4->setChecked(gpppdata.get_xserver_exit_disconnect());
  connect(chkbox4,SIGNAL(toggled(bool)),this,SLOT(xserver_toggled(bool)));
  tl->addMultiCellWidget(chkbox4, 7, 7, 1, 2);

  chkbox5 = new QCheckBox(klocale->translate("Minimize Window on Connect"),this,"iconifybox");
  chkbox5->setMinimumSize(chkbox5->sizeHint());
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

void GeneralWidget::logviewerchanged(const char *n){

  gpppdata.setlogViewer(n);

}


// void GeneralWidget::pppdpathchanged(const char *n) {

//   gpppdata.setpppdPath(n);

// }



void GeneralWidget::pppdtimeoutchanged(const char *n) {
  gpppdata.setpppdTimeout(n);

}

AboutWidget::AboutWidget( QWidget *parent, const char *name)
  : QWidget(parent, name)
{
  QGridLayout *tl = new QGridLayout(this, 4, 4, 10, 10);
  tl->addRowSpacing(0, fontMetrics().lineSpacing() - 10); // magic
  box = new QGroupBox(this,"box");
  box->setTitle(klocale->translate("About kppp"));
  tl->addMultiCellWidget(box, 0, 3, 0, 3);

  label1 = new QLabel(this, "About");
  label1->setAlignment(AlignLeft|ExpandTabs);

  QString string;
  string = "kppp "KPPPVERSION;
  string += klocale->translate("\nA dialer and front-end to pppd\n\n"
			       "Copyright (c) 1997\nBernd Johannes Wuebben\n"
			       "wuebben@math.cornell.edu");
  label1->setText(string);
  label1->setMinimumSize(label1->sizeHint());
  tl->addMultiCellWidget(label1, 1, 1, 1, 2);

  QString pixdir = app->kde_datadir() + QString("/kppp/pics/");  

  QPixmap pm((pixdir + "kppplogo.xpm").data());
  QLabel *logo = new QLabel(this);
  logo->setPixmap(pm);
  logo->setFixedSize(pm.size());
  tl->addWidget(logo, 2, 1);

  QString string2 = 
    klocale->translate("With contributions from:\n\n"
		       "Mario Weilguni\n"
		       "Markus Wuebben\n"
		       "Jesus Fuentes Saavedra\n"
		       "Harri Porten\n"
		       "Peter Silva");
  
  label2 = new QLabel(this,"About2");
  label2->setAlignment(AlignLeft|ExpandTabs);
  label2->setText(string2);
  label2->setMinimumSize(label2->sizeHint());
  tl->addWidget(label2, 2, 2);
  
  tl->activate();
}



ModemWidget::ModemWidget( QWidget *parent, const char *name)
  : QWidget(parent, name)
{
  QGridLayout *tl = new QGridLayout(this, 10, 4, 10, 10);
  tl->addRowSpacing(0, fontMetrics().lineSpacing() - 10); // magic

  box = new QGroupBox(klocale->translate("Serial device"), this,"box");
  tl->addMultiCellWidget(box, 0, 9, 0, 3);

  label1 = new QLabel(klocale->translate("Modem Device:"), this,"modem");
  label1->setMinimumSize(label1->sizeHint());
  tl->addWidget(label1, 1, 1);
  
  modemdevice = new QComboBox(false,this, "modemdevice");
  modemdevice->insertItem("/dev/modem");
  modemdevice->insertItem("/dev/cua0");
  modemdevice->insertItem("/dev/cua1");
  modemdevice->insertItem("/dev/cua2");
  modemdevice->insertItem("/dev/cua3");
  modemdevice->insertItem("/dev/ttyS0");
  modemdevice->insertItem("/dev/ttyS1");
  modemdevice->insertItem("/dev/ttyS2");
  modemdevice->insertItem("/dev/ttyS3");
  modemdevice->setMinimumWidth(modemdevice->sizeHint().width());
  modemdevice->setFixedHeight(modemdevice->sizeHint().height());
  tl->addWidget(modemdevice, 1, 2);
   
  connect(modemdevice, SIGNAL(activated(int)), SLOT(setmodemdc(int)));

  label2 = new QLabel(klocale->translate("Flow Control:"), this,"Flow");
  label2->setMinimumSize(label2->sizeHint());
  tl->addWidget(label2, 2, 1);

  flowcontrol = new QComboBox(false,this);
  flowcontrol->insertItem("CRTSCTS");
  flowcontrol->insertItem("XON/XOFF");
  flowcontrol->insertItem(klocale->translate("None"));
  flowcontrol->setMinimumWidth(flowcontrol->sizeHint().width());
  flowcontrol->setFixedHeight(flowcontrol->sizeHint().height());
  tl->addWidget(flowcontrol, 2, 2);

  connect(flowcontrol, SIGNAL(activated(int)), SLOT(setflowcontrol(int)));


  labelenter = new QLabel(klocale->translate("Line Termination:"), 
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
  baud_label->setText(klocale->translate("Connection Speed:"));
  MIN_SIZE(baud_label);
  tl->addWidget(baud_label, 4, 1);
  
  QHBoxLayout *l1 = new QHBoxLayout;
  tl->addLayout(l1, 4, 2);
  baud_c = new QComboBox(this, "baud_c");

#ifdef B460800 
  baud_c->insertItem("460800");
#endif

#ifdef B230400
  baud_c->insertItem("230400");
#endif

#ifdef B115200
  baud_c->insertItem("115200");
#endif

#ifdef B57600
  baud_c->insertItem("57600");
#endif

  baud_c->insertItem("38400");
  baud_c->insertItem("19200");
  baud_c->insertItem("9600");
  baud_c->insertItem("2400");
  
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
  label4 = new QLabel(klocale->translate("Modem Lock File:"),
		      this,"modemlockfilelabel");
  label4->setMinimumSize(label4->sizeHint());
  tl->addWidget(label4, 6, 1);

  modemlockfile = new QLineEdit(this, "modemlockfile");
  modemlockfile->setMaxLength(PATH_SIZE);
  modemlockfile->setText("XXXXXXXXXXXX");
  modemlockfile->setMinimumWidth(modemlockfile->sizeHint().width());
  modemlockfile->setFixedHeight(modemlockfile->sizeHint().height());
  modemlockfile->setText(gpppdata.modemLockFile());
  connect(modemlockfile, SIGNAL(textChanged(const char*)),
	  SLOT(modemlockfilechanged(const char*)));
  tl->addWidget(modemlockfile, 6, 2);

  //Modem Timeout Line Edit Box
  label3 = new QLabel(this,"modemtimeoutlabel");
  label3->setText(klocale->translate("Modem Timeout:"));
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

  labeltmp = new QLabel(klocale->translate("Seconds"), this,"seconds");
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

void ModemWidget::modemlockfilechanged(const char *n) {
  gpppdata.setModemLockFile(n);
}

void ModemWidget::modemtimeoutchanged(const char *n) {
  gpppdata.setModemTimeout(n);
}





// Add functions
char *itoa(int n) {
  static char buf[10];
  
  sprintf(buf,"%d",n);  

  return buf;
}



ModemWidget2::ModemWidget2( QWidget *parent, const char *name)
  : QWidget(parent, name)
{
  QGridLayout *tl = new QGridLayout(this, 3, 3, 10, 10);
  tl->addRowSpacing(0, fontMetrics().lineSpacing() - 10); // magic

  box = new QGroupBox(klocale->translate("Modem"), this,"box");
  tl->addMultiCellWidget(box, 0, 2, 0, 2);

  QVBoxLayout *l1 = new QVBoxLayout;
  tl->addLayout(l1, 1, 1);
  tl->setColStretch(1, 1);
  tl->setRowStretch(1, 1);
  l1->addStretch(1);

  QHBoxLayout *l10 = new QHBoxLayout;
  l1->addLayout(l10);
  label4 = new QLabel(this,"busywaitlabel");
  label4->setText(klocale->translate("Busy Wait:"));
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
  labeltmp->setText(klocale->translate("Seconds"));
  labeltmp->setMinimumSize(labeltmp->sizeHint());
  l10->addWidget(labeltmp, 1);
  l10->addStretch(1);

  // the checkboxes
  l1->addSpacing(10);
  l1->addStretch(1);

  QHBoxLayout *l12 = new QHBoxLayout;
  l1->addLayout(l12);
  l12->addStretch(1);
  chkbox1 = 
    new QCheckBox(klocale->translate("Modem Asserts CD Line."), 
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
  modemcmds = new QPushButton(klocale->translate("Modem Commands"), this);
  modeminfo_button = new QPushButton(klocale->translate("Query Modem"), this);
  terminal_button = new QPushButton(klocale->translate("Terminal"), this);
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


//   fline = new QFrame(this,"line");
//   fline->setFrameStyle(QFrame::HLine |QFrame::Sunken);
//   fline->setGeometry(20,195,295,3);

}

void ModemWidget2::modemcmdsbutton() {
  ModemCommands mc(this);
  mc.exec();
}

void ModemWidget2::query_modem() {

  for(int i = 0; i < NUM_OF_ATI; i++){
    ati_query_strings[i] = "";
  }

  modemtrans = new ModemTransfer(this,"modemquery");
  if(  modemtrans->exec() == QDialog::Accepted)
    query_done();

}

void ModemWidget2::query_done(){
    
    ModemInfo mi(NULL,NULL);
    mi.exec();
}

void ModemWidget2::terminal(){
  
  MiniTerm terminal(NULL,NULL);
  terminal.exec();

}

void ModemWidget2::use_cdline_toggled(bool on){
    gpppdata.setUseCDLine(on);
}

void ModemWidget2::busywaitchanged(const char *n) {
  gpppdata.setbusyWait(n);
}


#include "general.moc"
