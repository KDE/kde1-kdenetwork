/**********************************************************************

	--- Dlgedit generated file ---

	File: NewMsgDlg.cpp
	Last generated: Fri Jul 11 20:18:41 1997

 *********************************************************************/

#include "NewMsgDlg.h"
#include <Kconfig.h>
#include <kapp.h>
#undef Unsorted
#include <qfiledlg.h>

#define Inherited NewMsgDlgData

NewMsgDlg::NewMsgDlg
(
	messagePart* p,
	KTempFile* temp,
	QWidget* parent,
	const char* name
)
	:
	Inherited( parent, name )
{
	part=p;
	t=temp;
	setCaption( "Add new message part" );
	typeList->insertItem("text/plain");
	typeList->setCurrentItem(0);
}


NewMsgDlg::~NewMsgDlg()
{
}

void NewMsgDlg::medit()
{
    medit(typeList->currentItem());
}

void NewMsgDlg::cancel()
{
    delete this;
}

void NewMsgDlg::medit(int n)
{
    int f=t->create("posting","");
    t->file(f)->open(IO_WriteOnly);
    t->file(f)->close();

    part->name=t->file(f)->name();
    part->type=typeList->text(n);

    emit edit(part->type);
    delete this;
}

void NewMsgDlg::read()
{
    QString f=QFileDialog::getOpenFileName();
    if(!f.isEmpty())
    {
        part->name=f;
        part->type=typeList->text(typeList->currentItem());
        delete this;
    }
}
