/** kotalkd process.c **/

#include "../ktalkd/includ.h"
#include <sys/param.h>
#ifdef HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#include <netdb.h>
#include <errno.h>
#include <syslog.h>
#include <stdio.h>
#include <string.h>
#include <utmp.h>
#include <pwd.h>
#include <stdlib.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <fcntl.h>

#include "print.h"

extern int debug_mode;

/* Local inet address */
static struct  in_addr my_machine_addr;
/* Control (daemon) address. */
static struct sockaddr_in ctl_addr;
/* Port number of the talk daemon */
static int daemon_port;
/* NEW_CTL_MSG used to talk to our ntalk daemon */
static NEW_CTL_MSG new_msg;
/* NEW_CTL_RESPONSE used to talk to our ntalk daemon */
static NEW_CTL_RESPONSE new_resp;
/* Ctl socket */
static int ctl_sockt;

/* Opens a connection to the local ntalk daemon */
void open_local_socket(char * hostname)
{
    struct hostent *hp;
    ksize_t length;
    
    /* find the server's port */
    struct servent * sp = getservbyname("ntalk", "udp");
    if (sp == 0) {
        syslog(LOG_ERR, "talk: ntalk/udp: service is not registered.");
        exit(-1);
    }
    daemon_port = sp->s_port; /* already in network byte order */

    /* look up the address of the local host */
    hp = gethostbyname(hostname);
    if (!hp) {
        syslog(LOG_ERR, "GetHostByName failed.");
        exit(-1);
    }
    memcpy(&my_machine_addr, hp->h_addr, hp->h_length);

    /* open the ctl socket */
    ctl_addr.sin_family = AF_INET;
    ctl_addr.sin_addr = my_machine_addr;
    ctl_addr.sin_port = 0;
    ctl_sockt = socket(AF_INET, SOCK_DGRAM, 0);
    if (ctl_sockt <= 0) {
        syslog(LOG_ERR, "Bad socket");
        exit(-1);
    }
    if (bind(ctl_sockt,(struct sockaddr *)&ctl_addr, sizeof(ctl_addr)) != 0) {
        syslog(LOG_ERR, "Couldn't bind to control socket");
        exit(-1);
    }
    length = sizeof(ctl_addr);
    if (getsockname(ctl_sockt, (struct sockaddr *)&ctl_addr, &length) == -1) {
        syslog(LOG_ERR, "Bad address for control socket");
        exit(-1);        
    }

    /* Fill the msg structure (ntalk protocol) */
    new_msg.vers = TALK_VERSION;
    new_msg.ctl_addr = *(struct sockaddr *)&ctl_addr;
    new_msg.ctl_addr.sa_family = htons(AF_INET);

}

void close_local_socket()
{
    close(ctl_sockt);
}

#define CTL_WAIT 2	/* time to wait for a response, in seconds */

/** SOCKDGRAM is unreliable, so we must repeat messages if we have
 * not received an acknowledgement within a reasonable amount
 * of time */
void ctl_transact(int type)
{
    fd_set read_mask, ctl_mask;
    int nready=0, cc;
    struct timeval wait;
    struct sockaddr_in daemon_addr;

    daemon_addr.sin_family = AF_INET;
    daemon_addr.sin_addr = my_machine_addr;
    daemon_addr.sin_port = daemon_port;
    FD_ZERO(&ctl_mask);
    FD_SET(ctl_sockt, &ctl_mask);

    /* Keep sending the message until a response of
     * the proper type is obtained.
     */
    do {
        /* resend message until a response is obtained */
        do {
            cc = sendto(ctl_sockt, (char *)&new_msg, sizeof(new_msg), 0,
                        (struct sockaddr *)&daemon_addr,
                        sizeof (daemon_addr));
            if (cc != sizeof(new_msg)) {
                if (errno == EINTR)
                    continue;
                syslog(LOG_ERR,"Error on write to talk daemon");
            }
            read_mask = ctl_mask;
            wait.tv_sec = CTL_WAIT;
            wait.tv_usec = 0;
            nready = select(ctl_sockt+1, &read_mask, 0, 0, &wait);
            if (nready < 0) {
                if (errno == EINTR)
                    continue;
                syslog(LOG_ERR,"Error waiting for daemon response");
            }
            if (nready == 0) message("select returned 0 ! ");
        } while (nready == 0);
        /*
         * Keep reading while there are queued messages 
         * (this is not necessary, it just saves extra
         * request/acknowledgements being sent)
         */
        do {
            cc = recv(ctl_sockt, (char *)&new_resp, sizeof (new_resp), 0);
            if (cc < 0) {
                if (errno == EINTR)
                    continue;
                syslog(LOG_ERR,"Error on read from talk daemon");
            }
            read_mask = ctl_mask;
            /* an immediate poll */
            timerclear(&wait);
            nready = select(ctl_sockt+1, &read_mask, 0, 0, &wait);
        } while (nready > 0 && (new_resp.vers != TALK_VERSION || 
                                new_resp.type != type));
    } while (new_resp.vers != TALK_VERSION || 
             new_resp.type != type);
}

void  process_request(register OLD_CTL_MSG *mp, register OLD_CTL_RESPONSE *rp)
{
	if (debug_mode)
            print_old_request("process_request", mp);

        /* Process mp->ctl_addr (used to respond) */
        mp->ctl_addr.sa_family = ntohs(mp->ctl_addr.sa_family);
        if (mp->ctl_addr.sa_family != AF_INET) {
                syslog(LOG_WARNING, "Bad control address, family %d",
                    mp->ctl_addr.sa_family);
                rp->answer = BADCTLADDR;
                return ;
        }
        
        /* Cut out names (to prevent problems). Max length = 8 + 1 ('\0') */
        mp->l_name[OLD_NAME_SIZE-1] = '\0';
        mp->r_name[OLD_NAME_SIZE-1] = '\0';
        mp->r_tty[TTY_SIZE-1] = '\0';

        /* Create a NEW_CTL_MSG structure from the OLD_CTL_MSG */
        new_msg.type = mp->type;
        new_msg.id_num = mp->id_num;
        new_msg.addr = mp->addr;
        new_msg.pid = mp->pid;
        strcpy(new_msg.l_name, mp->l_name); /* cut before, no need to strncpy. */
        strcpy(new_msg.r_name, mp->r_name);
        strcpy(new_msg.r_tty, mp->r_tty);
        
        /* Now send request to local ktalkd */
        ctl_transact(mp->type);

        /* And fill our response from the one we got */
        rp->type = new_resp.type;
        rp->answer = new_resp.answer;
        rp->id_num = new_resp.id_num;
        rp->addr = new_resp.addr;
}
