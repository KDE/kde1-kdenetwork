//=============================================================================
// File:       header.cpp
// Contents:   Definitions for DwHeader
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
#include <mimelib/debug.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <iostream.h>
#include <mimelib/string.h>
#include <mimelib/header.h>
#include <mimelib/field.h>
#include <mimelib/body.h>
#include <mimelib/datetime.h>
#include <mimelib/mailbox.h>
#include <mimelib/address.h>
#include <mimelib/mechansm.h>
#include <mimelib/mediatyp.h>
#include <mimelib/msgid.h>
#include <mimelib/text.h>


class DwHeaderParser {
    friend class DwHeader;
private:
    DwHeaderParser(const DwString&);
    void Rewind();
    void NextField(DwString*);
    const DwString mString;
    size_t mPos;
};


DwHeaderParser::DwHeaderParser(const DwString& aStr)
  : mString(aStr)
{
    mPos = 0;
}


void DwHeaderParser::Rewind()
{
    mPos = 0;
}


void DwHeaderParser::NextField(DwString* aStr)
{
    if (!aStr) {
        return;
    }
    const char* buf = mString.data();
    size_t bufEnd = mString.length();
    size_t pos = mPos;
    size_t start = pos;
    size_t len = 0;
    while (pos < bufEnd) {
        if (buf[pos] == '\n'
            && pos+1 < bufEnd
            && buf[pos+1] != ' '
            && buf[pos+1] != '\t') {

            ++len;
            ++pos;
            break;
        }
        ++len;
        ++pos;
    }
    *aStr = mString.substr(start, len);
    mPos = pos;
}


//============================================================================


const char* const DwHeader::sClassName = "DwHeader";


DwHeader* (*DwHeader::sNewHeader)(const DwString&, DwMessageComponent*) = 0;


DwHeader* DwHeader::NewHeader(const DwString& aStr,
    DwMessageComponent* aParent)
{
    if (sNewHeader) {
        return sNewHeader(aStr, aParent);
    }
    else {
        return new DwHeader(aStr, aParent);
    }
}


DwHeader::DwHeader()
{
    mFirstField = 0;
    mClassId = eHeader;
    mClassName = sClassName;
}


DwHeader::DwHeader(const DwHeader& aHeader)
  : DwMessageComponent(aHeader)
{
    mFirstField = 0;
    if (aHeader.mFirstField) {
        CopyFields(aHeader.mFirstField);
    }
    mClassId = eHeader;
    mClassName = sClassName;
}


DwHeader::DwHeader(const DwString& aStr, DwMessageComponent* aParent)
  : DwMessageComponent(aStr, aParent)
{
    mFirstField = 0;
    mClassId = eHeader;
    mClassName = sClassName;
}


DwHeader::~DwHeader()
{
    if (mFirstField) {
        DeleteAllFields();
    }
}


const DwHeader& DwHeader::operator = (const DwHeader& aHeader)
{
    if (this == &aHeader) return *this;
    DwMessageComponent::operator = (aHeader);
    if (mFirstField) {
        DeleteAllFields();
    }
    if (aHeader.mFirstField) {
        CopyFields(aHeader.mFirstField);
    }
    if (mParent) {
        mParent->SetModified();
    }
    return *this;
}


void DwHeader::Parse()
{
    mIsModified = 0;
    DwHeaderParser parser(mString);
    DwString str;
    parser.NextField(&str);
    while (str != "") {
        DwField* field = DwField::NewField(str, this);
        field->Parse();
        _AddField(field);
        parser.NextField(&str);
    }
}


void DwHeader::Assemble()
{
    if (!mIsModified) return;
    mString = "";
    DwField* field = FirstField();
    while (field) {
        field->Assemble();
        mString += field->AsString();
        field = field->Next();
    }
    mString += DW_EOL;
    mIsModified = 0;
}


DwMessageComponent* DwHeader::Clone() const
{
    return new DwHeader(*this);
}


DwFieldBody& DwHeader::FieldBody(const DwString& aFieldName)
{
    assert(aFieldName != "");
    // First, search for field
    DwField* field = FindField(aFieldName);
    // If the field is not found, create the field and its field body
    if (field == 0) {
        field = DwField::NewField("", this);
        field->SetFieldNameStr(aFieldName);
        DwFieldBody* fieldBody = DwField::CreateFieldBody(aFieldName,
            "", field);
        field->SetFieldBody(fieldBody);
        AddField(field);
    }
    // Get the field body
    DwFieldBody* fieldBody = field->FieldBody();
    // If it does not exist, create it
    if (fieldBody == 0) {
        fieldBody = DwField::CreateFieldBody(aFieldName, "", field);
        field->SetFieldBody(fieldBody);
        SetModified();
    }
    return *fieldBody;
}


int DwHeader::NumFields() const
{
    int count = 0;
    DwField* field = mFirstField;
    while (field) {
        ++count;
        field = field->Next();
    }
    return count;
}


DwField* DwHeader::FindField(const char* aFieldName) const
{
    assert(aFieldName != 0);
    if (aFieldName == 0) return 0;
    DwField* field = mFirstField;
    while (field) {
        if (DwStrcasecmp(field->FieldNameStr(), aFieldName) == 0) {
            break;
        }
        field = field->Next();
    }
    return field;
}


