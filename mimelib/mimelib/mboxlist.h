//=============================================================================
// File:       mboxlist.h
// Contents:   Declarations for DwMailboxList
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

#ifndef DW_MBOXLIST_H
#define DW_MBOXLIST_H

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
//+ Name DwMailboxList -- Class for containing DwMailbox objects
//+ Description
//. {\tt DwMailboxList} represents a list of {\it mailboxes} as described
//. in RFC-822.
//=============================================================================


class DW_EXPORT DwMailboxList : public DwFieldBody {

public:

    DwMailboxList();
    DwMailboxList(const DwMailboxList& aList);
    DwMailboxList(const DwString& aStr, DwMessageComponent* aParent=0);
    //. The first constructor is the default constructor, which sets the
    //. {\tt DwMailboxList} object's string representation to the empty string
    //. and sets its parent to NULL.
    //.
    //. The second constructor is the copy constructor, which copies the
    //. string representation and all {\tt DwMailbox}es from {\tt aList}.
    //. The parent of the new {\tt DwMailboxList} object is set to NULL.
    //.
    //. The third constructor copies {\tt aStr} to the {\tt DwMailboxList}
    //. object's string representation and sets {\tt aParent} as its parent.
    //. The virtual member function {\tt Parse()} should be called immediately
    //. after this constructor in order to parse the string representation.
    //. Unless it is NULL, {\tt aParent} should point to an object of a class
    //. derived from {\tt DwField}.

    virtual ~DwMailboxList();

    const DwMailboxList& operator = (const DwMailboxList& aList);
    //. This is the assignment operator, which follows regular semantics.

    virtual void Parse();
    //. This virtual function, inherited from {\tt DwMessageComponent},
    //. executes the parse method for {\tt DwMailboxList} objects.
    //. It should be called immediately after the string representation
    //. is modified and before the parts of the broken-down
    //. representation are accessed.

    virtual void Assemble();
    //. This virtual function, inherited from {\tt DwMessageComponent},
    //. executes the assemble method for {\tt DwMailboxList} objects.
    //. It should be called whenever one of the object's attributes
    //. is changed in order to assemble the string representation from
    //. its broken-down representation.  It will be called
    //. automatically for this object by the parent object's
    //. {\tt Assemble()} member function if the is-modified flag is set.

    virtual DwMessageComponent* Clone() const;
    //. This virtual function, inherited from {\tt DwMessageComponent},
    //. creates a new {\tt DwMailboxList} on the free store that has the same
    //. value as this {\tt DwMailboxList} object.  The basic idea is that of
    //. a ``virtual copy constructor.''

    DwMailbox* FirstMailbox() const;
    //. Gets the first {\tt DwMailbox} object in the list.  
    //. Use the member function {\tt DwMailbox::Next()} to iterate.  
    //. Returns NULL if the list is empty.

    void Add(DwMailbox* aMailbox);
    //. Adds {\tt aMailbox} to the end of the list of {\tt DwMailbox} objects
    //. maintained by this {\tt DwMailboxList} object.

    void Remove(DwMailbox* aMailbox);
    //. Removes {\tt aMailbox} from the list of {\tt DwMailbox} objects
    //. maintained by this {\tt DwMailboxList} object.  The {\tt DwMailbox}
    //. object is not freed by this member function.

    void DeleteAll();
    //. Removes and frees all {\tt DwMailbox} objects from the list
    //. maintained by this {\tt DwMailboxList} object.

    static DwMailboxList* NewMailboxList(const DwString& aStr,
        DwMessageComponent* aParent);
    //. Creates a new {\tt DwMailboxList} object on the free store.
    //. If the static data member {\tt sNewMailboxList} is NULL, 
    //. this member function will create a new {\tt DwMailboxList}
    //. and return it.  Otherwise, {\tt NewMailboxList()} will call
    //. the user-supplied function pointed to by {\tt sNewMailboxList},
    //. which is assumed to return an object from a class derived from
    //. {\tt DwMailboxList}, and return that object.

    //+ Var sNewMailboxList
    static DwMailboxList* (*sNewMailboxList)(const DwString&,
        DwMessageComponent*);
    //. If {\tt sNewMailboxList} is not NULL, it is assumed to point to a 
    //. user-supplied function that returns an object from a class derived from 
    //. {\tt DwMailboxList}.

protected:

    DwMailbox* mFirstMailbox;
    // points to first {\tt DwMailbox} object in list

    void _AddMailbox(DwMailbox* aMailbox);
    //. Adds a mailbox, but does not set the is-modified flag

    void _DeleteAll();
    //. Removes and deletes all {\tt DwMailbox} objects from the list
    //. maintained by this {\tt DwMailboxList} object.  Doesn't set the
    //. is-modified flag.

private:

    static const char* const sClassName;

    void CopyList(const DwMailbox* aFirst);

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


class DW_EXPORT DwMailboxListParser {
public:
    enum {
        eMbError,
        eMbGroup,
        eMbMailbox,
        eMbNull,
        eMbEnd
    };
    DwMailboxListParser(const DwString& aStr);
    virtual ~DwMailboxListParser();
    const DwString& MbString() { return mMbString.Tokens(); }
    int MbType() { return mMbType; }
    int IsNull() { return (mMbType == eMbNull) ? 1 : 0; }
    int IsEnd()  { return (mMbType == eMbEnd) ? 1 : 0; }
    int Restart();
    int operator ++ (); // prefix increment operator
protected:
    void ParseNextMailbox();
    DwRfc822Tokenizer mTokenizer;
    DwTokenString mMbString;
    int mMbType;
};

#endif
