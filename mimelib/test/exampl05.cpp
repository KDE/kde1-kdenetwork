//=============================================================================
// File:       exampl05.cpp
// Contents:   Source code for Example 5 -- Creating a message with
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
#include "attach.h"


int main()
{
    // Initialize the library

    DwInitialize();

    // Create a MultipartMessage

    MessageWithAttachments msg;

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

    // Add text
    
    DwString text = "Some files are attached\n";
    msg.SetText(text);

    // Add 7bit attachment

    msg.Attach7bitFile("test05.txt");

    // Add 8bit attachment

    msg.Attach8bitFile("test05.txt");

    // Add binary attachment

    msg.AttachBinaryFile("test05.txt");

    // Write it to a file

    ofstream ostrm("exampl05.out");
    ostrm << msg.AsString();

    return 0;
}
