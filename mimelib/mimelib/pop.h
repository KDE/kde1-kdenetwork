//=============================================================================
// File:       pop.h
// Contents:   Declarations for DwPopClient
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

#ifndef DW_POP_H
#define DW_POP_H

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


class DwPopClient : public DwProtocolClient {

public:
   
    DwPopClient();

    ~DwPopClient();

    virtual int Open(const char* aServer, DwUint16 aPort=110);
    DwObserver* SetObserver(DwObserver* aObserver);

    // Server reponses

    int ReplyCode() const;
    const DwString& SingleLineResponse() const;
    const DwString& MultiLineResponse() const;

    // POP commands

    int User(const char* aName);
    //. Sends a USER command to the server.  {\tt aName} is the name of the user
    //. that is sent in the command.
	//. The return value will be '+' if the command is successful at the server,
    //. '-' if the command is unsuccessful at the server, or -1 if an error
    //. occurs.  The complete response returned by the server can be obtained
    //. by calling the member function {\tt SingleLineResponse()} immediately
    //. after a call to this member function.  To get more information about
    //. an error indicated by a return value of -1, call
    //. {\tt DwProtocolClient::LastError()}.

    int Pass(const char* aPasswd);
    //. Sends a PASS command to the server.  {\tt aPasswd} is the password that
    //. is sent in the command.
	//. The return value will be '+' if the command is successful at the server,
    //. '-' if the command is unsuccessful at the server, or -1 if an error
    //. occurs.  The complete response returned by the server can be obtained
    //. by calling the member function {\tt SingleLineResponse()} immediately
    //. after a call to this member function.  To get more information about
    //. an error indicated by a return value of -1, call
    //. {\tt DwProtocolClient::LastError()}.

    int Quit();
    //. Sends a QUIT command to the server.
	//. The return value will be '+' if the command is successful at the server,
    //. '-' if the command is unsuccessful at the server, or -1 if an error
    //. occurs.  The complete response returned by the server can be obtained
    //. by calling the member function {\tt SingleLineResponse()} immediately
    //. after a call to this member function.  To get more information about
    //. an error indicated by a return value of -1, call
    //. {\tt DwProtocolClient::LastError()}.

    int Stat();
    //. Sends a STAT command to the server.
	//. The return value will be '+' if the command is successful at the server,
    //. '-' if the command is unsuccessful at the server, or -1 if an error
    //. occurs.  The complete response returned by the server can be obtained
    //. by calling the member function {\tt SingleLineResponse()} immediately
    //. after a call to this member function.  To get more information about
    //. an error indicated by a return value of -1, call
    //. {\tt DwProtocolClient::LastError()}.

    int List();
    //. Sends a LIST command, without a message number, to the server.
    //. The return value will be '+' if the command is successful at the server,
    //. '-' if the command is unsuccessful at the server, or -1 if an error
    //. occurs.  The complete response returned by the server can be obtained
    //. by calling the member functions {\tt SingleLineResponse()}, which returns
    //. the initial response from the server, and {\tt MultiLineResponse()},
    //. which returns the multiline response that follows.  The multiline
    //. response will be emtpy if the command fails.  The multiline response
    //. may optionally be received interactively by an observer: see the
    //. documentation entry for {\tt SetObserver}.  To get more information
    //. about an error indicated by a return value of -1, call
    //. {\tt DwProtocolClient::LastError()}.

    int List(int aMsg);
    //. Sends a LIST command, with {\tt aMsg} as the message number, to the server.
	//. The return value will be '+' if the command is successful at the server,
    //. '-' if the command is unsuccessful at the server, or -1 if an error
    //. occurs.  The complete response returned by the server can be obtained
    //. by calling the member function {\tt SingleLineResponse()} immediately
    //. after a call to this member function.  To get more information about
    //. an error indicated by a return value of -1, call
    //. {\tt DwProtocolClient::LastError()}.

    int Retr(int aMsg);
    //. Sends a RETR command, with {\tt aMsg} as the message number, to the server.
    //. The return value will be '+' if the command is successful at the server,
    //. '-' if the command is unsuccessful at the server, or -1 if an error
    //. occurs.  The complete response returned by the server can be obtained
    //. by calling the member functions {\tt SingleLineResponse()}, which returns
    //. the initial response from the server, and {\tt MultiLineResponse()},
    //. which returns the multiline response that follows.  The multiline
    //. response will be emtpy if the command fails.  The multiline response
    //. may optionally be received interactively by an observer: see the
    //. documentation entry for {\tt SetObserver}.  To get more information
    //. about an error indicated by a return value of -1, call
    //. {\tt DwProtocolClient::LastError()}.

    int Dele(int aMsg);
    //. Sends a DELE command, with {\tt aMsg} as the message number, to the server.
	//. The return value will be '+' if the command is successful at the server,
    //. '-' if the command is unsuccessful at the server, or -1 if an error
    //. occurs.  The complete response returned by the server can be obtained
    //. by calling the member function {\tt SingleLineResponse()} immediately
    //. after a call to this member function.  To get more information about
    //. an error indicated by a return value of -1, call
    //. {\tt DwProtocolClient::LastError()}.

