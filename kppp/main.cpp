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

#include <config.h>


#include <qfileinf.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <locale.h>
#include <errno.h>

#ifdef HAVE_SYS_PARAM_H
#include <sys/param.h>
#endif

#ifdef BSD
#include <stdlib.h>
#else
#include <getopt.h>
#endif

#include <kmsgbox.h>
#include <kquickhelp.h>
#include <kbuttonbox.h>
#include <kiconloader.h>

#include "acctselect.h"
#include "main.h"
#include "version.h"
#include "macros.h"
#include "pap.h"
#include "chap.h"
#include "docking.h"
#include "runtests.h"
#include "modem.h"
#include "ppplog.h"
#include "log.h"
#include "groupbox.h"
#include "newwidget.h"
#include "requester.h"

#include <X11/Xlib.h>

KPPPWidget*	p_kppp;
QString 	cmdl_account;

bool	have_cmdl_account;
bool    quit_on_disconnect = false;
bool    terminate_connection = false;

// this is needed for volume accounting and updated in pppstatdlg.cpp
int totalbytes;

// for testing purposes
bool TESTING=0;

QString local_ip_address;
QString remote_ip_address;
QString pidfile;

void usage(char* progname) {
  fprintf(stderr, "%s -- valid command line options:\n", progname);
  fprintf(stderr, " -h : describe command line options\n");
  fprintf(stderr, " -c account_name : connect to account account_name\n");
  fprintf(stderr, " -k : terminate an existing connection\n");
  fprintf(stderr, " -q : quit after end of connection\n");
  fprintf(stderr, " -r rule_file : check syntax of rule_file\n");
  shutDown(1);
}

void banner(char* progname){
  fprintf(stderr,"%s version " KPPPVERSION "\n",progname); 
  fprintf(stderr,"(c) 1997-1998 Bernd Johannes Wuebben ");
  fprintf(stderr,"<wuebben@kde.org>\n");
  fprintf(stderr,"(c) 1997-1998 Mario Weilguni ");
  fprintf(stderr,"<mweilguni@kde.org>\n");
  fprintf(stderr,"(c) 1998 Harri Porten <porten@kde.org>\n");
  fprintf(stderr,"Use -h for the list of valid command line options.\n\n");
  shutDown(0);
}

void showNews() {
  /*
   * Introduce the QuickHelp feature to new users of this version
   */
  #define QUICKHELP_HINT "Hint_QuickHelp"
  if(gpppdata.readNumConfig(GENERAL_GRP, QUICKHELP_HINT, 0) == 0) {
    QDialog dlg(0, 0, true);
    dlg.setCaption(i18n("Recent changes in kppp"));
    
    QVBoxLayout *tl = new QVBoxLayout(&dlg, 10, 10);
    QHBoxLayout *l1 = new QHBoxLayout(10);
    QVBoxLayout *l2 = new QVBoxLayout(10);
    tl->addLayout(l1);

    QLabel *icon = new QLabel(&dlg);
    icon->setPixmap(ICON("exclamation.xpm"));
    icon->setFixedSize(icon->sizeHint());
    l1->addWidget(icon);
    l1->addLayout(l2);

    QLabel *l = newLabel(i18n("From version 1.4.8 on, kppp has a new feature\n"
			      "called \"Quickhelp\". It's similar to a tooltip,\n"
			      "but you can activate it whenever you want.\n"
			      "\n"
			      "To activate it, simply click on a control like\n"
			      "a button or a label with the right mouse button.\n"
			      "If the item supports Quickhelp, a popup menu\n"
			      "will appear leading to Quickhelp.\n"
			      "\n"
			      "To test it, right-click somewhere in this text."),
			 &dlg);
    
    QCheckBox *cb = new QCheckBox(i18n("Don't show this hint again"), &dlg);
    cb->setFixedSize(cb->sizeHint());

    KButtonBox *bbox = new KButtonBox(&dlg);
    bbox->addStretch(1);
    QPushButton *ok = bbox->addButton(i18n("Ok"));
    ok->setDefault(true);
    dlg.connect(ok, SIGNAL(clicked()),
		&dlg, SLOT(accept()));
    bbox->addStretch(1);
    bbox->layout();

    l2->addWidget(l);
    l2->addWidget(cb);
    tl->addWidget(bbox);
    tl->freeze();

    KQuickHelp::add(cb, 
    KQuickHelp::add(l, i18n(
			    "This is an example for <b>QuickHelp</b>.\n"
			    "This window will stay open until you\n"
			    "click a mouse button or a hit a key.\n")));

    dlg.exec();
    if(cb->isChecked())
      gpppdata.writeConfig(GENERAL_GRP, QUICKHELP_HINT, 1);   
  }
}

