#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/un.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <iostream.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>

#include <dlfcn.h>

#include "controller.h"
#include "../config.h"

uint PukeController::uiBaseWinId = 10; // Gives a little seperation from the controller id

PukeController::PukeController(QString sock, QObject *parent=0, const char *name=0) /*fold00*/
  : PObject(parent)
{
  int len, prev_umask;
  struct sockaddr_un unix_addr;

  running = FALSE; // Running has to be true before we do any work

  // Set the umask to something sane that doesn't allow others to take over ksirc
  prev_umask = umask(0177);

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

  /*
   * Setup widget data trees
   */
  WidgetList.setAutoDelete(TRUE);
  widgetCF.setAutoDelete(TRUE);

  initHdlr(); // Setup message command handlers.

  /*
  wrControl = new WidgetRunner(this, "widgetrunner");
  connect(wrControl, SIGNAL(outputMessage(int, PukeMessage *)),
	  this,  SLOT(writeBuffer(int, PukeMessage *)));
  
  lrControl = new LayoutRunner(wrControl, this, "layoutrunner");
  connect(lrControl, SIGNAL(outputMessage(int, PukeMessage *)),
          this,  SLOT(writeBuffer(int, PukeMessage *)));

  */
  // Set umask back so it doesn't affect dcc's and so forth.
  umask(prev_umask);

}

PukeController::~PukeController() /*fold00*/
{
}

void PukeController::NewConnect(int) /*fold00*/
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

  /*
   * Now we add ourselves as a client to the fd so we can process messages going to us
   */
  WidgetS *ws = new WidgetS;
  ws->pwidget = this;
  ws->type = 1;
  insertPObject(cfd, ControllerWinId, ws);
  
}


void PukeController::Writeable(int fd) /*fold00*/
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

void PukeController::writeBuffer(int fd, PukeMessage *message) /*fold00*/
{
  if(qidConnectFd[fd]){
    //    if(qidConnectFd[fd]->writeable == FALSE){
    //      cerr << "PUKE: Writing to FD that's not writeable: " << fd << endl;
    //    }
    if(message != 0){
      if(message->cArg[0] == 0)
	memset(message->cArg, 45, 50);
      int bytes = write(fd, message, sizeof(PukeMessage));
      //      cerr << "Wrote: " << bytes << endl;
      if(bytes <= 0){
	switch(errno){
	case EAGAIN: // Don't do anything for try again
	  break;
	default:
	  perror("Puke: write on socket failed");
	  // Don't call closefd() since deletes are called on write's
	  // since write is being called from the destructors, etc of
	  // the widgets.  (bad things happend when you call write
	  // then your return; path ceasaes to exist.
	  //	  closefd(fd);
	}
      }
    }
  }
  else{
//    closefd(fd);
    cerr << "PUKE: Attempt to write to unkown fd:" << fd << endl;
  }
}

void PukeController::Traffic(int fd) /*fold00*/
{
  PukeMessage pm;
  int bytes = -1;
  memset(&pm, 0, sizeof(pm));
  while((bytes = read(fd, &pm, sizeof(PukeMessage))) > 0){
    if(bytes != sizeof(PukeMessage)){
      cerr << "Short message, Got: " << bytes << " Wanted: " << sizeof(PukeMessage) << " NULL Padded" << endl;
    }
    /*
    printf("Traffic on: %d => %d %d %d %s\n", 
	   fd,
	   pm.iCommand, 
	   pm.iWinId,
	   pm.iArg,
	   pm.cArg);
    */
    if(pm.cArg[49] != 0){
      pm.cArg[49] = 0;
      warning("PukeController: Message was NOT null terminated\n");
    }
    MessageDispatch(fd, &pm);
    memset(&pm, 0, sizeof(pm));
  }
  if(bytes <= 0){ // Shutdown the socket!
    switch(errno){
    case EAGAIN: // Don't do anything for try again
      break;
    case 0:
      break;     // We just read nothing, don't panic
    default:
      perror("PukeController: read failed");
      closefd(fd);
    }
  }
  else{
    qidConnectFd[fd]->sr->setEnabled(TRUE);
  }
}


void PukeController::ServMessage(QString, int, QString) /*fold00*/
{
  
}

