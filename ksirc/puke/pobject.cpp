#include "pobject.h"
#include "commands.h"

PObject::PObject(QObject *pobject) /*FOLD00*/
  : QObject(pobject)
{
  // Connect slots as needed
  obj = 0;
  setWidget(0);
}

PObject::~PObject() /*FOLD00*/
{
  //  debug("PObject: in destructor");
  delete widget();
  obj = 0;
  setWidget(0);
}

PObject *PObject::createWidget(CreateArgs &ca) /*FOLD00*/
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

void PObject::messageHandler(int fd, PukeMessage *pm) /*FOLD00*/
{
  PukeMessage pmRet;
  warning("PObject: Unkown Command: %d", pm->iCommand);
  pmRet.iCommand = PUKE_INVALID;
  pmRet.iWinId = pm->iWinId;
  pmRet.iArg = 0;
  pmRet.cArg[0] = 0;
  emit outputMessage(fd, &pmRet);
}

void PObject::setWidget(QObject *_o) /*FOLD00*/
{
  //  debug("PObject setwidget called");
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

void PObject::setWidgetId(widgetId *pwI) /*FOLD00*/
{
  wI = *pwI;
  //  debug("PObject: set widget id %d", wI.iWinId);
}

widgetId PObject::widgetIden() /*FOLD00*/
{
  //  debug("PObject: called widget id %d", wI.iWinId);
  return wI;
}

void PObject::swidgetDestroyed(){ /*FOLD00*/
  //  debug("PObject: got destroy %d", widgetIden().iWinId);
  emit widgetDestroyed(widgetIden());
}

PukeController *PObject::controller() { /*FOLD00*/
  return pController;
}