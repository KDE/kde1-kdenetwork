#include "aboutDlg.h"
#include <kapp.h>

#define Inherited QDialog

#include "aboutDlg.moc"

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
    setCaption( klocale->translate("About KRN") );
    connect(b1,SIGNAL(clicked()),SLOT(accept()));
}


aboutDlg::~aboutDlg()
{
}
