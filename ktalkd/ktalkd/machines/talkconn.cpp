/*
 * Copyright (c) 1983 Regents of the University of California, (c) 1998 David Faure
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

/*
 * This file handles haggling with the various talk daemons to
 * get a socket to talk to. sockt is opened and connected in
 * the progress
 */

#include "talkconn.h"

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <termios.h>
#include <syslog.h>
#include <netdb.h>

#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#ifdef TIME_WITH_SYS_TIME
#include <time.h>
#endif
#else
#include <time.h>
#endif
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#include "../print.h"
#include "../defs.h" // for hostname

#ifndef SOMAXCONN
#warning SOMAXCONN not defined in your headers
#define SOMAXCONN 5
#endif

TalkConnection::TalkConnection(struct in_addr caller_machine_addr, 
                               char * r_name,
                               char * local_user,
                               ProtocolType _protocol) : 
   protocol(_protocol), his_machine_addr(caller_machine_addr)
{
    /* look up the address of the local host */
    struct hostent *hp = gethostbyname(Options::hostname);
    if (!hp) {
        syslog(LOG_ERR, "GetHostByName failed for %s.",Options::hostname);
        exit(-1);
    }
    memcpy(&my_machine_addr, hp->h_addr, hp->h_length);

    if (protocol == noProtocol) {
        /*        CheckProtocol checkProtocol(my_machine_addr);
        if (checkProtocol.CheckHost(caller_machine_addr))
            protocol=checkProtocol.getProtocol();
        else
            p_error("No protocol found. Aborting."); */
        protocol = ntalkProtocol; // HACK FOR THE MOMENT
    }

    if (protocol == ntalkProtocol) {
        new_msg.vers = TALK_VERSION;
        new_msg.pid = htonl (getpid ()); // is it necessary ?
        *new_msg.r_tty = '\0';
    } else /* protocol == talkProtocol */ {
        old_msg.pid = htonl (getpid ()); // is it necessary ?
        *old_msg.r_tty = '\0';
    }
    strncpy(new_msg.l_name,local_user,NEW_NAME_SIZE);
    strncpy(new_msg.r_name,r_name,NEW_NAME_SIZE);
    strncpy(old_msg.l_name,local_user,OLD_NAME_SIZE);
    strncpy(old_msg.r_name,r_name,OLD_NAME_SIZE);

    /* find the server's port */
    struct servent * sp = 
        getservbyname((protocol == talkProtocol) ? "talk" : "ntalk", "udp");
    if (sp == 0)
        p_error("Service is not registered.\n"); /* quit */

    daemon_port = sp->s_port; // already in network byte order
    // message2("Daemon port found : %d",htons(daemon_port));

    ctl_sockt = -1; // Note that it is not initialized

#ifdef FvK
    /* I'm not responsible for this patch. I include it because some people
       asked me to. D.F. */
    /* July 1998 : the answering machine has just been completely rewritten.
       This patch has been copied here, but is completely UNTESTED and isn't
       even supposed to compile. Send me necessary adaptations... */
    /* <FvK> patch : (used to find the correct interface, when other than eth0). */
    struct sockaddr_in foo;
    int sock, i;
    /* If socket fails, code will see it. */
    sock = socket(AF_INET, SOCK_DGRAM, 0);

    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = his_machine_addr.s_addr;
    sin.sin_port = sp->s_port;

    /*  Now here is the trick.  We connect to the other side. */
    if ((sock >= 0) &&
        (connect(sock, (struct sockaddr *) &sin, sizeof(sin)) == 0)) {
        /* Bingo.  Now fetch the address. */
        foo = sin;
        i = sizeof(foo);
        if (getsockname(sock, (struct sockaddr *) &foo, &i) == 0) {
            my_machine_addr = foo.sin_addr;
        }
    }
    /* Loose the socket. */
    close(sock);
    /* </FvK> */
#endif
}

TalkConnection::~TalkConnection()
{
    if (ctl_sockt != -1)
        close_sockets();
}

