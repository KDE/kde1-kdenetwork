/*
 *
 *              kPPP: A pppd Front End for the KDE project
 *              Copyright (C) 1997 Bernd Johannes Wuebben
 *                      wuebben@math.cornell.edu
 * 
 * This file was contributed by Mario Weilguni <mweilguni@sime.com>
 * Thanks Mario!
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this program; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <qdir.h>
#include "chap.h"
#include "pap.h"

bool CHAP_UseCHAP() {
  //  return (bool)(gpppdata.authMethod() == AUTH_CHAP);  FIX
}

bool CHAP_CreateAuthFile() {
  //  return PAP_CreateAuthFile(CHAP_AUTH_FILE);
}

bool CHAP_RemoveAuthFile() {
  //  return PAP_RemoveAuthFile(CHAP_AUTH_FILE);
}
