#include "setupdlg.h"
#include "setupdlg.moc"

#include "Trace.h"

#include <qpixmap.h>
#include <qfont.h>
#include <qlabel.h>
#include <qgroupbox.h>

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
	setCaption(i18n("KBiff Setup"));

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
	tabctl->setGeometry(5, 5, 320, 280);

	resize(330, 295);
}

KBiffSetup::~KBiffSetup()
{
}

void KBiffSetup::invokeHelp()
{
	kapp->invokeHTMLHelp("kbiff/kbiff.html", "");
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
	poll_label->setText(klocale->translate("Poll (sec):"));
	poll_label->adjustSize();

	// the command to run when clicked
	QLabel* command_label = new QLabel(this);
	command_label->setGeometry(10, 50, 65, 30);
	command_label->setText(klocale->translate("Mail client:"));
	command_label->adjustSize();

	int x = QMAX(poll_label->width() + 15, command_label->width() + 15);

	editPoll = new QLineEdit(this);
	editPoll->setGeometry(x, 15, 60, MIN_HEIGHT);

	editCommand = new QLineEdit(this);
	editCommand->setGeometry(x, 45, 300 - x, MIN_HEIGHT);

	// do we dock automatically?
	checkDock = new QCheckBox(this);
	checkDock->setText(klocale->translate("Dock in panel"));
	checkDock->setGeometry(x, 75, 150, 25);
	checkDock->adjustSize();

	// should we support session management?
	checkNoSession = new QCheckBox(this);
	checkNoSession->setText(klocale->translate("Use session management"));
	checkNoSession->setGeometry(x, 100, 190, 25);
	checkNoSession->adjustSize();

	// group box to hold the icons together
	QGroupBox* icons_groupbox = new QGroupBox(this);
	icons_groupbox->setTitle(klocale->translate("Icons:"));
	icons_groupbox->move(10, 125);

	x = 125 + fontMetrics().lineSpacing();

	// "no mail" pixmap button
	QLabel* nomail_label = new QLabel(this);
	nomail_label->setText(klocale->translate("No Mail:"));
	nomail_label->adjustSize();

	// "old mail" pixmap button
	QLabel* oldmail_label = new QLabel(this);
	oldmail_label->setText(klocale->translate("Old Mail:"));
	oldmail_label->adjustSize();

	// "new mail" pixmap button
	QLabel* newmail_label = new QLabel(this);
	newmail_label->setText(klocale->translate("New Mail:"));
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

void KBiffGeneralTab::setCommand(const char* command)
{
	editCommand->setText(command);
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

const char* KBiffGeneralTab::getCommand()
{
	return editCommand->text();
}

const int KBiffGeneralTab::getPoll()
{
	return atoi(editPoll->text());
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
KBiffMailboxTab::KBiffMailboxTab(QWidget *parent)
	: QWidget(parent)
{
TRACEINIT("KBiffMailboxTab::KBiffMailboxTab()");
	QPushButton *browse = new QPushButton(this);
	browse->setText(i18n("Browse"));
	browse->setGeometry(235, 15, 70, 25);

	QLabel* protocol_label = new QLabel(this);
	protocol_label->setText(i18n("Protocol:"));
	protocol_label->move(15, 15);
	protocol_label->adjustSize();
	comboProtocol = new QComboBox(this);
	comboProtocol->setGeometry(85, 15, 100, MIN_HEIGHT);

	QLabel* mailbox_label = new QLabel(this);
	mailbox_label->setText(i18n("Mailbox:"));
	mailbox_label->move(15, 45);
	mailbox_label->adjustSize();
	editMailbox = new QLineEdit(this);
	editMailbox->setGeometry(85, 45, 220, MIN_HEIGHT);

	QLabel* server_label = new QLabel(this);
	server_label->setText(i18n("Server:"));
	server_label->move(15, 95);
	server_label->adjustSize();
	editServer = new QLineEdit(this);
	editServer->setGeometry(85, 95, 220, MIN_HEIGHT);

	QLabel* user_label = new QLabel(this);
	user_label->setText(i18n("User:"));
	user_label->move(15, 125);
	user_label->adjustSize();
	editUser = new QLineEdit(this);
	editUser->setGeometry(85, 125, 220, MIN_HEIGHT);

	QLabel* password_label = new QLabel(this);
	password_label->setText(i18n("Password:"));
	password_label->move(15, 155);
	password_label->adjustSize();
	editPassword = new QLineEdit(this);
	editPassword->setGeometry(85, 155, 220, MIN_HEIGHT);
	editPassword->setEchoMode(QLineEdit::Password);

	checkStorePassword = new QCheckBox(this);
	checkStorePassword->setGeometry(85, 185, 220, MIN_HEIGHT);
	checkStorePassword->setText(i18n("Store password"));
	checkStorePassword->adjustSize();

	QPushButton *advanced = new QPushButton(this);
	advanced->setText(i18n("Advanced"));
	advanced->setGeometry(225, 210, 80, 25);
}

void KBiffMailboxTab::setMailbox(const KURL&)
{
}

KURL KBiffMailboxTab::getMailbox()
{
	KURL url;

	return url;
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
