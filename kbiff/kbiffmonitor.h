/*
 * kbiffmonitor.h
 * Copyright (C) 1998 Kurt Granroth <granroth@kde.org>
 *
 * $Id$
 *
 */
#ifndef KBIFFMONITOR_H
#define KBIFFMONITOR_H

#include <qobject.h>
#include <kurl.h>

#include <qfileinf.h>
#include <qdir.h>
#include <qdatetm.h>
#include <stdio.h>
#include <qregexp.h>
#include <qstring.h>

typedef enum
{
	NewMail,
	NoMail,
	OldMail,
	UnknownState
} KBiffMailState;

/**
 * A "biff"-like class that can monitor local and remote mailboxes for new
 * mail.  KBiffMonitor currently supports four protocols.
 * 
 * <UL>
 * <LI>mbox</LI> Local mbox style files
 * <LI>pop3</LI> POP3 
 * <LI>imap4</LI> imap4 
 * <LI>maildir</LI> Mailboxes in maildir format
 * </UL>
 *
 * A typical usage would look like so:
 *
 * <PRE>
 *    KBiffMonitor mon;
 *    mon.setMailbox("imap4://user:password@some.host.net/mailbox");
 *    mon.setPollInterval(15);
 *    mon.start();
 *
 *    connect(&mon, SIGNAL(signal_newMail()), this, SLOT(processNewMail()));
 *    connect(&mon, SIGNAL(signal_oldMail()), this, SLOT(processOldMail()));
 *    connect(&mon, SIGNAL(signal_noMail()), this, SLOT(processNoMail()));
 * </PRE>
 *
 * @short A "biff" class that monitors local and remote mailboxes
 * @author Kurt Granroth <granroth@kde.org>
 * @version 0.4.8
 */
class KBiffMonitor : public QObject
{

	Q_OBJECT
public:

	/**
	 * Constructor.  Does not take any arguments
	 */
	KBiffMonitor();

	/**
	 * Destructor.
	 */
	virtual ~KBiffMonitor();

	/**
	 * Returns the current state of the mailbox (NewMail, OldMail, NoMail, or
	 * UnknownState)
	 */
	KBiffMailState getMailState() const { return mailState; }

	/**
	 * Returns the current mailbox being monitored
	 */
	const char* getMailbox() const { return mailbox; }

	/**
	 * Returns <CODE>true</CODE> is KBiffMonitor is currently monitoring
	 * a mailbox.
	 */
	bool isRunning() { return started; }

public slots:
	/**
	 * Sets the mailbox to monitor.  It uses a KURL to specify the
	 * protocol, host, username, password, port and path (depending on
	 * protocol type).  KBiffMonitor recognizes four protocols:
	 * 
	 * <UL>
	 * <LI>mbox</LI> Local files
	 * <LI>pop3</LI> POP3 
	 * <LI>imap4</LI> IMAP4 
	 * <LI>maildir</LI> Mailboxes in maildir format
	 * </UL>
	 *
	 * Some examples:
	 * <PRE>
	 * mbox:/var/spool/mail/granroth
	 * </PRE>
	 *
	 * This would monitor a local file called '/var/spool/mail/granroth'
	 *
	 * <PRE>
	 * pop3://granroth:password@host.net:1234
	 * </PRE>
	 *
	 * This would monitor POP3 mailbox 'granroth' on server 'host.net'
	 * using 1234 as the port and 'password' as the password.
	 *
	 * <PRE>
	 * imap4://granroth:password@host.net/Mail/mailbox
	 * </PRE>
	 *
	 * This would monitor IMAP4 mailbox 'Mail/mailbox' on server 'host.net'
	 * with 'granroth' as the user and 'password' as the password.
	 */
	void setMailbox(KURL& url);

	/**
	 * Overloaded for convenience
	 */
	void setMailbox(const char* url);

	/**
	 * Sets the password for the POP3 and IMAP4 protocols.
	 */
	void setPassword(const char* password);

	/**
	 * Set the interval between mailbox reads.  This is in seconds.
	 */
	void setPollInterval(const int interval);

	/**
	 * Start monitoring the mailbox
	 */
	void start();

	/**
	 * Stop monitoring the mailbox
	 */
	void stop();

	/**
	 * Fakes KBiffMonitor into thinking that the mailbox was just read
	 */
	void setMailboxIsRead();

	/**
	 * Forces a mailbox check
	 */
	void checkMailNow();

signals:
	/**
	 * This will get <CODE>emit</CODE>ed when new mail arrives
	 */
	void signal_newMail();

	/**
	 * This will get <CODE>emit</CODE>ed when new mail arrives
	 */
	void signal_newMail(const int num_new, const char* mailbox);

	/**
	 * This will get <CODE>emit</CODE>ed when no mail exists
	 */
	void signal_noMail();

	/**
	 * This will get <CODE>emit</CODE>ed when no mail exists
	 */
	void signal_noMail(const char* mailbox);

	/**
	 * This will get <CODE>emit</CODE>ed when the mailbox is read
	 */
	void signal_oldMail();

	/**
	 * This will get <CODE>emit</CODE>ed when the mailbox is read
	 */
	void signal_oldMail(const char* mailbox);

	/**
	 * This will get <CODE>emit</CODE>ed everytime the mailbox
	 * should be checked (determined by @ref #setPollInterval)
	 */
	void signal_checkMail();

protected:
	void timerEvent(QTimerEvent *);

protected slots:
	void checkLocal();
	void checkPop();
	void checkMaildir();
	void checkImap();

protected:
	// protected (non-slot) functions
	void determineState(unsigned int size, const QDateTime& last_read,
	                    const QDateTime& last_modified);
	void determineState(unsigned int size);
	void determineState(KBiffMailState state, const int num = -1);

private:
	// General stuff
	int     poll;
	int     oldTimer;
	bool    started;

	// Mailbox stuff
	QString mailbox;
	QString server;
	QString user;
	QString password;
	int     port;

	// State variables
	KBiffMailState mailState;
	unsigned int   lastSize;
	QDateTime      lastRead;
};

/**
 * @internal
 */
class KBiffSocket
{
public:
	KBiffSocket();
	virtual ~KBiffSocket();

	bool connectSocket(const char* host, unsigned int port);

	int numberOfMessages();
	int numberOfNewMessages();

protected:
	QString readLine();
	int writeLine(const QString& line);
	void close();

	int socketFD;
	int messages;
	int newMessages;
};

/**
 * @internal
 */
class KBiffImap : public KBiffSocket
{
public:
	bool command(const QString& line, unsigned int seq);
	QString mungeUser(const QString& old_user);
};

/**
 * @internal
 */
class KBiffPop : public KBiffSocket
{
public:
	bool command(const QString& line);
};
#endif // KBIFFMONITOR_H
