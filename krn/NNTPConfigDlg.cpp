#include "NNTPConfigDlg.h"

#define Inherited QDialog

#include <Kconfig.h>

#include "NNTPConfigDlg.moc"

extern KConfig *conf;



NNTPConfigDlg::NNTPConfigDlg
    (
     QWidget* parent,
     const char* name
    )
    :
    Inherited( parent, name, TRUE ),
    NNTPConfigDlgData( this )
{
    setCaption( "KRN-NNTP configuration" );
    conf->setGroup("NNTP");
    servername->setText(conf->readEntry("NNTPServer").data());
    connectatstart->setChecked(conf->readNumEntry("ConnectAtStart"));
    authenticate->setChecked(conf->readNumEntry("Authenticate"));
    username->setText(conf->readEntry("Username").data());
    password->setText(conf->readEntry("Password").data());

    connect (b1,SIGNAL(clicked()),SLOT(accept()));
    connect (b2,SIGNAL(clicked()),SLOT(reject()));
}


NNTPConfigDlg::~NNTPConfigDlg()
{
}