extern "C" {

static int kppp_x_errhandler( Display *dpy, XErrorEvent *err ) {
  char errstr[256]; // safe
  
  /*
    if(gpppdata.pppdpid() >= 0) {
    kill(gpppdata.pppdpid(), SIGTERM);
    }
    
    p_kppp->stopAccounting();
    removedns();
    unlockdevice();*/
  
  XGetErrorText( dpy, err->error_code, errstr, 256 );
  fatal( "X Error: %s\n  Major opcode:  %d", errstr, err->request_code );
  return 0;
}


static int kppp_xio_errhandler( Display * ) {
  if(gpppdata.get_xserver_exit_disconnect()) {
    fprintf(stderr, "X11 Error!\n");
    if(gpppdata.pppdpid() >= 0)
      kill(gpppdata.pppdpid(), SIGTERM);    

    p_kppp->stopAccounting();
    removedns();
    Modem::modem->unlockdevice();
    return 0;
  } else{
    fatal( "%s: Fatal IO error: client killed", "kppp" );
    return 0;
  }
}

} /* extern "C" */


void make_directories() {
  QDir dir;

  QString d = KApplication::localkdedir();

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

  QString logdir = QDir::homeDirPath() + "/";
  logdir += ACCOUNTING_PATH "/Log";

  dir.setPath(logdir.data());
  if(!dir.exists()){
    dir.mkdir(logdir.data());
    chown(logdir.data(),getuid(),getgid());
    chmod(logdir.data(),S_IRUSR | S_IWUSR | S_IXUSR);
  }
}

#define MAX_NAME_LENGTH    64
pid_t fpid; // TODO: use gpppdata.suidChildPid() instead

int main( int argc, char **argv ) {

  // Don't insert anything above this line unless you really know what
  // you're doing. We're most likely running setuid root here,
  // until we drop this status a few lines below.
  int sockets[2];
  //  pid_t fpid;
  if(socketpair(AF_UNIX, SOCK_DGRAM, 0, sockets) != 0) {
    fprintf(stderr, "error creating socketpair !\n");
    exit(1);
  }

  if((fpid = fork())<0) {
    perror("kppp: fork() failed");
    exit(1);
  }

  if(fpid==0) {
    // child process
    // make process leader of new group
    setsid();
    close(sockets[0]);
    (void *) new Opener(sockets[1]);
    // we should never get here
    _exit(1);
  }
  // parent process
  close(sockets[1]);

  // drop setuid status
  setuid(getuid());
  setgid(getgid());
  //
  // end of setuid-dropping block.
  // 

  (void *) new Requester(sockets[0]);

  if(securityTests() != TEST_OK)
    shutDown(1);

  KApplication a(argc, argv, "kppp");

  // set portable locale for decimal point
  setlocale(LC_NUMERIC ,"C");

  int c;
  opterr = 0;
  while ((c = getopt(argc, argv, "c:khvr:qt")) != EOF) {
    switch (c)
      {

      case '?':
	fprintf(stderr, "%s: unknown option \"%s\"\n", 
		argv[0], argv[optind-1]);
	usage(argv[0]);
	shutDown(1);
      case 'c':
	{
	  // copy at most MAX_NAME_LENGTH bytes
	  char tmp[MAX_NAME_LENGTH];
	  strncpy(tmp, optarg, MAX_NAME_LENGTH-1);
	  
	  // terminate string
	  tmp[MAX_NAME_LENGTH-1] = 0; 
	  cmdl_account = tmp;
	  break;
	}
      case 'k':
        terminate_connection = true;
        break;
      case 'h':
	usage(argv[0]);
	break;
      case 'v':
	banner(argv[0]);
	break;
      case 'q':
	quit_on_disconnect = true;
	break;
      case 'r':
	{
	  // drop root
	  setuid(getuid());
          setgid(getgid());

	  // we need a KAppliction for locales, create one
	  shutDown(RuleSet::checkRuleFile(optarg));
	}
      case 't':
	TESTING=true;
	break;
      }
  }

  if(!cmdl_account.isEmpty()) {
    have_cmdl_account = true;
    Debug("cmdl_account:%s:\n",cmdl_account.data());
  }

  // make sure that nobody can read the password from the
  // config file
  QString configFile = a.localconfigdir() + "/" + a.appName() + "rc";
  if(access(configFile.data(), F_OK) == 0) {
    chown(configFile.data(), getuid(), getgid());
    chmod(configFile.data(), S_IRUSR | S_IWUSR);
  }
  make_directories();
  
  QString msg;
  int pid = create_pidfile();
  if(pid < 0) {
    msg.sprintf(i18n("kppp can't create or read from\n%s."), pidfile.data());
    QMessageBox::warning(0L, i18n("Error"), msg.data());
    shutDown(1);
  }
  
  if (terminate_connection) {
    setuid(getuid());
    setgid(getgid());
    if (pid > 0)
      kill(pid, SIGINT);
    else
      remove_pidfile();
    shutDown(0);
  }
  
  if (pid > 0) {
    msg.sprintf(i18n("kppp has detected a %s file.\n\n"
                     "Another instance of kppp seems to be "
                     "running under\nprocess-ID %d.\n\n"
                     "Make sure that you are not running another "
                     "kppp,\ndelete the pid file, and restart kppp."),
                pidfile.data(), pid);
    QMessageBox::warning(0L, i18n("Error"), msg.data());
    shutDown(1);
  }
  
  // Mario: testing
  if(TESTING) {
    //    gpppdata.open();
    //    PPPL_ShowLog();
    //    return a.exec();
  }

  KPPPWidget kppp;
  p_kppp = &kppp;

  // store id of fork()'ed process
  gpppdata.setSuidChildPid(fpid);
  Debug("suidChildPid: %i\n", (int) gpppdata.suidChildPid());

  // keep user informed about recent changes
  if(!have_cmdl_account)
    showNews();

  a.setMainWidget(&kppp);
  a.setTopWidget(&kppp);

  // we really don't want to die accidentally, since that would leave the
  // modem connected. If you really really want to kill me you must send 
  // me a SIGKILL.
  signal(SIGINT, sigint);
  signal(SIGTERM, SIG_IGN);
  signal(SIGHUP, SIG_IGN);
  signal(SIGCHLD, dieppp);

  XSetErrorHandler( kppp_x_errhandler );
  XSetIOErrorHandler( kppp_xio_errhandler );

  int ret = a.exec();

  remove_pidfile();

  //  return ret;
  shutDown(ret); // okay ?
}



