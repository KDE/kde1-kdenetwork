//=============================================================================
// File:       smtp.cpp
// Contents:   Definitions for DwSmtpClient
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
#include <mimelib/smtp.h>

#if defined(DW_UNIX)
#include <unistd.h>
#endif

#define SMTP_PORT 25
#define RECV_BUFFER_SIZE  8192
#define SEND_BUFFER_SIZE  1024

#if defined(DW_DEBUG_SMTP)
#  define DBG_SMTP_STMT(x) x
#else
#  define DBG_SMTP_STMT(x)
#endif


DwSmtpClient::DwSmtpClient()
{
    mRecvBuffer = new char[RECV_BUFFER_SIZE];
    mSendBuffer = new char[SEND_BUFFER_SIZE];
    mNumRecvBufferChars = 0;
    mRecvBufferPos = 0;
    mReplyCode = 0;
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
    mReplyCode = 0;
    mSingleLineResponse.clear();
    int err = DwProtocolClient::Open(aServer, aPort);
    if (! err) {
        PGetSingleLineResponse();
    }
    return mReplyCode;
}


int DwSmtpClient::ReplyCode() const
{
    return mReplyCode;
}


const DwString& DwSmtpClient::SingleLineResponse() const
{
    return mSingleLineResponse;
}


int DwSmtpClient::Helo()
{
    mReplyCode = 0;
    mSingleLineResponse.clear();
    strcpy(mSendBuffer, "HELO ");
    gethostname(&mSendBuffer[5], SEND_BUFFER_SIZE-8);
    strcat(mSendBuffer, "\r\n");
    DBG_SMTP_STMT(cout << "C: " << mSendBuffer << endl;)
    int bufferLen = strlen(mSendBuffer);
    int numSent = PSend(mSendBuffer, bufferLen);
    if (numSent == bufferLen) {
        PGetSingleLineResponse();
    }
    return mReplyCode;
}


int DwSmtpClient::Mail(const char* aFrom)
{
    mReplyCode = 0;
    mSingleLineResponse.clear();
    strcpy(mSendBuffer, "MAIL FROM:<");
    strcat(mSendBuffer, aFrom);
    strcat(mSendBuffer, ">\r\n");
    DBG_SMTP_STMT(cout << "C: " << mSendBuffer << endl;)
    int bufferLen = strlen(mSendBuffer);
    int numSent = PSend(mSendBuffer, bufferLen);
    if (numSent == bufferLen) {
        PGetSingleLineResponse();
    }
    return mReplyCode;
}


int DwSmtpClient::Rcpt(const char* aTo)
{
    mReplyCode = 0;
    mSingleLineResponse.clear();
    strcpy(mSendBuffer, "RCPT TO:<");
    strcat(mSendBuffer, aTo);
    strcat(mSendBuffer, ">\r\n");
    DBG_SMTP_STMT(cout << "C: " << mSendBuffer << endl;)
    int bufferLen = strlen(mSendBuffer);
    int numSent = PSend(mSendBuffer, bufferLen);
    if (numSent == bufferLen) {
        PGetSingleLineResponse();
    }
    return mReplyCode;
}


int DwSmtpClient::Data()
{
    mReplyCode = 0;
    mSingleLineResponse.clear();
    strcpy(mSendBuffer, "DATA\r\n");
    DBG_SMTP_STMT(cout << "C: " << mSendBuffer << endl;)
    int bufferLen = strlen(mSendBuffer);
    int numSent = PSend(mSendBuffer, bufferLen);
    if (numSent == bufferLen) {
        PGetSingleLineResponse();
    }
    return mReplyCode;
}


int DwSmtpClient::Rset()
{
    mReplyCode = 0;
    mSingleLineResponse.clear();
    strcpy(mSendBuffer, "RSET\r\n");
    DBG_SMTP_STMT(cout << "C: " << mSendBuffer << endl;)
    int bufferLen = strlen(mSendBuffer);
    int numSent = PSend(mSendBuffer, bufferLen);
    if (numSent == bufferLen) {
        PGetSingleLineResponse();
    }
    return mReplyCode;
}


int DwSmtpClient::Send(const char* aFrom)
{
    mReplyCode = 0;
    mSingleLineResponse.clear();
    strcpy(mSendBuffer, "SEND FROM:<");
    strcat(mSendBuffer, aFrom);
    strcat(mSendBuffer, ">\r\n");
    DBG_SMTP_STMT(cout << "C: " << mSendBuffer << endl;)
    int bufferLen = strlen(mSendBuffer);
    int numSent = PSend(mSendBuffer, bufferLen);
    if (numSent == bufferLen) {
        PGetSingleLineResponse();
    }
    return mReplyCode;
}


int DwSmtpClient::Soml(const char* aFrom)
{
    mReplyCode = 0;
    mSingleLineResponse.clear();
    strcpy(mSendBuffer, "SOML FROM:<");
    strcat(mSendBuffer, aFrom);
    strcat(mSendBuffer, ">\r\n");
    DBG_SMTP_STMT(cout << "C: " << mSendBuffer << endl;)
    int bufferLen = strlen(mSendBuffer);
    int numSent = PSend(mSendBuffer, bufferLen);
    if (numSent == bufferLen) {
        PGetSingleLineResponse();
    }
    return mReplyCode;
}


