#ifndef KIOBROADCAST_H
#define KIOBROADCAST_H

#include <qobject.h>
#include <qstring.h>
#include <qdict.h>

#include "messageReceiver.h"
#include "ksircprocess.h"

class KSircIOBroadcast : public KSircMessageReceiver
{
public:
  KSircIOBroadcast(KSircProcess *_proc) : KSircMessageReceiver(_proc)
    {
      proc = _proc;
    }
  virtual ~KSircIOBroadcast();

  virtual void sirc_receive(QString str);
  virtual void control_message(QString str);

private:
  KSircProcess *proc;
};

#endif
