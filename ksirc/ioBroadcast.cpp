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
    if(it.current()->getBroadcast() == TRUE)
      it.current()->sirc_receive(QString(qstrdup(str.data())));
    ++it;
  }

}

void KSircIOBroadcast::control_message(int command, QString str)
{

  QDictIterator<KSircMessageReceiver> it(proc->getWindowList());

  it.toFirst();

  while(it.current()){
    if(it.current() != this)
      it.current()->control_message(command, QString(qstrdup(str.data())));
    ++it;
  }
}


filterRuleList *KSircIOBroadcast::defaultRules()
{
  filterRule *fr;
  filterRuleList *frl = new  filterRuleList();
  frl->setAutoDelete(TRUE);
  fr = new filterRule();
  fr->desc = "Search for dump ~'s";
  fr->search = "\\W~\\S+@\\S+\\W";
  fr->from = "~(\\S+@)";
  fr->to = "~~$1";
  frl->append(fr);
  fr = new filterRule();
  fr->desc = "Inverse to KSIRC italics";
  fr->search = ".*";
  fr->from = "(?g)\\x16";
  fr->to = "~i";
  frl->append(fr);
  fr = new filterRule();
  fr->desc = "Underline to KSIRC underline";
  fr->search = ".*";
  fr->from = "(?g)\\x1f";
  fr->to = "~u";
  frl->append(fr);
  fr = new filterRule();
  fr->desc = "Bold to KSIRC bold";
  fr->search = ".*";
  fr->from = "(?g)([^\\*])\\x02([^\\*])";
  fr->to = "$1~b$2";
  frl->append(fr);
  return frl;

}
