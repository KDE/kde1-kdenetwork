#include "pprogress.h"

PObject *createWidget(CreateArgs &ca) /*FOLD00*/
{
  PProgress *pw = new PProgress(ca.parent);
  KSProgress *ksp;
  if(ca.parent != 0 && ca.parent->widget()->isWidgetType() == TRUE)
    ksp = new KSProgress((QWidget *) ca.parent->widget());
  else
    ksp = new KSProgress();
  pw->setWidget(ksp);
  pw->setWidgetId(ca.pwI);
  return pw;
}


PProgress::PProgress( PObject *parent) /*fold00*/
  : PWidget(parent)
{
  //  debug("PProgress PProgress called");
  ksp = 0;
}

PProgress::~PProgress() /*fold00*/
{
  //  debug("PProgress: in destructor"); 
  delete widget(); // Delete the frame
  ksp=0;      // Set it to 0
  setWidget(ksp); // Now set all widget() calls to 0.
}

void PProgress::messageHandler(int fd, PukeMessage *pm) /*FOLD00*/
{
  //  debug("PProgress handler called");
  PukeMessage pmRet;
  if(widget() == 0){
    warning("Null widget");
    return;
  }
  switch(pm->iCommand){
  case PUKE_KSPROGRESS_SET_RANGE:
    {
      unsigned short int *size;
      size = (unsigned short int *) &pm->iArg;
      debug("Setting range to: %d %d", size[0], size[1]);
      if(size[0] > size[1])
          return;
      widget()->setRange(size[0], size[1]);
      pmRet.iCommand = PUKE_KSPROGRESS_SET_RANGE_ACK;
      pmRet.iWinId = pm->iWinId;
      pmRet.iArg = 0;
      pmRet.cArg[0] = 0;
      emit outputMessage(fd, &pmRet);
      break;
    }
  case PUKE_KSPROGRESS_SET_TOPTEXT:
    widget()->setTopText(QString(pm->cArg));
    pmRet.iCommand = PUKE_KSPROGRESS_SET_TOPTEXT;
    pmRet.iWinId = pm->iWinId;
    pmRet.iArg = 0;
    pmRet.cArg[0] = 0;
    emit outputMessage(fd, &pmRet);
    break;
  case PUKE_KSPROGRESS_SET_BOTTEXT:
    widget()->setBotText(QString(pm->cArg));
    pmRet.iCommand = PUKE_KSPROGRESS_SET_BOTTEXT;
    pmRet.iWinId = pm->iWinId;
    pmRet.iArg = 0;
    pmRet.cArg[0] = 0;
    emit outputMessage(fd, &pmRet);
    break;
  case PUKE_KSPROGRESS_SET_VALUE:
    widget()->setValue(pm->iArg);
    pmRet.iCommand = PUKE_KSPROGRESS_SET_VALUE_ACK;
    pmRet.iWinId = pm->iWinId;
    pmRet.iArg = 0;
    pmRet.cArg[0] = 0;
    emit outputMessage(fd, &pmRet);
    break;        
  default:
    PWidget::messageHandler(fd, pm);
  }
}

void PProgress::cancelPressed(){ /*FOLD00*/
  debug("Cancel Pressed");
  PukeMessage pmRet;
  pmRet.iCommand = PUKE_KSPROGRESS_CANCEL_ACK;
  pmRet.iWinId = widgetIden().iWinId;
  pmRet.iArg = 0;
  pmRet.cArg[0] = 0;
  emit outputMessage(widgetIden().fd, &pmRet);
}

void PProgress::setWidget(KSProgress *_f) /*FOLD00*/
{
  //  debug("PProgress setWidget called");
  ksp = _f;
  if(widget() != 0){
    connect(widget(), SIGNAL(cancel()),
            this, SLOT(cancelPressed()));
  }
  PWidget::setWidget(ksp);

}


KSProgress *PProgress::widget() /*fold00*/
{
  //  debug("PProgress widget called");
  return ksp;
}
