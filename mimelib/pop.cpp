//=============================================================================
// File:       pop.cpp
// Contents:   Definitions for DwPopClient
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

#define DW_IMPLEMENTATION

#include <mimelib/config.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <mimelib/pop.h>

#define POP_PORT 110
#define RECV_BUFFER_SIZE  8192
#define SEND_BUFFER_SIZE  1024

#if defined(DW_DEBUG_POP)
#  define DBG_POP_STMT(x) x
#else
#  define DBG_POP_STMT(x)
#endif


DwPopClient::DwPopClient()
{
    mSendBuffer = new char[SEND_BUFFER_SIZE];
    mRecvBuffer = new char[RECV_BUFFER_SIZE];
    mNumRecvBufferChars = 0;
    mRecvBufferPos = 0;
    mReplyCode = 0;
    mObserver = 0;
}


DwPopClient::~DwPopClient()
{
    if (mRecvBuffer) {
        delete [] mRecvBuffer;
        mRecvBuffer = 0;
    }
    if (mSendBuffer) {
        delete [] mSendBuffer;
        mSendBuffer = 0;
    }
}


int DwPopClient::Open(const char* aServer, DwUint16 aPort)
{
    mReplyCode = 0;
    mSingleLineResponse.clear();
    mMultiLineResponse.clear();
    int err = DwProtocolClient::Open(aServer, aPort);
    if (! err) {
        PGetSingleLineResponse();
    }
    return mReplyCode;
}


DwObserver* DwPopClient::SetObserver(DwObserver* aObserver)
{
    DwObserver* obs = mObserver;
    mObserver = aObserver;
    return obs;
}


int DwPopClient::ReplyCode() const
{
    return mReplyCode;
}


const DwString& DwPopClient::SingleLineResponse() const
{
    return mSingleLineResponse;
}


const DwString& DwPopClient::MultiLineResponse() const
{
    return mMultiLineResponse;
}


int DwPopClient::User(const char* aName)
{
    mReplyCode = 0;
    mSingleLineResponse.clear();
    mMultiLineResponse.clear();
    strcpy(mSendBuffer, "USER ");
    strcat(mSendBuffer, aName);
    strcat(mSendBuffer, "\r\n");
    DBG_POP_STMT(cout << "C: " << mSendBuffer << endl;)
    int bufferLen = strlen(mSendBuffer);
    int numSent = PSend(mSendBuffer, bufferLen);
    if (numSent == bufferLen) {
        PGetSingleLineResponse();
    }
    return mReplyCode;
}


int DwPopClient::Pass(const char* aPasswd)
{
    mReplyCode = 0;
    mSingleLineResponse.clear();
    mMultiLineResponse.clear();
    strcpy(mSendBuffer, "PASS ");
    strcat(mSendBuffer, aPasswd);
    strcat(mSendBuffer, "\r\n");
    DBG_POP_STMT(cout << "C: " << mSendBuffer << endl;)
    int bufferLen = strlen(mSendBuffer);
    int numSent = PSend(mSendBuffer, bufferLen);
    if (numSent == bufferLen) {
        PGetSingleLineResponse();
    }
    return mReplyCode;
}


int DwPopClient::Quit()
{
    mReplyCode = 0;
    mSingleLineResponse.clear();
    mMultiLineResponse.clear();
    strcpy(mSendBuffer, "QUIT\r\n");
    DBG_POP_STMT(cout << "C: " << mSendBuffer << endl;)
    int bufferLen = strlen(mSendBuffer);
    int numSent = PSend(mSendBuffer, bufferLen);
    if (numSent == bufferLen) {
        PGetSingleLineResponse();
    }
    return mReplyCode;
}


int DwPopClient::Stat()
{
    mReplyCode = 0;
    mSingleLineResponse.clear();
    mMultiLineResponse.clear();
    strcpy(mSendBuffer, "STAT\r\n");
    DBG_POP_STMT(cout << "C: " << mSendBuffer << endl;)
    int bufferLen = strlen(mSendBuffer);
    int numSent = PSend(mSendBuffer, bufferLen);
    if (numSent == bufferLen) {
        PGetSingleLineResponse();
    }
    return mReplyCode;
}


