/*
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


#include "main.h"

#ifdef COMPILE_PIX
#include "modemnone.h"
#include "modemboth.h"
#include "modemright.h"
#include "modemleft.h"
#endif

#include <kmsgbox.h>


#include "pppstatdlg.h"
#include "config.h"

extern XPPPWidget *p_xppp;
extern KApplication *app;
extern PPPData gpppdata;
extern bool do_stats();
extern bool init_stats();

extern int 	ibytes;
extern int 	ipackets;
extern int 	compressedin;
extern int 	uncompressedin;
extern int 	errorin;
extern int 	obytes;
extern int	opackets;
extern int 	compressed;
extern int 	packetsunc;
extern int 	packetsoutunc;
extern QString  local_ip_address;
extern QString  remote_ip_address;


PPPStatsDlg::PPPStatsDlg(QWidget *parent, const char *name,QWidget *mainwidget)
  : QWidget(parent, name,0 ){


  (void) mainwidget;

  this->setCaption("kppp Statistics");

  box = new QGroupBox("Statistics",this);
  box->setGeometry(5,5,410,305);

  for(int i =0 ; i < 5; i++){
    labela1[i] = new QLabel(this);
    labela1[i]->setGeometry(20,110 + 40*i,80,25);

    labela2[i] = new QLabel(this);
    labela2[i]->setGeometry(105,110 + 40*i,90,25); 
    labela2[i]->setFrameStyle(QFrame::WinPanel | QFrame::Sunken);
//    labela2[i]->setBackgroundColor(white);

    labelb1[i] = new QLabel(this);
    labelb1[i]->setGeometry(220,110 + 40*i,85,25);

    labelb2[i] = new QLabel(this);
    labelb2[i]->setGeometry(310,110 + 40*i,90,25); 
    labelb2[i]->setFrameStyle(QFrame::WinPanel | QFrame::Sunken);
  //  labelb2[i]->setBackgroundColor(white);
  }

  labela1[0]->setText("bytes in");
  labelb1[0]->setText("bytes out");

  labela1[1]->setText("packets in");
  labelb1[1]->setText("packets out");

  labela1[2]->setText("vjcomp in");
  labelb1[2]->setText("vjcomp out");

  labela1[3]->setText("vjunc in");
  labelb1[3]->setText("vjunc out");

  labela1[4]->setText("vjerr");
  labelb1[4]->setText("non-vj");

  ip_address_label1 = new QLabel(this);
  ip_address_label1->setGeometry(200,30,100,25);
  ip_address_label1->setText("Local Addr:");


  ip_address_label2 = new QLineEdit(this);
  ip_address_label2->setGeometry(290,30,110,25);
  ip_address_label2->setFocusPolicy(QWidget::NoFocus);

  ip_address_label3 = new QLabel(this);
  ip_address_label3->setGeometry(200,70,100,25);
  ip_address_label3->setText("Remote Addr:");

  ip_address_label4 = new QLineEdit(this);
  ip_address_label4->setGeometry(290,70,110,25);
  ip_address_label4->setFocusPolicy(QWidget::NoFocus);

  cancelbutton = new QPushButton(this,"cancelbutton");
  cancelbutton->setGeometry(310,320,90,30);
  cancelbutton->setText("OK");
  connect(cancelbutton, SIGNAL(clicked()), this,SLOT(cancel()));

  /*
  if ( !modem_pixmap.loadFromData(Terminal_xpm_data, Terminal_xpm_len) ) {
    QMessageBox::message( "Error", "Could not load Terminal.xpm" );
  }
  if ( !modem_left_pixmap.loadFromData(terminal_left_xpm_data, terminal_left_xpm_len)){
    QMessageBox::message( "Error", "Could not load termleft.xpm" );
  }
  if ( !modem_right_pixmap.loadFromData(terminal_right_xpm_data, terminal_right_xpm_len)){
    QMessageBox::message( "Error", "Could not load termright.xpm" );
  }
  if ( !modem_both_pixmap.loadFromData(terminal_both_xpm_data, terminal_both_xpm_len) ){
    QMessageBox::message( "Error", "Could not load termboth.xpm" );
  }
  */

  QString pixdir = app->kdedir() + QString("/share/apps/kppp/pics/");  

#define PMERROR(pm) \
  QMessageBox::warning(this, "Error", "Could not load " pm "!")

#ifdef COMPILE_PIX
  if ( !big_modem_both_pixmap.loadFromData(modemboth_data, modemboth_len) ){
    PMERROR("modemboth.xpm");
  }
  if ( !big_modem_left_pixmap.loadFromData(modemleft_data, modemleft_len) ){
    PMERROR("modemleft.xpm");
  }
  if ( !big_modem_right_pixmap.loadFromData(modemright_data, modemright_len) ){
    PMERROR("modemright.xpm")
  }
  if ( !big_modem_none_pixmap.loadFromData(modemnone_data, modemnone_len) ){
    PMERROR("modemnone.xpm");
  }
