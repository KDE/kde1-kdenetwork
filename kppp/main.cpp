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

#include <qfileinf.h>
#include <unistd.h>
#include <kmsgbox.h>
#include <stdio.h>

#if defined(__FreeBSD__) || defined(__NetBSD__) 
#include <stdlib.h>
#else
#include <getopt.h>
#endif


#include "acctselect.h"
#include "main.h"
#include "version.h"
#include "homedir.h"

#include <X11/Xlib.h>

XPPPWidget*	p_xppp;
KApplication*	app;
QString 	cmdl_account;

bool	have_cmdl_account;
bool 	pppd_has_died = false;
bool 	reconnect_on_disconnect = false;
bool 	modem_is_locked = false;

QString old_hostname;
QString local_ip_address;
QString remote_ip_address;

void terminatepppd();

void usage(char* progname){

  fprintf(stderr, "%s -- valid command line options:\n", progname);
  fprintf(stderr, " -h describe command line options\n");
  fprintf(stderr, " -c account_name : connect to account account_name\n");
  fprintf(stderr, " -r rule_file: check syntax of rule_file\n");
  exit(1);

}

void banner(char* progname){

  fprintf(stderr,"%s version " KPPPVERSION "\n",progname); 
  fprintf(stderr,"Copyright (c) 1997 Bernd Johannes Wuebben ");
  fprintf(stderr,"wuebben@math.cornell.edu\n");
  fprintf(stderr,"Use -h for the list of valid of command line options.\n");
  exit(0);

}

extern "C" {

static int kppp_x_errhandler( Display *dpy, XErrorEvent *err )
{
    char errstr[256];

    /*
    if(gpppdata.pppdpid() >= 0) {
      kill(gpppdata.pppdpid(), SIGTERM);
    }

    p_xppp->stopAccounting();
    removedns();
    unlockdevice();*/

    XGetErrorText( dpy, err->error_code, errstr, 256 );
    fatal( "X Error: %s\n  Major opcode:  %d", errstr, err->request_code );
    return 0;
}


static int kppp_xio_errhandler( Display * ){


  if(gpppdata.get_xserver_exit_disconnect()){

    printf("X11 Error!");
    if(gpppdata.pppdpid() >= 0) {
      kill(gpppdata.pppdpid(), SIGTERM);
    }

    p_xppp->stopAccounting();
    removedns();
    unlockdevice();	
    return 0;
  }
  else{

    fatal( "%s: Fatal IO error: client killed", "kppp" );
    return 0;

  }
}

} /* extern "C" */                                  


void make_directories(){

  QDir dir;

  QString d;
  d = getHomeDir();

  d += "/.kde";
  dir.setPath(d.data());
  if(!dir.exists()){
    dir.mkdir(d.data());
    chown(d.data(),getuid(),getgid());
    chmod(d.data(),S_IRUSR | S_IWUSR | S_IXUSR);
  }

  d += "/share";
  dir.setPath(d.data());
  if(!dir.exists()){
    dir.mkdir(d.data());
    chown(d.data(),getuid(),getgid());
    chmod(d.data(),S_IRUSR | S_IWUSR | S_IXUSR);
  }

  d += "/apps";
  dir.setPath(d.data());
  if(!dir.exists()){
    dir.mkdir(d.data());
    chown(d.data(),getuid(),getgid());
    chmod(d.data(),S_IRUSR | S_IWUSR | S_IXUSR);
  }

  d += "/kppp" ;

  dir.setPath(d.data());
  if(!dir.exists()){
    dir.mkdir(d.data());
    chown(d.data(),getuid(),getgid());
    chmod(d.data(),S_IRUSR | S_IWUSR | S_IXUSR);
  }

  
  d += "/Rules/";

  dir.setPath(d.data());
  if(!dir.exists()){
    dir.mkdir(d.data());
    chown(d.data(),getuid(),getgid());
    chmod(d.data(),S_IRUSR | S_IWUSR | S_IXUSR);
  }

  QString logdir;
  logdir = getHomeDir();
  logdir += ACCOUNTING_PATH "/Log";

  dir.setPath(logdir.data());
  if(!dir.exists()){
    dir.mkdir(logdir.data());
    chown(logdir.data(),getuid(),getgid());
    chmod(logdir.data(),S_IRUSR | S_IWUSR | S_IXUSR);
  }
}


