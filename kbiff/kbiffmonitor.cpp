/*
 * kbiffmonitor.cpp
 * Copyright (C) 1998 Kurt Granroth <granroth@kde.org>
 *
 * This file contains the implementation of KBiffMonitor and
 * associated classes.
 *
 * $Id$
 */
#include "kbiffmonitor.h"
#include "kbiffmonitor.moc"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

#include "Trace.h"

KBiffMonitor::KBiffMonitor()
	: QObject()
{
TRACEINIT("KBiffMonitor::KBiffMonitor()");
	// Initialize variables
	lastSize   = 0;
	oldTimer   = 0;
	mailState  = UnknownState;
	started    = false;
	lastRead.setTime_t(0);
}

KBiffMonitor::~KBiffMonitor()
{
TRACEINIT("KBiffMonitor::~KBiffMonitor()");
}

void KBiffMonitor::start()
{
TRACEINIT("KBiffMonitor::start()");
	started  = true;
	oldTimer = startTimer(poll * 1000);
	emit(signal_checkMail());
}

void KBiffMonitor::stop()
{
TRACEINIT("KBiffMonitor::stop()");
	if (oldTimer > 0)
		killTimer(oldTimer);

	lastSize   = 0;
	oldTimer   = 0;
	mailState  = UnknownState;
	started    = false;
	lastRead.setTime_t(0);
}

void KBiffMonitor::setPollInterval(const int interval)
{
TRACEINIT("KBiffMonitor::setPollInterval()");
	poll = interval;

	// Kill any old timers that may be running
	if (oldTimer > 0)
	{
		killTimer(oldTimer);

		// Start a new timer will the specified time
		oldTimer = startTimer(interval * 1000);

		emit(signal_checkMail());
	}
}

void KBiffMonitor::setMailbox(const char* url)
{
TRACEINIT("KBiffMonitor::setMailbox()");
	KURL kurl(url);
	setMailbox(kurl);
}

void KBiffMonitor::setMailbox(KURL& url)
{
TRACEINIT("KBiffMonitor::setMailbox()");

	if (!strcmp(url.protocol(), "imap4"))
	{
		disconnect(this);

		connect(this, SIGNAL(signal_checkMail()), SLOT(checkImap()));
		server   = url.host();
		user     = url.user();
		password = url.passwd();

		mailbox  = QString(url.path()).right(strlen(url.path()) - 1); 
		port     = (url.port() > 0) ? url.port() : 143;
	}

	if (!strcmp(url.protocol(), "pop3"))
	{
		disconnect(this);

		connect(this, SIGNAL(signal_checkMail()), SLOT(checkPop()));
		server   = url.host();
		user     = url.user();
		password = url.passwd();
		port     = (url.port() > 0) ? url.port() : 110;
	}

	if (!strcmp(url.protocol(), "mbox"))
	{
		disconnect(this);

		connect(this, SIGNAL(signal_checkMail()), SLOT(checkLocal()));
		mailbox = url.path();
	}

	if (!strcmp(url.protocol(), "rsh"))
	{
		disconnect(this);

		connect(this, SIGNAL(signal_checkMail()), SLOT(checkRsh()));
		server  = url.host();
		user    = url.user();
		mailbox = url.path();
	}

	if (!strcmp(url.protocol(), "maildir"))
	{
		disconnect(this);

		connect(this, SIGNAL(signal_checkMail()), SLOT(checkMaildir()));
		mailbox = url.path();
	}
}

void KBiffMonitor::setMailboxIsRead()
{
TRACEINIT("KBiffMonitor::setMailboxIsRead()");
	lastRead  = QDateTime::currentDateTime();
	determineState(OldMail);
}

void KBiffMonitor::checkMailNow()
{
TRACEINIT("KBiffMonitor::checkMailNow()");
	emit(signal_checkMail());
}

void KBiffMonitor::setPassword(const char* pass)
{
TRACEINIT("KBiffMonitor::setPassword()");
	password = pass;
}

void KBiffMonitor::timerEvent(QTimerEvent *)
{
TRACEINIT("KBiffMonitor::timerEvent()");
	emit(signal_checkMail());
}

void KBiffMonitor::checkLocal()
{
TRACEINIT("KBiffMonitor::checkLocal()");
	// get the information about this local mailbox
	QFileInfo mbox(mailbox);

	// check if we have new mail
	determineState(mbox.size(), mbox.lastRead(), mbox.lastModified());
}

void KBiffMonitor::checkRsh()
{
TRACEINIT("KBiffMonitor::checkRsh()");
	// Get the size of the remote mailbox
	unsigned int size;
	size  = checkRshSize();

	// Check if we have new mail
	determineState(size);
}

void KBiffMonitor::checkPop()
{
TRACEINIT("KBiffMonitor::checkPop()");
	QString command;
	KBiffPop pop;

	if (pop.connectSocket(server, port) == false)
		return;
	
//	command.sprintf("USER %s\r\n", (const char*)user);
	command = "USER " + user + "\r\n";
	if (pop.command(command) == false)
		return;

//	command.sprintf("PASS %s\r\n", (const char*)password);
	command = "PASS " + user + "\r\n";
	if (pop.command(command) == false)
		return;

	determineState(pop.numberOfMessages());
}

