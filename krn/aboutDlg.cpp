#include "aboutDlg.h"

#define Inherited QDialog

aboutDlg::aboutDlg
    (
     QWidget* parent,
     const char* name
    )
    :
    Inherited( parent, name, TRUE ),
    aboutDlgData( this )
{
    setBackgroundColor(QColor("white"));
    setCaption( "About KRN" );
    connect(b1,SIGNAL(clicked()),SLOT(accept()));
}


aboutDlg::~aboutDlg()
{
}
