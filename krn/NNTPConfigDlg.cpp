#include "NNTPConfigDlg.h"

#define Inherited QDialog

#include <kconfig.h>
#include <kapp.h>

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
    setCaption( klocale->translate("KRN-NNTP configuration") );
    conf->setGroup("NNTP");
    servername->setText(conf->readEntry("NNTPServer"));
    smtpserver->setText(conf->readEntry("SMTPServer"));
    connectatstart->setChecked(conf->readNumEntry("ConnectAtStart"));
    authenticate->setChecked(conf->readNumEntry("Authenticate"));
    username->setText(conf->readEntry("Username"));
    password->setText(conf->readEntry("Password"));

    connect (b1,SIGNAL(clicked()),SLOT(accept()));
    connect (b2,SIGNAL(clicked()),SLOT(reject()));
}


NNTPConfigDlg::~NNTPConfigDlg()
{
}
