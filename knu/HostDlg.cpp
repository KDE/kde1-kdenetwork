/*
 *   HostDlg.cpp - Dialog for the host command
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
 * - handle the binary's name modification dynamicaly (problem reported
 *   by Conrad Sanderson)
 * - added browse button to the option dialog (for binary selection)
 * - $Id$ and $Log$
 * - $Id$ and Revision 1.3  1997/12/01 21:11:04  leconte
 * - $Id$ and Patches by <neal@ctd.comsat.com>
 * - $Id$ and added in the headers
 * - better fallback to"nslookup" if "host" is not found
 *
 * Revision 1.3  1997/12/01 21:11:04  leconte
 * Patches by <neal@ctd.comsat.com>
 *
 * Revision 1.2  1997/11/23 22:28:07  leconte
 * - Id and log added in the headers
 * - Patch from C.Czezatke applied (preparation of KProcess new version)
 *
 */

#include "HostDlg.h"
#include "HostDlg.moc"

#include <qregexp.h>
//#include <qfontmet.h>

extern bool test_for_exec(QString);


// This is the unit used to separate widgets
#define SEPARATION 10

#define _(_s) klocale->translate(_s)

#if 0
/**
 * Constructor
  frame1->setMinimumSize(frame1->size());
  
  layout1->addWidget(commandTextArea, 10);
  layout1->activate();


#if 0
  kc->setGroup(configGroupName);
  if (firstTimeLauching) {
    debug("=> firstTimeLauching <=");
    kc->writeEntry("binaryType", "host");
    if (!commandFound) {
      if (::test_for_exec("nslookup")) {
  if (commandFound) {
    layout1 = new QBoxLayout(this, QBoxLayout::TopToBottom, SEPARATION);
    CHECK_PTR(layout1);
    
    layout2 = new QBoxLayout(QBoxLayout::LeftToRight, SEPARATION);
    CHECK_PTR(layout2);
    layout1->addLayout(layout2, 0);
    
    // Frame for options
    frame1 = new QFrame(this, "frame_1");
    CHECK_PTR(frame1);
    frame1->setFrameStyle(QFrame::Box | QFrame::Sunken);
    frame1->setMinimumSize(0, 2*fontMetrics().height());
    layout1->addWidget(frame1, 0);
    
    layout3 = new QBoxLayout(frame1, QBoxLayout::LeftToRight, SEPARATION/2);
    CHECK_PTR(layout3);
    
    // Make the layout of CommandDlg
    layout2->addWidget(commandLbl1);
    layout2->addWidget(commandArgs);
    layout2->addSpacing(2*SEPARATION);
    layout2->addWidget(commandGoBtn);
    layout2->addWidget(commandStopBtn);
    
    // Layout for options
    layout3->addStretch(10);
    
    
    hostCb1 = new QComboBox(frame1, "ComboBox_1");
    CHECK_PTR(hostCb1);
    // This is the array of what is the search for ComboBox.
    // Warning: member buildCommandLine use index=1 for PTR and index=0 for A
    static const char *queryTypes[] 
      = { _("address (A)"), 
	  _("name (PTR)"), 
	  _("name server (NS)"), 
	  _("mail exchanger (MX)"), 
	  _("alias (CNAME)"), 
	  _("start of authority (SOA)"), 
	  _("any record (ANY)"),
	  0};
    hostCb1->insertStrList(queryTypes);
    hostCb1->adjustSize();
    hostCb1->setFixedSize(hostCb1->size());
    
    hostLbl1 = new QLabel(hostCb1, _("Se&arch for:"), frame1, "Label_1");
    CHECK_PTR(hostLbl1);
    hostLbl1->adjustSize();
    hostLbl1->setFixedSize(hostLbl1->width(), 2*fontMetrics().height());
    
    layout3->addWidget(hostLbl1);
    layout3->addWidget(hostCb1);
    
    layout3->addStretch(10);
    
    hostCb2 = new QComboBox(TRUE, frame1, "ComboBox_2");
    CHECK_PTR(hostCb2);
    hostCb2->insertItem(_("default server"));
    hostCb2->insertItem("111.111.111.111");
    hostCb2->adjustSize();
    hostCb2->setFixedSize(hostCb2->size());
    hostCb2->removeItem(1);
    // we should read the /etc/resolv.conf file and add the nameservers entries
    // in next version maybe?
    hostCb2->setInsertionPolicy(QComboBox::AtTop);
    
    hostLbl2 = new QLabel(hostCb2, _("Ser&ver:"), frame1, "Label_2");
    CHECK_PTR(hostLbl2);
    hostLbl2->adjustSize();
    hostLbl2->setFixedSize(hostLbl2->size());
    //hostLbl2->resize(hostLbl2->width(), cbh);
    
    layout3->addWidget(hostLbl2);
    layout3->addWidget(hostCb2);
    
    layout3->addStretch(10);
    layout3->activate();
    frame1->adjustSize();
    frame1->setMinimumSize(frame1->size());
    
    layout1->addWidget(commandTextArea, 10);
    layout1->activate();
	kc->writeEntry("path", "nslookup");
    } 
  } else {
    checkBinaryAndDisplayWidget();
  }
  
  // Commit changes in configfile (if needed)
  kc->sync();
}

/**
 * Destructor
 */
