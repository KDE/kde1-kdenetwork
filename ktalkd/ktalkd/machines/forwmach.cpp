/*
 * Copyright (c) 1983  Regents of the University of California, (c) 1998 David Faure
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
 *
 */

#include "../includ.h"
#include "forwmach.h"
#include <stdio.h>
#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#endif
#ifdef HAVE_SYS_TYPES_H 
#include <sys/types.h>
#endif
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <string.h>
#include <syslog.h>
#include <errno.h>
#include <netdb.h>
#include <malloc.h>
#include "../print.h"
#include "../defs.h"
#include "../table.h"
#include "../readconf.h"
#include "../process.h"

ForwMachine::ForwMachine(const NEW_CTL_MSG * mp,
                         char * forward,
                         char * _forwardMethod)
{
    // Store callee as 'local_user'
    strncpy(local_user, mp->r_name, NAME_SIZE);
         // -1 is to be sure to have a '\0' at the end
    // Store caller's name
    strncpy(caller_username, mp->l_name, NAME_SIZE);

    // Forward method : from string to enumerate
    if (!strcmp(_forwardMethod,"FWA"))
        forwardMethod = FWA;
    else if (!strcmp(_forwardMethod,"FWR"))
        forwardMethod = FWR;
    else if (!strcmp(_forwardMethod,"FWT"))
        forwardMethod = FWT;
    else syslog(LOG_ERR,"Unknown forward method : %s",_forwardMethod);

    // Get answerer machine address and username
    if (getNames(forward)) {
        message_s("-- Talking to %s",answ_user);
        message_s("-- On %s",answ_machine_name);
        // Create a new talk connection, to the answerer ...
        tcAnsw = new TalkConnection(answ_machine_addr,
                                    answ_user,
                                    // from caller's username
                                    (char *) mp->l_name);
        tcAnsw->open_sockets();        
        // and from here if FWT or ...
        if (forwardMethod != FWT) {
            //from the caller if FWA or FWR
            tcAnsw->set_addr(&mp->addr);
            // but WE DO NOT CHANGE THE ctl_addr, we want the response !!
        }
        // Store caller's ctl_addr (to respond to its announce)
        caller_ctl_addr = mp->ctl_addr;
        // And his machine addr (to send a LOOK_UP)
        caller_machine_addr = ((struct sockaddr_in * )(&mp->ctl_addr))->sin_addr;
    }
}

ForwMachine::~ForwMachine()
{
    free(answ_machine_name);
    delete tcAnsw;
}

/** Fills private fields from forward
 * @param forward user@host to forward the talk */
int ForwMachine::getNames(char * forward)
{ /* taken from old get_names.c */
    register char *cp;

    /* strip out the machine name of the target */
    for (cp = forward; *cp && !index("@:!.", *cp); cp++)
        ;
    if (*cp == '\0') {
        /* this is a forward to a local user */
        strncpy(answ_user, forward, NAME_SIZE);
        answ_machine_name = strdup(hostname); /* set by the daemon */
    } else {
        if (*cp == '@') {
            /* user@host */
            *cp++ = '\0';
            strncpy(answ_user, forward, NAME_SIZE);
            answ_machine_name = strdup(cp);
        } else {
            /* host.user or host!user or host:user */
            *cp++ = '\0';
            strncpy(answ_user, cp, NAME_SIZE);
            answ_machine_name = strdup(forward);
        }
    }

    struct hostent * hp = gethostbyname(answ_machine_name);
    if (!hp) {
        syslog(LOG_ERR, "%s: %m", answ_machine_name);
        return 0;
    }
    memcpy(&answ_machine_addr, hp->h_addr, hp->h_length);
    return 1;
}

int ForwMachine::isLookupForMe(const NEW_CTL_MSG * mp)
{
    /** We want to check if this LOOK_UP concerns this forwmachine.
     * It does if :
        mp->l_name = answ_user
        mp->r_name = caller_username
        mp->addr.sin_addr is 0.0.0.0, can't be tested ...
        mp->ctl_addr.sin_addr could be tested but how ?
    */
    if (debug_mode)
    { 
        syslog(LOG_DEBUG,"-- mp->l_name : '%s' answerer : '%s' mp->r_name : '%s' caller : '%s'",
               mp->l_name, answ_user, mp->r_name, caller_username);
    }

    return ( (!strcmp(mp->l_name, answ_user)) &&
             (!strcmp(mp->r_name, caller_username)) );

}

char * ForwMachine::findMatch(NEW_CTL_MSG * mp)
{
    /** Check if there is a forwarding machine on this host,
     * matching answerer = r_name and caller = l_name
     * Then return the initial callee (local_user), to display in ktalkdlg 
     * This is used by local forwards, and therefore also if NEUBehaviour=1 */
    if ((!strcmp(mp->l_name, caller_username)) &&
        (!strcmp(mp->r_name, answ_user)) )
        return local_user;
    return NULL;
}

