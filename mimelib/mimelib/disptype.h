//=============================================================================
// File:       disptype.h
// Contents:   Declarations for DwDispositionType
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

#ifndef DW_DISPTYPE_H
#define DW_DISPTYPE_H

#ifndef DW_CONFIG_H
#include <mimelib/config.h>
#endif

#ifndef DW_STRING_H
#include <mimelib/string.h>
#endif

#ifndef DW_FIELDBDY_H
#include <mimelib/fieldbdy.h>
#endif

class DwParameter;

//=============================================================================
//+ Name DwDispositionType -- Class representing a MIME content-disposition field-body
//+ Description
//. {\tt DwDispositionType} represents a {\it field-body} for the 
//. Content-Disposition header {\it field} as described in RFC-1806.  The
//. parse method for {\tt DwDispositionType} extracts the
//. {\it disposition-type} and optional {\it filename-parm} from the string
//. representation.  The {\it filename-parm} is represented by a reference
//. to a {\tt DwParameter} object.  Since RFC-1806 specifically states that
//. new parameters may be added in the future, a {\tt DwDispositionType}
//. object actually contains a list of {\tt DwParameters} objects.
//.
//. {\tt DwContentType} provides a member function {\tt DispositionType()}
//. to retrieve the {\it disposition-type} as an enumerated value and a
//. member function {\tt SetDispositionType()} to set the 
//. {\it disposition-type} from an enumerated value.  Enumerated values are
//. defined only for the standard types.  For standard or non-standard types,
//. you can get or set the {\it disposition-type} using the member functions
//. {\tt DispositionTypeStr()} and {\tt SetDispositionTypeStr()}.  The 
//. member function {\tt FirstParameter()} returns the first
//. {\tt DwParameter} object, if any, contained by the
//. {\tt DwDispositionType} object, or NULL if no {\tt DwParameter} objects
//. are present.  For the special case of the filename parameter,
//. {\tt DwDispositionType} provides convenience functions {\tt Filename()}
//. and {\tt SetFilename()} to get or set it.
//.
//. RFC-1806 specifically states that the Content-Disposition header field
//. is experimental and not a proposed standard.
//=============================================================================

//+ Noentry ~DwDispositionType _AddParameter EnumToStr StrToEnum
//+ Noentry DeleteParameterList CopyParameterList mDispositionType
//+ Noentry mDispositionTypeStr mFilenameStr mFirstParameter
//+ Noentry PrintDebugInfo _PrintDebugInfo CheckInvariants


class DW_EXPORT DwDispositionType : public DwFieldBody {

public:

    DwDispositionType();
    DwDispositionType(const DwDispositionType& aDispType);
    DwDispositionType(const DwString& aStr, DwMessageComponent* aParent=0);
    //. The first constructor is the default constructor, which sets the
    //. {\tt DwDispositionType} object's string representation to the empty
    //. string and sets its parent to NULL.
    //.
    //. The second constructor is the copy constructor, which copies the
    //. string representation from {\tt aDispType} and all of its children.
    //. The parent of the new {\tt DwDispositionType} object is set to NULL.
    //.
    //. The third constructor copies {\tt aStr} to the {\tt DwDispositionType}
    //. object's string representation and sets {\tt aParent} as its parent.
    //. The virtual member function {\tt Parse()} should be called immediately
    //. after this constructor in order to parse the string representation.
    //. Unless it is NULL, {\tt aParent} should point to an object of a class
    //. derived from {\tt DwField}.

    virtual ~DwDispositionType();

    const DwDispositionType& operator = (const DwDispositionType& aDispType);
    //. This is the assignment operator, which follows regular semantics.

    virtual void Parse();
    //. This virtual function, inherited from {\tt DwMessageComponent},
    //. executes the parse method for {\tt DwDispositionType} objects.
    //. It should be called immediately after the string representation
    //. is modified and before the parts of the broken-down
    //. representation are accessed.

    virtual void Assemble();
    //. This virtual function, inherited from {\tt DwMessageComponent},
    //. executes the assemble method for {\tt DwDispositionType} objects.
    //. It should be called whenever one of the object's attributes
    //. is changed in order to assemble the string representation from
    //. its broken-down representation.  It will be called
    //. automatically for this object by the parent object's
    //. {\tt Assemble()} member function if the is-modified flag is set.

    virtual DwMessageComponent* Clone() const;
    //. This virtual function, inherited from {\tt DwMessageComponent},
    //. creates a new {\tt DwDispositionType} object on the free store that
    //. has the same value as this {\tt DwDispositionType} object.  The basic
    //. idea is that of a ``virtual copy constructor.''

    int DispositionType() const;
    //. Returns the {\it disposition-type} as an enumerated value.
    //. Valid enumerated types, which are defined in dw_mime.h, include
    //. eDispTypeNull, eDispTypeUnknown, eDispTypeInline, and
    //. eDispTypeAttachment.

    void SetDispositionType(int aType);
    //. Sets the {\it disposition-type} from the enumerated value {\tt aType}.
    //. Valid enumerated types, which are defined in dw_mime.h, include
    //. eDispTypeNull, eDispTypeUnknown, eDispTypeInline, and
    //. eDispTypeAttachment.

    const DwString& DispositionTypeStr() const;
    //. Returns the {\it disposition-type} as a string.

    void SetDispositionTypeStr(const DwString& aStr);
    //. Sets the {\it disposition-type} from a string.

    const DwString& Filename() const;
    //. This convenience function returns the {\it value} from the
    //. {\it filename-parm} (filename parameter), if present.  If no
    //. {\it filename-parm} is present, the empty string is returned.

    void SetFilename(const DwString& aStr);
    //. This convenience function sets the {\it value} of the
    //. {\it filename-parm} (filename parameter) to {\tt aStr}.

    DwParameter* FirstParameter() const;
    //. Returns the first {\tt DwParameter} object in the list managed by
    //. this {\tt DwDispositionType} object, or NULL if no parameters are
    //. present.  Use {\tt DwParameter::Next()} to iterate through the list.

    void AddParameter(DwParameter* aParam);
    //. Adds a {\tt DwParameter} object to the list managed by this
    //. {\tt DwDispositionType} object.

    static DwDispositionType* NewDispositionType(const DwString& aStr,
        DwMessageComponent* aParent);
    //. Creates a new {\tt DwDispositionType} object on the free store.
    //. If the static data member {\tt sNewDispositionType} is NULL, 
    //. this member function will create a new {\tt DwDispositionType}
    //. and return it.  Otherwise, {\tt NewDispositionType()} will call
    //. the user-supplied function pointed to by {\tt sNewDispositionType},
    //. which is assumed to return an object from a class derived from
    //. {\tt DwDispositionType}, and return that object.

    //+ Var sNewDispositionType
    static DwDispositionType* (*sNewDispositionType)(const DwString&,
        DwMessageComponent*);
    //. If {\tt sNewDispositionType} is not NULL, it is assumed to point
    //. to a  user-supplied function that returns an object from a class
    //. derived from {\tt DwDispositionType}.

protected:

    void _AddParameter(DwParameter* aParam);
    //. Add parameter.  Don't set is-modified flag.

    virtual void EnumToStr();
    virtual void StrToEnum();
    void DeleteParameterList();
    void CopyParameterList(DwParameter* aFirst);

    int mDispositionType;
    DwString mDispositionTypeStr;
    DwString mFilenameStr;
    DwParameter* mFirstParameter;

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

#endif
