#include "messageReceiver.h"
#include "ksircprocess.h"

KSircMessageReceiver::~KSircMessageReceiver()
{
}

void KSircMessageReceiver::sirc_receive(QString)
{
  warning("Pure virtual sirc_receive called!!!!");
}

void KSircMessageReceiver::control_message(QString)
{
  warning("Pure virutal control_messaga called for a message_receiver!\n");
}


