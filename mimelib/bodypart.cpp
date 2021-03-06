//=============================================================================
// File:       bodypart.cpp
// Contents:   Definitions for DwBodyPart
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
#include <mimelib/debug.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <iostream>
#include <mimelib/string.h>
#include <mimelib/headers.h>
#include <mimelib/bodypart.h>
#include <mimelib/body.h>
#include <mimelib/message.h>

const char* const DwBodyPart::sClassName = "DwBodyPart";


DwBodyPart* (*DwBodyPart::sNewBodyPart)(const DwString&,
    DwMessageComponent*) = 0;


DwBodyPart* DwBodyPart::NewBodyPart(const DwString& aStr,
    DwMessageComponent* aParent)
{
    if (sNewBodyPart) {
        return sNewBodyPart(aStr, aParent);
    }
    else {
        return new DwBodyPart(aStr, aParent);
    }
}


DwBodyPart::DwBodyPart()
{
    mNext = 0;
    mClassId = kCidBodyPart;
    mClassName = sClassName;
}


DwBodyPart::DwBodyPart(const DwBodyPart& aPart)
  : DwEntity(aPart)
{
    mNext = 0;
    mClassId = kCidBodyPart;
    mClassName = sClassName;
}


DwBodyPart::DwBodyPart(const DwString& aStr, DwMessageComponent* aParent)
  : DwEntity(aStr, aParent)
{
    mNext = 0;
    mClassId = kCidBodyPart;
    mClassName = sClassName;
}



DwBodyPart::~DwBodyPart()
{
}


const DwBodyPart& DwBodyPart::operator = (const DwBodyPart& aPart)
{
    if (this == &aPart) return *this;
    DwEntity::operator = (aPart);
    return *this;
}


DwBodyPart* DwBodyPart::Next() const
{
    return (DwBodyPart*) mNext;
}


void DwBodyPart::SetNext(const DwBodyPart* aPart)
{
    mNext = aPart;
}


DwMessageComponent* DwBodyPart::Clone() const
{
    return new DwBodyPart(*this);
}



void DwBodyPart::PrintDebugInfo(ostream& aStrm, int aDepth) const
{
#if defined(DW_DEBUG_VERSION)
    aStrm << "----------- Debug info for DwBodyPart class -----------\n";
    _PrintDebugInfo(aStrm);
    int depth = aDepth - 1;
    depth = (depth >= 0) ? depth : 0;
    if (aDepth == 0 || depth > 0) {
        mHeaders->PrintDebugInfo(aStrm, depth);
        mBody->PrintDebugInfo(aStrm, depth);
    }
#endif // defined(DW_DEBUG_VERSION)
}


void DwBodyPart::_PrintDebugInfo(ostream& aStrm) const
{
#if defined(DW_DEBUG_VERSION)
    DwEntity::_PrintDebugInfo(aStrm);
    aStrm << "Next body part:   ";
    if (mNext) {
        aStrm << mNext->ObjectId() << '\n';
    }
    else {
        aStrm << "(none)\n";
    }
#endif // defined(DW_DEBUG_VERSION)
}


void DwBodyPart::CheckInvariants() const
{
#if defined(DW_DEBUG_VERSION)
    DwEntity::CheckInvariants();
#endif // defined(DW_DEBUG_VERSION)
}