KPPPWidget::KPPPWidget( QWidget *parent, const char *name )
  : QWidget(parent, name) 
{
  tabWindow = 0;

  bool config;
  config = gpppdata.open();

  // before doing anything else, run a few tests

  int result = runTests();
  if(result == TEST_CRITICAL)
    shutDown(4);

  QVBoxLayout *tl = new QVBoxLayout(this, 10, 10);

  fline1 = new QFrame(this);
  fline1->setFrameStyle(QFrame::HLine |QFrame::Sunken);
  fline1->setFixedHeight(3);
  tl->addWidget(fline1);

  QGridLayout *l1 = new QGridLayout(3, 4);
  tl->addLayout(l1);
  l1->addColSpacing(0, 10);
  l1->addColSpacing(3, 10);
  l1->setColStretch(1, 3);
  l1->setColStretch(2, 4);

  label1 = new QLabel(this);
  label1->setText(i18n("Connect to: "));
  MIN_SIZE(label1);
  l1->addWidget(label1, 0, 1);

  connectto_c = new QComboBox(true,this);
  connect(connectto_c, SIGNAL(activated(int)), 
	  SLOT(newdefaultaccount(int)));
  MIN_SIZE(connectto_c);
  l1->addWidget(connectto_c, 0, 2);  

  ID_Label = new QLabel(this);
  ID_Label->setText(i18n("Login ID:"));
  MIN_SIZE(ID_Label);
  l1->addWidget(ID_Label, 1, 1);

  // the entry line for usernames
  ID_Edit = new QLineEdit(this);
  MIN_WIDTH(ID_Edit);
  FIXED_HEIGHT(ID_Edit);
  l1->addWidget(ID_Edit, 1, 2);
  connect(ID_Edit, SIGNAL(returnPressed()),
	  this, SLOT(enterPressedInID()));
  KQuickHelp::add(ID_Label, 
  KQuickHelp::add(ID_Edit,
		  i18n("Type in the username that you got from your\n"
		       "ISP. This is especially important for PAP\n"
		       "and CHAP. You may omit this when you use\n"
		       "terminal-based or script-based authentication.\n"
		       "\n"
		       "<b>Important</b>: case is important here:\n"
		       "<i>myusername</i> is not the same as <i>MyUserName</i>!")));

  PW_Label = new QLabel(this);
  PW_Label->setText(i18n("Password:"));
  MIN_SIZE(PW_Label);
  l1->addWidget(PW_Label, 2, 1);

  PW_Edit= new QLineEdit(this);
  PW_Edit->setEchoMode(QLineEdit::Password);
  MIN_WIDTH(PW_Edit);
  FIXED_HEIGHT(PW_Edit);
  l1->addWidget(PW_Edit, 2, 2);
  connect(PW_Edit, SIGNAL(returnPressed()),
	  this, SLOT(enterPressedInPW()));
 
  KQuickHelp::add(PW_Label, 
  KQuickHelp::add(PW_Edit,
		  i18n("Type in the password that you got from your\n"
		       "ISP. This is especially important for PAP\n"
		       "and CHAP. You may omit this when you use\n"
		       "terminal-based or script-based authentication.\n"
		       "\n"
		       "<b>Important</b>: case is important here:\n"
		       "<i>mypassword</i> is not the same as <i>MyPassword</i>!")));


  QHBoxLayout *l3 = new QHBoxLayout;
  tl->addSpacing(5);
  tl->addLayout(l3);
  tl->addSpacing(5);
  l3->addSpacing(10);
  log = new QCheckBox(i18n("Show Log Window"), this);
  connect(log, SIGNAL(toggled(bool)), 
	  this, SLOT(log_window_toggled(bool)));
  log->setChecked(gpppdata.get_show_log_window());
  MIN_SIZE(log);
  l3->addWidget(log);
  KQuickHelp::add(log, 
		  i18n("This controls whether a log window is shown.\n"
		       "A log window shows the communication between\n"
		       "<i>kppp</i> and your modem. This will help you\n"
		       "in tracking down problems.\n"
		       "\n"
		       "Turn it off if <i>kppp</i> routinely connects without\n"
		       "problems"));

  fline = new QFrame(this);
  fline->setFrameStyle(QFrame::HLine |QFrame::Sunken);
  fline->setFixedHeight(3);
  tl->addWidget(fline);

  QHBoxLayout *l2 = new QHBoxLayout;
  tl->addLayout(l2);

  int minw = 0;
  quit_b = new QPushButton(i18n("Quit"), this);
  connect( quit_b, SIGNAL(clicked()), SLOT(quitbutton()));
  MIN_HEIGHT(quit_b);
  if(quit_b->sizeHint().width() > minw)
    minw = quit_b->sizeHint().width();

  setup_b = new QPushButton(i18n("Setup"), this);
  connect( setup_b, SIGNAL(clicked()), 
	   SLOT(expandbutton()));
  MIN_HEIGHT(setup_b);
  if(setup_b->sizeHint().width() > minw)
    minw = setup_b->sizeHint().width();

  if (!config) 
    setup_b->setEnabled(false);

  help_b = new QPushButton(i18n("Help"), this);
  connect( help_b, SIGNAL(clicked()), SLOT(helpbutton()));
  MIN_HEIGHT(help_b);
  if(help_b->sizeHint().width() > minw)
    minw = help_b->sizeHint().width();

  connect_b = new QPushButton(i18n("Connect"), 
			      this, "connect_b");
  connect_b->setFocus();
  connect(connect_b, SIGNAL(clicked()), SLOT(connectbutton()));
  MIN_HEIGHT(connect_b);
  if(connect_b->sizeHint().width() > minw)
    minw = connect_b->sizeHint().width();

  //  minw = QMAX(minw,70);
  quit_b->setMinimumWidth(minw);
  setup_b->setMinimumWidth(minw);
  help_b->setMinimumWidth(minw);
  connect_b->setMinimumWidth(minw);

  l2->addWidget(quit_b);
  l2->addWidget(setup_b);
  l2->addWidget(help_b);
  l2->addSpacing(20);
  l2->addWidget(connect_b);

  tl->freeze();

  (void)new Modem;

  // we also connect cmld_start to the connectbutton so that I can run
  // the dialer through a command line argument
  connect(this,SIGNAL(cmdl_start()),this,SLOT(connectbutton())); 

  con_win = new ConWindow(0,"conw",this);
  KWM::setMiniIcon(con_win->winId(), kapp->getMiniIcon());
  con_win->setGeometry(QApplication::desktop()->width()/2-160,
		    QApplication::desktop()->height()/2-55,
		    320,110);
  KWM::setMiniIcon(con_win->winId(), kapp->getMiniIcon());

  // connect to the accounting object
  connect(&accounting, SIGNAL(changed(QString, QString)),
	  con_win, SLOT(slotAccounting(QString, QString)));

  stats = new PPPStatsDlg(0,"stats",this);
  stats->hide();

  (void)new DockWidget("dockw");

  debugwindow = new DebugWidget(0,"debugwindow");
  KWM::setMiniIcon(debugwindow->winId(), kapp->getMiniIcon());
  debugwindow->setGeometry(QApplication::desktop()->width()/2+190,
		    QApplication::desktop()->height()/2-55,
		    debugwindow->width(),debugwindow->height());
  debugwindow->hide();

  // load up the accounts combo box

  resetaccounts();
  con = new ConnectWidget(0, "con");
  KWM::setMiniIcon(con->winId(), kapp->getMiniIcon());
  connect(this, SIGNAL(begin_connect()),con, SLOT(preinit()));
  con->setGeometry(QApplication::desktop()->width()/2-175,
		    QApplication::desktop()->height()/2-55,
		    350,110);

  // connect the ConnectWidgets various signals
  connect(con, SIGNAL(debugMessage(const char *)),
	  debugwindow, SLOT(statusLabel(const char *)));
  connect(con, SIGNAL(debugMessage(QString)),
	  debugwindow, SLOT(statusLabel(QString)));
  connect(con, SIGNAL(toggleDebugWindow()),
	  debugwindow, SLOT(toggleVisibility()));
  connect(con, SIGNAL(debugPutChar(char)),
	  debugwindow, SLOT(addChar(char)));
  connect(con, SIGNAL(startAccounting()),
	  this, SLOT(startAccounting()));
  connect(con, SIGNAL(stopAccounting()),
	  this, SLOT(stopAccounting()));
    
  setGeometry(QApplication::desktop()->width()/2 - width()/2,
	      QApplication::desktop()->height()/2 - height()/2,
	      width(), height());

  if(have_cmdl_account){
    bool result;
    result = gpppdata.setAccount(cmdl_account.data());
    if (!result){
      QString string;
      string.sprintf(i18n("No such Account:\n%s"),cmdl_account.data());
      QMessageBox::warning(this, i18n("Error"), string.data());
      have_cmdl_account = false;
      this->show();
    } else
      emit cmdl_start();    
  } else
    show();
}


