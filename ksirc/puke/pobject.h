#ifndef POBJECT_H
#define POBJECT_H

class PObject;

#include <qobject.h>
#include "pmessage.h"

class PObject : public QObject
{
  Q_OBJECT
 public:
  PObject(QObject *parent = 0);
  virtual ~PObject();

  /**
   * Creates a new QObject and returns a PObject
   */
  static PObject *createWidget(widgetId *pwI, PObject *parent);

  /**
   * Handles messages from dsirc
   * PObject can't get messages so return an error
   */
  virtual void messageHandler(int fd, PukeMessage *pm);

  /**
   * Sets the current opbect
   */
  virtual void setWidget(QObject *w);
  
  /**
   * Returns the current object
   */
  virtual QObject *widget();

  /**
   * Sets the window id
   */
  virtual void setWidgetId(widgetId *pwI);
  /**
   * Returns the current window identifier
   */
  virtual widgetId widgetIden();

 signals:
  void outputMessage(int fd, PukeMessage *pm);
  void widgetDestroyed(widgetId wI);

 protected slots:
  void swidgetDestroyed();

 private:

  QObject *obj;

  widgetId wI;
};

#endif
