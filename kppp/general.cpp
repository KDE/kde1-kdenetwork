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
#include "general.h"
#include "version.h"
#include "kintedit.h"


QString ati_query_strings[NUM_OF_ATI];
extern KApplication*	app;

GeneralWidget::GeneralWidget( QWidget *parent, const char *name)
  : QWidget(parent, name)
{

  box = new QGroupBox(this,"box");
  box->setGeometry(10,10,320,260);
  box->setTitle("kppp Setup");
  
  label1 = new QLabel(this,"path");
  label1->setText("pppd Path:");
  label1->setGeometry(25,45,80,20);

  //pppd Path Line Edit Box
  pppdpath = new QLineEdit(this, "pppdpath");
  pppdpath->setGeometry(130, 40, 150, 23);
  pppdpath->setMaxLength(PATH_SIZE);
  pppdpath->setText(gpppdata.pppdPath());
  connect(pppdpath, SIGNAL(textChanged(const char*)),
	  SLOT(pppdpathchanged(const char*)));

  //pppd Timeout Line Edit Box

  label6 = new QLabel(this,"timeout");
  label6->setText("pppd Timeout:");
  label6->setGeometry(25,85,80,20);

  pppdtimeout = new KIntLineEdit(this, "pppdtimeout");
  pppdtimeout->setGeometry(130, 80, 35, 23);
  pppdtimeout->setMaxLength(TIMEOUT_SIZE);
  pppdtimeout->setText(gpppdata.pppdTimeout());
  connect(pppdtimeout, SIGNAL(textChanged(const char*)),
	  SLOT(pppdtimeoutchanged(const char*)));

  labeltmp = new QLabel(this,"seconds");
  labeltmp->setText("Seconds");
  labeltmp->setGeometry(175,85,50,20);


  logviewerlabel = new QLabel(this,"logviewerlabel");
  logviewerlabel->setText("Log Viewer:");
  logviewerlabel->setGeometry(25,125,80,20);


  logviewer = new QLineEdit(this, "logvieweredit");
  logviewer->setGeometry(130, 120, 150, 23);
  logviewer->setMaxLength(PATH_SIZE);
  logviewer->setText(gpppdata.logViewer());
  connect(logviewer, SIGNAL(textChanged(const char*)),
	  SLOT(logviewerchanged(const char*)));

  chkbox2 = new QCheckBox("Automatic Redial on Disconnect",this,"redialbox");
  chkbox2->adjustSize();
  chkbox2->setGeometry(25,160,chkbox2->width(),chkbox2->height());
  chkbox2->setChecked(gpppdata.get_automatic_redial());
  connect(chkbox2,SIGNAL(toggled(bool)),this,SLOT(redial_toggled(bool)));

  chkbox3 = new QCheckBox("Show Clock on Caption",this,"captionbox");
  chkbox3->adjustSize();
  chkbox3->setGeometry(25,180,160,chkbox3->height());
  chkbox3->setChecked(gpppdata.get_show_clock_on_caption());
  connect(chkbox3,SIGNAL(toggled(bool)),this,SLOT(caption_toggled(bool)));

  chkbox4 = new QCheckBox("Disconnect on X-server shutdown",this,"captionbox");
  chkbox4->adjustSize();
  chkbox4->setGeometry(25,200,240,chkbox4->height());
  chkbox4->setChecked(gpppdata.get_xserver_exit_disconnect());
  connect(chkbox4,SIGNAL(toggled(bool)),this,SLOT(xserver_toggled(bool)));
}

void GeneralWidget::caption_toggled(bool on){

  bool was_on;
  was_on = gpppdata.get_show_clock_on_caption();

  if (on){
    gpppdata.set_show_clock_on_caption(TRUE);
    if(!was_on) // we need to resort to this nonsense since 
                // the toggled signal is emitted when we set 
                // the toggle button in the constructor and we don't want 
                // to save the data once for each CheckBox immediately after starting up
      gpppdata.save();
  }
  else{ /*off*/
    gpppdata.set_show_clock_on_caption(FALSE);
    if(was_on)
      gpppdata.save();
  }


}


void GeneralWidget::redial_toggled(bool on){

  bool was_on;
  was_on = gpppdata.get_automatic_redial();

  if (on){
    gpppdata.set_automatic_redial(TRUE);
    if(!was_on) // we need to resort to this nonsense since 
                // the toggled signal is emitted when we set 
                // the toggle button in the constructor and we don't want 
                // to save the data once for each CheckBox immediately after starting up
      gpppdata.save();
  }
  else{ /*off*/
    gpppdata.set_automatic_redial(FALSE);
    if(was_on)
      gpppdata.save();
  }
}

