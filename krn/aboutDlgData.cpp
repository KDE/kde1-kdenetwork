
#include "aboutDlgData.h"


#include <qframe.h>
#include <qlabel.h>
#include <qlistbox.h>
#include <qpixmap.h>

extern QString pixpath;

aboutDlgData::aboutDlgData
    (
     QWidget* parent
    )
{
    b1 = new QPushButton( parent, "PushButton_1" );
    b1->setGeometry( 10, 260, 380, 30 );
    b1->setText( "OK" );
    b1->setAutoDefault( TRUE );
    
    QLabel* tmpQLabel;
    tmpQLabel = new QLabel( parent, "Label_1" );
    tmpQLabel->setGeometry( 10, 120, 380, 30 );
    tmpQLabel->setText( "Part of the KDE project." );
    tmpQLabel->setBackgroundColor(QColor("white"));
    tmpQLabel->setAlignment( 36 );
    
    QListBox* tmpQListBox;
    tmpQListBox = new QListBox( parent, "ListBox_1" );
    tmpQListBox->setGeometry( 10, 190, 380, 60 );
    {
        QColorGroup normal( QColor( QRgb(16711680) ), QColor( QRgb(16777215) ), QColor( QRgb(16777215) ), QColor( QRgb(6316128) ), QColor( QRgb(10789024) ), QColor( QRgb(255) ), QColor( QRgb(16777215) ) );
        QColorGroup disabled( QColor( QRgb(8421504) ), QColor( QRgb(12632256) ), QColor( QRgb(16777215) ), QColor( QRgb(6316128) ), QColor( QRgb(10789024) ), QColor( QRgb(8421504) ), QColor( QRgb(12632256) ) );
        QColorGroup active( QColor( QRgb(0) ), QColor( QRgb(12632256) ), QColor( QRgb(16777215) ), QColor( QRgb(6316128) ), QColor( QRgb(10789024) ), QColor( QRgb(0) ), QColor( QRgb(16777215) ) );
        QPalette palette( normal, disabled, active );
        tmpQListBox->setPalette( palette );
    }
    tmpQListBox->setFrameStyle( 51 );
    tmpQListBox->setLineWidth( 2 );
    tmpQListBox->insertItem( "Roberto Alsina <ralsina@unl.edu.ar>" );
    tmpQListBox->insertItem( "Magnus Reftel <d96reftl@dtek.chalmers.se>" );
    tmpQListBox->insertItem( "Sander Alberink <sander.alberink@bigfoot.com>" );
    tmpQListBox->insertItem( "" );
    tmpQListBox->insertItem( "And Many thanks to:" );
    tmpQListBox->insertItem( "" );
    tmpQListBox->insertItem( "Doug Sauder <dwsauder@fwb.gulf.net>" );
    tmpQListBox->insertItem( "Stephan Kulow <coolo@kde.org>" );
    tmpQListBox->insertItem( "Bernd Wuebben <wuebben@math.cornell.edu>" );
    tmpQListBox->insertItem( "All KDE developers" );
    tmpQListBox->insertItem( "Troll Tech" );
    tmpQListBox->insertItem( "All testers" );
    tmpQListBox->setScrollBar( TRUE );
    
    tmpQLabel = new QLabel( parent, "Label_3" );
    tmpQLabel->setGeometry( 10, 160, 380, 30 );
    tmpQLabel->setText( "Credits & Thanks" );
    tmpQLabel->setAlignment( 292 );
    tmpQLabel->setBackgroundColor(QColor("white"));
    
    QFrame* tmpQFrame;
    tmpQFrame = new QFrame( parent, "Frame_1" );
    tmpQFrame->setGeometry( 87, 5, 225, 113 );
    tmpQFrame->setBackgroundPixmap(QPixmap(pixpath+"logo.xpm"));
    
    
    parent->resize( 400, 300 );
}
