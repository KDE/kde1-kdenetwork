/*
 * Copyright (c) 1998 Burkhard Lehner and David Faure
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <config.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/un.h>
#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#else
#include <time.h>
#endif
#include "print.h"
#include "announce.h" // for N_CHARS

int sendToKtalk (const char *username, const char *announce)
/*
   sends an announcement to a running ktalk client.
      username: name of the user who shall receive the announce
      announce: name and IP address of the one who requests the talk

      return value: 1 if ktalk was found and was running
                    0 otherwise
*/

{
  int sock = -1;
  struct sockaddr_un ktalkAddr, tempAddr;
  int result = 0;

  ktalkAddr.sun_family = AF_UNIX;
  strcpy (ktalkAddr.sun_path, "/tmp/ktalk-");
  strcat (ktalkAddr.sun_path, username);
  if ((sock = socket (AF_UNIX, SOCK_DGRAM, 0)) < 0) return 0;
  if (bind (sock, (struct sockaddr *) &ktalkAddr, 
            sizeof (ktalkAddr)) == 0) {
    /* bind succeeds => socket didn't exists, so no ktalk is running */
    close (sock);
    unlink (ktalkAddr.sun_path);
    message("Socket not found. No ktalk running.");
    return 0;
  }

  tempAddr.sun_family = AF_UNIX;
  if (tmpnam (tempAddr.sun_path) == 0 || 
      bind (sock, (struct sockaddr *) &tempAddr, sizeof (tempAddr)) == -1 ||
      chmod (tempAddr.sun_path,
             S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH) ) {
      close (sock);
      return 0;
  }

  char buffer [N_CHARS+2];
  buffer [0] = 1;
  strcpy (buffer + 1, announce); // announce is at most N_CHARS long.
  unsigned int announcelen = strlen(buffer);
  unsigned int len;
  len = sendto (sock, buffer, announcelen, 0,
                (struct sockaddr *) &ktalkAddr, sizeof (ktalkAddr));
  if (len == announcelen) {
    fd_set readFDs;
    FD_ZERO (&readFDs);
    FD_SET (sock, &readFDs);
    char answer;
    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 500000;  // Wait for answer at most 0.5 seconds
    if ( (select (sock + 1, &readFDs, 0, 0, &timeout) > 0) &&
         (recv (sock, &answer, 1, 0) == 1) ) {
        result = ( answer == 42 ); // Answer from ktalk has to be 42.
    }
  }
  close (sock);
  unlink (tempAddr.sun_path);
  return result;
}
