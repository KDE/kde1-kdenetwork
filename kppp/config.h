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


#ifndef _CONFIG_H_
#define _CONFIG_H_


// Warning: If you fiddle with the following directories you have
// to adjust make_directories() in main.cpp()

// Define data file ($HOME relative)
#define KPPPDATA_FILE ".kde/share/apps/kppp/kpppdata"
// specifies the directory where kppp stores it's local
// accounting files and it's logfile (relative to the
// users homedirectory)
#define ACCOUNTING_PATH	".kde/share/apps/kppp/"


// Define the default modem response timeout
#define MODEM_TIMEOUT "60"

// Define the default time for pppd to get the interface up
#define PPPD_TIMEOUT "30"

// Define the default time to wait after a busy signal before redialing
#define BUSY_WAIT "0"



// Every PPP_STATS_INTERVAL milli seconds kppp will read
// and display the ppp statistics IF the stats window
// is visible. If the stats window is not visible 
// the stats are not taken.
// 200 milli secs is 5 times per second and results in
// 0 load on my machine. Play with this parameter if 
// you feel like it.

#define PPP_STATS_INTERVAL 200 
// comment this out to get some more debugging info
/*
#define MY_DEBUG
*/

// NOTE: Any changes made here will make your current kpppdata file
//       incompatible with the new kPPP program!

// Define the maximum number of accounts
#define MAX_ACCOUNTS 10

// Define the mamimum number of script entries
#define MAX_SCRIPT_ENTRIES 15

// Define the maximun number of DNS entries
#define MAX_DNS_ENTRIES 5

// Define the maximum number of arguments passed to the pppd daemon
#define MAX_PPPD_ARGUMENTS 20

// Define the maximun number of lines of /etc/resolv.conf
#define MAX_RESOLVCONF_LINES 30

// Lock File for the modem device (Needed by mgetty users)
#define MODEM_LOCK_FILE "/var/lock/LCK..modem"

// /proc dir 
#define PROC_DIR "/proc"

#endif


