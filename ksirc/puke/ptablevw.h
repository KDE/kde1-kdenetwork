#ifndef PTABLEVIEW_H
#define PTABLWVIEW_H

class PTableView;

#include <qtablevw.h>
#include "pmessage.h"
#include "pframe.h"
#include "controller.h"

// Init and setup code
extern "C" {
PWidget *createWidget(widgetId *pwi, PWidget *parent);
}

class PTableView : public PFrame
{
  Q_OBJECT
 public:
  PTableView ( PWidget * parent );
  virtual ~PTableView ();
  
  virtual void messageHandler(int fd, PukeMessage *pm);
  
  virtual void setWidget(QTableView *_tbl);
  virtual QTableView *widget();

  public slots:

 signals:
  void outputMessage(int fd, PukeMessage *pm);

 private:
  QTableView *tbl;
};

#endif