void KBiffMonitor::checkImap()
{
TRACEINIT("KBiffMonitor::checkImap()");
	QString command;
	int seq = 1000;
	KBiffImap imap;

	if (imap.connectSocket(server, port) == false)
		return;
	
	command = QString().setNum(seq) + " LOGIN " + user + " " + password + "\r\n";
	if (imap.command(command, seq) == false)
		return;
	seq++;

	command = QString().setNum(seq) + " STATUS " + mailbox + " (messages recent)\r\n";
	if (imap.command(command, seq) == false)
		return;
	seq++;

	command = QString().setNum(seq) + " LOGOUT\r\n";
	if (imap.command(command, seq) == false)
		return;

	if (imap.numberOfMessages() == 0)
		determineState(NoMail);
	else
	{
		if (imap.numberOfNewMessages() > 0)
			determineState(NewMail);
		else
			determineState(OldMail);
	}
}

void KBiffMonitor::determineState(unsigned int size)
{
TRACEINIT("KBiffMonitor::determineState()");
	// check for no mail
	if (size == 0)
	{
		if (mailState != NoMail)
		{
			mailState = NoMail;
			lastSize  = 0;
			emit(signal_noMail());
		}

		return;
	}

	// check for new mail
	if (size > lastSize)
	{
		if (mailState != NewMail)
		{
			mailState = NewMail;
			lastSize  = size;
			emit(signal_newMail());
		}

		return;
	}

	// if we have *some* mail, but the state is unknown,
	// then we'll consider it old
	if (mailState == UnknownState)
	{
		mailState = OldMail;
		lastSize  = size;
		emit(signal_oldMail());

		return;
	}

	// check for old mail
	if (size < lastSize)
	{
		if (mailState != OldMail)
		{
			mailState = OldMail;
			lastSize  = size;
			emit(signal_oldMail());
		}
	}
}

void KBiffMonitor::determineState(KBiffMailState state)
{
TRACEINIT("KBiffMonitor::determineState()");
	if ((state == NewMail) && (mailState != NewMail))
	{
		mailState = NewMail;
		emit(signal_newMail());
	}
	else
	if ((state == NoMail) && (mailState != NoMail))
	{
		mailState = NoMail;
		emit(signal_noMail());
	}
	else
	if ((state == OldMail) && (mailState != OldMail))
	{
		mailState = OldMail;
		emit(signal_oldMail());
	}
}

void KBiffMonitor::determineState(unsigned int size, const QDateTime& last_read, const QDateTime& last_modified)
{
TRACEINIT("KBiffMonitor::determineState()");
	// Check for NoMail
	if (size == 0)
	{
		// Is this a new state?
		if (mailState != NoMail)
		{
			// Yes, the user has just nuked the entire mailbox
			mailState = NoMail;
			lastRead  = last_read;
			lastSize  = 0;

			// Let the world know of the new state
			emit(signal_noMail());
		}

		return;
	}

	// There is some mail.  See if it is new or not.  To be new, the
	// mailbox must have been modified after it was last read AND the
	// current size must be greater then it was before.
	if ((last_modified > last_read) && (size > lastSize))
	{
		// We have new mail!
		mailState = NewMail;
		lastRead  = last_read;
		lastSize  = size;

		// Let the world know of the new state
		emit(signal_newMail());

		return;
	}

TRACEF("mailState = %d", mailState);
TRACEF("last_read = %s", (const char*)last_read.toString());
TRACEF("lastRead = %s", (const char*)lastRead.toString());
	// Finally, check if the state needs to change to OldMail
	if ((mailState != OldMail) && (last_read > lastRead))
	{
		mailState = OldMail;
		lastRead  = last_read;
		lastSize  = size;

		// Let the world know of the new state
		emit(signal_oldMail());

		return;
	}

	// If we get to this point, then the state now is exactly the
	// same as the state when last we checked.  Do nothing at this
	// point.
}

unsigned int KBiffMonitor::checkRshSize()
{
TRACEINIT("KBiffMonitor::checkRshSize()");
	unsigned int size = 0;

	// Get the file status from the remote host
	QString rshcmd;
	rshcmd.sprintf("rsh -l %s %s ls -o %s",
		(const char*)user,
		(const char*)server,
		(const char*)mailbox);

	// Now run the command in a pipe and parse the result
	FILE *result = popen((const char*)rshcmd, "r");
	if (result)
	{
		char line[256];
		if (fgets(line, 256, result))
		{
			QString str(line);
			// This will match e.g., "-r-w------ 1 user "
			QRegExp perm("[-rwx]+[ \t]+[0-9]+[ \t]+[a-zA-Z0-9]+[ \t]+");
			QRegExp sizere("[0-9]+");
   			int perlen, size_len;
			int pos;

			if ((pos = perm.match(str, 0, &perlen)) != -1)
			{
				if((pos = sizere.match(str, perlen, &size_len)) != -1)
				{
					size = str.mid(pos, size_len).toUInt();
				}
			}
		}
		
		// close the pipe
		pclose(result);
	}

	return size;
}

