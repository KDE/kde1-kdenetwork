#include "decoderDlg.h"

#include "kdecode.h"
#define Inherited KTopLevelWidget

extern QString pixpath;
extern KDecode *decoder;

#define SAVE 1

decoderDlg::decoderDlg (const char* name ) :Inherited( name )
{
    QPixmap pixmap;
    KToolBar *tool = new KToolBar (this, "tool");
    addToolBar (tool);
    tool->setPos( KToolBar::Top );

    pixmap.load(pixpath+"save.xpm");
    tool->insertItem(pixmap,SAVE,true,"Save file");
    tool->show();
    QObject::connect (tool, SIGNAL (clicked (int)), this, SLOT (actions (int)));

    
    list = new KTabListBox(this);
    list->setNumCols(4);
    list->setColumn(0,"ID",40);
    list->setColumn(1,"",20,KTabListBox::PixmapColumn);
    list->setColumn(2,"Size",100);
    list->setColumn(3,"Name",200);
    setView (list);
    list->show();
    list->dict().insert("G",new QPixmap(pixpath+"green-bullet.xpm"));//GOOD
    list->dict().insert("B",new QPixmap(pixpath+"red-bullet.xpm"));  //BAD
    list->dict().insert("U",new QPixmap(pixpath+"black-bullet.xpm"));//UGLY
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
