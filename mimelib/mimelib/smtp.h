//=============================================================================
// File:       smtp.h
// Contents:   Declarations for DwSmtpClient
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

#ifndef DW_SMTP_H
#define DW_SMTP_H

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
//+ Name DwSmtpClient -- Class for handling the client side of an SMTP session
//+ Description
//. {\tt DwSmtpClient} is a class that handles the client side of an SMTP session.
//. Specifically, {\tt DwSmtpClient} provides facilities for opening a connection
//. to an SMTP server, sending commands and data to the server,  and receiving
//. responses from the server.
//.
//. {\tt DwSmtpClient} is derived from {\tt DwProtocolClient}.  For information
//. about inherited member functions, especially member functions for detecting
//. failures or errors, see the man page for {\tt DwProtocolClient}.
//.
//. SMTP is a simple protocol in which the client sends simple commands to the
//. server and the server sends back a simple response.  Every client command
//. consists of a command word and possibly an argument.  Every server response
//. consists of a three-digit numeric reply code and a descriptive phrase.
//. {\tt DwSmtpClient} provides facilities for you to send commands to the server
//. and receive responses from the server.  {\tt DwSmtpClient} does not force you
//. to send commands in the proper order -- that is your responsibility.
//.
//. To open a connection to the server, call the member function {\tt Open()}
//. with the name of the server.  {\tt Open()} accepts an optional argument
//. that specifies the TCP port that the server listens to.  The default port
//. is the standard SMTP port (25).  {\tt Open()} may fail, so you should check
//. the return value to verify that it succeeded.  To close the connection, call
//. the inherited member function {\tt DwProtocolClient::Close()}.  To check if
//. a connection is open, call the inherited member function
//. {\tt DwProtocolClient::IsOpen()}.  {\tt IsOpen()} returns a value that
//. indicates whether or not a call to {\tt Open()} was successful; it will
//. not detect failure in the network or a close operation by the remote host.
//.
//. For each SMTP command, {\tt DwSmtpClient} has a member function that sends
//. that command and receives the server response.  If the command takes an
//. argument, then that argument is passed as a function argument to the
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
//. The numeric reply code from the server is remembered by the {\tt DwSmtpClient}
//. object; the reply code from the last command sent may be retrieved by calling
//. the member function {\tt ReplyCode()}.  The entire reponse from the server is
//. also remembered; the reponse to the last command sent can be retrieved
//. by calling the member function {\tt SingleLineReponse()}.  If no response
//. has been received, possibly because of a communications error or failure,
//. {\tt ReplyCode()} will return zero and {\tt SingleLineResponse()} will return
//. an empty string.
//.
//. Following the successful response to a DATA command, an SMTP client sends
//. bulk data containing a message to be sent to the server.  To perform this
//. bulk data transfer, {\tt DwSmtpClient} provides the member function
//. {\tt SendData()}.  In the current implementation, {\tt SendData()} does
//. not convert end of line characters, so it is your responsibility to convert
//. the end of line characters to CR LF, if necessary.  (You may use the utility
//. function {\tt DwToCrLfEol()} to do the conversion.)  {\tt SendData()} will
//. perform the character stuffing to protect '.' at the beginning of a line,
//. and it will append the final [CR LF] '.' CR LF.  It is possible to divide
//. data and make multiple calls to {\tt SendData()}; however, if you do so,
//. please note the following paragraph.
//.
//. Note: Because of a feature (some would say bug) in the current implementation,
//. {\tt SendData()} will not detect a '.' at the beginning of a line if the
//. CR LF '.' sequence is split between two calls to {\tt SendData()}.  This
//. problem will probably be resolved in a future version, but be aware that
//. such a change will require a change in {\tt DwSmtpClient}'s interface.
//=============================================================================


class DwSmtpClient : public DwProtocolClient {

public:

    DwSmtpClient();
    //. Initializes the {\tt DwSmtpClient} object.
    //.
    //. It is possible that the constructor may fail.  To verify that the
	//. constructor has succeeded, call the member function {\tt LastError()}
    //. and check that it returns zero.  (In the WIN32 implementation, the
    //. constructor calls the Winsock function {\tt WSAStartup()}.)

    ~DwSmtpClient();

    virtual int Open(const char* aServer, DwUint16 aPort=25);
    //. Opens a connection to the server {\tt aServer} at TCP port {\tt aPort}.
    //. {\tt aServer} may be either a host name, such as "smtp.acme.com" or
    //. an IP number in dotted decimal format, such as "129.32.20.16".  The
    //. default value for {\tt aPort} is 25, the well-known port for SMTP
    //. assigned by the Internet Assigned Numbers Authority (IANA).
    //.
    //. If the connection attempt succeeds, the server will send back a response.
    //. {\tt Open()} returns the server's numeric reply code, which should be
    //. 220.  The full response from the server can be retrieved by calling
    //. {\tt SingleLineResponse()}.
    //.
    //. If the connection attempt fails, {\tt Open()} returns 0.  To determine
    //. what error occurred when a connection attempt fails, call the inherited
    //. member function {\tt DwProtocolClient::LastError()}.  To determine if
    //. a failure also occurred, call the inherited member function
    //. {\tt DwProtocolClient::LastFailure()}.

