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
#include <qgroupbox.h>
#include <qfileinfo.h>
#include <qstrlist.h>
#include <qlayout.h>

#include <kapp.h>
#include <ktabctl.h>
#include <ksimpleconfig.h>

#include <qlistview.h> // KBiffMailboxTab

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
KBiffSetup::KBiffSetup(const char *profile)
	: QDialog(0, 0, true, 0)
{
TRACEINIT("KBiffSetup::KBiffSetup()");

	// make sure the profile is *something*
	if (!profile)
		profile = "Inbox";

	setCaption(i18n("KBiff Setup"));

	// mailbox groupbox
	QGroupBox* profile_groupbox = new QGroupBox(i18n("Profile:"), this);

	QBoxLayout *profile_layout = new QBoxLayout(profile_groupbox,
	                                            QBoxLayout::Down, 12, 5);
	profile_layout->addSpacing(8);

	// combo box to hold the profile names
	comboProfile = new QComboBox(false, profile_groupbox);
	comboProfile->setSizeLimit(10);
	comboProfile->setFixedHeight(comboProfile->sizeHint().height());
	profile_layout->addWidget(comboProfile);

	QBoxLayout *button_layout = new QBoxLayout(QBoxLayout::LeftToRight, 10);
	profile_layout->addLayout(button_layout);

	// add new profile button
	QPushButton *add_button = new QPushButton(i18n("&New..."), profile_groupbox);
	add_button->setFixedHeight(add_button->sizeHint().height());
//	connect(add_button, SIGNAL(clicked()), SLOT(m_addNewMailbox()));
	button_layout->addWidget(add_button);
	button_layout->addSpacing(10);

	// rename current profile button
	QPushButton *rename_button = new QPushButton(i18n("&Rename..."),
	                                             profile_groupbox);
	rename_button->setFixedHeight(rename_button->sizeHint().height());
//	connect(rename_button, SIGNAL(clicked()), SLOT(m_renameMailbox()));
	button_layout->addWidget(rename_button);
	button_layout->addSpacing(10);

	// delete current profile button
	QPushButton *delete_button = new QPushButton(i18n("&Delete"),
	                                             profile_groupbox);
	delete_button->setFixedHeight(delete_button->sizeHint().height());
//	connect(delete_button, SIGNAL(clicked()), SLOT(m_deleteMailbox()));
	button_layout->addWidget(delete_button);

	//  add the profile name stuff into the main layout
	QBoxLayout *main_layout = new QBoxLayout(this, QBoxLayout::Down);
	main_layout->addWidget(profile_groupbox, 1);

	// setup the tabs
	KTabCtl *tabctl = new KTabCtl(this);
	generalTab = new KBiffGeneralTab(profile, tabctl);
	newmailTab = new KBiffNewMailTab(profile, tabctl);
	mailboxTab = new KBiffMailboxTab(profile, tabctl);
	aboutTab   = new KBiffAboutTab(tabctl); 

	connect(comboProfile, SIGNAL(highlighted(const char *)),
	        generalTab, SLOT(readConfig(const char *)));
	connect(comboProfile, SIGNAL(highlighted(const char *)),
	        newmailTab, SLOT(readConfig(const char *)));
	connect(comboProfile, SIGNAL(highlighted(const char *)),
	        mailboxTab, SLOT(readConfig(const char *)));

	// add the tabs
	tabctl->addTab(generalTab, i18n("General"));
	tabctl->addTab(newmailTab, i18n("New Mail"));
	tabctl->addTab(mailboxTab, i18n("Mailbox"));
	tabctl->addTab(aboutTab, i18n("About"));

	// add the tab control to the main layout
	main_layout->addWidget(tabctl, 2);

	QBoxLayout *bottom_layout = new QBoxLayout(QBoxLayout::LeftToRight, 10);

	// and add the bottom buttons to the main layout
	main_layout->addLayout(bottom_layout, 1);
	main_layout->activate();

	// help button
	help = new QPushButton(i18n("Help"), this);
	help->setMinimumSize(help->sizeHint());
	connect(help, SIGNAL(clicked()), SLOT(invokeHelp()));
	bottom_layout->addWidget(help);
	bottom_layout->addSpacing(50);

	// ok button
	ok = new QPushButton(i18n("OK"), this);
	ok->setDefault(true);
	ok->setMinimumSize(ok->sizeHint());
	connect(ok, SIGNAL(clicked()), SLOT(slotDone()));
	bottom_layout->addWidget(ok);

	// cancel button
	cancel = new QPushButton(i18n("Cancel"), this);
	cancel->setMinimumSize(cancel->sizeHint());
	connect(cancel, SIGNAL(clicked()), SLOT(reject()));
	bottom_layout->addWidget(cancel);

	resize(350, 400);
//	setFixedSize(330, y);
//	initDefaults();
//	setWidgets();
	readConfig(profile);
}

