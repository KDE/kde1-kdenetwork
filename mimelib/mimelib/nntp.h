//=============================================================================
// File:       nntp.h
// Contents:   Declarations for DwNntpClient
// Maintainer: Doug Sauder <dwsauder@fwb.gulf.net>
// WWW:        http://www.fwb.gulf.net/~dwsauder/mimepp.html
// $Revision$
// $Date$
//
// Copyright (c) 1996, 1997 Douglas W. Sauder
// All rights reserved.
// 
// IN NO EVENT SHALL DOUGLAS W. SAUDER BE LIABLE TO ANY PARTY FOR DIRECT,
// INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT OF
// THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF DOUGLAS W. SAUDER
// HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// DOUGLAS W. SAUDER SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT
// NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
// PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS ON AN "AS IS"
// BASIS, AND DOUGLAS W. SAUDER HAS NO OBLIGATION TO PROVIDE MAINTENANCE,
// SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
//
//=============================================================================

#ifndef DW_NNTP_H
#define DW_NNTP_H

#include <stdio.h>

#ifndef DW_CONFIG_H
#include <mimelib/config.h>
#endif

#ifndef DW_PROTOCOL_H
#include <mimelib/protocol.h>
#endif

#ifndef DW_STRING_H
#include <mimelib/string.h>
#endif


//=============================================================================
//+ Name DwNntpClient -- Class for handling the client side of an NNTP session
//+ Description
//. {\tt DwNntpClient} is a class that handles the client side of an NNTP session.
//. Specifically, {\tt DwNntpClient} provides facilities for opening a connection
//. to an NNTP server, sending commands and data to the server, receiving
//. responses from the server, and closing the connection.
//.
//. {\tt DwNntpClient} is derived from {\tt DwProtocolClient}.  For information
//. about inherited member functions, especially member functions for detecting
//. failures or errors, see the man page for {\tt DwProtocolClient}.
//.
//. In an NNTP session, the client sends simple commands to the server and
//. receives back server responses.  A client command consists of a command
//. word and zero or more argument words.  A server response consists of a
//. status line and possibly some additional lines of text.  The status line
//. consists of a three-digit numeric reply code followed by text information.
//. The reply code always indicates success or failure.  {\tt DwNntpClient}
//. provides facilities for you to send commands to the server and receive
//. responses from the server.
//.
//. To open a connection to the server, call the member function {\tt Open()}
//. with the name of the server.  {\tt Open()} accepts an optional argument
//. that specifies the TCP port that the server listens to.  The default port
//. is the standard NNTP port (119).  {\tt Open()} may fail, so you should check
//. the return value to verify that it succeeded.  To close the connection, call
//. the inherited member function {\tt DwProtocolClient::Close()}.  To check if
//. a connection is open, call the inherited member function
//. {\tt DwProtocolClient::IsOpen()}.  {\tt IsOpen()} returns a value that
//. indicates whether or not a call to {\tt Open()} was successful; it will
//. not detect failure in the network or a close operation by the remote host.
//.
//. For each NNTP command, {\tt DwNntpClient} has a member function that sends
//. that command and receives the server response.  If the command takes
//. arguments, then those arguments are passed as function arguments to the
//. member function.  The command member functions return an integer that is
//. the numeric code returned by the server.  Your program must check the
//. returned reply code to determine whether or not the command was accepted
//. and performed by the server.  In some cases, because of a communications
//. error or some other error, it is not possible for the member function to
//. send the command or receive the response.  When this happens, the member
//. function will return 0.  You can determine the precise error or failure
//. by calling the inherited member functions {\tt DwProtocolClient::LastError()}
//. or {\tt DwProtocolClient::LastFailure()}.
//.
//. The numeric reply code from the server is remembered by the {\tt DwNntpClient}
//. object; it can be retrieved by calling the member function {\tt ReplyCode()}.
//. The single-line status reponse from the server is also remembered; it can be
//. retrieved by calling the member function {\tt SingleLineReponse()}.  If no
//. status response has been received, possibly because of a communications error
//. or failure, {\tt ReplyCode()} will return zero and {\tt SingleLineResponse()}
//. will return an empty string.
//.
//. A numeric reply code and a single-line response is sent by the server for
//. all NNTP commands.  For some commands, such as when the client requests an
//. article body, the server will send a multi-line text response following the
//. single-line status response.  Multi-line text responses
//. can be received in either of two ways.  The simplest way is to just call
//. the member function {\tt MultiLineResponse()} after a command completes
//. successfully.  This simple method works fine for non-interactive applications.
//. It can be a problem in interactive applications, however, because there is no
//. data to display to a user until the entire multi-line response is retrieved.
//. An alternative way to receive a multi-line response is to retrieve it one
//. line at a time by assigning a {\tt DwObserver} object to the {\tt DwNntpClient}.
//. The abstract class {\tt DwObserver}, defined in protocol.h, contains just one
//. virtual member function, {\tt Notify()}, which is called by the
//. {\tt DwNntpClient} object after each line is received.  To receive the
//. multi-line response one line at a time, define your own subclass of
//. {\tt DwObserver} and override the virtual function {\tt Notify()} to call
//. the {\tt MultiLineResponse()} member function of the {\tt DwNntpClient}
//. object.  To do this, you will probably want to save a pointer to the
//. {\tt DwNntpClien} object as a data member in your {\tt DwObserver} subclass.
//. Then assign the {\tt DwObserver} object by calling the member function
//. {\tt SetObserver()}.
//.
//. (More to follow)
//=============================================================================


