//=============================================================================
// File:       exampl04.cpp
// Contents:   Source code for Example 4 -- Parsing a multipart message
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

#include <stdlib.h>
#include <iostream.h>
#include <fstream.h>
#include "multipar.h"


int main()
{
    // Initialize the library
    DwInitialize();

    // Read message from file
    char* buff = new char[10000];
    int buffPos = 0;
    ifstream istrm("test04.txt");
    while (istrm) {
        char ch;
        istrm.get(ch);
        buff[buffPos++] = ch;
        if (buffPos == 10000) break;
    }
    DwString messageStr(buff, 10000, 0, buffPos);
    istrm.close();
    
    // Create a DwMessage and parse it.  The DwMessage should be created on
    // the free store, since it will be added to the MultipartMessage.

    DwMessage* msg = DwMessage::NewMessage(messageStr, 0);
    msg->Parse();

    // Make sure it is a multipart message
    // If is not a multipart message, we could create a BasicMessage instead,
    // but we won't do that in this example.
    
    if (msg->Header().ContentType().Type() != DwMime::kTypeMultipart) {
        cerr << "Not a multipart message\n";
        return 0;
    }
    
    // Create a MultipartMessage

    MultipartMessage multipart(msg);

    // Open file stream for output

    ofstream ostrm("exampl04.out");

    // Print the header fields

    ostrm << "Type -> "    << multipart.TypeStr()    << "\n";
    ostrm << "Subtype -> " << multipart.SubtypeStr() << "\n";
    ostrm << "Date -> "    << multipart.DateStr()    << "\n";
    ostrm << "From -> "    << multipart.From()       << "\n";
    ostrm << "To -> "      << multipart.To()         << "\n";
    ostrm << "Cc -> "      << multipart.Cc()         << "\n";
    ostrm << "Bcc -> "     << multipart.Bcc()        << "\n";
    ostrm << "Subject -> " << multipart.Subject()    << "\n";
    
    // Read the body parts and print them

    MultipartBodyPart part;
    DwString body;
    int numParts = multipart.NumberOfParts();
    for (int idx=0; idx < numParts; ++idx) {
        multipart.BodyPart(idx, part);
        ostrm << "\nBody part number " << idx << "\n";
        ostrm << "Type -> " << part.TypeStr() << "\n";
        ostrm << "Subtype -> " << part.SubtypeStr() << "\n";
        ostrm << "Content transfer encoding -> "
            << part.ContentTransferEncodingStr() << "\n";
        ostrm << "Content description -> "
            << part.ContentDescription() << "\n";
        int cte = part.ContentTransferEncoding();
        if (cte == DwMime::kCteBase64) {
            DwDecodeBase64(part.Body(), body);
            ostrm << "Body (decoded) ->\n" << body << "\n";
        }
        else if (cte == DwMime::kCteQuotedPrintable) {
            DwDecodeQuotedPrintable(part.Body(), body);
            ostrm << "Body (decoded) ->\n" << body << "\n";
        }
        else {
            body = part.Body();
            ostrm << "Body ->\n" << body << "\n";
        }
    }
    return 0;
}

