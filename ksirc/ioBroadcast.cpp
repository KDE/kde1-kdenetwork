/**********************************************************************
 
 The IO Broadcaster

 $$Id$$

 The IO broadcaster bassed on the ksircmessage receiver takes a
 message and send it all ksircmessage receivers, except of course it
 self.

 It does the same with control_messages.

 Implementation: 

  Make a QDictIterator, iterate over the windows sedning to each
  broadcaster that's not itself.  

  *** NOTE! don't have 2 broadcasters or else they'll broadcast forever!

**********************************************************************/
 

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

void KSircIOBroadcast::control_message(QString str)
{

  QDictIterator<KSircMessageReceiver> it(proc->getWindowList());

  it.toFirst();

  while(it.current()){
    if(it.current() != this)
      it.current()->control_message(QString(qstrdup(str.data())));
    ++it;
  }
}
