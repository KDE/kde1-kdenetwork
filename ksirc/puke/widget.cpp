
#include "widget.h"

#include "pwidget.h"

#include "../config.h"

#include <dlfcn.h>

uint WidgetRunner::uiBaseWinId = 0;

WidgetRunner::WidgetRunner(QObject * parent=0, const char * name=0)
: QObject(parent, name)
{
  WidgetList.setAutoDelete(TRUE);
  widgetCF.setAutoDelete(TRUE);
  
  widgetCreate *wc;

  wc = new widgetCreate;
  wc->wc = PWidget::createWidget;
  widgetCF.insert(PWIDGET_WIDGET, wc);

  //  wc = new widgetCreate;
  //  wc->wc = PFrame::createWidget;
  //  widgetCF.insert(PWIDGET_FRAME, wc);

  
}

WidgetRunner::~WidgetRunner()
{
}

bool WidgetRunner::checkWidgetId(widgetId *pwi)
{
  if(WidgetList[pwi->fd] != NULL)
    if(WidgetList[pwi->fd]->find(pwi->iWinId) != NULL)
      return TRUE;
  
  return FALSE;
}

PWidget *WidgetRunner::id2pwidget(widgetId *pwi){
  if(checkWidgetId(pwi) == TRUE){
    return WidgetList[pwi->fd]->find(pwi->iWinId)->pwidget;
  }
  return 0;
}

void WidgetRunner::inputMessage(int fd, PukeMessage *pm){
  widgetId wI, wIret;
  wI.fd = fd;
  wI.iWinId = pm->iWinId;
  if(pm->iCommand == PUKE_WIDGET_CREATE){
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
    PukeMessage pmRet = *pm;
    QIntDict<WidgetS> *qidWS = WidgetList[fd];
    if(qidWS == 0){
      debug("WidgetRunner:: no such set of widget descriptors?");
      return;
    }
    if(qidWS->find(wI.iWinId)){
      debug("Closing: %d", wI.iWinId);
      // Remove the list item then delete the widget.  This will stop
      // the destroyed signal from trying to remove it again.
      PWidget *pw = qidWS->find(wI.iWinId)->pwidget;
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
    PWidget *(*wc)(widgetId *wI, PWidget *parent);
    widgetCreate *wC;

    pm->cArg[49] = 0;
    handle = dlopen(kSircConfig->kdedir + "/share/apps/ksirc/" + QString(pm->cArg), RTLD_LAZY|RTLD_GLOBAL);
    if (!handle) {
      fputs(dlerror(), stderr);
      fputs("\n", stderr);
      goto load_barfed; // OK I SHOULD USE EXCEPTIONS!!!
    }
    wc =  (PWidget *(*)(widgetId *wI, PWidget *parent) )
      dlsym(handle, "createWidget");
    if ((error = dlerror()) != NULL)  {
      fputs(error, stderr);
      goto load_barfed; // Oh shutup ;)
    }
    wC = new widgetCreate;
    wC->wc = wc;
    wC->dlhandle = handle;
    widgetCF.insert(pm->iArg, wC);
    
    pmRet.iCommand = -pm->iCommand;
    emit outputMessage(fd, &pmRet);
    goto finish;
  load_barfed: 
    pmRet.iCommand = -pm->iCommand;
    pmRet.iArg = 1;
    emit outputMessage(fd, &pmRet);
  finish:
    if(1){}  // I hate goto's
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

void WidgetRunner::closeWidget(widgetId wI){
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

widgetId WidgetRunner::createWidget(widgetId wI, int iType)
{
  widgetId wIret;
  PWidget *parent = 0; // Defaults to no parent
  WidgetS *ws = new WidgetS;
  
  if(widgetCF[iType] == NULL){ // No such widget, bail out.
    wIret.fd = 0;
    wIret.iWinId = 0;
    return wIret;
  }
  
  uiBaseWinId++; // Get a new base win id

  // wIret holds the current widget id for the new widget
  wIret.iWinId = uiBaseWinId;
  wIret.fd = wI.fd;
  
  if(checkWidgetId(&wI) == TRUE)
    parent = WidgetList[wI.fd]->find(wI.iWinId)->pwidget;

  ws->pwidget = (widgetCF[iType]->wc)(&wIret, parent);
  ws->type = iType;
  connect(ws->pwidget, SIGNAL(widgetDestroyed(widgetId)),
	  this, SLOT(closeWidget(widgetId)));

  connect(ws->pwidget, SIGNAL(outputMessage(int, PukeMessage*)),
	  this, SIGNAL(outputMessage(int, PukeMessage*)));

  if(WidgetList[wI.fd] == NULL){
    QIntDict<WidgetS> *qidWS = new QIntDict<WidgetS>;
    qidWS->setAutoDelete(TRUE);
    WidgetList.insert(wI.fd, qidWS);
  }
  WidgetList[wI.fd]->insert(uiBaseWinId, ws);

  return wIret;
}

void WidgetRunner::closefd(int fd)
{
  QIntDict<WidgetS> *qidWS = WidgetList[fd];
  if(qidWS == 0){
    debug("WidgetRunner:: Close called twice?");
    return;
  }
  QIntDictIterator<WidgetS> it(*qidWS);
  if(it.count() == 0){
    debug("WidgetRunner: nothing left to delete\n");
  }
  else{
    while(it.current()){
      debug("Closing: %ld", it.currentKey());
      delete WidgetList[fd]->find(it.currentKey())->pwidget;
      // deleting the widget gets it removed from the WidgetList[fd]
      //    WidgetList[fd]->remove(it.currentKey());
      ++it;
    }
  }
  WidgetList.remove(fd);
}

