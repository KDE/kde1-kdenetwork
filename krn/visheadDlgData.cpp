
#include "visheadDlgData.h"


visheadDlgData::visheadDlgData
(
	QWidget* parent
)
{
	entry = new QLineEdit( parent, "LineEdit_1" );
	entry->setGeometry( 10, 10, 120, 30 );
	entry->setText( "" );

	headerlist = new QListBox( parent, "ListBox_1" );
	headerlist->setGeometry( 220, 10, 160, 190 );
	headerlist->setFrameStyle( 51 );
	headerlist->setLineWidth( 2 );

	b3 = new QPushButton( parent, "PushButton_1" );
	b3->setGeometry( 140, 10, 70, 30 );
	b3->setText( "->" );

	b1 = new QPushButton( parent, "PushButton_2" );
	b1->setGeometry( 10, 210, 100, 30 );
	b1->setText( "OK" );

	b2 = new QPushButton( parent, "PushButton_3" );
	b2->setGeometry( 280, 210, 100, 30 );
	b2->setText( "Cancel" );

	b4 = new QPushButton( parent, "PushButton_4" );
	b4->setGeometry( 140, 70, 70, 30 );
	b4->setText( "<-" );

	all = new QCheckBox( parent, "CheckBox_1" );
	all->setGeometry( 10, 150, 200, 30 );
	all->setText( "Show All Headers" );

	parent->resize( 390, 250 );
}
