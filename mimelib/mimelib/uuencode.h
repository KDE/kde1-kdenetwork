//=============================================================================
// File:       uuencode.h
// Contents:   Declarations for DwUuencode
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

#ifndef DW_UUENCODE_H
#define DW_UUENCODE_H

#ifndef DW_CONFIG_H
#include <mimelib/config.h>
#endif

#ifndef DW_STRING_H
#include <mimelib/string.h>
#endif


class DW_EXPORT DwUuencode {

public:

    DwUuencode();
    virtual ~DwUuencode();

    void SetFileName(const char* aName);
    const char* FileName() const;

    void SetMode(DwUint16 aMode);
    DwUint16 Mode() const;

    void SetBinaryChars(const DwString& aStr);
    const DwString& BinaryChars() const;

    void SetAsciiChars(const DwString& aStr);
    const DwString& AsciiChars() const;

    DwBool Encode();

    DwBool Decode();

private:

    char     mFileName[256];
    DwUint16 mMode;

    DwString mBinaryChars;
    DwString mAsciiChars;
   
};

#endif
