// A brief description of what this file does
// should go here.
// $Id$

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

public slots:
	void invokeHelp();

private:
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
	KBiffGeneralTab(QWidget *parent=0);

	const char* getButtonNewMail();
	const char* getButtonOldMail();
	const char* getButtonNoMail();
	const char* getCommand();
	const int   getPoll();
	const bool  getDock();
	const bool  getSessionManagement();

public slots:
	void setSessionManagement(bool);
	void setButtonNewMail(const char*);
	void setButtonNoMail(const char*);
	void setButtonOldMail(const char*);
	void setCommand(const char*);
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
	KBiffNewMailTab(QWidget *parent=0);
	~KBiffNewMailTab();

	bool getRunCommand();
	const char* getRunCommandPath();
	bool getPlaySound();
	const char* getPlaySoundPath();
	bool getBeep();

public slots:
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

class KBiffMailboxTab : public QWidget
{
	Q_OBJECT
public:
	KBiffMailboxTab(QWidget *parent=0);

	void setMailbox(const KURL& url);
	KURL getMailbox();

private:
	QComboBox *comboProtocol;
	QLineEdit *editMailbox;
	QLineEdit *editServer;
	QLineEdit *editUser;
	QLineEdit *editPassword;
	QCheckBox *checkStorePassword;
};

class KBiffAboutTab : public QWidget
{
	Q_OBJECT
public:
	KBiffAboutTab(QWidget *parent=0);

protected slots:
	void mailTo(const char*);
	void homepage(const char*);
};

#endif // SETUPDLG_H
