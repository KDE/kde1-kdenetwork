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


DwPopClient::DwPopClient()
{
    mSendBuffer = new char[SEND_BUFFER_SIZE+1];
    mRecvBuffer = new char[RECV_BUFFER_SIZE+1];
    mNumRecvBufferChars = 0;
    mRecvBufferPos = 0;
    mResponseCode = 0;
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
    mSingleLineResponse.clear();
    mResponseCode = -1;
    int err = DwProtocolClient::Open(aServer, aPort);
    if (! err) {
        PGetSingleLineResponse();
    }
    return mResponseCode;
}


DwObserver* DwPopClient::SetObserver(DwObserver* aObserver)
{
    DwObserver* obs = mObserver;
    mObserver = aObserver;
    return obs;
}


int DwPopClient::ResponseCode() const
{
    return mResponseCode;
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
    strcpy(mSendBuffer, "USER ");
    strcat(mSendBuffer, aName);
    strcat(mSendBuffer, "\r\n");

    // for debugging
    cout << "C: " << mSendBuffer << endl;

    mResponseCode = -1;
    int bufferLen = strlen(mSendBuffer);
    int numSent = Send(mSendBuffer, bufferLen);
    if (numSent == bufferLen) {
        PGetSingleLineResponse();
    }
    return mResponseCode;
}


int DwPopClient::Pass(const char* aPasswd)
{
    strcpy(mSendBuffer, "PASS ");
    strcat(mSendBuffer, aPasswd);
    strcat(mSendBuffer, "\r\n");

    // for debugging
    cout << "C: " << mSendBuffer << endl;

    mResponseCode = -1;
    int bufferLen = strlen(mSendBuffer);
    int numSent = Send(mSendBuffer, bufferLen);
    if (numSent == bufferLen) {
        PGetSingleLineResponse();
    }
    return mResponseCode;
}


int DwPopClient::Quit()
{
    strcpy(mSendBuffer, "QUIT\r\n");

    // for debugging
    cout << "C: " << mSendBuffer << endl;

    mResponseCode = -1;
    int bufferLen = strlen(mSendBuffer);
    int numSent = Send(mSendBuffer, bufferLen);
    if (numSent == bufferLen) {
        PGetSingleLineResponse();
    }
    return mResponseCode;
}


int DwPopClient::Stat()
{
    strcpy(mSendBuffer, "STAT\r\n");

    // for debugging
    cout << "C: " << mSendBuffer << endl;

    mResponseCode = -1;
    int bufferLen = strlen(mSendBuffer);
    int numSent = Send(mSendBuffer, bufferLen);
    if (numSent == bufferLen) {
        PGetSingleLineResponse();
    }
    return mResponseCode;
}


int DwPopClient::List()
{
    strcpy(mSendBuffer, "LIST\r\n");

    // for debugging
    cout << "C: " << mSendBuffer << endl;

    mResponseCode = -1;
    int bufferLen = strlen(mSendBuffer);
    int numSent = Send(mSendBuffer, bufferLen);
    if (numSent == bufferLen) {
        PGetSingleLineResponse();
        if (mResponseCode == '+') {
            PGetMultiLineResponse();
        }
    }
    return mResponseCode;
}


int DwPopClient::List(int aMsg)
{
    sprintf(mSendBuffer, "LIST %d\r\n", aMsg);

    // for debugging
    cout << "C: " << mSendBuffer << endl;

    mResponseCode = -1;
    int bufferLen = strlen(mSendBuffer);
    int numSent = Send(mSendBuffer, bufferLen);
    if (numSent == bufferLen) {
        PGetSingleLineResponse();
    }
    return mResponseCode;
}


int DwPopClient::Retr(int aMsg)
{
    sprintf(mSendBuffer, "RETR %d\r\n", aMsg);

    // for debugging
    cout << "C: " << mSendBuffer << endl;

    mResponseCode = -1;
    int bufferLen = strlen(mSendBuffer);
    int numSent = Send(mSendBuffer, bufferLen);
    if (numSent == bufferLen) {
        PGetSingleLineResponse();
        if (mResponseCode == '+') {
            PGetMultiLineResponse();
        }
    }
    return mResponseCode;
}