// Message Dispatcher is in messagedispatcher.cpp


void PukeController::MessageDispatch(int fd, PukeMessage *pm) /*FOLD00*/
{
    try {

        /*
         * Get the object id, this may produce a errorNuSuchWidget
         */
        PObject *obj = id2pobject(fd, pm->iWinId);
        
        /*
         * Call the message hanlder for the widget
         */
        obj->messageHandler(fd, pm);
    }
    catch(errorNoSuchWidget &err){
        PukeMessage pmRet;
        pmRet.iCommand = PUKE_INVALID;
        pmRet.iWinId = pm->iWinId;
        pmRet.iArg = 0;
        pmRet.cArg[0] = 0;
        emit outputMessage(fd, &pmRet);
        return;
    }
    catch(errorCommandFailed &err){
        PukeMessage pmRet;
        pmRet.iCommand = err.command();
        pmRet.iWinId = pm->iWinId;
        pmRet.iArg = err.iarg();
        pmRet.cArg[0] = 0;
        emit outputMessage(fd, &pmRet);
        return;
    }

/*
  commandStruct *cs;

  cs = qidCommandTable[pm->iCommand];

  if(cs != NULL){
    (this->*(cs->cmd))(fd,pm);
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

*/
}

void PukeController::initHdlr() /*FOLD00*/
{

  widgetCreate *wc;

  wc = new widgetCreate;
  wc->wc = PWidget::createWidget;
  widgetCF.insert(PWIDGET_WIDGET, wc);

  wc = new widgetCreate;
  wc->wc = PObject::createWidget;
  widgetCF.insert(PWIDGET_OBJECT, wc);


  // Each function handler gets an entry in the qidCommandTable
  commandStruct *cs;


  // Invalid is the default invalid handler
  cs = new commandStruct;
  cs->cmd = hdlrPukeInvalid;
  cs->dlhandle = 0x0;
  qidCommandTable.insert(PUKE_INVALID, cs);

  
  // Setup's handled by the setup handler
  cs = new commandStruct;
  cs->cmd = hdlrPukeSetup;
  cs->dlhandle = 0x0;
  qidCommandTable.insert(PUKE_SETUP, cs);

  // We don't receive PUKE_SETUP_ACK's we send them.
  cs = new commandStruct;
  cs->cmd = hdlrPukeInvalid;
  cs->dlhandle = 0x0;
  qidCommandTable.insert(PUKE_SETUP_ACK, cs);  

  cs = new commandStruct;
  cs->cmd = hdlrPukeEcho;
  cs->dlhandle = 0x0;
  qidCommandTable.insert(PUKE_ECHO, cs);

}

// Start message handlers

void PukeController::hdlrPukeInvalid(int fd, PukeMessage *) /*fold00*/
{
  PukeMessage pmOut;
  memset(&pmOut, 0, sizeof(pmOut));
  this->writeBuffer(fd, &pmOut);
}


void PukeController::hdlrPukeSetup(int fd, PukeMessage *pm) /*fold00*/
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

void PukeController::hdlrPukeEcho(int fd, PukeMessage *pm) /*fold00*/
{
  PukeMessage pmOut;
  memcpy(&pmOut, pm, sizeof(PukeMessage));
  pmOut.iCommand = PUKE_ECHO_ACK;
  pmOut.iWinId = pm->iWinId;
  pmOut.iArg = pm->iArg;
  this->writeBuffer(fd, &pmOut);
}

void PukeController::closefd(int fd) /*fold00*/
{
  if(qidConnectFd[fd] == NULL){
    debug("PukeController: Connect table NULL, closed twice?");
    return;
  }
  // Call wrControl after check for being called twice.
  close(fd);
  qidConnectFd[fd]->sr->setEnabled(FALSE); // Shut them off
  qidConnectFd[fd]->sw->setEnabled(FALSE);
  delete qidConnectFd[fd]->sr;
  delete qidConnectFd[fd]->sw;
  qidConnectFd[fd]->server.truncate(0);
  qidConnectFd.remove(fd);

  // Last thingwe do is clean up wrControl, this will ensure that
  // We don't try and write any output.
//  wrControl->closefd(fd);
}

