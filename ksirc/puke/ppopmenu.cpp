#include "ppopmenu.h"

PObject *createWidget(CreateArgs &ca)
{
  PPopupMenu *pm = new PPopupMenu(ca.parent);
  QPopupMenu *qpm;
  if(ca.fetchedObj != 0 && ca.fetchedObj->inherits("QPopupMenu") == TRUE)
    qpm= (QPopupMenu *) ca.fetchedObj;
  if(ca.parent != 0 && ca.parent->widget()->isWidgetType() == TRUE)
    qpm = new QPopupMenu((QWidget *) ca.parent->widget());
  else
    qpm = new QPopupMenu();
  pm->setWidget(qpm);
  pm->setWidgetId(ca.pwI);
  return pm;
}


PPopupMenu::PPopupMenu(PObject *parent)
  : PTableView(parent)
{
  //  debug("PLineEdit PLineEdit called");
  menu = 0;
  setWidget(menu);
  pmd = new PMenuData(this);
}

PPopupMenu::~PPopupMenu()
{
  //  debug("PLineEdit: in destructor"); 
  delete widget();     // Delete the frame
  menu = 0;          // Set it to 0
  setWidget(menu); // Now set all widget() calls to 0.
  delete pmd;
}

void PPopupMenu::messageHandler(int fd, PukeMessage *pm)
{
  PukeMessage pmRet;
  switch(pm->iCommand){
    /*
  case PUKE_LINED_SET_MAXLENGTH:
    if(widget() == 0){
      debug("PLineEdit: No Widget set");
      return;
    }
    widget()->setMaxLength(pm->iArg);
    pmRet.iCommand = - pm->iCommand;
    pmRet.iWinId = pm->iWinId;
    pmRet.iArg = widget()->maxLength();
    emit outputMessage(fd, &pmRet);
    break;
    */
  default:
    if(pmd->messageHandler(fd, pm) == FALSE) // Call pmd's even filter
      PTableView::messageHandler(fd, pm);
  }
}

void PPopupMenu::setWidget(QPopupMenu *_menu)
{
  menu = _menu;
  PTableView::setWidget(menu);
}


QPopupMenu *PPopupMenu::widget()
{
  return menu;
}
