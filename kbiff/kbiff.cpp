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

#include <qmovie.h>

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

	// enable the session management stuff
	connect(kapp, SIGNAL(saveYourself()), this, SLOT(saveYourself()));

	reset();
}

KBiff::~KBiff()
{
}

void KBiff::processSetup(const KBiffSetup* setup, bool run)
{
TRACEINIT("KBiff::processSetup()");
	// General settings
	profile     = setup->getProfile();
	mailClient  = setup->getMailClient();
	sessions    = setup->getSessionManagement();
	noMailIcon  = setup->getNoMailIcon();
	newMailIcon = setup->getNewMailIcon();
	oldMailIcon = setup->getOldMailIcon();

	// New mail
	systemBeep     = setup->getSystemBeep();
	runCommand     = setup->getRunCommand();
	runCommandPath = setup->getRunCommandPath();
	playSound      = setup->getPlaySound();
	playSoundPath  = setup->getPlaySoundPath();
	notify         = setup->getNotify();

	// set all the new mailboxes
	setMailboxList(setup->getMailboxList(), setup->getPoll());

	// change the dock state if necessary
TRACEF("docked = %d", docked);
TRACEF("setup->getDock() = %d", setup->getDock());
	if (docked != setup->getDock())
		dock();

	if (run)
		start();

	delete setup;
}

void KBiff::setMailboxList(const QList<KURL>& mailbox_list, unsigned int poll)
{
TRACEINIT("KBiff::setMailboxList");
	QList<KURL> tmp_list = mailbox_list;

	myMUTEX = true;
	if (isRunning())
		stop();
	monitorList.clear();
	
	KURL *url;
	for (url = tmp_list.first(); url != 0; url = tmp_list.next())
	{
		TRACEF("Now adding %s", url->url().data());
		KBiffMonitor *monitor = new KBiffMonitor();
		monitor->setMailbox(*url);
		monitor->setPollInterval(poll);
		connect(monitor, SIGNAL(signal_newMail()), this, SLOT(haveNewMail()));
		connect(monitor, SIGNAL(signal_noMail()), this, SLOT(displayPixmap()));
		connect(monitor, SIGNAL(signal_oldMail()), this, SLOT(displayPixmap()));
		monitorList.append(monitor);
	}
	myMUTEX = false;
}

inline const bool KBiff::isDocked() const
{
	return docked;
}

void KBiff::readSessionConfig()
{
TRACEINIT("KBiff::readSesionConfig()");
	KConfig *config = kapp->getSessionConfig();

	config->setGroup("KBiff");

	profile = config->readEntry("Profile", "Inbox");
	docked = config->readBoolEntry("IsDocked", false);
	bool run = config->readBoolEntry("IsRunning", true);

	processSetup(new KBiffSetup(profile), run);
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
		// execute the command
		if (!mailClient.isEmpty())
		{
			KProcess client;
			// we need to munge mailClient to have client + params
			int index;
			QString cmd(mailClient.simplifyWhiteSpace());
			while ((index = cmd.find(' ')) > 0)
			{
				client << cmd.left(index);
				cmd = cmd.mid(index+1, cmd.length());
			}
			client << cmd;

			client.start(KProcess::DontCare);
		}
	}
}

bool KBiff::isGIF8x(const char* file_name)
{
TRACEINIT("KBiff::isGIF8x()");

	/* The first test checks if we can open the file */
	QFile gif8x(file_name);
	if (gif8x.open(IO_ReadOnly) == false)
		return false;

	/**
	 * The GIF89 format specifies that the first five bytes of
	 * the file shall have the characters 'G' 'I' 'F' '8' '9'.
	 * The GIF87a format specifies that the first six bytes
	 * shall read 'G' 'I' 'F' '8' '7' 'a'.  Knowing that, we
	 * shall read in the first six bytes and test away.
	 */
	char header[6];
	int bytes_read = gif8x.readBlock(header, 6);

	/* Close the file just to be nice */
	gif8x.close();

	/* If we read less than 6 bytes, then its definitely not GIF8x */
	if (bytes_read < 6)
		return false;

	/* Now test for the magical GIF8(9|7a) */
	if (header[0] == 'G' &&
		 header[1] == 'I' &&
		 header[2] == 'F' &&
		 header[3] == '8' &&
		 header[4] == '9' ||
		(header[4] == '7' && header[5] == 'a'))
	{
		/* Success! */
		TRACE("Is GIF8x!");
		return true;
	}

	/* Apparently not GIF8(9|7a) */
	return false;
}

///////////////////////////////////////////////////////////////////////////
// Protected Slots
///////////////////////////////////////////////////////////////////////////
void KBiff::saveYourself()
{
TRACEINIT("KBiff::saveYourself()");
	if (sessions)
	{
		KConfig *config = kapp->getSessionConfig();
		config->setGroup("KBiff");

		config->writeEntry("Profile", profile);
		config->writeEntry("IsDocked", docked);
		config->writeEntry("IsRunning", isRunning());

		config->sync();

		TRACEF("Saving session profile: %s", profile.data());
	}
}

void KBiff::invokeHelp()
{
	kapp->invokeHTMLHelp("kbiff/kbiff.html", "");
}

