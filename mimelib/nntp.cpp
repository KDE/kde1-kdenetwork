//=============================================================================
// File:       nntp.cpp
// Contents:   Definitions for DwNntpClient
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
#include <mimelib/nntp.h>

#define NNTP_PORT 119
#define RECV_BUFFER_SIZE  8192
#define SEND_BUFFER_SIZE  1024

// To do: create a private Initialize() member function that is called from
// the constructor and from Open() and that initializes the state.

// To do: change SendData() to accept a DwString

DwNntpClient::DwNntpClient()
{
    mSendBuffer = new char[SEND_BUFFER_SIZE+1];
    mRecvBuffer = new char[RECV_BUFFER_SIZE+1];
    mLastChar = -1;
    mLastLastChar = -1;
    mNumRecvBufferChars = 0;
    mRecvBufferPos = 0;
    mResponseCode = 0;
    mObserver = 0;
}


DwNntpClient::~DwNntpClient()
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


int DwNntpClient::Open(const char* aServer, DwUint16 aPort)
{
    mSingleLineResponse.clear();
    mResponseCode = -1;
    int err = DwProtocolClient::Open(aServer, aPort);
    if (! err) {
        PGetSingleLineResponse();
    }
    return mResponseCode;
}


DwObserver* DwNntpClient::SetObserver(DwObserver* aObserver)
{
    DwObserver* obs = mObserver;
    mObserver = aObserver;
    return obs;
}


int DwNntpClient::ResponseCode() const
{
    return mResponseCode;
}


const DwString& DwNntpClient::SingleLineResponse() const
{
    return mSingleLineResponse;
}


const DwString& DwNntpClient::MultiLineResponse() const
{
    return mMultiLineResponse;
}


int DwNntpClient::Article(int aArticleNum)
{
    if (aArticleNum >= 0) {
        sprintf(mSendBuffer, "ARTICLE %d\r\n", aArticleNum);
    }
    else {
        strcpy(mSendBuffer, "ARTICLE\r\n");
    }

    // for debugging
    cout << "C: " << mSendBuffer << endl;

    mResponseCode = -1;
    int bufferLen = strlen(mSendBuffer);
    int numSent = Send(mSendBuffer, bufferLen);
    if (numSent == bufferLen) {
        PGetSingleLineResponse();
        if (mResponseCode/100%10 == 2) {
            PGetMultiLineResponse();
        }
    }
    return mResponseCode;
}


int DwNntpClient::Article(const char* aMsgId)
{
    if (!aMsgId || !*aMsgId) {
        // error!
        return -1;
    }
    strcpy(mSendBuffer, "ARTICLE ");
    strncat(mSendBuffer, aMsgId, 80);
    strcat(mSendBuffer, "\r\n");

    // for debugging
    cout << "C: " << mSendBuffer << endl;

    mResponseCode = -1;
    int bufferLen = strlen(mSendBuffer);
    int numSent = Send(mSendBuffer, bufferLen);
    if (numSent == bufferLen) {
        PGetSingleLineResponse();
        if (mResponseCode/100%10 == 2) {
            PGetMultiLineResponse();
        }
    }
    return mResponseCode;
}


int DwNntpClient::Head(int aArticleNum)
{
    if (aArticleNum >= 0) {
        sprintf(mSendBuffer, "HEAD %d\r\n", aArticleNum);
    }
    else {
        strcpy(mSendBuffer, "HEAD\r\n");
    }

    // for debugging
    cout << "C: " << mSendBuffer << endl;

    mResponseCode = -1;
    int bufferLen = strlen(mSendBuffer);
    int numSent = Send(mSendBuffer, bufferLen);
    if (numSent == bufferLen) {
        PGetSingleLineResponse();
        if (mResponseCode/100%10 == 2) {
            PGetMultiLineResponse();
        }
    }
    return mResponseCode;
}


int DwNntpClient::Head(const char* aMsgId)
{
    if (!aMsgId || !*aMsgId) {
        return -1;
    }
    strcpy(mSendBuffer, "HEAD ");
    strncat(mSendBuffer, aMsgId, 80);
    strcat(mSendBuffer, "\r\n");

    // for debugging
    cout << "C: " << mSendBuffer << endl;

    mResponseCode = -1;
    int bufferLen = strlen(mSendBuffer);
    int numSent = Send(mSendBuffer, bufferLen);
    if (numSent == bufferLen) {
        PGetSingleLineResponse();
        if (mResponseCode/100%10 == 2) {
            PGetMultiLineResponse();
        }
    }
    return mResponseCode;
}