void KPPPWidget::prepareSetupDialog() {
  if(tabWindow == 0) {
    tabWindow = new QTabDialog( 0, 0, TRUE );
    tabWindow->setCaption( i18n("kppp Configuration") );
    tabWindow->setOkButton(i18n("OK"));
    tabWindow->setCancelButton(i18n("Cancel"));
    tabWindow->setFixedSize( 355, 350 ); // this doesn't seem to work in Qt 1.1

    accounts = new AccountWidget(tabWindow,"accounts");
    connect(accounts, SIGNAL(resetaccounts()),
	    this, SLOT(resetaccounts()));
    connect(accounts, SIGNAL(resetCosts(const char *)),
	    &accounting, SLOT(resetCosts(const char *)));
    modem1 = new ModemWidget(tabWindow);
    modem2 = new ModemWidget2(tabWindow);
    general = new GeneralWidget(tabWindow);
    graph = new GraphSetup(tabWindow);
    about  = new AboutWidget(tabWindow);
    
    tabWindow->addTab( accounts, i18n("Accounts") );
    tabWindow->addTab( modem1, i18n("Device") );
    tabWindow->addTab( modem2, i18n("Modem") );
    tabWindow->addTab( general, i18n("PPP") );
    tabWindow->addTab( graph, i18n("Graph") );
    tabWindow->addTab( about, i18n("About") );
  }
}


