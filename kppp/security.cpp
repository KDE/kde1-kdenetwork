/******************************************************************************
 *
 *            kPPP: A pppd front end for the KDE project
 *
 ******************************************************************************
 *
 * $Id$
 *
 * $Revision$
 *
 ******************************************************************************
 * 
 * This widget contains the security related settings of kppp
 *
 ******************************************************************************
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
 *
 ******************************************************************************
 */

#include <qdir.h>
#include <kapp.h>
#include <qlayout.h>
#include <kintegerline.h>
#include <kquickhelp.h>
#include "macros.h"
#include "log.h"
#include "groupbox.h"
#include "newwidget.h"
#include "security.moc"

SecurityWidget::SecurityWidget(QWidget *parent, const char *name) : 
  KCheckGroupBox(i18n("Advanced Security"), parent, name)
{
  QVBoxLayout *tl = new QVBoxLayout(peer(), 0, 0);
  perUser = new KCheckGroupBox(i18n("Allowed users"), peer());
  perGroup = new KCheckGroupBox(i18n("Allowed groups"), peer());
  tl->addWidget(perUser);
  tl->addWidget(perGroup);

  QHBoxLayout *l1 = new QHBoxLayout(perUser->peer(), 0, 0);
  QHBoxLayout *l2 = new QHBoxLayout(perGroup->peer(), 0, 0);
  KTabListBox *allowedUsers = new KTabListBox(perUser->peer(), "", 2);
  KTabListBox *allowedGroups = new KTabListBox(perGroup->peer(), "", 2);
  l1->addWidget(allowedUsers);
  l2->addWidget(allowedGroups);

  tl->activate();
  l1->activate();
  l2->activate();

  allowedUsers->setNumCols(2);
  allowedUsers->setColumnWidth(16);
}
