/*
 *              kPPP: A pppd Front End for the KDE project
 *
 * $Id$
 *
 *              Copyright (C) 1997,98 Bernd Johannes Wuebben,
 *		                      Mario Weilguni,
 *                                    Harri Porten
 *
 *
 * This file was contributed by Harri Porten <porten@tu-harburg.de>
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

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/uio.h>
#include <sys/socket.h> 
#include <sys/types.h>
#include <sys/socket.h> 
#include <sys/un.h>
#include <fcntl.h>
#include <assert.h>
#include <string.h>
#include <errno.h>

#ifdef HAVE_RESOLV_H
#include <resolv.h>
#endif

#include "opener.h"
#include "kpppconfig.h"
#include "pap.h"
#include "devices.h"

#ifndef _PATH_RESCONF
#define _PATH_RESCONF "/etc/resolv.conf"
#endif

#define Debug(s) fprintf(stderr, s "\n");

Opener::Opener(int s) : socket(s) {
  mainLoop();
}

void Opener::mainLoop() {

  int len;
  int fd = -1;
  int flags, mode;
  const char *device;
  union AllRequests request;
  struct ResponseHeader response;
  struct msghdr	msg;
  struct iovec iov;

  iov.iov_base = (void *) &request;
  iov.iov_len = sizeof(request);
  
  msg.msg_name = 0L;
  msg.msg_namelen = 0;
  msg.msg_iov = &iov;
  msg.msg_iovlen = 1;
  msg.msg_control = 0L;
  msg.msg_controllen = 0;

  lockfile[0] = '\0';

  // loop forever
  while(1) {
    len = recvmsg(socket, &msg, 0);
    if(len < 0) {
      perror("Opener: error reading from socket");
      _exit(1);
    }

    switch(request.header.type) {

    case OpenDevice:
      Debug("Opener: received OpenDevice");
      assert(len == sizeof(struct OpenModemRequest));
      device = deviceByIndex(request.modem.deviceNum);
      response.status = 0;
      if ((fd = open(device, O_RDWR|O_NDELAY)) == -1) {
        Debug("error opening modem device !");
        fd = open(DEVNULL, O_RDONLY);
        response.status = -errno;
        sendFD(DEVNULL, fd, &response);
      } else
        sendFD(device, fd, &response);
      close(fd);
      break;

    case OpenLock:
      Debug("Opener: received OpenLock");
      assert(len == sizeof(struct OpenLockRequest));
      flags = request.lock.flags;
      assert(flags == O_RDONLY || flags == O_WRONLY|O_TRUNC|O_CREAT); 
      if(flags == O_WRONLY|O_TRUNC|O_CREAT)
        mode = 0644;
      else
        mode = 0;

      device = deviceByIndex(request.lock.deviceNum);
      assert(strlen(LOCK_DIR)+strlen(device) < MaxPathLen);
      strncpy(lockfile, LOCK_DIR"/LCK..", MaxPathLen);
      strncat(lockfile, device + strlen("/dev/"),
              MaxPathLen - strlen(lockfile));
      lockfile[MaxPathLen] = '\0';
      response.status = 0;
      // TODO:
//   struct stat st;
//   if(stat(lockfile.data(), &st) == -1) {
//     if(errno == EBADF)
//       return -1;
//   } else {
//     // make sure that this is a regular file
//     if(!S_ISREG(st.st_mode)) 
//       return -1;
//   }
      if ((fd = open(lockfile, flags, mode)) == -1) {
        Debug("error opening lockfile!");
        lockfile[0] = '\0';
        fd = open(DEVNULL, O_RDONLY);
        response.status = -errno;
        sendFD(DEVNULL, fd, &response);
      } else {
	fchown(fd, 0, 0);
        sendFD(lockfile, fd, &response);
      }
      close(fd);
      break;

    case RemoveLock:
      Debug("Opener: received RemoveLock");
      assert(len == sizeof(struct RemoveLockRequest));
      response.status = unlink(lockfile);
      lockfile[0] = '\0';
      sendResponse(&response);
      break;

    case OpenResolv:
      Debug("Opener: received OpenResolv");
      assert(len == sizeof(struct OpenResolvRequest));
      flags = request.resolv.flags;
      response.status = 0;
      if ((fd = open(_PATH_RESCONF, flags)) == -1) {
        Debug("error opening resolv.conf!");
        fd = open(DEVNULL, O_RDONLY);
        response.status = -errno;
        sendFD(DEVNULL, fd, &response);
      } else
        sendFD(_PATH_RESCONF, fd, &response);
      close(fd);
      break;

    case OpenSysLog:
      Debug("Opener: received OpenSysLog");
      assert(len == sizeof(struct OpenLogRequest));
      response.status = 0;
      if ((fd = open("/var/log/messages", O_RDONLY)) == -1) {
        if ((fd = open("/var/log/syslog.ppp", O_RDONLY)) == -1) {
          Debug("error opening syslog file !");
          fd = open(DEVNULL, O_RDONLY);
          response.status = -errno;
          sendFD(DEVNULL, fd, &response);
        } else
          sendFD("/var/log/syslog.ppp", fd, &response);
      } else
        sendFD("/var/log/messages", fd, &response);
      close(fd);
      break;

    case SetSecret:
      Debug("Opener: received SetSecret");
      assert(len == sizeof(struct SetSecretRequest));
      response.status = !createAuthFile(request.secret.authMethod,
					request.secret.username,
					request.secret.password);
      sendResponse(&response);
      break;

    case RemoveSecret:
      Debug("Opener: received RemoveSecret");
      assert(len == sizeof(struct RemoveSecretRequest));
      response.status = !removeAuthFile(request.remove.authMethod);
      sendResponse(&response);
      break;

    case Stop:
      Debug("Opener: received STOP command");
      _exit(0);
      break;

    default:
      Debug("Opener: unknown command type. Exiting ...");
      _exit(1);
    }
  }
}


//
// Send an open fd over a UNIX socket pair
//
int Opener::sendFD(const char *path, int fd,
                       struct ResponseHeader *response) {

  struct { struct cmsghdr cmsg; int fd; } control;
  struct msghdr	msg;
  struct iovec iov[2];

  msg.msg_name = 0L;
  msg.msg_namelen = 0;
  msg.msg_iov = &iov[0];
  msg.msg_iovlen = 2;

  // Send data
  iov[0].iov_base = (void *) response;
  iov[0].iov_len = sizeof(struct ResponseHeader);
  iov[1].iov_base = (void *) path;
  iov[1].iov_len = strlen(path) + 1;

  // Send a (duplicate of) the file descriptor
  control.cmsg.cmsg_len = sizeof(struct cmsghdr) + sizeof(int);
  control.cmsg.cmsg_level = SOL_SOCKET;
  control.cmsg.cmsg_type = SCM_RIGHTS;

  msg.msg_control = &control;
  msg.msg_controllen = control.cmsg.cmsg_len;

  // What's the duplicating good for ?
  //  *((int *) &control.cmsg.cmsg_data) = dup(ttyfd);

// Let's try it this way. Should work on FreeBSD, too.
#ifdef CMSG_DATA
  *((int *)CMSG_DATA(&control.cmsg)) = fd;
#else
  *((int *) &control.cmsg.cmsg_data) = fd;
#endif

  if (sendmsg(socket, &msg, 0) < 0) {
    perror("unable to send file descriptors");
    return -1;
  }

  return 0;
}

int Opener::sendResponse(struct ResponseHeader *response) {

  struct msghdr	msg;
  struct iovec iov;

  msg.msg_name = 0L;
  msg.msg_namelen = 0;
  msg.msg_iov = &iov;
  msg.msg_iovlen = 1;
  msg.msg_control = 0L;
  msg.msg_controllen = 0;

  // Send data
  iov.iov_base = (void *) response;
  iov.iov_len = sizeof(struct ResponseHeader);

  if (sendmsg(socket, &msg, 0) < 0) {
    perror("unable to send response");
    return -1;
  }

  return 0;
}

const char* Opener::deviceByIndex(int idx) {

  const char *device = 0L;

  for(int i = 0; devices[i]; i++)
    if(i == idx)
      device = devices[i];
  assert(device);
  return device;
}