int main( int argc, char **argv ) { 

  int c;
  opterr = 0;

  while ((c = getopt(argc, argv, "c:hvr:")) != -1){
    switch (c)
      {

      case '?':
	fprintf(stderr, "%s: unknown option \"%s\"\n", 
		argv[0], argv[optind-1]);
	usage(argv[0]);
	exit(1);	
      case 'c':
	cmdl_account = optarg;
	break;
      case 'h':
	usage(argv[0]);
	break;
      case 'v':
	banner(argv[0]);
	break;
      case 'r':
	RuleSet::checkRuleFile(optarg);
	exit(0);
      }
  }

  if(!cmdl_account.isEmpty()){
 
   have_cmdl_account = true;

#ifdef MY_DEBUG
    printf("cmdl_account:%s:\n",cmdl_account.data());
#endif

  }

  KApplication a(argc, argv,"kppp");

  app = &a;

  make_directories();
  XPPPWidget xppp;
  p_xppp = &xppp;


  a.setMainWidget(&xppp);
  a.setTopWidget(&xppp);

  // we really don't want to die accidentally, since that would leave the
  // modem connected. If you really really want to kill me you must send 
  // me a SIGKILL. 
  signal(SIGINT, SIG_IGN);
  signal(SIGTERM, SIG_IGN);
  signal(SIGHUP, SIG_IGN);

  signal(SIGCHLD, dieppp);

  XSetErrorHandler( kppp_x_errhandler );
  XSetIOErrorHandler( kppp_xio_errhandler );
                                                 
  return a.exec();

}



