/*
 *              kPPP: A pppd Front End for the KDE project
 *
 * $Id$
 *              Copyright (C) 1997 Bernd Johannes Wuebben
 *                      wuebben@math.cornell.edu
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

#include "edit.h"
#include "termios.h"
#include <qlayout.h>

extern bool isnewaccount;

#define MIN_SIZE(w) w->setMinimumSize(w->sizeHint());
#define FIXED_SIZE(w) w->setFixedSize(w->sizeHint());
#define FIXED_HEIGHT(w) w->setFixedHeight(w->sizeHint().height());
#define MIN_WIDTH(w) w->setMinimumWidth(w->sizeHint().width());

DialWidget::DialWidget( QWidget *parent, const char *name )
  : QWidget(parent, name)
{
  const int GRIDROWS = 11;

  QGridLayout *tl = new QGridLayout(this, GRIDROWS, 4, 10, 10);
  tl->addRowSpacing(0, fontMetrics().lineSpacing() - 10);
  box = new QGroupBox(this,"box");
  box->setTitle(klocale->translate("Dial Setup"));
  tl->addMultiCellWidget(box, 0, GRIDROWS-1, 0, 3);

  connect_label = new QLabel(this);
  connect_label->setText(klocale->translate("Connection Name:"));
  MIN_SIZE(connect_label);
  tl->addWidget(connect_label, 1, 1);

  connectname_l = new QLineEdit(this, "connectname_l");
  connectname_l->setMaxLength(ACCNAME_SIZE);
  FIXED_HEIGHT(connectname_l);
  MIN_WIDTH(connectname_l);
  tl->addWidget(connectname_l, 1, 2);
  
  number_label = new QLabel(this);
  number_label->setText(klocale->translate("Phone Number:"));
  MIN_SIZE(number_label);
  tl->addWidget(number_label, 2, 1);

  number_l = new QLineEdit(this, "number_l");
  number_l->setMaxLength(PHONENUMBER_SIZE);
  MIN_WIDTH(number_l);
  FIXED_HEIGHT(number_l);
  tl->addWidget(number_l, 2, 2);

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
  FIXED_HEIGHT(baud_c);
  MIN_WIDTH(baud_c);
  l1->addWidget(baud_c);
  l1->addStretch(1);

  command_label = new QLabel(this);
  command_label->setText(klocale->translate("Execute Program\nupon Connect:"));
  command_label->setAlignment(AlignVCenter);
  MIN_SIZE(command_label);
  tl->addWidget(command_label, 6, 1);

  command = new QLineEdit(this);
  command->setMaxLength(COMMAND_SIZE);
  FIXED_HEIGHT(command);
  MIN_WIDTH(command);
  tl->addWidget(command, 6, 2);

  pppd_label = new QLabel(this);
  pppd_label->setText(klocale->translate("Edit default pppd:"));
  MIN_SIZE(pppd_label);
  tl->addWidget(pppd_label, 8, 1);

  QHBoxLayout *l2 = new QHBoxLayout;
  tl->addLayout(l2, 8, 2);
  pppdargs = new QPushButton(klocale->translate("Arguments"), this);
  connect(pppdargs, SIGNAL(clicked()), SLOT(pppdargsbutton()));
  MIN_SIZE(pppdargs);
  FIXED_HEIGHT(pppdargs);
  l2->addWidget(pppdargs);
  l2->addStretch(3);

  // Set defaults if editing an exhisting connection
  if(!isnewaccount) {
    connectname_l->setText(gpppdata.accname());
    number_l->setText(gpppdata.phonenumber());
    command->setText(gpppdata.command());

    //set the modem speed
    for(int i=0; i < baud_c->count(); i++)
      if(strcmp(baud_c->text(i), gpppdata.speed()) == 0)
	baud_c->setCurrentItem(i);
  }

  //  this->setFixedSize(340,322);
  tl->activate();
}



bool DialWidget::save() {

  //first check to make sure that the account name is unique!
  if(strcmp(connectname_l->text(), "") == 0 ||
     !gpppdata.isUniqueAccname(connectname_l->text())) {
    return false;
  }
  else {
    gpppdata.setAccname(connectname_l->text());
    gpppdata.setPhonenumber(number_l->text());
    gpppdata.setSpeed(baud_c->text(baud_c->currentItem()));
    gpppdata.setCommand(command->text());
    return true;
  }
}


void DialWidget::pppdargsbutton() {
  PPPdArguments pa(this);
  pa.exec();
}


//
// IPWidget
//
IPWidget::IPWidget( QWidget *parent, const char *name )
  : QWidget(parent, name)
{

  box1 = new QGroupBox(this,"box1");
  box1->setGeometry(10,10,345,310);
  box1->setTitle(klocale->translate("IP Setup"));

  box = new QGroupBox(this,"box");
  box->setGeometry(50,87,270,120);
  box->setTitle("          ");

  ipaddress_l = new IPLineEdit(this, "ipaddress_l");
  ipaddress_l->setGeometry(160, 120, 110, 25);
  ipaddress_l->setMaxLength(IPADDR_SIZE);

  ipaddress_label = new QLabel(this);
  ipaddress_label->setGeometry(70,120,80,30);
  ipaddress_label->setText(klocale->translate("IP Address:"));

  subnetmask_l = new IPLineEdit(this, "subnetmask_l");
  subnetmask_l->setGeometry(160, 150, 110, 25);
  subnetmask_l->setMaxLength(IPADDR_SIZE);

  sub_label = new QLabel(this);
  sub_label->setGeometry(70,150,80,30);
  sub_label->setText(klocale->translate("Subnet Mask:"));

  rb = new QButtonGroup(this, "rb");
  rb->setFrameStyle(QFrame::NoFrame);
  rb->setGeometry(75, 40, 135, 70);
  connect(rb, SIGNAL(clicked(int)), SLOT(hitIPSelect(int)));

  dynamicadd_rb = new QRadioButton(rb, "dynamicadd_rb");
  dynamicadd_rb->setText(klocale->translate("Dynamic IP Address"));
  dynamicadd_rb->setGeometry(0, 0, 200, 30);

  staticadd_rb = new QRadioButton(rb, "dynamicadd_rb");
  staticadd_rb->setText(klocale->translate("Static IP Address"));
  staticadd_rb->setGeometry(0, 40, 200, 30);

  autoname=new QCheckBox(klocale->translate("Auto-configure hostname from this IP"),
	this,"autoname");
  autoname->adjustSize();
  autoname->setGeometry(75,260,250,autoname->height());
  autoname->setChecked(gpppdata.autoname());
  connect(autoname,SIGNAL(toggled(bool)),this,SLOT(autoname_t(bool)));

  //load info from gpppdata
  if(!isnewaccount) {
    if(strcmp(gpppdata.ipaddr(),"0.0.0.0")==0 && 
       strcmp(gpppdata.subnetmask(),"0.0.0.0")==0) {
      dynamicadd_rb->setChecked(TRUE);
      hitIPSelect(0);
      autoname->setChecked(gpppdata.autoname());
    }
    else {
      ipaddress_l->setText(gpppdata.ipaddr());
      subnetmask_l->setText(gpppdata.subnetmask());
      staticadd_rb->setChecked(TRUE);
      autoname->setChecked(FALSE);
    }
  }
  else {
    dynamicadd_rb->setChecked(TRUE);
    hitIPSelect(0);
  }

}

void IPWidget::autoname_t(bool on){
  on=on;
}

void IPWidget::save() {
  gpppdata.setIpaddr(ipaddress_l->text());
  gpppdata.setSubnetmask(subnetmask_l->text());
  gpppdata.setAutoname(autoname->isChecked()); 
}


void IPWidget::hitIPSelect( int i ) {
  if(i == 0) {
    ipaddress_label->setEnabled(FALSE);
    sub_label->setEnabled(FALSE);
    ipaddress_l->setText("0.0.0.0");
    ipaddress_l->setEnabled(FALSE);
    subnetmask_l->setText("0.0.0.0");
    subnetmask_l->setEnabled(FALSE);
  }
  else {

    ipaddress_label->setEnabled(TRUE);
    sub_label->setEnabled(TRUE);
    ipaddress_l->setEnabled(TRUE);
    ipaddress_l->setText("");
    subnetmask_l->setEnabled(TRUE);
    subnetmask_l->setText("");
  }
}



DNSWidget::DNSWidget( QWidget *parent, const char *name )
  : QWidget(parent, name)
{
  QGridLayout *tl = new QGridLayout(this, 3, 3, 10, 10);
  tl->addRowSpacing(0, fontMetrics().lineSpacing() - 10);
  box = new QGroupBox(this,"box");
  box->setTitle(klocale->translate("DNS Servers"));
  tl->addMultiCellWidget(box, 0, 2, 0, 2);
  tl->setRowStretch(1, 1);
  tl->setColStretch(1, 1);
  tl->addColSpacing(0, 15);
  tl->addColSpacing(2, 15);
  tl->addRowSpacing(2, 10);

  QVBoxLayout *l1 = new QVBoxLayout;
  tl->addLayout(l1, 1, 1);
  l1->addSpacing(10);

  QGridLayout *l11 = new QGridLayout(5, 2);
  l1->addLayout(l11);

  dnsdomain_label = new QLabel(this,"dnsdomainlabel");
  dnsdomain_label->setText(klocale->translate("Domain Name:"));
  MIN_SIZE(dnsdomain_label);
  l11->addWidget(dnsdomain_label, 0, 0);

  dnsdomain = new QLineEdit(this, "dnsdomain");
  dnsdomain->setMaxLength(DOMAIN_SIZE);
  FIXED_HEIGHT(dnsdomain);
  MIN_WIDTH(dnsdomain);
  l11->addWidget(dnsdomain, 0, 1);
  l11->addRowSpacing(1, 15);

  dns_label = new QLabel(this,"dnslabel");
  dns_label->setText(klocale->translate("DNS IP Address:"));
  MIN_SIZE(dns_label);
  l11->addWidget(dns_label, 2, 0);

  QHBoxLayout *l110 = new QHBoxLayout;
  l11->addLayout(l110, 2, 1);
  dnsipaddr = new IPLineEdit(this, "dnsipaddr");
  dnsipaddr->setMaxLength(IPADDR_SIZE);
  connect(dnsipaddr, SIGNAL(returnPressed()), SLOT(adddns()));
  FIXED_HEIGHT(dnsipaddr);
  l110->addWidget(dnsipaddr, 4);
  l110->addStretch(3);

  QHBoxLayout *l111 = new QHBoxLayout;
  l11->addLayout(l111, 3, 1);
  add = new QPushButton(klocale->translate("Add"), this, "add");
  connect(add, SIGNAL(clicked()), SLOT(adddns()));
  FIXED_HEIGHT(add);
  MIN_WIDTH(add);
  l111->addWidget(add);
  l111->addStretch(1);

  remove = new QPushButton(klocale->translate("Remove"), this, "remove");
  connect(remove, SIGNAL(clicked()), SLOT(removedns()));
  FIXED_HEIGHT(remove);
  MIN_WIDTH(remove);
  l111->addWidget(remove);

  servers_label = new QLabel(this,"servers");
  servers_label->setText(klocale->translate("DNS Address List:"));
  servers_label->setAlignment(AlignTop|AlignLeft);
  MIN_SIZE(servers_label);
  l11->addWidget(servers_label, 4, 0);
 
  dnsservers = new QListBox(this, "dnsservers");
  dnsservers->setMinimumSize(150, 100);
  l11->addWidget(dnsservers, 4, 1);

  exdnsdisabled_toggle = new QCheckBox(klocale->translate(
     "Disable existing DNS Servers during Connection"), 
				       this);
  MIN_SIZE(exdnsdisabled_toggle);
  exdnsdisabled_toggle->setChecked(gpppdata.exDNSDisabled());
  l1->addStretch(2);
  l1->addWidget(exdnsdisabled_toggle);
  l1->addStretch(1);   
 
  // restore data if editing
  if(!isnewaccount) {
    for(int i=0; gpppdata.dns(i) &&
	  i <= MAX_DNS_ENTRIES-1; i++)
      dnsservers->insertItem(gpppdata.dns(i));
    dnsdomain->setText(gpppdata.domain());
  }

  tl->activate();
}

void DNSWidget::save() {
  if (dnsservers->count() > 0)
    for(uint i=0; i < dnsservers->count(); i++)
      gpppdata.setDns(i, dnsservers->text(i));
  else 
    gpppdata.setDns(0, 0);
  gpppdata.setDomain(dnsdomain->text());
  gpppdata.setExDNSDisabled(exdnsdisabled_toggle->isChecked());
}


void DNSWidget::adddns() {
  if(dnsservers->count() < MAX_DNS_ENTRIES) {
    dnsservers->insertItem(dnsipaddr->text());
    dnsipaddr->setText("");
  }
}


void DNSWidget::removedns() {
  int i;
  i = dnsservers->currentItem();
  if(i != -1)
    dnsservers->removeItem(i);
}


//
// GatewayWidget
//
GatewayWidget::GatewayWidget( QWidget *parent, const char *name )
  : QWidget(parent, name)
{


  box1 = new QGroupBox(this,"box1");
  box1->setGeometry(10,10,345,310);
  box1->setTitle(klocale->translate("Gateway Setup"));

  box = new QGroupBox(this,"box");
  box->setGeometry(50,85,270,120);
  box->setTitle("            ");

  rb = new QButtonGroup(this, "rb");
  rb->setFrameStyle(QFrame::NoFrame);
  rb->setGeometry(80, 40, 135, 60);
  connect(rb, SIGNAL(clicked(int)), SLOT(hitGatewaySelect(int)));

  defaultgateway = new QRadioButton(rb, "defaultgateway");
  defaultgateway->setGeometry(0, 0, 135, 20);
  defaultgateway->setText(klocale->translate("Default Gateway"));

  staticgateway = new QRadioButton(rb, "staticgateway");
  staticgateway->setGeometry(0, 40, 135, 20);
  staticgateway->setText(klocale->translate("Static Gateway"));

  gatewayaddr = new IPLineEdit(this, "gatewayaddr");
  gatewayaddr->setGeometry(160, 130, 110, 25);
  gatewayaddr->setMaxLength(IPADDR_SIZE);


  gate_label = new QLabel(this, "label");
  gate_label->setGeometry(70,125,70,30);
  gate_label->setText(klocale->translate("Gateway\nIP Address:"));

  defaultroute=new QCheckBox(klocale->translate("Assign the Default Route to this Gateway"),
	this,"defaultroute");
  defaultroute->adjustSize();
  defaultroute->setGeometry(55,260,250,defaultroute->height());
  defaultroute->setChecked(gpppdata.defaultroute());
  connect(defaultroute,SIGNAL(toggled(bool)),this,SLOT(defaultroute_t(bool)));


  //load info from gpppdata
  if(!isnewaccount) {
    if(strcmp(gpppdata.gateway(),"0.0.0.0")==0 ) {
      defaultgateway->setChecked(TRUE);
      hitGatewaySelect(0);
    }
    else {
      gatewayaddr->setText(gpppdata.gateway());
      staticgateway->setChecked(TRUE);
    }
    defaultroute->setChecked(gpppdata.defaultroute());
  }
  else {
    defaultgateway->setChecked(TRUE);
    hitGatewaySelect(0);
    defaultroute->setChecked(TRUE);
  }
}

void GatewayWidget::defaultroute_t(bool on){
  on=on;
}

void GatewayWidget::save() {
  gpppdata.setGateway(gatewayaddr->text());
  gpppdata.setDefaultroute(defaultroute->isChecked()); 
}


void GatewayWidget::hitGatewaySelect( int i ) {
  if(i == 0) {
    gatewayaddr->setText("0.0.0.0");
    gatewayaddr->setEnabled(FALSE);
    gate_label->setEnabled(FALSE);
  }
  else {
    gatewayaddr->setEnabled(TRUE);
    gatewayaddr->setText("");
    gate_label->setEnabled(TRUE);
  }
}



ScriptWidget::ScriptWidget( QWidget *parent, const char *name )
  : QWidget(parent, name)
{ 
  const int GRIDROWS = 3;

  QGridLayout *tl = new QGridLayout(this, GRIDROWS, 3, 10, 10);
  tl->addRowSpacing(0, fontMetrics().lineSpacing() - 10);
  box = new QGroupBox(this,"box");
  box->setTitle(klocale->translate("Edit Script"));
  tl->addMultiCellWidget(box, 0, GRIDROWS-1, 0, 2);

  QVBoxLayout *l1 = new QVBoxLayout;
  tl->addLayout(l1, 1, 1);

  se = new ScriptEdit(this, "se");
  connect(se, SIGNAL(returnPressed()), SLOT(addButton()));
  l1->addWidget(se);

  add = new QPushButton(klocale->translate("Add"), this, "add");
  connect(add, SIGNAL(clicked()), SLOT(addButton()));
  FIXED_HEIGHT(add);
  MIN_WIDTH(add);

  insert = new QPushButton(klocale->translate("Insert"), this, "insert");
  connect(insert, SIGNAL(clicked()), SLOT(insertButton()));
  FIXED_HEIGHT(insert);
  MIN_WIDTH(insert);

  remove = new QPushButton(klocale->translate("Remove"), this, "remove");
  connect(remove, SIGNAL(clicked()), SLOT(removeButton()));
  FIXED_HEIGHT(remove);
  MIN_WIDTH(remove);

  QHBoxLayout *l11 = new QHBoxLayout;
  l1->addLayout(l11);
  l11->addWidget(add);
  l11->addStretch(1);
  l11->addWidget(insert);
  l11->addStretch(1);
  l11->addWidget(remove);
  
  QHBoxLayout *l12 = new QHBoxLayout(0);
  l1->addLayout(l12);
  stl = new QListBox(this, "stl");
  stl->setSmoothScrolling(false);
  stl->setAutoScrollBar(false);
  connect(stl, SIGNAL(highlighted(int)), SLOT(stlhighlighted(int)));
  stl->setMinimumSize(QSize(70, 140));

  sl = new QListBox(this, "sl");
  sl->setSmoothScrolling(false);
  sl->setAutoScrollBar(false);
  connect(sl, SIGNAL(highlighted(int)), SLOT(slhighlighted(int)));
  sl->setMinimumSize(QSize(150, 140));

  slb = new QScrollBar(this, "slb");
  slb->setFixedWidth(slb->sizeHint().width());
  connect(slb, SIGNAL(valueChanged(int)), SLOT(scrolling(int)));

  l12->addWidget(stl, 1);
  l12->addWidget(sl, 3);
  l12->addWidget(slb, 0);

  //load data from gpppdata
  if(!isnewaccount) {
    for( int i=0; gpppdata.scriptType(i) &&
	   i <= MAX_SCRIPT_ENTRIES-1; i++) {
      stl->insertItem(gpppdata.scriptType(i));
      sl->insertItem(gpppdata.script(i));
    }
  }

  adjustScrollBar();

  tl->activate();
}


bool ScriptWidget::check() {
  uint lstart = 0;
  uint lend  = 0;
  uint errcnt = 0;

  if(sl->count() > 0)   {
    for( uint i=0; i <= sl->count()-1; i++) {
	if ( 0 == strcmp( "LoopStart", stl->text(i)) )  {
		lstart++;
        }
	if ( 0 == strcmp( "LoopEnd", stl->text(i)) )  {
		lend++;
        }
	if ( lend > lstart ) errcnt++;
    }
    return ( (errcnt == 0 ) && (lstart == lend) );
  } 
  return true;
}


void ScriptWidget::save() {

  if(sl->count() > 0)   
    for( uint i=0; i <= sl->count()-1; i++) {
      gpppdata.setScriptType(i, stl->text(i));
      gpppdata.setScript(i, sl->text(i));
    }
  else {                
    gpppdata.setScriptType(0, 0);
    gpppdata.setScript(0, 0);
  }
}                        



void ScriptWidget::adjustScrollBar() {
  if((int)sl->count() <= sl->numItemsVisible())
    slb->setRange(0, 0);
  else
    slb->setRange(0, (sl->count() - sl->numItemsVisible())+1);
}


void ScriptWidget::scrolling(int i) {
  sl->setTopItem(i);
  stl->setTopItem(i);
}


void ScriptWidget::slhighlighted(int i) {
  stl->setCurrentItem(i);
}


void ScriptWidget::stlhighlighted(int i) {
  sl->setCurrentItem(i);
}


void ScriptWidget::addButton() {
  //don't allow more than the maximum script entries
  if(sl->count() == MAX_SCRIPT_ENTRIES-1)
    return;

  switch(se->type()) {
    case ScriptEdit::Expect:
      stl->insertItem("Expect");
      sl->insertItem(se->text());
      break;

    case ScriptEdit::Send:
      stl->insertItem("Send");
      sl->insertItem(se->text());
      break;

    case ScriptEdit::Pause:
      stl->insertItem("Pause");
      sl->insertItem(se->text());
      break;

    case ScriptEdit::Hangup:
      stl->insertItem("Hangup");
      sl->insertItem("");
      break;

    case ScriptEdit::Answer:
      stl->insertItem("Answer");
      sl->insertItem("");
      break;

    case ScriptEdit::Timeout:
      stl->insertItem("Timeout");
      sl->insertItem(se->text());
      break;

    case ScriptEdit::Password:
      stl->insertItem("Password");
      sl->insertItem(se->text());
      break;

    case ScriptEdit::ID:
      stl->insertItem("ID");
      sl->insertItem(se->text());
      break;

    case ScriptEdit::Prompt:
      stl->insertItem("Prompt");
      sl->insertItem(se->text());
      break;

    case ScriptEdit::PWPrompt:
      stl->insertItem("PWPrompt");
      sl->insertItem(se->text());
      break;


    case ScriptEdit::LoopStart:
      stl->insertItem("LoopStart");
      sl->insertItem(se->text());
      break;

    case ScriptEdit::LoopEnd:
      stl->insertItem("LoopEnd");
      sl->insertItem(se->text());
      break;

    default:
      break;
  }

  //get the scrollbar adjusted, and scroll the list so we can see what
  //we're adding to
  adjustScrollBar();
  slb->setValue(slb->maxValue());

  //clear the text in the entry box
  se->setText("");
}


void ScriptWidget::insertButton() {
  //exit if there is no highlighted item, or we've reached the
  //maximum entries in the script list
  if(sl->currentItem() < 0 || (sl->count() == MAX_SCRIPT_ENTRIES-1))
    return;

  switch(se->type()) {
    case ScriptEdit::Expect:
      stl->insertItem("Expect", stl->currentItem());
      sl->insertItem(se->text(), sl->currentItem());
      break;

    case ScriptEdit::Send:
      stl->insertItem("Send", stl->currentItem());
      sl->insertItem(se->text(), sl->currentItem());
      break;

    case ScriptEdit::Pause:
      stl->insertItem("Pause", stl->currentItem());
      sl->insertItem(se->text(), sl->currentItem());
      break;

    case ScriptEdit::Hangup:
      stl->insertItem("Hangup", stl->currentItem());
      sl->insertItem("", sl->currentItem());
      break;

    case ScriptEdit::Answer:
      stl->insertItem("Answer", stl->currentItem());
      sl->insertItem("", sl->currentItem());
      break;

    case ScriptEdit::Timeout:
      stl->insertItem("Timeout", stl->currentItem());
      sl->insertItem(se->text(), sl->currentItem());
      break;

    case ScriptEdit::Password:
      stl->insertItem("Password", stl->currentItem());
      sl->insertItem(se->text(), sl->currentItem());
      break;

    case ScriptEdit::ID:
      stl->insertItem("ID", stl->currentItem());
      sl->insertItem(se->text(), sl->currentItem());
      break;

    case ScriptEdit::Prompt:
      stl->insertItem("Prompt", stl->currentItem());
      sl->insertItem(se->text(), sl->currentItem());
      break;

    case ScriptEdit::PWPrompt:
      stl->insertItem("PWPrompt", stl->currentItem());
      sl->insertItem(se->text(), sl->currentItem());
      break;


    case ScriptEdit::LoopStart:
      stl->insertItem("LoopStart", stl->currentItem());
      sl->insertItem(se->text(), sl->currentItem());
      break;

    case ScriptEdit::LoopEnd:
      stl->insertItem("LoopEnd", stl->currentItem());
      sl->insertItem(se->text(), sl->currentItem());
      break;

    default:
      break;
  }
  adjustScrollBar();
  se->setText("");
}


void ScriptWidget::removeButton() {

 int stlc;

  if(sl->currentItem() < 0)
    return;

  stlc = stl->currentItem();
  sl->removeItem(sl->currentItem());
  stl->removeItem(stlc);
  adjustScrollBar();

}

#include "edit.moc"
