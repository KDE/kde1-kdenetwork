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
#include <sys/socket.h> 
#include <sys/types.h>
#include <sys/socket.h> 
#include <sys/un.h>
#include <fcntl.h>
#include <assert.h>
#include <string.h>

#ifdef HAVE_RESOLV_H
#include <resolv.h>
#endif

#include "opener.h"
#include "kpppconfig.h"
#include "pap.h"

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

  char filename[MaxPathLen+1+1]; // +1 just to be sure
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
      strncpy(filename, request.modem.modemPath, MaxPathLen);
      filename[MaxPathLen] = '\0';
      response.status = 0;
      if ((fd = open(filename, O_RDWR|O_NDELAY)) == -1) {
        Debug("error opening modem device !");
        fd = open(DEVNULL, O_RDONLY);
        response.status = -errno;
        sendFD(DEVNULL, fd, &response);
      } else
        sendFD(filename, fd, &response);
      break;

    case OpenLock:
      Debug("Opener: received OpenLock");
      assert(len == sizeof(struct OpenLockRequest));
      strncpy(lockfile, request.lock.file, MaxPathLen);
      lockfile[MaxPathLen] = '\0';
      flags = request.lock.flags;
      assert(flags == O_RDONLY || flags == O_WRONLY|O_TRUNC|O_CREAT); 
      if(flags == O_WRONLY|O_TRUNC|O_CREAT)
        mode = 0644;
      else
        mode = 0;
      response.status = 0;
      if ((fd = open(lockfile, flags, mode)) == -1) {
        fprintf(stderr, "error opening lockfile!\n");
        lockfile[0] = '\0';
        fd = open(DEVNULL, O_RDONLY);
        response.status = -errno;
        sendFD(DEVNULL, fd, &response);
      } else
        sendFD(lockfile, fd, &response);
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
        sendFD(filename, fd, &response);
      break;

    case OpenSysLog:
      Debug("Opener: received OpenSysLog");
      assert(len == sizeof(struct OpenLogRequest));
      response.status = 0;
      if ((fd = open("/var/log/messages", O_RDONLY)) == -1) {
        if ((fd = open("/var/log/syslog.ppp", O_RDONLY)) == -1) {
          fprintf(stderr, "error opening syslog file !\n");
          fd = open(DEVNULL, O_RDONLY);
          response.status = -errno;
          sendFD(DEVNULL, fd, &response);
        } else
          sendFD("/var/log/syslog.ppp", fd, &response);
      } else
        sendFD("/var/log/messages", fd, &response);
      break;

    case SetSecret:
      Debug("Opener: received SetSecret");
      assert(len == sizeof(struct SetSecretRequest));
      response.status = createAuthFile(request.secret.authMethod,
                                       request.secret.username,
                                       request.secret.password);
      sendResponse(&response);
      break;

    case RemoveSecret:
      Debug("Opener: received RemoveSecret");
      assert(len == sizeof(struct RemoveSecretRequest));
      response.status = removeAuthFile(request.remove.authMethod);
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
int Opener::sendFD(const char *ttypath, int ttyfd,
                       struct ResponseHeader *response) {

  struct { struct cmsghdr cmsg; int fd; } control;
  struct msghdr	msg;
  struct iovec iov[2];

  msg.msg_name = 0L;
  msg.msg_namelen = 0;
  msg.msg_iov = &iov[0];
  msg.msg_iovlen = 2;
  msg.msg_control = &control;
  msg.msg_controllen = sizeof(control);

  // Send data
  iov[0].iov_base = (void *) response;
  iov[0].iov_len = sizeof(struct ResponseHeader);
  iov[1].iov_base = (void *) ttypath;
  iov[1].iov_len = strlen(ttypath) + 1;

  // Send a (duplicate of) the file descriptor
  control.cmsg.cmsg_len = sizeof(struct cmsghdr) + sizeof(int);
  control.cmsg.cmsg_level = SOL_SOCKET;
  control.cmsg.cmsg_type = SCM_RIGHTS;
  *((int *) &control.cmsg.cmsg_data) = dup(ttyfd);

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