void TalkConnection::open_sockets()
{
    /* Control (daemon) address. */
    struct sockaddr_in ctl_addr;
    /* My address. */
    struct sockaddr_in my_addr;

    /* open the ctl socket */
    ksize_t length;
    ctl_addr.sin_family = AF_INET;
    ctl_addr.sin_addr = my_machine_addr;
    ctl_addr.sin_port = 0;
    ctl_sockt = socket(AF_INET, SOCK_DGRAM, 0);
    if (ctl_sockt <= 0)
        p_error("Bad socket");
    if (bind(ctl_sockt,(struct sockaddr *)&ctl_addr, sizeof(ctl_addr)) != 0)
        p_error("Couldn't bind to control socket");
    length = sizeof(ctl_addr);
    if (getsockname(ctl_sockt, (struct sockaddr *)&ctl_addr, &length) == -1)
        p_error("Bad address for ctl socket");

    /* open the text socket */
    my_addr.sin_family = AF_INET;
    my_addr.sin_addr = my_machine_addr;
    my_addr.sin_port = 0;
    sockt = socket(AF_INET, SOCK_STREAM, 0);
    if (sockt <= 0)
        p_error("Bad socket");
    if (bind(sockt, (struct sockaddr *)&my_addr, sizeof(my_addr)) != 0)
        p_error("Binding local socket");
    length = sizeof(my_addr);
    if (getsockname(sockt, (struct sockaddr *)&my_addr, &length) == -1)
        p_error("Bad address for socket");

    /* save the result */

    set_addr((const struct sockaddr *)&my_addr);
    set_ctl_addr((const struct sockaddr *)&ctl_addr);
}

void TalkConnection::set_addr(const struct sockaddr * addr)
{
    old_msg.addr = *addr;
    old_msg.addr.sa_family = htons(AF_INET);
    new_msg.addr = *addr;
    new_msg.addr.sa_family = htons(AF_INET);
}

void TalkConnection::set_ctl_addr(const struct sockaddr * ctl_addr)
{
    old_msg.ctl_addr = *ctl_addr;
    old_msg.ctl_addr.sa_family = htons(AF_INET);
    new_msg.ctl_addr = *ctl_addr;
    new_msg.ctl_addr.sa_family = htons(AF_INET);
}

void TalkConnection::close_sockets()
{
    close(sockt);
    close(ctl_sockt);
}

#define CTL_WAIT 2	/* time to wait for a response, in seconds */

/*
 * SOCKDGRAM is unreliable, so we must repeat messages if we have
 * not received an acknowledgement within a reasonable amount
 * of time
 */
void TalkConnection::ctl_transact(int type, int id_num)
{
    fd_set read_mask, ctl_mask;
    int nready=0, cc, size, ok=0;
    struct timeval wait;
    struct sockaddr_in daemon_addr;
    char * msg;

    if (protocol == talkProtocol) {
        old_msg.type = type;
        old_msg.id_num = htonl(id_num);
        msg = (char *)&old_msg;
        size = sizeof old_msg;
    } else {
        new_msg.type = type;
        new_msg.id_num = htonl(id_num);
        msg = (char *)&new_msg;
        size = sizeof new_msg;
        print_request("ctl_transact: ",&new_msg);
    }

    daemon_addr.sin_family = AF_INET;
    daemon_addr.sin_addr = his_machine_addr;
    daemon_addr.sin_port = daemon_port;
    FD_ZERO(&ctl_mask);
    FD_SET(ctl_sockt, &ctl_mask);

    /* Keep sending the message until a response of
     * the proper type is obtained.
     */
    do {
        /* resend message until a response is obtained */
        do {
            cc = sendto(ctl_sockt, msg, size, 0,
                        (struct sockaddr *)&daemon_addr,
                        sizeof (daemon_addr));
            if (cc != size) {
                if (errno == EINTR)
                    continue;
                p_error("Error on write to talk daemon");
            }
            read_mask = ctl_mask;
            wait.tv_sec = CTL_WAIT;
            wait.tv_usec = 0;
            nready = ::select(ctl_sockt+1, &read_mask, 0, 0, &wait);
            if (nready < 0) {
                if (errno == EINTR)
                    continue;
                p_error("Error waiting for daemon response");
            }
            if (nready == 0) message("select returned 0 ! ");
        } while (nready == 0);
        /*
         * Keep reading while there are queued messages 
         * (this is not necessary, it just saves extra
         * request/acknowledgements being sent)
         */
        do {
            if (protocol == talkProtocol)
                cc = ::recv(ctl_sockt, (char *)&old_resp, sizeof (old_resp), 0);
            else
                cc = ::recv(ctl_sockt, (char *)&new_resp, sizeof (new_resp), 0);
            if (cc < 0) {
                if (errno == EINTR)
                    continue;
                p_error("Error on read from talk daemon");
            }
            read_mask = ctl_mask;
            /* an immediate poll */
            timerclear(&wait);
            nready = ::select(ctl_sockt+1, &read_mask, 0, 0, &wait);
            if (protocol == talkProtocol) ok = (old_resp.type == type);
            else ok = ((new_resp.type == type) && (new_resp.vers == TALK_VERSION));
        } while (nready > 0 && (!ok));
    } while (!ok);
    if (protocol == talkProtocol) {
        old_resp.id_num = ntohl(old_resp.id_num);
        old_resp.addr.sa_family = ntohs(old_resp.addr.sa_family);
    } else {
        new_resp.id_num = ntohl(new_resp.id_num);
        new_resp.addr.sa_family = ntohs(new_resp.addr.sa_family);
    }
}

