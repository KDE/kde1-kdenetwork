#include <qpushbt.h>


#include <kapp.h>


#include "typelayout.h"
#include "tlform.h"


#include "rulesDlg.h"
#include "rulesDlg.moc"


rulesDlg::rulesDlg():QDialog(0,0,true)
{
    TLForm *f=new TLForm("rules",
                         klocale->translate("KRN - Scoring Rules Editor"),
                         this);
    
    KTypeLayout *l=f->layout;

    l->addGroup ("top","",true);
    l->addListBox("rulenames");
    l->findWidget("rulenames")->setMinimumWidth(100);

    l->addGroup ("rulbut","");
    l->addButton ("save",klocale->translate("Save"));
    l->newLine();
    l->addButton ("saveas",klocale->translate("Save as"));
    l->newLine();
    l->addButton ("edit",klocale->translate("Edit"));
    l->newLine();
    l->addButton ("delete",klocale->translate("Delete"));
    l->endGroup(); //rulbut

    l->addGroup ("ruleedit","",true);
    

    l->addLabel("l1",klocale->translate("Match Articles With:"));
    l->newLine();
    l->addLineEdit("expr","");
    l->newLine();

    l->addLabel("l2",klocale->translate("In Field:"));
    l->newLine();

    QStrList *fields=new QStrList;
    fields->append( "Subject" );
    fields->append( "Sender" );
    fields->append( "Cached Body" );
    fields->append( "Body" );
    l->addComboBox("field",fields);
    l->setAlign("field",AlignLeft|AlignRight);
    delete fields;

    l->newLine();
    
    l->addGroup("gg","",false);
    l->addCheckBox("casesen",klocale->translate("Case Sensitive"),false);
    l->addCheckBox("wildmode",klocale->translate("Wildcard Mode"),false);
    l->endGroup(); //gg

    l->newLine();

    l->endGroup(); //ruleedit

    l->endGroup(); //top
    
    l->newLine();
    
    l->addGroup("buttons","",false);
    QPushButton *b1=(QPushButton *)(l->addButton("b1",klocale->translate("OK"))->widget);
    QPushButton *b2=(QPushButton *)(l->addButton("b2",klocale->translate("Cancel"))->widget);
    l->endGroup();

    connect (b1,SIGNAL(clicked()),SLOT(accept()));
    connect (b2,SIGNAL(clicked()),SLOT(reject()));
    
    
    l->activate();
    
}

rulesDlg::~rulesDlg()
{
}