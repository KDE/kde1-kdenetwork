#include "decoderDlg.h"

#include "ktoolbar.h"

#include <kapp.h>
#include <kconfig.h>
#include "kdecode.h"

#include "decoderDlg.moc"

#include <qpushbt.h>
#include <qlayout.h>
#include <qlined.h>
#include <qlabel.h>

#define Inherited QDialog

extern QString pixpath;
extern KDecode *decoder;

extern KConfig *conf;

#define SAVE 1

decoderDlg::decoderDlg (const char* name ) :Inherited( 0,name,true )
{

    conf->setGroup("Decoding");
    QVBoxLayout *l=new QVBoxLayout (this);
    
    setCaption(klocale->translate( "KRN - Decoder"));
    
    list = new KTabListBox(this,"",4);
    list->setColumn(0,klocale->translate("ID"),40);
    list->setColumn(1,"",20,KTabListBox::PixmapColumn);
    list->setColumn(2,klocale->translate("Size"),100);
    list->setColumn(3,klocale->translate("Name"),1);

    list->dict().insert("G",new QPixmap(pixpath+"green-bullet.xpm"));//GOOD
    list->dict().insert("B",new QPixmap(pixpath+"red-bullet.xpm"));  //BAD
    list->dict().insert("U",new QPixmap(pixpath+"black-bullet.xpm"));//UGLY

    QLabel *l1=new QLabel(klocale->translate("Destination directory:"),this);
    destdir=new QLineEdit(this);
    destdir->setText(conf->readEntry("DecodePath",getenv("HOME")));
    QPushButton *b2=new QPushButton(klocale->translate("Decode All"),this,"");
    QPushButton *b1=new QPushButton(klocale->translate("Done"),this,"");

    b1->setFixedHeight(30);
    b2->setFixedHeight(30);
    destdir->setFixedHeight(30);
    l1->setFixedHeight(30);
    
    QObject::connect(b1,SIGNAL(clicked()),this,SLOT(close()));

    l->addWidget(list,10);
    l->addWidget(l1,0);
    l->addWidget(destdir,0);
    l->addWidget(b2,10);
    l->addWidget(b1,0);
    l->activate();

    resize(350,400);
    
}

decoderDlg::~decoderDlg()
{
}

void decoderDlg::decodeAll()
{
}

void decoderDlg::close()
{
    conf->setGroup("Decoding");
    conf->writeEntry("DecodePath",destdir->text());
    accept();
}

void decoderDlg::actions(int action)
{
    switch (action)
    {
    case SAVE:
        {
            decoder->decode(list->currentItem());
            break;
        }
    }
}
