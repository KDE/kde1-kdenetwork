/*
 * setupdlg.cpp
 * Copyright (C) 1998 Kurt Granroth <granroth@kde.org>
 *
 * This file contains the implementation of the setup dialog
 * class for KBiff.
 *
 * $Id$
 */
#include "setupdlg.h"
#include "setupdlg.moc"

#include "Trace.h"

#include <qpixmap.h>
#include <qfont.h>
#include <qlabel.h>
#include <qgrpbox.h>
#include <qfileinf.h>

#include <kapp.h>
#include <ktabctl.h>

#include <kfiledialog.h> // KBiffNewMailTab

#include <kurllabel.h> // KBiffAboutTab
#include <kfm.h>       // KBiffAboutTab
#include <kprocess.h>  // KBiffAboutTab
#include <stdlib.h>    // KBiffAboutTab

// macros are a Bad Thing(tm), but layouts are too much of a pain for
// now and I got sick of typing this
#define MIN_HEIGHT QMAX(fontMetrics().lineSpacing() + (fontMetrics().descent() * 3), 25)

///////////////////////////////////////////////////////////////////////////
// KBiffSetup
///////////////////////////////////////////////////////////////////////////
KBiffSetup::KBiffSetup(const char *)
	: QDialog(0, 0, true, 0)
{
TRACEINIT("KBiffSetup::KBiffSetup()");
	int y;

	setCaption(i18n("KBiff Setup"));

	// mailbox groupbox
	QGroupBox* mailbox_groupbox = new QGroupBox(this);
	mailbox_groupbox->setGeometry(10, 10, 315, 100);
	mailbox_groupbox->setTitle(i18n("Profile:"));

	// combo box to hold the profile names
	comboProfile = new QComboBox(false, this);
	comboProfile->setGeometry(25, 30, 285, MIN_HEIGHT);
	connect(comboProfile, SIGNAL(highlighted(int)),
	                      SLOT(slotProfileSelected(int)));
	comboProfile->setSizeLimit(10);

	y = MIN_HEIGHT + 40;

	// add new profile button
	QPushButton* add_button = new QPushButton(this);
	add_button->setGeometry(25, y, 85, 30);
//	connect(add_button, SIGNAL(clicked()), SLOT(m_addNewMailbox()));
add_button->setEnabled(false);
	add_button->setText(i18n("&New..."));

	// rename current profile button
	QPushButton* rename_button = new QPushButton(this);
	rename_button->setGeometry(125, y, 85, 30);
//	connect(rename_button, SIGNAL(clicked()), SLOT(m_renameMailbox()));
rename_button->setEnabled(false);
	rename_button->setText(i18n("&Rename..."));

	// delete current profile button
	QPushButton* delete_button = new QPushButton(this);
	delete_button->setGeometry(225, y, 85, 30);
//	connect(delete_button, SIGNAL(clicked()), SLOT(m_deleteMailbox()));
delete_button->setEnabled(false);
	delete_button->setText(i18n("&Delete"));

	y += 30;

	mailbox_groupbox->resize(315, y);

	// setup the tabs
	KTabCtl *tabctl = new KTabCtl(this);
	generalTab = new KBiffGeneralTab(tabctl);
	newmailTab = new KBiffNewMailTab(tabctl);
	mailboxTab = new KBiffMailboxTab(tabctl);
	aboutTab   = new KBiffAboutTab(tabctl); 

	// add the tabs
	tabctl->addTab(generalTab, i18n("General"));
	tabctl->addTab(newmailTab, i18n("New Mail"));
	tabctl->addTab(mailboxTab, i18n("Mailbox"));
	tabctl->addTab(aboutTab, i18n("About"));

	y += 15;

	tabctl->setGeometry(5, y, 320, 280);

	y += 285;

	// help button
	help = new QPushButton(i18n("Help"), this);
	help->setGeometry(10, y, 75, 30);
	connect(help, SIGNAL(clicked()), SLOT(invokeHelp()));

	// ok button
	ok = new QPushButton(i18n("OK"), this);
	ok->setGeometry(165, y, 75, 30);
	ok->setDefault(true);
	connect(ok, SIGNAL(clicked()), SLOT(slotDone()));

	// cancel button
	cancel = new QPushButton(i18n("Cancel"), this);
	cancel->setGeometry(250, y, 75, 30);
	connect(cancel, SIGNAL(clicked()), SLOT(reject()));

	y += 40;

	resize(330, y);
	setFixedSize(330, y);

	initDefaults();
	setWidgets();
}

