/**********************************************************************

	--- Qt Architect generated file ---

	File: open_ksirc.cpp
	Last generated: Wed Nov 26 22:18:02 1997

 *********************************************************************/

#include "open_ksirc.h"
#include "ksircprocess.h"

#define Inherited open_ksircData

open_ksirc::open_ksirc
(
	QWidget* parent,
	const char* name
)
	:
	Inherited( parent, name )
{
	setCaption( "Connect to Server" );
	nameSLE->setFocus();
}


open_ksirc::~open_ksirc()
{
}

void open_ksirc::create_toplevel()
{
  if(strlen(nameSLE->text()) > 0)
    emit open_ksircprocess((QString)nameSLE->text());

  close(TRUE);

}
