//=============================================================================
// File:       proto_un.cpp
// Contents:   Definitions for DwClientProtocol
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

// Comments:
//
// 1. The program should handle the SIGPIPE signal. Ignoring it should be okay.
//
// 2. The recv() and send() system calls are *not* restarted if they are
//    interrupted by a signal. This behavior is necessary if we want to
//    be able to timeout a blocked call.
//
// 3. Should we set the SO_SNDTIMEO or SO_RCVTIMEO socket options?

#define DW_IMPLEMENTATION

#include <mimelib/config.h>
#include <mimelib/protocol.h>
#include <stdio.h>
#include <string.h>
#include <iostream.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <errno.h>

#ifndef INADDR_NONE
#define INADDR_NONE (-1)
#endif


DwProtocolClient::DwProtocolClient()
{
    mIsOpen = 0;
	mSocket = -1;
	mServerName = 0;
    memset(&mServerAddr, 0, sizeof(struct sockaddr_in));
    mErrorCode = 0;
}


DwProtocolClient::~DwProtocolClient()
{
    if (mIsOpen) {
        Close();
    }
}


DwBool DwProtocolClient::IsOpen() const
{
    return mIsOpen;
}


int DwProtocolClient::LastError() const
{
    return mErrorCode;
}


int DwProtocolClient::Open(const char* aServer, DwUint16 aPort)
{
    mErrorCode = 0;
    if (mIsOpen) {
        // error!
        return -1;
    }
    if (aServer == 0 || aServer[0] == 0) {
        // error!
        return -1;
    }
    if (mServerName) {
        delete [] mServerName;
        mServerName = 0;
    }
    mServerName = new char[strlen(aServer)+1];
    strcpy(mServerName, aServer);
    mPort = aPort;

    // Complete the server name structure.
    // First assume address is in dotted-decimal notation.  If that fails, do
    // a hostname lookup.

    memset(&mServerAddr, 0, sizeof(struct sockaddr_in));
    mServerAddr.sin_family = AF_INET;
    mServerAddr.sin_port = htons(mPort);
    mServerAddr.sin_addr.s_addr = inet_addr(mServerName);
    if (mServerAddr.sin_addr.s_addr != INADDR_NONE) {
        // for debugging
        cout << "Opening connection to " << mServerName << endl;
    }
    else {
        struct hostent* hostentp = gethostbyname(mServerName);
        if (hostentp == NULL) {
            // error!
            mErrorCode = h_errno;
            return -1;
        }
        struct in_addr* in_addrp = (struct in_addr*)*hostentp->h_addr_list;
        memcpy(&mServerAddr.sin_addr.s_addr, in_addrp, sizeof(struct in_addr));
        // for debugging
        cout << "Opening connection to " << mServerName
            << " (" << inet_ntoa(*in_addrp) << ')' << endl;
    }

    // Open the socket
    mSocket = socket(PF_INET, SOCK_STREAM, 0);
    if (mSocket == -1) {
        // error!
        mErrorCode = errno;
        return -1;
    }

    // Establish a connection to the server
    int err = connect(mSocket, (struct sockaddr*)&mServerAddr,
        sizeof(struct sockaddr_in));
    if (err == -1) {
        // error!
        mErrorCode = errno;
        return -1;
    }
    // for debugging
    cout << "Connection okay" << endl;
    mIsOpen = 1;
    return 0;
}


int DwProtocolClient::Close()
{
    mErrorCode = 0;
    if (! mIsOpen) {
        return 0;
    }
    int err = close(mSocket);
    if (err < 0) {
        mErrorCode = errno;
    }
    mIsOpen = 0;
    return 0;
}


int DwProtocolClient::Send(const char* aBuf, int aBufLen)
{
    mErrorCode = 0;
    if (! mIsOpen) {
        // error!
        return 0;
    }
    int ret;
    int numToSend = aBufLen;
    int numSent = 0;
    while (numToSend > 0) {
        ret = send(mSocket, &aBuf[numSent], numToSend, 0);
        if (ret == -1) {
            // error!
            mErrorCode = errno;
            return numSent;
        }
        else {
            numSent += ret;
            numToSend -= ret;
        }
    }
    return numSent;
}


int DwProtocolClient::Receive(char* aBuf, int aBufSize)
{
    if (! mIsOpen) {
        // error!
        return 0;
    }
    mErrorCode = 0;
    int numReceived;
    int ret = recv(mSocket, aBuf, aBufSize, 0);
    if (ret == -1) {
        // error!
        mErrorCode = errno;
        numReceived = 0;
    }
    else {
        numReceived = ret;
    }
    return numReceived;
}