int DwNntpClient::Body(int articleNum)
{
    if (articleNum >= 0) {
        sprintf(mSendBuffer, "BODY %d\r\n", articleNum);
    }
    else {
        strcpy(mSendBuffer, "BODY\r\n");
    }

    // for debugging
    cout << "C: " << mSendBuffer << endl;

    mResponseCode = -1;
    int bufferLen = strlen(mSendBuffer);
    int numSent = Send(mSendBuffer, bufferLen);
    if (numSent == bufferLen) {
        PGetSingleLineResponse();
        if (mResponseCode/100%10 == 2) {
            PGetMultiLineResponse();
        }
    }
    return mResponseCode;
}


int DwNntpClient::Body(const char* aMsgId)
{
    if (!aMsgId || !*aMsgId) {
        return -1;
    }
    strcpy(mSendBuffer, "BODY ");
    strncat(mSendBuffer, aMsgId, 80);
    strcat(mSendBuffer, "\r\n");

    // for debugging
    cout << "C: " << mSendBuffer << endl;

    mResponseCode = -1;
    int bufferLen = strlen(mSendBuffer);
    int numSent = Send(mSendBuffer, bufferLen);
    if (numSent == bufferLen) {
        PGetSingleLineResponse();
        if (mResponseCode/100%10 == 2) {
            PGetMultiLineResponse();
        }
    }
    return mResponseCode;
}


