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

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include <kurl.h>

#include <qapp.h>
#include <qstring.h>
#include <qregexp.h>
#include <qdir.h>
#include <qdatetm.h>

#include "Trace.h"

#define MAXSTR (1024)

static bool real_from(const char *buffer);
static const char *compare_header(const char *header, const char *field);

KBiffMonitor::KBiffMonitor()
	: QObject(),
	  poll(60),
	  oldTimer(0),
	  started(false),
	  newCount(0),
	  protocol(""),
	  mailbox(""),
	  server(""),
	  user(""),
	  password(""),
	  port(0),
	  mailState(UnknownState),
	  lastSize(0)
{
TRACEINIT("KBiffMonitor::KBiffMonitor()");
	lastRead.setTime_t(0);
	lastModified.setTime_t(0);
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
	lastModified.setTime_t(0);
	uidlList.clear();
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
		if (started)
		{
			oldTimer = startTimer(interval * 1000);

			emit(signal_checkMail());
		}
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
	protocol = url.protocol();

	if (protocol == "imap4")
	{
		disconnect(this);

		connect(this, SIGNAL(signal_checkMail()), SLOT(checkImap()));
		server   = url.host();
		user     = url.user();
		password = url.passwd();

		mailbox  = QString(url.path()).right(strlen(url.path()) - 1); 
		port     = (url.port() > 0) ? url.port() : 143;
	}

	if (protocol == "pop3")
	{
		disconnect(this);

		connect(this, SIGNAL(signal_checkMail()), SLOT(checkPop()));
		server   = url.host();
		user     = url.user();
		password = url.passwd();
		mailbox  = url.user();
		port     = (url.port() > 0) ? url.port() : 110;
	}

	if (protocol == "mbox")
	{
		disconnect(this);

		connect(this, SIGNAL(signal_checkMail()), SLOT(checkMbox()));
		mailbox = url.path();
	}

	if (protocol == "file")
	{
		disconnect(this);

		connect(this, SIGNAL(signal_checkMail()), SLOT(checkLocal()));
		mailbox = url.path();
	}

	if (protocol == "maildir")
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
	if (mailState == NewMail)
	{
		determineState(OldMail);
		mailState = NewMail;
	}
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

void KBiffMonitor::checkMbox()
{
TRACEINIT("KBiffMonitor::checkMbox()");
	// get the information about this local mailbox
	QFileInfo mbox(mailbox);

	// handle the NoMail case
	if (mbox.size() == 0)
	{
		determineState(NoMail);
		return;
	}

	// see if the state has changed
	if ((mbox.lastModified() != lastModified) || (mbox.size() != lastSize))
	{
		lastModified = mbox.lastModified();
		lastSize     = mbox.size();

		// ok, the state *has* changed.  see if the number of
		// new messages has, too.
		newCount = mboxMessages();

		// if there are any new messages, consider the state New
		if (newCount > 0)
			determineState(NewMail);
		else
			determineState(OldMail);
	}
}

void KBiffMonitor::checkPop()
{
TRACEINIT("KBiffMonitor::checkPop()");
	QString command;
	KBiffPop pop;

	if(pop.connectSocket(server, port) == false)
		return;

	command = "USER " + user + "\r\n";
	if (pop.command(command) == false)
		return;

	command = "PASS " + password + "\r\n";
	if (pop.command(command) == false)
		return;

	command = "UIDL\r\n";
	if (pop.command(command) == false)
	{
		command = "STAT\r\n";
		if (pop.command(command) == false)
		{
			command = "LIST\r\n";
			if (pop.command(command) == false)
				return;
		}
	}

	if (command == "UIDL\r\n")
		determineState(pop.getUidlList());
	else
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
	
	user = imap.mungeUser(user);

	// if user is null, assume PREAUTH
	if (user.isEmpty() == false)
	{
		command = QString().setNum(seq) + " LOGIN " + user + " " + password + "\r\n";
		if (imap.command(command, seq) == false)
			return;
		seq++;
	}

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
		{
			newCount = imap.numberOfNewMessages();
			determineState(NewMail);
		}
		else
			determineState(OldMail);
	}
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
			newCount = new_mailbox.count();
			int cur_count = cur_mailbox.count();

			// all messages in 'new' are new
			if (newCount > 0)
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
			emit(signal_noMail(mailbox));
		}

		return;
	}

	// check for new mail
	if (size > lastSize)
	{
		if (mailState != NewMail)
		{
			mailState = NewMail;
			newCount  = size - lastSize;
			lastSize  = size;
			emit(signal_newMail());
			emit(signal_newMail(newCount, mailbox));
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
		emit(signal_oldMail(mailbox));

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
			emit(signal_oldMail(mailbox));
		}
	}
}

