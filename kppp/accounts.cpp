/*
 *           kPPP: A pppd front end for the KDE project
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
#include <qfiledlg.h>
#include <kmsgbox.h>

#include "homedir.h"
#include "accounts.h"
#include "main.h"


//so we can call the function in the XPPPWidget to reset the dial-out
//account combo box


extern XPPPWidget *p_xppp;

bool isnewaccount;
void parseargs(char* buf, char** args);

AccountWidget::AccountWidget( QWidget *parent, const char *name )
  : QWidget( parent, name ){

  box = new QGroupBox(this,"box");
  box->setGeometry(10,10,320,260);
  box->setTitle("Account Setup");
    
  accountlist_l = new QListBox(this, "accountlist_l");
  accountlist_l->setGeometry(30, 40, 160, 130);
  connect(accountlist_l, SIGNAL(highlighted(int)),
	  this, SLOT(slotListBoxSelect(int)));

  edit_b = new QPushButton("Edit...", this, "edit_b");
  edit_b->setGeometry(220, 40, 80, 25);
  connect(edit_b, SIGNAL(clicked()), SLOT(editaccount()));

  new_b = new QPushButton("New...", this, "new_b");
  new_b->setGeometry(220, 75, 80, 25);
  connect(new_b, SIGNAL(clicked()), SLOT(newaccount()));

  copy_b = new QPushButton("Copy", this, "copy_b");
  copy_b->setGeometry(220, 110, 80, 25);
  connect(copy_b, SIGNAL(clicked()), SLOT(copyaccount()));


  delete_b = new QPushButton("Delete", this, "delete_b");
  delete_b->setGeometry(220, 145, 80, 25);
  connect(delete_b, SIGNAL(clicked()), SLOT(deleteaccount()));

  reset = new QPushButton("Reset Costs",this,"resetbutton");
  reset->setGeometry(220,187,80,25);
  reset->setEnabled(FALSE);
  connect(reset,SIGNAL(clicked()),this,SLOT(resetClicked()));

  log = new QPushButton("View Logs",this,"logbutton");
  log->setGeometry(220,222,80,25);
  //  log->setEnabled(FALSE);
  connect(log,SIGNAL(clicked()),this,SLOT(viewLogClicked()));

  costlabel = new QLabel("Phone Costs:",this,"costlable");
  costlabel->setGeometry(30,185,100,30);
  costlabel->setEnabled(FALSE);

  costedit = new QLineEdit(this,"costedit");
  costedit->setGeometry(110,187,80,25);
  costedit->setEnabled(FALSE);

  //load up account list from gppdata to the list box

  if(gpppdata.count() > 0) {
    for(int i=0; i <= gpppdata.count()-1; i++) {
      gpppdata.setAccountbyIndex(i);
      accountlist_l->insertItem(gpppdata.accname());
    }
  }

  slotListBoxSelect(accountlist_l->currentItem());
}



void AccountWidget::slotListBoxSelect(int idx) {

  delete_b->setEnabled((bool)(idx != -1));
  edit_b->setEnabled((bool)(idx != -1));
  copy_b->setEnabled((bool)(idx != -1));
  if(idx!=-1){
    reset->setEnabled(TRUE);
    costlabel->setEnabled(TRUE);
    costedit->setEnabled(TRUE);
    costedit->setText(p_xppp->accounting.getCosts(
       		      accountlist_l->text(accountlist_l->currentItem())));
 }
  else{
    reset->setEnabled(FALSE);
    costlabel->setEnabled(FALSE);
    costedit->setText("");
    costedit->setEnabled(FALSE);

  }
}

void AccountWidget::viewLogClicked(){

  QString d;
  d = getHomeDir();
  d += ACCOUNTING_PATH ;
  d += "/Log/";

  QString fileName = QFileDialog::getOpenFileName(d.data());

  if ( !fileName.isNull() ) {                

    /*      char *args[100];
	    char buffer[2024];
     */

      QString cmd;
      cmd.sprintf("%s %s\n",gpppdata.logViewer(),fileName.data());

      // strncpy(buffer,cmd.data(),2023);
      // parseargs(buffer,args);
      // printf("tyring to execute %s %s\n",gpppdata.logViewer(),*args);


    if(fork()==0){

      setuid(getuid());
      system(cmd.data());
      exit(0);

    }

    }

}

