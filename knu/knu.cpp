/*
 *   KDE network utilities (knu.cpp)
 *
 *   Copyright (C) 1997  Bertrand Leconte
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */
/*
 * $Id$
 *
 * $Log$
 * Revision 1.2  1997/11/23 22:28:17  leconte
 * - Id and Log added in the headers
 * - Patch from C.Czezatke applied (preparation of KProcess new version)
 *
 */

#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#include <qmsgbox.h>
#include <kmsgbox.h>
#include <qfileinf.h>

#include <kstdaccel.h>

#include "knu.h"
#include "knu.moc"

#include "version.h"

#define FRAME_BORDER_WIDTH 6


// Static data
QList<TopLevel>  TopLevel::windowList;

QString   CaptionString;

void quit();

#define _(_s) klocale->translate(_s)


/**
 * check to see if this current tab is enabled in the config file
 */
bool
isTabEnabled(QString tabName, KConfig *kc)
{
  kc->setGroup(tabName);
  if (kc->readNumEntry("enable", 1) == 0) {
    return(FALSE);
  } else {
    return(TRUE);
  }
}
 

/**
 * Constructor 
 */
TopLevel::TopLevel(QWidget *, const char *name)
  : KTopLevelWidget(name)
{
  KConfig         *kc = kapp->getConfig();
  PingDlg         *pd;
  TracerouteDlg   *td;
  HostDlg         *hd;

  windowList.setAutoDelete(FALSE);
  windowList.append(this);

  /*
   * Set caption string
   */
  setCaption(CaptionString);
  
  kconfig = kc;
    
  /*
   * Create MenuBar
   */
  createMenu();
  setMenu(menuBar);

  
  /*
   * Create the tabctrl widget
   */
  tabCtrl = new KTabCtl(this);
  CHECK_PTR(tabCtrl);
  connect(tabCtrl, SIGNAL(tabSelected(int)), 
	  this, SLOT(slotTabChanged(int)));
  pagesNumber = 0;

  /*********************
   * Don't forget the slotConfig function while adding tabs
   *********************/

  // ping tab
  if (isTabEnabled("Ping", kc)) {
    pd = new PingDlg("ping", tabCtrl, _("&Ping"));
    CHECK_PTR(pd);
    tabCtrl->addTab(pd, pd->name());
    pages[pagesNumber] = pd;
    pagesNumber++;
  }

  // traceroute tab 
  if (isTabEnabled("Traceroute", kc)) {
    td = new TracerouteDlg("traceroute", tabCtrl, _("&Traceroute"));
    CHECK_PTR(td);
    tabCtrl->addTab(td, td->name());
    pages[pagesNumber] = td;
    pagesNumber++;
  }

  // host tab
  if (isTabEnabled("Host resolution", kc)) {
    hd = new HostDlg("host", tabCtrl, _("Host &resolution"));
    /* 
     * the "host" command name can be modified by 
     * HostDlg::search_for_binary if host is not found
     */
    CHECK_PTR(hd);
    tabCtrl->addTab(hd, hd->name());
    pages[pagesNumber] = hd;
    pagesNumber++;
  }

  if (pagesNumber == 0) {
    // There is *no* command enabled ! 

    QMessageBox::critical(this, caption(),
			  _("There is no command enabled\n"
			  "in the configuration file.\n\n"
			  "So I can't do anything...\n"));
    exit(-1);
  }
  setFrameBorderWidth(FRAME_BORDER_WIDTH);
  //tabCtrl->adjustSize();
  //tabCtrl->setMinimumSize(tabCtrl->size());
  setView(tabCtrl);		// for KTopLevelWidget
  currentTab = -1;

  /*
   * Enable File->Close if needed
   */
  if (windowList.count() > 1) {
    TopLevel *toplevel = windowList.first();
    while (toplevel) {
      toplevel->fileMenu->setItemEnabled(toplevel->closeIndex, TRUE);
      toplevel = windowList.next();
    }
  } else {
    fileMenu->setItemEnabled(closeIndex, FALSE);
  }

  menuBar->show();
  adjustSize();

  setMinimumSize(QMAX(tabCtrl->minimumSize().width()
		      +2*FRAME_BORDER_WIDTH, 500), 
		 QMAX(tabCtrl->minimumSize().height()
		      +2*FRAME_BORDER_WIDTH, 300));
}


/**
 * Create the KMenuBar
 */
