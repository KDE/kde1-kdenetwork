#ifndef PWIDGET_H
#define PWIDGET_H

class PWidget;

#include <qobject.h>
#include <qwidget.h>
#include "pmessage.h"

class PWidget : public QObject
{
  Q_OBJECT
 public:
  PWidget(PWidget *parent = 0);
  virtual ~PWidget();

  // Init and setup code
  static PWidget *createWidget(widgetId *pwI, PWidget *parent);

  virtual void messageHandler(int fd, PukeMessage *pm);

  virtual void setWidget(QWidget *w);
  virtual QWidget *widget();

  virtual void setWidgetId(widgetId *pwI);
  virtual widgetId widgetIden();

 signals:
  void outputMessage(int fd, PukeMessage *pm);

 protected:
  bool eventFilter(QObject *o, QEvent *e);

 private:
  QWidget *w;

  widgetId wI;

  void (PWidget::*eventList[20])(QObject *, QEvent *);

  void eventNone(QObject *, QEvent *);
  void eventTimer(QObject *, QEvent *);
  void eventMouse(QObject *, QEvent *);
  void eventKey(QObject *, QEvent *);
  void eventFocus(QObject *, QEvent *);
  void eventPaint(QObject *, QEvent *);
  void eventMove(QObject *, QEvent *);
  void eventResize(QObject *, QEvent *);

};

#endif
