/*
 * kbiff.cpp
 * Copyright (C) 1998 Kurt Granroth <granroth@kde.org>
 *
 * This file contains the implementation of the main KBiff
 * widget
 *
 * $Id$
 */
#include "kbiff.h"
#include "kbiff.moc"

#include "Trace.h"

#include <kiconloader.h>
#include <kprocess.h>
#include <kwm.h>

#include "setupdlg.h"

KBiff::KBiff(QWidget *parent)
	: QLabel(parent)
{
TRACEINIT("KBiff::KBiff()");
	setAutoResize(true);
	setMargin(0);
	setAlignment(AlignLeft | AlignTop);


	// Init the audio server.  serverStatus return 0 when it is ok
	hasAudio = (audioServer.serverStatus() == 0) ? true : false;

	reset();

	monitor.setMailbox(mailbox);
	monitor.setPollInterval(poll);

	connect(&monitor, SIGNAL(signal_newMail()), this, SLOT(haveNewMail()));
	connect(&monitor, SIGNAL(signal_noMail()), this, SLOT(displayPixmap()));
	connect(&monitor, SIGNAL(signal_oldMail()), this, SLOT(displayPixmap()));
}

KBiff::~KBiff()
{
}

void KBiff::setMailbox(const KURL& new_mailbox)
{
	mailbox = new_mailbox;
	monitor.setMailbox(mailbox);
}

///////////////////////////////////////////////////////////////////////////
// Protected Virtuals
///////////////////////////////////////////////////////////////////////////
void KBiff::mousePressEvent(QMouseEvent *event)
{
TRACEINIT("KBiff::mousePressEvent()");
	// check if this is a right click
	if(event->button() == RightButton)
	{
		// popup the context menu
		popupMenu();
	}
	else
	{
		// force a "read"
		//monitor.setMailboxIsRead();

		// execute the command
		if (!mailClient.isEmpty())
		{
			KProcess client;
			client << mailClient;
			client.start(KProcess::DontCare);
		}
	}
}

///////////////////////////////////////////////////////////////////////////
// Protected Slots
///////////////////////////////////////////////////////////////////////////
void KBiff::invokeHelp()
{
	kapp->invokeHTMLHelp("kbiff/kbiff.html", "");
}

void KBiff::displayPixmap()
{
	// we will try to deduce the pixmap (or gif) name now.  it will
	// vary depending on the dock and mail state
	QString pixmap_name, mini_pixmap_name;
	switch (monitor.getMailState())
	{
		case NoMail:
			pixmap_name = noMailIcon;
			break;
		case NewMail:
			pixmap_name = newMailIcon;
			break;
		case OldMail:
			pixmap_name = oldMailIcon;
			break;
		default:
			pixmap_name = noMailIcon;
			break;
	}
	mini_pixmap_name = "mini-" + pixmap_name;

	// Get a list of all the pixmap paths.  This is needed since the
	// icon loader only returns the name of the pixmap -- NOT it's path!
	QStrList *dir_list = kapp->getIconLoader()->getDirList();
	QFileInfo file, mini_file;
	for (unsigned int i = 0; i < dir_list->count(); i++)
	{
		QString here = dir_list->at(i);

		// check if this file exists.  If so, we have our path
		file.setFile(here + '/' + pixmap_name);
		mini_file.setFile(here + '/' + mini_pixmap_name);

		// if we are docked, check if the mini pixmap exists FIRST.
		// if it does, we go with it.  if not, we look for the
		// the regular size one
		if (isDocked && mini_file.exists())
		{
			file = mini_file;
			break;
		}

		if (file.exists())
			break;
	}

	// at this point, we have the file to display.  so display it
	QPixmap pixmap(file.absFilePath());
	setPixmap(pixmap);
	adjustSize();
}

void KBiff::haveNewMail()
{
	displayPixmap();
	// beep if we are allowed to
	if (systemBeep)
	{
		kapp->beep();
	}

	// run a command if we have to
	if (runCommand)
	{
		// make sure the command exists
		if (!runCommandPath.isEmpty())
		{
			KProcess command;
			command << runCommandPath;
			command.start(KProcess::DontCare);
		}
	}

	// play a sound if we have to
	if (playSound && hasAudio)
	{
		// make sure something is specified
		if (!playSoundPath.isEmpty())
		{
			audioServer.play(playSoundPath);
			audioServer.sync();
		}
	}
}

void KBiff::dock()
{
	// destroy the old window
	if (this->isVisible())
	{
		this->hide();
		this->destroy(true, true);
		this->create(0, true, false);
		kapp->setMainWidget(this);

		// we don't want a "real" top widget if we are _going_ to
		// be docked.
		if (isDocked)
			kapp->setTopWidget(this);
		else
			kapp->setTopWidget(new QWidget);
	}

	if (isDocked == false)
	{
		isDocked = true;

		// enable docking
		KWM::setDockWindow(this->winId());
	}
	else
		isDocked = false;

	// (un)dock it!
	this->show();
	displayPixmap();
}

void KBiff::setup()
{
	KBiffSetup setup_dlg;

	setup_dlg.exec();
}

///////////////////////////////////////////////////////////////////////////
// Protected Functions
///////////////////////////////////////////////////////////////////////////
void KBiff::popupMenu()
{
	QPopupMenu *popup = new QPopupMenu(0, "popup");

	if (isDocked)
		popup->insertItem(i18n("&UnDock"), this, SLOT(dock()));
	else
		popup->insertItem(i18n("&Dock"), this, SLOT(dock()));
	popup->insertItem(i18n("&Setup..."), this, SLOT(setup()));
	popup->insertSeparator();
	popup->insertItem(i18n("&Help..."), this, SLOT(invokeHelp()));
	popup->insertSeparator();

	int check_id;
	check_id = popup->insertItem(i18n("&Check mail now"), &monitor, SLOT(checkMailNow()));
	if (monitor.isRunning())
	{
		popup->setItemEnabled(check_id, true);
		popup->insertItem(i18n("&Stop"), &monitor, SLOT(stop()));
	}
	else
	{
		popup->setItemEnabled(check_id, false);
		popup->insertItem(i18n("&Start"), &monitor, SLOT(start()));
	}
	popup->insertSeparator();
	popup->insertItem(i18n("E&xit"), kapp, SLOT(quit()));

	popup->popup(QCursor::pos());
}

void KBiff::reset()
{
	// reset all the member variables
	poll    = 15;
	mailbox = "imap4://granroth:letmein@localhost/Mail/testbox";

	systemBeep     = true;
	runCommand     = false;
	runCommandPath = "";
	playSound      = false;
	playSoundPath  = "";

	noMailIcon  = "nomail.xpm";
	newMailIcon = "newmail.xpm";
	oldMailIcon = "oldmail.xpm";

	isDocked   = false;

	mailClient = "xmutt";

	displayPixmap();
}
