#include "pobject.h"
#include "commands.h"

PObject::PObject(QObject *pobject)
  : QObject(pobject)
{
  // Connect slots as needed

}

PObject::~PObject()
{
  //  debug("PObject: in destructor");
  delete widget();
  obj = 0;
  setWidget(0);
}

PObject *PObject::createWidget(widgetId *pwi, PObject *parent)
{
  PObject *pw = new PObject(parent);
  QObject *o;
  if(parent != 0)
    o = new QObject(parent->widget());
  else
    o = new QObject();
  pw->setWidget(o);
  pw->setWidgetId(pwi);
  return pw;
}

void PObject::messageHandler(int fd, PukeMessage *pm)
{
  PukeMessage pmRet;
  warning("PObject: Unkown Command: %d", pm->iCommand);
  pmRet.iCommand = PUKE_INVALID;
  pmRet.iWinId = pm->iWinId;
  pmRet.iArg = 0;
  pmRet.cArg[0] = 0;
  emit outputMessage(fd, &pmRet);
}

void PObject::setWidget(QObject *_o)
{
  //  debug("PObject setwidget called");
  obj = _o;
  if(obj != 0){
    connect(widget(), SIGNAL(destroyed()),
	    this, SLOT(swidgetDestroyed()));
  }
}

QObject *PObject::widget()
{
  //  debug("PObject widget called");
  return obj;
}

void PObject::setWidgetId(widgetId *pwI)
{
  wI = *pwI;
  //  debug("PObject: set widget id %d", wI.iWinId);
}

widgetId PObject::widgetIden()
{
  //  debug("PObject: called widget id %d", wI.iWinId);
  return wI;
}

void PObject::swidgetDestroyed(){
  //  debug("PObject: got destroy %d", widgetIden().iWinId);
  emit widgetDestroyed(widgetIden());
}

