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
#include <qlist.h>

#include <kiconloaderdialog.h>
#include <kurl.h>

struct KBiffMailbox
{
	KURL url;
	bool store;
};

class KBiffGeneralTab;
class KBiffNewMailTab;
class KBiffMailboxTab;
class KBiffAboutTab;
class KBiffNewDlg;

class KBiffSetup : public QDialog
{
	Q_OBJECT
public:
	KBiffSetup(const char *name = 0);
	virtual ~KBiffSetup();

	inline const KURL getCurrentMailbox() const;
	inline const QList<KURL> getMailboxList() const;

	inline const char* getMailClient() const;
	inline const char* getRunCommandPath() const;
	inline const char* getPlaySoundPath() const;
	inline const char* getNoMailIcon() const;
	inline const char* getNewMailIcon() const;
	inline const char* getOldMailIcon() const;
	inline const bool getSystemBeep() const;
	inline const bool getRunCommand() const;
	inline const bool getPlaySound() const;
	inline const bool getNotify() const;
	inline const bool getDock() const;
	inline const bool getSessionManagement() const;
	inline const unsigned int getPoll() const;

public slots:
	void invokeHelp();

	void readConfig(const char *profile);
	void saveConfig();

protected slots:
	void slotDone();
	void slotAddNewProfile();
	void slotRenameProfile();
	void slotDeleteProfile();

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

	inline const char* getButtonNewMail() const;
	inline const char* getButtonOldMail() const;
	inline const char* getButtonNoMail() const;
	inline const char* getMailClient() const;
	inline const int   getPoll() const;
	inline const bool  getDock() const;
	inline const bool  getSessionManagement() const;

public slots:
	void readConfig(const char *profile);
	void saveConfig(const char *profile);

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

	inline const bool getRunCommand() const;
	inline const char* getRunCommandPath() const;
	inline const bool getPlaySound() const;
	inline const char* getPlaySoundPath() const;
	inline const bool getSystemBeep() const;
	inline const bool getNotify() const;

public slots:
	void readConfig(const char *profile);
	void saveConfig(const char *profile);

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
	QCheckBox *checkNotify;

	QPushButton *buttonBrowsePlaySound;
	QPushButton *buttonBrowseRunCommand;
};

class KBiffMailboxAdvanced : public QDialog
{
	Q_OBJECT
public:
	KBiffMailboxAdvanced();
	virtual ~KBiffMailboxAdvanced();

	inline const KURL getMailbox() const;
	inline const unsigned int getPort() const;

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

	inline const KURL getMailbox() const;
	inline const QList<KURL> getMailboxList() const;

public slots:
	void readConfig(const char *profile);
	void saveConfig(const char *profile);

protected slots:
	void slotDeleteMailbox();
	void slotNewMailbox();
	void slotMailboxSelected(QListViewItem *item);

	void protocolSelected(int protocol);
	void browse();
	void advanced();

protected:
	inline const char* scramble(const char* password, bool encode = true);
	inline const KURL defaultMailbox() const;

private:
	QDict<KBiffMailbox> *mailboxHash;
	QListViewItem       *oldItem;

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

class KBiffNewDlg : public QDialog
{
	Q_OBJECT
public:
	KBiffNewDlg(QWidget* parent = 0, const char* name = 0);

	const char* getName() const
		{ return editName->text(); }

private:
	QLineEdit *editName;
};

#endif // SETUPDLG_H
