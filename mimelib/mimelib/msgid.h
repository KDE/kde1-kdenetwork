//=============================================================================
// File:       msgid.h
// Contents:   Declarations for DwMsgId
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

#ifndef DW_MSGID_H
#define DW_MSGID_H

#ifndef DW_CONFIG_H
#include <mimelib/config.h>
#endif

#ifndef DW_FIELDBDY_H
#include <mimelib/fieldbdy.h>
#endif

//=============================================================================
//+ Name DwMsgId -- Class representing a MIME msg-id
//+ Description
//. {\tt DwMsgId} represents a {\it msg-id} as described in RFC-822.
//=============================================================================

class DW_EXPORT DwMsgId : public DwFieldBody {

public:

    DwMsgId();
    DwMsgId(const DwMsgId& aMsgId);
    DwMsgId(const DwString& aStr, DwMessageComponent* aParent=0);
    //. The first constructor is the default constructor, which sets the
    //. {\tt DwMsgId} object's string representation to the empty string
    //. and sets its parent to NULL.
    //.
    //. The second constructor is the copy constructor, which copies the
    //. string representation and all attributes from {\tt aMsgId}.
    //. The parent of the new {\tt DwMsgId} object is set to NULL.
    //.
    //. The third constructor copies {\tt aStr} to the {\tt DwMsgId}
    //. object's string representation and sets {\tt aParent} as its parent.
    //. The virtual member function {\tt Parse()} should be called immediately
    //. after this constructor in order to parse the string representation.
    //. Unless it is NULL, {\tt aParent} should point to an object of a class
    //. derived from {\tt DwField}.

    virtual ~DwMsgId();

    const DwMsgId& operator = (const DwMsgId& aMsgId);
    //. This is the assignment operator, which follows regular semantics.

    virtual void Parse();
    //. This virtual function, inherited from {\tt DwMessageComponent},
    //. executes the parse method for {\tt DwMsgId} objects.
    //. It should be called immediately after the string representation
    //. is modified and before the parts of the broken-down
    //. representation are accessed.

    virtual void Assemble();
    //. This virtual function, inherited from {\tt DwMessageComponent},
    //. executes the assemble method for {\tt DwMsgId} objects.
    //. It should be called whenever one of the object's attributes
    //. is changed in order to assemble the string representation from
    //. its broken-down representation.  It will be called
    //. automatically for this object by the parent object's
    //. {\tt Assemble()} member function if the is-modified flag is set.

    virtual DwMessageComponent* Clone() const;
    //. This virtual function, inherited from {\tt DwMessageComponent},
    //. creates a new {\tt DwMsgId} on the free store that has the same
    //. value as this {\tt DwMsgId} object.  The basic idea is that of
    //. a ``virtual copy constructor.''

    virtual void CreateDefault();
    //. Creates a value for the {\it msg-id}.  Uses the current time,
    //. process id, and fully qualified domain name for the host.

    const DwString& LocalPart() const;
    //. Returns the {\it local-part} of the {\it msg-id} as described in
    //. RFC-822.

    void SetLocalPart(const DwString& aLocalPart);
    //. Sets the {\it local-part} of the {\it msg-id} as described in
    //. RFC-822.

    const DwString& Domain() const;
    //. Returns the {\it domain} of the {\it msg-id} as described in
    //. RFC-822.

    void SetDomain(const DwString& aDomain);
    //. Sets the {\it domain} of the {\it msg-id} as described in
    //. RFC-822.

    static DwMsgId* NewMsgId(const DwString& aStr,
        DwMessageComponent* aParent);
    //. Creates a new {\tt DwMsgId} object on the free store.
    //. If the static data member {\tt sNewMsgId} is NULL, 
    //. this member function will create a new {\tt DwMsgId}
    //. and return it.  Otherwise, {\tt NewMsgId()} will call
    //. the user-supplied function pointed to by {\tt sNewMsgId},
    //. which is assumed to return an object from a class derived from
    //. {\tt DwMsgId}, and return that object.

    //+ Var sNewMsgId
    static DwMsgId* (*sNewMsgId)(const DwString&, DwMessageComponent*);
    //. If {\tt sNewMsgId} is not NULL, it is assumed to point to a 
    //. user-supplied function that returns an object from a class derived from
    //. {\tt DwMsgId}.

    static const char* sHostName;
    //. Host name of machine, used to create msg-id string.  This data member
    //. is ignored if the platform supports a gethostname() function call.

private:

    DwString mLocalPart;
    DwString mDomain;
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