void KPPPWidget::enterPressedInID() {
  PW_Edit->setFocus();
}


void KPPPWidget::enterPressedInPW() {
  connect_b->setFocus();
}


void KPPPWidget::log_window_toggled(bool on) {
  gpppdata.set_show_log_window(on);
}


void KPPPWidget::setup() {
  prepareSetupDialog();

  if(tabWindow->exec())
    gpppdata.save();
  else
    gpppdata.cancel();
}


void KPPPWidget::resetaccounts() {
  connectto_c->clear();

  if(gpppdata.count() == 0) {
    connectto_c->setEnabled(false);
    connect_b->setEnabled(false);
    log->setEnabled(false);
    ID_Edit->setEnabled(false);
    PW_Edit->setEnabled(false);
  }
  else {
    connectto_c->setEnabled(true);
    connect_b->setEnabled(true);
    log->setEnabled(true);
    ID_Edit->setEnabled(true);
    PW_Edit->setEnabled(true);
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
	
	ID_Edit->setText(gpppdata.storedUsername());
	PW_Edit->setText(gpppdata.storedPassword());
    }
  }
  else 
    if(gpppdata.count() > 0) {
       gpppdata.setDefaultAccount(connectto_c->text(0));
    }


  connect(ID_Edit, SIGNAL(textChanged(const char *)),
 	  this, SLOT(usernameChanged(const char *)));

  connect(PW_Edit, SIGNAL(textChanged(const char *)),
 	  this, SLOT(passwordChanged(const char *)));
}


void sigint(int) {
  Debug("Received a SIGINT\n");
  signal(SIGINT, sigint); // reinstall the sig handler

  // interrupt dial up
  if (p_kppp->con->isVisible())
    emit p_kppp->con->cancelbutton();

  // disconnect if online
  if (gpppdata.pppdpid() != -1)
    emit p_kppp->disconnect();
}


