/**********************************************************************
 
 The IO Discarder

 $$Id$$

 Simple rule, junk EVERYTHING!!!

**********************************************************************/
 

#include "ioDiscard.h"

KSircIODiscard::~KSircIODiscard()
{
}

void KSircIODiscard::sirc_receive(const char *, int)
{
}

void KSircIODiscard::control_message(int, QString)
{
}