void
TopLevel::createMenu()
{
  int mi;
  KStdAccel key = KStdAccel(kapp->getConfig());

  fileMenu = new QPopupMenu;
  CHECK_PTR(fileMenu);
  fileMenu->insertItem(_("&New window"), 
		       this, SLOT(slotNew()), key.openNew());
  fileMenu->insertSeparator();
  closeIndex = fileMenu->insertItem(_("&Close window"), 
				    this, SLOT(slotClose()), 
				    key.close());
  fileMenu->insertItem(_("&Quit"), this, SLOT(slotQuit()), 
		       key.quit());
  
  editMenu = new QPopupMenu;
  CHECK_PTR(editMenu);
  mi = editMenu->insertItem(_("&Copy"), this, SLOT(slotCopy()), 
			    key.copy());
  editMenu->setItemEnabled(mi, FALSE);
  editMenu->insertItem(_("Select &all"), 
		       this, SLOT(slotSelectAll()));
  editMenu->insertSeparator();
  editMenu->insertItem(_("C&lear output window"), 
		       this, SLOT(slotClear()));
  editMenu->insertSeparator();
  configIndex = editMenu->insertItem(_("P&references..."), 
				     this, SLOT(slotConfig()));

  helpMenu = new QPopupMenu;
  CHECK_PTR(helpMenu);
  helpMenu->insertItem(_("&Contents"),
		       this, SLOT(slotHelp()), key.help());
  helpMenu->insertSeparator();
  helpMenu->insertItem(_("&About"),
		       this, SLOT(slotAbout()), SHIFT+Key_F1);
  helpMenu->insertItem(_("About &Qt"), this, SLOT(slotAboutQt()));
  
  menuBar = new KMenuBar(this);
  CHECK_PTR(menuBar);
  menuBar->insertItem(_("&File"), fileMenu);
  menuBar->insertItem(_("&Edit"), editMenu);
  menuBar->insertSeparator();
  menuBar->insertItem(_("&Help"), helpMenu);
}


/**
 * Edit->Preferences menu entry
 */
void 
TopLevel::slotConfig()
{
  CommandCfgDlg   *configPages[10];
  OptionsDlg      *options;

  /*
   * Create options dialogBox
   */
  
  /* ping */
  CommandCfgDlg *ccd = new CommandCfgDlg(_("&Ping"), 0, "ping_cfg");
  CHECK_PTR(ccd);
  configPages[0] = ccd;
  
  /* traceroute */
  ccd = new CommandCfgDlg(_("&Traceroute"), 0, "traceroute_cfg");
  CHECK_PTR(ccd);
  configPages[1] = ccd;
  
  /* host resolution */
  HostCfgDlg *hcd = new HostCfgDlg(_("Host &resolution"), 0, "host_cfg");
  CHECK_PTR(hcd);
  configPages[2] = hcd;

  options = new OptionsDlg(configPages, 3, 0);
  CHECK_PTR(options);

  if (options->exec()) {
    kapp->getConfig()->sync();
  } else {
    // nothing to do
  }
  
  // distribute the new configs values among all the toplevel windows
  TopLevel *toplevel = TopLevel::windowList.first();
  
  while (toplevel) {
    toplevel->checkBinaryAndDisplayWidget();
    toplevel = TopLevel::windowList.next();
  }
}

/**
 * Pass checkBinaryAndDisplayWidget to each tab
 */
void 
TopLevel::checkBinaryAndDisplayWidget()
{
  int i;
  for (i=0; i<pagesNumber; i++) {
    pages[i]->checkBinaryAndDisplayWidget();
  }
}


/**
 * Destructor
 */
TopLevel::~TopLevel()
{
  windowList.removeRef(this);
  delete(menuBar);
}


/**
 * called when a tab is changed
 */
void
TopLevel::slotTabChanged(int newTab)
{
  if (newTab != currentTab) {
    if (currentTab >= 0) {
      pages[currentTab]->tabDeselected();
    }
    pages[newTab]->tabSelected();
    currentTab = newTab;
  }
}

/**
 * File->Quit menu entry
 */
void 
TopLevel::slotQuit()
{
  ::quit();
}

/**
 * Edit->Copy menu entry
 */
void 
TopLevel::slotCopy()
{
}

/**
 * Edit->Clear output window menu entry
 */
void 
TopLevel::slotClear()
{
  pages[currentTab]->clearOutput();
}

/**
 * File->New window menu entry
 */
void 
TopLevel::slotNew()
{
  TopLevel *toplevel = new TopLevel;
  toplevel->show();
}

