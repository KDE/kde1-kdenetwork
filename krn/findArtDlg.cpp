#include "findArtDlg.h"

#define Inherited QWidget

#include "findArtDlg.moc"

findArtDlg::findArtDlg
    (
     QWidget* parent,
     const char* name
    )
    :
    Inherited( parent, name ),
    findArtDlgData( this )
{
    setCaption( "KRN - Article Searcher" );
    expr->setFocus();
    QObject::connect(b1,SIGNAL(clicked()),this,SLOT(find()));
    QObject::connect(b2,SIGNAL(clicked()),this,SLOT(close()));
    QObject::connect(expr,SIGNAL(returnPressed()),this,SLOT(find()));
}


findArtDlg::~findArtDlg()
{
}

void findArtDlg::close()
{
    hide();
}

void findArtDlg::find()
{
    emit FindThis (expr->text(),field->currentText());
}
