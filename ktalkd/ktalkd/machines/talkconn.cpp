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
#include "../process.h" // for prepare_response()

#ifndef VWERASE
#ifdef VWERSE
 /* for AIX */
#define VWERASE VWERSE
#else
#warning VWERASE not defined. Defining as 14.
#define VWERASE 14
#endif
#endif

void TalkConnection::init()
{
#ifndef OTALK
    msg.vers = TALK_VERSION;
#endif
    msg.pid = htonl (getpid ()); // is it necessary ?
    *msg.r_tty = '\0';

    /* find the server's port */
#ifdef OTALK
    struct servent * sp = getservbyname("talk", "udp");
#else
    struct servent * sp = getservbyname("ntalk", "udp");
#endif
    if (sp == 0) {
        char buff[50];
        sprintf(buff, "talk: %s/%s: service is not registered.\n",
                "ntalk", "udp");
        p_error(buff); /* quit */
    }
    daemon_port = sp->s_port; // already in network byte order
    // message2("Daemon port found : %d",htons(daemon_port));

    /* look up the address of the local host */
    struct hostent *hp = gethostbyname(hostname);
    if (!hp) {
        syslog(LOG_ERR, "GetHostByName failed.");
        exit(-1);
    }
    memcpy(&my_machine_addr, hp->h_addr, hp->h_length);

    ctl_sockt = -1; // Note that it is not initialized
}

TalkConnection::TalkConnection(struct in_addr caller_machine_addr, 
                               char * r_name,
                               char * local_user)
{
    init();

    strncpy(msg.l_name,local_user,NAME_SIZE);
    strncpy(msg.r_name,r_name,NAME_SIZE);

    his_machine_addr = caller_machine_addr;

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
    msg.addr = *addr;
    msg.addr.sa_family = htons(AF_INET);
}

void TalkConnection::set_ctl_addr(const struct sockaddr * ctl_addr)
{
    msg.ctl_addr = *ctl_addr;
    msg.ctl_addr.sa_family = htons(AF_INET);
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
    int nready=0, cc;
    struct timeval wait;
    struct sockaddr_in daemon_addr;

    msg.type = type;
    msg.id_num = htonl(id_num);

    if (debug_mode)
      print_request("ctl_transact: ",&msg);

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
            cc = sendto(ctl_sockt, (char *)&msg, sizeof(msg), 0,
                        (struct sockaddr *)&daemon_addr,
                        sizeof (daemon_addr));
            if (cc != sizeof(msg)) {
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
            cc = ::recv(ctl_sockt, (char *)&response, sizeof (response), 0);
            if (cc < 0) {
                if (errno == EINTR)
                    continue;
                p_error("Error on read from talk daemon");
            }
            read_mask = ctl_mask;
            /* an immediate poll */
            timerclear(&wait);
            nready = ::select(ctl_sockt+1, &read_mask, 0, 0, &wait);
        } while (nready > 0 && (
#ifndef OTALK
                       response.vers != TALK_VERSION || 
#endif
                       response.type != type));
    } while (
#ifndef OTALK
                       response.vers != TALK_VERSION || 
#endif
                       response.type != type);
    response.id_num = ntohl(response.id_num);
    response.addr.sa_family = ntohs(response.addr.sa_family);
}

/** Look for an invitation on remote machine */
int TalkConnection::look_for_invite()
{
    /* Check for invitation on caller's machine */

    ctl_transact(LOOK_UP, 0);

    /* the switch is for later options, such as multiple invitations */
    switch (response.answer) {

	case SUCCESS:
            msg.id_num = htonl(response.id_num);
            message("TalkConnection::look_for_invite : got SUCCESS");
            if (response.addr.sa_family != AF_INET)
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
    if (::listen(sockt, 5) != 0)
        p_error("Error on attempt to listen for caller");
}

/** Accept a connection from another talk client */
int TalkConnection::accept()
{
    int new_sockt;
    while ((new_sockt = ::accept(sockt, 0, 0)) < 0) {
        if (errno == EINTR)
            continue;
        p_error("Unable to connect with your party");
    }
    ::close(sockt);
    sockt = new_sockt;
    return sockt;
}

/** Connect to another talk client. */
int TalkConnection::connect()
{
    message("Waiting to connect");
    do {
        errno = 0;
        if (::connect(sockt, (struct sockaddr *) &response.addr, sizeof (struct sockaddr)) != -1)
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
        /*        message2("Count rest: %d.",count);
                  message2("  [ Nbsent : %d.",nbsent); */
        /* sleep(1); ONLY to debug !! */
        fsync(sockt);
    }
    write(sockt,"\n",1);
}

/** p_error prints the system error message in the log
 * and then exits. */
void TalkConnection::p_error(const char *str)
{
    syslog(LOG_ERR,str);
    _exit(0);
}
