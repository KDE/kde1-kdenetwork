/*
 *   CommandDlg.cpp - Command Widget
 * 
 *   part of knu: KDE network utilities
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
 * - $Id$ and $Log$ added in the headers
 * - Patch from C.Czezatke applied (preparation of KProcess new version)
 *
 */

#include <errno.h>
#include <ctype.h>

#include <kapp.h>
#include <qregexp.h>
#include <kmsgbox.h>
#include <qsocknot.h>
#include <qfileinf.h>
#include <qpixmap.h>
#include <qmsgbox.h> 


#include "CommandDlg.h"
#include "CommandDlg.moc"

#ifdef HAVE_CONFIG
#include <config.h>
#endif

#define _(_s) klocale->translate(_s)

// This is the unit used to separate widgets
#define SEPARATION 10

// defined in knu.cpp
//extern QString search_for_binary(QString);
extern bool test_for_exec(QString);



/*
 * We want to remove the ampersand in a string
 */ 
QString removeAmpersand(const QString &s1)
{
  QString s2 = s1.copy();
  
  s2.replace(QRegExp("&"), "" );
  return(s2);
}



/**
 * Constructor
 */
CommandDlg::CommandDlg(QString commandName, 
		       QWidget* parent,	const char* name)
	: QWidget(parent, name)
{
  KConfig *kc = kapp->getConfig();

  configGroupName = removeAmpersand(name);
  kc->setGroup(configGroupName);
  commandFound = TRUE;
  firstTimeLauching = FALSE;

  if (!kc->hasKey("path")) {
    // It's the first execution, 
    // so we have to search for the pathname
    firstTimeLauching = TRUE;

    kc->writeEntry("path", commandName);
    if (!::test_for_exec(kc->readEntry("path"))) {
      commandFound = FALSE;
    } else {
      // All is OK : we can enable this tab.
      if (!kc->hasKey("enable")) {
	kc->writeEntry("enable", 1);
      }
    }
  } else {
    // We have a path in path=, so check if it still exists
    if (!::test_for_exec(kc->readEntry("path"))) {
      commandFound = FALSE;
    }
  }

  // Commit changes in configfile (if needed)
  kc->sync();

  if (commandFound) {
    commandArgs = new QLineEdit(this, "lineedit_1" );
    //CB commandArgs = new QComboBox(TRUE, this, "combobox_1" );//CB
    //CB commandArgs->setInsertionPolicy(QComboBox::AtTop);//CB

    commandArgs->setMaximumSize(QLayout::unlimited, 30);
    connect(commandArgs, SIGNAL(returnPressed()), 
	    this, SLOT(slotLauchCommand()));
    connect(commandArgs, SIGNAL(textChanged(const char*)), 
	    this, SLOT(slotEnableGoBtn(const char*)));
    commandArgs->setMaxLength(1024);
    commandArgs->setEchoMode(QLineEdit::Normal);
    commandArgs->setFrame(TRUE);
    
    commandLbl1 = new QLabel(commandArgs, _("H&ost:"), this);
    CHECK_PTR(commandLbl1);
    commandLbl1->adjustSize();
    commandLbl1->setFixedSize(commandLbl1->width(), 30);
    
    
    commandGoBtn = new QPushButton(this, "pushbutton_3" );
    commandGoBtn->setFixedSize(70, 30);
    connect(commandGoBtn, SIGNAL(clicked()),
	    this, SLOT(slotLauchCommand()));
    commandGoBtn->setText( _("&Go!") );
    commandGoBtn->setEnabled(FALSE);
    commandGoBtn->setAutoDefault(TRUE);
    isGoBtnEnabled=FALSE;
    //BL commandGoBtn->setFocusPolicy(QWidget::StrongFocus);
    
    commandStopBtn = new QPushButton(this, "pushbutton_4" );
    commandStopBtn->setFixedSize(70, 30);
    connect(commandStopBtn, SIGNAL(clicked()), 
	    this, SLOT(slotStopCommand()));
    commandStopBtn->setText(_("&Stop"));
    commandStopBtn->setEnabled(FALSE);
    
    commandTextArea = new QMultiLineEdit(this, "multilineedit_1" );
    commandTextArea->setReadOnly(TRUE);
    commandTextArea->setFocusPolicy(QWidget::NoFocus);

  } else {
    
    /*
     * Command not found
     */
    layoutNoBin = new QBoxLayout(this, QBoxLayout::TopToBottom, SEPARATION);
    CHECK_PTR(layoutNoBin);
    
    commandLblNoBin = new QLabel(_("This command binary was not found. \n"
				   "You can give its path "
				   "in the Edit->Preferences... menu."),
				 this);
    CHECK_PTR(commandLblNoBin);
    commandLblNoBin->setAlignment(AlignCenter);
    
    layoutNoBin->addWidget(commandLblNoBin);
    layoutNoBin->activate();
    
  }
}


