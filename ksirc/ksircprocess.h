#ifndef KSIRCPROCESS
#define KSIRCPROCESS

class KSircProcess;
class KSircTopLevel;

#include <qobject.h>

#include <kprocess.h>

#include "iocontroller.h"
#include "messageReceiver.h"

class KSircProcess : public QObject
{
  Q_OBJECT;
  friend class KSircIOController;
public:
  KSircProcess(char *_server=0L, QObject * parent=0, const char * name=0);
  ~KSircProcess();

  QDict<KSircMessageReceiver> &getWindowList()
    {
      return TopList;
    }

signals:
  virtual void made_toplevel(QString, QString);
  virtual void delete_toplevel(QString, QString);
  virtual void changeChannel(QString, QString, QString);

public slots:
  virtual void new_toplevel(QString);
//  virtual void new_ksircprocess(QString);
  virtual void close_toplevel(KSircTopLevel *, char *);
  virtual void default_window(KSircTopLevel *);
  virtual void recvChangeChannel(QString, QString);

private:
  KSircIOController *iocontrol;
  KProcess *proc;

  QDict<KSircMessageReceiver> TopList;

  bool running_window;
  bool default_follow_focus;

  char *server;

};

#endif