int DwPopClient::List()
{
    mReplyCode = 0;
    mSingleLineResponse.clear();
    mMultiLineResponse.clear();
    strcpy(mSendBuffer, "LIST\r\n");
    DBG_POP_STMT(cout << "C: " << mSendBuffer << endl;)
    int bufferLen = strlen(mSendBuffer);
    int numSent = PSend(mSendBuffer, bufferLen);
    if (numSent == bufferLen) {
        PGetSingleLineResponse();
        if (mReplyCode == '+') {
            PGetMultiLineResponse();
        }
    }
    return mReplyCode;
}


int DwPopClient::List(int aMsg)
{
    mReplyCode = 0;
    mSingleLineResponse.clear();
    mMultiLineResponse.clear();
    sprintf(mSendBuffer, "LIST %d\r\n", aMsg);
    DBG_POP_STMT(cout << "C: " << mSendBuffer << endl;)
    int bufferLen = strlen(mSendBuffer);
    int numSent = PSend(mSendBuffer, bufferLen);
    if (numSent == bufferLen) {
        PGetSingleLineResponse();
    }
    return mReplyCode;
}


int DwPopClient::Retr(int aMsg)
{
    mReplyCode = 0;
    mSingleLineResponse.clear();
    mMultiLineResponse.clear();
    sprintf(mSendBuffer, "RETR %d\r\n", aMsg);
    DBG_POP_STMT(cout << "C: " << mSendBuffer << endl;)
    int bufferLen = strlen(mSendBuffer);
    int numSent = PSend(mSendBuffer, bufferLen);
    if (numSent == bufferLen) {
        PGetSingleLineResponse();
        if (mReplyCode == '+') {
            PGetMultiLineResponse();
        }
    }
    return mReplyCode;
}


int DwPopClient::Dele(int aMsg)
{
    mReplyCode = 0;
    mSingleLineResponse.clear();
    mMultiLineResponse.clear();
    sprintf(mSendBuffer, "DELE %d\r\n", aMsg);
    DBG_POP_STMT(cout << "C: " << mSendBuffer << endl;)
    int bufferLen = strlen(mSendBuffer);
    int numSent = PSend(mSendBuffer, bufferLen);
    if (numSent == bufferLen) {
        PGetSingleLineResponse();
    }
    return mReplyCode;
}


int DwPopClient::Noop()
{
    mReplyCode = 0;
    mSingleLineResponse.clear();
    mMultiLineResponse.clear();
    strcpy(mSendBuffer, "NOOP\r\n");
    DBG_POP_STMT(cout << "C: " << mSendBuffer << endl;)
    int bufferLen = strlen(mSendBuffer);
    int numSent = PSend(mSendBuffer, bufferLen);
    if (numSent == bufferLen) {
        PGetSingleLineResponse();
    }
    return mReplyCode;
}


int DwPopClient::Rset()
{
    mReplyCode = 0;
    mSingleLineResponse.clear();
    mMultiLineResponse.clear();
    strcpy(mSendBuffer, "RSET\r\n");
    DBG_POP_STMT(cout << "C: " << mSendBuffer << endl;)
    int bufferLen = strlen(mSendBuffer);
    int numSent = PSend(mSendBuffer, bufferLen);
    if (numSent == bufferLen) {
        PGetSingleLineResponse();
    }
    return mReplyCode;
}


int DwPopClient::Apop(const char* aName, const char* aDigest)
{
    mReplyCode = 0;
    mSingleLineResponse.clear();
    mMultiLineResponse.clear();
    sprintf(mSendBuffer, "APOP %s %s\r\n", aName, aDigest);
    DBG_POP_STMT(cout << "C: " << mSendBuffer << endl;)
    int bufferLen = strlen(mSendBuffer);
    int numSent = PSend(mSendBuffer, bufferLen);
    if (numSent == bufferLen) {
        PGetSingleLineResponse();
    }
    return mReplyCode;
}


int DwPopClient::Top(int aMsg, int aNumLines)
{
    mReplyCode = 0;
    mSingleLineResponse.clear();
    mMultiLineResponse.clear();
    sprintf(mSendBuffer, "TOP %d %d\r\n", aMsg, aNumLines);
    DBG_POP_STMT(cout << "C: " << mSendBuffer << endl;)
    int bufferLen = strlen(mSendBuffer);
    int numSent = PSend(mSendBuffer, bufferLen);
    if (numSent == bufferLen) {
        PGetSingleLineResponse();
        if (mReplyCode == '+') {
            PGetMultiLineResponse();
        }
    }
    return mReplyCode;
}