KBiffSetup::~KBiffSetup()
{
}

void KBiffSetup::invokeHelp()
{
	kapp->invokeHTMLHelp("kbiff/kbiff.html", "");
}

///////////////////////////////////////////////////////////////////////
// Protected slots
///////////////////////////////////////////////////////////////////////
void KBiffSetup::slotDone()
{
	accept();
}

void KBiffSetup::slotProfileSelected(int /*profile*/)
{
}

///////////////////////////////////////////////////////////////////////
// Protected (non-slot) functions
///////////////////////////////////////////////////////////////////////
void KBiffSetup::initDefaults()
{
TRACEINIT("KBiffSetup::initDefaults()");
	// set the defaults for all the variables
	profile       = "Inbox";
	comboProfile->insertItem(profile);

	poll          = 60;
	mailClient    = (getenv("MAILER")) ? getenv("MAILER") : "kmail";
	dockInPanel   = true;
	useSessions   = true;
	oldmailPixmap = "oldmail.xpm";
	newmailPixmap = "newmail.xpm";
	nomailPixmap  = "nomail.xpm";

	runCommand    = false;
	commandPath   = "";
	playSound     = false;
	soundPath     = "";
	playBeep      = true;

	QFileInfo mailbox_info(getenv("MAIL"));
	if (mailbox_info.exists() == false)
	{
		QString s("/var/spool/mail/");
		s += getlogin();
		mailbox_info.setFile(s);
	}
	mailbox.setProtocol("mbox");
	mailbox.setPath(mailbox_info.absFilePath());
	mailbox.setPort(0);
}

void KBiffSetup::setWidgets()
{
TRACEINIT("KBiffSetup::setWidgets()");
	// General widgets
	generalTab->setPoll(poll);
	generalTab->setMailClient(mailClient);
	generalTab->setDock(dockInPanel);
	generalTab->setSessionManagement(useSessions);
	generalTab->setButtonNewMail(newmailPixmap);
	generalTab->setButtonNoMail(nomailPixmap);
	generalTab->setButtonOldMail(oldmailPixmap);

	// New mail widgets
	newmailTab->setRunCommand(runCommand);
	newmailTab->setRunCommandPath(commandPath);
	newmailTab->setPlaySound(playSound);
	newmailTab->setPlaySoundPath(soundPath);
	newmailTab->setBeep(playBeep);

	// Mailbox
	mailboxTab->setMailbox(mailbox);
}