XPPPWidget::XPPPWidget( QWidget *parent, const char *name )
  : QWidget(parent, name){

  bool config;
  config = gpppdata.open(app);

  connected = false;

  fline1 = new QFrame(this,"line");
  fline1->setFrameStyle(QFrame::HLine |QFrame::Sunken);
  fline1->setGeometry(15,7,327,3);

  ID_Label = new QLabel(this,"lableid");
  ID_Label->setText(klocale->translate("Login ID:"));
  ID_Label->setGeometry(30,27,70,20);

  ID_Edit = new QLineEdit(this,"idedit");
  ID_Edit->setGeometry(120,25,189,24);
  ID_Edit->setText(gpppdata.Id());

  PW_Label = new QLabel(this,"lablepw");
  PW_Label->setText(klocale->translate("Password:"));
  PW_Label->setGeometry(30,62,70,20);


  PW_Edit= new QLineEdit(this,"pwedit");
  PW_Edit->setGeometry(120,60,189,24);
  PW_Edit->setEchoMode(QLineEdit::Password);

  label1 = new QLabel(this,"lable1");
  label1->setText(klocale->translate("Connect to: "));
  label1->setGeometry(30,100,70,20);

  connectto_c = new QComboBox(true,this, "connectto_c");
  connectto_c->setGeometry(120, 97, 190, 28);
  connect(connectto_c, SIGNAL(activated(int)), SLOT(newdefaultaccount(int)));


  fline = new QFrame(this,"line");
  fline->setFrameStyle(QFrame::HLine |QFrame::Sunken);
  fline->setGeometry(15,172,327,3);

  quit_b = new QPushButton(klocale->translate("Quit"), this, "quit");
  quit_b->setGeometry(15, 187, 70, 25);
  connect( quit_b, SIGNAL(clicked()), SLOT(quitbutton()));

  setup_b = new QPushButton(klocale->translate("Setup"), this, "setup");
  setup_b->setGeometry(90, 187, 70, 25);
  connect( setup_b, SIGNAL(clicked()), SLOT(expandbutton()));

  if (!config) setup_b->setEnabled(false);

  help_b = new QPushButton(klocale->translate("Help"), this, "help");
  help_b->setGeometry(165, 187, 70, 25);
  connect( help_b, SIGNAL(clicked()), SLOT(helpbutton()));


  log = new QCheckBox(klocale->translate("Show Log Window"), this,"log");
  log->adjustSize();
  log->setGeometry(30,145,150,log->height());
  connect(log,SIGNAL(toggled(bool)),this,SLOT(log_window_toggled(bool)));

  log->setChecked(gpppdata.get_show_log_window());

  connect_b = new QPushButton(klocale->translate("Connect"), this, "connect_b");
  connect_b->setGeometry(265, 187, 80, 25);
  connect_b->setFocus();
  connect_b->setText(klocale->translate("Connect"));
  connect(connect_b, SIGNAL(clicked()), SLOT(connectbutton()));

  // we also connect cmld_start to the connectbutton so that I can run
  // the dialer through a command line argument
  connect(this,SIGNAL(cmdl_start()),this,SLOT(connectbutton())); 

  tabWindow = new QTabDialog( 0, 0, TRUE );
  tabWindow->setCaption( klocale->translate("kppp Configuration") );
  tabWindow->setOkButton(klocale->translate("OK"));
  tabWindow->setCancelButton(klocale->translate("Cancel"));
  tabWindow->resize( 355, 350 );
  tabWindow->setFixedSize( 355, 350 ); // this doesn't seem to work in Qt 1.1

  accounts = new AccountWidget(tabWindow,"accounts");
  modem = new ModemWidget(tabWindow,"modem");
  modem2 = new ModemWidget2(tabWindow,"modem2");
  general = new GeneralWidget(tabWindow,"general");
  about  = new AboutWidget(tabWindow,"about");
  
  tabWindow->addTab( accounts, klocale->translate("Accounts") );
  tabWindow->addTab( modem, klocale->translate("Modem") );
  tabWindow->addTab( modem2, klocale->translate("More ...") );
  tabWindow->addTab( general, klocale->translate("PPP") );
  tabWindow->addTab( about, klocale->translate("About") );

  con_win = new ConWindow(0,"conw",this);
  con_win->setGeometry(QApplication::desktop()->width()/2-160,
		    QApplication::desktop()->height()/2-55,
		    320,110);
  KWM::setMiniIcon(con_win->winId(), app->getMiniIcon());

  // connect to the accounting object
  connect(&accounting, SIGNAL(changed(QString, QString)),
	  con_win, SLOT(slotAccounting(QString, QString)));

  stats = new PPPStatsDlg(0,"stats",this);
  stats->hide();

  debugwindow = new DebugWidget(0,"debugwindow");
  debugwindow->setGeometry(QApplication::desktop()->width()/2+190,
		    QApplication::desktop()->height()/2-55,
		    debugwindow->width(),debugwindow->height());
  debugwindow->hide();

  // load up the accounts combo box

  resetaccounts();
  con = new ConnectWidget(0, "con");
  connect(this, SIGNAL(begin_connect()),con, SLOT(preinit()));
  con->setGeometry(QApplication::desktop()->width()/2-175,
		    QApplication::desktop()->height()/2-55,
		    350,110);



  this->setGeometry(QApplication::desktop()->width()/2 - 180,
		    QApplication::desktop()->height()/2 - 110,
		    360,220);

  this->setFixedSize(360,220);


  if(have_cmdl_account){
    bool result;
    result = gpppdata.setAccount(cmdl_account.data());
    if (!result){
      QString string;
      string.sprintf(klocale->translate("No such Account:\n%s"),cmdl_account.data());
      QMessageBox::warning(this, klocale->translate("Error"), string.data());
      have_cmdl_account = false;
      this->show();
    }
    else{
      emit cmdl_start();
    }
  }
  else{
    this->show();
  }
}


