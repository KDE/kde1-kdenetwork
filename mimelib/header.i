//=============================================================================
// File:       header.i
// Contents:   Inline functions for DwHeader
// Maintainer: Doug Sauder <dwsauder@fwb.gulf.net>
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


DW_INLINE DwBool DwHeader::HasBcc() const
{
    return FindField("bcc") ? 1 : 0;
}


DW_INLINE DwBool DwHeader::HasCc() const
{
    return FindField("cc") ? 1 : 0;
}


DW_INLINE DwBool DwHeader::HasComments() const
{
    return FindField("comments") ? 1 : 0;
}


DW_INLINE DwBool DwHeader::HasDate() const
{
    return FindField("date") ? 1 : 0;
}


DW_INLINE DwBool DwHeader::HasEncrypted() const
{
    return FindField("encrypted") ? 1 : 0;
}


DW_INLINE DwBool DwHeader::HasFrom() const
{
    return FindField("from") ? 1 : 0;
}


DW_INLINE DwBool DwHeader::HasInReplyTo() const
{
    return FindField("in-reply-to") ? 1 : 0;
}


DW_INLINE DwBool DwHeader::HasKeywords() const
{
    return FindField("keywords") ? 1 : 0;
}


DW_INLINE DwBool DwHeader::HasMessageId() const
{
    return FindField("message-id") ? 1 : 0;
}


DW_INLINE DwBool DwHeader::HasReceived() const
{
    return FindField("received") ? 1 : 0;
}


DW_INLINE DwBool DwHeader::HasReferences() const
{
    return FindField("references") ? 1 : 0;
}


DW_INLINE DwBool DwHeader::HasReplyTo() const
{
    return FindField("reply-to") ? 1 : 0;
}


DW_INLINE DwBool DwHeader::HasResentBcc() const
{
    return FindField("resent-bcc") ? 1 : 0;
}


DW_INLINE DwBool DwHeader::HasResentCc() const
{
    return FindField("resent-cc") ? 1 : 0;
}


DW_INLINE DwBool DwHeader::HasResentDate() const
{
    return FindField("resent-date") ? 1 : 0;
}


DW_INLINE DwBool DwHeader::HasResentFrom() const
{
    return FindField("resent-from") ? 1 : 0;
}


DW_INLINE DwBool DwHeader::HasResentMessageId() const
{
    return FindField("resent-message-id") ? 1 : 0;
}


DW_INLINE DwBool DwHeader::HasResentReplyTo() const
{
    return FindField("resent-reply-to") ? 1 : 0;
}


DW_INLINE DwBool DwHeader::HasResentSender() const
{
    return FindField("resent-sender") ? 1 : 0;
}


DW_INLINE DwBool DwHeader::HasResentTo() const
{
    return FindField("resent-to") ? 1 : 0;
}


DW_INLINE DwBool DwHeader::HasReturnPath() const
{
    return FindField("return-path") ? 1 : 0;
}


DW_INLINE DwBool DwHeader::HasSender() const
{
    return FindField("sender") ? 1 : 0;
}


DW_INLINE DwBool DwHeader::HasSubject() const
{
    return FindField("subject") ? 1 : 0;
}


DW_INLINE DwBool DwHeader::HasTo() const
{
    return FindField("to") ? 1 : 0;
}


DW_INLINE DwBool DwHeader::HasApproved() const
{
    return FindField("approved") ? 1 : 0;
}


DW_INLINE DwBool DwHeader::HasControl() const
{
    return FindField("control") ? 1 : 0;
}


DW_INLINE DwBool DwHeader::HasDistribution() const
{
    return FindField("distribution") ? 1 : 0;
}


DW_INLINE DwBool DwHeader::HasExpires() const
{
    return FindField("expires") ? 1 : 0;
}


DW_INLINE DwBool DwHeader::HasFollowupTo() const
{
    return FindField("followup-to") ? 1 : 0;
}


DW_INLINE DwBool DwHeader::HasLines() const
{
    return FindField("lines") ? 1 : 0;
}


DW_INLINE DwBool DwHeader::HasNewsgroups() const
{
    return FindField("newsgroups") ? 1 : 0;
}


DW_INLINE DwBool DwHeader::HasOrganization() const
{
    return FindField("organization") ? 1 : 0;
}


DW_INLINE DwBool DwHeader::HasPath() const
{
    return FindField("path") ? 1 : 0;
}


DW_INLINE DwBool DwHeader::HasSummary() const
{
    return FindField("summary") ? 1 : 0;
}


DW_INLINE DwBool DwHeader::HasXref() const
{
    return FindField("xref") ? 1 : 0;
}


DW_INLINE DwBool DwHeader::HasContentDescription() const
{
    return FindField("content-description") ? 1 : 0;
}


DW_INLINE DwBool DwHeader::HasContentId() const
{
    return FindField("content-id") ? 1 : 0;
}


DW_INLINE DwBool DwHeader::HasContentTransferEncoding() const
{
    return FindField("content-transfer-encoding") ? 1 : 0;
}


DW_INLINE DwBool DwHeader::HasCte() const
{
    return FindField("content-transfer-encoding") ? 1 : 0;
}


DW_INLINE DwBool DwHeader::HasContentType() const
{
    return FindField("content-type") ? 1 : 0;
}


DW_INLINE DwBool DwHeader::HasMimeVersion() const
{
    return FindField("mime-version") ? 1 : 0;
}


DW_INLINE DwBool DwHeader::HasContentDisposition() const
{
    return FindField("content-disposition") ? 1 : 0;
}