///////////////////////////////////////////////////////////////////////
// KBiffGeneralTab
///////////////////////////////////////////////////////////////////////
KBiffGeneralTab::KBiffGeneralTab(QWidget *parent)
	: QWidget(parent)
{
TRACEINIT("KBiffGeneralTab::KBiffGeneralTab()");
	// the poll time (in seconds)
	QLabel* poll_label = new QLabel(this);
	poll_label->setGeometry(10, 20, 65, 30);
	poll_label->setText(i18n("Poll (sec):"));
	poll_label->adjustSize();

	// the command to run when clicked
	QLabel* command_label = new QLabel(this);
	command_label->setGeometry(10, 50, 65, 30);
	command_label->setText(i18n("Mail client:"));
	command_label->adjustSize();

	int x = QMAX(poll_label->width() + 15, command_label->width() + 15);

	editPoll = new QLineEdit(this);
	editPoll->setGeometry(x, 15, 60, MIN_HEIGHT);

	editCommand = new QLineEdit(this);
	editCommand->setGeometry(x, 45, 300 - x, MIN_HEIGHT);

	// do we dock automatically?
	checkDock = new QCheckBox(this);
	checkDock->setText(i18n("Dock in panel"));
	checkDock->setGeometry(x, 75, 150, 25);
	checkDock->adjustSize();

	// should we support session management?
	checkNoSession = new QCheckBox(this);
	checkNoSession->setText(i18n("Use session management"));
	checkNoSession->setGeometry(x, 100, 190, 25);
	checkNoSession->adjustSize();

	// group box to hold the icons together
	QGroupBox* icons_groupbox = new QGroupBox(this);
	icons_groupbox->setTitle(i18n("Icons:"));
	icons_groupbox->move(10, 125);

	x = 125 + fontMetrics().lineSpacing();

	// "no mail" pixmap button
	QLabel* nomail_label = new QLabel(this);
	nomail_label->setText(i18n("No Mail:"));
	nomail_label->adjustSize();

	// "old mail" pixmap button
	QLabel* oldmail_label = new QLabel(this);
	oldmail_label->setText(i18n("Old Mail:"));
	oldmail_label->adjustSize();

	// "new mail" pixmap button
	QLabel* newmail_label = new QLabel(this);
	newmail_label->setText(i18n("New Mail:"));
	newmail_label->adjustSize();

	nomail_label->move(25, x);
	oldmail_label->move(158 - (oldmail_label->width()/2),x);
	newmail_label->move(290 - newmail_label->width(),x);

	x += nomail_label->height() + 5;

	buttonNoMail = new KIconLoaderButton(this);
	buttonNoMail->setGeometry(
		nomail_label->x() + (nomail_label->width()/2) - 25,
		x, 50, 50);

	buttonOldMail = new KIconLoaderButton(this);
	buttonOldMail->setGeometry(
		oldmail_label->x() + (oldmail_label->width()/2) - 25,
		x, 50, 50);

	buttonNewMail = new KIconLoaderButton(this);
	buttonNewMail->setGeometry(
		newmail_label->x() + (newmail_label->width()/2) - 25,
		x, 50, 50);

	icons_groupbox->resize(295,
		(fontMetrics().lineSpacing() * 2) + 65);
}

KBiffGeneralTab::~KBiffGeneralTab()
{
}

void KBiffGeneralTab::setSessionManagement(bool enable)
{
	checkNoSession->setChecked(enable);
}

const bool KBiffGeneralTab::getSessionManagement()
{
	return checkNoSession->isChecked();
}

void KBiffGeneralTab::setDock(bool dock)
{
	checkDock->setChecked(dock);
}

const bool KBiffGeneralTab::getDock()
{
	return checkDock->isChecked();
}

void KBiffGeneralTab::setButtonNewMail(const char* new_mail)
{
	buttonNewMail->setIcon(new_mail);
	buttonNewMail->setPixmap(kapp->getIconLoader()->loadIcon(new_mail));
}

void KBiffGeneralTab::setButtonOldMail(const char* old_mail)
{
	buttonOldMail->setIcon(old_mail);
	buttonOldMail->setPixmap(kapp->getIconLoader()->loadIcon(old_mail));
}

void KBiffGeneralTab::setButtonNoMail(const char* no_mail)
{
	buttonNoMail->setIcon(no_mail);
	buttonNoMail->setPixmap(kapp->getIconLoader()->loadIcon(no_mail));
}

void KBiffGeneralTab::setPoll(int poll)
{
	editPoll->setText(QString().setNum(poll));
}

void KBiffGeneralTab::setMailClient(const char* client)
{
	editCommand->setText(client);
}

const char* KBiffGeneralTab::getButtonOldMail()
{
	return buttonOldMail->icon();
}

const char* KBiffGeneralTab::getButtonNewMail()
{
	return buttonNewMail->icon();
}

const char* KBiffGeneralTab::getButtonNoMail()
{
	return buttonNoMail->icon();
}

const char* KBiffGeneralTab::getMailClient()
{
	return editCommand->text();
}

