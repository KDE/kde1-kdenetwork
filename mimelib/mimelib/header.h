//=============================================================================
// File:       header.h
// Contents:   Declarations for DwHeader
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

#ifndef DW_HEADER_H
#define DW_HEADER_H

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

#ifndef DW_ENTITY_H
#include <mimelib/entity.h>
#endif

#ifndef DW_MSGID_H
#include <mimelib/msgid.h>
#endif

#ifndef DW_MAILBOX_H
#include <mimelib/mailbox.h>
#endif

#ifndef DW_MEDIATYP_H
#include <mimelib/mediatyp.h>
#endif

#ifndef DW_DATETIME_H
#include <mimelib/datetime.h>
#endif

#ifndef DW_MECHANSM_H
#include <mimelib/mechansm.h>
#endif

#ifndef DW_DISPTYPE_H
#include <mimelib/disptype.h>
#endif

class DwMessage;
class DwBodyPart;
class DwField;
class DwFieldBody;
class DwDateTime;
class DwMailboxList;
class DwAddressList;
class DwMediaType;
class DwMechanism;
class DwText;

//=============================================================================
//+ Name DwHeader -- Class representing a MIME header
//+ Description
//. {\tt DwHeader} represents the {\it header} of a {\it message}, or 
//. a {\it body part} in a multipart {\it message}, as described in RFC-822
//. and RFC-1521.  A {\it header} consists of a series of {\it fields}.
//. In MIME++, a {\tt DwHeader} object contains a list of {\tt DwField}
//. objects.  These contained {\tt DwField} objects are the children of
//. a {\tt DwHeader} object.  The parent of a {\tt DwHeader} object is
//. the {\tt DwEntity} that contains it.  (See the discussion or children
//. and parents in the entry for {\tt DwMessageComponent}.)
//. 
//. {\tt DwHeader} provides a member function {\tt Parse()} for parsing it;
//. that is, creating the list of {\tt DwField} objects corresponding to the
//. {\it fields} it contains.  It also provides a member function
//. {\tt Assemble()} for combining its {\it fields} to create its string
//. representation.
//.
//. You can access the {\it field-body} for a specific well-known {\it field}
//. by using the member function {\tt <Field>()}, where {\tt <Field>} is
//. the {\it field-name} of the {\it field} with hyphens removed and the
//. first word following a hyphen capitalized.  For example, to access the
//. {\it field-body} for the ``MIME-version'' {\it field}, use {\tt MimeVersion()}.
//. The member function {\tt <Field>()} will create a field with
//. {\it field-name} {\tt <Field>} if such a {\it field} does not exist.
//. You can check for the existence of a specific well-known {\it field} by
//. using the member function {\tt Has<Field>()}.  For example, to check
//. for the existence of the MIME-version {\it field}, use 
//. {\tt HasMimeVersion()}.  Well-known {\it field}s are those documented in
//. RFC-822, RFC-1036, RFC-1521, and possibly other RFCs.
//.
//. In the case of an {\it extension-field} or {\it user-defined-field}, 
//. you can access the {\it field-body} of the {\it field} by calling the
//. member function {\tt FieldBody()} with the {\it field-name} as an
//. argument.  If the {\it extension-field} or {\it user-defined-field}
//. does not exist, {\tt FieldBody()} will create it.  You check for the
//. existence of an {\it extension-field} or {\it user-defined-field} by
//. using the member function {\tt HasField()} with the {\it field-name}
//. as its argument.
//.
//. You can iterate over all {\it fields} in the {\it header} by using
//. the member function {\tt FirstField()} to get the first {\tt DwField}
//. object in the list, and then using {\tt DwField::Next()} to iterate
//. through the list.
//=============================================================================


class DW_EXPORT DwHeader : public DwMessageComponent {

public:

    DwHeader();
    DwHeader(const DwHeader& aHeader);
    DwHeader(const DwString& aStr, DwMessageComponent* aParent=0);
    //. The first constructor is the default constructor, which sets the
    //. {\tt DwHeader} object's string representation to the empty string
    //. and sets its parent to NULL.
    //.
    //. The second constructor is the copy constructor, which copies the
    //. string representation from {\tt aHeader} and all of its children.
    //. The parent of the new {\tt DwHeader} object is set to NULL.
    //.
    //. The third constructor copies {\tt aStr} to the {\tt DwHeader}
    //. object's string representation and sets {\tt aParent} as its parent.
    //. The virtual member function {\tt Parse()} should be called immediately
    //. after this constructor in order to parse the string representation.
    //. Unless it is NULL, {\tt aParent} should point to an object of a class
    //. derived from {\tt DwEntity}.

    virtual ~DwHeader();

    const DwHeader& operator = (const DwHeader& aHeader);
    //. This is the assignment operator, which follows regular semantics.

