/* check_protocol.cpp */

/* This class WILL be used by talkconn.cpp when opening a connection. */

/* routines to check which talk protocol (old talk or new talk) is installed
   on a host with a given IP address. Hosts which have already been tested
   are saved in a list so that the next request can be answered immediately.
 */

#include "check_protocol.h"
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <netdb.h>

#include <errno.h>

CheckProtocol::CheckProtocol (struct in_addr _localMachineAddr)   // IP address of local machine
         : localMachineAddr(_localMachineAddr), 
           intern_protocol(noProtocol),
           in_use(0),
           result_found(0)
{
    /* find the server's ports */
    struct servent * sp = getservbyname("talk", "udp");
    if (sp == 0)
        syslog(LOG_ERR, "check_protocol: talk/udp: service is not registered.\n");
    talkDaemonPort = sp->s_port; // already in network byte order

    sp = getservbyname("ntalk", "udp");
    if (sp == 0)
        syslog(LOG_ERR, "check_protocol: ntalk/udp: service is not registered.\n");
    ntalkDaemonPort = sp->s_port; // already in network byte order

    newDefaultMsg.vers = TALK_VERSION;
    newDefaultMsg.type = oldDefaultMsg.type = LOOK_UP;
    newDefaultMsg.id_num = oldDefaultMsg.id_num = htonl (0L);
    newDefaultMsg.addr.sa_family = htons (AF_INET);
    oldDefaultMsg.addr.sa_family = htons (AF_INET);
    newDefaultMsg.ctl_addr.sa_family = htons (AF_INET);
    oldDefaultMsg.ctl_addr.sa_family = htons (AF_INET);
    newDefaultMsg.pid = oldDefaultMsg.pid = htonl (getpid ());
    strncpy (newDefaultMsg.l_name, "ktalkd", NEW_NAME_SIZE); // whatever
    strncpy (oldDefaultMsg.l_name, "ktalkd", OLD_NAME_SIZE);
    strncpy (newDefaultMsg.r_name, "ktalk", NEW_NAME_SIZE);
    strncpy (oldDefaultMsg.r_name, "ktalk", OLD_NAME_SIZE);
    newDefaultMsg.r_tty [0] = oldDefaultMsg.r_tty [0] = '\0'; 
}

int CheckProtocol::CheckHost (struct in_addr host, 
                              int retry, int timeout) {

  /* CheckHost may only be called once! */
  if (in_use) return 0;
  in_use = 1;
  hostAddr = host;

  /* if host has been checked before... */
/*  if (hostList [(long) host.s_addr]) {
    intern_protocol = * (hostList [(long) host.s_addr]);
    syslog (LOG_WARNING,
             "Found protocol (cached) %s for IP %ld.%ld.%ld.%ld",
             (intern_protocol == 1) ? "otalk" : "ntalk",
             htonl (host.s_addr) / 16777216L,
             htonl (host.s_addr) / 65536L % 256,
             htonl (host.s_addr) / 256L % 256,
             htonl (host.s_addr) % 256L);
    emit found (intern_protocol);
    return 1;
  }
*/
  retryMaximum = retry;

  /* create sockets */
  old_socket = socket (AF_INET, SOCK_DGRAM, 0);
  new_socket = socket (AF_INET, SOCK_DGRAM, 0);
  if (old_socket < 0 || new_socket < 0) {
      syslog(LOG_ERR, "CheckProtocol::CheckHost(): socket() failed!");
      return 0;
  }

  /* bind sockets */
  new_addr.sin_family = old_addr.sin_family = AF_INET;
  new_addr.sin_addr.s_addr = old_addr.sin_addr.s_addr = INADDR_ANY;
  new_addr.sin_port = old_addr.sin_port = htons (0);
  ksize_t len = sizeof old_addr;
  if (bind (old_socket, (struct sockaddr *) &old_addr, len) != 0 ||
      bind (new_socket, (struct sockaddr *) &new_addr, len) != 0) {
    syslog(LOG_ERR, "CheckProtocol::CheckHost(): bind() failed!");
    ::close (old_socket);
    ::close (new_socket);
    return 0;
  }
  if (getsockname (old_socket, (struct sockaddr *) &old_addr, &len) == -1 ||
      getsockname (new_socket, (struct sockaddr *) &new_addr, &len) == -1) {
    syslog (LOG_ERR,
            "CheckProtocol::CheckHost(): getsockname() failed!");
    ::close (old_socket);
    ::close (new_socket);
    return 0;
  }

  /* init QSocketNotifiers */
/*  old_notifier = new QSocketNotifier (old_socket, QSocketNotifier::Read, this);
  connect (old_notifier, SIGNAL (activated (int)), 
                         SLOT (responseReceived (int)));
  new_notifier = new QSocketNotifier (new_socket, QSocketNotifier::Read, this);
  connect (new_notifier, SIGNAL (activated (int)), 
                         SLOT (responseReceived (int)));
*/

  /* send control messages to daemons and return to caller */
  put_messages ();
  retryCounter = 0;
//  startTimer (timeout * 1000);
  return 1;
}