//Note: this is a friend function of KPPPWidget class (kppp)
void dieppp(int sig) {
  pid_t id;
  int st;

  Debug("Received a signal: %d\n",sig);

  if(sig == SIGCHLD){    
    Debug("The Signal received was a SIGCHLD\n");
    signal(SIGCHLD, dieppp); // reinstall the sig handler
    id = wait(&st);

    // TODO
//     if (id > 0) {
//       if(WIFEXITED(st))
//         Debug("process %i exited normally with return code %i\n", id, WEXITSTATUS(st));
//       else
//         Debug("process %i exited abnormally.\n", id);
//     } else
//       Debug("wait() returned error %i\n", errno);    
    
    // if we are not connected pppdpid is -1 so have have to check for that
    // in the followin line to make sure that we don't raise a false alarm
    // such as would be the case when the log file viewer exits.
    if(id == gpppdata.pppdpid() && gpppdata.pppdpid() != -1) { 
      Debug("It was pppd that died\n");

      // when we killpppd() on Cancel in ConnectWidget 
      // we set pppid to -1 so we won't 
      // enter this block

      // just to be sure
      Requester::rq->removeSecret(AUTH_PAP);
      Requester::rq->removeSecret(AUTH_CHAP);

      gpppdata.setpppdpid(-1);
      
      Debug("Executing command on disconnect since pppd has died:\n");
      execute_command(gpppdata.command_on_disconnect());

      p_kppp->stopAccounting();

      p_kppp->con_win->stopClock();
      DockWidget::dock_widget->stop_stats();
      DockWidget::dock_widget->undock();      

      if(!gpppdata.pppdError())
        gpppdata.setpppdError(E_PPPD_DIED);
      removedns();
      Modem::modem->unlockdevice();      
      
      if(!gpppdata.automatic_redial()) {
	p_kppp->quit_b->setFocus();
	p_kppp->show();
	p_kppp->con_win->stopClock();
	p_kppp->stopAccounting();
	p_kppp->con_win->hide();
	p_kppp->con->hide();

	gpppdata.setpppdpid(-1);
	
	KApplication::beep();
	QString msg;
	switch (gpppdata.pppdError())
	  {
	  case E_IF_TIMEOUT:
	    msg = "Timeout expired while waiting for the PPP interface ";
	    msg += "to come up!";
	    break;
	  
	  default: 
	    msg = "The pppd daemon died unexpectedly!";
	  }
	
	KMsgBox msgb(0, 
		     i18n("Error"), 
		     i18n(msg),
		     KMsgBox::STOP | KMsgBox::DB_FIRST,
		     i18n("OK"),
		     i18n("Details..."));
	if(msgb.exec() == 2)
	  PPPL_ShowLog();
      } else { /* reconnect on disconnect */
	Debug("Trying to reconnect ... \n");

        if(gpppdata.authMethod() == AUTH_PAP)
          Requester::rq->setPAPSecret(gpppdata.storedUsername(),
                                      gpppdata.password.data());
        if(gpppdata.authMethod() == AUTH_CHAP)
          Requester::rq->setCHAPSecret(gpppdata.storedUsername(),
                                       gpppdata.password.data());

	p_kppp->con_win->hide();
	p_kppp->con_win->stopClock();
	p_kppp->stopAccounting();
	gpppdata.setpppdpid(-1);
	KApplication::beep();
	emit p_kppp->cmdl_start();
      }
    }
    //    if(id == gpppdata.suiChildPid() && gpppdata.suidChildPid() != -1) {
    if(id == fpid && fpid != -1) {
      Debug("It was the setuid child that died\n");

      //      gpppdata.setSuidChildPid(-1);
      fpid = -1;
    }
  }
}


void KPPPWidget::newdefaultaccount(int i) {
  gpppdata.setDefaultAccount(connectto_c->text(i));
  gpppdata.save();
  ID_Edit->setText(gpppdata.storedUsername());
  PW_Edit->setText(gpppdata.storedPassword());
}


void KPPPWidget::expandbutton() {
  setup();
}


