#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/un.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <iostream.h>
#include <fcntl.h>

#include <dlfcn.h>

#include "controller.h"

PukeController::PukeController(QString sock, QObject *parent=0, const char *name=0)
  : QObject(parent, name)
{
  int len;
  struct sockaddr_un unix_addr;

  running = FALSE; // Running has to be true before we do any work

  if(sock.length() == 0){
    qsPukeSocket = getenv("HOME");
    if(qsPukeSocket.length() == 0){
      qsPukeSocket = "/tmp";
    }
    qsPukeSocket += "/.ksirc.socket";
  }
  else{
    qsPukeSocket = sock.data();
  }

  unlink(qsPukeSocket);
  iListenFd = socket(AF_UNIX, SOCK_STREAM, 0);
  if(iListenFd < 0){
    perror("PUKE: Unix Domain Socket create failed");
    return;
  }
  memset(&unix_addr, 0, sizeof(unix_addr));
  unix_addr.sun_family = AF_UNIX;
  strcpy(unix_addr.sun_path, qsPukeSocket);
  len = sizeof(unix_addr.sun_family) + qsPukeSocket.length();
  if( bind(iListenFd, (struct sockaddr *) &unix_addr, len) < 0){
    perror("PUKE: Could not bind to Unix Domain Socket");
    return;
  }

  if(listen(iListenFd, 5) < 0){
    perror("PUKE: Could not listen for inbound connections");
    return;
  }

  running = TRUE;
  
  fcntl(iListenFd, F_SETFL, O_NONBLOCK);  // Set it non-block so that
                                          // accept() never blocks.
  
  qsnListen = new QSocketNotifier(iListenFd, QSocketNotifier::Read, this);
  connect(qsnListen, SIGNAL(activated(int)),
	  this, SLOT(NewConnect(int)));

  qidConnectFd.setAutoDelete(TRUE);

  qidCommandTable.setAutoDelete(TRUE);

  initHdlr(); // Setup message command handlers.

  wrControl = new WidgetRunner(this, "widgetrunner");
  connect(wrControl, SIGNAL(outputMessage(int, PukeMessage *)),
	  this,  SLOT(writeBuffer(int, PukeMessage *)));
  
  lrControl = new LayoutRunner(wrControl, this, "layoutrunner");
  connect(lrControl, SIGNAL(outputMessage(int, PukeMessage *)),
	  this,  SLOT(writeBuffer(int, PukeMessage *)));  

}

PukeController::~PukeController()
{
}

void PukeController::NewConnect(int)
{
  int cfd, len = 0;
  struct sockaddr_un unix_addr;

  cfd = accept(iListenFd, (struct sockaddr *)&unix_addr, &len);
  if(cfd < 0){
    perror("PUKE: NewConnect fired, but no new connect");
    return;
  }
  fcntl(cfd, F_SETFL, O_NONBLOCK);  // Set it non-block so that
                                    // cfd() never blocks.

  fdStatus *fds = new fdStatus();
  fds->sr = new QSocketNotifier(cfd, QSocketNotifier::Read, this);
  fds->sw = new QSocketNotifier(cfd, QSocketNotifier::Write, this);
  connect(fds->sr, SIGNAL(activated(int)),
	  this, SLOT(Traffic(int)));
  connect(fds->sw, SIGNAL(activated(int)),
	  this, SLOT(Writeable(int)));
  qidConnectFd.insert(cfd, fds);
  qsnListen->setEnabled(TRUE);
}


void PukeController::Writeable(int fd)
{
  if(qidConnectFd[fd]){
    qidConnectFd[fd]->writeable = TRUE;
    qidConnectFd[fd]->sw->setEnabled(FALSE);
    //
    // Insert buffer flushing code here.
    //
  }
  else{
    cerr << "PUKE: Unkonwn fd: " << fd << endl;
  }
}

void PukeController::writeBuffer(int fd, PukeMessage *message)
{
  if(qidConnectFd[fd]){
    //    if(qidConnectFd[fd]->writeable == FALSE){
    //      cerr << "PUKE: Writing to FD that's not writeable: " << fd << endl;
    //    }
    if(message != 0){
      if(message->cArg[0] == 0)
	memset(message->cArg, 45, 50);
      int bytes = write(fd, message, sizeof(PukeMessage));
      cerr << "Wrote: " << bytes << endl;
      if(bytes <= 0){
	switch(errno){
	case EAGAIN: // Don't do anything for try again
	  break;
	default:
	  perror("Puke: write on socket failed");
	  closefd(fd);
	}
      }
    }
  }
  else{
    closefd(fd);
    cerr << "PUKE: Attempt to write to unkown fd:" << fd << endl;
  }
}