void GeneralWidget::xserver_toggled(bool on){

  bool was_on;
  was_on = gpppdata.get_xserver_exit_disconnect();

  if (on){
    gpppdata.set_xserver_exit_disconnect(TRUE);
    if(!was_on) // we need to resort to this nonsense since 
                // the toggled signal is emitted when we set 
                // the toggle button in the constructor and we don't want 
                // to save the data once for each CheckBox immediately after starting up
      gpppdata.save();
  }
  else{ /*off*/
    gpppdata.set_xserver_exit_disconnect(FALSE);
    if(was_on)
      gpppdata.save();
  }
}

void GeneralWidget::logviewerchanged(const char *n){

  gpppdata.setlogViewer(n);
  gpppdata.save();

}


void GeneralWidget::pppdpathchanged(const char *n) {

  gpppdata.setpppdPath(n);
  gpppdata.save();

}



void GeneralWidget::pppdtimeoutchanged(const char *n) {
  gpppdata.setpppdTimeout(n);
  gpppdata.save();
}

AboutWidget::AboutWidget( QWidget *parent, const char *name)
  : QWidget(parent, name)
{

  box = new QGroupBox(this,"box");
  box->setGeometry(10,10,320,260);
  box->setTitle("About kppp");

  label1 = new QLabel(this,"About");
  label1->setAlignment(AlignLeft|WordBreak|ExpandTabs);

  QString string;
  string = "kppp "KPPPVERSION" a dialer and front-end to pppd\n\n"\
    "Copyright (c) 1997 Bernd Johannes Wuebben\n"\
    "wuebben@math.cornell.edu\n"\
    "\n";

  label1->setText(string);


  label1->setGeometry(25,45,300,60);

  QString string2;
  string2 = 
    "With contributions from:\n"\
    "Mario Weilguni\n"
    "Markus Wuebben\n"\
    "Jesus Fuentes Saavedra\n\n"\
    "kppp is based in part on ezppp "\
    "(C) 1997\nJay Painter\n";
  
  label2 = new QLabel(this,"About2");
  label2->setAlignment(AlignLeft|WordBreak|ExpandTabs);
  label2->setText(string2);
  label2->setGeometry(150,120,150,130);


  QString pixdir = app->kdedir() + QString("/share/apps/kppp/pics/");  
  QPixmap pm((pixdir + "kppplogo.xpm").data());
  QLabel *logo = new QLabel(this);
  logo->setPixmap(pm);
  logo->setGeometry(30, 130, pm.width(), pm.height());
}