void KPPPWidget::connectbutton() {

  // make sure to connect to the account that is selected in the combo box
  // (exeption: an account given by a command line argument)
  if(!have_cmdl_account) gpppdata.setAccount(connectto_c->currentText());

  QFileInfo info(gpppdata.pppdPath());

  if(!info.exists()){
    QString string;   
    string.sprintf(i18n("kppp can not find:\n %s\n"
    			"Please install pppd properly and/or adjust\n"
    			"the location of the pppd executable on\n"
			"the PPP tab of the setup dialog.\n"
			"This will insure that kppp can find the\n"
			"PPP daemon.\n"
			"Thank You"),
		   gpppdata.pppdPath());
    QMessageBox::warning(this, i18n("Error"), string.data());
    return;
  }

  if(!info.isExecutable()){

    QString string;   
    string.sprintf(i18n("kppp can not execute:\n %s\nPlease make sure that"
		   "you have given kppp setuid permission and that\n"
		   "pppd is executable."),gpppdata.pppdPath());
    QMessageBox::warning(this, 
			 i18n("Error"),
			 string.data());
    return;

  }

  QFileInfo info2(gpppdata.modemDevice());

  if(!info2.exists()){
    QString string;   
    string.sprintf(i18n("kppp can not find:\n %s\nPlease make sure you have setup\n"
		   "your modem device properly\n"
		   "and/or adjust\n the location of the modem device on "
		   "the modem tab of\n"
		   "the setup dialog.\n Thank You"),gpppdata.modemDevice());
    QMessageBox::warning(this, 
			 i18n("Error"),
			 string.data());
    return;
  }

  gpppdata.setPassword(PW_Edit->text());

  // if this is a PAP account, ensure that password and username are
  // supplied
  if(gpppdata.authMethod() == AUTH_PAP) {
    if(strlen(ID_Edit->text()) == 0 || strlen(PW_Edit->text()) == 0) {
      QMessageBox::warning(this,
			   i18n("Error"),
			   i18n(
                           "You have selected the authentication\n"
			   "method PAP. This requires that you\n"
			   "supply a username and a password!"));
      return;
    } else {      
      if(!Requester::rq->setPAPSecret(gpppdata.storedUsername(),
                                      gpppdata.password.data())) {
	QString s;
	s.sprintf(i18n("Cannot create PAP authentication\n"
				     "file \"%s\""), PAP_AUTH_FILE);
	QMessageBox::warning(this,
			     i18n("Error"),
			     s.data());
	return;
      }
    }
  }

  // if this is a CHAP account, ensure that password and username are
  // supplied
  if(gpppdata.authMethod() == AUTH_CHAP) {
    if(strlen(ID_Edit->text()) == 0 || strlen(PW_Edit->text()) == 0) {
      QMessageBox::warning(this,
			   i18n("Error"),
			   i18n(
                           "You have selected the authentication\n"
			   "method CHAP. This requires that you\n"
			   "supply a username and a password!"));
      return;
    } else {      
      if(!Requester::rq->setCHAPSecret(gpppdata.storedUsername(),
                                       gpppdata.password.data())) {
	QString s;
	s.sprintf(i18n("Cannot create CHAP authentication\n"
				     "file \"%s\""), CHAP_AUTH_FILE);
	QMessageBox::warning(this,
			     i18n("Error"),
			     s.data());
	return;
      }
    }
  }
  
  if (strlen(gpppdata.phonenumber()) == 0) {
    QString s;
    s.sprintf(i18n("You must specify a telephone "
		   "number!"));
    QMessageBox::warning(this, i18n("Error"), s.data());
    return;
  }

  this->hide();

  QString tit = i18n("Connecting to: ");
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
    con->debug->setText(i18n("Log")); // set Log/Hide button text to Hide
  }	
  
  debugwindow->clear();
  emit begin_connect();
}


void KPPPWidget::disconnect() {
  if (strcmp(gpppdata.command_before_disconnect(), "") != 0) {
    con_win->hide();
    con->show();
    con->setCaption("Disconnecting ...");
    con->setMsg("Executing command before disconnection.");

    kapp->processEvents();
    pid_t id = execute_command(gpppdata.command_before_disconnect());
    int i, status;

    do {
      kapp->processEvents();
      i = waitpid(id, &status, WNOHANG);
      usleep(500000);
    } while (i == 0 && errno == 0);

    con->hide();
  }

  kapp->processEvents();

  stats->stop_stats();
  killpppd();

  execute_command(gpppdata.command_on_disconnect());
  
  Requester::rq->removeSecret(AUTH_PAP);
  Requester::rq->removeSecret(AUTH_CHAP);

  removedns();
  Modem::modem->unlockdevice();
  
  con_win->stopClock();
  p_kppp->stopAccounting();
  con_win->hide();
  
  if (DockWidget::dock_widget->isDocked()) {
    DockWidget::dock_widget->stop_stats();
    DockWidget::dock_widget->undock();
  }

  if(quit_on_disconnect)
    kapp->exit(0);
  else {
    this->quit_b->setFocus();
    this->show();
  }
}


void KPPPWidget::helpbutton() {
  kapp->invokeHTMLHelp("kppp/kppp.html","");
}             


void KPPPWidget::quitbutton() {
  if(gpppdata.pppdpid() >= 0) {    
    bool ok = QMessageBox::query(i18n("Quit kPPP?"), 
				 i18n("Exiting kPPP will close your PPP Session."),
				 i18n("Yes"),
				 i18n("No"));

    if(ok) {
      killpppd();
      execute_command(gpppdata.command_on_disconnect());
      removedns();
      Modem::modem->unlockdevice();
      kapp->quit();
    }
  } else {
    if (strcmp(gpppdata.accname(), "") != 0 && !gpppdata.storePassword())
      gpppdata.setStoredPassword("");
    kapp->quit();
  }
}


void KPPPWidget::rulesetLoadError() {
  QMessageBox::warning(this, 
		       i18n("Error"), 
		       ruleset_load_errmsg.data());
}


