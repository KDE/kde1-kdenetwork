#ifndef PPUSHBUTTON_H
#define PPUSHBUTTON_H

class PPushButton;

#include <qpushbt.h>
#include "pmessage.h"
#include "pbutton.h"
#include "controller.h"

// Init and setup code
extern "C" {
PWidget *createWidget(widgetId *pwi, PWidget *parent);
}

class PPushButton : public PButton
{
  Q_OBJECT
 public:
  PPushButton ( PWidget * parent );
  virtual ~PPushButton ();
  
  virtual void messageHandler(int fd, PukeMessage *pm);
  
  virtual void setWidget(QPushButton *_b);
  virtual QPushButton *widget();

 public slots:
  
 signals:
  void outputMessage(int fd, PukeMessage *pm);

 private:
  QPushButton *button;

};

#endif
