//=============================================================================
// File:       mediatyp.h
// Contents:   Declarations for DwMediaType
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

#ifndef DW_MEDIATYP_H
#define DW_MEDIATYP_H

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
//+ Name DwMediaType -- Class representing a MIME media-type
//+ Description
//. {\tt DwMediaType} represents a {\it field-body} for the Content-Type
//. header field as described in RFC-2045.  (The term {\it media-type}
//. does not appear in RFC-2045 -- it was borrowed from RFC-2068, which
//. describes HTTP.)  The parse method for
//. {\tt DwMediaType} extracts the type and subtype from the string
//. representation and sets the corresponding attributes in the
//. {\tt DwMediaType} object.  It also creates a list of {\tt DwParameter}
//. objects, which are children of the {\tt DwMediaType} object.
//. {\tt DwMediaType} provides methods to set or get its type and
//. subtype attributes, plus methods to manage its list of {\tt DwParameter}
//. objects.
//.
//. Some MIME parsers have problems with folded header fields, and this
//. especially seems to be a problem with the Content-Type field.
//. To disable folding when the {\tt DwMediaType} object is assembled,
//. call the inherited member function {\tt DwFieldBody::SetFolding()}
//. with an argument of {\tt DwFalse} to disable it.
//=============================================================================

//+ Noentry ~DwMediaType
//+ Noentry _AddParameter TypeEnumToStr TypeStrToEnum SubtypeEnumToStr
//+ Noentry SubtypeStrToEnum DeleteParameterList CopyParameterList
//+ Noentry mType mSubtype mTypeStr mSubtypeStr mBoundaryStr mFirstParameter

class DW_EXPORT DwMediaType : public DwFieldBody {

public:

    DwMediaType();
    DwMediaType(const DwMediaType& aMediaType);
    DwMediaType(const DwString& aStr, DwMessageComponent* aParent=0);
    //. The first constructor is the default constructor, which sets the
    //. {\tt DwMediaType} object's string representation to the empty string
    //. and sets its parent to NULL.
    //.
    //. The second constructor is the copy constructor, which copies the
    //. string representation from {\tt aMediaType} and all of its children.
    //. The parent of the new {\tt DwMediaType} object is set to NULL.
    //.
    //. The third constructor copies {\tt aStr} to the {\tt DwMediaType}
    //. object's string representation and sets {\tt aParent} as its parent.
    //. The virtual member function {\tt Parse()} should be called immediately
    //. after this constructor in order to parse the string representation.
    //. Unless it is NULL, {\tt aParent} should point to an object of a class
    //. derived from {\tt DwField}.

    virtual ~DwMediaType();

    const DwMediaType& operator = (const DwMediaType& aMediaType);
    //. This is the assignment operator, which follows regular semantics.

    virtual void Parse();
    //. This virtual function, inherited from {\tt DwMessageComponent},
    //. executes the parse method for {\tt DwMediaType} objects.
    //. It should be called immediately after the string representation
    //. is modified and before the parts of the broken-down
    //. representation are accessed.

    virtual void Assemble();
    //. This virtual function, inherited from {\tt DwMessageComponent},
    //. executes the assemble method for {\tt DwMediaType} objects.
    //. It should be called whenever one of the object's attributes
    //. is changed in order to assemble the string representation from
    //. its broken-down representation.  It will be called
    //. automatically for this object by the parent object's
    //. {\tt Assemble()} member function if the is-modified flag is set.

    virtual DwMessageComponent* Clone() const;
    //. This virtual function, inherited from {\tt DwMessageComponent},
    //. creates a new {\tt DwMediaType} object on the free store that
    //. has the same value as this {\tt DwMediaType} object.  The basic
    //. idea is that of a ``virtual copy constructor.''

    int Type() const;
    //. Returns the type from the Content-Type {\it field-body} as an
    //. enumerated value.  Enumerated values are defined for all
    //. standard types in the file dw_mime.h.  If the type is non-standard
    //. {\tt eTypeUnknown} is returned.  The member function
    //. {\tt TypeStr()} may be used to get the value of any type,
    //. standard or non-standard, as a string.

