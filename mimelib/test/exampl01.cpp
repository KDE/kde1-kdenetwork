//=============================================================================
// File:       exampl01.cpp
// Contents:   Source code for Example 1 -- Creating a simple message
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
#include "basicmsg.h"

int main()
{
    // Initialize the library
    DwInitialize();
    
    // Get a buffer of data from a text file
    char* buff = new char[10000];
    int buffPos = 0;
    ifstream istrm("test01.txt");
    while (1) {
        char ch;
        istrm.get(ch);
        if (!istrm || buffPos == 9999) break;
        buff[buffPos++] = ch;
    }
    istrm.close();
    
    // Add the buffer to a DwString
    DwString data(buff, 10000, 0, buffPos);

    // Create a message
    BasicMessage msg;
    
    // Create MIME-Version and Message-id header fields
    msg.SetAutomaticFields();

    // Set header fields
    msg.SetDate(time(NULL));
    msg.SetTypeStr("Text");
    msg.SetSubtypeStr("Plain");
    msg.SetCteStr("7bit");
    msg.SetFrom("Alfred <alfred@batcave.us>");
    msg.SetTo("Bruce Wayne <bwayne@mega.com>");
    msg.SetCc("Robin");
    msg.SetBcc("Penguin");
    msg.SetSubject("Here's a simple message");

    // Set body
    msg.SetBody(data);

    // Write it to a file
    ofstream ostrm("exampl01.out");
    ostrm << msg.AsString();

    return 0;
}