void PukeController::Traffic(int fd)
{
  PukeMessage pm;
  int bytes = -1;
  memset(&pm, 0, sizeof(pm));
  while((bytes = read(fd, &pm, sizeof(PukeMessage))) > 0){
    if(bytes != sizeof(PukeMessage)){
      cerr << "Short message, Got: " << bytes << " Wanted: " << sizeof(PukeMessage) << " NULL Padded" << endl;
    }
    printf("Traffic on: %d => %d %d %d %s\n", 
	   fd,
	   pm.iCommand, 
	   pm.iWinId,
	   pm.iArg,
	   pm.cArg);
    MessageDispatch(fd, &pm);
    memset(&pm, 0, sizeof(pm));
  }
  if(bytes <= 0){ // Shutdown the socket!
    switch(errno){
    case EAGAIN: // Don't do anything for try again
      break;
    default:
      perror("PukeController: read failed");
      closefd(fd);
    }
  }
  else{
    qidConnectFd[fd]->sr->setEnabled(TRUE);
  }
}


void PukeController::ServMessage(QString, int, QString)
{
  
}

// Message Dispatcher is in messagedispatcher.cpp


void PukeController::MessageDispatch(int fd, PukeMessage *pm)
{

  commandStruct *cs;

  cs = qidCommandTable[pm->iCommand];

  if(cs != NULL){
    (cs->cmd)(fd,pm);
  }
  else if((pm->iCommand >= 1000) && (pm->iCommand < 10000)){
    wrControl->inputMessage(fd, pm);
  }
  else if((pm->iCommand >= 11000) && (pm->iCommand < 12000)){
    lrControl->inputMessage(fd, pm);
  }
  else{
    hdlrPukeInvalid(fd,pm);
  }

}

void PukeController::initHdlr()
{
  // Each function handler gets an entry in the qidCommandTable

  commandStruct *cs;


  // Invalid is the default invalid handler
  cs = new commandStruct;
  cs->cmd = hdlrPukeInvalid;
  qidCommandTable.insert(PUKE_INVALID, cs);

  
  // Setup's handled by the setup handler
  cs = new commandStruct;
  cs->cmd = hdlrPukeSetup;
  qidCommandTable.insert(PUKE_SETUP, cs);

  // We don't receive PUKE_SETUP_ACK's we send them.
  cs = new commandStruct;
  cs->cmd = hdlrPukeInvalid;
  qidCommandTable.insert(PUKE_SETUP_ACK, cs);  

  cs = new commandStruct;
  cs->cmd = hdlrPukeEcho;
  qidCommandTable.insert(PUKE_ECHO, cs);  

}

// Start message handlers

void PukeController::hdlrPukeInvalid(int fd, PukeMessage *)
{
  PukeMessage pmOut;
  memset(&pmOut, 0, sizeof(pmOut));
  this->writeBuffer(fd, &pmOut);
}


void PukeController::hdlrPukeSetup(int fd, PukeMessage *pm)
{
  PukeMessage pmOut;
  memset(&pmOut, 0, sizeof(pmOut));
  pmOut.iCommand = PUKE_SETUP_ACK;
  pmOut.iArg = 1;
  if((strlen(pm->cArg) > 0) && 
     (this->qidConnectFd[fd] != NULL)){
    debug("Fd: %d cArg: %s", fd, pm->cArg);
    this->qidConnectFd[fd]->server = qstrdup(pm->cArg);
    pmOut.iWinId = pm->iWinId;
    pmOut.iArg = sizeof(PukeMessage);
  }
  this->writeBuffer(fd, &pmOut);
}

void PukeController::hdlrPukeEcho(int fd, PukeMessage *pm)
{
  PukeMessage pmOut;
  memcpy(&pmOut, pm, sizeof(PukeMessage));
  pmOut.iCommand = PUKE_ECHO_ACK;
  pmOut.iWinId = pm->iWinId;
  pmOut.iArg = pm->iArg;
  this->writeBuffer(fd, &pmOut);
}

void PukeController::closefd(int fd)
{
  close(fd);
  wrControl->closefd(fd);
  if(qidConnectFd[fd] == NULL){
    debug("PukeController: Connect table NULL, closed twice?");
    return;
  }
  qidConnectFd[fd]->sr->setEnabled(FALSE); // Shut them off
  qidConnectFd[fd]->sw->setEnabled(FALSE);
  delete qidConnectFd[fd]->sr;
  delete qidConnectFd[fd]->sw;
  qidConnectFd[fd]->server.truncate(0);
  qidConnectFd.remove(fd);
}
