/***************************************************************************
 *                              KAStatusBar.cpp                            *
 *                            -------------------                          *
 *                         Source file for KArchie                         *
 *                  -A programm to display archie queries                  *
 *                                                                         *
 *                KArchie is written for the KDE-Project                   *
 *                         http://www.kde.org                              *
 *                                                                         *
 *   Copyright (C) Oct 1997 Jörg Habenicht                                 *
 *                  E-Mail: j.habenicht@europemail.com                     *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   * 
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          * 
 *                                                                         *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the Free Software           *
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.             *
 *                                                                         *
 ***************************************************************************/

//#include "KArchie.h"
#include "KAStatusBar.moc"
#include "KArchie.h"
#include "KASettingsTypes.h"
#include <qstring.h>

/* static members */
const int KAStatusBar::statusId = 0;
const int KAStatusBar::hostId = 1;
const int KAStatusBar::hitsId = 2;
const int KAStatusBar::progressId = 3;
const int KAStatusBar::triesId = 4;
const int KAStatusBar::ledId = 5;

const char KAStatusBar::statusTxt[] = "Status: ";
const char KAStatusBar::hostTxt[] = "Host: ";
const char KAStatusBar::hitsTxt[] = " hits";
const char KAStatusBar::progressTxt[] = "Progress: ";
const char KAStatusBar::triesTxt[] = ". try";


KAStatusBar::KAStatusBar(QWidget *parent, char *name)
  :KStatusBar( parent, name )
{
  setInsertOrder( RightToLeft );

  //  insertItem( i18n("Progress:         "), progressId );
  insertItem( i18n("    0 hits"), hitsId );
  insertItem( i18n("    0. try"), triesId );
  insertItem( i18n("Host: none                               "), hostId );
  insertItem( i18n("Status: ready                        "), statusId );

  // set alignment of hits to rightalign
  setAlignment( hitsId, 2 );
  setAlignment( triesId, 2 );

}

KAStatusBar::~KAStatusBar()
{
}

void 
KAStatusBar::startLED()
{
  ledTimerID = startTimer( 1000 );
}

void 
KAStatusBar::stopLED()
{
  killTimer( ledTimerID );
}

void 
KAStatusBar::slotChangeStatus( const char *status  )
{
  QString tmp( statusTxt );
  tmp += status;
  changeItem( tmp, statusId );
  //  repaint();
  updateRects();
}

void 
KAStatusBar::slotChangeHost( const char *host )
{
  QString tmp( hostTxt );
  tmp += host;
  changeItem( tmp, hostId );
  //  repaint();
  updateRects();
}

void 
KAStatusBar::slotChangeHits( const int hits )
{
  QString tmp;
  tmp.setNum( hits );
  tmp += hitsTxt;
  changeItem( tmp, hitsId );
  //  repaint();
  updateRects();
}

void 
KAStatusBar::slotChangeProgress( const int progress )
{
  QString tmp;
  tmp.setNum( progress );
  tmp = progress + tmp;
  changeItem( tmp , progressId );
  //  repaint();
  updateRects();
}

void 
KAStatusBar::slotChangeTries( int tries )
{
  QString tmp;
  tmp.setNum( tries );
  tmp += triesTxt;
  changeItem( tmp, triesId );
  //  repaint();  
  updateRects();
}

void 
KAStatusBar::slotConfigChanged()
{
  debug("KAStatusBar::slotConfigChanged");
  KConfig *config = KApplication::getKApplication()->getConfig();
  config->setGroup( ConfigEntries::HostConfigGroup );
  slotChangeHost( config->readEntry( ConfigEntries::CurrentHostEntry,
				     ConfigEntries::CurrentHostDefault ));
}

void 
KAStatusBar::timerEvent( QTimerEvent *te )
{
  if (te->timerId() == ledTimerID) {
    
    repaint();
  }
}
