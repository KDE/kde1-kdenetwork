#include "pwidget.h"
#include "commands.h"

#include <qevent.h>

PWidget::PWidget(PWidget *)
  : QObject()
{
  debug("PWidget constructor called");

  w = 0;

  eventList[0] = eventNone;
  eventList[1] = eventTimer;
  eventList[2] = eventMouse;
  eventList[3] = eventMouse;
  eventList[4] = eventMouse;
  eventList[5] = eventMouse;
  eventList[6] = eventKey;
  eventList[7] = eventKey;
  eventList[8] = eventFocus;
  eventList[9] = eventFocus;
  eventList[10] = eventFocus;
  eventList[11] = eventFocus;
  eventList[12] = eventPaint;
  eventList[13] = eventMove;
  eventList[14] = eventResize;
  eventList[15] = eventNone;
  eventList[16] = eventNone;
  eventList[17] = eventNone;
  eventList[18] = eventNone;
  eventList[19] = eventNone;

  // Connect slots as needed

}

PWidget::~PWidget()
{
  debug("PWidget: in destructor");
  delete widget();
  w = 0;
  setWidget(0);
}

PWidget *PWidget::createWidget(widgetId *pwi, PWidget *parent)
{
  PWidget *pw = new PWidget(parent);
  QWidget *tw;
  if(parent != 0)
    tw = new QWidget(parent->widget());
  else
    tw = new QWidget();
  pw->setWidget(tw);
  pw->setWidgetId(pwi);
  return pw;
}

void PWidget::messageHandler(int fd, PukeMessage *pm)
{
  debug("PWidget handler called");
  PukeMessage pmRet;
  switch(pm->iCommand){
  case PUKE_WIDGET_SHOW:
    widget()->show();
    pmRet.iCommand = PUKE_WIDGET_SHOW_ACK;
    pmRet.iWinId = pm->iWinId;
    pmRet.iArg = 0;
    pmRet.cArg[0] = 0;
    emit outputMessage(fd, &pmRet);
    break;
  case PUKE_WIDGET_HIDE:
    widget()->hide();
    pmRet.iCommand = PUKE_WIDGET_HIDE_ACK;
    pmRet.iWinId = pm->iWinId;
    pmRet.iArg = 0;
    pmRet.cArg[0] = 0;
    emit outputMessage(fd, &pmRet);
    break;    
  case PUKE_WIDGET_REPAINT:
    widget()->repaint(pm->iArg);
    pmRet.iCommand = PUKE_WIDGET_REPAINT_ACK;
    pmRet.iWinId = pm->iWinId;
    pmRet.iArg = 0;
    pmRet.cArg[0] = 0;
    emit outputMessage(fd, &pmRet);
    break;
  case PUKE_WIDGET_RESIZE:
    {
      unsigned short int *size;
      size = (unsigned short int *) &pm->iArg;
      debug("Resizing to: %d => %d %d", pm->iArg, size[0], size[1]);
      widget()->resize(size[0], size[1]);
      size[0] = (short) widget()->height();
      size[1] = (short) widget()->width();
      pmRet.iCommand = PUKE_WIDGET_RESIZE_ACK;
      pmRet.iWinId = pm->iWinId;      
      pmRet.iArg = pm->iArg;
      pmRet.cArg[0] = 0;
      emit outputMessage(fd, &pmRet);
    }
    break;
  case PUKE_WIDGET_MOVE:
    {
      unsigned short int *pos;
      pos = (unsigned short int *) &pm->iArg;
      debug("Moving to: %d => %d %d", pm->iArg, pos[0], pos[1]);
      widget()->move(pos[0], pos[1]);
      pos[0] = (short) widget()->x();
      pos[1] = (short) widget()->y();
      pmRet.iCommand = PUKE_WIDGET_MOVE_ACK;
      pmRet.iWinId = pm->iWinId;      
      pmRet.iArg = pm->iArg;
      pmRet.cArg[0] = 0;
      emit outputMessage(fd, &pmRet);
    }
    break;
  default:
    warning("PWidget: Unkown Command: %d", pm->iCommand);
    pmRet.iCommand = PUKE_INVALID;
    pmRet.iWinId = pm->iWinId;
    pmRet.iArg = 0;
    pmRet.cArg[0] = 0;
    emit outputMessage(fd, &pmRet);
  }
}

void PWidget::setWidget(QWidget *_w)
{
  debug("PWidget setwidget called");
  w = _w;
  if(w != 0){
    widget()->installEventFilter(this);
    connect(widget(), SIGNAL(destroyed()),
	    this, SLOT(swidgetDestroyed()));
  }
}

QWidget *PWidget::widget()
{
  debug("PWidget widget called");
  return w;
}

void PWidget::setWidgetId(widgetId *pwI)
{
  wI = *pwI;
  debug("PWidget: set widget id %d", wI.iWinId);
}

widgetId PWidget::widgetIden()
{
  debug("PWidget: called widget id %d", wI.iWinId);
  return wI;
}

void PWidget::swidgetDestroyed(){
  debug("PWidget: got destroy %d", widgetIden().iWinId);
  emit widgetDestroyed(widgetIden());
}

