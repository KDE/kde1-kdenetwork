/**********************************************************************

	--- Qt Architect generated file ---

	File: KSCutDialog.cpp
	Last generated: Mon Dec 29 21:40:17 1997

 *********************************************************************/

#include "KSCutDialog.h"
#include <qevent.h>
#include <iostream.h>

#define Inherited KSCutDialogData

int KSCutDialog::open = 0;

KSCutDialog::KSCutDialog
(
	QWidget* parent,
	const char* name
)
	:
	Inherited( parent, name )
{
	setCaption( "Cut Window" );
	open = 1;
}


KSCutDialog::~KSCutDialog()
{
  open = 0;
}

void KSCutDialog::closeEvent(QCloseEvent *)
{
  delete this;
}

void KSCutDialog::setText(QString text)
{
  IrcMLE->setText(text);
  IrcMLE->setCursorPosition(IrcMLE->numLines(), 0);
}

void KSCutDialog::scrollToBot()
{
  IrcMLE->setFocus();
  IrcMLE->setCursorPosition(IrcMLE->numLines(), 0);
}


