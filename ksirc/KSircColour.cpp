/**********************************************************************

	--- Qt Architect generated file ---

	File: KSircColour.cpp
	Last generated: Fri Dec 12 22:54:52 1997

 *********************************************************************/

#include "KSircColour.h"
#include "config.h"

#define Inherited KSircColourData

KSircColour::KSircColour
(
	QWidget* parent,
	const char* name
)
	:
	Inherited( parent, name )
{
	setCaption( "Colour Settings" );

	// Set the text colour for the 4 main buttons.
	// Get the pointer values and off you go.

	ColourText->setColor( *kSircConfig->colour_text );
	ColourInfo->setColor( *kSircConfig->colour_info );
	ColourChan->setColor( *kSircConfig->colour_chan );
	ColourError->setColor( *kSircConfig->colour_error );
}


KSircColour::~KSircColour()
{
}

void KSircColour::ok()
{

  // Write the values back again.  This will change the painter 
  // colours on the fly.
  
  *kSircConfig->colour_text = ColourText->color();
  *kSircConfig->colour_info = ColourInfo->color();
  *kSircConfig->colour_chan = ColourChan->color();
  *kSircConfig->colour_error = ColourError->color();
  close(1);
}

void KSircColour::cancel()
{
  close(1);
}
