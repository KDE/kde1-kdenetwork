#include "pbutton.h"
#include <stdio.h>

PWidget *createWidget(widgetId *pwi, PWidget *parent)
{
  PButton *pb = new PButton(parent);
  QButton *qb;
  if(parent != 0)
    qb = new QButton(parent->widget());
  else
    qb = new QButton();
  pb->setWidget(qb);
  pb->setWidgetId(pwi);
  return pb;
}


PButton::PButton(PWidget *parent)
  : PWidget(parent)
{
  //  debug("PLineEdit PLineEdit called");
  button = 0;
  setWidget(button);
}

PButton::~PButton()
{
  //  debug("PLineEdit: in destructor"); 
  delete widget();     // Delete the frame
  button=0;          // Set it to 0
  setWidget(button); // Now set all widget() calls to 0.
}

void PButton::messageHandler(int fd, PukeMessage *pm)
{
  PukeMessage pmRet;
  switch(pm->iCommand){
    case PUKE_BUTTON_SET_TEXT:
    if(checkWidget() == FALSE)
      return;

    widget()->setText(pm->cArg);    // set the text

    pmRet.iCommand = - pm->iCommand;// Create ack
    pmRet.iWinId = - pm->iWinId;
    strncpy(pmRet.cArg, widget()->text(), 50);
    emit outputMessage(fd, &pmRet);
    break;
  case PUKE_BUTTON_SET_PIXMAP:
    if(checkWidget() == FALSE)
      return;
    
    widget()->setPixmap(QPixmap(pm->cArg));

    pmRet.iCommand = - pm->iCommand;
    pmRet.iWinId = pm->iWinId;
    pmRet.iArg = widget()->pixmap()->isNull();
    emit outputMessage(fd, &pmRet);
    break;
  case PUKE_BUTTON_SET_AUTORESIZE:
    if(checkWidget() == FALSE)
      return;

    widget()->setAutoResize(pm->iArg);

    pmRet.iCommand = - pm->iCommand;
    pmRet.iWinId = - pm->iWinId;
    pmRet.iArg = widget()->autoResize();
    emit outputMessage(fd, &pmRet);
    break;
  default:
    PWidget::messageHandler(fd, pm);
  }
}

void PButton::setWidget(QButton *_qb)
{
  button = _qb;
  if(_qb != 0){
    connect(button, SIGNAL(pressed()),
	    this, SLOT(buttonPressed()));
    connect(button, SIGNAL(released()),
	    this, SLOT(buttonReleased()));
    connect(button, SIGNAL(clicked()),
	    this, SLOT(buttonClicked()));
    connect(button, SIGNAL(toggled(bool)),
	    this, SLOT(buttonToggled(bool)));
    
  }
  PWidget::setWidget(_qb);

}


QButton *PButton::widget()
{
  return button;
}

void PButton::buttonMessage(int iCommand)
{
  PukeMessage pmRet;

  pmRet.iCommand = iCommand;
  pmRet.iArg = 0;
  pmRet.iWinId = widgetIden().iWinId;
  pmRet.cArg[0] = 0;

  emit outputMessage(widgetIden().fd, &pmRet);
}

void PButton::buttonPressed()
{
  buttonMessage(PUKE_BUTTON_PRESSED_ACK);
}

void PButton::buttonReleased()
{
  buttonMessage(PUKE_BUTTON_RELEASED_ACK);
}

void PButton::buttonClicked()
{
  buttonMessage(PUKE_BUTTON_CLICKED_ACK);
}

void PButton::buttonToggled(bool)
{
  buttonMessage(PUKE_BUTTON_TOGGLED_ACK);
}

bool PButton::checkWidget()
{
  if(widget() == 0){
    debug("PButton: No Widget set");
    return FALSE;
  }
  return TRUE;
}
