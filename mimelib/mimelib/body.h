//=============================================================================
// File:       body.h
// Contents:   Declarations for DwBody
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

#ifndef DW_BODY_H
#define DW_BODY_H

#ifndef DW_CONFIG_H
#include <mimelib/config.h>
#endif

#ifndef DW_STRING_H
#include <mimelib/string.h>
#endif

#ifndef DW_ENTITY_H
#include <mimelib/entity.h>
#endif

class DwMessage;
class DwEntity;
class DwBodyPart;

//=============================================================================
//+ Name DwBody -- Class representing a MIME message body
//+ Description
//. {\tt DwBody} represents a {\it body}, as described in RFC-1521.
//=============================================================================

class DW_EXPORT DwBody : public DwMessageComponent {

    friend class DwHeader;
    friend class DwEntity;
    friend class DwBodyPart;

public:

    DwBody();
    DwBody(const DwBody& aBody);
    DwBody(const DwString& aStr, DwMessageComponent* aParent=0);
    //. The first constructor is the default constructor, which sets the
    //. {\tt DwBody} object's string representation to the empty string
    //. and sets its parent to NULL.
    //.
    //. The second constructor is the copy constructor, which copies the
    //. string representation from {\tt aBody} and all of its children.
    //. The parent of the new {\tt DwBody} object is set to NULL.
    //.
    //. The third constructor copies {\tt aStr} to the {\tt DwBody}
    //. object's string representation and sets {\tt aParent} as its parent.
    //. The virtual member function {\tt Parse()} should be called immediately
    //. after this constructor in order to parse the string representation.
    //. Unless it is NULL, {\tt aParent} should point to an object of a class
    //. derived from {\tt DwEntity}.

    virtual ~DwBody();

    const DwBody& operator = (const DwBody& aBody);
    //. This is the assignment operator, which follows regular semantics.

    virtual void Parse();
    //. This virtual function, inherited from {\tt DwMessageComponent},
    //. executes the parse method for {\tt DwBody} objects.
    //. It should be called immediately after the string representation
    //. is modified and before the parts of the broken-down
    //. representation are accessed.  Only {\it bodies} with content-type
    //. of multipart or message require parsing.  In either case, the
    //. {\tt DwBody} object must be able to find the message header.
    //. Therefore, if the {\tt DwBody} object is not the child of a
    //. {\tt DwEntity} ({\it i.e.}, {\tt DwMessage} or {\tt DwBodyPart})
    //. object, the {\it body} cannot be parsed because the content-type
    //. and multipart boundary cannot be determined.

    virtual void Assemble();
    //. This virtual function, inherited from {\tt DwMessageComponent},
    //. executes the assemble method for {\tt DwBody} objects.
    //. It should be called whenever one of the object's attributes
    //. is changed in order to assemble the string representation from
    //. its broken-down representation.  It will be called
    //. automatically for this object by the parent object's
    //. {\tt Assemble()} member function if the is-modified flag is set.
    //. Only {\it bodies} with content-type of
    //. multipart or message require assembling.  In either case, the
    //. {\tt DwBody} object must be able to find the message header.
    //. Therefore, if the {\tt DwBody} object is not the child of a
    //. {\tt DwEntity} ({\it i.e.}, {\tt DwMessage} or {\tt DwBodyPart})
    //. object, the {\it body} cannot be assembled because the content-type
    //. and/or multipart boundary cannot be determined.

    virtual DwMessageComponent* Clone() const;
    //. This virtual function, inherited from {\tt DwMessageComponent},
    //. creates a new {\tt DwBody} on the free store that has the same
    //. value as this {\tt DwBody} object.  The basic idea is that of
    //. a ``virtual copy constructor.''

    DwBodyPart* FirstBodyPart() const;
    //. For a multipart {\it body}, this member function returns the
    //. first {\it body part}, represented by a {\tt DwBodyPart} object.
    //. Use {\tt DwBodyPart::Next()} to iterate through the list of
    //. {\tt DwBodyPart}s.

    void AddBodyPart(DwBodyPart* aPart);
    //. For a multipart {\it body}, this member function appends a
    //. {\it body part}, represented by a {\tt DwBodyPart} object.
    //. The {\tt DwBody} object takes responsibility for deleting the
    //. appended {\tt DwBodyPart} object.

    DwMessage* Message() const;
    //. For a {\it body} with content-type of message, this member function
    //. returns the {\it message} encapsulated in the {\it body}.

    void AddMessage(DwMessage* aMessage);
    //. For a {\it body} with content-type of message, this member function
    //. sets the {\it message} encapsulated in the {\it body}.

    static DwBody* NewBody(const DwString& aStr, DwMessageComponent* aParent);
    //. Creates a new {\tt DwBody} object on the free store.
    //. If the static data member {\tt sNewBody} is NULL, 
    //. this member function will create a new {\tt DwBody}
    //. and return it.  Otherwise, {\tt NewBody()} will call
    //. the user-supplied function pointed to by {\tt sNewBody},
    //. which is assumed to return an object from a class derived from
    //. {\tt DwBody}, and return that object.

    //+ Var sNewBody
    static DwBody* (*sNewBody)(const DwString&, DwMessageComponent*);
    //. If {\tt sNewBody} is not NULL, it is assumed to point to a
    //. user-supplied function that returns an object from a class
    //. derived from {\tt DwBody}.

private:

    DwString    mBoundaryStr;
    //. A cache for the boundary string, which is obtained from the
    //. header associated with this body.

    DwString    mPreamble;
    //. Contains the preamble -- the text preceding the first boundary --
    //. in a multipart/* media type.

    DwString    mEpilogue;
    //. Contains the epilogue -- the text following the last boundary --
    //. in a multipart/* media type.

    DwBodyPart* mFirstBodyPart;
    //. Points to the first body part in a multipart/* media type.  Is NULL if
    //. there are no body parts.

    DwMessage*  mMessage;
    //. Points to the contained message, in a message/* media type.

    static const char* const sClassName;

    void _AddBodyPart(DwBodyPart*);
    //. Adds a body part to a multipart body.  This function is different
    //. from {\tt AddBodyPart} in that it does not set the is-modified
    //. flag.

    void _AddMessage(DwMessage*);
    //. Adds a message to a "message/*" body.  This function is different
    //. from {\tt AddMessage()} in that it does not set the is-modified
    //. flag.

    void DeleteBodyParts();
    void CopyBodyParts(const DwBodyPart* aFirst);

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

