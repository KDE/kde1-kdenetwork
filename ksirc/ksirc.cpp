/*
 * Needed items
 * 1. Move the fork and dsirc out of the main, and into the ksirc object
 * 2. Multiple toplevel widgets.
 * 3. Handle sig chils and sig pipes to control dsircc
 * 4. Send a /quit and/or kill dsirc on exit
 *
 */


//#include "toplevel.h"
//#include "ksircprocess.h"
//#include "iocontroller.h"
//#include "ioBroadcast.h"
#include "servercontroller.h"
#include <iostream.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

#include <kapp.h>

KApplication *kApp;
KConfig *kConfig;
//QDict<KSircTopLevel> TopList;
//QDict<KSircMessageReceiver> TopList;

int main( int argc, char ** argv )
{
  // Start the KDE application
  kApp = new KApplication( argc, argv, QString("ksirc") );

  // Starts the toplevel and give it sirc pipe items.

  kConfig = kApp->getConfig();

  //  KSircProcess proc("opus.dal.net");
  servercontroller *control = new servercontroller();
  control->show();

  //  KSircTopLevel *toplevel = new KSircTopLevel();
  //  TopList.insert("default", toplevel);
  //  TopList.insert("!all", new KSircIOBroadcast());

  //  ioc = new KSircIOController(sirc_stdin, sirc_stdout, sirc_stderr);

  //  toplevel->show();
  
  //  kApp->setMainWidget(toplevel);

  kApp->connect(kApp, SIGNAL(lastWindowClosed()), kApp, SLOT(quit()));
  
  kApp->exec();

  //  kill(sirc_pid, 15);  // on normal exit, do try and kill dsirc...
}