#else
  if ( !big_modem_both_pixmap.load(pixdir + "modemboth.xpm") ){
    PMERROR("modemboth.xpm");
  }
  if ( !big_modem_left_pixmap.load(pixdir + "modemleft.xpm") ){
    PMERROR("modemleft.xpm");
  }
  if ( !big_modem_right_pixmap.load(pixdir + "modemright.xpm") ){
    PMERROR("modemright.xpm");
  }
  if ( !big_modem_none_pixmap.load(pixdir + "modemnone.xpm") ){
    PMERROR("modemnone.xpm");
  }
#endif


  this->setMinimumSize(420,355);
  this->setMaximumSize(420,355);

  clocktimer = new QTimer(this);
  connect(clocktimer, SIGNAL(timeout()), SLOT(timeclick()));

}


PPPStatsDlg::~PPPStatsDlg() {

 clocktimer->stop();
  
}


void PPPStatsDlg::cancel(){
  
  this->hide();

}


void PPPStatsDlg::take_stats(){

  init_stats();
  ips_set = false;
  clocktimer->start(PPP_STATS_INTERVAL);

}

void PPPStatsDlg::stop_stats(){
  
  clocktimer->stop();

}


void PPPStatsDlg::paintEvent (QPaintEvent *e) {

  (void) e;

  paintIcon();

}

void PPPStatsDlg::paintIcon(){

    if((ibytes_last != ibytes) && (obytes_last != obytes)){
      //      bitBlt( box, 35,25, &modem_both_pixmap );    
      bitBlt( box, 30,20, &big_modem_both_pixmap );    
      ibytes_last = ibytes;
      obytes_last = obytes;
      return;
    }
    
    if (ibytes_last != ibytes){
      //      bitBlt( box, 35,25, &modem_left_pixmap );    
      bitBlt( box, 30,20, &big_modem_left_pixmap );    
      ibytes_last = ibytes;
      obytes_last = obytes;
      return;
    }
    
    if(obytes_last != obytes){
      //      bitBlt( box, 35,25, &modem_right_pixmap );    
      bitBlt( box, 30,20, &big_modem_right_pixmap );    
      ibytes_last = ibytes;
      obytes_last = obytes;
      return;
    }
    
    //    bitBlt( box, 35,25, &modem_pixmap );    
    bitBlt( box, 30,20, &big_modem_none_pixmap );    
    ibytes_last = ibytes;
    obytes_last = obytes;

} 
 
void PPPStatsDlg::timeclick() {

  if( this->isVisible()){
    update_data(do_stats());  
    paintIcon();
  }  
}

void PPPStatsDlg::closeEvent( QCloseEvent *e ){

  e->ignore();                            
	
}


void PPPStatsDlg::update_data(bool data_available){

  (void) data_available;
  
  ibytes_string.sprintf("%d",ibytes);
  ipackets_string.sprintf("%d",ipackets);
  compressedin_string.sprintf("%d",compressedin);
  uncompressedin_string.sprintf("%d",uncompressedin);
  errorin_string.sprintf("%d",errorin);
  obytes_string.sprintf("%d",obytes);
  opackets_string.sprintf("%d",opackets);
  compressed_string.sprintf("%d",compressed);
  packetsunc_string.sprintf("%d",packetsunc);
  packetsoutunc_string.sprintf("%d",packetsoutunc);

  labela2[0]->setText(ibytes_string);
  labela2[1]->setText(ipackets_string);
  labela2[2]->setText(compressedin_string);
  labela2[3]->setText(uncompressedin_string);
  labela2[4]->setText(errorin_string);

  labelb2[0]->setText(obytes_string);
  labelb2[1]->setText(opackets_string);
  labelb2[2]->setText(compressed_string);
  labelb2[3]->setText(packetsunc_string);
  labelb2[4]->setText(packetsoutunc_string);

  if(ips_set == false){

    // if I don't resort to this trick it is imposible to 
    // copy/paste the ip out of the lineedits due to 
    // reset of cursor position on setText()

    if( !local_ip_address.isEmpty() ){
      ip_address_label2->setText(local_ip_address);
    }
    else{
      ip_address_label2->setText("unavailable");
    }

    if( !remote_ip_address.isEmpty() ){
      ip_address_label4->setText(remote_ip_address);
    }
    else{
      ip_address_label4->setText("unavailable");
    }
    ips_set = true;
  }

}





