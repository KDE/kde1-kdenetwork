#include "ioBroadcast.h"
#include <iostream.h>

KSircIOBroadcast::~KSircIOBroadcast()
{
}

void KSircIOBroadcast::sirc_receive(QString str)
{

  QDictIterator<KSircMessageReceiver> it(proc->getWindowList());

  it.toFirst();

  while(it.current()){
    if(it.current() != this)
      it.current()->sirc_receive(QString(qstrdup(str.data())));
    ++it;
  }

}