void KBiff::displayPixmap()
{
TRACEINIT("KBiff::displayPixmap()");
	if (myMUTEX)
		return;
	TRACE("After MUTEX");

	// we will try to deduce the pixmap (or gif) name now.  it will
	// vary depending on the dock and mail state
	QString pixmap_name, mini_pixmap_name;
	bool has_new = false, has_old = false, has_no = true;
	KBiffMonitor *monitor;
	for (monitor = monitorList.first();
	     monitor != 0 && has_new == false;
		  monitor = monitorList.next())
	{
		TRACE("Checking mailbox");
		switch (monitor->getMailState())
		{
			case NoMail:
				has_no = true;
				break;
			case NewMail:
				has_new = true;
				break;
			case OldMail:
				has_old = true;
				break;
			default:
				has_no = true;
				break;
		}
	}
	TRACE("Done checking mailboxes");

	if (has_new)
		pixmap_name = newMailIcon;
	else if (has_old)
		pixmap_name = oldMailIcon;
	else
		pixmap_name = noMailIcon;
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
		if (docked && mini_file.exists())
		{
			file = mini_file;
			break;
		}

		if (file.exists())
			break;
	}

	TRACEF("Displaying %s", file.absFilePath().data());
	// at this point, we have the file to display.  so display it
	if (isGIF8x(file.absFilePath()))
		setMovie(QMovie(file.absFilePath().data()));
	else
		setPixmap(QPixmap(file.absFilePath()));
	adjustSize();
}

void KBiff::haveNewMail()
{
TRACEINIT("KBiff::haveNewMail()");
	displayPixmap();
	// notify if we must
	if (notify)
	{
		// do notify
	}

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
TRACEINIT("KBiff::dock()");
	// destroy the old window
	if (this->isVisible())
	{
		this->hide();
		this->destroy(true, true);
		this->create(0, true, false);
		kapp->setMainWidget(this);

		// we don't want a "real" top widget if we are _going_ to
		// be docked.
		if (docked)
			kapp->setTopWidget(this);
		else
			kapp->setTopWidget(new QWidget);
	}

	if (docked == false)
	{
		docked = true;

		// enable docking
		KWM::setDockWindow(this->winId());
	}
	else
		docked = false;

	// (un)dock it!
	this->show();
	displayPixmap();
}

void KBiff::setup()
{
TRACEINIT("KBiff::setup()");
	KBiffSetup* setup_dlg = new KBiffSetup(profile);

	if (setup_dlg->exec())
		processSetup(setup_dlg, true);
}

void KBiff::checkMailNow()
{
TRACEINIT("KBiff::checkMailNow()");
	KBiffMonitor *monitor;
	for (monitor = monitorList.first();
	     monitor != 0;
		  monitor = monitorList.next())
	{
		monitor->checkMailNow();
	}
}

void KBiff::stop()
{
TRACEINIT("KBiff::stop()");
	KBiffMonitor *monitor;
	for (monitor = monitorList.first();
	     monitor != 0;
		  monitor = monitorList.next())
	{
		monitor->stop();
	}
}

void KBiff::start()
{
TRACEINIT("KBiff::start()");
	myMUTEX = true;
	KBiffMonitor *monitor;
	for (unsigned int i = 0; i < monitorList.count(); i++)
	{
		TRACE("Starting a monitor");
		monitor = monitorList.at(i);
		monitor->start();
	}
	myMUTEX = false;
	displayPixmap();
}

///////////////////////////////////////////////////////////////////////////
// Protected Functions
///////////////////////////////////////////////////////////////////////////
void KBiff::popupMenu()
{
TRACEINIT("KBiff::popupMenu()");
	QPopupMenu *popup = new QPopupMenu(0, "popup");

	if (docked)
		popup->insertItem(i18n("&UnDock"), this, SLOT(dock()));
	else
		popup->insertItem(i18n("&Dock"), this, SLOT(dock()));
	popup->insertItem(i18n("&Setup..."), this, SLOT(setup()));
	popup->insertSeparator();
	popup->insertItem(i18n("&Help..."), this, SLOT(invokeHelp()));
	popup->insertSeparator();

	int check_id;
	check_id = popup->insertItem(i18n("&Check mail now"), this, SLOT(checkMailNow()));

	if (isRunning())
	{
		popup->setItemEnabled(check_id, true);
		popup->insertItem(i18n("&Stop"), this, SLOT(stop()));
	}
	else
	{
		popup->setItemEnabled(check_id, false);
		popup->insertItem(i18n("&Start"), this, SLOT(start()));
	}

	popup->insertSeparator();
	popup->insertItem(i18n("E&xit"), kapp, SLOT(quit()));

	popup->popup(QCursor::pos());
}

void KBiff::reset()
{
TRACEINIT("KBiff::reset()");
	// reset all the member variables
	systemBeep     = true;
	runCommand     = false;
	runCommandPath = "";
	playSound      = false;
	playSoundPath  = "";
	notify         = false;

	noMailIcon  = "nomail.xpm";
	newMailIcon = "newmail.xpm";
	oldMailIcon = "oldmail.xpm";

	docked    = false;

	mailClient  = "xmutt";

	myMUTEX = false;
}

bool KBiff::isRunning()
{
TRACEINIT("KBiff::isRunning()");
	bool is_running = false;
	KBiffMonitor *monitor;
	for (monitor = monitorList.first();
	     monitor != 0;
		  monitor = monitorList.next())
	{
		if (monitor->isRunning())
		{
			is_running = true;
			break;
		}
	}
	return is_running;
}