int ForwMachine::transmit_chars(int sockt1, int sockt2, unsigned char * buf)
{
    int nb = read(sockt1, buf, BUFSIZ);
    if (nb <= 0) {
        return 0; // finished.
    }
    write(sockt2, buf, nb);
    if ((nb <= 0) && (errno != EINTR)) {
        syslog(LOG_ERR,"Unexpected error in write to socket");
    }
    return nb;
}

void ForwMachine::connect_FWT(TalkConnection * tcCaller)
{
    /** FWT : This is the method in which we take the connection to both
     * clients and send each character received from one side to the other
     * side. This allows to pass a firewall for instance. */
    message_s("-- connect_FWT : Waiting for connection from Answerer (%s)", answ_user);
    if (tcAnsw->accept())
    {
        message_s("-- connect_FWT : Trying to connect to Caller (%s)",caller_username);
        if (tcCaller->connect())
        {
            message_s("-- connect_FWT : Connected to caller (%s)", caller_username);
            message("-- connect_FWT : Connected to both. Let's go");
            int socktC = tcCaller->get_sockt();
            int socktA = tcAnsw->get_sockt();
            int max_sockt = (socktC>socktA) ? socktC : socktA;
            unsigned char buf[BUFSIZ];
            fd_set read_mask;
            int nb;
            int nbtot = 0;
            for (;;) {
                FD_ZERO(&read_mask);
                FD_SET(socktA, &read_mask); // wait on both connections
                FD_SET(socktC, &read_mask);
                nb = select(max_sockt+1, &read_mask, NULL, NULL, NULL); // no timeout
                if (nb <= 0) {
                    if (errno == EINTR) {
                        continue;
                    }
                    /* panic, we don't know what happened */
                    TalkConnection::p_error("Unexpected error from select");
                }
                if (FD_ISSET(socktA, &read_mask)) {
                    /* There is data on sockt A */
                    nb = transmit_chars(socktA, socktC, buf);
                    if (nb==0) return ;
                }
                if (FD_ISSET(socktC, &read_mask)) {
                    /* There is data on sockt C */
                    nb = transmit_chars(socktC, socktA, buf);
                    if (nb==0) return ;
                    nbtot += nb;
                    if (nbtot == 3) // just after the 3 edit chars
                    {
                        struct hostent * hp = gethostbyaddr((char *)&caller_machine_addr,
                                                            sizeof (struct in_addr), AF_INET);
                        if (hp != (struct hostent *)0) {
                            // Write first line for answerer.
                            // i18n() missing
                            sprintf((char *)buf, "Speaking to %s@%s\n", caller_username, hp->h_name);
                            write(socktA, (char *)buf, strlen((char *)buf));
                        } else message("-- ERROR : Unable to resolve caller_machine_addr !");
                    }
                }
            } // for
        } else syslog(LOG_ERR,"-- FWT : Caller connected, but not answerer !");
    } else syslog(LOG_ERR,"-- FWT : Caller did not connect !");
}

void ForwMachine::sendResponse(const struct sockaddr target, NEW_CTL_RESPONSE * rp)
{
        int cc = sendto(0 /*talkd_sockt*/, (char *) rp,
                        sizeof (NEW_CTL_RESPONSE), 0, &target,
                        sizeof (struct sockaddr));
        if (cc != sizeof (NEW_CTL_RESPONSE))
            syslog(LOG_WARNING, "sendto: %m");
}

void ForwMachine::processLookup(const NEW_CTL_MSG * mp, NEW_CTL_RESPONSE * rp)
{
    if (fork()==0) /* let the daemon process other messages, including ours */
    {
        message_s("------------- Got LOOKUP : send it to caller (%s)", caller_username);
        // Let's send a LOOK_UP on caller's machine, to make sure he still 
        // wants to speak to the callee...
        TalkConnection * tcCaller = new TalkConnection(caller_machine_addr,
                                                       caller_username,
                                                       local_user);
        tcCaller->open_sockets();
        tcCaller->ctl_transact(LOOK_UP, 0);
        NEW_CTL_RESPONSE * resp = tcCaller->getResponse();
        message("------------- Done. Forward response to answerer");
        
        rp->answer = resp->answer;
        rp->id_num = htonl(resp->id_num);
        // Now send the response to the answerer
        if (forwardMethod == FWR)
        {        
            // with caller's addr copied in the NEW_CTL_RESPONSE (if FWR),
            // so that they can talk to each other.
            rp->addr = resp->addr;
            rp->addr.sa_family = htons(resp->addr.sa_family);
        }
        else // FWT. (FWA doesn't let us get the LOOK_UP)
        {
            // in this case, we copy in the NEW_CTL_RESPONSE the address
            // of the connection socket set up here for the answerer
            rp->addr = tcAnsw->get_addr();
            rp->addr.sa_family = htons(AF_INET);
        }
        if (debug_mode) print_response("-- => response (processLookup)", rp);
        tcAnsw->listen(); // start listening before we send the response,
        // just in case the answerer is very fast (ex: answ mach)
        sendResponse(mp->ctl_addr, rp);
        if (forwardMethod == FWT)
            connect_FWT(tcCaller);
        delete tcCaller;
        _exit(0);
    }
}

