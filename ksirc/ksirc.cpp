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

#include "config.h"

KApplication *kApp;
KConfig *kConfig;
global_config *kSircConfig;
//QDict<KSircTopLevel> TopList;
//QDict<KSircMessageReceiver> TopList;

int main( int argc, char ** argv )
{
  // Start the KDE application
  kApp = new KApplication( argc, argv, QString("ksirc") );

  // Starts the toplevel and give it sirc pipe items.


  // Get config, and setup internal structure.

  kConfig = kApp->getConfig();

  kSircConfig = new global_config;

  kConfig->setGroup("Colours");
  kSircConfig->colour_text = new QColor(kConfig->readColorEntry("text", &black));
  kSircConfig->colour_info = new QColor(kConfig->readColorEntry("info", &blue));
  kSircConfig->colour_chan = new QColor(kConfig->readColorEntry("chan", &green));
  kSircConfig->colour_error = new QColor(kConfig->readColorEntry("red", &red));

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
  kConfig->sync();

  //  kill(sirc_pid, 15);  // on normal exit, do try and kill dsirc...
}















