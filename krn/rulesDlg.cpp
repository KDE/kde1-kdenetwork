#include <qpushbt.h>


#include <kapp.h>


#include "typelayout.h"
#include "tlform.h"


#include "rulesDlg.h"
#include "rulesDlg.moc"


rulesDlg::rulesDlg():QWidget()
{
    TLForm *f=new TLForm("rules",
                         klocale->translate("KRN - Scoring Rules Editor"),
                         this);
    
    KTypeLayout *l=f->layout;

    l->addGroup ("top","",true);
    l->addListBox("rulenames");

    l->addGroup ("rulbut","");
    l->addButton ("save",klocale->translate("Save"));
    l->newLine();
    l->addButton ("saveas",klocale->translate("Save as"));
    l->endGroup(); //rulbut

    l->addGroup ("ruleedit","",true);
    

    l->addLabel("l1",klocale->translate("Find Articles With:"));
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
    delete fields;

    l->newLine();
    
    l->addGroup("gg","",false);
    l->addCheckBox("casesen",klocale->translate("Case Sensitive"),false);
    l->addCheckBox("wildmode",klocale->translate("Wildcard Mode"),false);
    l->endGroup(); //gg

    l->newLine();

    l->addGroup("gg","",false);
    l->addIntLineEdit("expire","",4);
    l->addLabel("l1","days until rule dies");
    l->endGroup(); //gg
    
    l->endGroup(); //ruleedit

    l->endGroup(); //top
    
    l->newLine();
    
    l->addGroup("buttons","",false);
    QPushButton *b1=(QPushButton *)(l->addButton("b1",klocale->translate("OK"))->widget);
    QPushButton *b2=(QPushButton *)(l->addButton("b2",klocale->translate("Cancel"))->widget);
    l->endGroup();
    
    l->activate();
    
}

rulesDlg::~rulesDlg()
{
}