/*
 *            kPPP: A pppd front end for the KDE project
 *
 * $Id$
 * 
 *            Copyright (C) 1997 Bernd Johannes Wuebben 
 *                   wuebben@math.cornell.edu
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

#include "kintedit.h"


void KIntLineEdit:: keyPressEvent( QKeyEvent *e ){
  char key = e->ascii();
  
  if( isdigit( key ) 
      || ( e->key() == Key_Return) || ( e->key() == Key_Enter    )
      || ( e->key() == Key_Delete) || ( e->key() == Key_Backspace)
      || ( e->key() == Key_Left  ) || ( e->key() == Key_Right    )){
    
    QLineEdit::keyPressEvent( e );
    return;
  } else {
    e->ignore();
    return;
  }
};
