/**********************************************************************

	--- Dlgedit generated file ---

	File: PostDialog.cpp
	Last generated: Fri Jul 11 21:12:45 1997

 *********************************************************************/

#include "PostDialog.h"

#include <qstring.h>
#include <qregexp.h>
#include <Kconfig.h>
#include <kapp.h>
#include <kmsgbox.h>
#include "PartProps.h"
#include "NewMsgDlg.h"
#include <mimelib/mimepp.h>

#define Inherited PostDialogData

PostDialog::PostDialog
(
	QString defGroup,
	QString defSubject,
	QWidget* parent,
	const char* name
)
	:
	Inherited( parent, name )
{
	setCaption( "Article manager" );
	enableButtons(FALSE);
	newsgroup->setText(defGroup);
	subject->setText(defSubject);
	temp=new KTempFile();

	tip=new QToolTipGroup(this);
	connect(tip,SIGNAL(showTip(const char*)),helpLabel,
	        SLOT(setText(const char*)));
	connect(tip,SIGNAL(removeTip()),helpLabel,SLOT(clear()));
	QToolTip::add(addBtn, "Add part", tip, "Add a new text or "
	              "attachment to the article");
	QToolTip::add(cancelBtn, "Cancel article", tip,
	              "Cancel this article, and discard any text written");
	QToolTip::add(postNBtn, "Post now", tip, "Send this article now!");
	
}


PostDialog::~PostDialog()
{
    delete temp;
}

void PostDialog::edit(char* type, char* name)
{
    CHECK_PTR(type);
    CHECK_PTR(name);
    debug("editing file %s of type type %s",name,type);
    if(strcmp(type,"text/plain")!=0)
    {
        warning("Sorry, only plaintext editing is supported yet!");
        return;
    }
    QString editor;
    KConfig* conf=kapp->getConfig();
    conf->setGroup("Editors");
    editor=conf->readEntry(type);
    if(editor.isEmpty())
    {
        warning("No editor for type %s found!",type);
        return;
    }
    editor.replace(QRegExp("%s"),name);
    system(editor);
}

void PostDialog::editPart(int n)
{
    debug("editing part %d",n);
    //debug("(file=%s, type=%s)",part.at(n)->type.data(), part.at(n)->name.data());
    edit(part.at(n)->type.data(), part.at(n)->name.data());
}

void PostDialog::editPart(const char* f)
{
    unsigned int n;
    for(n=0; n<part.count(); n++) if(part.at(n)->name==f) break;
    editPart(n-1);
}

void PostDialog::propsPart()
{
    debug("Props for itm %d (%s)", partList->currentItem(), part.at(partList->currentItem())->name.data());
    PartProps p(part.at(partList->currentItem()),this);
    p.show();
}

void PostDialog::addPart()
{
    part.append(new messagePart);
    NewMsgDlg d(part.at(part.count()-1),temp,this);
    connect(&d,SIGNAL(edit(const char*)),this,SLOT(editPart(const char*)));
    d.show();
    if(!part.at(part.count()-1)->name.isEmpty())
    {
        //debug("Got type=%s, name=%s",part.at(part.count()-1)->type.data(),part.at(part.count()-1)->name.data());
        partList->insertItem(part.at(part.count()-1)->name);
        enableButtons(TRUE);
    }
    else {
        part.remove(part.count()-1);
        debug("New part creation canceled");
    }
}

void PostDialog::postLater()
{
    warning("Not implemented yet!");
}

void PostDialog::showHelp()
{
    kapp->invokeHTMLHelp("dialogs.html","posting");
}

void PostDialog::removePart()
{
    int n=partList->currentItem();
    if(n==-1) return;
    debug("Removing item %d",n);
    part.remove(n);
    partList->removeItem(n);
    if(partList->currentItem()==-1) enableButtons(FALSE);
}

void PostDialog::editPart()
{
    editPart(partList->currentItem());
}

void PostDialog::select(int)
{
    //Emty stub. Could be used to do fun things some day.
}

void PostDialog::cancel()
{
    if(part.count()!=0)
    {
        int c=KMsgBox::yesNo(this,"Confirm cancel","There are edited message "
                       "parts in this article.\n Are you sure you want to "
                       "cancel this article (and loose these parts)?");
        if(c==1) delete this;
    }
    else delete this;
}

void PostDialog::postNow()
{
    DwMessage* m=new DwMessage;

    KConfig* conf=kapp->getConfig();


    DwField s;
    s.SetFieldNameStr("subject");
    s.SetFieldBodyStr(subject->text());
    s.Assemble();
    m->Headers().AddField(&s);

    DwField g;
    g.SetFieldNameStr("newsgroups");
    g.SetFieldBodyStr(newsgroup->text());
    g.Assemble();
    m->Headers().AddField(&g);

    DwField adr;
    DwGroup* ag=new DwGroup;
    conf->setGroup("Identity");
    QString name=conf->readEntry("RealName");
    ag->SetPhrase(name.data());
    QString address=conf->readEntry("Address");
    ag->NewGroup(address.data(),&adr);
    ag->Assemble();
    adr.SetFieldNameStr("from");
    adr.SetFieldBody(ag);
    m->Headers().AddField(&adr);

    DwBodyPart* p;
    DwField *c;
    QFile f;
    char* t;
    //FIXME: leaks memory
    for(int a=0; a<(int)part.count(); a++)
    {
        debug("Creating part %d",a);
        debug("Type=%s, name=%s",part.at(a)->type.data(), part.at(a)->name.data());
        f.setName(part.at(a)->name);
        f.open(IO_ReadOnly);
        t=(char*)malloc(f.size());
        f.readBlock(t,f.size());
        f.close();

        p=new DwBodyPart;
        c=new DwField;
        c->SetFieldNameStr("Content-type");
        c->SetFieldBodyStr(part.at(a)->type.data());
        p->Headers().AddField(c);

        p->FromString(t);
        p->Parse();

        p->Assemble();
        m->Body().AddBodyPart(p);
        debug("Appended part: \"%s\"",p->AsString().data());
    }
    m->Assemble();
    m->Parse();
    debug("Ready to post. Article=\"%s\"",m->AsString().data());
    //replace FALSE with a function call that actually tries to post the article
    if(FALSE)
    {
        delete this;
    }
    else
    {
        KMsgBox::message(this,"Posting failed","Krn was not able to post "
                         "your article (posting is not supported yet, only "
                         "composing.) :-(");
    }
}

void PostDialog::enableButtons(bool b)
{
    postNBtn->setEnabled(b);
    editBtn->setEnabled(b);
    removeBtn->setEnabled(b);
    propsBtn->setEnabled(b);
    postLBtn->setEnabled(FALSE);
}
