#ifndef KBIFF_H 
#define KBIFF_H 

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif 

#include <kbiffmonitor.h>

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

protected:
	void mousePressEvent(QMouseEvent *);

protected:
	void popupMenu();
	void reset();

protected slots:
	void invokeHelp();
	void displayPixmap();
	void haveNewMail();
	void dock();
	void setup();

private:
	KBiffMonitor monitor;

	// Capability
	bool   hasAudio;
	KAudio audioServer;
	
	// General settings
	unsigned int poll;
	KURL         mailbox;
	QString      mailClient;

	// New mail
	bool    systemBeep;
	bool    runCommand;
	QString runCommandPath;
	bool    playSound;
	QString playSoundPath;

	bool isDocked;

	QString noMailIcon;
	QString newMailIcon;
	QString oldMailIcon;
};

#endif // KBIFF_H 
