/**********************************************************************

	--- Dlgedit generated file ---

	File: PartProps.cpp
	Last generated: Fri Jul 11 22:21:38 1997

 *********************************************************************/

#include "PartProps.h"

#define Inherited PartPropsData

PartProps::PartProps
(
	messagePart* p,
	QWidget* parent,
	const char* name
)
	:
	Inherited( parent, name )
{
	QString caption="Properties for "+p->name;
	setCaption(caption);
	encodings->insertItem("none");
}


PartProps::~PartProps()
{
}

void PartProps::cancel()
{
    delete this;
}

void PartProps::ok()
{
    delete this;
}
