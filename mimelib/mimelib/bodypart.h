//=============================================================================
// File:       bodypart.h
// Contents:   Declarations for DwBodyPart
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

#ifndef DW_BODYPART_H
#define DW_BODYPART_H

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
class DwBody;


//=============================================================================
//+ Name DwBodyPart -- Class representing MIME message body-part
//+ Description
//. {\tt DwBodyPart} represents a {\it body part}, as described in RFC-1521.
//. Just as a {\it body part} in RFC-1521 is a type of {\it entity}, so
//. in MIME++ {\tt DwBodyPart} is a subclass of {\tt DwEntity}.  While a
//. {\it message} is also an {\it entity}, a {\it body part} is different
//. from a {\it message} in that it appears only in the {\it body} of a
//. multipart {\it entity}.  MIME++ generally enforces this rule.  The
//. primary difference, though, between a {\tt DwBodyPart} and a
//. {\tt DwMessage} is that a {\tt DwBodyPart} has the necessary
//. attributes that permit its inclusion in a list.
//.
//. As with {\tt DwMessage}, most of the functionality of {\tt DwBodyPart} is
//. implemented by the abstract class {\tt DwEntity}.
//=============================================================================

class DW_EXPORT DwBodyPart : public DwEntity {

public:

    DwBodyPart();
    DwBodyPart(const DwBodyPart& aPart);
    DwBodyPart(const DwString& aStr, DwMessageComponent* aParent=0);
    //. The first constructor is the default constructor, which sets the
    //. {\tt DwBodyPart} object's string representation to the empty string
    //. and sets its parent to NULL.
    //.
    //. The second constructor is the copy constructor, which copies the
    //. string representation from {\tt aPart} and all of its children.
    //. The parent of the new {\tt DwBodyPart} object is set to NULL.
    //.
    //. The third constructor copies {\tt aStr} to the {\tt DwBodyPart} 
    //. object's string representation and sets {\tt aParent} as its parent.
    //. The virtual member function {\tt Parse()} should be called immediately
    //. after this constructor in order to parse the string representation.

    virtual ~DwBodyPart();

    const DwBodyPart& operator = (const DwBodyPart& aPart);
    //. This is the assignment operator, which follows regular semantics.

    virtual DwMessageComponent* Clone() const;
    //. This virtual function, inherited from {\tt DwMessageComponent},
    //. creates a new {\tt DwBodyPart} on the free store that has the same
    //. value as this {\tt DwBodyPart} object.  The basic idea is that of
    //. a ``virtual copy constructor.''

    static DwBodyPart* NewBodyPart(const DwString& aStr,
        DwMessageComponent* aParent);
    //. Create a new {\tt DwBodyPart} on the free store.
    //. If the static data member {\tt sNewBodyPart} is NULL, 
    //. this member function will create a new {\tt DwBodyPart}
    //. and return it.  Otherwise, {\tt NewBodyPart()} will call
    //. the user-supplied function pointed to by {\tt sNewBodyPart},
    //. which is assumed to return an object from a class derived from
    //. {\tt DwBodyPart}, and return that object.

    DwBodyPart* Next() const;
    //. This member function returns the next {\it body part} following
    //. this {\it body part} in the list of {\it body parts} contained
    //. in a multipart {\it body}.

    void SetNext(const DwBodyPart* aPart);
    //. This advanced function sets {\tt aPart} as the next {\it body part}
    //. following this {\it body part} in the list of {\it body parts}
    //. contained in a multipart {\it body}.  Since {\tt DwBody} contains
    //. a member function for adding a {\tt DwBodyPart} object to its
    //. list, this function is not useful for most applications.

    //+ Var sNewBodyPart
    static DwBodyPart* (*sNewBodyPart)(const DwString&, DwMessageComponent*);
    //. If {\tt sNewBodyPart} is not NULL, it is assumed to point to a
    //. user-supplied function that returns an object from a class
    //. derived from {\tt DwBodyPart}.

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

private:

    const DwBodyPart* mNext;
    static const char* const sClassName;

};

#endif