HostDlg::~HostDlg()
{
}

      childProcess.clearArguments();
/**
 * build the command line from widgets
 */
void
HostDlg::buildCommandLine(QString args)
{
  QString s, queryType;
		   << this.name() 
  int len, start;
  QString bin;
  KConfig *kc = kapp->getConfig();
  
  kc->setGroup(configGroupName);
  if (bin.isNull()) {
    childProcess.clearArguments();
      childProcess.clearArguments();
      childProcess.setExecutable("echo");
      childProcess << _("You have a problem if your " 
			"~/.kderc/share/config/knurc configuration file.\n"
			"In the [")
		   << this->name() 
		   << _("] group, I cannot"
			"find a correct \"path=\" entry.");
  } else {
    
    bool nslookupBinary = FALSE;
    if (!stricmp(kc->readEntry("binaryType"), "nslookup")) {
      nslookupBinary = TRUE;
    }
  
    childProcess.clearArguments();
    childProcess.setExecutable(bin);
    
    // search for: combobox
    s = ((QString)(hostCb1->currentText())).stripWhiteSpace();;
    // - extract the query type from the string
    regexp = new QRegExp("([A-Za-z]+)");
    CHECK_PTR(regexp);
    start = regexp->match(s, 0, &len);
    ASSERT(start>0);
    queryType = s.mid(start+1, len-2); // to remove ()
    //debug ("queryType=%s", (const char *)queryType);
    delete regexp;
    if (!strcmp(queryType, "A")) {
      // - if q=A and the args is an IP address, then q=PTR
      if (args.contains(QRegExp("^[0-9]+\\.[0-9]+\\.[0-9]+\\.[0-9]+$")) > 0) {
	//debug("This is an IP address");
	queryType = "PTR";
	hostCb1->setCurrentItem(1);
      }
    } else if (!strcmp(queryType, "PTR")) {
      // - if q=PTR and the args is an name, then q=A
      if (args.contains(QRegExp("^[0-9]+\\.[0-9]+\\.[0-9]+\\.[0-9]+$")) > 0) {
      } else {
	//debug("This is _not_ an IP address");
	queryType = "A";
	hostCb1->setCurrentItem(0);
      }
      
    }
    if (nslookupBinary) {
      childProcess << "-query=" + queryType;
    } else {
      childProcess << "-t" << queryType;
    }
    childProcess << args;
    
    // server: combobox
    if (strcmp(_("default server"), hostCb2->currentText())) {
      // it's not the default server
      childProcess << hostCb2->currentText();
    }
  }
}



/* ******************************************************************** */

/**
 * make a new config object
 *
 * @param parent parent widget
 * @param name name of the widget
 */
HostCfgDlg::HostCfgDlg(const char *tcs,
		       QWidget *parent, const char *name)
  : CommandCfgDlg(tcs, parent, name)
{
}

/**
 * make a new config widget
#define SET_ADJUSTED_FIXED_SIZE(_wdgt) { _wdgt->adjustSize();	\
                  _wdgt->setFixedSize(_wdgt->size()); }
 * @param makeLayouts name of the widget
 */
