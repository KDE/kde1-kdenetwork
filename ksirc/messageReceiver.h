#ifndef KMESSAGERECEIVER_H
#define KMESSAGERECEIVER_H

#include <qstring.h>

class KSircProcess;

class KSircMessageReceiver
{
public:
  KSircMessageReceiver(KSircProcess *_proc)
    {
      proc = _proc;
    }
  virtual ~KSircMessageReceiver();

  virtual void sirc_receive(QString str) = 0;

  virtual void control_message(QString str);

private:
  KSircProcess *proc;

};

#endif
