//=============================================================================
// File:       proto_un.cpp
// Contents:   Definitions for DwClientProtocol
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

// Comments:
//
// 1. The program should handle the SIGPIPE signal. Ignoring it should be okay.
//
// 2. The recv() and send() system calls are *not* restarted if they are
//    interrupted by a signal. This behavior is necessary if we want to
//    be able to timeout a blocked call.

#define DW_IMPLEMENTATION

#include <mimelib/config.h>
#include <mimelib/protocol.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <iostream.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/time.h>

#ifndef INADDR_NONE
#define INADDR_NONE (-1)
#endif

#if defined(DW_DEBUG_PROTO)
#  define DBG_PROTO_STMT(x) x
#else
#  define DBG_PROTO_STMT(x)
#endif


DwProtocolClient::DwProtocolClient()
{
    mIsOpen         = DwFalse;
    mSocket         = -1;
    mPort           = 0;
    mServerName     = 0;
    mReceiveTimeout = 90;
    mLastCommand    = 0;
    mFailureCode    = 0;
    mFailureStr     = 0;
    mErrorCode      = 0;
    mErrorStr       = 0;
}


DwProtocolClient::~DwProtocolClient()
{
    if (mIsOpen) {
        Close();
    }
    if (mServerName) {
        delete [] mServerName;
        mServerName = 0;
    }
}


int DwProtocolClient::Open(const char* aServer, DwUint16 aPort)
{
    mFailureCode = kFailNoFailure;
    mErrorCode = kErrNoError;

    if (mIsOpen) {
        // error!
        mErrorCode = kErrBadUsage;
        return -1;
    }
    if (aServer == 0 || aServer[0] == 0) {
        // error!
        mErrorCode = kErrBadParameter;
        return -1;
    }
    if (mServerName) {
        delete [] mServerName;
        mServerName = 0;
    }
    mServerName = new char[strlen(aServer)+1];
    strcpy(mServerName, aServer);
    mPort = aPort;

    // Open the socket

    mSocket = socket(PF_INET, SOCK_STREAM, 0);
    if (mSocket == -1) {
        // error!
        int err = errno;
        HandleError(err, ksocket);
        return -1;
    }

    // If the server is specified by an IP number in dotted decimal form,
    // then try to connect to that IP number.

    int err = -1;
    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(struct sockaddr_in));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(mPort);
    serverAddr.sin_addr.s_addr = inet_addr(mServerName);
    if (serverAddr.sin_addr.s_addr != INADDR_NONE) {
        DBG_PROTO_STMT(cout << "Trying connection to " << mServerName << endl;)
        err = connect(mSocket, (struct sockaddr*)&serverAddr,
            sizeof(struct sockaddr_in));
    }

    // Otherwise, do a host name lookup.

    else {
        struct hostent* hostentp = gethostbyname(mServerName);
        if (hostentp == NULL) {
            close(mSocket);
            mSocket = -1;
            // error!
            int err = h_errno;
            HandleError(err, kgethostbyname);
            return -1;
        }

        // Connect to the server.  Try each IP number until one succeeds.

        char** addr_list = hostentp->h_addr_list;
        while (*addr_list) {
            struct in_addr* in_addrp = (struct in_addr*)*addr_list;
            memcpy(&serverAddr.sin_addr.s_addr, in_addrp, sizeof(struct in_addr));
            DBG_PROTO_STMT(cout << "Trying connection to " << mServerName;)
            DBG_PROTO_STMT(cout << " (" << inet_ntoa(*in_addrp) << ')' << endl;)
            err = connect(mSocket, (struct sockaddr*)&serverAddr,
                sizeof(struct sockaddr_in));
            if (err != -1) {
                break;
            }
            ++addr_list;
        }
    }

    if (err == -1) {
        close(mSocket);
        mSocket = -1;
        // error!
        mErrorCode = errno;
        HandleError(err, kconnect);
        return -1;
    }
    DBG_PROTO_STMT(cout << "Connection okay" << endl;)
    mIsOpen = DwTrue;
    return 0;
}


DwBool DwProtocolClient::IsOpen() const
{
    return mIsOpen;
}


int DwProtocolClient::Close()
{
    mFailureCode = kFailNoFailure;
    mErrorCode = kErrNoError;

    if (! mIsOpen) {
        // error!
        mErrorCode = kErrBadUsage;
        return -1;
    }
    int err = close(mSocket);
    if (err < 0) {
        // error!
        int err = errno;
        HandleError(err, kclosesocket);
        return -1;
    }
    mIsOpen = DwFalse;
    return 0;
}


int DwProtocolClient::SetReceiveTimeout(int aSecs)
{
    mReceiveTimeout = aSecs;
    return 0;
}


int DwProtocolClient::LastCommand() const
{
    return mLastCommand;
}


int DwProtocolClient::LastFailure() const
{
    return mFailureCode;
}


const char* DwProtocolClient::LastFailureStr() const
{
    return mFailureStr;
}


int DwProtocolClient::LastError() const
{
    return mErrorCode;
}


const char* DwProtocolClient::LastErrorStr() const
{
    return mErrorStr;
}


int DwProtocolClient::PSend(const char* aBuf, int aBufLen)
{
    mFailureCode = kFailNoFailure;
    mErrorCode = kErrNoError;

    if (! mIsOpen) {
        // error!
        mErrorCode = kErrBadUsage;
        return 0;
    }
    int ret;
    int numToSend = aBufLen;
    int numSent = 0;
    while (numToSend > 0) {
        ret = send(mSocket, &aBuf[numSent], numToSend, 0);
        if (ret == -1) {
            // error!
            int err = errno;
            HandleError(err, ksend);
            break;
        }
        else {
            numSent += ret;
            numToSend -= ret;
        }
    }
    return numSent;
}


int DwProtocolClient::PReceive(char* aBuf, int aBufSize)
{
    mFailureCode = kFailNoFailure;
    mErrorCode = kErrNoError;

    if (! mIsOpen) {
        // error!
        mErrorCode = kErrBadUsage;
        return 0;
    }

    // Suspend until there's input to read

    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(mSocket, &readfds);
    struct timeval timeout;
    timeout.tv_sec = mReceiveTimeout;
    timeout.tv_usec = 0;
    int numFds = select(mSocket+1, &readfds, 0, 0, &timeout);

    // Read the input, if available

    int numReceived = 0;
    if (numFds == 1) {
        int ret = recv(mSocket, aBuf, aBufSize, 0);
        if (ret == -1) {
            // error!
            int err = errno;
            HandleError(err, krecv);
            numReceived = 0;
        }
        else /* if (ret != -1) */ {
            numReceived = ret;
        }
    }

    // Otherwise, there was a timeout, error, or an interruption by a signal

    else /* if (numFds != 1)*/ {
        if (numFds == 0) {
            DBG_PROTO_STMT(cout << "Receive timed out" << endl;)
        }
        numReceived = 0;
    }

    return numReceived;
}


void DwProtocolClient::HandleError(int aErrorCode, int aSystemCall)
{
    // To do: this function is not finished
    mErrorCode = aErrorCode;
}