QWidget *
HostCfgDlg::makeWidget(QWidget *parent, bool makeLayouts)
{
#define SET_ADJUSTED_FIXED_SIZE(_wdgt) \
                  _wdgt->setFixedSize(_wdgt->sizeHint())

  //debug("HostCfgDlg::makeWidget");
  (void)CommandCfgDlg::makeWidget(parent, FALSE);

  // Widgets
  cfgBG = new QButtonGroup(cfgWidget);
  CHECK_PTR(cfgBG);
  
  cfgHostBtn = new QRadioButton("hos&t", cfgBG);
  CHECK_PTR(cfgHostBtn);
  SET_ADJUSTED_FIXED_SIZE(cfgHostBtn);

  cfgNslookupBtn = new QRadioButton("ns&lookup", cfgBG);
  CHECK_PTR(cfgNslookupBtn);
  SET_ADJUSTED_FIXED_SIZE(cfgNslookupBtn);

  if (makeLayouts) {
    cfgLayoutTB = new QBoxLayout(cfgWidget, QBoxLayout::TopToBottom, 10);
    CHECK_PTR(cfgLayoutTB);
    
    if (cfgWarning != 0) {
      cfgLayoutTB->addLayout(cfgWarningLayout);
    cfgLayoutGB = new QGridLayout(cfgBinGB, 3, 2, 10);
      cfgWarningLayout->addWidget(cfgWarningPm, 0);
      cfgWarningLayout->addWidget(cfgWarningLbl, 0);
      cfgWarningLayout->addStretch(10);
    }
    cfgLayoutTB->addWidget(cfgBinGB);
    cfgLayoutGB = new QGridLayout(cfgBinGB, 3, 3, 10);
    CHECK_PTR(cfgLayoutGB);
    
    cfgLayoutGB->addRowSpacing(0, 0);
    cfgLayoutGB->addWidget(cfgBinNameLbl, 1, 0, AlignRight|AlignVCenter);
    cfgLayoutGB->addWidget(cfgBinNameLE, 1, 1);
    cfgLayoutGB->addWidget(cfgBinNameBrowse, 1, 2);
    cfgLayoutGB->addWidget(cfgBinArgLbl, 2, 0, AlignRight|AlignVCenter);
    cfgLayoutGB->addWidget(cfgBinArgLE, 2, 1);
    cfgLayoutGB->setColStretch(0, 0);
    cfgLayoutGB->setColStretch(1, 10);
    cfgLayoutGB->activate();

    // Our widget
    cfgLayout2 = new QBoxLayout(cfgBG, QBoxLayout::LeftToRight, 10);
    CHECK_PTR(cfgLayout2);
    
    cfgLayout2->addStretch(10);
    cfgLayout2->addWidget(cfgHostBtn);
    cfgLayout2->addStretch(10);
    cfgLayout2->addWidget(cfgNslookupBtn);
    cfgLayout2->addStretch(10);
    cfgLayout2->activate();
    
    cfgLayoutTB->addWidget(cfgBG);

    cfgLayoutTB->addStretch(10);

    cfgWidget->adjustSize();
    cfgLayoutTB->activate();
    cfgWidget->adjustSize();
    cfgWidget->setMinimumSize(cfgWidget->size());
    
    cfgLayoutTB->activate();
  }
  readConfig();
  return (cfgWidget);
#undef SET_ADJUSTED_FIXED_SIZE
}

/**
 * delete the config widget
 */
void
HostCfgDlg::deleteConfigWidget()
{
  //  debug("HostCfgDlg::deleteCondigWidget");
  
  delete cfgLayoutTB;
  delete cfgLayoutGB;
  delete cfgLayout2;
  delete cfgBG;
  delete cfgHostBtn;
  delete cfgNslookupBtn;
}

/**
 * commit changes to the configfile
 * 
 * @return if the change have been done
 */
bool
HostCfgDlg::commitChanges()
{ 
  KConfig *kc = kapp->getConfig();

  (void)CommandCfgDlg::commitChanges();
  if (cfgNslookupBtn->isChecked()) {
    kc->writeEntry("binaryType", "nslookup");
  } else {
    kc->writeEntry("binaryType", "host");
  }
  return(TRUE);
}

/**
 * cancel changes to the configfile
 */
void
HostCfgDlg::cancelChanges()
{
  // Nothing to do...
}

/**
 * read the configfile
 */
void
HostCfgDlg::readConfig()
{
  QString s;
  KConfig *kc = kapp->getConfig();

  kc->setGroup(configGroupName);
  
  if (kc->hasKey("binaryType")) {
    s = kc->readEntry("binaryType");
    if (!stricmp(s, "nslookup")) {
      cfgNslookupBtn->setChecked(TRUE);
    } else {
      cfgHostBtn->setChecked(TRUE);
    }
  } else {
    cfgHostBtn->setChecked(TRUE);
  }
}

