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
#include <qmsgbox.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <pwd.h>

#include "config.h"

// string lengths

#define PATH_SIZE 120 
#define MODEMSTR_SIZE 50
#define ACCNAME_SIZE 50
#define PHONENUMBER_SIZE 25
#define COMMAND_SIZE 255
#define IPADDR_SIZE 15
#define DOMAIN_SIZE 50
#define TIMEOUT_SIZE 4

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
#define ID_KEY		   "ID"

// modem
#define MODEMDEV_KEY       "Device"
#define LOCKFILE_KEY       "Lockfile"
#define FLOWCONTROL_KEY    "FlowControl"
#define SPEED_KEY          "Speed"
#define TIMEOUT_KEY        "Timeout"
#define BUSYWAIT_KEY       "BusyWait"
#define INITSTR_KEY        "InitString"
#define INITRESP_KEY       "InitResponse"
#define DIALSTR_KEY        "DialString"
#define CONNECTRESP_KEY    "ConnectResponse"
#define BUSYRESP_KEY       "BusyResponse"
#define NOCARRIERRESP_KEY  "NoCarrierResponse"
#define NODIALTONERESP_KEY "NoDialToneResp"
#define HANGUPSTR_KEY      "HangupString"
#define HANGUPRESP_KEY     "HangUpResponse"
#define ANSWERSTR_KEY      "AnswerString"
#define RINGRESP_KEY       "RingResponse"
#define ANSWERRESP_KEY     "AnswerResponse"
#define ENTER_KEY          "Enter"
#define ESCAPESTR_KEY      "EscapeString"
#define ESCAPERESP_KEY     "EscapeResponse"
#define ESCAPEGUARDTIME_KEY "EscapeGuardTime"
#define FASTINIT_KEY       "FastModemInit"

// account
#define NAME_KEY           "Name"
#define PHONENUMBER_KEY    "Phonenumber"
#define AUTH_KEY           "Authentication"
#define STORED_PASSWORD_KEY "Password"
#define STORED_USERNAME_KEY "Username"
#define STORE_PASSWORD_KEY "StorePassword"
#define COMMAND_KEY        "Command"
#define DISCONNECT_COMMAND_KEY "DisconnectCommand"
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
#define ICONIFY_ON_CONNECT_KEY "iconifyOnConnect" 
#define TOTALCOSTS_KEY     "TotalCosts"

class PPPData {
public:
  PPPData();
  ~PPPData() {};

  // general functions

  bool open(const KApplication*);
  void save();
  void cancel();

  // function to read/write date to configuration file

  const char* readConfig(const char *, const char *, const char *);
  int readNumConfig(const char *, const char *, int);
  const char* readListConfig(const char *, const char *, int);
  void writeConfig(const char *, const char *, const char *);
  void writeConfig(const char *, const char *, int);
  void writeListConfig(const char *, const char *, int, const char*);

  // functions to set/get general xppp info

  const char* Password();
  const char* Id();

  void setPassword(const char* );
  void setId(const char*);

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

  void set_iconify_on_connect(bool set);
  const bool get_iconify_on_connect();

  const char* logViewer();
  void setlogViewer(const char*);

  const char* pppdPath();
  void setpppdPath(const char *);

  const char* enter();
  void setEnter(const char *);

  const char * pppdTimeout();
  void setpppdTimeout(const char *);

  const char * busyWait();
  void setbusyWait(const char *);

  const char* modemLockFile();
  void setModemLockFile(const char *);

  int modemEscapeGuardTime();
  void setModemEscapeGuardTime(int i);

  void setModemEscapeStr(const char* n);
  const char* modemEscapeStr();

  void setModemEscapeResp(const char* n);
  const char* modemEscapeResp();

  const char* modemDevice();
  void setModemDevice(const char *);

  const char* flowcontrol();
  void setFlowcontrol(const char *);

  const char * modemTimeout();
  void setModemTimeout(const char *);

  // modem command strings/responses

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

  void setFastModemInit(const int n);
  int  FastModemInit();
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

  const int authMethod();
  void setAuthMethod(int);

  const char *storedUsername();
  void setStoredUsername(const char*);

  const char *storedPassword();
  void setStoredPassword(const char*);

  const bool storePassword();
  void setStorePassword(bool);

  const char* speed();
  void setSpeed(const char *);

  const char* command_on_connect();
  void setCommand_on_connect(const char*);

  const char* command_on_disconnect();
  void setCommand_on_disconnect(const char*);

  const char* ipaddr();
  void setIpaddr(const char *);

  const char* subnetmask();
  void setSubnetmask(const char *);

  const bool AcctEnabled();
  void setAcctEnabled(bool set);

  const bool exDNSDisabled();
  void setExDNSDisabled(bool set);

  const bool autoname();
  void setAutoname(bool set);

  const char* gateway();
  void setGateway(const char *);

  const bool defaultroute();
  void setDefaultroute(bool set);

  const char* dns(int);
  void setDns(int, const char*);

  const char* domain();
  void setDomain(const char *);

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

  const char *totalCosts();
  void setTotalCosts(const char *);


public:
  QString password;
  QString ID;
private:

  int highcount;                         // index of highest account
  int caccount;                          // index of the current account
  QString cgroup;                        // name of current config group

  KConfig* config;                       // configuration object   

  pid_t pppdprocessid;                   // process ID of the child pppd
                                         // daemon


};

extern PPPData gpppdata;

#endif