bool PukeController::checkWidgetId(widgetId *pwi) /*fold00*/
{
  if(WidgetList[pwi->fd] != NULL)
    if(WidgetList[pwi->fd]->find(pwi->iWinId) != NULL)
      return TRUE;
  
  return FALSE;
}

PObject *PukeController::id2pobject(widgetId *pwi){ /*fold00*/
  if(checkWidgetId(pwi) == TRUE){
    return WidgetList[pwi->fd]->find(pwi->iWinId)->pwidget;
  }
  throw(errorNoSuchWidget(*pwi));
  return 0; // never reached
}

PObject *PukeController::id2pobject(int fd, int iWinId){ /*fold00*/
  widgetId wi;
  wi.fd = fd;
  wi.iWinId = iWinId;

  return id2pobject(&wi);
}

void PukeController::insertPObject(int fd, int iWinId, WidgetS *obj){ /*fold00*/
    if(WidgetList[fd] == NULL){
        QIntDict<WidgetS> *qidWS = new QIntDict<WidgetS>;
        qidWS->setAutoDelete(TRUE);
        WidgetList.insert(fd, qidWS);
    }
    WidgetList[fd]->insert(iWinId, obj);
}

void PukeController::messageHandler(int fd, PukeMessage *pm) { /*FOLD00*/
  widgetId wI, wIret;
  wI.fd = fd;
  wI.iWinId = pm->iWinId;

  commandStruct *cs;

  cs = qidCommandTable[pm->iCommand];

  if(cs != NULL){
    (this->*(cs->cmd))(fd,pm);
  }
  else if(pm->iCommand == PUKE_WIDGET_CREATE){
    wIret = wI;
    wIret.iWinId = createWidget(wI, pm->iArg).iWinId; // Create the acutal pw
    PukeMessage pmRet;
    pmRet.iCommand = PUKE_WIDGET_CREATE_ACK;
    pmRet.iWinId = wIret.iWinId;
    pmRet.iArg = 0;
    strncpy(pmRet.cArg, pm->cArg, 50);
    emit outputMessage(fd, &pmRet);
  }
  else if(pm->iCommand == PUKE_WIDGET_DELETE){
    if(pm->iWinId == ControllerWinId) // Don't try and delete ourselves
      throw(errorCommandFailed(PUKE_INVALID,0));

    PukeMessage pmRet = *pm;
    QIntDict<WidgetS> *qidWS = WidgetList[fd];
    if(qidWS == 0){
      debug("WidgetRunner:: no such set of widget descriptors?");
      throw(errorCommandFailed(PUKE_INVALID, 0));
    }
    if(qidWS->find(wI.iWinId)){
      debug("Closing: %d", wI.iWinId);
      // Remove the list item then delete the widget.  This will stop
      // the destroyed signal from trying to remove it again.
      PObject *pw = qidWS->find(wI.iWinId)->pwidget;
      qidWS->remove(wI.iWinId);
      delete pw; pw = 0;
      pmRet.iCommand = PUKE_WIDGET_DELETE_ACK;
    }
    else {
      pmRet.iArg = 1;
      strcpy(pmRet.cArg, "No Such Widget");
      pmRet.iCommand = PUKE_WIDGET_DELETE_ACK;
      warning("WidgetRunner: no such widget: %d", wI.iWinId);
    }
    emit outputMessage(fd, &pmRet);
  }
  else if(pm->iCommand == PUKE_WIDGET_LOAD){
    PukeMessage pmRet = *pm;
    void *handle;
    const char *error;
    PObject *(*wc)(widgetId *wI, PObject *parent);
    widgetCreate *wC;

    pm->cArg[49] = 0;
    handle = dlopen(kSircConfig->kdedir + "/share/apps/ksirc/" + QString(pm->cArg), RTLD_LAZY|RTLD_GLOBAL);
    if (!handle) {
      fputs(dlerror(), stderr);
      fputs("\n", stderr);
      emit(errorCommandFailed(-pm->iCommand, 1));
    }
    wc =  (PObject *(*)(widgetId *wI, PObject *parent) )
      dlsym(handle, "createWidget");
    if ((error = dlerror()) != NULL)  {
      fputs(error, stderr);
      emit(errorCommandFailed(-pm->iCommand, 1));
    }
    wC = new widgetCreate;
    wC->wc = wc;
    wC->dlhandle = handle;
    widgetCF.insert(pm->iArg, wC);

    pmRet.iCommand = -pm->iCommand;
    emit outputMessage(fd, &pmRet);
  }
  else if(pm->iCommand == PUKE_WIDGET_UNLOAD){
    const char *error;
    if(widgetCF[pm->iArg]){
      dlclose(widgetCF[pm->iArg]->dlhandle);
      if ((error = dlerror()) != NULL)  {
	fputs(error, stderr);
	pm->iCommand = -pm->iCommand;
	pm->iArg = -1;
	emit outputMessage(fd, pm);
	return;
      }
      widgetCF.remove(pm->iArg);
      pm->iCommand = -pm->iCommand;
      emit outputMessage(fd, pm);
    }
  }
  else{
    if(checkWidgetId(&wI) == TRUE){
      WidgetList[wI.fd]->find(wI.iWinId)->pwidget->messageHandler(fd, pm);
    }
    else{
      PukeMessage pmRet;
      pmRet.iCommand = PUKE_INVALID;
      pmRet.iWinId = wI.iWinId;
      pmRet.iArg = 0;
      pmRet.cArg[0] = 0;
      emit outputMessage(fd, &pmRet);
    }
  }
}

