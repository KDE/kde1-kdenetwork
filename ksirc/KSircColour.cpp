/**********************************************************************

 Colour Setter for the text colours in TopLevel list box.

 GUI done with qtarch.

 Nothing special, just pops up a box with buttons, you click on them
 and up comes a colours selector.  Pretty basic.

 Only strange things is the ammount of dorking with pointers, you have
 to set the value os pointed to item, not change the pointer, hence
 the *ptr notation everywhere.

 *********************************************************************/

#include "KSircColour.h"
#include "config.h"

#include <kconfig.h>

#define Inherited KSircColourData

extern KConfig *kConfig;

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
  kConfig->setGroup("Colours");
  kConfig->writeEntry("text", *kSircConfig->colour_text);
  kConfig->writeEntry("info", *kSircConfig->colour_info);
  kConfig->writeEntry("chan", *kSircConfig->colour_chan);
  kConfig->writeEntry("error", *kSircConfig->colour_error);
  kConfig->sync();

}

void KSircColour::cancel()
{
  close(1);
}
