#include "NNTPConfigDlg.h"

#define Inherited QDialog

#include <qlabel.h>
#include <qlined.h>
#include <qpushbt.h>
#include <qchkbox.h>


#include <kconfig.h>
#include <kapp.h>

#include "tlform.h"
#include "typelayout.h"


#include "NNTPConfigDlg.moc"

extern KConfig *conf;



NNTPConfigDlg::NNTPConfigDlg(QWidget* parent, const char* name):Inherited( parent, name, TRUE )
{

    TLForm *f=new TLForm("expiration",
                         klocale->translate("KRN-NNTP configuration"),
                         this);
    
    KTypeLayout *l=f->layout;

    l->addGroup("servers",klocale->translate("Servers"),true);
    conf->setGroup("sending mail");
    l->addLabel("l1", klocale->translate("SMTP server"));
    l->newLine();
    smtpserver=(QLineEdit *)(l->addLineEdit("smtpserver",conf->readEntry("Smtp Host"))->widget);
    l->newLine();

    l->addLabel("l2", klocale->translate("NNTP Server"));
    l->newLine();
    conf->setGroup("NNTP");
    servername=(QLineEdit *)(l->addLineEdit("servername",conf->readEntry("NNTPServer"))->widget);;
    l->endGroup();

    l->newLine();

    l->addGroup("lower");

    authenticate=(QCheckBox *)(l->addCheckBox("authenticate",
                                              klocale->translate("Authenticate"),
                                              conf->readNumEntry("Authenticate"))->widget);

    connectatstart=(QCheckBox *)(l->addCheckBox("connectatstart",
                                              klocale->translate("Connect on Startup"),
                                                conf->readNumEntry("ConnectAtStart"))->widget);

    l->newLine();

    l->addLabel ("l3",klocale->translate("User Name"));
    username=(QLineEdit *)(l->addLineEdit("username",conf->readEntry("Username"))->widget);
    
    l->newLine();
    l->addLabel ("l4",klocale->translate("Password"));
    password=(QLineEdit *)(l->addLineEdit("password",conf->readEntry("Password"))->widget);

    l->endGroup();
    l->newLine();

    l->addGroup("buttons","",false);
    QPushButton *b1=(QPushButton *)(l->addButton("b1",klocale->translate("OK"))->widget);
    QPushButton *b2=(QPushButton *)(l->addButton("b2",klocale->translate("Cancel"))->widget);
    
    l->endGroup();
    l->activate();
    

    servername->setText(conf->readEntry("NNTPServer"));
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
