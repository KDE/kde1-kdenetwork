/**********************************************************************

	--- Qt Architect generated file ---

	File: open_top.cpp
	Last generated: Sun Nov 23 17:55:22 1997

 *********************************************************************/

#include "open_top.h"

#define Inherited open_topData

open_top::open_top
(
	QWidget* parent,
	const char* name
)
	:
	Inherited( parent, name )
{
	setCaption( "New Window For" );
	nameSLE->setFocus();
}


open_top::~open_top()
{
}

void open_top::create_toplevel()
{
  if(strlen(nameSLE->text()) > 0)
    emit open_toplevel( (QString) nameSLE->text());

  close(TRUE);

}