    int ReplyCode() const;
    //. Returns the reply code received from the server in response to the last
    //. client command.  If no response was received, perhaps because of a
    //. communications failure, {\tt ReplyCode()} returns zero.

    const DwString& SingleLineResponse() const;
    //. Returns the response last received from the server.  If no response was
    //. received, perhaps because of a communications failure,
    //. {\tt SingleLineResponse} returns an empty string.

    int Helo();
    //. Sends the SMTP HELO command and returns the reply code received from
    //. the server.  If no response is received, perhaps because of an error,
    //. the function returns zero.

    int Mail(const char* aFrom);
    //. Sends the SMTP MAIL command with {\tt aFrom} as the sender and returns
    //. the reply code received from the server.  If no response is received,
    //. perhaps because of an error, the function returns zero.

    int Rcpt(const char* aTo);
    //. Sends the SMTP RCPT command with {\tt aTo} as the recipient and returns
    //. the reply code received from the server.  If no response is received,
    //. perhaps because of an error, the function returns zero.

    int Data();
    //. Sends the SMTP DATA command and returns the reply code received from
    //. the server.  If no response is received, perhaps because of an error,
    //. the function returns zero.

    int Rset();
    //. Sends the SMTP RSET command and returns the reply code received from
    //. the server.  If no response is received, perhaps because of an error,
    //. the function returns zero.

    int Send(const char* aFrom);
    //. Sends the SMTP SEND command with {\tt aFrom} as the sender and returns
    //. the reply code received from the server.  If no response is received,
    //. perhaps because of an error, the function returns zero.

    int Soml(const char* aFrom);
    //. Sends the SMTP SOML command with {\tt aFrom} as the sender and returns
    //. the reply code received from the server.  If no response is received,
    //. perhaps because of an error, the function returns zero.

    int Saml(const char* aFrom);
    //. Sends the SMTP SAML command with {\tt aFrom} as the sender and returns
    //. the reply code received from the server.  If no response is received,
    //. perhaps because of an error, the function returns zero.

    int Vrfy(const char* aName);
    //. Sends the SMTP VRFY command with {\tt aName} as the argument and returns
    //. the reply code received from the server.  If no response is received,
    //. perhaps because of an error, the function returns zero.

    int Expn(const char* aName);
    //. Sends the SMTP EXPN command with {\tt aName} as the argument and returns
    //. the reply code received from the server.  If no response is received,
    //. perhaps because of an error, the function returns zero.

    int Help(const char* aArg=0);
    //. Sends the SMTP HELP command with {\tt aArg} as the argument and returns
    //. the reply code received from the server.  If no response is received,
    //. perhaps because of an error, the function returns zero.

    int Noop();
    //. Sends the SMTP NOOP command and returns the reply code received from
    //. the server.  If no response is received, perhaps because of an error,
    //. the function returns zero.

    int Quit();
    //. Sends the SMTP QUIT command and returns the reply code received from
    //. the server.  If no response is received, perhaps because of an error,
    //. the function returns zero.

    int Turn();
    //. Sends the SMTP TURN command and returns the reply code received from
    //. the server.  If no response is received, perhaps because of an error,
    //. the function returns zero.

    int SendData(const DwString& aStr);
    int SendData(const char* aBuf, int aBufLen);
    //. Sends bulk data to the server and returns the reply code received.
    //. A bulk data transfer follows a DATA command and is used to send a
    //. complete message to the server.
    //.
    //. In the current implementation, {\tt SendData()} does not convert end
    //. of line characters, so it is your responsibility to convert the end
    //. of line characters to CR LF, if necessary.  (You may use the utility
    //. function {\tt DwToCrLfEol()} to do the conversion.)  {\tt SendData()}
    //. will perform the character stuffing to protect '.' at the beginning of
    //. a line, and it will append the final [CR LF] '.' CR LF.  It is possible
    //. to divide the data and make multiple calls to {\tt SendData()}; however,
    //. this may cause problems in the current implementation if a CR LF '.'
    //. sequence is split between calls.

private:

    char*    mSendBuffer;
    char*    mRecvBuffer;
    int      mNumRecvBufferChars;
    int      mRecvBufferPos;
    int      mReplyCode;
    DwString mSingleLineResponse;

    void PGetSingleLineResponse();
    int PGetLine(char** aPtr, int* aLen);

};

#endif
