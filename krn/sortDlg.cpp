#include <qpushbt.h>
#include <qstrlist.h>


#include <kapp.h>


#include "typelayout.h"
#include "tlform.h"


#include "sortDlg.h"
#include "sortDlg.moc"

sortDlg::sortDlg():QWidget()
{
    TLForm *f=new TLForm("sorting",
                         klocale->translate("KRN - Header Sorting"),
                         this);
    
    KTypeLayout *l=f->layout;

    l->addGroup("up","",true);

    QStrList *allkeys=new QStrList;
    allkeys->append(klocale->translate("Subject"));
    allkeys->append(klocale->translate("Score"));
    allkeys->append(klocale->translate("Sender"));
    allkeys->append(klocale->translate("Date"));
    allkeys->append(klocale->translate("None"));

    l->addLabel("l1",klocale->translate("Sort by:"));
    l->newLine();
    l->addLabel("l1","This doesn't work yet!");
    l->newLine();
    l->addComboBox("key1",allkeys);
    l->newLine();
    l->addComboBox("key2",allkeys);
    l->newLine();
    l->addComboBox("key3",allkeys);
    l->newLine();
    l->addComboBox("key4",allkeys);

    l->setAlign("key1",AlignLeft|AlignRight);
    l->setAlign("key2",AlignLeft|AlignRight);
    l->setAlign("key3",AlignLeft|AlignRight);
    l->setAlign("key4",AlignLeft|AlignRight);
    
    delete allkeys;
    l->newLine();
    l->addCheckBox("threaded",klocale->translate("Use Threading"));

    l->endGroup();
    l->newLine();

    l->addGroup("buttons","",false);
    QPushButton *b1=(QPushButton *)(l->addButton("b1",klocale->translate("OK"))->widget);
    QPushButton *b2=(QPushButton *)(l->addButton("b2",klocale->translate("Cancel"))->widget);
    l->endGroup();
    
    l->activate();
    
}

sortDlg::~sortDlg()
{
}
