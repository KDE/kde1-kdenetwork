//=============================================================================
// File:       entity.h
// Contents:   Declarations for DwEntity
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

#ifndef DW_ENTITY_H
#define DW_ENTITY_H

#ifndef DW_CONFIG_H
#include <mimelib/config.h>
#endif

#ifndef DW_STRING_H
#include <mimelib/string.h>
#endif

#ifndef DW_MSGCMP_H
#include <mimelib/msgcmp.h>
#endif

class DwHeader;
class DwBody;

//=============================================================================
//+ Name DwEntity -- Abstract class representing a MIME entity
//+ Description
//. RFC-1521 defines an {\it entity} as either a {\it message} or a 
//. {\it body-part}, both of which have a {\it header} and a {\it body}.  
//. In the MIME++ class hierarchy, an {\it entity} is represented by the
//. class {\tt DwEntity}.  A {\tt DwEntity} is characterized by the fact
//. that it contains both a {\tt DwHeader} and a {\tt DwBody}, just like
//. an {\it entity} has a {\it header} and a {\it body}.  These contained
//. objects are the children of a {\tt DwEntity} object.  (See the discussion
//. in the entry for {\tt DwMessageComponent}.)
//.
//. Since {\tt DwEntity} is an abstract base class, you may not
//. instantiate objects of type
//. {\tt DwEntity}.  Instead, you must instantiate objects of a class
//. derived from {\tt DwEntity}.  Just as RFC-1521 specifies that both 
//. {\it messages} and {\it body-parts} are {\it entities}, so also MIME++
//. provides classes {\tt DwMessage} and {\tt DwBodyPart} as subclasses
//. of {\tt DwEntity}.
//.
//. See also: {\tt DwMessageComponent}, {\tt DwMessage}, {\tt DwBodyPart}
//=============================================================================

class DW_EXPORT DwEntity : public DwMessageComponent {

public:

    DwEntity();
    DwEntity(const DwEntity& aEntity);
    DwEntity(const DwString& aStr, DwMessageComponent* aParent=0);
    //. The first constructor is the default constructor, which sets the
    //. {\tt DwEntity} object's string representation to the empty string
    //. and sets its parent to NULL.
    //.
    //. The second constructor is the copy constructor, which copies the
    //. string representation from {\tt aEntity} and all of its children.
    //. The parent of the new {\tt DwEntity} object is set to NULL.
    //.
    //. The third constructor copies {\tt aStr} to the {\tt DwEntity} 
    //. object's string representation and sets {\tt aParent} as its parent.
    //. The virtual member function {\tt Parse()} should be called immediately
    //. after this constructor in order to parse the string representation.

    virtual ~DwEntity();

    const DwEntity& operator = (const DwEntity& aEntity);
    //. This is the assignment operator, which follows regular semantics.

    virtual void Parse();
    //. This virtual function, inherited from {\tt DwMessageComponent},
    //. executes the parse method for {\tt DwEntity} objects.
    //. The parse method parses the string representation of
    //. the {\tt DwEntity} object into its broken-down representation, which
    //. consists of a {\it header} and a {\it body}.  The broken-down
    //. representation is implemented as contained {\tt DwHeader} and
    //. {\tt DwBody} objects, which are the children of a {\tt DwEntity}.
    //. (See the discussion in the entry for {\tt DwMessageComponent}.)
    //. After the {\it header} and {\it body} are parsed, this member function
    //. calls the {\tt Parse()} member function of the contained 
    //. {\tt DwHeader} and {\tt DwBody} objects; you do not have to explicitly
    //. parse the {\tt DwHeader} and {\tt DwBody} objects.
    //.
    //. This member function must be called after the string representation is
    //. set or modified, and before the {\tt header} or {\tt body] of the entity are
    //. accessed.

    virtual void Assemble();
    //. This virtual function, inherited from {\tt DwMessageComponent},
    //. executes the assemble method for {\tt DwEntity} objects.
    //. The assemble method assembles the
    //. broken-down representation of the {\tt DwEntity} object into its
    //. string representation.  The broken-down representation consists
    //. of a {\it header} and a {\it body}, and is implemented
    //. as contained {\tt DwHeader} and
    //. {\tt DwBody} objects, which are the children of a {\tt DwEntity}.
    //. (See the discussion in the entry for {\tt DwMessageComponent}.)
    //. Before the string representation is assembled, this member function
    //. will call the {\tt Assemble()} member function of the contained
    //. {\tt DwHeader} and {\tt DwBody} objects; you do not have to explicitly
    //. assemble the {\tt DwHeader} and {\tt DwBody} objects.
    //.
    //. This member function should be called after
    //. any component of the broken-down representation is set or changed
    //. and before the string representation is retrieved.

    DwHeader& Header() const;
    //. This function returns the {\tt DwHeader} contained by this object.

    DwBody& Body() const;
    //. This function returns the {\tt DwBody} contained by this object.

protected:

    DwHeader* mHeader;
    DwBody*   mBody;

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
