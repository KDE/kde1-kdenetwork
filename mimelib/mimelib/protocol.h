//=============================================================================
// File:       proto_un.h
// Contents:   Declarations for DwClientProtocol
// Maintainer: Doug Sauder <dwsauder@fwb.gulf.net>
// WWW:        http://www.fwb.gulf.net/~dwsauder/<<mimepp.html
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

#ifndef DW_PROTOCOL_H
#define DW_PROTOCOL_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#ifndef DW_CONFIG_H
#include <mimelib/config.h>
#endif

#ifndef DW_STRING_H
#include <mimelib/string.h>
#endif


class DwProtocolClient {

public:

    virtual ~DwProtocolClient();

    int Close();
    //. Closes the connection to the server.

    DwBool IsOpen() const;
    //. Returns true value if the connection to the server is open.

    int LastError() const;
    //. Returns error code for the last error that occurred.

protected:

    DwProtocolClient();
    //. Initializes the {\tt DwProtocolClient} object.
    //. 
    //. In a Windows environment, this constructor also opens the Winsock DLL.
    //. To verify that the DLL was opened okay, call the member function 
    //. {\tt LastError()} and verify that it returns zero.

    virtual int Open(const char* aServer, DwUint16 aPort);
    //. Opens a connection to the server specified by {\tt aServer}
    //. at the port specified by {\tt aPort}.  If the object is unable
    //. to open a connection, it returns -1.  To determine what error
    //. occurred when the connection attempt fails, call the member
    //. function {\tt LastError()}.

    int Send(const char* aBuf, int aBufLen);
    //. Sends {\tt aBufLen} characters from the buffer {\tt aBuf}.  Returns
    //. the number of characters sent.  If the number of characters sent
    //. is less than the number of characters specified in {\tt aBufLen},
    //. the caller should call {\tt LastError()} to determine the reason.

    int Receive(char* aBuf, int aBufSize);
    //. Receives up to {\tt aBufSize} characters into the buffer {\tt aBuf}.
    //. Returns the number of characters received.  If zero is returned, the
    //. caller should call the member function {\tt LastError()} to determine
    //. what, if any, error occurred.

    DwBool   mIsDllOpen;
    DwBool   mIsOpen;
    int      mSocket;
    DwUint16 mPort;
    char*    mServerName;
    struct sockaddr_in mServerAddr;
    int      mErrorCode;

};

#endif
