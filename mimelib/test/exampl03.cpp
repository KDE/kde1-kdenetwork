//=============================================================================
// File:       exampl03.cpp
// Contents:   Source code for Example 3 -- Creating a multipart message
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
#include <time.h>
#include <iostream.h>
#include <fstream.h>
#include "multipar.h"


int main()
{
    // Initialize the library
    DwInitialize();
    
    // Get a buffer of data from a text file
    char* buff = new char[10000];
    int buffPos = 0;
    ifstream istrm("test03.txt");
    while (1) {
        char ch;
        istrm.get(ch);
        if (!istrm || buffPos == 9999) break;
        buff[buffPos++] = ch;
    }
    buff[buffPos] = 0;
    istrm.close();

    // Add the buffer to a DwString
    DwString data(buff, 10000, 0, buffPos);

    // Create a MultipartMessage
    MultipartMessage msg;

    // Create MIME-Version and Message-id header fields
    msg.SetAutomaticFields();

    // Set header fields
    DwUint32 t = (DwUint32) time(NULL);
    msg.SetDate(t);
    msg.SetFrom("Alfred <alfred@batcave.us>");
    msg.SetTo("Bruce Wayne <bwayne@mega.com>");
    msg.SetCc("Robin");
    msg.SetBcc("Penguin");
    msg.SetSubject("Here's a multipart message");

    // Add body part 1
    MultipartBodyPart part;
    part.SetType(DwMime::kTypeText);
    part.SetSubtype(DwMime::kSubtypePlain);
    part.SetContentTransferEncoding(DwMime::kCte7bit);
    part.SetContentDescription("text, unencoded");
    part.SetBody(data);
    msg.AddBodyPart(part);

    // Add body part 2
    part.SetType(DwMime::kTypeText);
    part.SetSubtype(DwMime::kSubtypePlain);
    part.SetContentTransferEncoding(DwMime::kCteBase64);
    part.SetContentDescription("text, base64 encoded");
    DwString ascData;
    DwEncodeBase64(data, ascData);
    part.SetBody(ascData);
    msg.AddBodyPart(part);

    // Write it to a file
    ofstream ostrm("exampl03.out");
    ostrm << msg.AsString();

    return 0;
}
