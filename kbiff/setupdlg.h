/*
 * setupdlg.h
 * Copyright (C) 1998 Kurt Granroth <granroth@kde.org>
 *
 * This file contains the setup dialog and related widgets
 * for KBiff.  All user configuration is done here.
 *
 * $Id$
 */
#ifndef SETUPDLG_H
#define SETUPDLG_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif //HAVE_CONFIG_H

#include <qwidget.h>
#include <qdialog.h>
#include <qlineedit.h>
#include <qcheckbox.h>
#include <qpushbutton.h>
#include <qcombobox.h>
#include <qlistview.h>
#include <qdict.h>

#include <kiconloaderdialog.h>
#include <kurl.h>

class KBiffGeneralTab;
class KBiffNewMailTab;
class KBiffMailboxTab;
class KBiffAboutTab;

class KBiffSetup : public QDialog
{
	Q_OBJECT
public:
	KBiffSetup(const char *name = 0);
	virtual ~KBiffSetup();

	KURL getMailbox() const;

public slots:
	void invokeHelp();

	void readConfig(const char *profile);
	void saveConfig();

protected slots:
	void slotDone();

private:
	// "outer" dialog
	QComboBox   *comboProfile;
	QPushButton *help;
	QPushButton *ok;
	QPushButton *cancel;

	// tabs
	KBiffGeneralTab *generalTab;
	KBiffNewMailTab *newmailTab;
	KBiffMailboxTab *mailboxTab;
	KBiffAboutTab   *aboutTab;
};

class KBiffGeneralTab : public QWidget
{
	Q_OBJECT
public:
	KBiffGeneralTab(const char* profile = 0, QWidget *parent=0);
	virtual ~KBiffGeneralTab();

	const char* getButtonNewMail();
	const char* getButtonOldMail();
	const char* getButtonNoMail();
	const char* getMailClient();
	const int   getPoll();
	const bool  getDock();
	const bool  getSessionManagement();

public slots:
	void readConfig(const char *profile);
	void saveConfig(const char *profile);

	void setSessionManagement(bool);
	void setButtonNewMail(const char*);
	void setButtonNoMail(const char*);
	void setButtonOldMail(const char*);
	void setMailClient(const char*);
	void setPoll(int);
	void setDock(bool);

private:
	QLineEdit* editPoll;
	QLineEdit* editCommand;
	QCheckBox* checkDock;
	QCheckBox* checkNoSession;

	KIconLoaderButton *buttonNoMail;
	KIconLoaderButton *buttonOldMail;
	KIconLoaderButton *buttonNewMail;
};

class KBiffNewMailTab : public QWidget
{
	Q_OBJECT
public:
	KBiffNewMailTab(const char* profile = 0, QWidget *parent=0);
	virtual ~KBiffNewMailTab();

	bool getRunCommand();
	const char* getRunCommandPath();
	bool getPlaySound();
	const char* getPlaySoundPath();
	bool getBeep();

public slots:
	void readConfig(const char *profile);
	void saveConfig(const char *profile);

	void setRunCommand(bool);
	void setRunCommandPath(const char*);
	void setPlaySound(bool);
	void setPlaySoundPath(const char*);
	void setBeep(bool);
		
protected slots:
	void enableRunCommand(bool);
	void enablePlaySound(bool);
	void browseRunCommand();
	void browsePlaySound();

private:
	QLineEdit *editRunCommand;
	QLineEdit *editPlaySound;

	QCheckBox *checkRunCommand;
	QCheckBox *checkPlaySound;
	QCheckBox *checkBeep;

	QPushButton *buttonBrowsePlaySound;
	QPushButton *buttonBrowseRunCommand;
};

class KBiffMailboxAdvanced : public QDialog
{
	Q_OBJECT
public:
	KBiffMailboxAdvanced();
	virtual ~KBiffMailboxAdvanced();

	KURL getMailbox() const;
	unsigned int getPort() const;
	void setPort(unsigned int the_port, bool enable = true);
	void setMailbox(const KURL& url);

protected slots:
	void portModified(const char* text);

private:
	QString    password;
	QLineEdit *mailbox;
	QLineEdit *port;
};

class KBiffMailboxTab : public QWidget
{
	Q_OBJECT
public:
	KBiffMailboxTab(const char* profile = 0, QWidget *parent=0);
	virtual ~KBiffMailboxTab();

	void setMailbox(const KURL& url);
	KURL getMailbox() const;

public slots:
	void readConfig(const char *profile);
	void saveConfig(const char *profile);

protected slots:
	void slotMailboxSelected(QListViewItem *item);

	void protocolSelected(int protocol);
	void browse();
	void advanced();

private:
	QDict<char> mailboxHash;

	unsigned int port;
	QComboBox   *comboProtocol;
	QLineEdit   *editMailbox;
	QLineEdit   *editServer;
	QLineEdit   *editUser;
	QLineEdit   *editPassword;
	QCheckBox   *checkStorePassword;
	QPushButton *buttonBrowse;
	QListView   *mailboxes;
};

class KBiffAboutTab : public QWidget
{
	Q_OBJECT
public:
	KBiffAboutTab(QWidget *parent=0);
	virtual ~KBiffAboutTab();

protected slots:
	void mailTo(const char*);
	void homepage(const char*);
};

#endif // SETUPDLG_H