/**
 * File->Close window menu entry
 */
void 
TopLevel::slotClose()
{
  TopLevel *toplevel;

  if (windowList.count()>1) {
    delete this;

    if (windowList.count() == 1) {
      toplevel = windowList.first();
      toplevel->fileMenu->setItemEnabled(toplevel->closeIndex, FALSE);
    }
  } else {
    ::quit();
  }
}

/**
 * Edit->Select all menu entry
 */
void 
TopLevel::slotSelectAll()
{
  pages[currentTab]->selectAll();
}

/**
 * Help->About menu entry
 */
void 
TopLevel::slotAbout()
{
  QString str;

  str = CaptionString.copy();
  if (strcmp(CaptionString, KNU_CAPTION)) {
    str += "\n("; str += KNU_CAPTION; str += ")";
  }

  QMessageBox::about( this, _("About..."),
		      str +
		      "\n"
		      "Version " + KNU_VERSION + "\n\n"
		      "Bertrand Leconte\n"
		      "<B.Leconte@mail.dotcom.fr>\n"
		      );
  
}


/**
 * Help->AboutQt menu entry
 */
void 
TopLevel::slotAboutQt()
{
  QMessageBox::aboutQt(this);
}

/**
 * This is called when the application is closed by the wm.
 * we make this act like the File->Quit menu entry
 */
void 
TopLevel::closeEvent (QCloseEvent *)
{
  TopLevel *toplevel;

  if (windowList.count()>1) {
    delete this;		// I _know_ that I used new...
    if (windowList.count() == 1) {
      // disable other window Close menu entry
      toplevel = windowList.first();
      toplevel->fileMenu->setItemEnabled(toplevel->closeIndex, FALSE);
    }
  } else {
    ::quit();
  }
}

/**
 * Help->Contents menu entry
 */
void 
TopLevel::slotHelp()
{
  kapp->invokeHTMLHelp("", "");
}


/*
 * Called when the user is login out.
 */
void
TopLevel::saveProperties(KConfig *kc)
{
  kc->writeEntry("CurrentTab", currentTab);
}

void
TopLevel::readProperties(KConfig *kc)
{
  //debug("TopLevel::readProperties(KConfig*)");
  int entry = kc->readNumEntry("CurrentTab", -1);
  //debug("readNumEntry = %d", entry);
  if (entry >= 0) {
    //tabCtrl->setCurrentTab(entry);
    // This is not implemented yet (bug somewhere. Qt ?)
  }
}



/**
 * Application quit function
 */
void
quit()
{
  // we have to scan the whole list and delete toplevel
  TopLevel *toplevel = TopLevel::windowList.first();
  
  while (toplevel) {
    delete toplevel;
    toplevel = TopLevel::windowList.next();
  }
  kapp->quit();
}


/**
 * Test if a filename is executable.
 * If there is not / in the filename, search in the path.
 */
bool
test_for_exec(QString filename)
{
  bool rc = FALSE;

  if (strchr(filename, '/')) {
    // It's a full path
    if (access(filename, X_OK)) {
      rc = FALSE;
    } else {
      rc = TRUE;
    }
  } else {
    // Check in the path
    char *pathEnv = getenv("PATH");
    char *p;
    char *path = (char *)malloc(strlen(pathEnv)+1);

    strcpy(path, pathEnv);

    p = strtok(path, ":");
    while (p != 0) {
      //debug("  ELEM = \"%s\"", p);
      if (test_for_exec((QString)p + "/" + filename)) {
	rc = TRUE;
	break;
      }
      p = strtok(NULL, ":");
    }

  }
  return(rc);
}


/**
 * Application main function
 */
int
main(int argc, char **argv)
{
  const QString  appname = KNU_APPNAME;
  CaptionString = KNU_CAPTION;

  KApplication  *a = new KApplication(argc, argv, appname);

  if (strcmp(a->getCaption(), KNU_APPNAME)) {
    CaptionString = a->getCaption();
  }

  // Session Management
  if (a->isRestored()) {
    int n = 1;
    while (KTopLevelWidget::canBeRestored(n)) {
      TopLevel *toplevel = new TopLevel((QWidget*)0, (const char*) appname);
      CHECK_PTR(toplevel);
      toplevel->restore(n);
      n++;
    } 
  } else {
    TopLevel *toplevel = new TopLevel((QWidget*)0, (const char*) appname);
    CHECK_PTR(toplevel);
    
    toplevel->show();
  }
  return a->exec();
}

