#include "ppushbt.h"
#include <stdio.h>

PWidget *createWidget(widgetId *pwi, PWidget *parent)
{
  PPushButton *pb = new PPushButton(parent);
  QPushButton *qb;
  if(parent != 0)
    qb = new QPushButton(parent->widget());
  else
    qb = new QPushButton();
  pb->setWidget(qb);
  pb->setWidgetId(pwi);
  return pb;
}


PPushButton::PPushButton(PWidget *parent)
  : PButton(parent)
{
  //  debug("PLineEdit PLineEdit called");
  button = 0;
  setWidget(button);
}

PPushButton::~PPushButton()
{
  //  debug("PLineEdit: in destructor"); 
  delete widget();     // Delete the frame
  button=0;          // Set it to 0
  setWidget(button); // Now set all widget() calls to 0.
}

void PPushButton::messageHandler(int fd, PukeMessage *pm)
{
  PukeMessage pmRet;
  switch(pm->iCommand){
  default:
    PButton::messageHandler(fd, pm);
  }
}

void PPushButton::setWidget(QPushButton *_qb)
{
  button = _qb;
  if(_qb != 0){
  }
  PButton::setWidget(_qb);

}


QPushButton *PPushButton::widget()
{
  return button;
}