widgetId PukeController::createWidget(widgetId wI, int iArg) /*FOLD00*/
{
  widgetId wIret;
  PWidget *parent = 0; // Defaults to no parent
  WidgetS *ws = new WidgetS;

  /*
   * The parent widget ID and type are packed into the iArg
   * the pattern is 2 shorts.
   */

  unsigned short int *iaArg;
  iaArg = (unsigned short int *) &iArg;
  int iParent = iaArg[1];
  int iType = iaArg[0];

  wI.iWinId = iParent; // wI is the identifier for the parent widget
  
  if(widgetCF[iType] == NULL){ // No such widget, bail out.
    wIret.fd = 0;
    wIret.iWinId = 0;
    throw(errorCommandFailed(PUKE_INVALID,1));
  }
  
  uiBaseWinId++; // Get a new base win id

  // wIret holds the current widget id for the new widget
  wIret.iWinId = uiBaseWinId;
  wIret.fd = wI.fd;
  
  if(checkWidgetId(&wI) == TRUE){
    PObject *obj = WidgetList[wI.fd]->find(wI.iWinId)->pwidget;
    if(obj->widget()->isWidgetType() == FALSE){
      throw(errorCommandFailed(PUKE_INVALID, 0));
    }
    parent = (PWidget *) obj;
  }

  ws->pwidget = (widgetCF[iType]->wc)(&wIret, parent);
  ws->type = iType;
  connect(ws->pwidget, SIGNAL(widgetDestroyed(widgetId)),
	  this, SLOT(closeWidget(widgetId)));

  connect(ws->pwidget, SIGNAL(outputMessage(int, PukeMessage*)),
	  this, SIGNAL(outputMessage(int, PukeMessage*)));

  // insertPBoject(fd, uiBaseWinId, ws);
  // The widget list has to exist since we have ourselves in the list
  WidgetList[wIret.fd]->insert(wIret.iWinId, ws);
  return wIret;
}

void PukeController::closeWidget(widgetId wI){ /*fold00*/
  // Check to make sure we have a valid widget
  // Then remove it from the list.
  // Don't delete it since well, we're being called since it's already 
  // being deleted.
  if(checkWidgetId(&wI) == TRUE){
    WidgetList[wI.fd]->remove(wI.iWinId);

    PukeMessage pmRet;
    memset(&pmRet, 0, sizeof(PukeMessage));
    pmRet.iCommand = PUKE_WIDGET_DELETE_ACK;
    pmRet.iWinId = wI.iWinId;
    pmRet.iArg = 0;
    emit outputMessage(wI.fd, &pmRet);
    debug("Sent close message, returning");
    return;
  }
  warning("WidgetRunner: widget delete %d %d failed", wI.fd, wI.iWinId);
}
