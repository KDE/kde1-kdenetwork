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
    silentconnect=(QCheckBox *)(l->addCheckBox("silentconnect",
                                              klocale->translate("Connect without asking"),
                                                conf->readNumEntry("SilentConnect"))->widget);
    l->skip();
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
    

    connect (b1,SIGNAL(clicked()),SLOT(save()));
    connect (b2,SIGNAL(clicked()),SLOT(reject()));
}


NNTPConfigDlg::~NNTPConfigDlg()
{
}


NNTPConfigDlg::save()
{
    conf->setGroup("NNTP");
    conf->writeEntry("NNTPServer",servername->text());
    conf->writeEntry("ConnectAtStart",connectatstart->isChecked());
    conf->writeEntry("SilentConnect",silentconnect->isChecked());
    conf->writeEntry("Authenticate",authenticate->isChecked());
    conf->writeEntry("Username",username->text());
    conf->writeEntry("Password",password->text());
    conf->setGroup("sending mail");
    conf->writeEntry("Smtp Host",smtpserver->text());
    conf->sync();
    accept();
    debug ("deleting NNTP dialog");
    delete this;
}
