//=============================================================================
// File:       basicmsg.cpp
// Contents:   Definitions for BasicMessage
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

#include <assert.h>
#include <stdlib.h>
#include "basicmsg.h"


BasicMessage::BasicMessage()
{
    mMessage = DwMessage::NewMessage(mEmptyString, 0);
}


BasicMessage::BasicMessage(DwMessage* aMsg)
{
    mMessage = aMsg;
}


BasicMessage::~BasicMessage()
{
    if (mMessage != 0) {
        delete mMessage;
    }
}


void BasicMessage::TakeMessage(DwMessage* aMsg)
{
    // Delete the old DwMessage

    if (mMessage) {
        delete mMessage;
    }

    // Assign the new DwMessage

    mMessage = aMsg;
}


const DwString& BasicMessage::AsString()
{
    // Assemble the DwMessage

    mMessage->Assemble();
    
    // Return its string contents

    return mMessage->AsString();
}


void BasicMessage::SetAutomaticFields()
{
    DwHeader& header = mMessage->Header();
    header.MimeVersion().FromString("1.0");
    header.MessageId().CreateDefault();
}


const DwString& BasicMessage::DateStr() const
{
    // Access the 'Date' header field and return its contents as a string

    DwHeader& header = mMessage->Header();
    if (header.HasDate()) {
        return header.Date().AsString();
    }
    else {
        return mEmptyString;
    }
}


DwUint32 BasicMessage::Date() const
{
    // Access the 'Date' header field and return its contents as a UNIX
    // time (i.e. POSIX time)

    DwHeader& header = mMessage->Header();
    if (header.HasDate()) {
        return header.Date().AsUnixTime();
    }
    else {
        return (DwUint32) -1;
    }
}


void BasicMessage::SetDate(DwUint32 aUnixTime)
{
    // Access the 'Date' header field and set its contents from a UNIX
    // time (i.e. POSIX time)

    mMessage->Header().Date().FromUnixTime(aUnixTime);
}


const DwString& BasicMessage::To() const
{
    // Access the 'To' header field and return its contents as a string

    DwHeader& header = mMessage->Header();
    if (header.HasTo()) {
        return header.To().AsString();
    }
    else {
        return mEmptyString;
    }
}


void BasicMessage::SetTo(const DwString& aStr)
{
    // Access the 'To' header field and set its contents from a string

    mMessage->Header().To().FromString(aStr);
}


const DwString& BasicMessage::Cc() const
{
    // Access the 'Cc' header field and return its contents as a string

    DwHeader& header = mMessage->Header();
    if (header.HasCc()) {
        return header.Cc().AsString();
    }
    else {
        return mEmptyString;
    }
}


void BasicMessage::SetCc(const DwString& aStr)
{
    // Access the 'Cc' header field and set its contents from a string

    mMessage->Header().Cc().FromString(aStr);
}


const DwString& BasicMessage::Bcc() const
{
    // Access the 'Bcc' header field and return its contents as a string

    DwHeader& header = mMessage->Header();
    if (header.HasBcc()) {
        return header.Bcc().AsString();
    }
    else {
        return mEmptyString;
    }
}


void BasicMessage::SetBcc(const DwString& aStr)
{
    // Access the 'Bcc' header field and set its contents from a string

    mMessage->Header().Bcc().FromString(aStr);
}


const DwString& BasicMessage::From() const
{
    // Access the 'From' header field and return its contents as a string

    DwHeader& header = mMessage->Header();
    if (header.HasFrom()) {
        return header.From().AsString();
    }
    else {
        return mEmptyString;
    }
}


void BasicMessage::SetFrom(const DwString& aStr)
{
    // Access the 'From' header field and set its contents from a string

    mMessage->Header().From().FromString(aStr);
}


const DwString& BasicMessage::Subject() const
{
    // Access the 'Subject' header field and return its contents as a string

    DwHeader& header = mMessage->Header();
    if (header.HasSubject()) {
        return header.Subject().AsString();
    }
    else {
        return mEmptyString;
    }
}


void BasicMessage::SetSubject(const DwString& aStr)
{
    // Access the 'Subject' header field and set its contents from a string

    mMessage->Header().Subject().FromString(aStr);
}


const DwString& BasicMessage::TypeStr() const
{
    // Access the 'Content-Type' header field and return its 'type'
    // as a string

    DwHeader& header = mMessage->Header();
    if (header.HasContentType()) {
        return header.ContentType().TypeStr();
    }
    else {
        return mEmptyString;
    }
}