ModemWidget::ModemWidget( QWidget *parent, const char *name)
  : QWidget(parent, name)
{

  box = new QGroupBox(this,"box");
  box->setGeometry(10,10,320,260);
  box->setTitle("Modem Setup");

  label1 = new QLabel(this,"modem");
  label1->setGeometry(30,30,100,30);
  label1->setText("Modem Device:");

  modemdevice = new QComboBox(false,this, "modemdevice");
  modemdevice->setGeometry(155, 30, 150, 25);
  modemdevice->insertItem("/dev/modem");
  modemdevice->insertItem("/dev/cua0");
  modemdevice->insertItem("/dev/cua1");
  modemdevice->insertItem("/dev/cua2");
  modemdevice->insertItem("/dev/cua3");
  connect(modemdevice, SIGNAL(activated(int)), SLOT(setmodemdc(int)));


  label2 = new QLabel(this,"Flow");
  label2->setGeometry(30,65,100,25);
  label2->setText("Flow Control:");

  flowcontrol = new QComboBox(false,this);
  flowcontrol->setGeometry(155, 65, 150, 25);
  flowcontrol->insertItem("CRTSCTS");
  flowcontrol->insertItem("XON/XOFF");
  flowcontrol->insertItem("None");
  connect(flowcontrol, SIGNAL(activated(int)), SLOT(setflowcontrol(int)));


  labelenter = new QLabel(this,"enter");
  labelenter->setText("Enter is:");
  labelenter->setGeometry(30,102,80,20);

  enter = new QComboBox(false,this);
  enter->setGeometry(155, 100, 150, 25);
  enter->insertItem("CR");
  enter->insertItem("LF");
  enter->insertItem("CR/LF");
  connect(enter, SIGNAL(activated(int)), SLOT(setenter(int)));
  
  for(int i=0; i <= enter->count()-1; i++) {
    if(strcmp(gpppdata.enter(), enter->text(i)) == 0)
      enter->setCurrentItem(i);
  }

  //Modem Lock File
  label4 = new QLabel(this,"modemlockfilelabel");
  label4->setGeometry(30,132,100,30);
  label4->setText("Modem Lock File:");

  modemlockfile = new QLineEdit(this, "modemlockfile");
  modemlockfile->setGeometry(155, 134, 150, 23);
  modemlockfile->setMaxLength(PATH_SIZE);
  modemlockfile->setText(gpppdata.modemLockFile());
  connect(modemlockfile, SIGNAL(textChanged(const char*)),
	  SLOT(modemlockfilechanged(const char*)));



  //Modem Timeout Line Edit Box
  label3 = new QLabel(this,"modemtimeoutlabel");
  label3->setGeometry(30,164,100,30);
  label3->setText("Modem Timeout:");

  modemtimeout = new KIntLineEdit(this, "modemtimeout");
  modemtimeout->setGeometry(155, 166, 40, 23);
  modemtimeout->setMaxLength(TIMEOUT_SIZE);
  modemtimeout->setText(gpppdata.modemTimeout());
  connect(modemtimeout, SIGNAL(textChanged(const char*)),
	  SLOT(modemtimeoutchanged(const char*)));

  label4 = new QLabel(this,"busywaitlabel");
  label4->setGeometry(30,196,100,30);
  label4->setText("Busy Wait:");

  busywait = new KIntLineEdit(this, "busywait");
  busywait->setGeometry(155, 198, 40, 23);
  busywait->setMaxLength(TIMEOUT_SIZE);
  busywait->setText(gpppdata.busyWait());
  connect(busywait, SIGNAL(textChanged(const char*)),
	  SLOT(busywaitchanged(const char*)));


  labeltmp = new QLabel(this,"seconds");
  labeltmp->setGeometry(210,164,50,30);
  labeltmp->setText("Seconds");

  labeltmp = new QLabel(this,"seconds");
  labeltmp->setGeometry(210,196,50,30);
  labeltmp->setText("Seconds");


  //set stuff from gpppdata
  for(int i=0; i <= modemdevice->count()-1; i++) {
    if(strcmp(gpppdata.modemDevice(), modemdevice->text(i)) == 0)
      modemdevice->setCurrentItem(i);
  }

  for(int i=0; i <= flowcontrol->count()-1; i++) {
    if(strcmp(gpppdata.flowcontrol(), flowcontrol->text(i)) == 0)
      flowcontrol->setCurrentItem(i);
  }



}

void ModemWidget::setenter(int i) {
  i = i ;  // shut up compiler
  gpppdata.setEnter(enter->text(enter->currentItem()));
  gpppdata.save();
}


void ModemWidget::setmodemdc(int i) {
  gpppdata.setModemDevice(modemdevice->text(i));
  gpppdata.save();
}

void ModemWidget::setflowcontrol(int i) {
  gpppdata.setFlowcontrol(flowcontrol->text(i));
  gpppdata.save();
}

void ModemWidget::modemlockfilechanged(const char *n) {
  gpppdata.setModemLockFile(n);
  gpppdata.save();
}

void ModemWidget::modemtimeoutchanged(const char *n) {
  gpppdata.setModemTimeout(n);
  gpppdata.save();
}

void ModemWidget::busywaitchanged(const char *n) {
  gpppdata.setbusyWait(n);
  gpppdata.save();
}







// Add functions
char *itoa(int n){
  static char buf[10];
  
  sprintf(buf,"%d",n);  

  return buf;
}



ModemWidget2::ModemWidget2( QWidget *parent, const char *name)
  : QWidget(parent, name)
{

  box = new QGroupBox(this,"box");
  box->setGeometry(10,10,320,260);
  box->setTitle("More ...");

  modemcmds = new QPushButton("Modem Commands", this);
  modemcmds->setGeometry(140, 80, 150, 25);
  connect(modemcmds, SIGNAL(clicked()), SLOT(modemcmdsbutton()));

  modeminfo_button = new QPushButton("Query Modem", this);
  modeminfo_button->setGeometry(140, 120, 150, 25);
  connect(modeminfo_button, SIGNAL(clicked()), SLOT(query_modem()));

  terminal_button = new QPushButton("Terminal", this);
  terminal_button->setGeometry(140, 160, 150, 25);
  connect(terminal_button, SIGNAL(clicked()), SLOT(terminal()));



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
