/*
 * kbiff.h
 * Copyright (C) 1998 Kurt Granroth <granroth@kde.org>
 *
 * This file contains the declaration of the main KBiff
 * widget.
 *
 * $Id$
 */
#ifndef KBIFF_H 
#define KBIFF_H 

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif 

#include <qlist.h>

#include <kbiffmonitor.h>
#include <setupdlg.h>

#include <kurl.h>
#include <kapp.h>

// mediatool.h is needed by kaudio.h
extern "C" {
#include <mediatool.h>
} 
#include <kaudio.h>

#include <qlabel.h>

class KBiff : public QLabel
{
	Q_OBJECT
public:
	KBiff(QWidget *parent = 0);
	virtual ~KBiff();

	const bool isDocked() const;

	void processSetup(const KBiffSetup* setup, bool start);
	void readSessionConfig();

	void setMailboxList(const QList<KURL>& mailbox_list, unsigned int poll = 60);

protected:
	void mousePressEvent(QMouseEvent *);

protected:
	void popupMenu();
	void reset();
	bool isRunning();
	bool isGIF8x(const char* file);

protected slots:
	void saveYourself();
	void invokeHelp();
	void displayPixmap();
	void haveNewMail(const int, const char*);
	void dock();
	void setup();
	void checkMailNow();
	void readMailNow();
	void readPop3MailNow();
	void stop();
	void start();

protected:
	bool myMUTEX;
	QList<KBiffMonitor> monitorList;

	// Capability
	bool    hasAudio;
	KAudio  audioServer;
	
	// General settings
	QString profile;
	QString mailClient;

	// New mail
	bool    systemBeep;
	bool    runCommand;
	QString runCommandPath;
	bool    playSound;
	QString playSoundPath;
	bool    notify;

	bool    docked;
	bool    sessions;

	bool    isSecure;

	QString noMailIcon;
	QString newMailIcon;
	QString oldMailIcon;
};

#endif // KBIFF_H 
