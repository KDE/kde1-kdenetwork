//=============================================================================
// File:       boyermor.h
// Contents:   Declarations for DwBoyerMoore
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

#ifndef DW_BOYERMOR_H
#define DW_BOYERMOR_H

#include <stddef.h>

#ifndef DW_CONFIG_H
#include <mimelib/config.h>
#endif

class DwString;


class DW_EXPORT DwBoyerMoore {

public:
    
    DwBoyerMoore(const char* aCstr);
    DwBoyerMoore(const DwString& aStr);
    virtual ~DwBoyerMoore();

    void Assign(const char* aCstr);
    void Assign(const DwString& aStr);

    size_t FindIn(const DwString& aStr, size_t aPos);

private:

    size_t mPatLen;
    char* mPat;
    unsigned char mSkipAmt[256];

    void _Assign(const char* aPat, size_t aPatLen);
};

#endif