class DwNntpClient : public DwProtocolClient {

//This is just temporary
friend class NNTP;

public:

    enum {
        kCmdNoCommand,
        kCmdArticle,
        kCmdBody,
        kCmdHead,
        kCmdStat,
        kCmdGroup,
        kCmdkHelp,
        kCmdIhave,
        kCmdLast,
        kCmdList,
        kCmdNewgroups,
        kCmdNewnews,
        kCmdNext,
        kCmdPost,
        kCmdQuit,
        kCmdSlave
    };

    DwNntpClient();
    //. Initializes the {\tt DwNntpClient} object.
    //.
    //. It is possible that the constructor may fail.  To verify that the
	//. constructor has succeeded, call the member function {\tt LastError()}
    //. and check that it returns zero.  (In the WIN32 implementation, the
    //. constructor calls the Winsock function {\tt WSAStartup()}.)

    ~DwNntpClient();

    virtual int Open(const char* aServer, DwUint16 aPort=119);
    //. Opens a connection to the server {\tt aServer} at TCP port {\tt aPort}.
    //. {\tt aServer} may be either a host name, such as "news.acme.com" or
    //. an IP number in dotted decimal format, such as "129.32.20.16".  The
    //. default value for {\tt aPort} is 119, the well-known port for NNTP
    //. assigned by the Internet Assigned Numbers Authority (IANA).
    //.
    //. If the connection attempt succeeds, the server will send back a response.
    //. {\tt Open()} returns the server's numeric reply code, which should have a
    //. 2 as its first digit.  The full response from the server can be retrieved
    //. by calling {\tt SingleLineResponse()}.
    //.
    //. If the connection attempt fails, {\tt Open()} returns 0.  To determine
    //. what error occurred when a connection attempt fails, call the inherited
    //. member function {\tt DwProtocolClient::LastError()}.  To determine if
    //. a failure also occurred, call the inherited member function
    //. {\tt DwProtocolClient::LastFailure()}.

    DwObserver* SetObserver(DwObserver* aObserver);
    //. Sets the observer object that interacts with the {\tt DwNntpClient}
    //. to retrieve a multi-line response.  If an observer is set,
    //. {\tt DwNntpClient} will call the observer's {\tt Notify()} method
    //. after each line of the multi-line response is received.  To remove
    //. an observer, call {\tt SetObserver()} with a NULL argument.
    //. {\tt SetObserver()} returns the previously set observer, or NULL if
    //. no observer was previously set.

    int LastCommand() const;
    //. Returns the last command sent to the server.  This member function is
    //. useful when an observer is used to retrieve multi-line responses from
    //. the server.

    int ReplyCode() const;
    //. Returns the reply code received from the server in response to the last
    //. client command.  If no response was received, perhaps because of a
    //. communications failure, {\tt ReplyCode()} returns zero.

