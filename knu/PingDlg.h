/* -*- C++ -*-
 *
 *   PingDlg.h - Dialog for the ping command
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

#ifndef __PingDlg_h__
#define __PingDlg_h__

#include <qchkbox.h>
#include <qlayout.h>

#include "CommandDlg.h"

class PingDlg: public CommandDlg
{
  Q_OBJECT

public:
  PingDlg(QString commandName, 
	  QWidget* parent = NULL, const char* name = NULL);
  virtual ~PingDlg();
  

protected:
  void         buildCommandLine(QString);

  // widgets
  QCheckBox   *pingCb1;
  QLineEdit   *pingLe2;
  QLabel      *pingLbl2;
  QFrame      *frame1;

  // layout
  QBoxLayout  *layout3;
};
#endif // __PingDlg_h__

