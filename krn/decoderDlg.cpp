#include "decoderDlg.h"

#include "ktoolbar.h"

#include <kapp.h>
#include "kdecode.h"

#include "decoderDlg.moc"

#include <qpushbt.h>

#define Inherited QDialog

extern QString pixpath;
extern KDecode *decoder;

#define SAVE 1

decoderDlg::decoderDlg (const char* name ) :Inherited( 0,name,true )
{
    setCaption(klocale->translate( "KRN - Decoder"));
    
    list = new KTabListBox(this,"",4);
    list->setColumn(0,klocale->translate("ID"),40);
    list->setColumn(1,"",20,KTabListBox::PixmapColumn);
    list->setColumn(2,klocale->translate("Size"),100);
    list->setColumn(3,klocale->translate("Name"),200);

    list->dict().insert("G",new QPixmap(pixpath+"green-bullet.xpm"));//GOOD
    list->dict().insert("B",new QPixmap(pixpath+"red-bullet.xpm"));  //BAD
    list->dict().insert("U",new QPixmap(pixpath+"black-bullet.xpm"));//UGLY
    list->resize(400,380);
    resize(400,400);

    QPushButton *b1=new QPushButton("Done",this,"");
    b1->setGeometry (1,381,398,18);

    QObject::connect(b1,SIGNAL(clicked()),this,SLOT(accept()));
}

decoderDlg::~decoderDlg()
{
}

void decoderDlg::actions(int action)
{
    switch (action)
    {
    case SAVE:
        {
            decoder->decode(list->currentItem(),0);
            break;
        }
    }
}
