#include <qapp.h> 
#include <qsocknot.h>
#include <qregexp.h>

#include <unistd.h> 

#include "ksticker.h"

class StdInTicker : public KSTicker
{
  Q_OBJECT
public:
  StdInTicker();
  ~StdInTicker();

public slots:
void readsocket(int socket);
  
};

StdInTicker::StdInTicker()
  : KSTicker()
{
}

StdInTicker::~StdInTicker()
{
}

void StdInTicker::readsocket(int socket)
{
  char buf[1024];
  int bytes = read(socket, buf, 1024);
  QString str(buf, bytes);
  str.replace(QRegExp("\n"), " // ");
  mergeString(str);
}


int main(int argc, char **argv){
  QApplication a(argc, argv);

  KSTicker *kst = new StdInTicker();
  QSocketNotifier *sn = new QSocketNotifier(0, QSocketNotifier::Read);
  QObject::connect(sn, SIGNAL(activated(int)), 
	  kst, SLOT(readsocket(int)));
  a.setMainWidget(kst);
  kst->show();
  return a.exec();
}