void XPPPWidget::log_window_toggled(bool on){
  
  gpppdata.set_show_log_window(on);
  
}


void XPPPWidget::setup()
{
  
  if(tabWindow->exec())
    gpppdata.save();
  else
    gpppdata.cancel();

}


void XPPPWidget::resetaccounts() {

  connectto_c->clear();

  if(gpppdata.count() == 0) {
    connectto_c->setEnabled(false);
    connect_b->setEnabled(false);
    log->setEnabled(false);
  }
  else {
    connectto_c->setEnabled(true);
    connect_b->setEnabled(true);
    log->setEnabled(true);
  }

  //load the accounts
  for(int i=0; i <= gpppdata.count()-1; i++) {
    gpppdata.setAccountbyIndex(i);
     connectto_c->insertItem(gpppdata.accname());
  }

  //set the default account
  if(strcmp(gpppdata.defaultAccount(), "") != 0) {
    for(int i=0; i < connectto_c->count(); i++)
       if(strcmp(gpppdata.defaultAccount(), connectto_c->text(i)) == 0) {
 	connectto_c->setCurrentItem(i);
	gpppdata.setAccountbyIndex(i);
    }
  }
  else
    if(gpppdata.count() > 0) {
       gpppdata.setDefaultAccount(connectto_c->text(0));
    }
    
}


//Note: this is a friend function of XPPPWidget class (xppp)

void dieppp(int sig) {

  pid_t id;
  int st;

#ifdef MY_DEBUG
  printf("Received a signal: %d\n",sig);
#endif

  if(sig == SIGCHLD){

#ifdef MY_DEBUG
  printf("The Signal received was a SIGCHLD\n");
#endif

    signal(SIGCHLD, dieppp); // reinstall the sig handler
    id = wait(&st);

#ifdef MY_DEBUG
    printf("pid of the process which died: %d\n",id);
#endif

    // if we are not connected pppdpid is -1 so have have to check for that
    // in the followin line to make sure that we don't raise a false alarm
    // such as would be the case when the log file viewer exits.

    if(id == gpppdata.pppdpid() && gpppdata.pppdpid() != -1) { 
      
#ifdef MY_DEBUG
  printf("It was pppd that died\n");
#endif
      // when we killppp() on Cancel in ConnectWidget 
      // we set pppid to -1 so we won't 
      // enter this block

      p_xppp->stopAccounting();
      p_xppp->con_win->stopClock();
      
      gpppdata.setpppdpid(-1);

      pppd_has_died = true;
      removedns();
      unlockdevice();
      
      if(!reconnect_on_disconnect){
      p_xppp->quit_b->setFocus();
      p_xppp->show();
      p_xppp->con_win->stopClock();
      p_xppp->stopAccounting();
      p_xppp->con_win->hide();
      p_xppp->con->hide();

      gpppdata.setpppdpid(-1);
    
      app->beep();
      QMessageBox::warning( 0, klocale->translate("Error"), 
			klocale->translate("The pppd daemon died unexpectedly!"));
      }
      else{/* reconnect on disconnect */
#ifdef MY_DEBUG
  printf("Trying to reconnect\n");
#endif
	p_xppp->con_win->hide();
	p_xppp->con_win->stopClock();
	p_xppp->stopAccounting();
	gpppdata.setpppdpid(-1);
	app->beep();
	emit p_xppp->cmdl_start();
      }
    }
  }
}



void XPPPWidget::newdefaultaccount(int i) {
  gpppdata.setDefaultAccount(connectto_c->text(i));
  gpppdata.save();
}


void XPPPWidget::expandbutton() {
  setup();
}


