
#include "fontsDlgData.h"


#include <qlabel.h>
fontsDlgData::fontsDlgData
(
	QWidget* parent
)
{
	b1 = new QPushButton( parent, "PushButton_1" );
	b1->setGeometry( 10, 320, 100, 30 );
	b1->setText( "OK" );

	b2 = new QPushButton( parent, "PushButton_2" );
	b2->setGeometry( 330, 320, 100, 30 );
	b2->setText( "Cancel" );

	fontSize = new QComboBox( FALSE, parent, "ComboBox_1" );
	fontSize->setGeometry( 120, 20, 100, 30 );
	fontSize->setSizeLimit( 10 );
	fontSize->insertItem( "Small" );
	fontSize->insertItem( "Normal" );
	fontSize->insertItem( "Large" );
	fontSize->insertItem( "Huge" );

	QLabel* tmpQLabel;
	tmpQLabel = new QLabel( parent, "Label_1" );
	tmpQLabel->setGeometry( 10, 20, 100, 30 );
	tmpQLabel->setText( "Font Size:" );

	samp1 = new QLineEdit( parent, "LineEdit_1" );
	samp1->setGeometry( 330, 80, 100, 30 );
	samp1->setText( "test" );

	samp2 = new QLineEdit( parent, "LineEdit_2" );
	samp2->setGeometry( 330, 120, 100, 30 );
	samp2->setText( "test" );

	stdFontName = new QComboBox( FALSE, parent, "ComboBox_2" );
	stdFontName->setGeometry( 120, 80, 200, 30 );
	stdFontName->setSizeLimit( 10 );

	fixedFontName = new QComboBox( FALSE, parent, "ComboBox_3" );
	fixedFontName->setGeometry( 120, 120, 200, 30 );
	fixedFontName->setSizeLimit( 10 );

	tmpQLabel = new QLabel( parent, "Label_2" );
	tmpQLabel->setGeometry( 10, 80, 100, 30 );
	tmpQLabel->setText( "Standard Font" );

	tmpQLabel = new QLabel( parent, "Label_3" );
	tmpQLabel->setGeometry( 10, 120, 100, 30 );
	tmpQLabel->setText( "Fixed Font" );

	bgColor = new KColorButton( parent, "PushButton_3" );
	bgColor->setGeometry( 220, 180, 90, 30 );
	bgColor->setText( "" );

	fgColor = new KColorButton( parent, "PushButton_4" );
	fgColor->setGeometry( 220, 210, 90, 30 );
	fgColor->setText( "" );

	tmpQLabel = new QLabel( parent, "Label_4" );
	tmpQLabel->setGeometry( 110, 180, 110, 30 );
	tmpQLabel->setText( "Background Color" );

	tmpQLabel = new QLabel( parent, "Label_5" );
	tmpQLabel->setGeometry( 110, 210, 110, 30 );
	tmpQLabel->setText( "Foreground Color" );

	tmpQLabel = new QLabel( parent, "Label_6" );
	tmpQLabel->setGeometry( 110, 240, 110, 30 );
	tmpQLabel->setText( "Link Color" );

	tmpQLabel = new QLabel( parent, "Label_7" );
	tmpQLabel->setGeometry( 110, 270, 110, 30 );
	tmpQLabel->setText( "Followed Color" );

	linkColor = new KColorButton( parent, "PushButton_5" );
	linkColor->setGeometry( 220, 240, 90, 30 );
	linkColor->setText( "" );

	followColor = new KColorButton( parent, "PushButton_6" );
	followColor->setGeometry( 220, 270, 90, 30 );
	followColor->setText( "" );

	parent->resize( 440, 360 );
}