DW_INLINE DwBool DwHeader::HasField(const char* aFieldName) const
{
    return FindField(aFieldName) ? 1 : 0;
}


DW_INLINE DwBool DwHeader::HasField(const DwString& aFieldName) const
{
    return FindField(aFieldName) ? 1 : 0;
}


DW_INLINE DwAddressList& DwHeader::Bcc()
{
    return (DwAddressList&) FieldBody("Bcc");
}


DW_INLINE DwAddressList& DwHeader::Cc()
{
    return (DwAddressList&) FieldBody("Cc");
}


DW_INLINE DwText& DwHeader::Comments()
{
    return (DwText&) FieldBody("Comments");
}


DW_INLINE DwDateTime& DwHeader::Date()
{
    return (DwDateTime&) FieldBody("Date");
}


DW_INLINE DwText& DwHeader::Encrypted()
{
    return (DwText&) FieldBody("Encrypted");
}


DW_INLINE DwMailboxList& DwHeader::From()
{
    return (DwMailboxList&) FieldBody("From");
}


DW_INLINE DwText& DwHeader::InReplyTo()
{
    return (DwText&) FieldBody("In-Reply-To");
}


DW_INLINE DwText& DwHeader::Keywords()
{
    return (DwText&) FieldBody("Keywords");
}


DW_INLINE DwMsgId& DwHeader::MessageId()
{
    return (DwMsgId&) FieldBody("Message-Id");
}


DW_INLINE DwText& DwHeader::Received()
{
    return (DwText&) FieldBody("Received");
}


DW_INLINE DwText& DwHeader::References()
{
    return (DwText&) FieldBody("References");
}


DW_INLINE DwAddressList& DwHeader::ReplyTo()
{
    return (DwAddressList&) FieldBody("Reply-To");
}


DW_INLINE DwAddressList& DwHeader::ResentBcc()
{
    return (DwAddressList&) FieldBody("Resent-Bcc");
}


DW_INLINE DwAddressList& DwHeader::ResentCc()
{
    return (DwAddressList&) FieldBody("Resent-Cc");
}


DW_INLINE DwDateTime& DwHeader::ResentDate()
{
    return (DwDateTime&) FieldBody("Resent-Date");
}


DW_INLINE DwMailboxList& DwHeader::ResentFrom()
{
    return (DwMailboxList&) FieldBody("Resent-From");
}


DW_INLINE DwMsgId& DwHeader::ResentMessageId()
{
    return (DwMsgId&) FieldBody("Resent-Message-Id");
}


DW_INLINE DwAddressList& DwHeader::ResentReplyTo()
{
    return (DwAddressList&) FieldBody("Resent-Reply-To");
}


DW_INLINE DwMailbox& DwHeader::ResentSender()
{
    return (DwMailbox&) FieldBody("Resent-Sender");
}


DW_INLINE DwAddressList& DwHeader::ResentTo()
{
    return (DwAddressList&) FieldBody("Resent-To");
}


DW_INLINE DwAddress& DwHeader::ReturnPath()
{
    return (DwAddress&) FieldBody("Return-Path");
}


DW_INLINE DwMailbox& DwHeader::Sender()
{
    return (DwMailbox&) FieldBody("Sender");
}


DW_INLINE DwText& DwHeader::Subject()
{
    return (DwText&) FieldBody("Subject");
}


DW_INLINE DwAddressList& DwHeader::To()
{
    return (DwAddressList&) FieldBody("To");
}


DW_INLINE DwText& DwHeader::Approved()
{
    return (DwText&) FieldBody("Approved");
}


DW_INLINE DwText& DwHeader::Control()
{
    return (DwText&) FieldBody("Control");
}


DW_INLINE DwText& DwHeader::Distribution()
{
    return (DwText&) FieldBody("Distribution");
}


DW_INLINE DwText& DwHeader::Expires()
{
    return (DwText&) FieldBody("Expires");
}


DW_INLINE DwText& DwHeader::FollowupTo()
{
    return (DwText&) FieldBody("FollowupTo");
}


DW_INLINE DwText& DwHeader::Lines()
{
    return (DwText&) FieldBody("Lines");
}


DW_INLINE DwText& DwHeader::Newsgroups()
{
    return (DwText&) FieldBody("Newsgroups");
}


DW_INLINE DwText& DwHeader::Organization()
{
    return (DwText&) FieldBody("Organization");
}


DW_INLINE DwText& DwHeader::Path()
{
    return (DwText&) FieldBody("Path");
}


DW_INLINE DwText& DwHeader::Summary()
{
    return (DwText&) FieldBody("Summary");
}


DW_INLINE DwText& DwHeader::Xref()
{
    return (DwText&) FieldBody("Xref");
}



DW_INLINE DwText& DwHeader::ContentDescription()
{
    return (DwText&) FieldBody("Content-Description");
}


DW_INLINE DwMsgId& DwHeader::ContentId()
{
    return (DwMsgId&) FieldBody("Content-Id");
}


DW_INLINE DwMechanism& DwHeader::ContentTransferEncoding()
{
    return (DwMechanism&)
        FieldBody("Content-Transfer-Encoding");
}


DW_INLINE DwMechanism& DwHeader::Cte()
{
    return (DwMechanism&)
        FieldBody("Content-Transfer-Encoding");
}


DW_INLINE DwMediaType& DwHeader::ContentType()
{
    return (DwMediaType&) FieldBody("Content-Type");
}


DW_INLINE DwText& DwHeader::MimeVersion()
{
    return (DwText&) FieldBody("MIME-Version");
}


DW_INLINE DwText& DwHeader::ContentDisposition()
{
    return (DwText&) FieldBody("Content-Disposition");
}