void XPPPWidget::connectbutton() {

  QFileInfo info(gpppdata.pppdPath());
  
  if(!info.exists()){
    QString string;   
    string.sprintf(klocale->translate("kppp can not find:\n %s\nPlease install pppd properly "
				      "and/or adjust\n the location of the pppd executable on "
				      "the misc tab of\n"
				      "the setup dialog.\n Thank You"),
		   gpppdata.pppdPath());
    QMessageBox::warning(this, klocale->translate("Error"), string.data());
    return;
  }

  if(!info.isExecutable()){

    QString string;   
    string.sprintf(klocale->translate("kppp can not execute:\n %s\nPlease make sure that"
		   "you have given kppp setuid permission and that\n"
		   "pppd is executable."),gpppdata.pppdPath());
    QMessageBox::warning(this, "Error", string.data());
    return;

  }

  QFileInfo info2(gpppdata.modemDevice());

  if(!info2.exists()){
    QString string;   
    string.sprintf(klocale->translate("kppp can not find:\n %s\nPlease make sure you have setup\n"
		   "your modem device properly\n"
		   "and/or adjust\n the location of the modem device on "
		   "the modem tab of\n"
		   "the setup dialog.\n Thank You"),gpppdata.modemDevice());
    QMessageBox::warning(this, "Error", string.data());
    return;
  }

  gpppdata.setId(ID_Edit->text());
  gpppdata.setPassword(PW_Edit->text());
  
  this->hide();

  QString tit = klocale->translate("Connecting to: ");
  tit += gpppdata.accname();
  con->setCaption(tit);

  con->show();

  if (!(log->isChecked())){
    debugwindow->hide();
  }
  else{
    debugwindow->clear();
    debugwindow->show();
    con->raise();
    con->debug->setText(klocale->translate("Log")); // set Log/Hide button text to Hide
  }	
  
  debugwindow->clear();
  emit begin_connect();
}


void XPPPWidget::disconnect() {

  app->processEvents();

  stats->stop_stats();
  terminatepppd();
  gpppdata.setpppdpid(-1);
  removedns();
  unlockdevice();
  
  con_win->stopClock();
  p_xppp->stopAccounting();
  con_win->hide();
  p_xppp->quit_b->setFocus();
  p_xppp->show();
}



void XPPPWidget::helpbutton() {

  app->invokeHTMLHelp("kppp/kppp.html","");

  /*  if ( fork() == 0 )
        {
                QString path = DOCS_PATH;
                path += "/kppp.html";
                execlp( "kdehelp", "kdehelp", path.data(), 0 );
                exit( 1 );
        }*/

}             



void XPPPWidget::quitbutton() {

  if(gpppdata.pppdpid() >= 0) {

    bool ok = QMessageBox::query(klocale->translate("Quit kPPP?"), 
				 klocale->translate("Exiting kPPP will close your PPP Session."),
				 klocale->translate("Yes"),
				 klocale->translate("No"));

    if(ok) {
      terminatepppd();
      gpppdata.setpppdpid(-1);
      removedns();
      unlockdevice();
      qApp->quit();
    }
  }
  else {
    qApp->quit();
  }

}


void XPPPWidget::startAccounting() {
  // load the ruleset

  if(!gpppdata.AcctEnabled())
    return;
  
  if(!accounting.loadRuleSet(gpppdata.accountingFile())) {
    QString s = klocale->translate("Can not load the accounting\nruleset \"");
    s += gpppdata.accountingFile();
    s += "\"!";
    QMessageBox::warning(this, klocale->translate("Error"), s.data());
    return;
  } 
  else
    {
      accounting.slotStart();
    }

}

void XPPPWidget::stopAccounting() {

  if(!gpppdata.AcctEnabled())
    return;

  accounting.slotStop();
}


void terminatepppd() {
  
#ifdef MY_DEBUG
printf("In terminatepppd(): I will attempt to kill pppd\n");
#endif

  int stat;

  if(gpppdata.pppdpid() >= 0) {

    if(kill(gpppdata.pppdpid(), SIGTERM) < 0)
      qApp->beep();
      wait(&stat);
  }

  gpppdata.setpppdpid(-1);

}

#include "main.moc"
