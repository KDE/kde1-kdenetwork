//=============================================================================
// File:       field.h
// Contents:   Declarations for DwField
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

#ifndef DW_FIELD_H
#define DW_FIELD_H

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

class DwHeaders;
class DwFieldBody;

//=============================================================================
//+ Name DwField -- Class representing a MIME header field
//+ Description
//. {\tt DwField} represents a {\it field} as described in RFC-822.  A
//. {\it field} consists of a {\it field-name} and a {\it field-body}.  In
//. MIME++, a {\tt DwField} contains a {\tt DwString} that contains its
//. {\it field-name}, a {\tt DwString} that contains its {\it field-body}, and
//. a {\tt DwFieldBody} that contains a broken-down version of its 
//. {\it field-body}.
//. A {\tt DwField} object has only one child, which is the {\tt DwFieldBody}
//. object it contains.  The parent of a {\tt DwField} object is the 
//. {\tt DwHeaders} object that contains it.  (See the discussion in the entry
//. for {\tt DwMessageComponent}.)
//.
//. A {\tt DwField} object can be included in a list of {\tt DwField}
//. objects; usually this is the list of {\tt DwField} objects maintained
//. by its parent {\tt DwHeaders} object.
//=============================================================================


class DW_EXPORT DwField : public DwMessageComponent {

    friend class DwHeaders;

public:

    DwField();
    DwField(const DwField& aField);
    DwField(const DwString& aStr, DwMessageComponent* aParent=0);
    //. The first constructor is the default constructor, which sets the
    //. {\tt DwField} object's {\it field-name} string and 
    //. {\it field-body} string to the empty
    //. string, set its parent to NULL, and sets its {\tt DwFieldBody}
    //. object to NULL.
    //.
    //. The second constructor is the copy constructor, which
    //. copies the {\it field-name} string,
    //. {\it field-body} string, and the attached {\tt DwFieldBody} object
    //. from {\tt aField}.
    //.
    //. The third constructor copies {\tt aStr} to the object's string
    //. representation and sets {\tt aParent} as the object's parent.
    //. The virtual member function {\tt Parse()} should be called
    //. immediately after this constructor in order to parse the string
    //. representation.
    //. Unless it is NULL, {\tt aParent} should point to an object of a class
    //. derived from {\tt DwHeaders}.

    virtual ~DwField();

    const DwField& operator = (const DwField& aField);
    //. Assignment operator.  This operator copies the field name string,
    //. field body string, and the attached {\tt DwFieldBody} object
    //. from {\tt aField}.

    virtual void Parse();
    //. This virtual function, inherited from {\tt DwMessageComponent},
    //. executes the parse method for {\tt DwField} objects.
    //. The parse method parses the string representation of
    //. the {\tt DwField} object into its broken-down representation.  This
    //. member function must be called after the string representation is
    //. set or modified, and before the {\it field-name}
    //. or {\it field-body} is retrieved.

    virtual void Assemble();
    //. This virtual function, inherited from {\tt DwMessageComponent},
    //. executes the assemble method for {\tt DwField} objects.
    //. The assemble method assembles the
    //. broken-down representation of the {\tt DwField} object into its
    //. string representation.  This member function must be called after
    //. any component of the broken-down representation is set or modified
    //. and before the string representation is retrieved.

    virtual DwMessageComponent* Clone() const;
    //. This virtual function, inherited from {\tt DwMessageComponent},
    //. creates a new {\tt DwField} on the free store that has the same
    //. value as this {\tt DwField} object.  The basic idea is that of
    //. a ``virtual copy constructor.''

    DwFieldBody* FieldBody() const;
    //. Returns the {\tt DwFieldBody} object contained by this {\tt DwField}
    //. object.  If there is no field body, a NULL will be returned.

    const DwString& FieldNameStr() const;
    //. Returns the {\it field-name} of this {\it field} as a string.

