
#include "fontsDlgData.h"


#include <qlabel.h>
fontsDlgData::fontsDlgData
(
	QWidget* parent
)
{
	b1 = new QPushButton( parent, "PushButton_1" );
	b1->setGeometry( 10, 170, 100, 30 );
	b1->setText( "OK" );

	b2 = new QPushButton( parent, "PushButton_2" );
	b2->setGeometry( 330, 170, 100, 30 );
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

	parent->resize( 440, 210 );
}
