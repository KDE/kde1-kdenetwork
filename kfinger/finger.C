#include <stdlib.h>
#include <time.h>
#include <kapp.h>
#include "finger.h"
#include <unistd.h>

FingerProtocol::FingerProtocol(QString host,int port,char *buff) {
	socket=new KSocket(host,port);
	broken=(socket->socket()==-1);
        if(!broken)
	 socket->enableRead(TRUE);  
        s=buff; 
 connect(socket,SIGNAL(readEvent(KSocket *)),this,SLOT(readBuff(KSocket *))); 
 connect(socket,SIGNAL(closeEvent(KSocket *)),this,SIGNAL(finish())); 
}

FingerProtocol::~FingerProtocol() 
{
delete socket;
}

void FingerProtocol::readBuff(KSocket *) {

 socket->enableRead(FALSE);
	char c,n;
	do {
 KApplication::getKApplication()->processEvents();
 n=read(socket->socket(),&c,1);
		if (c!='\r') *s++=c;
	} while (n==1);
	*s=0;    
 update();
 socket->enableRead(TRUE);
}

void FingerProtocol::writeString(QString s) {
   write(socket->socket(),s,strlen(s));
}
#include "finger.moc"