/*
 * Destructor
 */
CommandDlg::~CommandDlg()
{
  slotStopCommand();
}


/**
 * This is called when the tab is selected, so we
 * can manage the focus
 */
void
CommandDlg::tabSelected()
{
  if (commandFound) {
    commandArgs->setFocus(); 
  }
}

/**
 * This is called when the tab is deselected, so we can
 * kill the command if needed
 */
void
CommandDlg::tabDeselected()
{
  slotStopCommand();
}

/**
 * This slot is connected to the lineedit, so we
 * can check if there is something in it to enable
 * the go button.
 */
void
CommandDlg::slotEnableGoBtn(const char *args)
{
  if (strlen(args) == 0) {
    if (isGoBtnEnabled) {
      isGoBtnEnabled = FALSE;
      commandGoBtn->setEnabled(isGoBtnEnabled);
    }
  } else {
    if (!isGoBtnEnabled) {
      isGoBtnEnabled = TRUE;
      commandGoBtn->setEnabled(isGoBtnEnabled);
    }
  }
}


/*
 * This function check if we can call the sub-program
 * (we make some basic checks on length and special characters)
 */
bool
CommandDlg::checkInput(QString *args)
{
  char c;
  int l;
  const char *p;
  
  l = args->length();
  if (l > 128) {
    return(FALSE);
  }
  p = (const char *)*args;
  while ((c = *p++)) {
    if ((!isalnum(c)) 
	&& (c!='.') && (c != '-') && (c != '/') && (c != '_')) {
      return(FALSE);
    }
  }
  return(TRUE);
}

/**
 * build the command line from widgets
 */
CommandDlg::buildCommandLine(QString args)
  childProcess.clearArguments();
{
  warning("CommandDlg::buildCommandLine must be derived");
  childProcess.clearArguments();
  childProcess.setExecutable("echo");
  childProcess << (const char *)args;
}


/*
 * this is called from the Go button
 */
void
CommandDlg::slotLauchCommand()
{
  QString str;
  QString args;

  if (childProcess.isRunning()) {
    return;
  }

  args = commandArgs->text();
  //CB args = commandArgs->currentText();//CB
  if (strlen(args) == 0) {
    // nothing to do (this should not be possible)
  } else {

    // Check the input
    if (!checkInput(&args)) {
      //warning("input not valid");
      commandArgs->selectAll();
      KApplication::beep();
      return;
    }

    // Install the "Stop" button, and hide "Go!"
    commandGoBtn->setEnabled(FALSE);
    commandStopBtn->setEnabled(TRUE);

    // Install waitCursor
    commandTextArea->setCursor(waitCursor);

    // separate commands with CR/LF
    if (commandTextArea->numLines() > 1) {
      int line;
      line = QMAX(commandTextArea->numLines()-2, 0);
      if (strlen(commandTextArea->textLine(line)) > 0) {
	commandTextArea->append("");
      }
    }

    //  Process creation
    buildCommandLine(args);

    connect(&childProcess, SIGNAL(processExited(KProcess *)), 
	    SLOT(slotProcessDead(KProcess *)));

    connect(&childProcess, SIGNAL(receivedStdout(KProcess *, char *, int)), 
	    this, SLOT(slotCmdStdout(KProcess *, char *, int)));
    connect(&childProcess, SIGNAL(receivedStderr(KProcess *, char *, int)), 
	    this, SLOT(slotCmdStdout(KProcess *, char *, int)));

    if (!childProcess.start(KProcess::NotifyOnExit, KProcess::AllOutput)) {
      // Process not started
      debug("Process not started");
      slotProcessDead(NULL);
      return;
    }
  }
}


/*
 * This is called by the Stop button
 */
void
CommandDlg::slotStopCommand()
{
  if (childProcess.isRunning()) {
    childProcess.kill(15);
  }
}


/*
 * Clean up all the things after the death of the command 
 * (gop button, cursor, ...)
 */