KBiffSetup::~KBiffSetup()
{
}

KURL KBiffSetup::getMailbox() const
{
	return mailboxTab->getMailbox();
}

void KBiffSetup::invokeHelp()
{
	kapp->invokeHTMLHelp("kbiff/kbiff.html", "");
}

void KBiffSetup::readConfig(const char* profile)
{
	QStrList profile_list;

	// open the config file
	QString config_file(KApplication::localconfigdir());
	config_file += "/kbiff2rc";
	KSimpleConfig *config = new KSimpleConfig(config_file);

	config->setGroup("General");

	// read in the mailboxes
	int number_of_mailboxes = config->readListEntry("Profiles", profile_list);
	delete config;

	// check if we have any mailboxes to read in
	if (number_of_mailboxes > 0)
	{
		// load up the combo box
		comboProfile->insertStrList(&profile_list);

		// read in the data from the first mailbox if we don't have a name
		for (int i = 0; i < comboProfile->count(); i++)
		{
			if (QString(profile) == comboProfile->text(i))
			{
				comboProfile->setCurrentItem(i);
				break;
			}
		}
	}
	else
		comboProfile->insertItem(profile);
}

void KBiffSetup::saveConfig()
{
	// open the config file for writing
	QString config_file(KApplication::localconfigdir());
	config_file += "/kbiff2rc";
	KSimpleConfig *config = new KSimpleConfig(config_file);

	config->setGroup("General");

	// get the list of profiles
	QStrList profile_list;
	for (int i = 0; i < comboProfile->count(); i++)
		profile_list.append(comboProfile->text(i));

	// write the mailboxes
	config->writeEntry("Profiles", profile_list);

	delete config;
}

///////////////////////////////////////////////////////////////////////
// Protected slots
///////////////////////////////////////////////////////////////////////
void KBiffSetup::slotDone()
{
TRACEINIT("KBiffSetup::slotDone()");
	QString profile = comboProfile->currentText();
	saveConfig();
	generalTab->saveConfig(profile);
	newmailTab->saveConfig(profile);
	mailboxTab->saveConfig(profile);
	accept();
}

