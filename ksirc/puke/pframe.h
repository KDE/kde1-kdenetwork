#ifndef PFRAME_H
#define PFRAME_H

class PFrame;

#include <qframe.h>
#include "pmessage.h"
#include "pwidget.h"
#include "controller.h"

class PFrame : public PWidget
{
  Q_OBJECT
 public:
  PFrame ( PWidget * parent );
  virtual ~PFrame ();
  
  // Init and setup code
  static PWidget *createWidget(widgetId *pwi, PWidget *parent);

  virtual void messageHandler(int fd, PukeMessage *pm);
  
  virtual void setWidget(QFrame *_f);
  virtual QFrame *widget();

 signals:
  void outputMessage(int fd, PukeMessage *pm);

 private:
  QFrame *frame;
};

#endif