int DwNntpClient::Stat(int articleNum)
{
    if (articleNum >= 0) {
        sprintf(mSendBuffer, "STAT %d\r\n", articleNum);
    }
    else {
        strcpy(mSendBuffer, "STAT\r\n");
    }

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


int DwNntpClient::Stat(const char* aMsgId)
{
    if (!aMsgId || !*aMsgId) {
        return -1;
    }
    strcpy(mSendBuffer, "STAT ");
    strncat(mSendBuffer, aMsgId, 80);
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


int DwNntpClient::Group(const char* aNewsgroupName)
{
    sprintf(mSendBuffer, "GROUP %s\r\n", aNewsgroupName);

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


int DwNntpClient::Help()
{
    strcpy(mSendBuffer, "HELP\r\n");

    // for debugging
    cout << "C: " << mSendBuffer << endl;

    mResponseCode = -1;
    int bufferLen = strlen(mSendBuffer);
    int numSent = Send(mSendBuffer, bufferLen);
    if (numSent == bufferLen) {
        PGetSingleLineResponse();
        if (mResponseCode/100%10 == 1) {
            PGetMultiLineResponse();
        }
    }
    return mResponseCode;
}


int DwNntpClient::Last()
{
    strcpy(mSendBuffer, "LAST\r\n");

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


int DwNntpClient::List()
{
    strcpy(mSendBuffer, "LIST\r\n");

    // for debugging
    cout << "C: " << mSendBuffer << endl;

    mResponseCode = -1;
    int bufferLen = strlen(mSendBuffer);
    int numSent = Send(mSendBuffer, bufferLen);
    if (numSent == bufferLen) {
        PGetSingleLineResponse();
        if (mResponseCode/100%10 == 2) {
            PGetMultiLineResponse();
        }
    }
    return mResponseCode;
}


int DwNntpClient::Newgroups(const char* aDate, const char* aTime,
    DwBool aIsGmt, const char* aDistribution)
{
    strcpy(mSendBuffer, "NEWGROUPS ");
    strcat(mSendBuffer, aDate);
    strcat(mSendBuffer, " ");
    strcat(mSendBuffer, aTime);
    if (aIsGmt) {
        strcat(mSendBuffer, " GMT");
    }
    if (aDistribution) {
        strcat(mSendBuffer, " ");
        strcat(mSendBuffer, aDistribution);
    }
    strcat(mSendBuffer, "\r\n");

    // for debugging
    cout << "C: " << mSendBuffer << endl;

    mResponseCode = -1;
    int bufferLen = strlen(mSendBuffer);
    int numSent = Send(mSendBuffer, bufferLen);
    if (numSent == bufferLen) {
        PGetSingleLineResponse();
        if (mResponseCode/100%10 == 2) {
            PGetMultiLineResponse();
        }
    }
    return mResponseCode;
}


int DwNntpClient::Newnews(const char* aNewsgroups, const char* aDate,
    const char* aTime, DwBool aIsGmt, const char* aDistribution)
{
    strcpy(mSendBuffer, "NEWNEWS ");
    strcat(mSendBuffer, aNewsgroups);
    strcat(mSendBuffer, " ");
    strcat(mSendBuffer, aDate);
    strcat(mSendBuffer, " ");
    strcat(mSendBuffer, aTime);
    if (aIsGmt) {
        strcat(mSendBuffer, " GMT");
    }
    if (aDistribution) {
        strcat(mSendBuffer, " ");
        strcat(mSendBuffer, aDistribution);
    }
    strcat(mSendBuffer, "\r\n");

    // for debugging
    cout << "C: " << mSendBuffer << endl;

    mResponseCode = -1;
    int bufferLen = strlen(mSendBuffer);
    int numSent = Send(mSendBuffer, bufferLen);
    if (numSent == bufferLen) {
        PGetSingleLineResponse();
        if (mResponseCode/100%10 == 2) {
            PGetMultiLineResponse();
        }
    }
    return mResponseCode;
}


int DwNntpClient::Next()
{
    strcpy(mSendBuffer, "NEXT\r\n");

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


int DwNntpClient::Post()
{
    strcpy(mSendBuffer, "POST\r\n");

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


int DwNntpClient::Quit()
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


int DwNntpClient::Slave()
{
    strcpy(mSendBuffer, "SLAVE\r\n");

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


int DwNntpClient::Date()
{
    strcpy(mSendBuffer, "DATE\r\n");

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


int DwNntpClient::SendData(const char* aBuf, int aBufLen)
{
    int pos = 0;
    int len = 0;
    const char* buf = 0;

    int lastLastChar = '\r';
    int lastChar = '\n';

    while (1) {

        len = SEND_BUFFER_SIZE;
        len = (len < aBufLen - pos) ? len : aBufLen - pos;
        if (len == 0) break;

        // Look for CR LF '.'.  If it is found, then we have to copy the buffer
        // and stuff an extra '.'.

        int hasCrLfDot = 0;
        int tLastChar = lastChar;
        int tLastLastChar = lastLastChar;
        for (int i=0; i < len; ++i) {
            int ch = aBuf[pos+i];
            if (tLastLastChar == '\r' && tLastChar == '\n' && ch == '.') {
                hasCrLfDot = 1;
                break;
            }
            tLastLastChar = tLastChar;
            tLastChar = ch;
        }
        if (! hasCrLfDot) {
            lastChar = tLastChar;
            lastLastChar = tLastLastChar;
            buf = &aBuf[pos];
            pos += len;
        }

        // If CR LF '.' was found, copy the chars to a different buffer and stuff
        // the extra '.'.

        else /* (hasCrLfDot) */ {
            tLastChar = lastChar;
            tLastLastChar = lastLastChar;
            int iDst = 0;
            int iSrc = 0;
            // Implementation note: be careful to avoid overrunning the
            // destination buffer when CR LF '.' are the last three characters
            // of the source buffer.
            while (iDst < SEND_BUFFER_SIZE && iSrc < len) {
                int ch = aBuf[pos+iSrc];
                if (tLastLastChar == '\r' && tLastChar == '\n' && ch == '.') {
                    if (iDst == SEND_BUFFER_SIZE-1) {
                        break;
                    }
                    mSendBuffer[iDst++] = '.';
                }
                mSendBuffer[iDst++] = ch;
                ++iSrc;
                tLastLastChar = tLastChar;
                tLastChar = ch;
            }
            lastChar = tLastChar;
            lastLastChar = tLastLastChar;
            len = iDst;
            buf = mSendBuffer;
            pos += iSrc;
        }

        // Send the buffer

        // To do: check for error from Send()
        Send(buf, len);
    }

    // Send final '.' CR LF.  If CR LF are not at the end of the buffer, then
    // send a CR LF '.' CR LF.

    if (lastLastChar == '\r' && lastChar == '\n') {
        Send(".\r\n", 3);
    }
    else {
        Send("\r\n.\r\n", 5);
    }

    // Get the server's response

    PGetSingleLineResponse();
    return mResponseCode;
}


void DwNntpClient::PGetSingleLineResponse()
{
    mResponseCode = -1;
    mSingleLineResponse.clear();
    char* ptr;
    int len;
    int err = PGetLine(&ptr, &len);
    if (! err) {
        mResponseCode = strtol(ptr, NULL, 10);
        mSingleLineResponse.assign(ptr, len);

        // for debugging
        char buffer[256];
        strncpy(buffer, ptr, len);
        buffer[len] = 0;
        cout << "S: " << buffer;
    }
}


void DwNntpClient::PGetMultiLineResponse()
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


int DwNntpClient::PGetLine(char** aPtr, int* aLen)
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