const int KBiffGeneralTab::getPoll()
{
	return QString(editPoll->text()).toInt();
}
///////////////////////////////////////////////////////////////////////
// KBiffNewMailTab
///////////////////////////////////////////////////////////////////////
KBiffNewMailTab::KBiffNewMailTab(QWidget *parent)
	: QWidget(parent)
{
TRACEINIT("KBiffNewMailTab::KBiffNewMailTab()");
	// setup the Run Command stuff
	checkRunCommand = new QCheckBox(this);
	checkRunCommand->setText(i18n("Run Command"));
	checkRunCommand->move(10, 10);
	checkRunCommand->adjustSize();

	editRunCommand = new QLineEdit(this);
	editRunCommand->setGeometry(10, 30, 215, MIN_HEIGHT);

	buttonBrowseRunCommand = new QPushButton(this);
	buttonBrowseRunCommand->setText(i18n("Browse"));
	buttonBrowseRunCommand->setGeometry(235, 30, 70, MIN_HEIGHT);

	// setup the Play Sound stuff
	checkPlaySound = new QCheckBox(this);
	checkPlaySound->setText(i18n("Play Sound"));
	checkPlaySound->move(10, 75);
	checkPlaySound->adjustSize();

	editPlaySound = new QLineEdit(this);
	editPlaySound->setGeometry(10, 95, 215, MIN_HEIGHT);

	buttonBrowsePlaySound = new QPushButton(this);
	buttonBrowsePlaySound->setText(i18n("Browse"));
	buttonBrowsePlaySound->setGeometry(235, 95, 70, MIN_HEIGHT);

	// setup the System Sound stuff
	checkBeep = new QCheckBox(this);
	checkBeep->setText(i18n("System Beep"));
	checkBeep->move(10, 140);
	checkBeep->adjustSize();

	// connect some slots and signals
	connect(buttonBrowsePlaySound, SIGNAL(clicked()),
	                                 SLOT(browsePlaySound()));
	connect(buttonBrowseRunCommand, SIGNAL(clicked()),
	                                  SLOT(browseRunCommand()));
	connect(checkPlaySound, SIGNAL(toggled(bool)),
	                          SLOT(enablePlaySound(bool)));
	connect(checkRunCommand, SIGNAL(toggled(bool)),
	                           SLOT(enableRunCommand(bool)));
}

KBiffNewMailTab::~KBiffNewMailTab()
{
}

bool KBiffNewMailTab::getRunCommand()
{
	return checkRunCommand->isChecked();
}

const char* KBiffNewMailTab::getRunCommandPath()
{
	return editRunCommand->text();
}

bool KBiffNewMailTab::getPlaySound()
{
	return checkPlaySound->isChecked();
}

const char* KBiffNewMailTab::getPlaySoundPath()
{
	return editPlaySound->text();
}

bool KBiffNewMailTab::getBeep()
{
	return checkBeep->isChecked();
}

void KBiffNewMailTab::setRunCommand(bool run)
{
	checkRunCommand->setChecked(run);
	enableRunCommand(run);
}

void KBiffNewMailTab::setRunCommandPath(const char* path)
{
	editRunCommand->setText(path);
}

void KBiffNewMailTab::setPlaySound(bool play)
{
	checkPlaySound->setChecked(play);
	enablePlaySound(play);
}

void KBiffNewMailTab::setPlaySoundPath(const char* path)
{
	editPlaySound->setText(path);
}

void KBiffNewMailTab::setBeep(bool beep)
{
	checkBeep->setChecked(beep);
}

void KBiffNewMailTab::enableRunCommand(bool enable)
{
	editRunCommand->setEnabled(enable);
	buttonBrowseRunCommand->setEnabled(enable);
}

void KBiffNewMailTab::enablePlaySound(bool enable)
{
	editPlaySound->setEnabled(enable);
	buttonBrowsePlaySound->setEnabled(enable);
}

void KBiffNewMailTab::browseRunCommand()
{
  QString command_path = KFileDialog::getOpenFileName();
  if (!command_path.isEmpty() && !command_path.isNull())
  {
    setRunCommandPath(command_path);
  }
}

void KBiffNewMailTab::browsePlaySound()
{
  QString sound_path = KFileDialog::getOpenFileName();
  if (!sound_path.isEmpty() && !sound_path.isNull())
  {
    setPlaySoundPath(sound_path);
  }
}

