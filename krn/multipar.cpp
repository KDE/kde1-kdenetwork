//=============================================================================
// File:       multipar.cpp
// Contents:   Definitions for MultiparBodyPart and MultipartMessage
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
#include "multipar.h"


MultipartBodyPart::MultipartBodyPart()
  : mBody(DwString()),
    mType("Text"),
    mSubtype("Plain"),
    mCte("7bit")
{
}


MultipartBodyPart::~MultipartBodyPart()
{
}


const DwString& MultipartBodyPart::TypeStr() const
{
    return mType;
}


int MultipartBodyPart::Type() const
{
    int type = DwTypeStrToEnum(mType);
    return type;
}


void MultipartBodyPart::SetTypeStr(const DwString& aStr)
{
    mType = aStr;
}


void MultipartBodyPart::SetType(int aType)
{
    DwTypeEnumToStr(aType, mType);
}



const DwString& MultipartBodyPart::SubtypeStr() const
{
    return mSubtype;
}


int MultipartBodyPart::Subtype() const
{
    int subtype = DwSubtypeStrToEnum(mSubtype);
    return subtype;
}


void MultipartBodyPart::SetSubtypeStr(const DwString& aStr)
{
    mSubtype = aStr;
}


void MultipartBodyPart::SetSubtype(int aSubtype)
{
    DwSubtypeEnumToStr(aSubtype, mSubtype);
}


const DwString& MultipartBodyPart::ContentTransferEncodingStr() const
{
    return mCte;
}


int MultipartBodyPart::ContentTransferEncoding() const
{
    int cte = DwCteStrToEnum(mCte);
    return cte;
}


void MultipartBodyPart::SetContentTransferEncodingStr(const DwString& aStr)
{
    mCte = aStr;
}


void MultipartBodyPart::SetContentTransferEncoding(int aCte)
{
    DwCteEnumToStr(aCte, mCte);
}


const DwString& MultipartBodyPart::CteStr() const
{
    return mCte;
}


int MultipartBodyPart::Cte() const
{
    int cte = DwCteStrToEnum(mCte);
    return cte;
}


void MultipartBodyPart::SetCteStr(const DwString& aStr)
{
    mCte = aStr;
}


void MultipartBodyPart::SetCte(int aCte)
{
    DwCteEnumToStr(aCte, mCte);
}


const DwString& MultipartBodyPart::ContentDescription() const
{
    return mContentDescription;
}


void MultipartBodyPart::SetContentDescription(const DwString& aStr)
{
    mContentDescription = aStr;
}

    
const DwString& MultipartBodyPart::ContentDisposition() const
{
    return mContentDisposition;
}

void MultipartBodyPart::SetContentDisposition(const DwString& aStr)
{
    mContentDisposition = aStr;
}


const DwString& MultipartBodyPart::Body() const
{
    return mBody;
}


void MultipartBodyPart::SetBody(const DwString& aStr)
{
    mBody = aStr;
}


//-------------------------------------------------------------------------


MultipartMessage::MultipartMessage()
{
}


MultipartMessage::MultipartMessage(DwMessage* aMsg)
  : BasicMessage(aMsg)
{
}


MultipartMessage::~MultipartMessage()
{
}


void MultipartMessage::SetAutomaticFields()
{
    BasicMessage::SetAutomaticFields();

    // Set the type to 'Multipart' and the subtype to 'Mixed'

    DwMediaType& contentType = mMessage->Header().ContentType();
    contentType.SetType(DwMime::kTypeMultipart);
    contentType.SetSubtype(DwMime::kSubtypeMixed);

    // Create a random printable string and set it as the boundary parameter

    contentType.CreateBoundary(0);
}


int MultipartMessage::NumberOfParts() const 
{
    int count = 0;
    DwBodyPart* part = mMessage->Body().FirstBodyPart();
    while (part) {
        ++count;
        part = part->Next();
    }
    return count;
}


