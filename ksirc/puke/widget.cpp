
#include "widget.h"

#include "pwidget.h"


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

  wc = new widgetCreate;
  wc->wc = PFrame::createWidget;
  widgetCF.insert(PWIDGET_FRAME, wc);

  
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
    debug("Create widget before cArg: %s", pm->cArg);
    strncpy(pmRet.cArg, pm->cArg, 50);
    debug("Create widget after cArg: %s", pm->cArg);
    debug("Create widget pmRet.cArg: %s", pmRet.cArg);
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
      delete qidWS->find(wI.iWinId)->pwidget;
      WidgetList[fd]->remove(wI.iWinId);
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
    debug("WidgetRunner: emtpy set?!?!?\n");
  }
  while(it.current()){
    debug("Closing: %ld", it.currentKey());
    delete WidgetList[fd]->find(it.currentKey())->pwidget;
    WidgetList[fd]->remove(it.currentKey());
    ++it;
  }
  WidgetList.remove(fd);
}

