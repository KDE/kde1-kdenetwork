#include <qpushbt.h>
#include <qlist.h>
#include <qlined.h>
#include <qlabel.h>
#include <qcombo.h>
#include <qlistbox.h>
#include <qchkbox.h>


#include <kapp.h>
#include <ksimpleconfig.h>


#include "typelayout.h"
#include "tlform.h"
#include "asker.h"


#include "rulesDlg.h"
#include "rulesDlg.moc"

extern KSimpleConfig *ruleFile;

rulesDlg::rulesDlg():QDialog(0,0,true)
{
    TLForm *f=new TLForm("rules",
                         klocale->translate("KRN - Scoring Rules Editor"),
                         this);
    
    l=f->layout;

    l->addGroup ("top","",true);

    ruleFile->setGroup("Index");
    QStrList names;
    ruleFile->readListEntry("RuleNames",names);

    list=(QListBox *)(l->addListBox("rulenames",&names)->widget);
    list->setMinimumWidth(100);
    connect (list,SIGNAL(highlighted(const char*)),
             this,SLOT(editRule(const char*)));
                       

    l->addGroup ("rulbut","");
    QPushButton *save=(QPushButton *)
        (l->addButton ("save",klocale->translate("Save"))->widget);
    connect (save,SIGNAL(clicked()),SLOT(saveRule()));
    l->newLine();


    QPushButton *saveas=(QPushButton *)
        (l->addButton ("saveas",klocale->translate("Save as"))->widget);
    connect (saveas,SIGNAL(clicked()),SLOT(saveRuleAs()));
    l->newLine();


    QPushButton *delrule=(QPushButton *)
        (l->addButton ("delete",klocale->translate("Delete"))->widget);
    connect (delrule,SIGNAL(clicked()),SLOT(deleteRule()));

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
    QPushButton *b1=(QPushButton *)(l->addButton("b1",klocale->translate("Done"))->widget);
    l->endGroup();

    connect (b1,SIGNAL(clicked()),SLOT(accept()));
    
    l->activate();

    list->setCurrentItem(0);
    
}

rulesDlg::~rulesDlg()
{
}


void rulesDlg::editRule(const char *name)
{
    rule=new Rule(0,0,Rule::Sender,false,false);
    rule->load(name);
    
    ((QLineEdit *)(l->findWidget("expr")))->setText(rule->regex.pattern());
    ((QComboBox *)(l->findWidget("field")))->setCurrentItem((int)rule->field);
    ((QCheckBox *)(l->findWidget("casesen")))->setChecked(rule->regex.caseSensitive());
    ((QCheckBox *)(l->findWidget("wildmode")))->setChecked(rule->regex.wildcard());

    
    delete rule;
}
void rulesDlg::saveRuleAs()
{
    Asker ask;
    ask.label->setText(klocale->translate("Rule name:"));
    ask.exec();
    rule=new Rule(ask.entry->text(),
                  ((QLineEdit *)(l->findWidget("expr")))->text(),
                  (Rule::Field)((QComboBox *)(l->findWidget("field")))->currentItem(),
                  ((QCheckBox *)(l->findWidget("casesen")))->isChecked(),
                  ((QCheckBox *)(l->findWidget("wildmode")))->isChecked());
    rule->save(ask.entry->text());
    list->insertItem(ask.entry->text());
    delete rule;
}

void rulesDlg::saveRule()
{
    QString name=list->text(list->currentItem());
    rule=new Rule(name,
                  ((QLineEdit *)(l->findWidget("expr")))->text(),
                  (Rule::Field)((QComboBox *)(l->findWidget("field")))->currentItem(),
                  ((QCheckBox *)(l->findWidget("casesen")))->isChecked(),
                  ((QCheckBox *)(l->findWidget("wildmode")))->isChecked());
    rule->save(name);
    delete rule;
}

void rulesDlg::deleteRule()
{
    QString name=list->text(list->currentItem());
    ruleFile->setGroup("Index");
    QStrList names;
    ruleFile->readListEntry("RuleNames",names);
    if (names.find(name.data())!=-1)
    {
        names.remove(name);
        ruleFile->writeEntry("RuleNames",names);
    }
    ruleFile->sync();
    list->removeItem(list->currentItem());
}