int DwPopClient::Uidl()
{
    mReplyCode = 0;
    mSingleLineResponse.clear();
    mMultiLineResponse.clear();
    strcpy(mSendBuffer, "UIDL\r\n");
    DBG_POP_STMT(cout << "C: " << mSendBuffer << endl;)
    int bufferLen = strlen(mSendBuffer);
    int numSent = PSend(mSendBuffer, bufferLen);
    if (numSent == bufferLen) {
        PGetSingleLineResponse();
        if (mReplyCode == '+') {
            PGetMultiLineResponse();
        }
    }
    return mReplyCode;
}


int DwPopClient::Uidl(int aMsg)
{
    mReplyCode = 0;
    mSingleLineResponse.clear();
    mMultiLineResponse.clear();
    sprintf(mSendBuffer, "UIDL %d\r\n", aMsg);
    DBG_POP_STMT(cout << "C: " << mSendBuffer << endl);
    int bufferLen = strlen(mSendBuffer);
    int numSent = PSend(mSendBuffer, bufferLen);
    if (numSent == bufferLen) {
        PGetSingleLineResponse();
        if (mReplyCode == '+') {
            PGetMultiLineResponse();
        }
    }
    return mReplyCode;
}


void DwPopClient::PGetSingleLineResponse()
{
    mReplyCode = 0;
    mSingleLineResponse.clear();
    char* ptr;
    int len;
    int err = PGetLine(&ptr, &len);
    if (! err) {
        mReplyCode = ptr[0];
        mSingleLineResponse.assign(ptr, len);
        DBG_POP_STMT(char buffer[256];)
        DBG_POP_STMT(strncpy(buffer, ptr, len);)
        DBG_POP_STMT(buffer[len] = 0;)
        DBG_POP_STMT(cout << "S: " << buffer;)
    }
}


void DwPopClient::PGetMultiLineResponse()
{
    mMultiLineResponse.clear();

    // Get a line at a time until we get CR LF . CR LF

    while (1) {
        char* ptr;
        int len;
        int err = PGetLine(&ptr, &len);

        // Check for an error

        if (err) {
            mReplyCode = 0;
            return;
        }

        // Check for '.' on a line by itself, which indicates end of multiline
        // response

        if (len >= 3 && ptr[0] == '.' && ptr[1] == '\r' && ptr[2] == '\n') {
            break;
        }

        // Remove '.' at beginning of line

        if (*ptr == '.') ++ptr;

        // If an observer is assigned, notify it.
        // Implementation note: An observer is assumed to fetch the multiline
        // response one line at a time, therefore we assign to the string,
        // rather than append to it.

        if (mObserver) {
            mMultiLineResponse.assign(ptr, len);
            mObserver->Notify();
        }
        else {
            mMultiLineResponse.append(ptr, len);
        }
    }
}


int DwPopClient::PGetLine(char** aPtr, int* aLen)
{
    // Restore the saved state

    int startPos = mRecvBufferPos;
    int pos = mRecvBufferPos;
    int lastChar = -1;

    // Keep trying until we get a complete line, detect an error, or determine that
    // the connection has been closed

    int isEndOfLineFound = 0;
    while (1) {

        // Search buffer for end of line chars. Stop when we find them or when
        // we exhaust the buffer.

        while (pos < mNumRecvBufferChars) {
            if (lastChar == '\r' && mRecvBuffer[pos] == '\n') {
                isEndOfLineFound = 1;
                ++pos;
                break;
            }
            lastChar = mRecvBuffer[pos];
            ++pos;
        }
        if (isEndOfLineFound) {
            *aPtr = &mRecvBuffer[startPos];
            *aLen = pos - startPos;
            mRecvBufferPos = pos;
            return 0;
        }

        // Replenish the buffer

        memmove(mRecvBuffer, &mRecvBuffer[startPos], mNumRecvBufferChars-startPos);
        mNumRecvBufferChars -= startPos;
        mRecvBufferPos = mNumRecvBufferChars;
        int bufLen = RECV_BUFFER_SIZE - mRecvBufferPos;
        int n = PReceive(&mRecvBuffer[mRecvBufferPos], bufLen);
        if (n == 0) {
            // The connection has been closed or an error occurred
            return -1;
        }
        mNumRecvBufferChars += n;
        startPos = 0;
        pos = mRecvBufferPos;
    }
}
