#include <kapp.h>
#include <kconfig.h>
#include <qapp.h> 
#include <qsocknot.h>
#include <qregexp.h>

#include <unistd.h> 

#include "ksticker.h"
#include "main.h"

KConfig *kConfig;

StdInTicker::StdInTicker()
  : KSTicker()
{
  kConfig->setGroup("defaults");
  setFont(kConfig->readFontEntry("font"));
  setSpeed(kConfig->readNumEntry("tick", 30), 
	   kConfig->readNumEntry("step", 3));
}

StdInTicker::~StdInTicker()
{
  int tick, step;
  speed(&tick, &step);
  kConfig->setGroup("defaults");
  kConfig->writeEntry("font", font());
  kConfig->writeEntry("tick", tick);
  kConfig->writeEntry("step", step);
  kConfig->sync();
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
  KApplication a(argc, argv);

  kConfig = a.getConfig();

  KSTicker *kst = new StdInTicker();
  QSocketNotifier *sn = new QSocketNotifier(0, QSocketNotifier::Read);
  QObject::connect(sn, SIGNAL(activated(int)), 
	  kst, SLOT(readsocket(int)));
  a.setMainWidget(kst);
  kst->show();
  return a.exec();
}