int BasicMessage::Type() const
{
    // Access the 'Content-Type' header field and return its 'type'
    // as an enumerated type

    DwHeader& header = mMessage->Header();
    if (header.HasContentType()) {
        return header.ContentType().Type();
    }
    else {
        return DwMime::kTypeNull;
    }
}


void BasicMessage::SetTypeStr(const DwString& aStr)
{
    // Access the 'Content-Type' header field and set its 'type'
    // from a string

    mMessage->Header().ContentType().SetTypeStr(aStr);
}


void BasicMessage::SetType(int aType)
{
    // Access the 'Content-Type' header field and set its 'type'
    // from an enumerated type

    mMessage->Header().ContentType().SetType(aType);
}


const DwString& BasicMessage::SubtypeStr() const
{
    // Access the 'Content-Type' header field and return its 'subtype'
    // as a string

    DwHeader& header = mMessage->Header();
    if (header.HasContentType()) {
        return header.ContentType().SubtypeStr();
    }
    else {
        return mEmptyString;
    }
}


int BasicMessage::Subtype() const
{
    // Access the 'Content-Type' header field and return its 'subtype'
    // as an enumerated type

    DwHeader& header = mMessage->Header();
    if (header.HasContentType()) {
        return header.ContentType().Subtype();
    }
    else {
        return DwMime::kSubtypeNull;
    }
}


void BasicMessage::SetSubtypeStr(const DwString& aStr)
{
    // Access the 'Content-Type' header field and set its 'subtype'
    // from a string

    mMessage->Header().ContentType().SetSubtypeStr(aStr);
}


void BasicMessage::SetSubtype(int aSubtype)
{
    // Access the 'Content-Type' header field and set its 'subtype'
    // from an enumerated type

    mMessage->Header().ContentType().SetSubtype(aSubtype);
}


const DwString& BasicMessage::ContentTransferEncodingStr() const
{
    // Access the 'Content-Transfer-Encoding' header field and return
    // its contents as a string

    DwHeader& header = mMessage->Header();
    if (header.HasContentTransferEncoding()) {
        return header.ContentTransferEncoding().AsString();
    }
    else {
        return mEmptyString;
    }
}


int BasicMessage::ContentTransferEncoding() const
{
    // Access the 'Content-Transfer-Encoding' header field and return
    // its contents as an enumerated type

    DwHeader& header = mMessage->Header();
    if (header.HasContentTransferEncoding()) {
        return header.ContentTransferEncoding().AsEnum();
    }
    else {
        return DwMime::kCteNull;
    }
}


void BasicMessage::SetContentTransferEncodingStr(const DwString& aStr)
{
    // Access the 'Content-Transfer-Encoding' header field and set
    // its contents from a string

    mMessage->Header().ContentTransferEncoding().FromString(aStr);
}


void BasicMessage::SetContentTransferEncoding(int aCte)
{
    // Access the 'Content-Transfer-Encoding' header field and set
    // its contents from an enumerated type

    mMessage->Header().ContentTransferEncoding().FromEnum(aCte);
}


const DwString& BasicMessage::CteStr() const
{
    // Access the 'Content-Transfer-Encoding' header field and return
    // its contents as a string

    DwHeader& header = mMessage->Header();
    if (header.HasContentTransferEncoding()) {
        return header.ContentTransferEncoding().AsString();
    }
    else {
        return mEmptyString;
    }
}


int BasicMessage::Cte() const
{
    // Access the 'Content-Transfer-Encoding' header field and return
    // its contents as an enumerated type

    DwHeader& header = mMessage->Header();
    if (header.HasContentTransferEncoding()) {
        return header.ContentTransferEncoding().AsEnum();
    }
    else {
        return DwMime::kCteNull;
    }
}


void BasicMessage::SetCteStr(const DwString& aStr)
{
    // Access the 'Content-Transfer-Encoding' header field and set
    // its contents from a string

    mMessage->Header().ContentTransferEncoding().FromString(aStr);
}


void BasicMessage::SetCte(int aCte)
{
    // Access the 'Content-Transfer-Encoding' header field and set
    // its contents from an enumerated type

    mMessage->Header().ContentTransferEncoding().FromEnum(aCte);
}


const DwString& BasicMessage::Body() const
{
    // Access the message body and return its contents as a string

    const DwString& body = mMessage->Body().AsString();
    return body;
}


void BasicMessage::SetBody(const DwString& aStr)
{
    // Access the message body and set its contents from a string

    mMessage->Body().FromString(aStr);
}


