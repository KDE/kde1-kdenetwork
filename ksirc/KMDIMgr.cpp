/***************************************************************************
                          KMDIMgr.cpp  -  description                              
                             -------------------                                         

    version              :                                   
    begin                : Mon Oct 19 18:42:27 CDT 1998
                                           
    copyright            : (C) 1998 by Timothy Whitfield and Glover George                         
    email                : timothy@ametro.net and dime@ametro.net                                     
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   * 
 *                                                                         *
 ***************************************************************************/

#include "KMDIMgr.h"

KMDIMgr::KMDIMgr (QWidget* p, const char *name, 
		KToolBar *toolbar ) :
		KMDIMgrBase (p,name)
{
    top_toolbar=toolbar;
    toolMin=toolMax=toolClose=0;
}

KMDIMgr::~KMDIMgr()
{
}

KMDIWindow* KMDIMgr::addWindow (QWidget *p,   int flag , const char* icon)
{
    KMDIWindow *result=KMDIMgrBase::addWindow (p,flag,icon);

    connect(result,SIGNAL(maximized(KMDIWindow *)),this,
        SLOT(slotWindowMaximized(KMDIWindow *)));
    connect(result,SIGNAL(restored(KMDIWindow *)),this,
        SLOT(slotWindowRestored(KMDIWindow *)));

    return result;    
}

void KMDIMgr::slotWindowMaximized(KMDIWindow *w)
{
    w->getTitlebar()->setEnabled(false);
    w->getFrame()->setFrameStyle( QFrame::Panel | QFrame::Plain );
    w->getFrame()->setLineWidth( 0 );

    toolMin=top_toolbar->insertButton(
        *w->getTitlebar()->getMinbtn()->pixmap(),
        2,SIGNAL(clicked()),w,SLOT(slotMinimize()));
    toolMax=top_toolbar->insertButton(
        *w->getTitlebar()->getMaxbtn()->pixmap(),
        3,SIGNAL(clicked()),w,SLOT(slotMaximize()));
    toolClose=top_toolbar->insertButton(
        *w->getTitlebar()->getClosebtn()->pixmap(),
        4,SIGNAL(clicked()),w,SLOT(slotClose()));
}

void KMDIMgr::slotWindowRestored(KMDIWindow *w)
{
    w->getTitlebar()->setEnabled(true);
    w->getFrame()->setFrameStyle( QFrame::Panel | QFrame::Raised );
    w->getFrame()->setLineWidth( 2 );

    top_toolbar->removeItem(1);
    top_toolbar->removeItem(2);
    top_toolbar->removeItem(3);
}
