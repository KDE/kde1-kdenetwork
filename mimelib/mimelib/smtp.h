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


class DwSmtpClient : public DwProtocolClient {

public:

    DwSmtpClient();

    ~DwSmtpClient();

    virtual int Open(const char* serverName, DwUint16 aPort=25);

    // Server reponses

    int ResponseCode() const;
    const DwString& SingleLineResponse() const;

    // SMTP commands

    int Helo();
    int Mail(const char* aFrom);
    int Rcpt(const char* aTo);
    int Data();
    int Rset();
    int SendCmd(const char* aFrom);
    int Soml(const char* aFrom);
    int Saml(const char* aFrom);
    int Vrfy(const char* aName);
    int Expn(const char* aName);
    int Help(const char* aArg=0);
    int Noop();
    int Quit();
    int Turn();

    int SendData(const char* aBuf, int aBufLen);

private:

    char*    mSendBuffer;
    char*    mRecvBuffer;
    int      mNumRecvBufferChars;
    int      mRecvBufferPos;
    int      mResponseCode;
    DwString mSingleLineResponse;

    void PGetSingleLineResponse();
    int PGetLine(char** aPtr, int* aLen);

};

#endif
