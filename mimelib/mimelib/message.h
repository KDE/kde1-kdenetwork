//=============================================================================
// File:       message.h
// Contents:   Declarations for DwMessage
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

#ifndef DW_MESSAGE_H
#define DW_MESSAGE_H

#ifndef DW_CONFIG_H
#include <mimelib/config.h>
#endif

#ifndef DW_ENTITY_H
#include <mimelib/entity.h>
#endif

//=============================================================================
//+ Name DwMessage -- Class representing a MIME message
//+ Description
//. {\tt DwMessage} represents an RFC-1521/RFC-822 {\it message}.  Just as a 
//. {\it message} in RFC-1521 is a type of {\it entity}, so in MIME++
//. {\tt DwMessage} is a subclass of {\tt DwEntity}.  Most of the functionality
//. of {\tt DwMessage} is implemented by the abstract class {\tt DwEntity}.
//.
//. See also: {\tt DwMessageComponent}, {\tt DwEntity}
//=============================================================================


class DW_EXPORT DwMessage : public DwEntity {

public:
    
    DwMessage();
    DwMessage(const DwMessage& aMessage);
    DwMessage(const DwString& aStr, DwMessageComponent* aParent=0);
    //. The first constructor is the default constructor, which sets the
    //. {\tt DwMessage} object's string representation to the empty string
    //. and sets its parent to NULL.
    //.
    //. The second constructor is the copy constructor, which copies the
    //. string representation from {\tt aMessage} and all of its children.
    //. The parent of the new {\tt DwMessage} object is set to NULL.
    //.
    //. The third constructor copies {\tt aStr} to the {\tt DwMessage}
    //. object's string representation and sets {\tt aParent} as its parent.
    //. The virtual member function {\tt Parse()} should be called immediately
    //. after this constructor in order to parse the string representation.

    virtual ~DwMessage();

    const DwMessage& operator = (const DwMessage& aMessage);
    //. This is the assignment operator, which follows regular semantics.

    virtual DwMessageComponent* Clone() const;
    //. This virtual function, inherited from {\tt DwMessageComponent},
    //. creates a new {\tt DwMessage} on the free store that has the same
    //. value as this {\tt DwMessage} object.  The basic idea is that of
    //. a ``virtual copy constructor.''

    static DwMessage* NewMessage(const DwString& aStr,
        DwMessageComponent* aParent);
    //. Creates a new {\tt DwMessage} object on the free store.
    //. If the static data member {\tt sNewMessage} is NULL, 
    //. this member function will create a new {\tt DwMessage}
    //. and return it.  Otherwise, {\tt NewMessage()} will call
    //. the user-supplied function pointed to by {\tt sNewMessage},
    //. which is assumed to return an object from a class derived from
    //. {\tt DwMessage}, and return that object.

    //+ Var sNewMessage
    static DwMessage* (*sNewMessage)(const DwString&, DwMessageComponent*);
    //. If {\tt sNewMessage} is not NULL, it is assumed to point to a user
    //. supplied function that returns an object from a class derived from 
    //. {\tt DwMessage}.

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

protected:

    void _PrintDebugInfo(ostream& aStrm) const;

};

#endif
