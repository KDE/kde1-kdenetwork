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

//#include <dlfcn.h>

#include <qobjcoll.h>

#include "controller.h"
#include "playout.h"
#include "../config.h"
#include "../objFinder.h"

#undef DEBUG

uint PukeController::uiBaseWinId = 10; // Gives a little seperation from the controller id

PukeController::PukeController(QString sock, QObject *parent, const char *name) /*fold00*/
  : PObject(parent, name)
{
  int len, prev_umask;
  struct sockaddr_un unix_addr;

  running = FALSE; // Running has to be true before we do any work
  bClosing = FALSE; // we're not trying to close so set this false.

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
  
  qsnListen = new QSocketNotifier(iListenFd, QSocketNotifier::Read, this, QString(name) + "_iListen");
  connect(qsnListen, SIGNAL(activated(int)),
	  this, SLOT(NewConnect(int)));

  qidConnectFd.setAutoDelete(TRUE);

  qidCommandTable.setAutoDelete(TRUE);

  /*
   * Setup widget data trees
   */
  WidgetList.setAutoDelete(TRUE);
  widgetCF.setAutoDelete(TRUE);

  /*
   * Connect outputMessage to the acutal write buffer function
   * outputMessage signals from pobjects are chained until they finally reach us.
   */
  connect(this, SIGNAL(outputMessage(int, PukeMessage *)),
	  this,  SLOT(writeBuffer(int, PukeMessage *)));

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
  unlink(qsPukeSocket);
}

void PukeController::NewConnect(int) /*fold00*/
{
  int cfd;
  int len = 0;
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
      int bytes = 0;
      if(message->iTextSize == 0 || message->cArg == 0){
        message->iHeader = iPukeHeader;
        message->iTextSize = 0;
        message->cArg = 0;
#ifdef DEBUG
        printf("Traffic on: %d <= %d %d %d %d 0x%x\n",
               fd,
               message->iCommand,
               message->iWinId,
               message->iArg,
               message->iTextSize,
               message->cArg);
#endif
        bytes = write(fd, message, 5 * sizeof(int));
      }
      else{
        struct OutMessageS {
          unsigned int iHeader;
          int iCommand;
          int iWinId;
          int iArg;
          int iTextSize;
          char cArg[message->iTextSize];
        } OutMessage;
        OutMessage.iHeader = iPukeHeader;
        OutMessage.iCommand = message->iCommand;
        OutMessage.iWinId = message->iWinId;
        OutMessage.iArg = message->iArg;
        OutMessage.iTextSize = message->iTextSize;
        memcpy(OutMessage.cArg, message->cArg, OutMessage.iTextSize);
        //        OutMessage.cArg[OutMessage.iTextSize] = 0; // Don't need to null out the last character
#ifdef DEBUG
        printf("Traffic on: %d <= %d %d %d %d 0x%x\n",
               fd,
               message->iCommand,
               message->iWinId,
               message->iArg,
               message->iTextSize,
               message->cArg);
#endif /* DEBUG */
        bytes = write(fd, &OutMessage, 5*sizeof(int) + (OutMessage.iTextSize) * sizeof(char));
      }
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
    closefd(fd);
    cerr << "PUKE: Attempt to write to unkown fd:" << fd << endl;
  }
}

