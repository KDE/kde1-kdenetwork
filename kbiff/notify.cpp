/*
 * notify.cpp
 * Copyright (C) 1998 Kurt Granroth <granroth@kde.org>
 *
 * This file contains the implementation of the KBiffNotify
 * widget
 *
 * $Id$
 */
#include "notify.h"

#include "Trace.h"

#include <qlayout.h>
#include <qlabel.h>
#include <qpixmap.h>
#include <qpushbutton.h>

#include <kiconloaderdialog.h>
#include <kapp.h>

KBiffNotify::KBiffNotify(const int num_new, const QString& mailbox)
	: QDialog(0, 0, true, 0)
{
TRACEINIT("KBiffNotify::KBiffNotify()");
TRACEF("%d new messages in %s", num_new, mailbox.data());
	setIcon(ICON("kbiff.xpm"));
	setCaption(i18n("You have new mail!"));

	resize(0, 0);

	QVBoxLayout *layout = new QVBoxLayout(this, 12);

	QHBoxLayout *upper_layout = new QHBoxLayout();
	layout->addLayout(upper_layout);

	QLabel *pixmap = new QLabel(this);
	pixmap->setPixmap(ICON("kbiff.xpm"));
	pixmap->setFixedSize(pixmap->sizeHint());
	upper_layout->addWidget(pixmap);

	QVBoxLayout *mailbox_layout = new QVBoxLayout();
	upper_layout->addLayout(mailbox_layout);

	QLabel *congrats = new QLabel(i18n("You have new mail!"), this);
	QFont the_font(congrats->font());
	the_font.setBold(true);
	congrats->setFont(the_font);
	congrats->setMinimumSize(congrats->sizeHint());
	mailbox_layout->addWidget(congrats);

	QString msg;
	msg.sprintf(i18n("New Messages: %d"), num_new);
	QLabel *how_many = new QLabel(msg, this);
	how_many->setMinimumSize(how_many->sizeHint());
	mailbox_layout->addWidget(how_many);

	msg.sprintf(i18n("Mailbox: %s"), mailbox.data());
	QLabel *which_one = new QLabel(msg, this);
	which_one->setMinimumSize(which_one->sizeHint());
	mailbox_layout->addWidget(which_one);

	QPushButton *ok = new QPushButton(i18n("OK"), this);
	ok->setFixedSize(ok->sizeHint());
	connect(ok, SIGNAL(clicked()), SLOT(accept()));

	QHBoxLayout *button_layout = new QHBoxLayout();
	layout->addLayout(button_layout);

	button_layout->addStretch(1);
	button_layout->addWidget(ok);
	button_layout->addStretch(1);

	layout->activate();
}

KBiffNotify::~KBiffNotify()
{
}
