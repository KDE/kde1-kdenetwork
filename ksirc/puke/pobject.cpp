#include "pobject.h"
#include "commands.h"

PObject::PObject(QObject *pobject, const char *name) /*fold00*/
  : QObject(pobject, name)
{
  // Connect slots as needed
  obj = 0;
  setWidget(0);
  manualTerm = FALSE;
}

PObject::~PObject() /*FOLD00*/
{
  debug("PObject: in destructor");
  delete widget();
  obj = 0;
  setWidget(0);
  debug("PObject: done destructor");
}

PObject *PObject::createWidget(CreateArgs &ca) /*fold00*/
{
  PObject *pw = new PObject(ca.parent);
  QObject *o;
  if(ca.parent != 0)
    o = new QObject(ca.parent->widget());
  else
    o = new QObject();
  pw->setWidget(o);
  pw->setWidgetId(ca.pwI);
  pw->setPukeController(ca.pc);
  return pw;
}

void PObject::messageHandler(int fd, PukeMessage *pm) /*fold00*/
{
  PukeMessage pmRet;
  if(pm->iCommand == PUKE_WIDGET_DELETE){
    /**
     * Emit the ack before the delete since we don't exist afterwards.
     */
    pmRet.iCommand = PUKE_WIDGET_DELETE_ACK;
    pmRet.iWinId = pm->iWinId;
    pmRet.iArg = 0;
    pmRet.cArg = 0;
    emit outputMessage(fd, &pmRet);

    manTerm();
    delete this;
  }
  if(pm->iCommand == PUKE_RELEASEWIDGET){
    /**
     * Emit the ack before the delete since we don't exist afterwards.
     */
    pmRet.iCommand = PUKE_RELEASEWIDGET_ACK;
    pmRet.iWinId = pm->iWinId;
    pmRet.iArg = 0;
    pmRet.cArg = 0;
    emit outputMessage(fd, &pmRet);

    /**
     * By setting the widget to 0 we loose the pointer and then don't delete it
     */
    setWidget(0);
    manTerm();
    delete this;
  }
  else {
    warning("PObject: Unkown Command: %d", pm->iCommand);
    pmRet.iCommand = PUKE_INVALID;
    pmRet.iWinId = pm->iWinId;
    pmRet.iArg = 0;
    pmRet.cArg = 0;
    emit outputMessage(fd, &pmRet);
  }
}

void PObject::setWidget(QObject *_o) /*FOLD00*/
{
  // Disconnect everything from the object we where listning too
  // Just in case it fires something off, we don't want to get it
  debug("PObject: in setWidget");
  if(widget() != 0){
    disconnect(widget(), SIGNAL(destroyed()),
               this, SLOT(swidgetDestroyed()));
  }
  
  obj = _o;
  if(obj != 0){
    connect(widget(), SIGNAL(destroyed()),
	    this, SLOT(swidgetDestroyed()));
  }
}

QObject *PObject::widget() /*FOLD00*/
{
  //  debug("PObject widget called");
  return obj;
}

void PObject::setWidgetId(widgetId *pwI) /*fold00*/
{
  wI = *pwI;
  //  debug("PObject: set widget id %d", wI.iWinId);
}

widgetId PObject::widgetIden() /*fold00*/
{
  //  debug("PObject: called widget id %d", wI.iWinId);
  return wI;
}

void PObject::swidgetDestroyed(){ /*FOLD00*/
  debug("PObject: got destroy %d", widgetIden().iWinId);
  setWidget(0x0);
  debug("PObject: widget pointer: %d", this->widget());
  if(manualTerm == FALSE){
    manTerm();
    debug("PObject: delete this");
    delete this;
  }
  debug("PObject: done destroy");
}

PukeController *PObject::controller() { /*FOLD00*/
  
  return pController;
}

void PObject::manTerm() {
  manualTerm = TRUE;
}