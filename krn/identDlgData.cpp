
#include "identDlgData.h"


#include <qlabel.h>
identDlgData::identDlgData
(
	QWidget* parent
)
{
	QLabel* tmpQLabel;
	tmpQLabel = new QLabel( parent, "Label_1" );
	tmpQLabel->setGeometry( 10, 20, 120, 30 );
	tmpQLabel->setText( "Real Name" );

	realname = new QLineEdit( parent, "LineEdit_1" );
	realname->setGeometry( 140, 20, 280, 30 );
	realname->setText( "" );

	tmpQLabel = new QLabel( parent, "Label_2" );
	tmpQLabel->setGeometry( 10, 60, 120, 30 );
	tmpQLabel->setText( "E-mail Address" );

	address = new QLineEdit( parent, "LineEdit_2" );
	address->setGeometry( 140, 60, 280, 30 );
	address->setText( "" );

	tmpQLabel = new QLabel( parent, "Label_3" );
	tmpQLabel->setGeometry( 10, 100, 120, 30 );
	tmpQLabel->setText( "Organization" );

	organization = new QLineEdit( parent, "LineEdit_3" );
	organization->setGeometry( 140, 100, 280, 30 );
	organization->setText( "" );

	b1 = new QPushButton( parent, "PushButton_1" );
	b1->setGeometry( 60, 140, 100, 30 );
	b1->setText( "OK" );
	b1->setAutoDefault( TRUE );

	b2 = new QPushButton( parent, "PushButton_2" );
	b2->setGeometry( 270, 140, 100, 30 );
	b2->setText( "Cancel" );

	parent->resize( 430, 190 );
}
