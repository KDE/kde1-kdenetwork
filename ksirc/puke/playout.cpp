#include <stdio.h>

#include "playout.h"
#include "commands.h"

PLayout::PLayout(QObject *pobject)
  : PObject(pobject)
{
  // Connect slots as needed
  setWidget(0);
}

PLayout::~PLayout()
{
  //  debug("PObject: in destructor");
  delete widget();
  layout = 0;
  setWidget(0);
}

PObject *PLayout::createWidget(CreateArgs &ca)
{
  PLayout *pw = new PLayout(ca.parent);
  QBoxLayout *qbl;
  int direction, border;
  // Retreive the border and direction information out of the
  // carg string
  if(sscanf(ca.pm->cArg, "%d-%d-", &direction, &border) < 2)
    throw(errorCommandFailed(-ca.pm->iCommand, -1));
  
  if((ca.parent != 0) &&
     (ca.parent->widget()->isWidgetType() == TRUE)){
    qbl = new QBoxLayout((QWidget *) ca.parent->widget(), (QBoxLayout::Direction) direction, border);
    //      debug("Creating layout with parent: %d", parent.iWinId);

  }
  else{
    qbl = new QBoxLayout((QBoxLayout::Direction) direction, border);
    //      debug("Creating layout NO PARENT", parent.iWinId);
  }
  pw->setWidget(qbl);
  pw->setWidgetId(ca.pwI);
  pw->setPukeController(ca.pc);
  return pw;
}

void PLayout::messageHandler(int fd, PukeMessage *pm)
{
  PukeMessage pmRet;
  
  if(pm->iCommand == PUKE_LAYOUT_DELETE){
    /**
     * Delete is diabled right now
     */
    /*
    if((pm->iArg < 0 ||
	(uint) pm->iArg > uiBaseLayoutId) ){
      pmRet.iArg = 1;
    }
    else{
      LayoutList.remove(pm->iArg);
      pmRet.iArg = 0;
      }
      */
    // Everything's done, generate reply
    pmRet.iCommand = PUKE_LAYOUT_DELETE_ACK;
    pmRet.iWinId = pm->iArg;
    pmRet.iArg = 0;
    emit outputMessage(fd, &pmRet);
  }
  else if(pm->iCommand == PUKE_LAYOUT_ADDWIDGET){
    widgetId wiWidget;
    wiWidget.fd = fd;
    wiWidget.iWinId = pm->iArg;
    PWidget *pw = controller()->id2pwidget(&wiWidget);
    //    debug("Adding widget with stretch: %d and align: %d", (int) pm->cArg[0],
    //	  (int) pm->cArg[1]);
    widget()->addWidget(pw->widget(), pm->cArg[0], pm->cArg[1]);

    pmRet.iCommand = PUKE_LAYOUT_ADDWIDGET_ACK; // ack the add widget
    pmRet.iWinId = pm->iWinId;
    pmRet.iArg = 0;
    pmRet.cArg[0] = 0;
    emit outputMessage(fd, &pmRet);
  }
  else if(pm->iCommand == PUKE_LAYOUT_ADDLAYOUT){
    PObject *pld = controller()->id2pobject(fd, pm->iWinId);
    PObject *pls = controller()->id2pobject(fd, pm->iArg);
    if( (pld->widget()->inherits("QBoxLayout") == FALSE) || (pls->widget()->inherits("QBoxLayout") == FALSE))
      throw(errorCommandFailed(PUKE_LAYOUT_ADDLAYOUT_ACK, 1));
    PLayout *plbd = (PLayout *) pld;
    PLayout *plbs = (PLayout *) pls;
    plbd->widget()->addLayout(plbs->widget(), pm->cArg[0]);

    pmRet.iCommand = PUKE_LAYOUT_ADDLAYOUT_ACK; // ack the add widget
    pmRet.iWinId = pm->iWinId;
    pmRet.iArg = 0;
    pmRet.cArg[0] = 0;
    emit outputMessage(fd, &pmRet);
  }
  else if(pm->iCommand == PUKE_LAYOUT_ADDSTRUT){
    PObject *po = controller()->id2pobject(fd, pm->iWinId);
    if(po->widget()->inherits("PBoxLayout") != TRUE)
      throw(errorCommandFailed(PUKE_LAYOUT_ADDSTRUT_ACK, 1));
    PLayout *pl = (PLayout *) po;
    
    pl->widget()->addStrut(pm->iArg);

    pmRet.iCommand = PUKE_LAYOUT_ADDSTRUT_ACK; // ack the add widget
    pmRet.iWinId = pm->iWinId;
    pmRet.cArg[0] = 0;
    emit outputMessage(fd, &pmRet);
  }
  else if(pm->iCommand == PUKE_LAYOUT_ACTIVATE){
    PObject *po = controller()->id2pobject(fd, pm->iWinId);
    if(po->widget()->inherits("PBoxLayout") != TRUE)
      throw(errorCommandFailed(PUKE_LAYOUT_ACTIVATE_ACK, 1));
    PLayout *pl = (PLayout *) po;

    pmRet.iArg = 0; // setup failure case
    pl->widget()->activate();

    pmRet.iCommand = PUKE_LAYOUT_ACTIVATE_ACK; // ack the add widget
    pmRet.iWinId = pm->iWinId;
    pmRet.cArg[0] = 0;
    emit outputMessage(fd, &pmRet);
  }
  else {
    PObject::messageHandler(fd, pm);
  }

}

void PLayout::setWidget(QBoxLayout *_layout)
{
  //  debug("PObject setwidget called");
  layout = _layout;
  if(layout != 0){
  }
  PObject::setWidget(_layout);
  
}

QBoxLayout *PLayout::widget()
{
  //  debug("PObject widget called");
  return layout;
}