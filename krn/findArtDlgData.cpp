
#include "findArtDlgData.h"


#include <qlabel.h>
findArtDlgData::findArtDlgData
    (
     QWidget* parent
    )
{
    b1 = new QPushButton( parent, "PushButton_1" );
    b1->setGeometry( 10, 140, 100, 30 );
    b1->setText( "Find" );
    b1->setAutoDefault( TRUE );
    
    b2 = new QPushButton( parent, "PushButton_2" );
    b2->setGeometry( 200, 140, 100, 30 );
    b2->setText( "Done" );
    
    field = new QComboBox( FALSE, parent, "ComboBox_1" );
    field->setGeometry( 10, 100, 290, 30 );
    field->setSizeLimit( 10 );
    field->insertItem( "Subject" );
    field->insertItem( "Sender" );
    
    expr = new QLineEdit( parent, "LineEdit_1" );
    expr->setGeometry( 10, 40, 290, 30 );
    expr->setText( "" );
    
    QLabel* tmpQLabel;
    tmpQLabel = new QLabel( parent, "Label_2" );
    tmpQLabel->setGeometry( 100, 70, 100, 30 );
    tmpQLabel->setText( "In Field" );
    tmpQLabel->setAlignment( 292 );
    
    tmpQLabel = new QLabel( parent, "Label_3" );
    tmpQLabel->setGeometry( 10, 10, 290, 30 );
    tmpQLabel->setText( "Find Articles With:" );
    tmpQLabel->setAlignment( 292 );
    
    parent->resize( 310, 180 );
}
