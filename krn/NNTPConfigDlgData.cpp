
#include "NNTPConfigDlgData.h"


#include <qlabel.h>
NNTPConfigDlgData::NNTPConfigDlgData
(
	QWidget* parent
)
{
	b1 = new QPushButton( parent, "PushButton_1" );
	b1->setGeometry( 30, 230, 100, 30 );
	b1->setText( "OK" );
	b1->setAutoDefault( TRUE );

	b2 = new QPushButton( parent, "PushButton_2" );
	b2->setGeometry( 300, 230, 100, 30 );
	b2->setText( "Cancel" );

	servername = new QLineEdit( parent, "LineEdit_1" );
	servername->setGeometry( 120, 20, 300, 30 );
	servername->setText( "" );

	QLabel* tmpQLabel;
	tmpQLabel = new QLabel( parent, "Label_1" );
	tmpQLabel->setGeometry( 10, 20, 100, 30 );
	tmpQLabel->setText( "NNTP Server" );

	connectatstart = new QCheckBox( parent, "CheckBox_1" );
	connectatstart->setGeometry( 270, 50, 150, 30 );
	connectatstart->setText( "Connect on Startup" );

	authenticate = new QCheckBox( parent, "CheckBox_2" );
	authenticate->setGeometry( 10, 50, 170, 30 );
	authenticate->setText( "Authenticate" );

	tmpQLabel = new QLabel( parent, "Label_2" );
	tmpQLabel->setGeometry( 120, 90, 70, 30 );
	tmpQLabel->setText( "User Name" );

	tmpQLabel = new QLabel( parent, "Label_3" );
	tmpQLabel->setGeometry( 120, 130, 60, 30 );
	tmpQLabel->setText( "Password" );

	username = new QLineEdit( parent, "LineEdit_2" );
	username->setGeometry( 200, 90, 150, 30 );
	username->setText( "" );

	password = new QLineEdit( parent, "LineEdit_3" );
	password->setGeometry( 200, 130, 150, 30 );
	password->setText( "" );

	tmpQLabel = new QLabel( parent, "Label_4" );
	tmpQLabel->setGeometry( 10, 180, 100, 30 );
	tmpQLabel->setText( "SMTP server" );

	smtpserver = new QLineEdit( parent, "LineEdit_4" );
	smtpserver->setGeometry( 120, 180, 300, 30 );
	smtpserver->setText( "" );

	parent->resize( 430, 270 );
}