    const DwString& FieldBodyStr() const;
    //. Returns the {\it field-body} of this {\it field} as a string.

    DwField* Next() const;
    //. Returns the next {\tt DwField} object following this
    //. {\tt DwField} object in the list contained in a {\tt DwHeaders}.
    //. Returns NULL if this object is last in the list.

    void SetFieldBody(DwFieldBody* aFieldBody);
    //. Sets the {\tt DwFieldBody} object contained by this object.

    void SetFieldNameStr(const DwString& aStr);
    //. Sets the {\it field-name} of this field.

    void SetFieldBodyStr(const DwString& aStr);
    //. Sets the {\it field-body} of this field.

    void SetNext(const DwField* aField);
    //. This {\it advanced} function sets {\tt aField} as the next 
    //. {\it field} following this {\it field} in the list of
    //. {\it field}s contained in the {\it headers}. Since {\tt DwHeaders}
    //. contains member functions for adding {\tt DwField} objects 
    //. to its list, this function is not useful for most applications.

    static DwField* NewField(const DwString& aStr,
        DwMessageComponent* aParent);
    //. Creates a new {\tt DwField} object on the free store.
    //. If the static data member {\tt sNewField} is NULL, 
    //. this member function will create a new {\tt DwField}
    //. and return it.  Otherwise, {\tt NewField()} will call
    //. the user-supplied function pointed to by {\tt sNewField},
    //. which is assumed to return an object from a class derived from
    //. {\tt DwField}, and return that object.

    static DwFieldBody* CreateFieldBody(const DwString& aFieldName,
        const DwString& aFieldBody, DwMessageComponent* aParent);
    //. The static member function {\tt CreateFieldBody()}
    //. is called from the {\tt Parse()}
    //. member function and is responsible for creating a
    //. {\tt DwFieldBody} object for this particular field.  A typical
    //. scenario might go as follows:
    //. This member function examines the {\it field-name} for this field,
    //. finds that it contains "To", creates a {\tt DwAddressList} object
    //. to contain the {\it field-body}, calls the {\tt Parse()} member
    //. function for the {\tt DwAddressList}, and sets the {\tt DwAddressList}
    //. object as this {\tt DwField} object's {\tt DwFieldBody}.
    //.
    //. If you want to override the behavior of {\tt CreateFieldBody()},
    //. you can do so by setting the public data member
    //. {\tt sCreateFieldBody} to point to your own function.
    //. {\tt CreateFieldBody()} first checks to see if
    //. {\tt sCreateFieldBody} is NULL.  If it is not,
    //. {\tt CreateFieldBody()} will assume that it points to a user-supplied
    //. function and will call that function.  If it is NULL,
    //. {\tt CreateFieldBody()} will call {\tt _CreateFieldBody()}, which
    //. actually creates the {\tt DwFieldBody} object.  You may call
    //. {\tt _CreateFieldBody()} from your own function for fields you
    //. do not wish to handle.

    static DwFieldBody* _CreateFieldBody(const DwString& aFieldName,
        const DwString& aFieldBody, DwMessageComponent* aParent);

    //+ Var sNewField
    static DwField* (*sNewField)(const DwString&, DwMessageComponent*);
    //. If {\tt sNewField} is not NULL, it is assumed to point to a
    //. user-supplied function that returns an object from a class derived from
    //. {\tt DwField}.

    //+ Var sCreateFieldBody
    static DwFieldBody* (*sCreateFieldBody)(const DwString& aFieldName,
        const DwString& aFieldBody, DwMessageComponent* aParent);
    //. See CreateFieldBody().

protected:

    DwString mFieldNameStr;
    // the {\it field-name}

    DwString mFieldBodyStr;
    // the {\it field-body}

    DwFieldBody* mFieldBody;
    // pointer to the {\tt DwFieldBody} object

    void _SetFieldBody(DwFieldBody* aFieldBody);
    //. This function does not set the is-modified flag

private:

    const DwField* mNext;
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