void KBiffMonitor::determineState(KBiffUidlList uidl_list)
{
TRACEINIT("KBiffMonitor::determineState()");    
	QString *UIDL;
	unsigned int messages = 0;

	// if the uidl_list is empty then the number of messages = 0  
	if (uidl_list.isEmpty())
	{
		if (mailState != NoMail)
		{
			lastSize  = 0;
			mailState = NoMail;
			emit(signal_noMail());
			emit(signal_noMail(mailbox));
		}
	}
	else
	{
		// if a member of uidl_list is not in the old uidlList then we have 
		// new mail
		for (UIDL = uidl_list.first(); UIDL != 0; UIDL = uidl_list.next())
		{
			if (uidlList.find(UIDL) == -1)
				messages++;
		}
TRACEF("new messages = %d", messages);

		// if there are any new messages, then notify
		if (messages > 0) 
		{
			lastSize  = newCount = messages;
			mailState = NewMail;
			emit(signal_newMail());
			emit(signal_newMail(newCount, mailbox));
		}
		/*
		else
		{
			mailState = OldMail;
			emit(signal_oldMail());
			emit(signal_oldMail(mailbox));
		}
		*/
	}

	uidlList = uidl_list;
}
    
void KBiffMonitor::determineState(KBiffMailState state)
{
TRACEINIT("KBiffMonitor::determineState()");
	if ((state == NewMail) && (mailState != NewMail))
	{
		mailState = NewMail;
		emit(signal_newMail());
		emit(signal_newMail(newCount, mailbox));
	}
	else
	if ((state == NoMail) && (mailState != NoMail))
	{
		mailState = NoMail;
		emit(signal_noMail());
		emit(signal_noMail(mailbox));
	}
	else
	if ((state == OldMail) && (mailState != OldMail))
	{
		mailState = OldMail;
		emit(signal_oldMail());
		emit(signal_oldMail(mailbox));
	}
}

void KBiffMonitor::determineState(unsigned int size, const QDateTime& last_read, const QDateTime& last_modified)
{
TRACEINIT("KBiffMonitor::determineState()");
TRACEF("mailState = %d", mailState);
TRACEF("last_read = %s", (const char*)last_read.toString());
TRACEF("lastRead = %s", (const char*)lastRead.toString());
TRACEF("last_modified = %s", last_modified.toString().data());
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
			emit(signal_noMail(mailbox));
		}

		return;
	}

	// There is some mail.  See if it is new or not.  To be new, the
	// mailbox must have been modified after it was last read AND the
	// current size must be greater then it was before.
	if ((last_modified >= last_read) && (size > lastSize))
	{
		// We have new mail!
		mailState = NewMail;
		lastRead  = last_read;
		lastSize  = size;
		newCount  = 1;

		// Let the world know of the new state
		emit(signal_newMail());
		emit(signal_newMail(1, mailbox));

		return;
	}

	// Finally, check if the state needs to change to OldMail
	if ((mailState != OldMail) && (last_read > lastRead))
	{
		mailState = OldMail;
		lastRead  = last_read;
		lastSize  = size;

		// Let the world know of the new state
		emit(signal_oldMail());
		emit(signal_oldMail(mailbox));

		return;
	}

	// If we get to this point, then the state now is exactly the
	// same as the state when last we checked.  Do nothing at this
	// point.
}

/**
 * The following function is lifted from unixdrop.cpp in the korn
 * distribution.  It is (C) Sirtaj Singh Kang <taj@kde.org> and is
 * used under the GPL license (and the author's permission).  It has
 * been slightly modified for formatting reasons.
 */
