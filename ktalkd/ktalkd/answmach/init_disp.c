/*
 * Copyright (c) 1983 Regents of the University of California.
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

/* From: @(#)init_disp.c	5.4 (Berkeley) 6/1/90 */

/*
 * Talk initialization.
 */

#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include "talk.h"
#include "../print.h"

extern char char_erase;

#ifndef VWERASE
#ifdef VWERSE
 /* for AIX */
#define VWERASE VWERSE
#else
#warning VWERASE not defined. Defining as 14.
#define VWERASE 14
#endif
#endif

/*
 * Trade edit characters with the other talk. By agreement
 * the first three characters each talk transmits after
 * connection are the three edit characters.
 */
 
void set_edit_chars(void)
{
	char buf[3], char_Werase;
	int cc;
	struct termios tios;
        /* Fill in the buffer */
	tcgetattr(0, &tios);
	buf[0] = tios.c_cc[VERASE];
	buf[1] = tios.c_cc[VKILL];
        message2("buf[1]=%d",buf[1]);
	char_Werase = tios.c_cc[VWERASE];

	if (char_Werase == (char) -1) {
		char_Werase = '\027';	 /* control W */
	}

	buf[2] = char_Werase;
        /* Write our config to the caller */
	cc = write(sockt, buf, sizeof(buf));
	if (cc != sizeof(buf) )
		p_error("Lost the connection");
        /* Read the caller configuration */
	cc = read(sockt, buf, sizeof(buf));
	if (cc != sizeof(buf) )
		p_error("Lost the connection");
	char_erase = buf[0];
}

/*
 * All done talking...hang up the phone
 */
void quit(void)
{
	_exit(0);
}