//////////////////////////////////////////////////////////////////////
// KBiffMailboxTab
//////////////////////////////////////////////////////////////////////
KBiffMailboxAdvanced::KBiffMailboxAdvanced()
	: QDialog(0, 0, true, 0)
{
	int y = 15;

	setCaption(i18n("Advanced Options"));
	QLabel *mailbox_label = new QLabel(this);
	mailbox_label->setText(i18n("Mailbox URL:"));
	mailbox_label->move(15, y+5);
	mailbox_label->adjustSize();

	mailbox = new QLineEdit(this);
	mailbox->move(mailbox_label->width() + 20, y);
	mailbox->resize(370 - mailbox_label->width(), MIN_HEIGHT);

	y += MIN_HEIGHT + 5;

	QLabel *port_label = new QLabel(this);
	port_label->setText(i18n("Port:"));
	port_label->move(15, y+5);
	port_label->adjustSize();

	port = new QLineEdit(this);
	port->move(mailbox_label->width() + 20, y);
	port->resize(100, MIN_HEIGHT);

	connect(port, SIGNAL(textChanged(const char*)),
	              SLOT(portModified(const char*)));

	y += MIN_HEIGHT + 5;

	QPushButton *ok = new QPushButton(this);
	ok->setText(i18n("OK"));
	ok->setGeometry(235, y, 75, MIN_HEIGHT);
	ok->setDefault(true);

	connect(ok, SIGNAL(clicked()), SLOT(accept()));

	QPushButton *cancel = new QPushButton(this);
	cancel->setText(i18n("Cancel"));
	cancel->setGeometry(315, y, 75, MIN_HEIGHT);
	connect(cancel, SIGNAL(clicked()), SLOT(reject()));

	resize(400, 110);
}

KBiffMailboxAdvanced::~KBiffMailboxAdvanced()
{
}

KURL KBiffMailboxAdvanced::getMailbox() const
{
	KURL url(mailbox->text());
	url.setPassword(password);
	return url;
}

unsigned int KBiffMailboxAdvanced::getPort() const
{
	return QString(port->text()).toInt();
}

void KBiffMailboxAdvanced::setMailbox(const KURL& url)
{
	password = url.passwd();
	KURL new_url(url);
	new_url.setPassword("");
	mailbox->setText(new_url.url());
}

void KBiffMailboxAdvanced::setPort(unsigned int the_port, bool enable)
{
	port->setEnabled(enable);
	port->setText(QString().setNum(the_port));
}

void KBiffMailboxAdvanced::portModified(const char *text)
{
	KURL url = getMailbox();
	url.setPort(QString(text).toInt());
	setMailbox(url);
}

