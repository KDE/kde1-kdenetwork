#include "ptablevw.h"

PWidget *createWidget(widgetId *pwi, PWidget *parent)
{
  warning("Table View is abstract class, cannot create an object from it!!!");
  PTableView *pt = new PTableView(parent);
  pt->setWidget(0);
  pt->setWidgetId(pwi);
  return pt;
}


PTableView::PTableView(PWidget *parent)
  : PFrame(parent)
{
  //  debug("PTableView PTableView called");
  tbl = 0;
  setWidget(tbl);
}

PTableView::~PTableView()
{
  // don't delete the widget since none acutally exists.
  // delete widget();     // Delete the frame
  //  tbl=0;          // Set it to 0
  setWidget(tbl); // Now set all widget() calls to 0.
}

void PTableView::messageHandler(int fd, PukeMessage *pm)
{
  PukeMessage pmRet;
  switch(pm->iCommand){
      /*
       case PUKE_LINED_SET_MAXLENGTH:
       if(widget() == 0){
       debug("PTableView: No Widget set");
       return;
       }
       widget()->setMaxLength(pm->iArg);
       pmRet.iCommand = - pm->iCommand;
       pmRet.iWinId = - pm->iWinId;
       pmRet.iArg = widget()->maxLength();
       emit outputMessage(fd, &pmRet);
       break;
       */
  default:
    PFrame::messageHandler(fd, pm);
  }
}

void PTableView::setWidget(QTableView *_tbv)
{
  tbl = _tbv;
  if(tbl != 0){
  }
  PFrame::setWidget(_tbv);

}


QTableView *PTableView::widget()
{
  return tbl;
}