DwField* DwHeader::FindField(const DwString& aFieldName) const
{
    DwField* field = mFirstField;
    while (field) {
        if (DwStrcasecmp(field->FieldNameStr(), aFieldName) == 0) {
            break;
        }
        field = field->Next();
    }
    return field;
}


void DwHeader::AddOrReplaceField(DwField* aField)
{
    assert(aField != 0);
    if (aField == 0) return;
    SetModified();
    const DwString& fieldName = aField->FieldNameStr();
    DwField* prevField = 0;
    DwField* field = mFirstField;
    while (field) {
        if (DwStrcasecmp(field->FieldNameStr(), fieldName) == 0) {
            break;
        }
        prevField = field;
        field = field->Next();
    }
    // Field was not found, so just add it
    if (!field) {
        _AddField(aField);
    }
    // Field was found. Replace the old one with the new one.
    else {
        if (prevField) {
            prevField->SetNext(aField);
        }
        else {
            mFirstField = aField;
        }
        aField->SetNext(field->Next());
        delete field;
    }
}


void DwHeader::AddField(DwField* aField)
{
    assert(aField != 0);
    if (aField == 0) return;
    _AddField(aField);
    SetModified();
}


void DwHeader::AddFieldAt(int aPos, DwField* aField)
{
    assert(aField != 0);
    if (aField == 0) return;
    SetModified();
    // Special case: empty list
    if (mFirstField == 0) {
        aField->SetNext(0);
        mFirstField = aField;
        return;
    }
    // Special case: aPos == 1 --> add at beginning
    if (aPos == 1) {
        aField->SetNext(mFirstField);
        mFirstField = aField;
        return;
    }
    // aPos == 0 --> at at end
    if (aPos == 0) {
        _AddField(aField);
        return;
    }
    int count = 2;
    DwField* field = mFirstField;
    while (field->Next() && count < aPos) {
        field = field->Next();
        ++count;
    }
    aField->SetNext(field->Next());
    field->SetNext(aField);
}


void DwHeader::RemoveField(DwField* aField)
{
    DwField* prevField = 0;
    DwField* field = mFirstField;
    while (field) {
        if (field == aField) {
            break;
        }
        prevField = field;
        field = field->Next();
    }
    // If we found the field...
    if (field) {
        if (prevField == 0) {
            mFirstField = field->Next();
        }
        else {
            prevField->SetNext(field->Next());
        }
        field->SetNext(0);
        SetModified();
    }
}


void DwHeader::DeleteAllFields()
{
    DwField* field = mFirstField;
    while (field) {
        DwField* nextField = field->Next();
        delete field;
        field = nextField;
    }
    mFirstField = 0;
}


void DwHeader::_AddField(DwField* aField)
{
    if (aField == 0) return;
    // Add field with setting is-modified flag for header
    aField->SetParent(this);
    // Special case: empty list
    if (mFirstField == 0) {
        mFirstField = aField;
        return;
    }
    DwField* field = mFirstField;
    while (field->Next()) {
        field = field->Next();
    }
    field->SetNext(aField);
}


void DwHeader::CopyFields(DwField* aFirst)
{
    DwField* field = aFirst;
    DwField* newField;
    while (field) {
        newField = (DwField*) field->Clone();
        _AddField(newField);
        field = field->Next();
    }
}


void DwHeader::PrintDebugInfo(ostream& aStrm, int aDepth) const
{
#if defined (DW_DEBUG_VERSION)
    aStrm <<
    "----------------- Debug info for DwHeader class ----------------\n";
    _PrintDebugInfo(aStrm);
    int depth = aDepth - 1;
    depth = (depth >= 0) ? depth : 0;
    if (aDepth == 0 || depth > 0) {
        DwField* field = mFirstField;
        while (field) {
            field->PrintDebugInfo(aStrm, depth);
            field = (DwField*) field->Next();
        }
    }
#endif // defined (DW_DEBUG_VERSION)
}


void DwHeader::_PrintDebugInfo(ostream& aStrm) const
{
#if defined (DW_DEBUG_VERSION)
    DwMessageComponent::_PrintDebugInfo(aStrm);
    aStrm << "Fields:           ";
    int count = 0;
    DwField* field = mFirstField;
    while (field) {
        if (count > 0) aStrm << ' ';
        aStrm << field->ObjectId();
        field = (DwField*) field->Next();
        ++count;
    }
    aStrm << '\n';
#endif // defined (DW_DEBUG_VERSION)
}


void DwHeader::CheckInvariants() const
{
#if defined (DW_DEBUG_VERSION)
    DwMessageComponent::CheckInvariants();
    DwField* field = mFirstField;
    while (field) {
        field->CheckInvariants();
        assert((DwMessageComponent*) this == field->Parent());
        field = (DwField*) field->Next();
    }
#endif // defined (DW_DEBUG_VERSION)
}


#ifndef DW_USE_INLINE_FUNCTIONS
#define DW_INLINE
#include "header.i"
#endif


