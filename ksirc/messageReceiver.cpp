#include "messageReceiver.h"
#include "ksircprocess.h"

KSircMessageReceiver::~KSircMessageReceiver()
{
}

void KSircMessageReceiver::sirc_receive(QString)
{
  warning("Pure virtual sirc_receive called!!!!");
}

void KSircMessageReceiver::sirc_stop(bool)
{
  warning("Pure virtual sirc_stop called!!!!");
}

void KSircMessageReceiver::control_message(QString)
{
}


