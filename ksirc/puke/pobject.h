#ifndef POBJECT_H
#define POBJECT_H

class PObject;
class CreateArgs;
class PukeController;

#include <qobject.h>
#include "pmessage.h"


class CreateArgs {
public:
  CreateArgs(PukeController *_pc, PukeMessage *_pm, widgetId *_pwI, PObject *_parent){
    pc = _pc;
    pwI = _pwI;
    parent = _parent;
    pm = _pm;
    fetchedObj = 0;
  }
  PukeController *pc;
  widgetId *pwI;
  PObject *parent;
  PukeMessage *pm;

  /**
   * name of the widget which was fetched from kSirc, this has to be set explicitly
   */
  QObject *fetchedObj;
};

class PObject : public QObject
{
  Q_OBJECT
 public:
  PObject(QObject *parent = 0, const char *name = 0);
  virtual ~PObject();

  /**
   * Creates a new QObject and returns a PObject
   */
  static PObject *createWidget(CreateArgs &ca);

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

  /**
   * Set's the puke controller for the widget
   */
  void setPukeController(PukeController *pc){
    pController = pc;
  }

 signals:
  void outputMessage(int fd, PukeMessage *pm);
  void widgetDestroyed(widgetId wI);

 protected slots:
   void swidgetDestroyed();

protected:
  PukeController *controller();

private:
  QObject *obj;
  PukeController *pController;
  widgetId wI;
};

#include "controller.h"
#endif