/*void CheckProtocol::timerEvent (QTimerEvent *) {
  retryCounter++;
  if (retryCounter > retryMaximum) {
    delete new_notifier;
    delete old_notifier;
    ::close (new_socket);
    ::close (old_socket);
    killTimers ();
    emit timeout ();
    return;
  }

  put_messages ();
}
*/
/*void CheckProtocol::responseReceived (int sockt) {
  ProtocolType *protocol;
  if (result_found) return;    // if one protocol was found, ignore others

  if (sockt == new_socket) {
    NEW_CTL_RESPONSE rp;
    if (recv (new_socket, (char *) &rp, sizeof (rp), 0) != sizeof (rp) ||
        rp.vers != TALK_VERSION ||
        rp.type != LOOK_UP) {
      syslog (LOG_WARNING,
              "CheckProtocol::responseReceived(): bad ntalk answer received");
      return;
    }
    protocol = &new_talk_dummy;
  } else if (sockt == old_socket) {
    OLD_CTL_RESPONSE rp;
    if (recv (old_socket, (char *) &rp, sizeof rp, 0) != sizeof rp ||
        rp.type != LOOK_UP) {
      syslog (syslog_INFO, 3900,
              "CheckProtocol::responseReceived(): bad otalk answer received");
      return;
    }
    protocol = &old_talk_dummy;
  } else {
    syslog (syslog_WARN, 3900, 
            "CheckProtocol::responseReceived(): unexpected packet received");
    return;
  }

  result_found = 1;
  intern_protocol = *protocol;
  syslog (LOG_WARNING, "Found protocol %s for IP %ld.%ld.%ld.%ld",
           (intern_protocol == 1) ? "otalk" : "ntalk",
           htonl (hostAddr.s_addr) / 16777216L,
           htonl (hostAddr.s_addr) / 65536L % 256,
           htonl (hostAddr.s_addr) / 256L % 256,
           htonl (hostAddr.s_addr) % 256L);
  killTimers ();
  delete new_notifier;
  delete old_notifier;
  ::close (old_socket);
  ::close (new_socket);
  // hostList.insert ((long) hostAddr.s_addr, protocol);
  emit found (intern_protocol);
}*/

void CheckProtocol::put_messages () {
  OLD_CTL_MSG old_msg = oldDefaultMsg;
  NEW_CTL_MSG new_msg = newDefaultMsg;

  sockaddr_in help_new = new_addr;
  help_new.sin_addr = localMachineAddr;
  sockaddr_in help_old = old_addr;
  help_old.sin_addr = localMachineAddr;

  new_msg.ctl_addr = *(struct sockaddr *) &help_new;
  old_msg.ctl_addr = *(struct sockaddr *) &help_old;
  new_msg.ctl_addr.sa_family = old_msg.ctl_addr.sa_family = htons (AF_INET);

  struct sockaddr_in daemon;
  daemon.sin_family = AF_INET;
  daemon.sin_addr = hostAddr;
  daemon.sin_port = ntalkDaemonPort;
  int len = sendto (new_socket, (char *) &new_msg, sizeof new_msg, 0,
                (struct sockaddr *) &daemon, sizeof daemon);
  if (len != sizeof new_msg) syslog(LOG_ERR,
           "CheckProtocol::CheckHost(): sendto() for ntalk failed!");

  daemon.sin_port = talkDaemonPort;
  len = sendto (old_socket, (char *) &old_msg, sizeof old_msg, 0,
                (struct sockaddr *) &daemon, sizeof daemon);
  if (len != sizeof old_msg) syslog(LOG_ERR,
           "CheckProtocol::CheckHost(): sendto() for otalk failed!");
}

// QIntDict <ProtocolType> CheckProtocol::hostList;
ProtocolType CheckProtocol::old_talk_dummy = talkProtocol;
ProtocolType CheckProtocol::new_talk_dummy = ntalkProtocol;