    const DwString& SingleLineResponse() const;
    //. Returns the single-line status response last received from the server.
    //. If no response was received, perhaps because of a communications failure,
    //. {\tt SingleLineResponse} returns an empty string.

    const DwString& MultiLineResponse() const;
    //. If no observer is set for this object, {\tt MultiLineResponse} returns
    //. a string that comprises the entire sequence of lines received from
    //. the server, if any.  Otherwise, if an observer is set for this object,
    //. {\tt MultiLineResponse} returns only the most recent line received.

    int Article(int aNumber=(-1));
    //. Sends the NNTP ARTICLE command and returns the reply code received from
    //. the server.  The optional argument {\tt aNumber} specifies the number
    //. of an article to retrieve.  If no response is received, perhaps because
    //. of an error, the function returns zero.

    int Article(const char* aMsgid);

    int Body(int aNumber=(-1));
    //. Sends the NNTP BODY command and returns the reply code received from
    //. the server.  The optional argument {\tt aNumber} specifies the number
    //. of an article to retrieve the body from.  If no response is received,
    //. perhaps because of an error, the function returns zero.

    int Body(const char* aMsgid);

    int Head(int aNumber=(-1));
    //. Sends the NNTP HEAD command and returns the reply code received from
    //. the server.  The optional argument {\tt aNumber} specifies the number
    //. of an article to retrieve the header lines from.  If no response is
    //. received, perhaps because of an error, the function returns zero.

    int Head(const char* aMsgid);

    int Stat(int aNumber=(-1));
    //. Sends the NNTP STAT command and returns the reply code received from
    //. the server.  The optional argument {\tt aNumber} specifies the number
    //. of an article to retrieve the header lines from.  If no response is
    //. received, perhaps because of an error, the function returns zero.

    int Stat(const char* aMsgid);

    int Group(const char* aNewsgroupName);
    //. Sends the NNTP GROUP command and returns the reply code received from
    //. the server.  The argument {\tt aNewsgroupName} specifies the newgroup
    //. to be selected.  If no response is received, perhaps because of an
    //. error, the function returns zero.

    int Help();
    //. Sends the NNTP HELP command and returns the reply code received from
    //. the server.  If no response is received, perhaps because of an error,
    //. the function returns zero.

    int Ihave(const char* aMsgId);

    int Last();
    //. Sends the NNTP LAST command and returns the reply code received from
    //. the server.  If no response is received, perhaps because of an error,
    //. the function returns zero.

    int List();
    //. Sends the NNTP LIST command and returns the reply code received from
    //. the server.  If no response is received, perhaps because of an error,
    //. the function returns zero.

    int Newgroups(const char* aDate, const char* aTime,
        DwBool aIsGmt=0, const char* aDistribution=0);
    int Newnews(const char* aNewsgroups, const char* aDate,
        const char* aTime, DwBool aIsGmt=0, const char* aDistribution=0);

    int Next();
    //. Sends the NNTP NEXT command and returns the reply code received from
    //. the server.  If no response is received, perhaps because of an error,
    //. the function returns zero.

    int Post();
    //. Sends the NNTP POST command and returns the reply code received from
    //. the server.  If no response is received, perhaps because of an error,
    //. the function returns zero.

    int Quit();
    //. Sends the NNTP QUIT command and returns the reply code received from
    //. the server.  If no response is received, perhaps because of an error,
    //. the function returns zero.

    int Slave();
    //. Sends the NNTP SLAVE command and returns the reply code received from
    //. the server.  If no response is received, perhaps because of an error,
    //. the function returns zero.

    //int Date();

    int SendData(const DwString& aStr);
    int SendData(const char* aBuf, int aBufLen);

private:

    char*       mSendBuffer;
    char*       mRecvBuffer;
    int         mLastChar;
    int         mLastLastChar;
    int         mNumRecvBufferChars;
    int         mRecvBufferPos;
    int         mReplyCode;
    DwString    mSingleLineResponse;
    DwString    mMultiLineResponse;
    DwObserver* mObserver;

    int PGetLine(char** aPtr, int* aLen);
    void PGetSingleLineResponse();
    void PGetMultiLineResponse();

};

#endif
