
#include "NNTPConfigDlgData.h"


#include <qlabel.h>
NNTPConfigDlgData::NNTPConfigDlgData
(
	QWidget* parent
)
{
	b1 = new QPushButton( parent, "PushButton_1" );
	b1->setGeometry( 60, 200, 100, 30 );
	b1->setText( "OK" );
	b1->setAutoDefault( TRUE );

	b2 = new QPushButton( parent, "PushButton_2" );
	b2->setGeometry( 250, 200, 100, 30 );
	b2->setText( "Cancel" );

	servername = new QLineEdit( parent, "LineEdit_1" );
	servername->setGeometry( 70, 20, 350, 30 );
	servername->setText( "" );

	QLabel* tmpQLabel;
	tmpQLabel = new QLabel( parent, "Label_1" );
	tmpQLabel->setGeometry( 10, 20, 60, 30 );
	tmpQLabel->setText( "Server" );

	connectatstart = new QCheckBox( parent, "CheckBox_1" );
	connectatstart->setGeometry( 270, 60, 150, 30 );
	connectatstart->setText( "Connect on Startup" );

	authenticate = new QCheckBox( parent, "CheckBox_2" );
	authenticate->setGeometry( 10, 60, 170, 30 );
	authenticate->setText( "Authenticate" );

	tmpQLabel = new QLabel( parent, "Label_2" );
	tmpQLabel->setGeometry( 120, 110, 70, 30 );
	tmpQLabel->setText( "User Name" );

	tmpQLabel = new QLabel( parent, "Label_3" );
	tmpQLabel->setGeometry( 120, 150, 60, 30 );
	tmpQLabel->setText( "Password" );

	username = new QLineEdit( parent, "LineEdit_2" );
	username->setGeometry( 200, 110, 150, 30 );
	username->setText( "" );

	password = new QLineEdit( parent, "LineEdit_3" );
	password->setGeometry( 200, 150, 150, 30 );
	password->setText( "" );

	parent->resize( 430, 250 );
}