///////////////////////////////////////////////////////////////////////
// Protected (non-slot) functions
///////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////
// KBiffGeneralTab
///////////////////////////////////////////////////////////////////////
KBiffGeneralTab::KBiffGeneralTab(const char* profile, QWidget *parent)
	: QWidget(parent)
{
TRACEINIT("KBiffGeneralTab::KBiffGeneralTab()");
TRACEF("profile = %s", profile);
	QGridLayout *top_grid = new QGridLayout(this, 5, 3, 12, 5);

	// the poll time (in seconds)
	QLabel* poll_label = new QLabel(i18n("Poll (sec):"), this);
	poll_label->setMinimumSize(poll_label->sizeHint());
	top_grid->addWidget(poll_label, 0, 0);

	// the command to run when clicked
	QLabel *mail_label = new QLabel(i18n("Mail client:"), this);
	mail_label->setMinimumSize(mail_label->sizeHint());
	top_grid->addWidget(mail_label, 1, 0);

	editPoll = new QLineEdit(this);
	editPoll->setMinimumSize(editPoll->sizeHint());
	top_grid->addWidget(editPoll, 0, 1);

	editCommand = new QLineEdit(this);
	editCommand->setMinimumSize(editCommand->sizeHint());
	top_grid->addMultiCellWidget(editCommand, 1, 1, 1, 2);

	// do we dock automatically?
	checkDock = new QCheckBox(i18n("Dock in panel"), this);
	checkDock->setMinimumSize(checkDock->sizeHint());
	top_grid->addMultiCellWidget(checkDock, 2, 2, 1, 2);

	// should we support session management?
	checkNoSession = new QCheckBox(i18n("Use session management"), this);
	checkNoSession->setMinimumSize(checkNoSession->sizeHint());
	top_grid->addMultiCellWidget(checkNoSession, 3, 3, 1, 2);

	// group box to hold the icons together
	QGroupBox* icons_groupbox = new QGroupBox(i18n("Icons:"), this);
	top_grid->addMultiCellWidget(icons_groupbox, 4, 4, 0, 2);
	top_grid->setRowStretch(4, 1);

	// layout to hold the icons inside the groupbox
	QGridLayout *icon_layout = new QGridLayout(icons_groupbox, 3, 3, 12, 5);
	icon_layout->addRowSpacing(0, 8);

	// "no mail" pixmap button
	QLabel* nomail_label = new QLabel(i18n("No Mail:"), icons_groupbox);
	nomail_label->setMinimumSize(nomail_label->sizeHint());
	icon_layout->addWidget(nomail_label, 1, 0);

	// "old mail" pixmap button
	QLabel* oldmail_label = new QLabel(i18n("Old Mail"), icons_groupbox);
	oldmail_label->setMinimumSize(oldmail_label->sizeHint());
	icon_layout->addWidget(oldmail_label, 1, 1);

	// "new mail" pixmap button
	QLabel* newmail_label = new QLabel(i18n("New Mail:"), icons_groupbox);
	newmail_label->setMinimumSize(newmail_label->sizeHint());
	icon_layout->addWidget(newmail_label, 1, 2);

	buttonNoMail = new KIconLoaderButton(icons_groupbox);
	buttonNoMail->setFixedSize(50, 50);
	icon_layout->addWidget(buttonNoMail, 2, 0);

	buttonOldMail = new KIconLoaderButton(icons_groupbox);
	buttonOldMail->setFixedSize(50, 50);
	icon_layout->addWidget(buttonOldMail, 2, 1);

	buttonNewMail = new KIconLoaderButton(icons_groupbox);
	buttonNewMail->setFixedSize(50, 50);
	icon_layout->addWidget(buttonNewMail, 2, 2);

	top_grid->activate();

	readConfig(profile);
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

void KBiffGeneralTab::readConfig(const char* profile)
{
TRACEINIT("KBiffGeneralTab::readConfig()");
	// open the config file
	QString config_file(KApplication::localconfigdir());
	config_file += "/kbiff2rc";
	KSimpleConfig *config = new KSimpleConfig(config_file);

	config->setGroup(profile);

	editPoll->setText(config->readEntry("Poll", "60"));
	editCommand->setText(config->readEntry("MailClient", "kmail"));
	checkDock->setChecked(config->readBoolEntry("Docked", true));
	checkNoSession->setChecked(config->readBoolEntry("Sessions", true));

	QString no_mail, old_mail, new_mail;
	no_mail = config->readEntry("NoMailPixmap", "nomail.xpm");
	old_mail = config->readEntry("OldMailPixmap", "oldmail.xpm");
	new_mail = config->readEntry("NewMailPixmap", "newmail.xpm");

	buttonOldMail->setIcon(old_mail);
	buttonOldMail->setPixmap(kapp->getIconLoader()->loadIcon(old_mail));
	buttonNewMail->setIcon(new_mail);
	buttonNewMail->setPixmap(kapp->getIconLoader()->loadIcon(new_mail));
	buttonNoMail->setIcon(no_mail);
	buttonNoMail->setPixmap(kapp->getIconLoader()->loadIcon(no_mail));

	delete config;
}

void KBiffGeneralTab::saveConfig(const char *profile)
{
	// open the config file for writing
	QString config_file(KApplication::localconfigdir());
	config_file += "/kbiff2rc";
	KSimpleConfig *config = new KSimpleConfig(config_file);

	config->setGroup(profile);

	config->writeEntry("Poll", editPoll->text());
	config->writeEntry("MailClient", editCommand->text());
	config->writeEntry("Docked", checkDock->isChecked());
	config->writeEntry("Sessions", checkNoSession->isChecked());
	config->writeEntry("NoMailPixmap", buttonNoMail->icon());
	config->writeEntry("NewMailPixmap", buttonNewMail->icon());
	config->writeEntry("OldMailPixmap", buttonOldMail->icon());
	delete config;
}

///////////////////////////////////////////////////////////////////////
// KBiffNewMailTab
///////////////////////////////////////////////////////////////////////
KBiffNewMailTab::KBiffNewMailTab(const char* profile, QWidget *parent)
	: QWidget(parent)
{
TRACEINIT("KBiffNewMailTab::KBiffNewMailTab()");
	QGridLayout *top_layout = new QGridLayout(this, 7, 3, 12, 0);

	top_layout->setColStretch(0, 1);
	top_layout->setColStretch(1, 0);
	top_layout->setColStretch(2, 0);

	top_layout->setRowStretch(0, 0);
	top_layout->setRowStretch(1, 0);
	top_layout->setRowStretch(2, 0);
	top_layout->setRowStretch(3, 0);
	top_layout->setRowStretch(4, 0);
	top_layout->setRowStretch(5, 0);
	top_layout->setRowStretch(6, 0);

	// setup the Run Command stuff
	checkRunCommand = new QCheckBox(i18n("Run Command"), this);
	checkRunCommand->setMinimumSize(checkRunCommand->sizeHint());
	top_layout->addWidget(checkRunCommand, 0, 0);

	editRunCommand = new QLineEdit(this);
	top_layout->addWidget(editRunCommand, 1, 0);

	top_layout->addColSpacing(1, 12);

	buttonBrowseRunCommand = new QPushButton(i18n("Browse"), this);
	buttonBrowseRunCommand->setMinimumSize(75, 25);
	top_layout->addWidget(buttonBrowseRunCommand, 1, 2);

	top_layout->addRowSpacing(2, 10);

	// setup the Play Sound stuff
	checkPlaySound = new QCheckBox(i18n("Play Sound"), this);
	checkPlaySound->setMinimumSize(checkPlaySound->sizeHint());
	top_layout->addWidget(checkPlaySound, 3, 0);

	editPlaySound = new QLineEdit(this);
	top_layout->addWidget(editPlaySound, 4, 0);

	buttonBrowsePlaySound = new QPushButton(i18n("Browse"), this);
	buttonBrowsePlaySound->setMinimumSize(75, 25);
	top_layout->addWidget(buttonBrowsePlaySound, 4, 2);

	// setup the System Sound stuff
	checkBeep = new QCheckBox(i18n("System Beep"), this);
	checkBeep->setMinimumSize(checkBeep->sizeHint());
	top_layout->addWidget(checkBeep, 5, 0);

	// connect some slots and signals
	connect(buttonBrowsePlaySound, SIGNAL(clicked()),
	                                 SLOT(browsePlaySound()));
	connect(buttonBrowseRunCommand, SIGNAL(clicked()),
	                                  SLOT(browseRunCommand()));
	connect(checkPlaySound, SIGNAL(toggled(bool)),
	                          SLOT(enablePlaySound(bool)));
	connect(checkRunCommand, SIGNAL(toggled(bool)),
	                           SLOT(enableRunCommand(bool)));

	readConfig(profile);
}

KBiffNewMailTab::~KBiffNewMailTab()
{
}

void KBiffNewMailTab::readConfig(const char* profile)
{
TRACEINIT("KBiffNewMailTab::readConfig()");
	// open the config file
	QString config_file(KApplication::localconfigdir());
	config_file += "/kbiff2rc";
	KSimpleConfig *config = new KSimpleConfig(config_file);

	config->setGroup(profile);

	checkRunCommand->setChecked(config->readBoolEntry("RunCommand", false));
	checkPlaySound->setChecked(config->readBoolEntry("PlaySound", false));
	checkBeep->setChecked(config->readBoolEntry("SystemBeep", true));
	editRunCommand->setText(config->readEntry("RunCommandPath"));
	editPlaySound->setText(config->readEntry("PlaySoundPath"));

	enableRunCommand(checkRunCommand->isChecked());
	enablePlaySound(checkPlaySound->isChecked());

	delete config;
}

void KBiffNewMailTab::saveConfig(const char *profile)
{
TRACEINIT("KBiffNewMailTab::saveConfig()");
	QString config_file(KApplication::localconfigdir());
	config_file += "/kbiff2rc";
	KSimpleConfig *config = new KSimpleConfig(config_file);

	config->setGroup(profile);

	config->writeEntry("RunCommand", checkRunCommand->isChecked());
	config->writeEntry("PlaySound", checkPlaySound->isChecked());
	config->writeEntry("SystemBeep", checkBeep->isChecked());
	config->writeEntry("RunCommandPath", editRunCommand->text());
	config->writeEntry("PlaySoundPath", editPlaySound->text());

	delete config;
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
	setCaption(i18n("Advanced Options"));

	QBoxLayout *top_layout = new QBoxLayout(this, QBoxLayout::Down, 12, 5);

	QGridLayout *grid_layout = new QGridLayout(2, 3, 12);
	top_layout->addLayout(grid_layout);

	QLabel *mailbox_label = new QLabel(i18n("Mailbox URL:"), this);
	grid_layout->addWidget(mailbox_label, 0, 0);

	QLabel *port_label = new QLabel(i18n("Port:"), this);
	grid_layout->addWidget(port_label, 1, 0);

	mailbox = new QLineEdit(this);
	mailbox->setMinimumSize(375, 25);
	grid_layout->addMultiCellWidget(mailbox, 0, 0, 1, 2);

	port = new QLineEdit(this);
	mailbox->setMinimumSize(100, 25);
	grid_layout->addWidget(port, 1, 1);
	grid_layout->addColSpacing(3, 200);

	connect(port, SIGNAL(textChanged(const char*)),
	              SLOT(portModified(const char*)));

	QBoxLayout *button_layout = new QBoxLayout(QBoxLayout::LeftToRight, 12);
	top_layout->addLayout(button_layout);

	QPushButton *ok = new QPushButton(i18n("OK"), this);
	ok->setMinimumSize(ok->sizeHint());
	ok->setDefault(true);
	button_layout->addWidget(ok);
	connect(ok, SIGNAL(clicked()), SLOT(accept()));

	QPushButton *cancel = new QPushButton(i18n("Cancel"), this);
	cancel->setMinimumSize(cancel->sizeHint());
	button_layout->addWidget(cancel);
	connect(cancel, SIGNAL(clicked()), SLOT(reject()));

	top_layout->activate();
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

KBiffMailboxTab::KBiffMailboxTab(const char* profile, QWidget *parent)
	: QWidget(parent)
{
TRACEINIT("KBiffMailboxTab::KBiffMailboxTab()");
	QGridLayout *grid = new QGridLayout(this, 8, 4, 12, 5);
	mailboxes = new QListView(this);
	mailboxes->addColumn(i18n("Mailbox:"));
	mailboxes->setColumnWidthMode(0, QListView::Maximum);

	grid->addMultiCellWidget(mailboxes, 0, 7, 0, 0);

	mailboxes->adjustSize();
	mailboxes->setMinimumWidth(mailboxes->columnWidth(0));

	connect(mailboxes, SIGNAL(selectionChanged(QListViewItem *)),
	                   SLOT(slotMailboxSelected(QListViewItem *)));

	QLabel *protocol_label = new QLabel(i18n("Protocol:"), this);
	protocol_label->setMinimumSize(protocol_label->sizeHint());

	QLabel *mailbox_label = new QLabel(i18n("Mailbox:"), this);
	mailbox_label->setMinimumSize(mailbox_label->sizeHint());

	QLabel *server_label = new QLabel(i18n("Server:"), this);
	server_label->setMinimumSize(server_label->sizeHint());

	QLabel *user_label = new QLabel(i18n("User:"), this);
	user_label->setMinimumSize(user_label->sizeHint());

	QLabel *password_label = new QLabel(i18n("Password:"), this);
	password_label->setMinimumSize(password_label->sizeHint());

	comboProtocol = new QComboBox(this);
	comboProtocol->insertItem("");
	comboProtocol->insertItem("mbox");
	comboProtocol->insertItem("maildir");
	comboProtocol->insertItem("imap4");
	comboProtocol->insertItem("pop3");
	comboProtocol->setMinimumSize(comboProtocol->sizeHint());

	connect(comboProtocol, SIGNAL(highlighted(int)),
	                       SLOT(protocolSelected(int)));

	buttonBrowse = new QPushButton(i18n("Browse"), this);
	buttonBrowse->setMinimumSize(75, 25);
	connect(buttonBrowse, SIGNAL(clicked()), SLOT(browse()));

	editMailbox = new QLineEdit(this);
	editMailbox->setMinimumHeight(25);

	editServer = new QLineEdit(this);
	editServer->setMinimumHeight(25);

	editUser = new QLineEdit(this);
	editUser->setMinimumHeight(25);

	editPassword = new QLineEdit(this);
	editPassword->setMinimumHeight(25);
	editPassword->setEchoMode(QLineEdit::Password);

	checkStorePassword = new QCheckBox(i18n("Store password"), this);
	checkStorePassword->setMinimumSize(checkStorePassword->sizeHint());

	QPushButton *advanced_button = new QPushButton(i18n("Advanced"), this);
	advanced_button->setMinimumSize(75, 25);
	connect(advanced_button, SIGNAL(clicked()), SLOT(advanced()));

	grid->addWidget(protocol_label, 0, 1);
	grid->addWidget(mailbox_label, 1, 1);
	grid->addWidget(server_label, 3, 1);
	grid->addWidget(user_label, 4, 1);
	grid->addWidget(password_label, 5, 1);

	grid->addWidget(comboProtocol, 0, 2);
	grid->addWidget(buttonBrowse, 0, 3);
	grid->addMultiCellWidget(editMailbox, 1, 1, 2, 3);
	grid->addMultiCellWidget(editServer, 3, 3, 2, 3);
	grid->addMultiCellWidget(editUser, 4, 4, 2, 3);
	grid->addMultiCellWidget(editPassword, 5, 5, 2, 3);
	grid->addMultiCellWidget(checkStorePassword, 6, 6, 2, 3);
	grid->addWidget(advanced_button, 7, 3);

	grid->setColStretch(1, 0);
	grid->setColStretch(2, 3);
	grid->setColStretch(3, 1);

	grid->activate();

	readConfig(profile);
}

KBiffMailboxTab::~KBiffMailboxTab()
{
}

void KBiffMailboxTab::readConfig(const char* profile)
{
TRACEINIT("KBiffMailboxTab::readConfig()");
	// open the config file
	QString config_file(KApplication::localconfigdir());
	config_file += "/kbiff2rc";
	KSimpleConfig *config = new KSimpleConfig(config_file);

	config->setGroup(profile);

	QStrList mailbox_list;

	int number_of_mailboxes = config->readListEntry("Mailboxes", mailbox_list);

	if (number_of_mailboxes > 0)
	{
		for (unsigned int i = 0; i < mailbox_list.count(); i+=2)
		{
			QString key = mailbox_list.at(i);
			QString url = mailbox_list.at(i+1);

			QListViewItem *item = new QListViewItem(mailboxes, key);
			item->setPixmap(0, QPixmap("mail.xpm"));

TRACEF("Inserting %s into %s", (const char*)url, (const char*)key);
			mailboxHash.insert(key, url);
		}
	}
	else
	{
		QFileInfo mailbox_info(getenv("MAIL"));
		if (mailbox_info.exists() == false)
		{
			QString s("/var/spool/mail/");
			s += getlogin();
			mailbox_info.setFile(s);
		}

		QString default_path("mbox:");
		default_path += mailbox_info.absFilePath();

TRACEF("Inserting %s into Default", (const char*)default_path);
		mailboxHash.insert("Default", default_path);

		QListViewItem *item = new QListViewItem(mailboxes, "Default");
		item->setPixmap(0, QPixmap("mail.xpm"));
		mailboxes->setSelected(item, true);
	}

	delete config;
}

void KBiffMailboxTab::saveConfig(const char *profile)
{
TRACEINIT("KBiffMailboxTab::saveConfig()");
	// open the config file
	QString config_file(KApplication::localconfigdir());
	config_file += "/kbiff2rc";
	KSimpleConfig *config = new KSimpleConfig(config_file);

	config->setGroup(profile);

	QStrList mailbox_list;

	for (QListViewItem *item = mailboxes->firstChild();
	     item;
		  item = item->nextSibling())
	{
TRACEF("mailbox: %s -> %s", item->text(0), mailboxHash["Default"]);
mailboxHash.statistics();
		mailbox_list.append(item->text(0));
		mailbox_list.append(mailboxHash[item->text(0)]);
	}

	config->writeEntry("Mailboxes", mailbox_list);
	delete config;
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

void KBiffMailboxTab::slotMailboxSelected(QListViewItem *item)
{
TRACEINIT("KBiffMailboxTab::slotMailboxSelected()");
TRACEF("mailbox = %s", item->text(0));
TRACEF("url = %s", mailboxHash[item->text(0)]);
	setMailbox(KURL(mailboxHash[item->text(0)]));
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
