#include "plistbox.h"

PWidget *createWidget(widgetId *pwi, PWidget *parent) /*FOLD00*/
{
  PListBox *plb = new PListBox(parent);
  QListBox *lb;
  if(parent != 0)
    lb = new QListBox(parent->widget());
  else
    lb = new QListBox();
  plb->setWidget(lb);
  plb->setWidgetId(pwi);
  return plb;
}


PListBox::PListBox(PWidget *parent) /*FOLD00*/
  : PTableView(parent)
{
  //  debug("PListBox PListBox called");
  lb = 0;
  setWidget(lb);
}

PListBox::~PListBox() /*FOLD00*/
{
  //  debug("PListBox: in destructor"); 
  delete widget();     // Delete the frame
  lb=0;          // Set it to 0
  setWidget(lb); // Now set all widget() calls to 0.
}

void PListBox::messageHandler(int fd, PukeMessage *pm) /*FOLD00*/
{
  PukeMessage pmRet;
  switch(pm->iCommand){
  case PUKE_LISTBOX_INSERT:
    if(!checkWidget())
      return;
    
    widget()->insertItem(pm->cArg, pm->iArg);
    pmRet.iCommand = - pm->iCommand;
    pmRet.iWinId = pm->iWinId;
    pmRet.iArg = widget()->count();
    pmRet.cArg[0] = 0;
    emit outputMessage(fd, &pmRet);
    break;
  case PUKE_LISTBOX_INSERT_SORT:
    if(!checkWidget())
      return;
    
    widget()->inSort(pm->cArg);
    pmRet.iCommand = - pm->iCommand;
    pmRet.iWinId = pm->iWinId;
    pmRet.iArg = widget()->count();
    pmRet.cArg[0] = 0;
    emit outputMessage(fd, &pmRet);
    break;
  case PUKE_LISTBOX_INSERT_PIXMAP:
    if(!checkWidget())
      return;
    
    widget()->insertItem(QPixmap(pm->cArg), pm->iArg);
    pmRet.iCommand = - pm->iCommand;
    pmRet.iWinId = pm->iWinId;
    pmRet.iArg = widget()->count();
    pmRet.cArg[0] = 0;
    emit outputMessage(fd, &pmRet);
    break;
  case PUKE_LISTBOX_HIGHLIGHT:
    if(!checkWidget())
      return;
    
    widget()->setCurrentItem(pm->iArg);
    pmRet.iCommand = - pm->iCommand;
    pmRet.iWinId = pm->iWinId;
    pmRet.iArg = widget()->currentItem();
    pmRet.cArg[0] = 0;
    emit outputMessage(fd, &pmRet);
    break;
  case PUKE_LISTBOX_REMOVE:
    if(!checkWidget())
      return;
    
    widget()->removeItem(pm->iArg);
    
    pmRet.iCommand = - pm->iCommand;
    pmRet.iWinId = pm->iWinId;
    pmRet.iArg = 0;
    pmRet.cArg[0] = 0;
    emit outputMessage(fd, &pmRet);
    break;

  default:
    PTableView::messageHandler(fd, pm);
  }
}

void PListBox::setWidget(QListBox *_lb) /*FOLD00*/
{
  lb = _lb;
  if(lb != 0){
    connect(lb, SIGNAL(highlighted(int)),
            this, SLOT(highlighted(int)));
    connect(lb, SIGNAL(selected(int)),
            this, SLOT(selected(int)));
  }
  PTableView::setWidget(lb);

}


QListBox *PListBox::widget() /*FOLD00*/
{
  return lb;
}


void PListBox::highlighted(int index) { /*FOLD00*/
  PukeMessage pmRet;

  debug("Got highlight");
  pmRet.iCommand = PUKE_LISTBOX_HIGHLIGHTED_ACK;
  pmRet.iWinId = widgetIden().iWinId;
  pmRet.iArg = index;
  if(widget()->text(index) != 0)
    strncpy(pmRet.cArg, widget()->text(index), 50);
  else
    pmRet.cArg[0] = 0;
  emit outputMessage(widgetIden().fd, &pmRet);
}

void PListBox::selected(int index) { /*FOLD00*/
  PukeMessage pmRet;

  debug("Got selected");
  pmRet.iCommand = PUKE_LISTBOX_SELECTED_ACK;
  pmRet.iWinId = widgetIden().iWinId;
  pmRet.iArg = index;
  if(widget()->text(index) != 0)
    strncpy(pmRet.cArg, widget()->text(index), 50);
  else
    pmRet.cArg[0] = 0;
  emit outputMessage(widgetIden().fd, &pmRet);
}

bool PListBox::checkWidget(){ /*FOLD00*/
  if(widget() == 0){
    debug("PListBox: No Widget set");
    return FALSE;
  }
  return TRUE;
}