KBiffMailboxTab::KBiffMailboxTab(QWidget *parent)
	: QWidget(parent)
{
TRACEINIT("KBiffMailboxTab::KBiffMailboxTab()");
	buttonBrowse = new QPushButton(this);
	buttonBrowse->setText(i18n("Browse"));
	buttonBrowse->setGeometry(230, 15, 70, 25);
	connect(buttonBrowse, SIGNAL(clicked()), SLOT(browse()));

	QLabel* protocol_label = new QLabel(this);
	protocol_label->setText(i18n("Protocol:"));
	protocol_label->move(15, 20);
	protocol_label->adjustSize();

	QLabel* mailbox_label = new QLabel(this);
	mailbox_label->setText(i18n("Mailbox:"));
	mailbox_label->move(15, 50);
	mailbox_label->adjustSize();

	QLabel* server_label = new QLabel(this);
	server_label->setText(i18n("Server:"));
	server_label->move(15, 100);
	server_label->adjustSize();

	QLabel* user_label = new QLabel(this);
	user_label->setText(i18n("User:"));
	user_label->move(15, 130);
	user_label->adjustSize();

	QLabel* password_label = new QLabel(this);
	password_label->setText(i18n("Password:"));
	password_label->move(15, 160);
	password_label->adjustSize();

	int x = QMAX(protocol_label->width() + 20, 
	             QMAX(mailbox_label->width() + 20,
					 QMAX(server_label->width() + 20,
					 QMAX(password_label->width() + 20, user_label->width() + 20))));

	comboProtocol = new QComboBox(this);
	comboProtocol->setGeometry(x, 15, 100, MIN_HEIGHT);
	comboProtocol->insertItem("");
	comboProtocol->insertItem("mbox");
	comboProtocol->insertItem("maildir");
	comboProtocol->insertItem("imap4");
	comboProtocol->insertItem("pop3");

	connect(comboProtocol, SIGNAL(highlighted(int)),
	                       SLOT(protocolSelected(int)));

	editMailbox = new QLineEdit(this);
	editMailbox->setGeometry(x, 45, 300 - x, MIN_HEIGHT);

	editServer = new QLineEdit(this);
	editServer->setGeometry(x, 95, 300 - x, MIN_HEIGHT);

	editUser = new QLineEdit(this);
	editUser->setGeometry(x, 125, 300 - x, MIN_HEIGHT);

	editPassword = new QLineEdit(this);
	editPassword->setGeometry(x, 155, 300 - x, MIN_HEIGHT);
	editPassword->setEchoMode(QLineEdit::Password);

	checkStorePassword = new QCheckBox(this);
	checkStorePassword->setGeometry(x, 185, 220, MIN_HEIGHT);
	checkStorePassword->setText(i18n("Store password"));
	checkStorePassword->adjustSize();

	QPushButton *advanced_bt = new QPushButton(this);
	advanced_bt->setText(i18n("Advanced"));
	advanced_bt->resize(QMAX(80, advanced_bt->sizeHint().width()), MIN_HEIGHT);
	advanced_bt->move(300 - advanced_bt->width(), 210);
	connect(advanced_bt, SIGNAL(clicked()), SLOT(advanced()));

	protocolSelected(0);
}

KBiffMailboxTab::~KBiffMailboxTab()
{
}

void KBiffMailboxTab::setMailbox(const KURL& url)
{
TRACEINIT("KBiffMailboxTab::setMailbox()");
	QString prot(url.protocol());

	if (prot == "mbox")
		protocolSelected(1);
	else if (prot == "maildir")
		protocolSelected(2);
	else if (prot == "imap4")
		protocolSelected(3);
	else if (prot == "pop3")
		protocolSelected(4);
	else
		return;

	if (editMailbox->isEnabled())
	{
		QString path(url.path());
		if (prot == "imap4" && !path.isEmpty() && path[0] == '/')
			path.remove(0, 1);
		editMailbox->setText(path);
	}

TRACEF("url.port() = %d", url.port());
	port = url.port();

	if (editServer->isEnabled())
		editServer->setText(url.host());
	if (editUser->isEnabled())
		editUser->setText(url.user());
	if (editPassword->isEnabled())
		editPassword->setText(url.passwd());
}

KURL KBiffMailboxTab::getMailbox() const
{
	KURL url;

	url.setProtocol(comboProtocol->currentText());

	if (editUser->isEnabled())
		url.setUser(editUser->text());

	if (editPassword->isEnabled())
		url.setPassword(editPassword->text());

	if (editServer->isEnabled())
		url.setHost(editServer->text());

	if (port != 0)
		url.setPort(port);

	if (editMailbox->isEnabled())
	{
		QString path(editMailbox->text());
		if (!path.isEmpty() && path[0] != '/')
			path.prepend("/");
		url.setPath(path);
	}
	return url;
}

