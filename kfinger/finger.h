#ifndef __PROTOCOL
#define __PROTOCOL
#include <qstring.h>
#include <qobject.h>
#include <ksock.h>
class FingerProtocol : public QObject {
Q_OBJECT
	public:
                FingerProtocol(QString host,int port,char * buff);
		~FingerProtocol();
		int broken;
                void writeString(QString s);
	protected:
		char *s;
		KSocket *socket;    
	signals:
	  void update();
          void finish();	
        public slots:
          void readBuff(KSocket *);
      };

#endif