int DwPopClient::Dele(int aMsg)
{
    sprintf(mSendBuffer, "DELE %d\r\n", aMsg);

    // for debugging
    cout << "C: " << mSendBuffer << endl;

    mResponseCode = -1;
    int bufferLen = strlen(mSendBuffer);
    int numSent = Send(mSendBuffer, bufferLen);
    if (numSent == bufferLen) {
        PGetSingleLineResponse();
    }
    return mResponseCode;
}


int DwPopClient::Noop()
{
    strcpy(mSendBuffer, "NOOP\r\n");

    // for debugging
    cout << "C: " << mSendBuffer << endl;

    mResponseCode = -1;
    int bufferLen = strlen(mSendBuffer);
    int numSent = Send(mSendBuffer, bufferLen);
    if (numSent == bufferLen) {
        PGetSingleLineResponse();
    }
    return mResponseCode;
}


int DwPopClient::Rset()
{
    strcpy(mSendBuffer, "RSET\r\n");

    // for debugging
    cout << "C: " << mSendBuffer << endl;

    mResponseCode = -1;
    int bufferLen = strlen(mSendBuffer);
    int numSent = Send(mSendBuffer, bufferLen);
    if (numSent == bufferLen) {
        PGetSingleLineResponse();
    }
    return mResponseCode;
}


int DwPopClient::Apop(const char* aName, const char* aDigest)
{
    sprintf(mSendBuffer, "APOP %s %s\r\n", aName, aDigest);

    // for debugging
    cout << "C: " << mSendBuffer << endl;

    mResponseCode = -1;
    int bufferLen = strlen(mSendBuffer);
    int numSent = Send(mSendBuffer, bufferLen);
    if (numSent == bufferLen) {
        PGetSingleLineResponse();
    }
    return mResponseCode;
}


int DwPopClient::Top(int aMsg, int aNumLines)
{
    sprintf(mSendBuffer, "TOP %d %d\r\n", aMsg, aNumLines);

    // for debugging
    cout << "C: " << mSendBuffer << endl;

    mResponseCode = -1;
    int bufferLen = strlen(mSendBuffer);
    int numSent = Send(mSendBuffer, bufferLen);
    if (numSent == bufferLen) {
        PGetSingleLineResponse();
        if (mResponseCode == '+') {
            PGetMultiLineResponse();
        }
    }
    return mResponseCode;
}


int DwPopClient::Uidl()
{
    strcpy(mSendBuffer, "UIDL\r\n");

    // for debugging
    cout << "C: " << mSendBuffer << endl;

    mResponseCode = -1;
    int bufferLen = strlen(mSendBuffer);
    int numSent = Send(mSendBuffer, bufferLen);
    if (numSent == bufferLen) {
        PGetSingleLineResponse();
        if (mResponseCode == '+') {
            PGetMultiLineResponse();
        }
    }
    return mResponseCode;
}


int DwPopClient::Uidl(int aMsg)
{
    sprintf(mSendBuffer, "UIDL %d\r\n", aMsg);

    // for debugging
    cout << "C: " << mSendBuffer << endl;

    mResponseCode = -1;
    int bufferLen = strlen(mSendBuffer);
    int numSent = Send(mSendBuffer, bufferLen);
    if (numSent == bufferLen) {
        PGetSingleLineResponse();
        if (mResponseCode == '+') {
            PGetMultiLineResponse();
        }
    }
    return mResponseCode;
}


void DwPopClient::PGetSingleLineResponse()
{
    mResponseCode = -1;
    mSingleLineResponse.clear();
    char* ptr;
    int len;
    int err = PGetLine(&ptr, &len);
    if (! err) {
        mResponseCode = ptr[0];
        mSingleLineResponse.assign(ptr, len);

        // for debugging
        char buffer[256];
        strncpy(buffer, ptr, len);
        buffer[len] = 0;
        printf("S: %s", buffer);
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
            mResponseCode = -1;
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
        int n = Receive(&mRecvBuffer[mRecvBufferPos], bufLen);
        if (n == 0) {
            // The connection has been closed or an error occurred
            return -1;
        }
        mNumRecvBufferChars += n;
        startPos = 0;
        pos = mRecvBufferPos;
    }
}