void PukeController::Traffic(int fd) /*fold00*/
{
  PukeMessage pm;
  int bytes = -1;
  memset(&pm, 0, sizeof(pm));
  while((bytes = read(fd, &pm, 5*sizeof(int))) > 0){
    if(bytes != 5*sizeof(int)){
      cerr << "Short message, Got: " << bytes << " Wanted: " << sizeof(PukeMessage) << " NULL Padded" << endl;
    }
#ifdef DEBUG
    printf("Traffic on: %d => %d %d %d %d",
           fd,
	   pm.iCommand, 
	   pm.iWinId,
	   pm.iArg,
           pm.iTextSize);
#endif /* DEBUG */
    if(pm.iHeader != iPukeHeader){
      warning("Invalid packet received, discarding!");
      return;
    }
    if(pm.iTextSize > 0){
      pm.cArg = new char[pm.iTextSize + 1];
      read(fd, pm.cArg, pm.iTextSize * sizeof(char));
      pm.cArg[pm.iTextSize] = 0x0; // Null terminate the string.
      printf(" %s\n", pm.cArg);
    }
    else {
        pm.cArg = 0;
        printf("\n");
    }
    MessageDispatch(fd, &pm);
    delete[] pm.cArg; // Free up cArg is used
    memset(&pm, 0, 5*sizeof(int));
  }
  if(bytes <= 0){ // Shutdown the socket!
    switch(errno){
    case EAGAIN: // Don't do anything for try again
      break;
//    case 0:
//      break;     // We just read nothing, don't panic
    case EIO:
    case EISDIR:
    case EBADF:
    case EINVAL:
    case EFAULT:
    default:
      perror("PukeController: read failed");
      closefd(fd);
      close(fd);
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


void PukeController::MessageDispatch(int fd, PukeMessage *pm) /*fold00*/
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
        pmRet.iTextSize = 0;
        emit outputMessage(fd, &pmRet);
        return;
    }
    catch(errorCommandFailed &err){
        PukeMessage pmRet;
        pmRet.iCommand = err.command();
        pmRet.iWinId = pm->iWinId;
	pmRet.iArg = err.iarg();
	pmRet.iTextSize = 0;
        emit outputMessage(fd, &pmRet);
        return;
    }
}

void PukeController::initHdlr() /*fold00*/
{

  widgetCreate *wc;

  wc = new widgetCreate;
  wc->wc = PWidget::createWidget;
  widgetCF.insert(PWIDGET_WIDGET, wc);

  wc = new widgetCreate;
  wc->wc = PObject::createWidget;
  widgetCF.insert(PWIDGET_OBJECT, wc);

  wc = new widgetCreate;
  wc->wc = PLayout::createWidget;
  widgetCF.insert(POBJECT_LAYOUT, wc);

  // Each function handler gets an entry in the qidCommandTable
  commandStruct *cs;


  // Invalid is the default invalid handler
  cs = new commandStruct;
  cs->cmd = &hdlrPukeInvalid;
  cs->dlhandle = 0x0;
  qidCommandTable.insert(PUKE_INVALID, cs);

  
  // Setup's handled by the setup handler
  cs = new commandStruct;
  cs->cmd = &hdlrPukeSetup;
  cs->dlhandle = 0x0;
  qidCommandTable.insert(PUKE_SETUP, cs);

  // We don't receive PUKE_SETUP_ACK's we send them.
  cs = new commandStruct;
  cs->cmd = &hdlrPukeInvalid;
  cs->dlhandle = 0x0;
  qidCommandTable.insert(PUKE_SETUP_ACK, cs);  

  cs = new commandStruct;
  cs->cmd = &hdlrPukeEcho;
  cs->dlhandle = 0x0;
  qidCommandTable.insert(PUKE_ECHO, cs);

  // Fetch widget gets the requested widget from the ServerController
  cs = new commandStruct;
  cs->cmd = &hdlrPukeFetchWidget;
  cs->dlhandle = 0x0;
  qidCommandTable.insert(PUKE_FETCHWIDGET, cs);

  // Fetch widget gets the requested widget from the ServerController
  cs = new commandStruct;
  cs->cmd = &hdlrPukeDumpTree;
  cs->dlhandle = 0x0;
  qidCommandTable.insert(PUKE_DUMPTREE, cs);



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
    pmOut.iArg = sizeof(PukeMessage) - sizeof(char *);
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

void PukeController::hdlrPukeDumpTree(int fd, PukeMessage *pm) /*fold00*/
{
  objFinder::dumpTree();
  
  PukeMessage pmOut;
  memcpy(&pmOut, pm, sizeof(PukeMessage));
  pmOut.iCommand = -pm->iCommand;
  pmOut.iWinId = pm->iWinId;
  pmOut.iArg = pm->iArg;
  this->writeBuffer(fd, &pmOut);
}


void PukeController::hdlrPukeFetchWidget(int fd, PukeMessage *pm) /*fold00*/
{
  widgetId wIret;
  
  /*
   * The parent widget ID and type are packed into the iArg
   * the pattern is 2 shorts.
   */

  unsigned short int *iaArg;
  iaArg = (unsigned short int *) &pm->iArg;
  int iParent = iaArg[1];
  int iType = iaArg[0];
//  bool bRegex = iParent > 0 ? TRUE : FALSE;

  char rand[50],name[50];
  sscanf(pm->cArg, "%s\t%s", rand, name);

  debug("Fetching new widget, type: %d, parent: %d objname: %s", iType, iParent, name);

  uiBaseWinId++; // Get a new base win id

  // wIret holds the current widget id for the new widget
  wIret.iWinId = uiBaseWinId;
  wIret.fd = fd;

  //  CreateArgs arg = CreateArgs(this, pm, &wIret, parent)
  CreateArgs arg(this, pm, &wIret, 0);

  // Let's go looking for the widget
  // Match any class with the right name
  QObject *obj = 0x0;
  if(strcmp(name, parent()->name()) == 0){
    obj = parent();
  }
  else {
    /*
    QObjectList *list = parent()->queryList( 0, name, bRegex, TRUE );
    QObjectListIt it( *list );          // iterate over the widgets
    if(it.current() == 0){
      wIret.fd = 0;
      wIret.iWinId = 0;
      throw(errorCommandFailed(PUKE_INVALID,5));
    }
    obj = it.current();
    */
    obj = objFinder::find(name, 0x0);
    if(obj == 0){
      wIret.fd = 0;
      wIret.iWinId = 0;
      throw(errorCommandFailed(PUKE_INVALID,5));
    }
    
  }
  debug("Found: %s", obj->name());
  obj->dumpObjectInfo();
  obj->dumpObjectTree();

  arg.fetchedObj = obj;
  
  WidgetS *ws = new WidgetS;
  ws->pwidget = (widgetCF[iType]->wc)(arg);
  ws->type = iType;
  connect(ws->pwidget, SIGNAL(widgetDestroyed(widgetId)),
	  this, SLOT(closeWidget(widgetId)));

  connect(ws->pwidget, SIGNAL(outputMessage(int, PukeMessage*)),
	  this, SIGNAL(outputMessage(int, PukeMessage*)));

  // insertPBoject(fd, uiBaseWinId, ws);
  // The widget list has to exist since we have ourselves in the list
  WidgetList[wIret.fd]->insert(wIret.iWinId, ws);

  debug("Fetched widget setup");

  PukeMessage pmRet;
  pmRet.iCommand = PUKE_WIDGET_CREATE_ACK;
  pmRet.iWinId = wIret.iWinId;
  pmRet.iArg = 0;
  pmRet.iTextSize = pm->iTextSize;
  pmRet.cArg = pm->cArg;
  emit outputMessage(fd, &pmRet);

}
void PukeController::closefd(int fd) /*fold00*/
{
  if(bClosing == TRUE)
    return;
  bClosing = TRUE;
  if(qidConnectFd[fd] == NULL){
    debug("PukeController: Connect table NULL, closed twice?");
    return;
  }
  // Shutof the listener before closing the socket, just in case.
  qidConnectFd[fd]->sr->setEnabled(FALSE); // Shut them off
  qidConnectFd[fd]->sw->setEnabled(FALSE);
  delete qidConnectFd[fd]->sr;
  delete qidConnectFd[fd]->sw;
  qidConnectFd[fd]->server.truncate(0);
  qidConnectFd.remove(fd);
  close(fd);

  /*
   * Now let's remove all traces of the widgets
   */
  QIntDict<WidgetS> *qidWS = WidgetList[fd];
  if(qidWS == 0){
    debug("WidgetRunner:: Close called twice?");
    return;
  }

  qidWS->remove(PUKE_CONTROLLER);
  QIntDictIterator<WidgetS> it(*qidWS);
  if(it.count() == 0){
      debug("WidgetRunner: nothing left to delete\n");
  }
  else{
    while(it.current()){
      /*
       * Delete all the layouts first
       *
       */
      long key = it.currentKey();
      if(WidgetList[fd]->find(key) == 0x0 ||
         WidgetList[fd]->find(key)->pwidget->widget()->inherits("QLayout") == FALSE){
        ++it;
        continue;
      }
      debug("Closing Layout: %ld", key);
      if(WidgetList[fd]->find(key) != 0x0)
        delete WidgetList[fd]->find(key)->pwidget;
      else
        debug("PUKE: Could no longer find request widget: %ld", it.currentKey());
      if(WidgetList[fd]->find(key) != 0x0){
        WidgetList[fd]->remove(key); // In case delete didn't get it
      }
      ++it;
    }
    /*
     * reset
     */
    QIntDictIterator<WidgetS> it(*qidWS);
    while(it.current()){
      /*
       * Now that the layouts are gone, remove the widgets
       */
      long key = it.currentKey();
      debug("Closing: %ld", key);
      if(WidgetList[fd]->find(key) != 0x0)
        delete WidgetList[fd]->find(key)->pwidget;
      else
        debug("PUKE: Could no longer find request widget: %ld", it.currentKey());
      if(WidgetList[fd]->find(key) != 0x0){
        WidgetList[fd]->remove(key); // In case delete didn't get it
      }
      ++it;
    }

  }
  WidgetList.remove(fd);
  bClosing = FALSE;
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

PWidget *PukeController::id2pwidget(widgetId *pwi){ /*fold00*/
  PObject *obj = id2pobject(pwi);
  if(obj->widget()->isWidgetType())
    return (PWidget *) obj;
  else
    throw(errorNoSuchWidget(*pwi));
}
void PukeController::insertPObject(int fd, int iWinId, WidgetS *obj){ /*FOLD00*/
    if(WidgetList[fd] == NULL){
        QIntDict<WidgetS> *qidWS = new QIntDict<WidgetS>;
        qidWS->setAutoDelete(FALSE);
        WidgetList.insert(fd, qidWS);
    }
    WidgetList[fd]->insert(iWinId, obj);
}

void PukeController::messageHandler(int fd, PukeMessage *pm) { /*fold00*/
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
    wIret.iWinId = createWidget(wI, pm).iWinId; // Create the acutal pw
    
    PukeMessage pmRet;
    pmRet.iCommand = PUKE_WIDGET_CREATE_ACK;
    pmRet.iWinId = wIret.iWinId;
    pmRet.iArg = 0;
    pmRet.cArg = strdup(pm->cArg);
//    printf("Got: %s Copied: %s\n", pm->cArg, pmRet.cArg);
    pmRet.iTextSize = strlen(pm->cArg);
    emit outputMessage(fd, &pmRet);
    free(pmRet.cArg);
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
      pmRet.cArg = strdup("No Such Widget");
      pmRet.iCommand = PUKE_WIDGET_DELETE_ACK;
      warning("WidgetRunner: no such widget: %d", wI.iWinId);
    }
    emit outputMessage(fd, &pmRet);
    if(pmRet.iArg == 1)
        free(pmRet.cArg);
  }
  else if(pm->iCommand == PUKE_WIDGET_LOAD){
    PukeMessage pmRet = *pm;
    KDynamicHandle handle;
//    const char *error;
    PObject *(*wc)(CreateArgs &ca);
    widgetCreate *wC;

    if(pm->iTextSize == 0){
      emit(errorCommandFailed(-pm->iCommand, 1));
      return;
    }

    handle = KDynamicLibrary::loadLibrary(kSircConfig->kdedir + "/lib/lib" + QString(pm->cArg), KDynamicLibrary::ResolveLazy);
    if (!handle) {
      emit(errorCommandFailed(-pm->iCommand, 1));
      return;
    }
    wc =  (PObject *(*)(CreateArgs &ca) )
        KDynamicLibrary::getSymbol(handle, "createWidget");

    wC = new widgetCreate;
    wC->wc = wc;
    wC->dlhandle = handle;
    widgetCF.insert(pm->iArg, wC);

    pmRet.iCommand = -pm->iCommand;
    pmRet.iTextSize = 0;
    emit outputMessage(fd, &pmRet);
  }
  else if(pm->iCommand == PUKE_WIDGET_UNLOAD){
//    const char *error;
    if(widgetCF[pm->iArg]){
        /*
      dlclose(widgetCF[pm->iArg]->dlhandle);
      if ((error = dlerror()) != NULL)  {
	fputs(error, stderr);
	pm->iCommand = -pm->iCommand;
	pm->iArg = -1;
	emit outputMessage(fd, pm);
	return;
        }
        */
      KDynamicLibrary::unloadLibrary(widgetCF[pm->iArg]->dlhandle);
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
      pmRet.iTextSize = 0;
      emit outputMessage(fd, &pmRet);
    }
  }
}

widgetId PukeController::createWidget(widgetId wI, PukeMessage *pm) /*fold00*/
{
  widgetId wIret;
  PWidget *parent = 0; // Defaults to no parent
  WidgetS *ws = new WidgetS;

  /*
   * The parent widget ID and type are packed into the iArg
   * the pattern is 2 shorts.
   */

  unsigned short int *iaArg;
  iaArg = (unsigned short int *) &pm->iArg;
  int iParent = iaArg[1];
  int iType = iaArg[0];

  debug("Creating new widget, type: %d, parent: %d", iType, iParent);

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

  //  CreateArgs arg = CreateArgs(this, pm, &wIret, parent)
  CreateArgs arg(this, pm, &wIret, parent);
  ws->pwidget = (widgetCF[iType]->wc)(arg);
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

void PukeController::closeWidget(widgetId wI){ /*FOLD00*/
  // Check to make sure we have a valid widget
  // Then remove it from the list.
  // Don't delete it since well, we're being called since it's already 
  // being deleted.
  //  debug("Want to delete: %d", wI.iWinId);
  
  if(checkWidgetId(&wI) == TRUE){
    WidgetList[wI.fd]->remove(wI.iWinId);

    PukeMessage pmRet;
    memset(&pmRet, 0, sizeof(PukeMessage));
    pmRet.iCommand = PUKE_WIDGET_DELETE_ACK;
    pmRet.iWinId = wI.iWinId;
    pmRet.iArg = 0;
    emit outputMessage(wI.fd, &pmRet);
//    debug("Sent close message to %d, returning", pmRet.iWinId);
    return;
  }
  warning("WidgetRunner: widget delete %d %d failed", wI.fd, wI.iWinId);
}