void KBiffMonitor::checkMaildir()
{
TRACEINIT("KBiffMonitor::checkMaildir()");
	// get the information about this local mailbox
	QDir mbox(mailbox);

	// make sure the mailbox exists
	if (mbox.exists())
	{
		// maildir stores its mail in MAILDIR/new and MAILDIR/cur
		QDir new_mailbox(mailbox + "/new");
		QDir cur_mailbox(mailbox + "/cur");

		// make sure both exist
		if (new_mailbox.exists() && cur_mailbox.exists())
		{
			// check only files
			new_mailbox.setFilter(QDir::Files);
			cur_mailbox.setFilter(QDir::Files);

			// get the number of messages in each
			int new_count = new_mailbox.count();
			int cur_count = cur_mailbox.count();

			// all messages in 'new' are new
			if (new_count > 0)
			{
				determineState(NewMail);
			}
			// failing that, we look for any old ones
			else if (cur_count > 0)
			{
				determineState(OldMail);
			}
			// failing that, we have no mail
			else
				determineState(NoMail);
		}
	}
}

///////////////////////////////////////////////////////////////////////////
// KBiffSocket
///////////////////////////////////////////////////////////////////////////
KBiffSocket::KBiffSocket() : messages(0), newMessages(-1)
{
}

KBiffSocket::~KBiffSocket()
{
TRACEINIT("KBiffSocket::~KBiffSocket()");
	::close(socketFD);
}

int KBiffSocket::numberOfMessages()
{
	return messages;
}

int KBiffSocket::numberOfNewMessages()
{
	return (newMessages > -1) ? newMessages : 0;
}

void KBiffSocket::close()
{
TRACEINIT("KBiffSocket::close()");
	::close(socketFD);
}

bool KBiffSocket::connectSocket(const char* host, unsigned int port)
{
TRACEINIT("KBiffSocket::connectSocket()");
	sockaddr_in  sin;
	hostent     *hent; 
	int addr;

	// get the socket
	socketFD = ::socket(AF_INET, SOCK_STREAM, IPPROTO_IP);

	// start setting up the socket info
	memset((char *)&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port   = htons(port);

	// get the address
	if ((addr = inet_addr(host)) == -1)
	{
		// get the address by host name
		if ((hent = gethostbyname(host)) == 0)
			return false;

		memcpy((void *)&sin.sin_addr, *(hent->h_addr_list), hent->h_length);
	}
	else
		// get the address by IP
		memcpy((void *)&sin.sin_addr, (void *)&addr, sizeof(addr));

	// the socket is correctly setup.  now connect
	if (::connect(socketFD, (sockaddr *)&sin, sizeof(sockaddr_in)) == -1)
		return false;

	// we're connected!  see if the connection is good
	if (readLine().find("OK") == -1)
		return false;

	// everything is swell
	return true;
}

int KBiffSocket::writeLine(const QString& line)
{
TRACEINIT("KBiffSocket::writeLine()");
printf("CLIENT> %s", (const char*)line);
	int bytes;

	if ((bytes = ::write(socketFD, line, line.size()-1)) <= 0)
		close();

	return bytes;
}

QString KBiffSocket::readLine()
{
TRACEINIT("KBiffSocket::readLine()");
	QString response;
	char buffer;
	while ((::read(socketFD, &buffer, 1) > 0) && (buffer != '\n'))
		response += buffer;

printf("SERVER> %s\n", (const char*)response);
	return response;
}

///////////////////////////////////////////////////////////////////////////
// KBiffImap
///////////////////////////////////////////////////////////////////////////
bool KBiffImap::command(const QString& line, unsigned int seq)
{
TRACEINIT("KBiffIMap::command()");
	int len, match;

	if (writeLine(line) <= 0)
		return false;

	QString ok, response;
	ok.sprintf("%d OK", seq);
	while (response = readLine())
	{
		// if the response is either good or bad, then return
		if (response.find(ok) > -1)
			return true;
		if (response.find("BAD") > -1)
			return false;

		// check the number of messages
		QRegExp messages_re("MESSAGES [0-9]*");
		if ((match = messages_re.match(response, 0, &len)) > -1)
			messages = response.mid(match + 9, len - 9).toInt();

		// check for new mail
		QRegExp recent_re("RECENT [0-9]*");
		if ((match = recent_re.match(response, 0, &len)) > -1)
			newMessages = response.mid(match + 7, len - 7).toInt();
	}

	return false;
}

///////////////////////////////////////////////////////////////////////////
// KBiffPop
///////////////////////////////////////////////////////////////////////////
bool KBiffPop::command(const QString& line)
{
TRACEINIT("KBiffPop::command()");
	int len, match;

	if (writeLine(line) <= 0)
		return false;

	QString response;
	response = readLine();

	// check the number of messages
	QRegExp message_re("[0-9]* messages");
	if ((match = message_re.match(response, 0, &len)) > -1)
		messages = response.mid(match, len - 9).toInt();

	// if the response is either good or bad, then return
	if (response.left(3) == "+OK")
		return true;

	return false;
}
