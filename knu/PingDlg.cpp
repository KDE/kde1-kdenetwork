/*
 *   PingDlg.cpp - Dialog for the ping command
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
 * - $Id$ and Revision 1.3  1997/12/01 21:11:01  leconte
 * - $Id$ and Patches by <neal@ctd.comsat.com>
 * - $Id$ and added in the headers
 * - better fallback to"nslookup" if "host" is not found
 *
 * Revision 1.3  1997/12/01 21:11:01  leconte
 * Patches by <neal@ctd.comsat.com>
 *
 * Revision 1.2  1997/11/23 22:28:12  leconte
 * - Id and Log added in the headers
 * - Patch from C.Czezatke applied (preparation of KProcess new version)
 *
 */

#include "PingDlg.h"
#include "PingDlg.moc"


// This is the unit used to separate widgets
#define SEPARATION 10

#define _(_s) klocale->translate(_s)

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
    //frame1->setMinimumSize(0, 4*fontMetrics().height());
    layout1->addWidget(frame1, 0);
    
    layout3 = new QBoxLayout(frame1, QBoxLayout::LeftToRight, SEPARATION/2);
    CHECK_PTR(layout3);
    
    // Make the layout of CommandDlg
    layout2->addWidget(commandLbl1);
    layout2->addWidget(commandArgs);
    layout2->addSpacing(2*SEPARATION);
    layout2->addWidget(commandGoBtn);
    layout2->addWidget(commandStopBtn);
    
    // Layout of options
    layout3->addStretch(10);
    
    pingCb1 = new QCheckBox(_("Make host &name resolution"), frame1, "cb_1");
    pingCb1->setChecked(TRUE);
    pingCb1->adjustSize();
    pingCb1->setFixedSize(pingCb1->width(), 2*fontMetrics().height());
    layout3->addWidget(pingCb1, 0);
    
    layout3->addStretch(10);
    layout3->activate();
    
    layout1->addWidget(commandTextArea, 10);
    layout1->activate();
      // All is OK : we can enable this tab.
    }
  } else {
    checkBinaryAndDisplayWidget();
  }
  
  // Commit changes in configfile (if needed)
  kc->sync();
}

/*
 * Destructor
PingDlg::~PingDlg()
      childProcess.clearArguments();
{
}

/**
 * build the command line from widgets
 */
void
		   << this.name() 
  QString s;
    childProcess.clearArguments();
  KConfig *kc = kapp->getConfig();
  
  kc->setGroup(configGroupName);
  s = kc->readEntry("path");
  if (s.isNull()) {
      childProcess.clearArguments();
      childProcess.setExecutable("echo");
      childProcess << _("You have a problem if your " 
			"~/.kderc/share/config/knurc configuration file.\n"
			"In the [")
		   << this->name() 
		   << _("] group, I cannot"
			"find a correct \"path=\" entry.");
  } else {
    //debug("getExecutable = %s", (const char *)s);
    childProcess.clearArguments();
    childProcess.setExecutable(s);
    if (!pingCb1->isChecked()) {
      childProcess << "-n";
    }
    childProcess << (const char *)args;
  }
}