void KPPPWidget::startAccounting() {
  // volume accounting
  totalbytes = 0;

  // load the ruleset
  if(!gpppdata.AcctEnabled())
    return;
  
  if(!accounting.loadRuleSet(gpppdata.accountingFile())) {
    QString s= i18n("Can not load the accounting\nruleset \"");
    s += gpppdata.accountingFile();
    s += "\"!";

    // starting the messagebox with a timer will prevent us
    // from blocking the calling function ConnectWidget::timerEvent
    ruleset_load_errmsg = s;
    QTimer::singleShot(0, this, SLOT(rulesetLoadError()));
    return;
  } else
	accounting.slotStart();
}

void KPPPWidget::stopAccounting() {
  // store volume accounting
  if(totalbytes != 0)
    gpppdata.setTotalBytes(totalbytes);

  if(!gpppdata.AcctEnabled())
    return;

  accounting.slotStop();
}


void KPPPWidget::usernameChanged(const char *) {
  // store username for later use
  gpppdata.setStoredUsername(ID_Edit->text());
}


void KPPPWidget::passwordChanged(const char *) {
    // store the password if so requested
  if(gpppdata.storePassword())
    gpppdata.setStoredPassword(PW_Edit->text());
  else
    gpppdata.setStoredPassword("");
}


void KPPPWidget::setPW_Edit(const char *pw) {
  PW_Edit->setText(pw);
}


void killpppd() {
  int stat;
  pid_t pid = gpppdata.pppdpid();
  
  if(pid >= 0) {
    gpppdata.setpppdpid(-1);

    Debug("In killpppd(): Sending SIGTERM to %d\n", pid);    
    if(kill(pid, SIGTERM) < 0) {
      Debug("Error terminating %d. Sending SIGKILL\n", pid);
      
      if(kill(pid, SIGKILL) < 0)
        Debug("Error killing %d\n", pid);
      
      KApplication::beep();
      return;
    }

    waitpid(&stat);
  }
}


pid_t execute_command (const char *command) {
    if(!command || strcmp(command, "") ==0 || strlen(command) > COMMAND_SIZE)
    return (pid_t) -1;
    
  pid_t id;
    
  Debug("Executing command: %s\n", command);

  if((id = fork()) == 0) {
    // don't bother dieppp()
    signal(SIGCHLD, SIG_IGN);

    // close file descriptors
    for (int fd = 3; fd < 20; fd++)
      close(fd);

    // drop privileges if running setuid root
    setuid(getuid());
    setgid(getgid());
    
    system(command);
    _exit(0);
  }	 

  return id;
}


// Create a file containing the current pid. Returns 0 on success, 
// -1 on failure or the pid of an already running kppp process. 
pid_t create_pidfile() {
  int fd = -1;
  char pidstr[40]; // safe
  
  pidfile = KApplication::localkdedir() + PIDFILE;

  if(access(pidfile.data(), F_OK) == 0) {
    
    if((access(pidfile.data(), R_OK) < 0) ||
       (fd = open(pidfile.data(), O_RDONLY)) < 0)
      return -1;

    int sz = read(fd, &pidstr, 32);
    close (fd);
    if (sz <= 0)
      return -1;
    pidstr[sz] = '\0';

    Debug("found kppp.pid containing: %s\n", pidstr);

    int oldpid;
    int match = sscanf(pidstr, "%d", &oldpid);

    // found a pid in pidfile ?
    if (match < 1 || oldpid <= 0)
      return -1;

    // check if process exists
    if (kill((pid_t)oldpid, 0) == 0 || errno != ESRCH)
      return oldpid;

    Debug("pidfile is stale\n");
  }

  if((fd = open(pidfile.data(), O_WRONLY | O_CREAT,
                S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) == -1)
    return -1;

  fchown(fd, getuid(), getgid());

  sprintf(pidstr, "%d\n", getpid());
  write(fd, pidstr, strlen(pidstr));
  close(fd);

  return 0;
}


bool remove_pidfile() {
  struct stat st;
  
  // only remove regular files with user write permissions
  if(stat(pidfile.data(), &st) == 0 )
    if(S_ISREG(st.st_mode) && (access(pidfile.data(), W_OK) == 0)) {
      unlink(pidfile.data());
      return true;
    }

  fprintf(stderr, "error removing pidfile.\n");
  return false;
}


void shutDown(int status) {

  pid_t pid;
  Debug("shutDown(%i)", status);
  //  pid = gpppdata.suidChildPid();
  pid = fpid;
  printf("pid=%i\n", pid);
  if(pid > 0) {
    //  gpppdata.setSuidChildPid(-1);
    fpid = -1;
    //   kill(pid, SIGTERM);
    kill(pid, SIGKILL);
  }
  exit(status);
}

#include "main.moc"