void MultipartMessage::BodyPart(int aIdx, MultipartBodyPart& aPart)
{
    // Get the DwBodyPart for this index

    DwBodyPart* part = mMessage->Body().FirstBodyPart();
    for (int curIdx=0; curIdx < aIdx && part; ++curIdx) {
        part = part->Next();
    }

    // If the DwBodyPart was found get the header fields and body

    if (part != 0) {
        DwHeader& header = part->Header();
        
        // Content-type

        if (header.HasContentType()) {
            const DwString& type    = header.ContentType().TypeStr();
            const DwString& subtype = header.ContentType().SubtypeStr();
            aPart.SetTypeStr(type);
            aPart.SetSubtypeStr(subtype);
        }
        else {
            // Set to defaults
            aPart.SetTypeStr("Text");
            aPart.SetSubtypeStr("Plain");
        }

        // Content-transfer-encoding

        if (header.HasContentTransferEncoding()) {
            const DwString& cte = header.ContentTransferEncoding().AsString();
            aPart.SetCteStr(cte);
        }
        else {
            // Set to default
            aPart.SetCteStr("7bit");
        }

        // Content-description

        if (header.HasContentDescription()) {
            const DwString& desc = header.ContentDescription().AsString();
            aPart.SetContentDescription(desc);
        }
        else {
            aPart.SetContentDescription("");
        }

        // Content-disposition

        if (header.HasContentDisposition()) {
            const DwString& disp = header.ContentDisposition().AsString();
            aPart.SetContentDisposition(disp);
        }
        else {
            aPart.SetContentDisposition("");
        }

        // Body

        const DwString& body = part->Body().AsString();
        aPart.SetBody(body);
    }
    
    // If the body part was not found, set all MultipartBodyPart attributes
    // to empty values.  This only happens if you don't pay attention to
    // the value returned from NumberOfParts().
    else {
        aPart.SetTypeStr("");
        aPart.SetSubtypeStr("");
        aPart.SetCteStr("");
        aPart.SetContentDescription("");
        aPart.SetContentDisposition("");
        aPart.SetBody("");
    }
}


void MultipartMessage::SetBodyPart(int aIdx, const MultipartBodyPart& aPart)
{
    DwBody& body = mMessage->Body();
    int numParts = NumberOfParts();
    DwBodyPart* part;
    // If indexed part exists already, just replace its values
    if (0 <= aIdx && aIdx < numParts) {
        part = body.FirstBodyPart();
        for (int curIdx=0; curIdx < aIdx; ++curIdx) {
            part = part->Next();
        }
    }
    // Otherwise, add as many new parts as necessary.
    else {
        while (numParts <= aIdx) {
            part = DwBodyPart::NewBodyPart(mEmptyString, 0);
            body.AddBodyPart(part);
            ++numParts;
        }
    }

    const DwString& type     = aPart.TypeStr();
    const DwString& subtype  = aPart.SubtypeStr();
    const DwString& cte      = aPart.CteStr();
    const DwString& contDesc = aPart.ContentDescription();
    const DwString& contDisp = aPart.ContentDisposition();
    const DwString& bodyStr  = aPart.Body();

    DwHeader& header = part->Header();
    if (type != "" && subtype != "") {
        header.ContentType().SetTypeStr(type);
        header.ContentType().SetSubtypeStr(subtype);
    }
    if (cte != "") {
        header.Cte().FromString(cte);
    }
    if (contDesc != "") {
        header.ContentDescription().FromString(contDesc);
    }
    if (contDisp != "") {
        header.ContentDisposition().FromString(contDisp);
    }
    part->Body().FromString(bodyStr);
}


void MultipartMessage::AddBodyPart(const MultipartBodyPart& aPart)
{
    DwBodyPart* part = DwBodyPart::NewBodyPart(mEmptyString, 0);

    const DwString& type     = aPart.TypeStr();
    const DwString& subtype  = aPart.SubtypeStr();
    const DwString& cte      = aPart.CteStr();
    const DwString& contDesc = aPart.ContentDescription();
    const DwString& contDisp = aPart.ContentDisposition();
    const DwString& bodyStr  = aPart.Body();

    DwHeader& header = part->Header();
    if (type != "" && subtype != "") {
        header.ContentType().SetTypeStr(type);
        header.ContentType().SetSubtypeStr(subtype);
    }
    if (cte != "") {
        header.Cte().FromString(cte);
    }
    if (contDesc != "") {
        header.ContentDescription().FromString(contDesc);
    }
    if (contDisp != "") {
        header.ContentDisposition().FromString(contDisp);
    }
    part->Body().FromString(bodyStr);

    mMessage->Body().AddBodyPart(part);
}