void
CommandDlg::slotProcessDead(KProcess *)
{
  // disconnect KProcess
  disconnect(&childProcess, SIGNAL(receivedStdout(KProcess *, char *, int)), 
	     this, SLOT(slotCmdStdout(KProcess *, char *, int)));
  disconnect(&childProcess, SIGNAL(receivedStderr(KProcess *, char *, int)), 
	     this, SLOT(slotCmdStdout(KProcess *, char *, int)));
  disconnect(&childProcess, SIGNAL(processExited(KProcess *)), 
	     this, SLOT(slotProcessDead(KProcess *)));
  
  // put back the "Go!" button up
  commandGoBtn->setEnabled(TRUE);
  commandStopBtn->setEnabled(FALSE);

  commandTextArea->setCursor(ibeamCursor);
  
  // to be ready for a new command
  commandArgs->selectAll();
  commandArgs->setFocus();
}

/*
 * Read the output of the command on a socket and append
 * it to the multilineedit.
 *
 * This is called by the main select loop in Qt (QSocketNotifier)
 */
void
CommandDlg::slotCmdStdout(KProcess *, char *buffer, int buflen)
{
  int  line, col;
  char *p;

  buffer[buflen] = 0;		// mark eot
  //debug("text = \"%s\"", buffer);

  // goto end of data
  line = QMAX(commandTextArea->numLines() - 1, 0);
  p = (char*)commandTextArea->textLine(line);
  col = 0;
  if (p != NULL) {
    col = strlen(p);
  }
  commandTextArea->setCursorPosition(line, col);
  
  // and insert text here
  commandTextArea->insertAt(buffer, line, col);
  commandTextArea->setCursorPosition(commandTextArea->numLines(), 0);
}

/**
 * clear the output (slot)
 */
void
CommandDlg::clearOutput()
{
  commandTextArea->clear();
}


/* ******************************************************************** */

CommandCfgDlg::CommandCfgDlg(const char *tcs, 
			     QWidget *parent, const char *name)
  : QWidget(parent, name)
{
  // We will have to give this back to the ConfigWindow
  tabCaptionString = tcs;
  configGroupName = removeAmpersand(tcs);
}

