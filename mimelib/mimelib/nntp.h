//=============================================================================
// File:       nntp.h
// Contents:   Declarations for DwNntpClient
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

#ifndef DW_NNTP_H
#define DW_NNTP_H

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


class DwObserver {
public:
    virtual void Notify()=0;
};


class DwNntpClient : public DwProtocolClient {

public:
   
    DwNntpClient();

    ~DwNntpClient();

    virtual int Open(const char* aServer, DwUint16 aPort=119);
    DwObserver* SetObserver(DwObserver* aObserver);

    // Server reponses

    int ResponseCode() const;
    const DwString& SingleLineResponse() const;
    const DwString& MultiLineResponse() const;

    // NNTP commands
	
    int Article(int aNumber=(-1));
    int Article(const char* aMsgid);
    int Body(int aNumber=(-1));
    int Body(const char* aMsgid);
    int Head(int aNumber=(-1));
    int Head(const char* aMsgid);
    int Stat();
    int Stat(int aNumber);
    int Stat(const char* aMsgid);
    int Group(const char* aNewsgroupName);
    int Help();
    int Ihave(const char* aMsgId);
    int Last();
    int List();
    int Newgroups(const char* aDate, const char* aTime,
        DwBool aIsGmt=0, const char* aDistribution=0);
    int Newnews(const char* aNewsgroups, const char* aDate,
        const char* aTime, DwBool aIsGmt=0, const char* aDistribution=0);
    int Next();
    int Post();
    int Quit();
    int Slave();

    int Date();
    int ListGroup(const char* aNewsgroup);

    int SendData(const char* aBuf, int aBufLen);

private:

    char*       mSendBuffer;
    char*       mRecvBuffer;
    int         mLastChar;
    int         mLastLastChar;
    int         mNumRecvBufferChars;
    int         mRecvBufferPos;
    int         mResponseCode;
    DwString    mSingleLineResponse;
    DwString    mMultiLineResponse;
    DwObserver* mObserver;

    int PGetLine(char** aPtr, int* aLen);
    void PGetSingleLineResponse();
    void PGetMultiLineResponse();

};

#endif
