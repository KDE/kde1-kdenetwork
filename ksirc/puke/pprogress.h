#ifndef PPROGRESS_H
#define PPPROGESS_H

class PProgress;

#include "../KSProgress/ksprogress.h"
#include "pmessage.h"
#include "pwidget.h"
#include "controller.h"

// Init and setup code
extern "C" {
PWidget *createWidget(widgetId *pwi, PWidget *parent);
}

class PProgress : public PWidget {
  Q_OBJECT
public:
  PProgress ( PWidget * parent = 0);
  virtual ~PProgress ();

  virtual void messageHandler(int fd, PukeMessage *pm);

  virtual void setWidget(KSProgress *_f);
  virtual KSProgress *widget();

signals:
  void outputMessage(int fd, PukeMessage *pm);

protected slots:
  void cancelPressed();

private:
  KSProgress *ksp;
};

#endif