    void SetType(int aType);
    //. Sets the type for the Content-Type {\it field-body} from the
    //. enumerated value {\tt aType}.  Enumerated values are defined for
    //. all standard types in the file dw_mime.h.  The member function
    //. {\tt SetTypeStr()} may be used to set the value of any type,
    //. standard or non-standard, from a string.

    const DwString& TypeStr() const;
    //. Returns the type from the Content-Type {\it field-body} as a
    //. string.

    void SetTypeStr(const DwString& aStr);
    //. Sets the type for the Content-Type {\it field-body} from a
    //. string.

    int Subtype() const;
    //. Returns the subtype from the Content-Type {\it field-body} as an
    //. enumerated value.  Enumerated values are defined for all
    //. standard subtypes in the file dw_mime.h.  If the subtype is
    //. non-standard {\tt eSubtypeUnknown} is returned.  The member function
    //. {\tt SubtypeStr()} may be used to get the value of any subtype,
    //. standard or non-standard, as a string.

    void SetSubtype(int aSubtype);
    //. Sets the subtype for the Content-Type {\it field-body} from the
    //. enumerated value {\tt aSubtype}.  Enumerated values are defined for
    //. all standard subtypes in the file dw_mime.h.  The member function
    //. {\tt SetSubtypeStr()} may be used to set the value of any subtype,
    //. standard or non-standard, from a string.

    const DwString& SubtypeStr() const;
    //. Returns the subtype from the Content-Type {\it field-body} as a
    //. string.

    void SetSubtypeStr(const DwString& aStr);
    //. Sets the subtype for the Content-Type {\it field-body} from a
    //. string.

    const DwString& Boundary() const;
    //. For the multipart type only, returns the value of the {\it boundary}
    //. {\it parameter}.  This member function is a convenience function
    //. that searches the {\tt DwParameter} children of this
    //. {\tt DwMediaType}.

    void SetBoundary(const DwString& aStr);
    //. For the multipart type only, sets the value of the {\it boundary}
    //. {\it parameter}.
    //. This member function is a convenience function that accesses the
    //. list of child {\tt DwParameter} objects for you.

    virtual void CreateBoundary(unsigned aLevel=0);
    //. For the multipart type only, creates a {\it boundary}.  {\tt aLevel}
    //. indicates the level of a nested multipart {\it body part}; if it
    //. is positive, it is used to form part of the created boundary string.
    //. This member function is a convenience function that accesses the
    //. list of child {\tt DwParameter} objects for you.

    DwParameter* FirstParameter() const;
    //. Returns the first {\tt DwParameter} object in the list managed by
    //. this {\tt DwMediaType} object.  Use {\tt DwParameter::Next()} to
    //. iterate through the list.

    void AddParameter(DwParameter* aParam);
    //. Adds a {\tt DwParameter} object to the list managed by this
    //. {\tt DwMediaType} object.

    static DwMediaType* NewMediaType(const DwString& aStr,
        DwMessageComponent* aParent);
    //. Creates a new {\tt DwMediaType} object on the free store.
    //. If the static data member {\tt sNewMediaType} is NULL, 
    //. this member function will create a new {\tt DwMediaType}
    //. and return it.  Otherwise, {\tt NewMediaType()} will call
    //. the user-supplied function pointed to by {\tt sNewMediaType},
    //. which is assumed to return an object from a class derived from
    //. {\tt DwMediaType}, and return that object.

    //+ Var sNewMediaType
    static DwMediaType* (*sNewMediaType)(const DwString&,
        DwMessageComponent*);
    //. If {\tt sNewMediaType} is not NULL, it is assumed to point to a 
    //. user-supplied function that returns an object from a class derived
    //. from {\tt DwMediaType}.

protected:

    // Add parameter.  Don't set is-modified flag.
    void _AddParameter(DwParameter* aParam);
    virtual void TypeEnumToStr();
    virtual void TypeStrToEnum();
    virtual void SubtypeEnumToStr();
    virtual void SubtypeStrToEnum();
    void DeleteParameterList();
    void CopyParameterList(DwParameter* aFirst);

    int mType;
    int mSubtype;
    DwString mTypeStr;
    DwString mSubtypeStr;
    DwString mBoundaryStr;
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