    virtual void Parse();
    //. This virtual function, inherited from {\tt DwMessageComponent},
    //. executes the parse method for {\tt DwHeader} objects.
    //. The parse method parses the string representation of
    //. the {\tt DwHeader} object into its broken-down representation,
    //. which consists of a collection of {\it fields}.  The broken-down
    //. representation is implemented as a list of {\tt DwField} objects,
    //. which are the children of a {\tt DwHeader} object.
    //. (See the discussion in the entry for {\tt DwMessageComponent}.)
    //. 
    //. This member function must be called after the string representation
    //. is set or modified, and before the fields of the header are accessed.

    virtual void Assemble();
    //. This virtual function, inherited from {\tt DwMessageComponent},
    //. executes the assemble method for {\tt DwHeader} objects.
    //. The assemble method assembles the
    //. broken-down representation of the {\tt DwHeader} object into its
    //. string representation.  The broken-down representation consists of
    //. a collection of {\it fields} implemented as a list of
    //. {\tt DwField} objects.  Before the string representation is assembled,
    //. this member function will call the {\tt Assemble()} member function of
    //. each {\tt DwField} object in its list; you do not have to explicitly
    //. assemble each {\tt DwField} object.
    //.
    //. This member function should be called after any component of the
    //. broken-down representation is set or modified and before the string
    //. representation is accessed.

    virtual DwMessageComponent* Clone() const;
    //. This virtual function, inherited from {\tt DwMessageComponent},
    //. creates a new {\tt DwHeader} on the free store that has the same
    //. value as this {\tt DwHeader} object.  The basic idea is that of
    //. a ``virtual copy constructor.''

    DwBool HasBcc() const;
    DwBool HasCc() const;
    DwBool HasComments() const;
    DwBool HasDate() const;
    DwBool HasEncrypted() const;
    DwBool HasFrom() const;
    DwBool HasInReplyTo() const;
    DwBool HasKeywords() const;
    DwBool HasMessageId() const;
    DwBool HasReceived() const;
    DwBool HasReferences() const;
    DwBool HasReplyTo() const;
    DwBool HasResentBcc() const;
    DwBool HasResentCc() const;
    DwBool HasResentDate() const;
    DwBool HasResentFrom() const;
    DwBool HasResentMessageId() const;
    DwBool HasResentReplyTo() const;
    DwBool HasResentSender() const;
    DwBool HasResentTo() const;
    DwBool HasReturnPath() const;
    DwBool HasSender() const;
    DwBool HasSubject() const;
    DwBool HasTo() const;
    // RFC-822 fields
    //
    DwBool HasApproved() const;
    DwBool HasControl() const;
    DwBool HasDistribution() const;
    DwBool HasExpires() const;
    DwBool HasFollowupTo() const;
    DwBool HasLines() const;
    DwBool HasNewsgroups() const;
    DwBool HasOrganization() const;
    DwBool HasPath() const;
    DwBool HasSummary() const;
    DwBool HasXref() const;
    // RFC-1036 fields
    //
    DwBool HasContentDescription() const;
    DwBool HasContentId() const;
    DwBool HasContentTransferEncoding() const;
    DwBool HasCte() const;
    DwBool HasContentType() const;
    DwBool HasMimeVersion() const;
    // RFC-1521 fields
    //
    DwBool HasContentDisposition() const;
    // RFC-1806
    //
    //. This group of member functions return a boolean value indicating
    //. whether a particular well-known {\it field} is present in the
    //. {\it header}.

    DwBool HasField(const char* aFieldName) const;
    DwBool HasField(const DwString& aFieldName) const;
    //. Returns true if the {\it field} specified by {\tt aFieldName} is
    //. present in the {\it header}.  These member function are used for
    //. {\it extension-fields} or {\it user-defined-fields}.

    DwAddressList&  Bcc();
    DwAddressList&  Cc();
    DwText&         Comments();
    DwDateTime&     Date();
    DwText&         Encrypted();
    DwMailboxList&  From();
    DwText&         InReplyTo();
    DwText&         Keywords();
    DwMsgId&        MessageId();
    DwText&         Received();
    DwText&         References();
    DwAddressList&  ReplyTo();
    DwAddressList&  ResentBcc();
    DwAddressList&  ResentCc();
    DwDateTime&     ResentDate();
    DwMailboxList&  ResentFrom();
    DwMsgId&        ResentMessageId();
    DwAddressList&  ResentReplyTo();
    DwMailbox&      ResentSender();
    DwAddressList&  ResentTo();
    DwAddress&      ReturnPath();
    DwMailbox&      Sender();
    DwText&         Subject();
    DwAddressList&  To();
    // RFC-822 fields
    //
    DwText& Approved();
    DwText& Control();
    DwText& Distribution();
    DwText& Expires();
    DwText& FollowupTo();
    DwText& Lines();
    DwText& Newsgroups();
    DwText& Organization();
    DwText& Path();
    DwText& Summary();
    DwText& Xref();
    // RFC-1036 fields (USENET messages)
    //
    DwText&         ContentDescription();
    DwMsgId&        ContentId();
    DwMechanism&    ContentTransferEncoding();
    DwMechanism&    Cte();
    DwMediaType&    ContentType();
    DwText&         MimeVersion();
    // RFC-1521 fields
    //
    DwDispositionType& ContentDisposition();
    // RFC-1806 Content-Disposition field
    //
    //. Returns a reference to the {\tt DwFieldBody} object that is associated
    //. with a particular {\it field}.  If the {\it field} is not already
    //. present in the {\it header}, it is created.  Use the corresponding
    //. {\tt Has<Field>()} function to test if the {\it field} already
    //. exists without creating it.

