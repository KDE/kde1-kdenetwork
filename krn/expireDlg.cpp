#include "expireDlg.h"



#include <qlayout.h>
#include <qlabel.h>
#include <qframe.h>

#include <kapp.h>
#include <klocale.h>
#include <kconfig.h>

#include "expireDlg.moc"


extern KConfig *conf;

ExpireDlg::ExpireDlg() :
    QDialog (0,klocale->translate("KRN - Expire Settings"),true)
{
    conf->setGroup("Cache");
    QVBoxLayout *vBox=new QVBoxLayout(this,8,8);
    QHBoxLayout *hBox=new QHBoxLayout(8);

    QGridLayout *grid=new QGridLayout(2,2,8);

    QLabel *l1=new QLabel (klocale->translate("For how long should we keep things (in days)?"),this);
    l1->setAlignment(AlignCenter);
    
    QLabel *l2=new QLabel (klocale->translate("Article Bodies:"),this);
    l2->setFixedSize(l2->sizeHint());
    bodies=new KIntLineEdit(this);
    bodies->setText(conf->readEntry("ExpireBodies","5"));
    bodies->setMaxLength(3);
    int i=bodies->fontMetrics().height()+5;
    int ww=bodies->fontMetrics().width("888")+10;
    bodies->setFixedHeight(i);
    bodies->setFixedWidth(ww);

    QLabel *l3=new QLabel (klocale->translate("Article Headers:"),this);
    l3->setFixedSize(l3->sizeHint());
    headers=new KIntLineEdit(this);
    headers->setText(conf->readEntry("ExpireHeaders","5"));
    headers->setMaxLength(3);
    headers->setFixedHeight(i);
    headers->setFixedWidth(ww);

    QPushButton *b1=new QPushButton (klocale->translate("OK"),this);
    QPushButton *b2=new QPushButton (klocale->translate("Cancel"),this);

    l1->setFixedHeight(3*i);
    l1->setMinimumWidth(l1->fontMetrics().width(klocale->translate("For how long should we keep things (in days)?"))+10);

    if (i<25)
    {
        b1->setFixedHeight(25);
        b2->setFixedHeight(25);
    }
    else
    {
        b1->setFixedHeight(i);
        b2->setFixedHeight(i);
    }

    int w=b1->fontMetrics().width(klocale->translate("OK"));
    if (w<75)
        w=75;
    b1->setFixedWidth(w);

    w=b2->fontMetrics().width(klocale->translate("OK"));
    if (w<75)
        w=75;
    b2->setFixedWidth(w);
                                  
    vBox->addWidget(l1,5,AlignCenter);
    vBox->addLayout(grid,0);

    grid->addWidget(l2,0,0,AlignRight);
    grid->addWidget(bodies,0,1,AlignLeft);
    grid->addWidget(l3,1,0,AlignRight);
    grid->addWidget(headers,1,1,AlignLeft);

    vBox->addLayout(hBox,2);
    hBox->addWidget(b1,0,AlignVCenter);
    hBox->addStretch(0);
    hBox->addWidget(b2,0,AlignVCenter);

    vBox->activate();

    resize(10,10);

    connect (b1,SIGNAL(clicked()),SLOT(save()));
    connect (b2,SIGNAL(clicked()),SLOT(reject()));
    
}

void ExpireDlg::save()
{
    conf->setGroup("Cache");
    conf->writeEntry("ExpireBodies",bodies->text());
    conf->writeEntry("ExpireHeaders",headers->text());
    accept();
}



ExpireDlg::~ExpireDlg()
{
}