int KBiffMonitor::mboxMessages()
{
	QFile mbox(mailbox);
	char *buffer         = new char[MAXSTR];
	int count            = 0;
	int msg_count        = 0;
	bool in_header       = false;
	bool has_content_len = false;
	bool msg_read        = false;
	long content_length  = 0;

	if (mbox.open(IO_ReadOnly) == false)
	{
		warning("countMail: file open error");
		return 0;
	}

	buffer[MAXSTR-1] = 0;

	while (mbox.readLine(buffer, MAXSTR-2) > 0)
	{
		// read a line from the mailbox

		if (!strchr(buffer, '\n') && !mbox.atEnd())
		{
			// read till the end of the line if we
			// haven't already read all of it.

			int c;

			while((c=mbox.getch()) >=0 && c !='\n');
		}

		if (!in_header && real_from(buffer))
		{
			// check if this is the start of a message
			has_content_len = false;
			in_header       = true;
			msg_read        = false;
		}
		else if (in_header)
		{
			// check header fields if we're already in one

			if (compare_header(buffer, "Content-Length"))
			{
				has_content_len = true;
				content_length  = atol(buffer + 15);
			}

			if (compare_header(buffer, "Status"))
			{
				const char *field = buffer;
				field += 7;
				while (field && (*field== ' ' || *field == '\t'))
					field++;

				if (*field == 'N' || *field == 'U')
					msg_read = false;
				else
					msg_read = true;
			}
			else if (buffer[0] == '\n' )
			{
				if (has_content_len)
					mbox.at(mbox.at() + content_length);

				in_header = false;

				if (!msg_read)
					count++;
			} 
		}//in header

		if(++msg_count >= 100 )
		{
			qApp->processEvents();
			msg_count = 0;
		}
	}//while

	mbox.close();

	return count;
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
	QString line(readLine());
	if ((line.find("OK") == -1) && (line.find("PREAUTH") == -1))
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
		if (response.find("NO ") > -1)
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

QString KBiffImap::mungeUser(const QString& old_user)
{
TRACEINIT("KBiffImap::mungeUser()");
TRACEF("old_user = %s", old_user.data());
	if (old_user.contains(' ') > 0)
	{
		QString new_user(old_user);

		if (new_user.left(1) != "\"")
			new_user.prepend("\"");
		if (new_user.right(1) != "\"")
			new_user.append("\"");

TRACEF("new_user = %s", new_user.data());
		return new_user;
	}
	else
		return old_user;
}

///////////////////////////////////////////////////////////////////////////
// KBiffPop
///////////////////////////////////////////////////////////////////////////
bool KBiffPop::command(const QString& line)
{
TRACEINIT("KBiffPop::command()");
	int len;

	if (writeLine(line) <= 0)
		return false;

	QString response;
	response = readLine();

	// check if the response was bad.  if so, return now
	if (response.left(4) == "-ERR")
		return false;

	// if the command was UIDL then build up the newUidlList
	if (line == "UIDL\r\n")
	{
		for (response = readLine(); response.left(1) != ".";
		     response = readLine())
		{
			uidlList.append(new QString(response.right(response.length() -
					response.find(" ") - 1)));
		}
	}
	else
	// get all response lines from the LIST command    
	// LIST and UIDL are return multilines so we have to loop around
	if (line == "LIST\r\n")
	{
		for (messages = 0, response = readLine(); response.left(1) != ".";
		     messages++, response = readLine());
	}
	else
	if (line == "STAT\r\n")
	{
		sscanf(response.data(), "+OK %d", &messages);
	}

	return true;
}

KBiffUidlList KBiffPop::getUidlList() const
{
	return uidlList;
}

/////////////////////////////////////////////////////////////////////////
/* The following is a (C) Sirtaj Singh Kang <taj@kde.org> */

#define whitespace(c)    (c == ' ' || c == '\t')

#define skip_white(c)	 while(c && (*c) && whitespace(*c) ) c++
#define skip_nonwhite(c) while(c && (*c) && !whitespace(*c) ) c++

#define skip_token(buf) skip_nonwhite(buf); if(!*buf) return false; \
	skip_white(buf); if(!*buf) return false;

static char *month_name[13] = {
	"jan", "feb", "mar", "apr", "may", "jun",
	"jul", "aug", "sep", "oct", "nov", "dec", NULL
};

static char *day_name[8] = {
	"sun", "mon", "tue", "wed", "thu", "fri", "sat", 0
};

static bool real_from(const char *buffer)
{
	/*
		A valid from line will be in the following format:

		From <user> <weekday> <month> <day> <hr:min:sec> [TZ1 [TZ2]] <year>
	 */

	int day;
	int i;
	int found;

	/* From */

	if(!buffer || !*buffer)
		return false;

	if (strncmp(buffer, "From ", 5))
		return false;

	buffer += 5;

	skip_white(buffer);

	/* <user> */
	if(*buffer == 0) return false;
	skip_token(buffer);

	/* <weekday> */
	found = 0;
	for (i = 0; day_name[i] != NULL; i++)
		found = found || (strnicmp(day_name[i], buffer, 3) == 0);

	if (!found)
		return false;

	skip_token(buffer);

	/* <month> */
	found = 0;
	for (i = 0; month_name[i] != NULL; i++)
		found = found || (strnicmp(month_name[i], buffer, 3) == 0);
	if (!found)
		return false;

	skip_token(buffer);

	/* <day> */
	if ( (day = atoi(buffer)) < 0 || day < 1 || day > 31)
		return false;

	return true;
}

static const char *compare_header(const char *header, const char *field)
{
	int len = strlen(field);

	if (strnicmp(header, field, len))
		return NULL;

	header += len;

	if( *header != ':' )
		return NULL;

	header++;

	while( *header && ( *header == ' ' || *header == '\t') )
		header++;

	return header;
}