int DwSmtpClient::Saml(const char* aFrom)
{
    mReplyCode = 0;
    mSingleLineResponse.clear();
    strcpy(mSendBuffer, "SAML FROM:<");
    strcat(mSendBuffer, aFrom);
    strcat(mSendBuffer, ">\r\n");
    DBG_SMTP_STMT(cout << "C: " << mSendBuffer << endl;)
    int bufferLen = strlen(mSendBuffer);
    int numSent = PSend(mSendBuffer, bufferLen);
    if (numSent == bufferLen) {
        PGetSingleLineResponse();
    }
    return mReplyCode;
}


int DwSmtpClient::Vrfy(const char* aName)
{
    mReplyCode = 0;
    mSingleLineResponse.clear();
    strcpy(mSendBuffer, "VRFY ");
    strcat(mSendBuffer, aName);
    strcat(mSendBuffer, "\r\n");
    DBG_SMTP_STMT(cout << "C: " << mSendBuffer << endl;)
    int bufferLen = strlen(mSendBuffer);
    int numSent = PSend(mSendBuffer, bufferLen);
    if (numSent == bufferLen) {
        PGetSingleLineResponse();
    }
    return mReplyCode;
}


int DwSmtpClient::Expn(const char* aName)
{
    mReplyCode = 0;
    mSingleLineResponse.clear();
    strcpy(mSendBuffer, "EXPN ");
    strcat(mSendBuffer, aName);
    strcat(mSendBuffer, "\r\n");
    DBG_SMTP_STMT(cout << "C: " << mSendBuffer << endl;)
    int bufferLen = strlen(mSendBuffer);
    int numSent = PSend(mSendBuffer, bufferLen);
    if (numSent == bufferLen) {
        PGetSingleLineResponse();
    }
    return mReplyCode;
}


int DwSmtpClient::Help(const char* aArg)
{
    mReplyCode = 0;
    mSingleLineResponse.clear();
    strcpy(mSendBuffer, "HELP");
    if (aArg) {
        strcat(mSendBuffer, " ");
        strcat(mSendBuffer, aArg);
    }
    strcat(mSendBuffer, "\r\n");
    DBG_SMTP_STMT(cout << "C: " << mSendBuffer << endl;)
    int bufferLen = strlen(mSendBuffer);
    int numSent = PSend(mSendBuffer, bufferLen);
    if (numSent == bufferLen) {
        PGetSingleLineResponse();
    }
    return mReplyCode;
}


int DwSmtpClient::Noop()
{
    mReplyCode = 0;
    mSingleLineResponse.clear();
    strcpy(mSendBuffer, "NOOP\r\n");
    DBG_SMTP_STMT(cout << "C: " << mSendBuffer << endl;)
    int bufferLen = strlen(mSendBuffer);
    int numSent = PSend(mSendBuffer, bufferLen);
    if (numSent == bufferLen) {
        PGetSingleLineResponse();
    }
    return mReplyCode;
}


int DwSmtpClient::Quit()
{
    mReplyCode = 0;
    mSingleLineResponse.clear();
    strcpy(mSendBuffer, "QUIT\r\n");
    DBG_SMTP_STMT(cout << "C: " << mSendBuffer << endl;)
    int bufferLen = strlen(mSendBuffer);
    int numSent = PSend(mSendBuffer, bufferLen);
    if (numSent == bufferLen) {
        PGetSingleLineResponse();
    }
    return mReplyCode;
}


int DwSmtpClient::SendData(const DwString& aStr)
{
    return SendData(aStr.data(), aStr.length());
}


int DwSmtpClient::SendData(const char* aBuf, int aBufLen)
{
    mReplyCode = 0;
    mSingleLineResponse.clear();

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

        int numSent = PSend(buf, len);
        if (numSent != len) {
            mReplyCode = 0;
            return mReplyCode;
        }
    }

    // Send final '.' CR LF.  If CR LF are not at the end of the buffer, then
    // send a CR LF '.' CR LF.

    if (lastLastChar == '\r' && lastChar == '\n') {
        PSend(".\r\n", 3);
    }
    else {
        PSend("\r\n.\r\n", 5);
    }

    // Get the server's response

    PGetSingleLineResponse();
    return mReplyCode;
}


void DwSmtpClient::PGetSingleLineResponse()
{
    mReplyCode = 0;
    mSingleLineResponse.clear();
    char* ptr;
    int len;
    int err = PGetLine(&ptr, &len);
    if (! err) {
        mReplyCode = strtol(ptr, NULL, 10);
        mSingleLineResponse.assign(ptr, len);
        DBG_SMTP_STMT(char buffer[256];)
        DBG_SMTP_STMT(strncpy(buffer, ptr, len);)
        DBG_SMTP_STMT(buffer[len] = 0;)
        DBG_SMTP_STMT(cout << "S: " << buffer;)
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