// PWidget specific
bool PWidget::eventFilter(QObject *o, QEvent *e)
{
  debug("PWidget: Got event: %d", e->type());
  if(e->type() < 20 && e->type() >= 0){
    eventList[e->type()](o,e);
  }
  else{
    PukeMessage pm;
    widgetId wI;
    
    wI = widgetIden();
    pm.iCommand = PUKE_EVENT_UNKOWN;
    pm.iWinId = wI.iWinId;
    pm.iArg = e->type();
    pm.cArg[0] = 0;
    emit outputMessage(wI.fd, &pm);

  }

  return false;
}

void PWidget::eventNone(QObject *, QEvent *e)  
{
  PukeMessage pm;
  widgetId wI;

  debug("PWidget: eventNone");
  
  wI = widgetIden();
  pm.iCommand = - e->type() - 1020; // 1030 offset for events
  pm.iWinId = wI.iWinId;
  pm.iArg = 0;
  pm.cArg[0] = 0;

  emit outputMessage(wI.fd, &pm);
  
}

void PWidget::eventTimer(QObject *, QEvent *e)  
{
  PukeMessage pm;
  widgetId wI;

  QTimerEvent *et = Q_TIMER_EVENT(e);
  
  wI = widgetIden();
  pm.iCommand = PUKE_WIDGET_EVENT_TIMER;
  pm.iWinId = wI.iWinId;
  pm.iArg = et->timerId();
  pm.cArg[0] = 0;

  emit outputMessage(wI.fd, &pm);
  
}

void PWidget::eventMouse(QObject *, QEvent *e)  
{
  PukeMessage pm;
  widgetId wI;

  QMouseEvent *me = Q_MOUSE_EVENT(e);
  
  wI = widgetIden();
  pm.iCommand = - e->type() - 1020; // 1020 offset for events
  pm.iWinId = wI.iWinId;
  pm.iArg = 0;

  // special cArg handling
  int *icArg = (int *) &pm.cArg;
  icArg[0] = me->x();
  icArg[1] = me->y();
  icArg[2] = me->button();
  icArg[3] = me->state();

  emit outputMessage(wI.fd, &pm);
  
}

void PWidget::eventKey(QObject *, QEvent *e)  
{
  PukeMessage pm;
  widgetId wI;

  QKeyEvent *ke = Q_KEY_EVENT(e);
  
  wI = widgetIden();
  pm.iCommand = - e->type() - 1020; // 1020 offset for events
  pm.iWinId = wI.iWinId;
  pm.iArg = 0;

  // special cArg handling
  int *icArg = (int *) &pm.cArg;
  icArg[0] = ke->key();
  icArg[1] = ke->ascii();
  icArg[2] = ke->state();

  emit outputMessage(wI.fd, &pm);
  
}

void PWidget::eventFocus(QObject *, QEvent *e)  
{
  PukeMessage pm;
  widgetId wI;

  debug("PWidget: eventFocus");

  QFocusEvent *fe = Q_FOCUS_EVENT(e);
  
  wI = widgetIden();
  pm.iCommand = - e->type() - 1020; // 1020 offset for events
  pm.iWinId = wI.iWinId;
  pm.iArg = 0;
  pm.cArg[0] = fe->gotFocus();
  pm.cArg[1] = fe->lostFocus();

  emit outputMessage(wI.fd, &pm);
  
}

void PWidget::eventPaint(QObject *, QEvent *e)  
{
  PukeMessage pm;
  widgetId wI;

  wI = widgetIden();
  pm.iCommand = - e->type() - 1020; // 1020 offset for events
  pm.iWinId = wI.iWinId;
  pm.iArg = 0;
  pm.cArg[0] = 0;

  emit outputMessage(wI.fd, &pm);
  
}

void PWidget::eventMove(QObject *, QEvent *e)  
{
  PukeMessage pm;
  widgetId wI;

  QMoveEvent *me = Q_MOVE_EVENT(e);
  
  wI = widgetIden();
  pm.iCommand = - e->type() - 1020; // 1020 offset for events
  pm.iWinId = wI.iWinId;
  pm.iArg = 0;

  // special cArg handling
  int *icArg = (int *) &pm.cArg;
  icArg[0] = me->pos().x();
  icArg[1] = me->pos().y();
  icArg[2] = me->oldPos().x();
  icArg[3] = me->oldPos().y();

  emit outputMessage(wI.fd, &pm);
  
}

void PWidget::eventResize(QObject *, QEvent *e)  
{
  PukeMessage pm;
  widgetId wI;

  QResizeEvent *re = Q_RESIZE_EVENT(e);
  
  wI = widgetIden();
  pm.iCommand = - e->type() - 1020; // 1020 offset for events
  pm.iWinId = wI.iWinId;
  pm.iArg = 0;

  // special cArg handling
  int *icArg = (int *) &pm.cArg;
  icArg[0] = re->size().height();
  icArg[1] = re->size().width();
  icArg[2] = re->oldSize().height();
  icArg[3] = re->oldSize().width();

  emit outputMessage(wI.fd, &pm);
  
}

