/* -*- C++ -*-
 *            kPPP: A pppd front end for the KDE project
 *
 * $Id$
 * 
 *            Copyright (C) 1997 Bernd Johannes Wuebben 
 *                   wuebben@math.cornell.edu
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

#include <qstring.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/types.h>

// returns the users homedirectory even if the program
// is running SUID
QString getHomeDir() {
  struct passwd *pwd;
  pwd = getpwuid(getuid());
  if(pwd == NULL)
    return QString("/");
  
  QString s = pwd->pw_dir;
  if(s.right(1) != "/")
    s += "/";
  return s;
}
      
    
  