void KBiffMailboxTab::protocolSelected(int protocol)
{
TRACEINIT("KBiffMailboxTab::protocolSelected()");
	comboProtocol->setCurrentItem(protocol);

	switch (protocol)
	{
		case 1: // mbox
		case 2: // maildir
			port = 0;
			buttonBrowse->setEnabled(true);
			editMailbox->setEnabled(true);
			editServer->setEnabled(false);
			editUser->setEnabled(false);
			editPassword->setEnabled(false);
			checkStorePassword->setEnabled(false);
			break;
		case 3: // IMAP4
			port = 143;
			editMailbox->setEnabled(true);
			buttonBrowse->setEnabled(false);
			editServer->setEnabled(true);
			editUser->setEnabled(true);
			editPassword->setEnabled(true);
			checkStorePassword->setEnabled(true);
			break;
		case 4: // POP3
			port = 110;
			editMailbox->setEnabled(false);
			buttonBrowse->setEnabled(false);
			editServer->setEnabled(true);
			editUser->setEnabled(true);
			editPassword->setEnabled(true);
			checkStorePassword->setEnabled(true);
			break;
		default: // blank
			port = 0;
			editMailbox->setEnabled(false);
			buttonBrowse->setEnabled(false);
			editServer->setEnabled(false);
			editUser->setEnabled(false);
			editPassword->setEnabled(false);
			checkStorePassword->setEnabled(false);
			break;
	}
}

void KBiffMailboxTab::browse()
{
	QString path = KFileDialog::getOpenFileName();
	if (!path.isEmpty() && !path.isNull())
	{
		editMailbox->setText(path);
	}
}

void KBiffMailboxTab::advanced()
{
	KBiffMailboxAdvanced advanced_dlg;
	QString prot(getMailbox().protocol());
	
	if (prot == "mbox" || prot == "maildir")
		advanced_dlg.setPort(port, false);
	else
		advanced_dlg.setPort(port);

	advanced_dlg.setMailbox(getMailbox());
	if (advanced_dlg.exec())
	{
		port = advanced_dlg.getPort();
		setMailbox(advanced_dlg.getMailbox());
	}
}

//////////////////////////////////////////////////////////////////////
// KBiffAboutTab
//////////////////////////////////////////////////////////////////////
KBiffAboutTab::KBiffAboutTab(QWidget *parent)
	: QWidget(parent)
{
TRACEINIT("KBiffAboutTab::KBiffAboutTab()");
	int x, y;

	// load in the kbiff pixmap
	QPixmap logo_pixmap(KApplication::kde_icondir() + '/' + "kbiff.xpm");

	KURLLabel *logo = new KURLLabel(this);
	logo->setPixmap(logo_pixmap);
	logo->move(10,10);
	logo->setFont(QFont("helvetica", 24, QFont::Bold));
	logo->setURL("http://www.pobox.com/~kurt_granroth/kbiff");
	logo->setText(i18n("KBiff"));
	logo->setTextAlignment(Right);
	logo->setUnderline(false);
	logo->setGlow(false);
	logo->setFloat(true);
	logo->setAutoResize(true);
	logo->adjustSize();

	connect(logo, SIGNAL(leftClickedURL(const char*)),
	                 SLOT(homepage(const char*)));

	x = logo_pixmap.width() + 15;
	y = 10;

	QFontMetrics big_fm(QFont("helvetica", 24, QFont::Bold));
	QFontMetrics small_fm(QFont("helvetica", 12));

	y += big_fm.lineSpacing();

	QLabel *version = new QLabel(this);
	version->setFont(QFont("helvetica", 12));
	version->setText(i18n("Version 1.1\n\nCopyright (C) 1998\nKurt Granroth"));
	version->setAutoResize(true);
	version->move(x, y);
	version->adjustSize();
	y += version->height();

	KURLLabel *email = new KURLLabel(this);
	email->setText("granroth@kde.org");
	email->setURL("granroth@kde.org");
	email->setFont(QFont("helvetica", 12));
	email->setUnderline(false);
	email->setGlow(false);
	email->setFloat();
	email->move(x, y);
	email->setAutoResize(true);
	email->adjustSize();
	connect(email, SIGNAL(leftClickedURL(const char*)),
	                 SLOT(mailTo(const char*)));
}

KBiffAboutTab::~KBiffAboutTab()
{
}

void KBiffAboutTab::mailTo(const char* url)
{
	// this kludge is necessary until KFM handles mailto: urls
	char *mailer = getenv("MAILER");
	if (mailer)
	{
		KProcess mailto;
		mailto << mailer << "-s \"KBiff\"" << url;
		mailto.start(KProcess::DontCare);
	}
}

void KBiffAboutTab::homepage(const char* url)
{
	KFM kfm;
	kfm.openURL(url);
}
