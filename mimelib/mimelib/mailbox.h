//=============================================================================
// File:       mailbox.h
// Contents:   Declarations for DwMailbox
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

#ifndef DW_MAILBOX_H
#define DW_MAILBOX_H

#ifndef DW_CONFIG_H
#include <mimelib/config.h>
#endif

#ifndef DW_STRING_H
#include <mimelib/string.h>
#endif

#ifndef DW_ADDRESS_H
#include <mimelib/address.h>
#endif

class DwHeaderField;
class DwGroup;

//=============================================================================
//+ Name DwMailbox -- Class representing a MIME mailbox
//+ Description
//. {\tt DwMailbox} represents a {\it mailbox} as described in RFC-822.
//=============================================================================


class DW_EXPORT DwMailbox : public DwAddress {

    friend class DwMailboxList;

public:

    DwMailbox();
    DwMailbox(const DwMailbox& aMailbox);
    DwMailbox(const DwString& aStr, DwMessageComponent* aParent=0);
    //. The first constructor is the default constructor, which sets the
    //. {\tt DwMailbox} object's string representation to the empty string
    //. and sets its parent to NULL.
    //.
    //. The second constructor is the copy constructor, which copies the
    //. string representation and all attributes from {\tt aMailbox}.
    //. The parent of the new {\tt DwMailbox} object is set to NULL.
    //.
    //. The third constructor copies {\tt aStr} to the {\tt DwMailbox}
    //. object's string representation and sets {\tt aParent} as its parent.
    //. The virtual member function {\tt Parse()} should be called immediately
    //. after this constructor in order to parse the string representation.
    //. Unless it is NULL, {\tt aParent} should point to an object of a class
    //. derived from {\tt DwField}.

    virtual ~DwMailbox();

    const DwMailbox& operator = (const DwMailbox& aMailbox);
    //. This is the assignment operator, which follows regular semantics.

    virtual void Parse();
    //. This virtual function, inherited from {\tt DwMessageComponent},
    //. executes the parse method for {\tt DwMailbox} objects.
    //. The parse method parses the string representation of
    //. the {\tt DwMailbox} object into its broken-down representation.  This
    //. member function must be called after the string representation is
    //. set or modified, and before the {\it local-part}
    //. or {\it domain} is retrieved.

    virtual void Assemble();
    //. This virtual function, inherited from {\tt DwMessageComponent},
    //. executes the assemble method for {\tt DwMailbox} objects.
    //. The assemble method assembles the
    //. broken-down representation of the {\tt DwMailbox} object into its
    //. string representation.  This member function must be called after
    //. any component of the broken-down representation is set or modified
    //. and before the string representation is retrieved.

    virtual DwMessageComponent* Clone() const;
    //. This virtual function, inherited from {\tt DwMessageComponent},
    //. creates a new {\tt DwMailbox} on the free store that has the same
    //. value as this {\tt DwMailbox} object.  The basic idea is that of
    //. a ``virtual copy constructor.''

    const DwString& FullName() const;
    //. Returns the full name for this {\tt DwMailbox} object.

    void SetFullName(const DwString& aFullName);
    //. Sets the full name for this {\tt DwMailbox} object from
    //. {\tt aFullName}.

    const DwString& Route() const;
    //. Returns the {\it route} for this {\tt DwMailbox} object.

    void SetRoute(const DwString& aRoute);
    //. Sets the {\it route} for this {\tt DwMailbox} object.

    const DwString& LocalPart() const;
    //. Returns the {\it local-part} for this {\tt DwMailbox} object.

    void SetLocalPart(const DwString& aLocalPart);
    //. Sets the {\it local-part} for this {\tt DwMailbox} object.

    const DwString& Domain() const;
    //. Returns the {\it domain} for this {\tt DwMailbox} object.

    void SetDomain(const DwString& aDomain);
    //. Sets the {\it domain} for this {\tt DwMailbox} object.

    static DwMailbox* NewMailbox(const DwString& aStr, DwMessageComponent*
        aParent);
    //. Creates a new {\tt DwMailbox} object on the free store.
    //. If the static data member {\tt sNewMailbox} is NULL, 
    //. this member function will create a new {\tt DwMailbox}
    //. and return it.  Otherwise, {\tt NewMailbox()} will call
    //. the user-supplied function pointed to by {\tt sNewMailbox},
    //. which is assumed to return an object from a class derived from
    //. {\tt DwMailbox}, and return that object.

    //+ Var sNewMailbox
    static DwMailbox* (*sNewMailbox)(const DwString&, DwMessageComponent*);
    //. If {\tt sNewMailbox} is not NULL, it is assumed to point to a 
    //. user-supplied function that returns an object from a class derived
    //. from {\tt DwMailbox}.

private:

    DwString  mFullName;
    DwString  mRoute;
    DwString  mLocalPart;
    DwString  mDomain;
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
