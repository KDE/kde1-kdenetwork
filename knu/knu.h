/* -*- C++ -*-
 *
 *   KDE network utilities (knu.h)
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

#ifndef _KNU_H
#define _KNU_H

#include <kapp.h>
#include <ktabctl.h>

#include <qpushbt.h>
#include <qpopmenu.h>
#include <qmenubar.h>
#include <qaccel.h>
#include <qframe.h>

#include <ktopwidget.h>

#include "PingDlg.h"
#include "TracerouteDlg.h"
#include "HostDlg.h"
#include "OptionsDlg.h"

/**
 * Main widget
 */
class TopLevel: public KTopLevelWidget
{
  Q_OBJECT

public:
  TopLevel(QWidget *parent=0, const char *name=0);
  ~TopLevel();
  void closeEvent(QCloseEvent *);
  //CommandDlg **getPages(int *pn) { *pn=pagesNumber; return pages; };
    
protected:
  virtual void saveProperties(KConfig*);
  virtual void readProperties(KConfig*);
  

  QPopupMenu    *fileMenu, *editMenu, *helpMenu;
  KMenuBar      *menuBar;
  KTabCtl       *tabCtrl;
  int            configIndex;
  int            closeIndex;
  CommandDlg    *pages[10];
  static CommandCfgDlg *cfgPages[10];
  int            pagesNumber;

public slots:
  void slotTabChanged(int);
  void slotQuit();  
  void slotClose();  
  void slotCopy();  
  void slotSelectAll();  
  void slotHelp();  
  void slotConfig();  
  void slotClear();  
  void slotAbout();
  void slotAboutQt();
  void slotNew();

private:
  KConfig    *kconfig;
  int         currentTab;

public:
  static QList<TopLevel>  windowList;

protected:
  void createMenu();

};

#endif

