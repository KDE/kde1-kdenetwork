//=============================================================================
// File:       fieldbdy.h
// Contents:   Declarations for DwFieldBody
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

#ifndef DW_FIELDBDY_H
#define DW_FIELDBDY_H

#include <iostream.h>

#ifndef DW_CONFIG_H
#include <mimelib/config.h>
#endif

#ifndef DW_STRING_H
#include <mimelib/string.h>
#endif

#ifndef DW_MSGCMP_H
#include <mimelib/msgcmp.h>
#endif

//=============================================================================
//+ Name DwFieldBody -- Class representing a MIME header field-body
//+ Description
//. {\tt DwFieldBody} represents the field-body element in the grammar
//. specified by RFC-822.  It is an abstract base class that defines
//. the interface common to all structured field bodies.
//.
//. Some MIME parsers are broken in that they do not handle the folding of
//. some fields properly.  {\tt DwFieldBody} folds its string representation
//. by default.  You can disable folding, however, by calling the
//. {\tt SetFolding()} member function.
//=============================================================================

class DW_EXPORT DwFieldBody : public DwMessageComponent {

    friend class DwField;

public:

    DwFieldBody();
    DwFieldBody(const DwFieldBody& aFieldBody);
    DwFieldBody(const DwString& aStr, DwMessageComponent* aParent=0);
    //. The first constructor is the default constructor, which sets the
    //. {\tt DwFieldBody} object's string representation to the empty
    //. string and set its parent to NULL.
    //.
    //. The second constructor is the copy constructor, which
    //. copies string representation and all elements of the broken-down
    //. represenation from {\tt aFieldBody}.
    //. The parent of the new {\tt DwHeader} object is set to NULL.
    //.
    //. The third constructor copies {\tt aStr} to the {\tt DwFieldBody}
    //. object's string
    //. representation and sets {\tt aParent} as the object's parent.
    //. The virtual member function {\tt Parse()} should be called
    //. immediately after this constructor in order to parse the string
    //. representation.
    //. Unless it is NULL, {\tt aParent} should point to an object of a class
    //. derived from {\tt DwField}.

    virtual ~DwFieldBody();

    const DwFieldBody& operator = (const DwFieldBody& aFieldBody);
    //. This is the assignment operator, which follows regular semantics.

    void SetOffset(int aOffset);
    //. Sets the offset to {\tt aOffset}.  The offset is used when folding
    //. lines.  It indicates how much the first line should be offset to
    //. account for the {\it field-name}, colon, and initial white space.

    void SetFolding(DwBool aTrueOrFalse);
    //. Enables ({\tt aTrueOrFalse = DwTrue}) or disables
    //. ({\tt aTrueOrFalse = DwFalse}) the folding of fields.  The default
    //. is to fold fields.  Unfortunately, some parsers are broke and
    //. do not handle folded lines properly.  This function allows a kludge
    //. to deal with these broken parsers.

    DwBool IsFolding() const;
    //. Returns a boolean indicating if folding of fields is enabled.

protected:

    int mLineOffset;
    DwBool mDoFolding;

private:

    static const char* const sClassName;

public:

    virtual void PrintDebugInfo(ostream& aStrm, int aDepth=0) const;
    //. This virtual function, inherited from {\tt DwMessageComponent},
    //. prints debugging information about this object to {\tt aStrm}.
    //. It will also call {\tt PrintDebugInfo()} for any of its child
    //. components down to a level of {\tt aDepth}.
    //.
    //. This member function is available only in the debug version of
    //. the library.

    virtual void CheckInvariants() const;
    //. Aborts if one of the invariants of the object fails.  Use this
    //. member function to track down bugs.
    //.
    //. This member function is available only in the debug version of
    //. the library.

protected:

    void _PrintDebugInfo(ostream& aStrm) const;

};


inline void DwFieldBody::SetOffset(int aOffset) 
{ 
    mLineOffset = aOffset; 
}


inline void DwFieldBody::SetFolding(DwBool aTrueOrFalse)
{
    mDoFolding = aTrueOrFalse;
}


inline DwBool DwFieldBody::IsFolding() const
{
    return mDoFolding;
}

#endif
