
#include "layout.h"

uint LayoutRunner::uiBaseLayoutId = 1;

LayoutRunner::LayoutRunner(WidgetRunner *_wr, QObject * parent=0, 
			   const char * name=0)
: QObject(parent, name)
{
  LayoutList.setAutoDelete(TRUE);
  wrControl = _wr;
}

LayoutRunner::~LayoutRunner()
{
}

bool LayoutRunner::checkLayoutId(widgetId *pwi)
{
  if(LayoutList[pwi->iWinId] != NULL)
      return TRUE;
  
  return FALSE;
}

QBoxLayout *LayoutRunner::id2layout(widgetId *pwi){
  return LayoutList[pwi->iWinId];
}

void LayoutRunner::inputMessage(int fd, PukeMessage *pm){
  PukeMessage pmRet;
  widgetId wI, wIret;
  wI.fd = fd;
  wI.iWinId = pm->iWinId;

  if(pm->iCommand == PUKE_LAYOUT_NEW){
    uiBaseLayoutId++; // Get a new base win id
    QBoxLayout *qbl;
    widgetId parent;
    PWidget *pw;
    unsigned short int *iArg;
    iArg = (unsigned short int *) &pm->iArg;
    parent.fd = fd;
    parent.iWinId = pm->iWinId;
    if((pw = wrControl->id2pwidget(&parent)) != 0){
      qbl = new QBoxLayout(pw->widget(), (QBoxLayout::Direction) iArg[0], (int) iArg[1]);
      //      debug("Creating layout with parent: %d", parent.iWinId);
      
    }
    else{
      qbl = new QBoxLayout((QBoxLayout::Direction)iArg[0], (int) iArg[1]);
      //      debug("Creating layout NO PARENT", parent.iWinId);
    }
    LayoutList.insert(uiBaseLayoutId, qbl);
    wIret.iWinId = uiBaseLayoutId;

    // Everything's done, generate reply
    pmRet.iCommand = PUKE_LAYOUT_NEW_ACK;
    pmRet.iWinId = wIret.iWinId;
    pmRet.iArg = 0;
    strncpy(pmRet.cArg, pm->cArg, 50);
    //    debug("Create layout after cArg: %s", pm->cArg);
    //    debug("Create layout pmRet.cArg: %s", pmRet.cArg);
    emit outputMessage(fd, &pmRet);
  }
  else if(pm->iCommand == PUKE_LAYOUT_DELETE){
    if((pm->iArg < 0 ||
        (uint) pm->iArg > uiBaseLayoutId) ){
      pmRet.iArg = 1;
    }
    else{
      LayoutList.remove(pm->iArg);
      pmRet.iArg = 0;
    }
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
    PWidget *pw = wrControl->id2pwidget(&wiWidget);
    QBoxLayout *qlb = LayoutList[pm->iWinId];
    if((pw == 0) ||
       (qlb == 0)){
      warning("Invalid widget for layout %d: %d %d", pm->iWinId, (int) pw, (int) qlb);
      pmRet.iCommand = PUKE_LAYOUT_ADDWIDGET_ACK; // Bail out invalid widget
      pmRet.iWinId = pm->iWinId;
      pmRet.iArg = 1;
      strcpy(pmRet.cArg, "Invalid Widget");
      emit outputMessage(fd, &pmRet);
      return;
    }
    //    debug("Adding widget with stretch: %d and align: %d", (int) pm->cArg[0],
    //	  (int) pm->cArg[1]);
    qlb->addWidget(pw->widget(), pm->cArg[0], pm->cArg[1]);

    pmRet.iCommand = PUKE_LAYOUT_ADDWIDGET_ACK; // ack the add widget
    pmRet.iWinId = pm->iWinId;
    pmRet.iArg = 0;
    pmRet.cArg[0] = 0;
    emit outputMessage(fd, &pmRet);
  }
  else if(pm->iCommand == PUKE_LAYOUT_ADDLAYOUT){
    QBoxLayout *qlbd = LayoutList[pm->iWinId];    
    QBoxLayout *qlbs = LayoutList[pm->iArg];
    if((qlbs == 0) ||
       (qlbd == 0)){
      pmRet.iCommand = PUKE_LAYOUT_ADDLAYOUT_ACK; // Bail out invalid layout
      pmRet.iWinId = pm->iWinId;
      pmRet.iArg = 1;
      pmRet.cArg[0] = 0;
      emit outputMessage(fd, &pmRet);
      warning("PBoxLayout: Attemped to add invalid layout");
      return;
    }
    qlbd->addLayout(qlbs, pm->cArg[0]);

    pmRet.iCommand = PUKE_LAYOUT_ADDLAYOUT_ACK; // ack the add widget
    pmRet.iWinId = pm->iWinId;
    pmRet.iArg = 0;
    pmRet.cArg[0] = 0;
    emit outputMessage(fd, &pmRet);
  }
  else if(pm->iCommand == PUKE_LAYOUT_ADDSTRUT){
    QBoxLayout *qlb = LayoutList[pm->iWinId];

    pmRet.iArg = 0; // setup failure case
    if(qlb != 0x0)
      qlb->addStrut(pm->iArg);
    else
      pmRet.iArg = 1;

    pmRet.iCommand = PUKE_LAYOUT_ADDSTRUT_ACK; // ack the add widget
    pmRet.iWinId = pm->iWinId;
    pmRet.cArg[0] = 0;
    emit outputMessage(fd, &pmRet);
  }
  else if(pm->iCommand == PUKE_LAYOUT_ACTIVATE){
    QBoxLayout *qlb = LayoutList[pm->iWinId];

    pmRet.iArg = 0; // setup failure case
    if(qlb != 0x0)
      qlb->activate();
    else
      pmRet.iArg = 1;

    pmRet.iCommand = PUKE_LAYOUT_ACTIVATE_ACK; // ack the add widget
    pmRet.iWinId = pm->iWinId;
    pmRet.cArg[0] = 0;
    emit outputMessage(fd, &pmRet);
  }

}


