//=============================================================================
// File:       smtp.cpp
// Contents:   Definitions for DwSmtpClient
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

#define DW_IMPLEMENTATION

#include <mimelib/config.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <mimelib/smtp.h>
#include <unistd.h>
#include <unistd.h>

#define SMTP_PORT 25
#define RECV_BUFFER_SIZE  8192
#define SEND_BUFFER_SIZE  1024


DwSmtpClient::DwSmtpClient()
{
    mRecvBuffer = new char[RECV_BUFFER_SIZE+1];
    mSendBuffer = new char[SEND_BUFFER_SIZE+1];
    mNumRecvBufferChars = 0;
    mRecvBufferPos = 0;
    mResponseCode = 0;
}


DwSmtpClient::~DwSmtpClient()
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


int DwSmtpClient::Open(const char* aServer, DwUint16 aPort)
{
    mSingleLineResponse.clear();
    mResponseCode = -1;
    int err = DwProtocolClient::Open(aServer, aPort);
    if (! err) {
        PGetSingleLineResponse();
    }
    return mResponseCode;
}


int DwSmtpClient::ResponseCode() const
{
    return mResponseCode;
}


const DwString& DwSmtpClient::SingleLineResponse() const
{
    return mSingleLineResponse;
}


int DwSmtpClient::Helo()
{
    strcpy(mSendBuffer, "HELO ");
    char hostname[100];
    gethostname(hostname, sizeof(hostname));
    hostname[sizeof(hostname)-1] = 0;
    strcat(mSendBuffer, hostname);
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


int DwSmtpClient::Mail(const char* aFrom)
{
    strcpy(mSendBuffer, "MAIL FROM:<");
    strcat(mSendBuffer, aFrom);
    strcat(mSendBuffer, ">\r\n");

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


int DwSmtpClient::Rcpt(const char* aTo)
{
    strcpy(mSendBuffer, "RCPT TO:<");
    strcat(mSendBuffer, aTo);
    strcat(mSendBuffer, ">\r\n");

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


int DwSmtpClient::Data()
{
    strcpy(mSendBuffer, "DATA\r\n");

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


int DwSmtpClient::Rset()
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


int DwSmtpClient::SendCmd(const char* aFrom)
{
    strcpy(mSendBuffer, "SEND FROM:<");
    strcat(mSendBuffer, aFrom);
    strcat(mSendBuffer, ">\r\n");

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


int DwSmtpClient::Soml(const char* aFrom)
{
    strcpy(mSendBuffer, "SOML FROM:<");
    strcat(mSendBuffer, aFrom);
    strcat(mSendBuffer, ">\r\n");

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


int DwSmtpClient::Saml(const char* aFrom)
{
    strcpy(mSendBuffer, "SAML FROM:<");
    strcat(mSendBuffer, aFrom);
    strcat(mSendBuffer, ">\r\n");

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


int DwSmtpClient::Vrfy(const char* aName)
{
    strcpy(mSendBuffer, "VRFY ");
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


int DwSmtpClient::Expn(const char* aName)
{
    strcpy(mSendBuffer, "EXPN ");
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


int DwSmtpClient::Help(const char* aArg)
{
    strcpy(mSendBuffer, "HELP");
    if (aArg) {
        strcat(mSendBuffer, " ");
        strcat(mSendBuffer, aArg);
    }
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


int DwSmtpClient::Noop()
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


int DwSmtpClient::Quit()
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


int DwSmtpClient::SendData(const char* aBuf, int aBufLen)
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


void DwSmtpClient::PGetSingleLineResponse()
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


int DwSmtpClient::PGetLine(char** aPtr, int* aLen)
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
