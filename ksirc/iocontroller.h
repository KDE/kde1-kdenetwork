#ifndef IOCONTROL_H
#define IOCONTROL_H

#include <qobject.h>
#include <qwidget.h>
#include <qsocknot.h>  
#include <qdict.h>

#include <kapp.h>
#include <kprocess.h>

class KSircIOController;

#include "messageReceiver.h"
#include "ksircprocess.h"

class KSircIOController : public QObject
{

  Q_OBJECT
  friend class KSircProcess;
public:
  KSircIOController(KProcess *, KSircProcess *);
  virtual ~KSircIOController();


public slots:
  virtual void stdout_read(KProcess *proc, char *_buffer, int buflen);
  virtual void stderr_read(KProcess *proc, char *_buffer, int buflen);

  virtual void stdin_write(QString&);

  virtual void sircDied(KProcess *);

private:
  QString holder;
  KProcess *proc;
  KSircProcess *ksircproc;

  QSocketNotifier *stdout_notif;   

  static int counter;

};

#endif
