#include "pmenudta.h"
#include "pobject.h"

/**
 * This is an empty function such that if it get's called we don't explode
 */
PObject *createWidget(CreateArgs &ca) {
    warning("Create called for PMenuData, bad things are about to happend\n");
    return new PObject();
}


PMenuData::PMenuData(PObject *_child)
{
  child = _child;
}

PMenuData::~PMenuData()
{
  // We don't nuke anything since we're kind of abstract and we let the parent take care of it
}

bool PMenuData::messageHandler(int fd, PukeMessage *pm)
{
    PukeMessage pmRet;
    PSubObject pb(child);
    int id = 0;
  switch(pm->iCommand){
  case PUKE_MENUDATA_INSERT_TEXT:
    id = ((QMenuData *) child->widget())->insertItem(pm->cArg, child->widget(), SLOT(activated()), pm->iArg);
    
    pmRet.iCommand = PUKE_MENUDATA_INSERT_TEXT_ACK;
    pmRet.iWinId = pm->iWinId;
    pmRet.iArg = id;
    pmRet.cArg[0] = 0;
    pb.outputMessage(fd, &pmRet);
    break;
  case PUKE_MENUDATA_INSERT_PIXMAP:
    id = ((QMenuData *) child->widget())->insertItem(QPixmap(pm->cArg), child->widget(), SLOT(activated()), pm->iArg);
    
    pmRet.iCommand = PUKE_MENUDATA_INSERT_TEXT_ACK;
    pmRet.iWinId = pm->iWinId;
    pmRet.iArg = id;
    pmRet.cArg[0] = 0;
    pb.outputMessage(fd, &pmRet);
    break;
  default:
    return FALSE;
  }
  return TRUE;
}