    int Noop();
    //. Sends a NOOP command to the server.
	//. The return value will be '+' if the command is successful at the server,
    //. '-' if the command is unsuccessful at the server, or -1 if an error
    //. occurs.  The complete response returned by the server can be obtained
    //. by calling the member function {\tt SingleLineResponse()} immediately
    //. after a call to this member function.  To get more information about
    //. an error indicated by a return value of -1, call
    //. {\tt DwProtocolClient::LastError()}.

    int Rset();
    //. Sends a RSET command to the server.
	//. The return value will be '+' if the command is successful at the server,
    //. '-' if the command is unsuccessful at the server, or -1 if an error
    //. occurs.  The complete response returned by the server can be obtained
    //. by calling the member function {\tt SingleLineResponse()} immediately
    //. after a call to this member function.  To get more information about
    //. an error indicated by a return value of -1, call
    //. {\tt DwProtocolClient::LastError()}.

    int Apop(const char* aName, const char* aDigest);
    //. Sends an APOP command, with {\tt aName} as the name and {\tt aDigest}
    //. as the digest, to the server.
	//. The return value will be '+' if the command is successful at the server,
    //. '-' if the command is unsuccessful at the server, or -1 if an error
    //. occurs.  The complete response returned by the server can be obtained
    //. by calling the member function {\tt SingleLineResponse()} immediately
    //. after a call to this member function.  To get more information about
    //. an error indicated by a return value of -1, call
    //. {\tt DwProtocolClient::LastError()}.

    int Top(int aMsg, int aNumLines);
    //. Sends a TOP command, with {\tt aMsg} as the message number and
    // {\tt aNumLines} as the number of lines, to the server.
    //. The return value will be '+' if the command is successful at the server,
    //. '-' if the command is unsuccessful at the server, or -1 if an error
    //. occurs.  The complete response returned by the server can be obtained
    //. by calling the member functions {\tt SingleLineResponse()}, which returns
    //. the initial response from the server, and {\tt MultiLineResponse()},
    //. which returns the multiline response that follows.  The multiline
    //. response will be emtpy if the command fails.  The multiline response
    //. may optionally be received interactively by an observer: see the
    //. documentation entry for {\tt SetObserver}.  To get more information
    //. about an error indicated by a return value of -1, call
    //. {\tt DwProtocolClient::LastError()}.

    int Uidl();
    //. Sends a UIDL command, without a message number, to the server.
    //. The return value will be '+' if the command is successful at the server,
    //. '-' if the command is unsuccessful at the server, or -1 if an error
    //. occurs.  The complete response returned by the server can be obtained
    //. by calling the member functions {\tt SingleLineResponse()}, which returns
    //. the initial response from the server, and {\tt MultiLineResponse()},
    //. which returns the multiline response that follows.  The multiline
    //. response will be emtpy if the command fails.  The multiline response
    //. may optionally be received interactively by an observer: see the
    //. documentation entry for {\tt SetObserver}.  To get more information
    //. about an error indicated by a return value of -1, call
    //. {\tt DwProtocolClient::LastError()}.

    int Uidl(int aMsg);
    //. Sends a UIDL command, with {\tt aMsg} as the message number, to the server.
	//. The return value will be '+' if the command is successful at the server,
    //. '-' if the command is unsuccessful at the server, or -1 if an error
    //. occurs.  The complete response returned by the server can be obtained
    //. by calling the member function {\tt SingleLineResponse()} immediately
    //. after a call to this member function.  To get more information about
    //. an error indicated by a return value of -1, call
    //. {\tt DwProtocolClient::LastError()}.

private:

    char*       mSendBuffer;
    char*       mRecvBuffer;
    int         mNumRecvBufferChars;
    int         mRecvBufferPos;
    int         mReplyCode;
    DwString    mSingleLineResponse;
    DwString    mMultiLineResponse;
    DwObserver* mObserver;

    int PGetLine(char** aPtr, int* aLen);
    // Tries to get one complete line of input from the socket.  On success,
    // the function sets {\tt *aPtr} to point to the beginning of the line in
    // the object's internal buffer, sets {\tt *aLen} to the length of the
    // line, including the CR LF, and returns 0.  On failure, the function
    // returns -1.

    void PGetSingleLineResponse();
    // Gets a single line of input, assigns that line {\tt mSingleLineResponse}, and
    // sets {\tt mReplyCode}.  On failure, clears {\tt mSingleLineResonse}
    // and sets {\tt mReplyCode} to -1.

    void PGetMultiLineResponse();
    // Gets a complete multiline response and assigns it to {\tt mMultiLineResponse},
    // or interacts with the {\tt DwObserver} object to deliver a multiline response
    // one line at a time.
    // If an error occurs, its sets {\tt mReplyCode} to -1.

};

#endif
