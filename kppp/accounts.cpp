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

#include <qlayout.h>
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
  : QWidget( parent, name )
{
  QGridLayout *tl = new QGridLayout(this, 3, 3, 10, 10);  
  tl->addRowSpacing(0, fontMetrics().lineSpacing() - 10); // magic
  box = new QGroupBox(this,"box");
  box->setTitle(klocale->translate("Account Setup"));
  tl->addMultiCellWidget(box, 0, 2, 0, 2);

  // add a vbox in the middle of the grid
  QVBoxLayout *l1 = new QVBoxLayout;
  tl->addLayout(l1, 1, 1);

  // add a hbox
  QHBoxLayout *l11 = new QHBoxLayout;
  l1->addLayout(l11);
    
  accountlist_l = new QListBox(this, "accountlist_l");
  accountlist_l->setMinimumSize(160, 128);
  connect(accountlist_l, SIGNAL(highlighted(int)),
	  this, SLOT(slotListBoxSelect(int)));
  l11->addWidget(accountlist_l, 10);

  QVBoxLayout *l111 = new QVBoxLayout;
  l11->addLayout(l111, 1);  
  edit_b = new QPushButton(klocale->translate("Edit..."), this, "edit_b");
  connect(edit_b, SIGNAL(clicked()), SLOT(editaccount()));
  edit_b->setMinimumSize(edit_b->sizeHint());
  l111->addWidget(edit_b);

  new_b = new QPushButton(klocale->translate("New..."), this, "new_b");
  connect(new_b, SIGNAL(clicked()), SLOT(newaccount()));
  new_b->setMinimumSize(new_b->sizeHint());
  l111->addWidget(new_b);

  copy_b = new QPushButton(klocale->translate("Copy"), this, "copy_b");
  connect(copy_b, SIGNAL(clicked()), SLOT(copyaccount()));
  copy_b->setMinimumSize(copy_b->sizeHint());
  l111->addWidget(copy_b);

  delete_b = new QPushButton(klocale->translate("Delete"), this, "delete_b");
  connect(delete_b, SIGNAL(clicked()), SLOT(deleteaccount()));
  delete_b->setMinimumSize(delete_b->sizeHint());
  l111->addWidget(delete_b);

  QHBoxLayout *l12 = new QHBoxLayout;
  l1->addStretch(1);
  l1->addLayout(l12);

  QVBoxLayout *l121 = new QVBoxLayout;
  l12->addLayout(l121);
  l121->addStretch(1);
  costlabel = new QLabel(klocale->translate("Phone Costs:"),this,"costlable");
  costlabel->setMinimumSize(costlabel->sizeHint());
  costlabel->setEnabled(FALSE);
  l121->addWidget(costlabel);

  costedit = new QLineEdit(this,"costedit");
  costedit->setFixedHeight(costedit->sizeHint().height());
  costedit->setEnabled(FALSE);
  l121->addWidget(costedit);
  l121->addStretch(1);

  QVBoxLayout *l122 = new QVBoxLayout;
  l12->addStretch(1);
  l12->addLayout(l122);
  
  l122->addStretch(1);
  reset = new QPushButton(klocale->translate("Reset Costs"),
			  this, "resetbutton");
  reset->setMinimumSize(reset->sizeHint());
  reset->setEnabled(FALSE);
  connect(reset,SIGNAL(clicked()),this,SLOT(resetClicked()));
  l122->addWidget(reset);

  log = new QPushButton(klocale->translate("View Logs"),
			this, "logbutton");
  log->setMinimumSize(log->sizeHint());
  //  log->setEnabled(FALSE);
  connect(log,SIGNAL(clicked()),this,SLOT(viewLogClicked()));
  l122->addWidget(log);
  l122->addStretch(1);

  //load up account list from gppdata to the list box

  if(gpppdata.count() > 0) {
    for(int i=0; i <= gpppdata.count()-1; i++) {
      gpppdata.setAccountbyIndex(i);
      accountlist_l->insertItem(gpppdata.accname());
    }
  }

  slotListBoxSelect(accountlist_l->currentItem());

  tl->activate();
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
 
  int ok = QMessageBox::information(this,klocale->translate("Reset Total"),
       klocale->translate("Are you sure you want to reset the accumulated\n"
       "telephone costs for the selected account to zero?"),
				    klocale->translate("Yes"),
				    klocale->translate("Cancel"),"",1,1);

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
   
      QMessageBox::information(this,klocale->translate("Sorry"),
			       klocale->translate("Maximum number of accounts reached."));
    return;
  }

  if (gpppdata.newaccount() == -1) return;

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
   
    QMessageBox::information(this,klocale->translate("Sorry"),
			     klocale->translate("Maximum number of accounts reached."));
    return;
  }

  if(accountlist_l->currentItem()<0){
    QMessageBox::information(this,klocale->translate("Sorry"),
			     klocale->translate("No account selected."));
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

  slotListBoxSelect(accountlist_l->currentItem());

}



int AccountWidget::doTab(){

  tabWindow = new QTabDialog(0,0,TRUE);
 
  if(strcmp(gpppdata.accname(), "") == 0) {
    tabWindow->setCaption(klocale->translate("New Account"));
    isnewaccount = true;
  }
  else {
    QString tit = klocale->translate("Edit Account: ");
    tit += gpppdata.accname();
    tabWindow->setCaption(tit);
    isnewaccount = false;
  }

  tabWindow->resize( 360, 400 );
  tabWindow->setOKButton(klocale->translate("OK"));
  tabWindow->setCancelButton(klocale->translate("Cancel"));

  //  tabWindow->setApplyButton();

  dial_w = new DialWidget(tabWindow, "dial_w");

  ip_w = new IPWidget(tabWindow, "ip_w");
  dns_w = new DNSWidget(tabWindow, "dns_w");
  gateway_w = new GatewayWidget(tabWindow, "gateway_w");
  script_w = new ScriptWidget(tabWindow, "script_w");
  acct = new AccountingSelector(tabWindow, "acct_w");

  tabWindow->addTab(dial_w,klocale->translate("Dial"));
  tabWindow->addTab(ip_w,klocale->translate("IP"));
  tabWindow->addTab(dns_w,klocale->translate("DNS"));
  tabWindow->addTab(gateway_w,klocale->translate("Gateway"));
  tabWindow->addTab(script_w,klocale->translate("Login Script"));
  tabWindow->addTab(acct, klocale->translate("Accounting"));

  int result = 0;
  bool ok = false;
  while (!ok){

    result = tabWindow->exec();
    ok = true;

    if(result == QDialog::Accepted) {
      if (script_w->check()) {
          if(dial_w->save()) {
		ip_w->save();
		dns_w->save();
		gateway_w->save();
		script_w->save();
		acct->save();
         } else {
	     QMessageBox::warning(this, klocale->translate("Error"), 
				     klocale->translate( "You must enter a unique\naccount name"));
		ok = false;
	 }
      } else {
	      QMessageBox::warning(this, klocale->translate("Error"), 
				   klocale->translate("Login script has unbalanced Loop Start/End") );
	      ok = false;
      }
    }
  }
  
 delete tabWindow;
 return result;
}

#include "accounts.moc"
