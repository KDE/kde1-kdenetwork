#include "decoderDlg.h"

#include "ktoolbar.h"

#include "kdecode.h"

#include "decoderDlg.moc"


#define Inherited QDialog

extern QString pixpath;
extern KDecode *decoder;

#define SAVE 1

decoderDlg::decoderDlg (const char* name ) :Inherited( 0,name,true )
{
    setCaption("KRN - Decoder");
    
    list = new KTabListBox(this,"",4);
//    list->setNumCols(4);
    list->setColumn(0,"ID",40);
    list->setColumn(1,"",20,KTabListBox::PixmapColumn);
    list->setColumn(2,"Size",100);
    list->setColumn(3,"Name",200);

    list->dict().insert("G",new QPixmap(pixpath+"green-bullet.xpm"));//GOOD
    list->dict().insert("B",new QPixmap(pixpath+"red-bullet.xpm"));  //BAD
    list->dict().insert("U",new QPixmap(pixpath+"black-bullet.xpm"));//UGLY
    list->resize(400,400);
    resize(400,400);
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