void ForwMachine::processAnnounce(NEW_CTL_RESPONSE * rp, int id_num)
{
    if (fork()==0) /* let the daemon process other messages, including ours */
    {
        // Send announce to the answerer, and wait for response
        message_s("-------------- ForwMachine : sending ANNOUNCE to %s",answ_user);
        tcAnsw->ctl_transact(ANNOUNCE, id_num);
        // Copy answer and id_num from the response struct
        message("-------------- ForwMachine : got a response");
        NEW_CTL_RESPONSE * resp = tcAnsw->getResponse();
        rp->answer = resp->answer;
        rp->id_num = htonl(resp->id_num);
        // Now send the response to the caller
        if (debug_mode) print_response("-- => response (processAnnounce)", rp);
        sendResponse(caller_ctl_addr, rp);
        _exit(0);
    }
}

extern "C" {
/** C interface for the Forwarding Machine */
char * launchForwMach(const NEW_CTL_MSG * mp, NEW_CTL_RESPONSE * rp, char * forward, char * forwardMethod)
  {
      message_s("-- launchForwMach : forward=%s",forward);
      message_s("-- launchForwMach : method=%s",forwardMethod);
      ForwMachine * fwm = new ForwMachine(mp, forward, forwardMethod);
      fwm->processAnnounce(rp, mp->id_num);
      return (char *) fwm;
  }

/** C interface for a lookup in the forwarding machines */
int forwMachProcessLookup(const NEW_CTL_MSG * mp, NEW_CTL_RESPONSE * rp)
  {
      /** This goes through the table, looking for non-NULL fwm entries.
       * After a cast to (ForwMachine *), the fwm entries allows us to
       * speak to currently availabe ForwMachines, to handle correctly 
       * this LOOK_UP */
      message("-- forwMachProcessLookup(mp,rp)");
      extern TABLE_ENTRY * table;
      TABLE_ENTRY *ptr;  
      for (ptr = table; ptr != 0L; ptr = ptr->next) {
          if (ptr->fwm != 0L)
          {
              ForwMachine * fwm = (ForwMachine *) ptr->fwm;
              if (fwm->isLookupForMe(mp)) {
                  message2("-- Found match : id %d", ptr->request.id_num);
                  fwm->processLookup(mp, rp);
                  /* We don't need the forwarding machine anymore. Delete it. */
                  delete_forwmach((char*)fwm); // delete from table
                  delete fwm;                  // delete the forwmach itself  
                  return 1;
              }
          }
      }
      return 0;
  }

/** Check if there is a forwarding machine on this machine,
 * matching answerer = r_name and caller = l_name
 * Then set callee to the initial callee, to display in ktalkdlg */
char * forwMachFindMatch(NEW_CTL_MSG * mp)
  {
      message("-- forwMachFindMatch(mp)");
      extern TABLE_ENTRY * table;
      TABLE_ENTRY *ptr;  
      char * callee;
      for (ptr = table; ptr != 0L; ptr = ptr->next) {
          if (ptr->fwm != 0L)
          {
              ForwMachine * fwm = (ForwMachine *) ptr->fwm;
              callee = fwm->findMatch(mp);
              if (callee) {
                  message2("-- Found match : id %d", ptr->request.id_num);
                  return callee;
              }
          }
      }
      return NULL;
  }

/** Cleans every remaining forwarding machine in the table.
 * Called before the _exit(0) in the daemon. */
void final_clean()
  {
      register TABLE_ENTRY *ptr;
      extern TABLE_ENTRY *table; // in table.c
      ptr = table;
      if (debug_mode)
          syslog(LOG_DEBUG, "final_clean()");
      for (ptr = table; ptr != 0L; ptr = ptr->next) {
          if (ptr->fwm != 0L)
          {
              message2("CLEAN : Found a forwarding machine to clean : id %d",ptr->request.id_num);
              delete (ForwMachine *) ptr->fwm;
          }
          message2("CLEAN : Deleting id %d", ptr->request.id_num);
          delete_entry(ptr);
      }
  }
}
