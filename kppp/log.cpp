/* -*- C++ -*-
 *            kPPP: A pppd front end for the KDE project
 *
 * $Id$
 *
 *            Copyright (C) 1997 Bernd Johannes Wuebben
 *                   wuebben@math.cornell.edu
 *
 * This file contributed by: Mario Weilguni, <mweilguni@sime.com>
 *
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this program; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "log.h"
#include <stdio.h>
#include <ctype.h>
#include <kapp.h>

const char *prgname() {
  if(kapp==0)
    return "kppp";
  else {
    char *p = kapp->argv()[0];
    if(strrchr(p, '/'))
      return strrchr(p, '/')+1;
    else
      return p;
  }
}

void PRINTDEBUG(char *file, int line, const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  
  // strip newlines
  QString s(fmt);
  while(s.length() && s.right(1) == "\n")
    s = s.left(s.length() - 1);

  fprintf(stderr, "[%s:%s:%d]: ", prgname(), file, line);
  vfprintf(stderr, s.data(), ap);
  fprintf(stderr, "\n");
  va_end(ap);
}
