/* -*- C++ -*-
 *
 *            kPPP: A pppd front end for the KDE project
 *
 * $Id$
 * 
 *            Copyright (C) 1997 Bernd Johannes Wuebben 
 *                   wuebben@math.cornell.edu
 *
 * based on EzPPP:
 * Copyright (C) 1997  Jay Painter
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


#ifndef _PPPDATA_H_
#define _PPPDATA_H_

#include <kapp.h>
#include <qstring.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <pwd.h>

#include "config.h"

// Define sizes for the data structures.
// Warning!  Do not change these unless you are I and you remember 
// the other things they effect.

// string lengths for struct gendata

#define PATH_SIZE 120 
#define MODEMDEV_SIZE 80
#define MODEMSTR_SIZE 50
#define FLOWCONTROL_SIZE 10


//string lengths for struct accdata

#define ACCNAME_SIZE 50
#define PHONENUMBER_SIZE 50
#define SPEED_SIZE 6
#define COMMAND_SIZE 255
#define IPADDR_SIZE 15
#define SCRIPT_TYPE_SIZE 30
#define SCRIPT_SIZE 50
#define ARGUMENT_SIZE 50
#define DOMAIN_SIZE 50
#define TIMEOUT_SIZE 4
#define ACCOUNTING_SIZE	200
#define BOOLEAN_SIZE 6

//
// keys for config file
//

// groups
#define GENERAL_GRP        "General"
#define MODEM_GRP          "Modem"
#define ACCOUNT_GRP        "Account"

// general
#define DEFAULTACCOUNT_KEY "DefaultAccount"
#define PPPDPATH_KEY       "pppdPath"
#define LOGVIEWER_KEY      "LogViewer"
#define PPPDTIMEOUT_KEY    "pppdTimeout"
#define SHOWCLOCK_KEY      "ShowClock"
#define SHOWLOGWIN_KEY     "ShowLogWindow"
#define AUTOREDIAL_KEY     "AutomaticRedial"
#define DISCONNECT_KEY     "DisconnectOnXServerExit"
#define NUMACCOUNTS_KEY    "NumberOfAccounts"

// modem
#define MODEMDEV_KEY       "Device"
#define LOCKFILE_KEY       "Lockfile"
#define FLOWCONTROL_KEY    "FlowControl"
#define TIMEOUT_KEY        "Timeout"
#define BUSYWAIT_KEY       "BusyWait"
#define INITSTR_KEY        "InitString"
#define INITRESP_KEY       "InitResponse"
#define DIALSTR_KEY        "DialString"
#define CONNECTRESP_KEY    "ConnectResponse"
#define BUSYRESP_KEY       "BusyResponse"
#define NOCARRIERRESP_KEY  "NoCarrierResponse"
#define NODIALTONERESP_KEY "NoDialToneResp"
#define ESCAPESTR_KEY      "EscapeString"
#define ESCAPERESP_KEY     "EscapeResponse"
#define ESCGUARDSTR_KEY    "EscapeGuard"
#define HANGUPSTR_KEY      "HangupString"
#define HANGUPRESP_KEY     "HangUpResponse"
#define ANSWERSTR_KEY      "AnswerString"
#define RINGRESP_KEY       "RingResponse"
#define ANSWERRESP_KEY     "AnswerResponse"
#define ENTER_KEY          "Enter"
#define FASTINIT_KEY       "FastModemInit"

// account
#define NAME_KEY           "Name"
#define PHONENUMBER_KEY    "Phonenumber"
#define SPEED_KEY          "Speed"
#define COMMAND_KEY        "Command"
#define IPADDR_KEY         "IPAddr"
#define SUBNETMASK_KEY     "SubnetMask"
#define ACCTENABLED_KEY    "AccountingEnabled"
#define ACCTFILE_KEY       "AccountingFile"
#define AUTONAME_KEY       "AutoName"
#define GATEWAY_KEY        "Gateway"
#define DEFAULTROUTE_KEY   "DefaultRoute"
#define DOMAIN_KEY         "Domain"
#define DNS_KEY            "DNS"
#define EXDNSDISABLED_KEY  "ExDNSDisabled"
#define SCRIPTCOM_KEY      "ScriptCommands"
#define SCRIPTARG_KEY      "ScriptArguments"
#define PPPDARG_KEY        "pppdArguments"

struct gendata {

  char defaultaccount[ACCNAME_SIZE+1];      // default account to connet
  char pppdpath[PATH_SIZE+1];               // path of pppd daemon
  char pppdtimeout[TIMEOUT_SIZE+1];         // timeout of pppd daemon
  char busywait[TIMEOUT_SIZE+1];         // time before redial on busy
  char enter[PATH_SIZE+1];                  // type of Enter string
  int  fastmodeminit;		  // Does the modem need extra time when initializing?

  char flowcontrol[FLOWCONTROL_SIZE+1];     // modem flow control
  char modemdevice[MODEMDEV_SIZE+1];        // path of modem device
  char modemtimeout[TIMEOUT_SIZE+1];        // modem response timeout 

  char modemlockfile[PATH_SIZE+1];	    // lock file
  char modeminitstr[MODEMSTR_SIZE+1];       // modem init string
  char initresp[MODEMSTR_SIZE+1];
  char modemdialstr[MODEMSTR_SIZE+1];       // modem dial string
  char connectresp[MODEMSTR_SIZE+1];
  char busyresp[MODEMSTR_SIZE+1];
  char nocarrierresp[MODEMSTR_SIZE+1];
  char nodialtoneresp[MODEMSTR_SIZE+1];

  char modemescapestr[MODEMSTR_SIZE+1];     // modem escape string.
  char modemescaperesp[MODEMSTR_SIZE+1];     
  int  modemescapeguardtime; 		    // 100'ths of a sec. 

  char modemhangupstr[MODEMSTR_SIZE+1];     // modem hangup string
  char hangupresp[MODEMSTR_SIZE+1];

  char modemanswerstr[MODEMSTR_SIZE+1];     // modem answer string
  char ringresp[MODEMSTR_SIZE+1];
  char answerresp[MODEMSTR_SIZE+1];  

  char show_clock_on_caption[ARGUMENT_SIZE+1];
  char show_log_window[ARGUMENT_SIZE+1];
  char disconnect_on_xserver_exit[ARGUMENT_SIZE+1];
  char automatic_redial[ARGUMENT_SIZE+1];
  char logviewer[PATH_SIZE+1];               // path of the log file view (e.g kedit)

};

struct accdata {
  char accname[ACCNAME_SIZE+1];                   // account name
  char phonenumber[PHONENUMBER_SIZE+1];           // phone number
  char speed[SPEED_SIZE+1];                       // connection speed
  char command[COMMAND_SIZE+1];              // command to execute upon connect
  char ipaddr[IPADDR_SIZE+1];                     // ipaddress
  char subnetmask[IPADDR_SIZE+1];                 // subnet mask
  char autoname[ARGUMENT_SIZE+1];		  // enable autoconfig hostname
  char domain[DOMAIN_SIZE+1];       		  // domain name
  char dns[MAX_DNS_ENTRIES][IPADDR_SIZE+1];       // dns ip address
  char exdnsdisabled[BOOLEAN_SIZE+1];	          // existing dns servers are disabled during connection
  char gateway[IPADDR_SIZE+1];                    // gateway ip address
  char defaultroute[ARGUMENT_SIZE+1];		  // enable defaultroute
  char stype[MAX_SCRIPT_ENTRIES][SCRIPT_TYPE_SIZE+1]; // type of script entry
  char sdata[MAX_SCRIPT_ENTRIES][SCRIPT_SIZE+1];      // script data
  char pppdarguments[MAX_PPPD_ARGUMENTS][ARGUMENT_SIZE+1]; //custom pppd arguments
  char accounting[ACCOUNTING_SIZE+1];		   // ruleset to use for accounting
  char accounting_enabled[BOOLEAN_SIZE+1];	   // is accounting enabled?
};


class PPPData {
public:
  PPPData();
  ~PPPData() {}

  // general functions

  void load(const KApplication*);
  void save();


  // functions to set/get general xppp info

  const char* defaultAccount();
  void setDefaultAccount(const char *);

  void set_xserver_exit_disconnect(bool set);
  const bool get_xserver_exit_disconnect();

  void set_show_clock_on_caption(bool set);
  const bool get_show_clock_on_caption();

  void set_show_log_window(bool set);
  const bool get_show_log_window();

  void set_automatic_redial(bool set);
  const bool get_automatic_redial();

  const char* logViewer();
  void setlogViewer(const char*);

  const char* pppdPath();
  void setpppdPath(const char *);

  const char* enter();
  void setEnter(const char *);

  int FastModemInit();
  void setFastModemInit(const int);

  const char * pppdTimeout();
  void setpppdTimeout(const char *);

  const char * busyWait();
  void setbusyWait(const char *);

  const char* modemLockFile();
  void setModemLockFile(const char *);

  const char* modemDevice();
  void setModemDevice(const char *);

  const char* flowcontrol();
  void setFlowcontrol(const char *);

  const char * modemTimeout();
  void setModemTimeout(const char *);

  // modem command stings/responses

  const char* modemInitStr();
  void setModemInitStr(const char *);

  const char* modemInitResp();
  void setModemInitResp(const char *);

  const char* modemDialStr();
  void setModemDialStr(const char*);

  const char* modemConnectResp();
  void setModemConnectResp(const char *);

  const char* modemBusyResp();
  void setModemBusyResp(const char *);

  const char* modemNoCarrierResp();
  void setModemNoCarrierResp(const char *);

  const char* modemNoDialtoneResp();
  void setModemNoDialtoneResp(const char *);


  const char* modemEscapeStr();
  void setModemEscapeStr(const char*);

  const char* modemEscapeResp();
  void setModemEscapeResp(const char*);

  const int modemEscapeGuardTime();
  void setModemEscapeGuardTime(const int);

  const char* modemHangupStr();
  void setModemHangupStr(const char*);

  const char* modemHangupResp();
  void setModemHangupResp(const char*);


  const char* modemAnswerStr();
  void setModemAnswerStr(const char*);

  const char* modemRingResp();
  void setModemRingResp(const char*);

  const char* modemAnswerResp();
  void setModemAnswerResp(const char*);

  // functions to set/get account information

  int count();
  bool setAccount(const char *);
  bool setAccountbyIndex(int);

  bool isUniqueAccname(const char*);

  bool deleteAccount();
  bool deleteAccount(const char *);
  int newaccount();       
  int copyaccount(int i);       

  const char* accname();
  void setAccname(const char *);
  
  const char* phonenumber();
  void setPhonenumber(const char *);

  const char* speed();
  void setSpeed(const char *);

  const char* command();
  void setCommand(const char*);

  const char* ipaddr();
  void setIpaddr(const char *);

  const char* subnetmask();
  void setSubnetmask(const char *);

  const bool AcctEnabled();
  void setAcctEnabled(bool set);

  const bool autoname();
  void setAutoname(bool set);

  const char* gateway();
  void setGateway(const char *);

  const bool defaultroute();
  void setDefaultroute(bool set);

  const char* dns(int);
  void setDns(int, const char *);

  const char* domain();
  void setDomain(const char *);
  
  const bool exDNSDisabled();
  void setExDNSDisabled(bool set);

  const char* scriptType(int);
  void setScriptType(int, const char*);

  const char* script(int);
  void setScript(int, const char *);

  const char* pppdArgument(int);
  void setpppdArgumentDefaults();
  void setpppdArgument(int, const char*);

  //functions to change/set the child pppd process info

  pid_t pppdpid();
  void setpppdpid(pid_t);

  // functions to set/query the accounting info
  const char *accountingFile();
  void setAccountingFile(const char *);


private:

  int highcount;                         // index of highest account
  int caccount;                          // index of the current account

  struct accdata ad[MAX_ACCOUNTS];       // account data
  struct gendata gd;                     // general data 

  pid_t pppdprocessid;                   // process ID of the child pppd
                                         // daemon

  KConfig* config;                       // configuration object   
};

extern PPPData gpppdata;

#endif
