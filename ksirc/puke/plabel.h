#ifndef PLABEL_H
#define PLABEL_H

class PLabel;

#include <qlabel.h>
#include "pmessage.h"
#include "pframe.h"
#include "controller.h"

// Init and setup code
extern "C" {
PWidget *createWidget(widgetId *pwi, PWidget *parent);
}

class PLabel : public PFrame
{
  Q_OBJECT
 public:
  PLabel ( PWidget * parent );
  virtual ~PLabel ();
  
  virtual void messageHandler(int fd, PukeMessage *pm);
  
  virtual void setWidget(QLabel *_l);
  virtual QLabel *widget();

 public slots:
  

 signals:
     void outputMessage(int fd, PukeMessage *pm);

protected:
  bool checkWidget();

 private:
  QLabel *label;
};

#endif