    DwFieldBody& FieldBody(const DwString& aFieldName);
    //. Returns a reference to the {\tt DwFieldBody} object that is associated
    //. with a particular {\it field}.  If the {\it field} is not already
    //. present in the {\it header}, it is created.  Use {\tt HasField()}
    //. to test if the {\it field} already exists without creating it.
    //. This member function allows access to {\it extension-field}s or
    //. {\it user-defined-field}s.

    int NumFields() const;
    //. Returns the number of {\it fields} contained by this {\it header}.

    DwField* FirstField() const;
    //. Return a pointer to the first {\tt DwField} object contained by this
    //. {\tt DwHeader} object.  Use this member function to begin an
    //. iteration over the entire list of {tt DwField} objects.  
    //. Continue the iteration by calling {\tt DwField::Next()} on the returned
    // {\tt DwField} object.

    DwField* FindField(const char* aFieldName) const;
    DwField* FindField(const DwString& aFieldName) const;
    //. Searches for a {\it field} by its {\it field-name}.  Returns NULL
    //. if the field is not found.  This is an {\it advanced} function:
    //. most applications should use the {\tt Find<Field>()} family of
    //. functions, or the functions {\tt HasField()}.

    void AddOrReplaceField(DwField* aField);
    //. Adds a {\it field}, implemented as a {\tt DwField} object, to this
    //. {\it header}.  If a {\it field} with the same {\it field-name}
    //. is already present in the {\it header}, it is replaced by the new
    //. {\it field}.
    //.
    //. {\tt DwHeader} takes responsibility for deleting the added
    //. {\tt DwField} object.
    //.
    //. This is an advanced function.  Consider using the member functions
    //. {\tt <Field>()} (e.g. {\tt To()}, {\tt ContentType()}, and so on)
    //. and {\tt FieldBody()} for adding {\it field}s to the {\it header}.

    void AddField(DwField* aField);
    //. Add a {\it field}, implemented as a {\tt DwField} object, to this
    //. {\it header}.  If a {\it field} with the same {\it field-name}
    //. is already present in the {\it header}, it is {\it not} replaced;
    //. thus, duplicate {\it fields} may occur when using this member
    //. function.
    //.
    //. {\tt DwHeader} takes responsibility for deleting the added
    //. {\tt DwField} object.
    //.
    //. This is an advanced function.  Consider using the member functions
    //. {\tt <Field>()} (e.g. {\tt To()}, {\tt ContentType()}, and so on)
    //. and {\tt FieldBody()} for adding {\it field}s to the {\it header}.

    void AddFieldAt(int aPos, DwField* aField);
    //. This member functions follows the semantics of {\tt AddField()}
    //. except that {\tt aPos} specifies a position for adding the {\it field}.
    //. A position of 1 indicates the beginning of the list.  A position of 0
    //. indicates the end of the list.
    //.
    //. This is an advanced function.  Consider using the member functions
    //. {\tt <Field>()} (e.g. {\tt To()}, {\tt ContentType()}, and so on)
    //. and {\tt FieldBody()} for adding {\it field}s to the {\it header}.

    void RemoveField(DwField* aField);
    //. Remove the {\tt DwField} object from the list.  The {\tt DwField}
    //. object is not deleted.

    void DeleteAllFields();
    //. Remove all {\tt DwField} objects from the list and {\tt delete} them.

    static DwHeader* NewHeader(const DwString& aStr,
        DwMessageComponent* aParent);
    //. Creates a new {\tt DwHeader} object on the free store.
    //. If the static data member {\tt sNewHeader} is NULL, 
    //. this member function will create a new {\tt DwHeader}
    //. and return it.  Otherwise, {\tt NewHeader()} will call
    //. the user-supplied function pointed to by {\tt sNewHeader},
    //. which is assumed to return an object from a class derived from
    //. {\tt DwHeader}, and return that object.

    //+ Var sNewHeader
    static DwHeader* (*sNewHeader)(const DwString&, DwMessageComponent*);
    //. If {\tt sNewHeader} is not NULL, it is assumed to point to a 
    //. user-supplied function that returns an object from a class derived from 
    //. {\tt DwHeader}.

protected:

    void _AddField(DwField* aField);
    //. Add field but don't set the is-modified flag

    DwField* mFirstField;

protected:

    static const char* const sClassName;

    void CopyFields(DwField* aFirst);

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

private:

    void _PrintDebugInfo(ostream& aStrm) const;

};


inline DwField* DwHeader::FirstField() const
{
    return mFirstField; 
}

#ifdef DW_USE_INLINE_FUNCTIONS
#define DW_INLINE inline
#include "header.i"
#endif

#endif