void AccountWidget::resetClicked(){

  if(accountlist_l->currentItem() == -1)
    return;
 
  int ok = QMessageBox::information(this,"Reset Total",
  "Are you sure you want to reset the accumulated\n"\
  "telephone costs for the selected account to zero?",
  "Yes","Cancel","",1,1);

  if(ok)
    return;
  
  p_xppp->accounting.resetCosts(
       		      accountlist_l->text(accountlist_l->currentItem()));
  costedit->setText("");


}

void AccountWidget::editaccount() {

  gpppdata.setAccount(accountlist_l->text(accountlist_l->currentItem()));

  int result = doTab();

  if(result == QDialog::Accepted) {
    accountlist_l->changeItem(gpppdata.accname(),accountlist_l->currentItem());
    p_xppp->resetaccounts();
    gpppdata.save();
  }

}


void AccountWidget::newaccount() {

  if(accountlist_l->count() == MAX_ACCOUNTS){
   
    QMessageBox::information(this,"Sorry","Maximum number of accounts reached.");
    return;
  }

  gpppdata.newaccount();

  int result = doTab();

  if(result == QDialog::Accepted) {
    accountlist_l->insertItem(gpppdata.accname());
    p_xppp->resetaccounts();
    gpppdata.save();
  }
  else {
    gpppdata.deleteAccount();
  }
}

void AccountWidget::copyaccount() {

  if(accountlist_l->count() == MAX_ACCOUNTS){
   
    QMessageBox::information(this,"Sorry","Maximum number of accounts reached.");
    return;
  }

  if(accountlist_l->currentItem()<0){
    QMessageBox::information(this,"Sorry","No account selected.");
    return;
  }

  gpppdata.copyaccount(accountlist_l->currentItem());

  accountlist_l->insertItem(gpppdata.accname());
  p_xppp->resetaccounts();
  gpppdata.save();

}


void AccountWidget::deleteaccount() {

  if(gpppdata.deleteAccount(accountlist_l->text(accountlist_l->currentItem())))
    accountlist_l->removeItem(accountlist_l->currentItem());

  p_xppp->resetaccounts();
  gpppdata.save();

}



int AccountWidget::doTab(){

  tabWindow = new QTabDialog(0,0,TRUE);
 
  if(strcmp(gpppdata.accname(), "") == 0) {
    tabWindow->setCaption("New Account");
    isnewaccount = true;
  }
  else {
    QString tit = "Edit Account: ";
    tit += gpppdata.accname();
    tabWindow->setCaption(tit);
    isnewaccount = false;
  }

  tabWindow->resize( 360, 400 );
  tabWindow->setCancelButton();

  //  tabWindow->setApplyButton();

  dial_w = new DialWidget(tabWindow, "dial_w");

  ip_w = new IPWidget(tabWindow, "ip_w");
  dns_w = new DNSWidget(tabWindow, "dns_w");
  gateway_w = new GatewayWidget(tabWindow, "gateway_w");
  script_w = new ScriptWidget(tabWindow, "script_w");
  acct = new AccountingSelector(tabWindow, "acct_w");

  tabWindow->addTab(dial_w,"Dial");
  tabWindow->addTab(ip_w,"IP");
  tabWindow->addTab(dns_w,"DNS");
  tabWindow->addTab(gateway_w,"Gateway");
  tabWindow->addTab(script_w,"Login Script");
  tabWindow->addTab(acct, "Accounting");

  int result = 0;
  bool ok = false;
  while (!ok){

    result = tabWindow->exec();
    ok = true;

    if(result == QDialog::Accepted) {
      if(dial_w->save()) {
	ip_w->save();
	dns_w->save();
	gateway_w->save();
	script_w->save();
	acct->save();
      }
      else {
	QMessageBox::warning(this, "Error", 
			 "You must enter a unique\naccount name");
	ok = false;
      }
    }    
  }
  
 delete tabWindow;
 return result;
}