/** Look for an invitation on remote machine */
int TalkConnection::look_for_invite(int mandatory)
{
    /* Check for invitation on caller's machine */
    ctl_transact(LOOK_UP, 0);

    char answer;
    int id_num;
    getResponseItems(&answer, &id_num, &lookup_addr);

    if (!mandatory) return 0;

    /* the switch is for later options, such as multiple invitations */
    switch (answer) {

	case SUCCESS:
            new_msg.id_num = htonl(id_num);
            old_msg.id_num = htonl(id_num);
            message("TalkConnection::look_for_invite : got SUCCESS");
            if (lookup_addr.sa_family != AF_INET)
                p_error("Response uses invalid network address");
            return (1);

	default:
            /* there wasn't an invitation waiting for us */
            message("TalkConnection::look_for_invite : didn't get SUCCESS");
            return (0);
    }
}

/** Prepare to accept a connection from another talk client */
void TalkConnection::listen()
{
    if (::listen(sockt, SOMAXCONN) != 0)
        p_error("Error on attempt to listen for caller");
}

/** Accept a connection from another talk client */
int TalkConnection::accept()
{
    int accept_sockt;
    while ((accept_sockt = ::accept(sockt, 0, 0)) < 0) {
        if (errno == EINTR)
            continue;
        p_error("Unable to connect with your party");
    }
    ::close(sockt);
    sockt = accept_sockt;
    return sockt;
}

/** Connect to another talk client. */
int TalkConnection::connect()
{
    message("Waiting to connect");
    do {
        errno = 0;
        if (::connect(sockt, &lookup_addr, sizeof (struct sockaddr)) != -1)
            return 1;
    } while (errno == EINTR);
    if (errno == ECONNREFUSED) {
        /*
         * The caller gave up, but his invitation somehow
         * was not cleared. Clear it and initiate an 
         * invitation. (We know there are no newer invitations,
         * the talkd works LIFO.)
         */
        message("ECONNREFUSED");
        ctl_transact(DELETE, 0);
        close_sockets();
        open_sockets();
        return 0;
    }
    p_error("Unable to connect with initiator");
    /*NOTREACHED*/
    return 0;
}

/** Trade edit characters with the other talk. By agreement
 * the first three characters each talk transmits after
 * connection are the three edit characters. 
 * A normal talk client uses tcgetattr() to get the chars,
 * but the daemon isn't connected to a terminal, so we can't call it.
 * We just send dummy chars, to disable control chars. */
void TalkConnection::set_edit_chars()
{
	char buf[3];
        int cc;
        buf[0] = buf[1] = buf[2] = 0xff;
        /* Write our config to the caller */
	cc = write(sockt, buf, sizeof(buf));
	if (cc != sizeof(buf) )
		p_error("Lost the connection");
        /* Read the caller configuration */
	cc = read(sockt, buf, sizeof(buf));
	if (cc != sizeof(buf) )
		p_error("Lost the connection");
	char_erase = buf[0]; // store it in TalkConnection
}

void TalkConnection::write_banner(char * banner)
{ /* writes the message 'banner', null-terminated */
    int count = strlen(banner);
    int nbsent;
    char * str = banner;
    /*    message2("Count : %d.",count); */
    while (count>0) {
        /* let's send 16 -bytes-max packets */
         if (count>=16) nbsent = write(sockt,str,16);
                  else nbsent = write(sockt,str,count);
        count -= nbsent;
        str += nbsent;
        fsync(sockt);
    }
    write(sockt,"\n",1);
}

void TalkConnection::getResponseItems(char * answer, int * id_num, struct sockaddr * addr) {
    if (protocol == talkProtocol) {
        if (answer) *answer = old_resp.answer;
        if (id_num) *id_num = old_resp.id_num;
        if (addr) *addr = old_resp.addr;
    } else {
        if (answer) *answer = new_resp.answer;
        if (id_num) *id_num = new_resp.id_num;
        if (addr) *addr = new_resp.addr;
    }
}

/** p_error prints the system error message in the log
 * and then exits. */
void TalkConnection::p_error(const char *str)
{
    syslog(LOG_ERR,str);
    _exit(0);
}
