/*
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

#include "pppdata.h"

PPPData gpppdata;

PPPData::PPPData() {
  char *p_ad = (char*)&ad;
  char *p_gd = (char*)&gd;

  // clear out all the data structures first
  for(uint i = 0; i <= (sizeof(accdata)-1)*MAX_ACCOUNTS; i++)
    p_ad[i] = '\0';

  for(uint i=0; i <= sizeof(gendata)-1; i++)
    p_gd[i] = '\0';


  //initalize variables
  highcount = -1;      // start out with no entries
  caccount = -1;       // set the current account index also
  pppdprocessid = -1;  // process ID of the child pppd daemon

}

//
// load data from configuration file 
//

void PPPData::load(const KApplication* a) {

  QString countstr;
  QStrList dnslist, scriptcomlist, scriptarglist, pppdArgumentlist;
  int count;

  config = a->getConfig();

  config->setGroup(GENERAL_GRP);
  setDefaultAccount(config->readEntry(DEFAULTACCOUNT_KEY, ""));
  setpppdPath(config->readEntry(PPPDPATH_KEY, "/usr/sbin/pppd"));
  setlogViewer(config->readEntry(LOGVIEWER_KEY, "/usr/local/kde/bin/kedit"));
  setpppdTimeout(config->readEntry(PPPDTIMEOUT_KEY, PPPD_TIMEOUT));
  set_show_clock_on_caption(config->readNumEntry(SHOWCLOCK_KEY, TRUE));
  set_show_log_window(config->readNumEntry(SHOWLOGWIN_KEY, FALSE));
  set_automatic_redial(config->readNumEntry(AUTOREDIAL_KEY, FALSE));
  set_xserver_exit_disconnect(config->readNumEntry(DISCONNECT_KEY, TRUE));
  count =  config->readNumEntry(NUMACCOUNTS_KEY, -1);  

  config->setGroup(MODEM_GRP);
  setModemDevice(config->readEntry(MODEMDEV_KEY, "/dev/modem"));
  setModemLockFile(config->readEntry(LOCKFILE_KEY, MODEM_LOCK_FILE));
  setFlowcontrol(config->readEntry(FLOWCONTROL_KEY, "CRTSCTS"));
  setModemTimeout(config->readEntry(TIMEOUT_KEY, MODEM_TIMEOUT));
  setbusyWait(config->readEntry(BUSYWAIT_KEY, BUSY_WAIT));

  setModemInitStr(config->readEntry(INITSTR_KEY, "ATZ"));
  setModemInitResp(config->readEntry(INITRESP_KEY, "OK"));
  setModemDialStr(config->readEntry(DIALSTR_KEY, "ATDT"));
  setModemConnectResp(config->readEntry(CONNECTRESP_KEY, "CONNECT"));
  setModemBusyResp(config->readEntry(BUSYRESP_KEY, "BUSY"));
  setModemNoCarrierResp(config->readEntry(NOCARRIERRESP_KEY, "NO CARRIER"));
  setModemNoDialtoneResp(config->readEntry(NODIALTONERESP_KEY,
					    "NO DIALTONE"));
  setModemHangupStr(config->readEntry(HANGUPSTR_KEY, "+++ATH"));
  setModemHangupResp(config->readEntry(HANGUPRESP_KEY, "OK"));
  setModemAnswerStr(config->readEntry(ANSWERSTR_KEY, "ATA"));
  setModemRingResp(config->readEntry(RINGRESP_KEY , "RING"));
  setModemAnswerResp(config->readEntry(ANSWERRESP_KEY, "CONNECT"));
  setEnter(config->readEntry(ENTER_KEY, "CR/LF"));
  setFastModemInit(config->readNumEntry(FASTINIT_KEY,1));

  // accounts
  for(int i = 0; i < count; i++) {
    caccount = 0;
    countstr.setNum(i+1);
    newaccount();
    
    config->setGroup(ACCOUNT_GRP + countstr);
    setAccname(config->readEntry(NAME_KEY));
    setPhonenumber(config->readEntry(PHONENUMBER_KEY));
    setSpeed(config->readEntry(SPEED_KEY));
    setCommand(config->readEntry(COMMAND_KEY));
    setIpaddr(config->readEntry(IPADDR_KEY));
    setSubnetmask(config->readEntry(SUBNETMASK_KEY));
    setAcctEnabled(config->readNumEntry(ACCTENABLED_KEY,false));
    setAccountingFile(config->readEntry(ACCTFILE_KEY));
    setAutoname(config->readNumEntry(AUTONAME_KEY,false));
    setGateway(config->readEntry(GATEWAY_KEY));
    setDefaultroute(config->readEntry(DEFAULTROUTE_KEY));
    setDomain(config->readEntry(DOMAIN_KEY));
    setExDNSDisabled(config->readNumEntry(EXDNSDISABLED_KEY,false));
    
    // read lists
    config->readListEntry(DNS_KEY, dnslist);
    config->readListEntry(SCRIPTCOM_KEY, scriptcomlist);
    config->readListEntry(SCRIPTARG_KEY, scriptarglist);
    config->readListEntry(PPPDARG_KEY, pppdArgumentlist);

    // analyse lists
    for(uint j = 0; j < MAX_DNS_ENTRIES &&
	  j < dnslist.count(); j++) {
      setDns(j, dnslist.at(j));
    }
    for(uint j=0; j < MAX_SCRIPT_ENTRIES &&
	  j < scriptcomlist.count(); j++) {
      setScriptType(j, scriptcomlist.at(j)); 
      setScript(j, scriptarglist.at(j));
    }
    for(uint j=0; j < MAX_PPPD_ARGUMENTS && 
	  j < pppdArgumentlist.count (); j++) {
      setpppdArgument(j, pppdArgumentlist.at(j));
    }
  }

  if(highcount >= 0) {
    if(strcmp(gd.defaultaccount, "") == 0) {
      setAccountbyIndex(0);
      setDefaultAccount(accname());
    }
  }
}

//
// save configuration data
//

void PPPData::save() {
  QString countstr;
  QStrList dnslist, scriptcomlist, scriptarglist, pppdArgumentlist;

  config->setGroup(GENERAL_GRP);
  config->writeEntry(DEFAULTACCOUNT_KEY, defaultAccount());
  config->writeEntry(PPPDPATH_KEY, pppdPath());
  config->writeEntry(LOGVIEWER_KEY, logViewer());
  config->writeEntry(PPPDTIMEOUT_KEY, pppdTimeout());
  config->writeEntry(SHOWCLOCK_KEY, get_show_clock_on_caption());
  config->writeEntry(SHOWLOGWIN_KEY, get_show_log_window());
  config->writeEntry(AUTOREDIAL_KEY, get_automatic_redial());
  config->writeEntry(DISCONNECT_KEY, get_xserver_exit_disconnect());
  config->writeEntry(NUMACCOUNTS_KEY, count());

  config->setGroup(MODEM_GRP);

  config->writeEntry(MODEMDEV_KEY, modemDevice());
  config->writeEntry(LOCKFILE_KEY, modemLockFile());
  config->writeEntry(FLOWCONTROL_KEY, flowcontrol());
  config->writeEntry(TIMEOUT_KEY, modemTimeout());
  config->writeEntry(BUSYWAIT_KEY, busyWait());
  config->writeEntry(INITSTR_KEY, modemInitStr());
  config->writeEntry(INITRESP_KEY, modemInitResp());
  config->writeEntry(DIALSTR_KEY, modemDialStr());
  config->writeEntry(CONNECTRESP_KEY, modemConnectResp());
  config->writeEntry(BUSYRESP_KEY, modemBusyResp());
  config->writeEntry(NOCARRIERRESP_KEY, modemNoCarrierResp());
  config->writeEntry(NODIALTONERESP_KEY, modemNoDialtoneResp());
  config->writeEntry(HANGUPSTR_KEY, modemHangupStr());
  config->writeEntry(HANGUPRESP_KEY, modemHangupResp());
  config->writeEntry(ANSWERSTR_KEY, modemAnswerStr());
  config->writeEntry(RINGRESP_KEY, modemRingResp());
  config->writeEntry(ANSWERRESP_KEY, modemAnswerResp());
  config->writeEntry(ENTER_KEY, enter());
  config->writeEntry(FASTINIT_KEY, FastModemInit());

  // accounts
  for(int i = 0; i < count(); i++) {
    countstr.setNum(i+1);
    setAccountbyIndex(i);
    // fill lists
    dnslist.clear();
    scriptcomlist.clear();
    scriptarglist.clear();
    pppdArgumentlist.clear();
    for(int j = 0; j <= MAX_DNS_ENTRIES && 
	  strcmp(dns(j), "") != 0; j++) {
      dnslist.append(dns(j));
    }
    for(int j=0; strcmp(scriptType(j), "") != 0 &&
	   j <= MAX_SCRIPT_ENTRIES-1; j++) {
      scriptcomlist.append(scriptType(j));
      scriptarglist.append(script(j));
    }
    for(int j=0; strcmp(pppdArgument(j), "") != 0 &&
	j <= MAX_PPPD_ARGUMENTS+5; j++) {
      pppdArgumentlist.append(pppdArgument(j));
  }

    config->setGroup(ACCOUNT_GRP + countstr);
    config->writeEntry(NAME_KEY, accname());
    config->writeEntry(PHONENUMBER_KEY, phonenumber());
    config->writeEntry(SPEED_KEY, speed());
    config->writeEntry(COMMAND_KEY, command());
    config->writeEntry(IPADDR_KEY, ipaddr());
    config->writeEntry(SUBNETMASK_KEY, subnetmask());
    config->writeEntry(ACCTENABLED_KEY, AcctEnabled());
    config->writeEntry(ACCTFILE_KEY, accountingFile());
    config->writeEntry(AUTONAME_KEY, (int) autoname());
    config->writeEntry(GATEWAY_KEY, gateway());
    config->writeEntry(DEFAULTROUTE_KEY, (int) defaultroute());
    config->writeEntry(DOMAIN_KEY, domain());
    config->writeEntry(EXDNSDISABLED_KEY, exDNSDisabled());
    // write lists
    config->writeEntry(DNS_KEY, dnslist);
    config->writeEntry(SCRIPTCOM_KEY, scriptcomlist);
    config->writeEntry(SCRIPTARG_KEY, scriptarglist);
    config->writeEntry(PPPDARG_KEY, pppdArgumentlist);
  }
  config->sync();
  setAccount(defaultAccount());
}

//
// functions to set/return general information
//

const char* PPPData::defaultAccount() {
  return gd.defaultaccount;
}


void PPPData::setDefaultAccount(const char *n) {

  strncpy(gd.defaultaccount, n, ACCNAME_SIZE);
  gd.defaultaccount[ACCNAME_SIZE] = '\0';

  //now set the current account index to the default account
  setAccount(gd.defaultaccount);
}


void PPPData::set_show_clock_on_caption(bool set){
  
  if(set){
    strncpy(gd.show_clock_on_caption,"CAPTIONS_ON",ARGUMENT_SIZE);
  }else{
    strncpy(gd.show_clock_on_caption,"CAPTIONS_OFF",ARGUMENT_SIZE);
  }
}

const bool PPPData::get_show_clock_on_caption(){
  
  bool result;

  if(strcmp(gd.show_clock_on_caption,"CAPTIONS_ON") == 0 ){
    result = true;
  }
  else{
    result = false;
  }

  return result;

};

void PPPData::set_xserver_exit_disconnect(bool set){
  
  if(set){
    strncpy(gd.disconnect_on_xserver_exit,"DISC_ON_XSERV_ON",ARGUMENT_SIZE);
  }else{
    strncpy(gd.disconnect_on_xserver_exit,"DISC_ON_XSERV_OFF",ARGUMENT_SIZE);
  }
}

const bool PPPData::get_xserver_exit_disconnect(){
  
  bool result;

  if(strcmp(gd.disconnect_on_xserver_exit,"DISC_ON_XSERV_ON") == 0 ){
    result = true;
  }
  else{
    result = false;
  }

  return result;

};


void PPPData::set_show_log_window(bool set){
  
  if(set){
    strncpy(gd.show_log_window,"SHOW_LOG_WINDOW_ON",ARGUMENT_SIZE);
  }else{
    strncpy(gd.show_log_window,"SHOW_LOG_WINDOW_OFF",ARGUMENT_SIZE);
  }
}

const bool PPPData::get_show_log_window(){
  
  bool result;

  if(strcmp(gd.show_log_window,"SHOW_LOG_WINDOW_ON") == 0 ){
    result = true;
  }
  else{
    result = false;
  }

  return result;

};

void PPPData::set_automatic_redial(bool set){
  
  if(set){
    strncpy(gd.automatic_redial,"AUTOMATIC_REDIAL_ON",ARGUMENT_SIZE);
  }else{
    strncpy(gd.automatic_redial,"AUTOMATIC_REDIAL_OFF",ARGUMENT_SIZE);
  }
}

const bool PPPData::get_automatic_redial(){
  
  bool result;

  if(strcmp(gd.automatic_redial,"AUTOMATIC_REDIAL_ON") == 0 ){
    result = true;
  }
  else{
    result = false;
  }

  return result;

};




const char* PPPData::pppdPath() {
  return gd.pppdpath;
}

void PPPData::setpppdPath(const char *n) {
  strncpy(gd.pppdpath, n, PATH_SIZE);
  gd.pppdpath[PATH_SIZE] = '\0';
}

const char* PPPData::logViewer() {
  return gd.logviewer;
}

void PPPData::setlogViewer(const char *n) {
  strncpy(gd.logviewer, n, PATH_SIZE);
  gd.logviewer[PATH_SIZE] = '\0';
}


const char* PPPData::enter() {
  return gd.enter;
}

void PPPData::setEnter(const char *n) {
  strncpy(gd.enter, n, PATH_SIZE);
  gd.enter[PATH_SIZE] = '\0';
}

void PPPData::setFastModemInit(const int n) {
  gd.fastmodeminit = n;
}

int PPPData::FastModemInit() {
  return gd.fastmodeminit;
}

const char* PPPData::pppdTimeout() {
  return gd.pppdtimeout;
}

void PPPData::setpppdTimeout(const char *n) {
  strncpy(gd.pppdtimeout, n, TIMEOUT_SIZE);
  gd.pppdtimeout[TIMEOUT_SIZE] = '\0';
}

const char* PPPData::busyWait() {
  return gd.busywait;
}

void PPPData::setbusyWait(const char *n) {
  strncpy(gd.busywait, n, TIMEOUT_SIZE);
  gd.busywait[TIMEOUT_SIZE] = '\0';
}

//
//"Modem" Rolladex card
//
const char* PPPData::modemDevice() {
  return gd.modemdevice;
}

void PPPData::setModemDevice(const char *n) {
  strncpy(gd.modemdevice, n, MODEMDEV_SIZE);
  gd.modemdevice[MODEMDEV_SIZE] = '\0';
}


const char* PPPData::flowcontrol() {
  return gd.flowcontrol;
}

void PPPData::setFlowcontrol(const char *n) {
  strncpy(gd.flowcontrol, n, FLOWCONTROL_SIZE);
  gd.flowcontrol[FLOWCONTROL_SIZE] = '\0';
}

const char* PPPData::modemLockFile() {
  return gd.modemlockfile;
}

void PPPData::setModemLockFile(const char *n) {
  strncpy(gd.modemlockfile, n, PATH_SIZE);
  gd.modemlockfile[PATH_SIZE] = '\0';
}

const char* PPPData::modemTimeout() {
  return gd.modemtimeout;
}

void PPPData::setModemTimeout(const char *n) {
  strncpy(gd.modemtimeout, n, TIMEOUT_SIZE);
  gd.modemtimeout[TIMEOUT_SIZE] = '\0';
}

//
//Advanced "Modem" dialog
//
const char* PPPData::modemInitStr() {
  return gd.modeminitstr;
}

void PPPData::setModemInitStr(const char *n) {
  strncpy(gd.modeminitstr, n, MODEMSTR_SIZE);
  gd.modeminitstr[MODEMSTR_SIZE] = '\0';
} 


const char* PPPData::modemInitResp() {
  return gd.initresp;
}

void PPPData::setModemInitResp(const char *n) {
  strncpy(gd.initresp, n, MODEMSTR_SIZE);
  gd.initresp[MODEMSTR_SIZE] = '\0';
} 


const char* PPPData::modemDialStr() {
  return gd.modemdialstr;
}

void PPPData::setModemDialStr(const char *n) {
  strncpy(gd.modemdialstr, n, MODEMSTR_SIZE);
  gd.modemdialstr[MODEMSTR_SIZE] = '\0';
} 


const char* PPPData::modemConnectResp() {
  return gd.connectresp;
}

void PPPData::setModemConnectResp(const char *n) {
  strncpy(gd.connectresp, n, MODEMSTR_SIZE);
  gd.connectresp[MODEMSTR_SIZE] = '\0';
}


const char* PPPData::modemBusyResp() {
  return gd.busyresp;
}

void PPPData::setModemBusyResp(const char *n) {
  strncpy(gd.busyresp, n, MODEMSTR_SIZE);
  gd.busyresp[MODEMSTR_SIZE] = '\0';
}


const char* PPPData::modemNoCarrierResp() {
  return gd.nocarrierresp;
}

void PPPData::setModemNoCarrierResp(const char *n) {
  strncpy(gd.nocarrierresp, n, MODEMSTR_SIZE);
  gd.nocarrierresp[MODEMSTR_SIZE] = '\0';
}


const char* PPPData::modemNoDialtoneResp() {
  return gd.nodialtoneresp;
}

void PPPData::setModemNoDialtoneResp(const char *n) {
  strncpy(gd.nodialtoneresp, n, MODEMSTR_SIZE);
  gd.nodialtoneresp[MODEMSTR_SIZE] = '\0';
}


const char* PPPData::modemHangupStr() {
  return gd.modemhangupstr;
}

void PPPData::setModemHangupStr(const char *n) {
  strncpy(gd.modemhangupstr, n, MODEMSTR_SIZE);
  gd.modemhangupstr[MODEMSTR_SIZE] = '\0';
} 


const char* PPPData::modemHangupResp() {
  return gd.hangupresp;
}

void PPPData::setModemHangupResp(const char *n) {
  strncpy(gd.hangupresp, n, MODEMSTR_SIZE);
  gd.hangupresp[MODEMSTR_SIZE] = '\0';
}


const char* PPPData::modemAnswerStr() {
  return gd.modemanswerstr;
}

void PPPData::setModemAnswerStr(const char *n) {
  strncpy(gd.modemanswerstr, n, MODEMSTR_SIZE);
  gd.modemanswerstr[MODEMSTR_SIZE] = '\0';
} 


const char* PPPData::modemRingResp() {
  return gd.ringresp;
}

void PPPData::setModemRingResp(const char *n) {
  strncpy(gd.ringresp, n, MODEMSTR_SIZE);
  gd.ringresp[MODEMSTR_SIZE] = '\0';
}


const char* PPPData::modemAnswerResp() {
  return gd.answerresp;
}

void PPPData::setModemAnswerResp(const char *n) {
  strncpy(gd.answerresp, n, MODEMSTR_SIZE);
  gd.answerresp[MODEMSTR_SIZE] = '\0';
}


//
// functions to set/return account information
//

//returns number of accounts
int PPPData::count() {
  return highcount+1;
}


bool PPPData::setAccount( const char *aname ) {
  for(int i = 0; i <= highcount; i++)
    if(strncmp(ad[i].accname, aname, ACCNAME_SIZE) == 0) {
      caccount = i;
      return true;
    }
  return false;
}


bool PPPData::setAccountbyIndex(int i) {
  if(i >= 0 && i <= highcount) {
    caccount = i;
    return true;
  }

  return false;
}


bool PPPData::isUniqueAccname(const char *n) {
  for(int i=0; i <= highcount; i++) {
    if(strncmp(ad[i].accname, n, ACCNAME_SIZE) == 0  && i != caccount)
      return false;
  }

  return true;
}


bool PPPData::deleteAccount() {
  if(caccount < 0)
    return false;

  char *p_ad1, *p_ad2;

  for(int i = caccount+1; i <= highcount; i++) {
    p_ad1 = (char*)&(ad[i-1]);
    p_ad2 = (char*)&(ad[i]);

    for(uint j = 0; j <= sizeof(accdata)-1; j++)
      p_ad1[j] = p_ad2[j];
  }

  //make sure the top account is cleared
  p_ad2 = (char*)&(ad[highcount]);
  for(uint j = 0; j <= sizeof(accdata)-1; j++)
    p_ad2[j] = '\0';

  highcount--;
  if(caccount > highcount)
    caccount = highcount;

  return true;
} 


bool PPPData::deleteAccount( const char *aname ) {
  if(!setAccount(aname))
    return false;

  char *p_ad1, *p_ad2;

  for(int i = caccount+1; i <= highcount; i++) {
    p_ad1 = (char*)&(ad[i-1]);
    p_ad2 = (char*)&(ad[i]);

    for(uint j = 0; j <= sizeof(accdata)-1; j++)
      p_ad1[j] = p_ad2[j];
  }

  //make sure the top account is cleared
  p_ad2 = (char*)&(ad[highcount]);
  for(uint j = 0; j <= sizeof(accdata)-1; j++)
    p_ad2[j] = '\0';

  highcount--;
  if(caccount > highcount)
    caccount = highcount;

  return true;
}


int PPPData::newaccount() {

  if(highcount >= MAX_ACCOUNTS)
    return -1;

  highcount++;
  caccount = highcount;

  // first clear out the record, just to be safe
  char *p_ad = (char*)&(ad[caccount]);
  for(uint i = 0; i <= sizeof(accdata)-1; i++)
    p_ad[i] = '\0';

  setpppdArgumentDefaults();

  return caccount;
}

int PPPData::copyaccount(int i) {

  if(highcount >= MAX_ACCOUNTS)
    return -1;

  highcount++;
  caccount = highcount;

  ad[highcount] = ad [i];
  QString tempname;
  tempname = ad[i].accname;
  tempname += "_copy";
  if(tempname.size() > ACCNAME_SIZE)
    tempname = tempname.left(ACCNAME_SIZE -1);
  strncpy(ad[highcount].accname, tempname.data(),tempname.size());

  return caccount;
}


const char* PPPData::accname() {
  return ad[caccount].accname;
}


void PPPData::setAccname( const char *n ) {
  if(caccount >= 0) {
    //change the default account name along with the account name
    if(strncmp(ad[caccount].accname, gd.defaultaccount, ACCNAME_SIZE) == 0)
      setDefaultAccount(n);

    strncpy(ad[caccount].accname, n, ACCNAME_SIZE);
    ad[caccount].accname[ACCNAME_SIZE] = '\0';
  }
}


const char* PPPData::phonenumber() {
  return ad[caccount].phonenumber;
}


void PPPData::setPhonenumber( const char *n ) {
  if(caccount >= 0) {
    strncpy(ad[caccount].phonenumber, n, PHONENUMBER_SIZE);
    ad[caccount].phonenumber[PHONENUMBER_SIZE] = '\0';
  }
}


const char* PPPData::speed() {
  return ad[caccount].speed;
}


void PPPData::setSpeed( const char *n ) {
  if(caccount >= 0) {
    strncpy(ad[caccount].speed, n, SPEED_SIZE);
    ad[caccount].speed[SPEED_SIZE] = '\0';
  }
}


const char* PPPData::command() {
  return ad[caccount].command;
}


void PPPData::setCommand( const char *n ) {
  if(caccount >= 0) {
    strncpy(ad[caccount].command, n, COMMAND_SIZE);
    ad[caccount].command[COMMAND_SIZE] = '\0';
  }
}


const char* PPPData::ipaddr() {
  return ad[caccount].ipaddr;
}


void PPPData::setIpaddr( const char *n ) {
  if(caccount >= 0) {
    strncpy(ad[caccount].ipaddr, n, IPADDR_SIZE);
    ad[caccount].ipaddr[IPADDR_SIZE] = '\0';
  } 
}


const char* PPPData::subnetmask() {
  return ad[caccount].subnetmask;
}


void PPPData::setSubnetmask( const char *n ) {
  if(caccount >= 0) {
    strncpy(ad[caccount].subnetmask, n, IPADDR_SIZE);
    ad[caccount].subnetmask[IPADDR_SIZE] = '\0';
  }
}


const bool PPPData::autoname(){
  
  bool result;

  if(strcmp(ad[caccount].autoname,"AUTOCONFIG_HOSTNAME_ON") == 0 ){
    result = true;
  }
  else{
    result = false;
  }

  return result;

};

void PPPData::setAutoname(bool set){
  
  if(set){
    strncpy(ad[caccount].autoname,"AUTOCONFIG_HOSTNAME_ON",ARGUMENT_SIZE);
  }else{
    strncpy(ad[caccount].autoname,"AUTOCONFIG_HOSTNAME_OFF",ARGUMENT_SIZE);
  }
}


void PPPData::setAcctEnabled(bool _set){

 if(_set){

   strcpy(ad[caccount].accounting_enabled,"TRUE");

 }
 else{	
    strcpy(ad[caccount].accounting_enabled,"FALSE");
 }

 return;
}


const bool PPPData::AcctEnabled(){
  
  bool result;
  if(strcmp(ad[caccount].accounting_enabled,"TRUE") == 0 ){
    result = true;
  }
  else{
    result = false;
  }

  return result;


}
const char* PPPData::gateway() {
  return ad[caccount].gateway;
}


void PPPData::setGateway( const char *n ) {
  if(caccount >= 0) {
    strncpy(ad[caccount].gateway, n, IPADDR_SIZE);
    ad[caccount].gateway[IPADDR_SIZE] = '\0';
  }
}


const bool PPPData::defaultroute(){
  
  bool result;

  if(strcmp(ad[caccount].defaultroute,"DEFAULTROUTE_ON") == 0 ){
    result = true;
  }
  else{
    result = false;
  }

  return result;

};

void PPPData::setDefaultroute(bool set){
  
  if(set){
    strncpy(ad[caccount].defaultroute,"DEFAULTROUTE_ON",ARGUMENT_SIZE);
  }else{
    strncpy(ad[caccount].defaultroute,"DEFAULTROUTE_OFF",ARGUMENT_SIZE);
  }
}


const char* PPPData::dns( int i ) {
  if(i >= 0 && i <= MAX_DNS_ENTRIES-1)
    return ad[caccount].dns[i];
  else
    return NULL;
}


void PPPData::setDns(int i, const char *n) {
  if(i >= 0 && i <= MAX_DNS_ENTRIES-1) {
    strncpy(ad[caccount].dns[i], n, IPADDR_SIZE);
    ad[caccount].dns[i][IPADDR_SIZE] = '\0';
  }

  for(int j=i+1; j <= MAX_DNS_ENTRIES-1; j++) {
    strncpy(ad[caccount].dns[j], "", IPADDR_SIZE);
    ad[caccount].dns[j][IPADDR_SIZE] = '\0';
  }
}

void PPPData::setExDNSDisabled(bool _set){

  if(_set){
    strcpy(ad[caccount].exdnsdisabled,"TRUE");
  }
  else{	
    strcpy(ad[caccount].exdnsdisabled,"FALSE");
  }

  return;
}


const bool PPPData::exDNSDisabled(){
  
  bool result;
  
  if(strcmp(ad[caccount].exdnsdisabled,"TRUE") == 0 ){
    result = true;
  }
  else{
    result = false;
  }

  return result;
}

const char* PPPData::domain() {
  return ad[caccount].domain;
}

void PPPData::setDomain( const char *n ) {
  if(caccount >= 0) {
    strncpy(ad[caccount].domain, n, DOMAIN_SIZE);
    ad[caccount].domain[DOMAIN_SIZE] = '\0';
  }
}


const char* PPPData::scriptType(int i) {
  if(i >= 0 && i <= MAX_SCRIPT_ENTRIES-1) {
    return ad[caccount].stype[i];
  }
  else
    return NULL;
}


void PPPData::setScriptType(int i, const char *n) {
  if(i >= 0 && i <= MAX_SCRIPT_ENTRIES-1) {
    strncpy(ad[caccount].stype[i], n, SCRIPT_TYPE_SIZE);
    ad[caccount].stype[i][SCRIPT_TYPE_SIZE] = '\0';
  }

  for(int j=i+1; j <= MAX_SCRIPT_ENTRIES-1; j++) {
    strncpy(ad[caccount].stype[j], "", SCRIPT_TYPE_SIZE);
    ad[caccount].stype[j][SCRIPT_TYPE_SIZE] = '\0';
  }
}


const char* PPPData::script(int i) {
  if(i >= 0 && i <= MAX_SCRIPT_ENTRIES-1) {
    return ad[caccount].sdata[i];
  }
  else
    return NULL;
}


void PPPData::setScript(int i, const char *n) {
  if(i >= 0 && i <= MAX_SCRIPT_ENTRIES-1) {
    strncpy(ad[caccount].sdata[i], n, SCRIPT_SIZE);
    ad[caccount].sdata[i][SCRIPT_SIZE] = '\0';
  }

  for(int j=i+1; j <= MAX_SCRIPT_ENTRIES-1; j++) {
    strncpy(ad[caccount].sdata[j], "", SCRIPT_SIZE);
    ad[caccount].sdata[j][SCRIPT_SIZE] = '\0';
  }
}


const char *PPPData::accountingFile() {
  return ad[caccount].accounting;
}

void PPPData::setAccountingFile(const char *s) {
  strncpy(ad[caccount].accounting, s, ACCOUNTING_SIZE);
}

const char* PPPData::pppdArgument(int i) {
  if(i >= 0 && i <= MAX_PPPD_ARGUMENTS-1) {
    return ad[caccount].pppdarguments[i];
  }
  return NULL;
}

void PPPData::setpppdArgumentDefaults() {
  strcpy(ad[caccount].pppdarguments[0], "-detach");
/*  strcpy(ad[caccount].pppdarguments[1], "defaultroute");*/

  for(int i=1; i <= MAX_PPPD_ARGUMENTS-1; i++)
    strcpy(ad[caccount].pppdarguments[i], "");
  strcpy(ad[caccount].accounting_enabled,"FALSE");
}

void PPPData::setpppdArgument(int i, const char *n) {
  if(i >= 0 && i <= MAX_PPPD_ARGUMENTS-1) {
    strncpy(ad[caccount].pppdarguments[i], n, ARGUMENT_SIZE);
    ad[caccount].pppdarguments[i][ARGUMENT_SIZE] = '\0';
  }

  for(int j=i+1; j <= MAX_PPPD_ARGUMENTS-1; j++) {
    strncpy(ad[caccount].pppdarguments[j], "", ARGUMENT_SIZE);
    ad[caccount].pppdarguments[j][ARGUMENT_SIZE] = '\0';
  }
}


//
//functions to change/set the child pppd process info
//

pid_t PPPData::pppdpid() {
  return pppdprocessid;
}


void PPPData::setpppdpid(pid_t id) {
  pppdprocessid = id;
}