QWidget *
CommandCfgDlg::makeWidget(QWidget *parent, bool makeLayouts)
{
#define SET_ADJUSTED_FIXED_SIZE(_wdgt) { _wdgt->adjustSize();	\
                  _wdgt->setFixedSize(_wdgt->size()); }

  KConfig   *kc = kapp->getConfig();
  
  cfgWidget = new QWidget(parent);

  // We will have to give this back to the ConfigWindow
  kc->setGroup(configGroupName);

  // Widgets creation
  cfgBinGB = new QGroupBox(_("Binary"), cfgWidget);
  CHECK_PTR(cfgBinGB);

  cfgBinNameLE = new QLineEdit(cfgBinGB);
  CHECK_PTR(cfgBinNameLE);
  cfgBinNameLE->setMinimumSize(cfgBinGB->fontMetrics()
			         .width("----------------------"), 
			       (int)(2*cfgBinGB->fontMetrics().height()));
  cfgBinNameLE->setMaximumSize(QLayout::unlimited,
			       (int)(2*cfgBinGB->fontMetrics().height()));
  
  cfgBinNameLbl = new QLabel(cfgBinNameLE, _("Path&name:"), cfgBinGB);
  CHECK_PTR(cfgBinNameLbl);
  SET_ADJUSTED_FIXED_SIZE(cfgBinNameLbl);

  cfgBinArgLE = new QLineEdit(cfgBinGB);
  CHECK_PTR(cfgBinArgLE);
  cfgBinArgLE->setMinimumSize(cfgBinGB->fontMetrics()
			      .width("----------------------"), 
			      (int)(2*cfgBinGB->fontMetrics().height()));
  cfgBinArgLE->setMaximumSize(QLayout::unlimited,
			      (int)(2*cfgBinGB->fontMetrics().height()));
  
  cfgBinArgLbl = new QLabel(cfgBinArgLE, _("Additional &arguments:"),
			    cfgBinGB);
  CHECK_PTR(cfgBinArgLbl);
  SET_ADJUSTED_FIXED_SIZE(cfgBinArgLbl);

  /*
   * Have we to display a warning???
   */
  cfgWarning = 0;
  if (kc->readNumEntry("enable", 1) == 0) {
    // We have to display a warning
    cfgWarning = new QFrame(cfgWidget);
    CHECK_PTR(cfgWarning);

    cfgWarningPm = new QLabel(cfgWarning);
    CHECK_PTR(cfgWarningPm);
    //QPixmap *xpm = new QPixmap((const char **)smiling_xpm);
    //cfgWarningPm->setPixmap(*xpm);
    cfgWarningPm->setPixmap(QMessageBox::standardIcon(QMessageBox::Warning, 
						      style()));
    SET_ADJUSTED_FIXED_SIZE(cfgWarningPm);

    cfgWarningLbl = new QLabel(_("This command had been disabled in "
				 "the configuration file."), cfgWarning);
    CHECK_PTR(cfgWarningLbl);
    SET_ADJUSTED_FIXED_SIZE(cfgWarningLbl);
    SET_ADJUSTED_FIXED_SIZE(cfgWarning);
    
    cfgWarningLayout = new QBoxLayout(cfgWarning, QBoxLayout::LeftToRight, 5);
    CHECK_PTR(cfgWarningLayout);
    cfgWarningLayout->addStretch(10);
    cfgWarningLayout->addWidget(cfgWarningPm, 0);
    //cfgWarningLayout->addSpacing(10);
    cfgWarningLayout->addWidget(cfgWarningLbl, 0);
    cfgWarningLayout->addStretch(10);
  }
  
  // Layouts
  if (makeLayouts) {
    cfgLayoutTB = new QBoxLayout(cfgWidget, QBoxLayout::TopToBottom, 10);
    CHECK_PTR(cfgLayoutTB);
    
    if (cfgWarning != 0) {
      cfgLayoutTB->addWidget(cfgWarning);
      cfgWarningLayout->activate();
    }
    cfgLayoutTB->addWidget(cfgBinGB);
    
    cfgLayoutGB = new QGridLayout(cfgBinGB, 3, 2, 10);
    CHECK_PTR(cfgLayoutGB);
    
    cfgLayoutGB->addRowSpacing(0, 0);
    cfgLayoutGB->addWidget(cfgBinNameLbl, 1, 0, AlignRight|AlignVCenter);
    cfgLayoutGB->addWidget(cfgBinNameLE, 1, 1);
    cfgLayoutGB->addWidget(cfgBinArgLbl, 2, 0, AlignRight|AlignVCenter);
    cfgLayoutGB->addWidget(cfgBinArgLE, 2, 1);
    cfgLayoutGB->setColStretch(0, 0);
    cfgLayoutGB->setColStretch(1, 10);
    cfgLayoutGB->activate();

    cfgLayoutTB->addStretch(10);

    cfgWidget->adjustSize();
    cfgLayoutTB->activate();
    cfgWidget->adjustSize();
    cfgWidget->setMinimumSize(cfgWidget->size());
  }
  // Now, we read the configfile
  readConfig();
  
  return cfgWidget;
#undef SET_ADJUSTED_FIXED_SIZE
}

/**
 * Pseudo Destructor
 */
void
CommandCfgDlg::deleteWidget()
{
  delete cfgLayoutGB;
  delete cfgLayoutTB;
  if (cfgWarning != 0) {
    delete cfgWarning;
    cfgWarning = 0;
  }
  delete cfgBinNameLbl;
  delete cfgBinArgLbl;
  delete cfgBinNameLE;
  delete cfgBinArgLE;
}

CommandCfgDlg::~CommandCfgDlg()
{
}

/**
 * commit changes to the configfile
 * 
 * @return if the change have been done
 */
bool
CommandCfgDlg::commitChanges()
{
  QString s;
  KConfig *kc = kapp->getConfig();

  //debug("CommandCfgDlg::commitChanges");
  kc->setGroup(configGroupName);
  
  kc->writeEntry("path", cfgBinNameLE->text());
  kc->writeEntry("arguments", cfgBinArgLE->text());

  return(TRUE);
}

/**
 * cancel changes to the configfile
 */
void
CommandCfgDlg::cancelChanges()
{
  //debug("CommandCfgDlg::cancelChanges");
  // nothing to do
}

/**
 * read the configfile
 */
void
CommandCfgDlg::readConfig()
{
  QString s;
  KConfig *kc = kapp->getConfig();

  //debug("CommandCfgDlg::readConfig");
  kc->setGroup(configGroupName);
  
  if (kc->hasKey("path")) {
    s = kc->readEntry("path");
    cfgBinNameLE->setText(s);
  }
  if (kc->hasKey("arguments")) {
    s = kc->readEntry("arguments");
    cfgBinArgLE->setText(s);
  }
}
