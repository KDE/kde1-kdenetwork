/*************************************************************************

 Main KSirc start 

 $$Id$$

 Main start file that defines 3 global vars, etc

 Does nothing to special, start create new KApplcaiton (incorrectly,
 btw, should use the code Kalle posted so it can be session managment
 restarted, etc) loads colours then fires off the main widget.  On
 exit it calls a sync so the kConfig get written.  (Is this a bug?)

*************************************************************************/

/*
 * Needed items
 * 4. Send a /quit and/or kill dsirc on exit
 * */


#include "servercontroller.h"

#include <iostream.h>
#include <time.h> 


#include <qfont.h>
#include <qmessagebox.h> 

#include <kapp.h>
#include <kconfig.h>

#include "config.h"
#include "cdate.h"

KApplication *kApp;
KConfig *kConfig;
global_config *kSircConfig;
//QDict<KSircTopLevel> TopList;
//QDict<KSircMessageReceiver> TopList;

int main( int argc, char ** argv )
{
  // Start the KDE application
  kApp = new KApplication( argc, argv, QString("ksirc") );

  QString ver = KSIRC_VERSION;
  if(ver.contains(".") == 0){
    // This is a development version
    // 4 week timeout, warn after 2
    int ntime = time(NULL);
    if(ntime - COMPILE_DATE > 4838400){
      // To Old
      QMessageBox::critical(0, "kSirc",
			    QString(i18n("kSirc Alpha releases have an 8 week\n"
                                         "expiry date.\n\n"
                                         "kSirc-ALPHA HAS EXPIRED!!\n"
                                         "Please upgrade or use a beta release")));
      exit(1);
    }
    else if(ntime - COMPILE_DATE > 2419200){
      QMessageBox::warning(0, "kSirc",
			   QString(i18n("kSirc Alpha release have an 8 week\n"
                                        "expiry date.\n\n"
                                        "THIS VERSION WILL EXPIRE IN UNDER 4 WEEKS")));
    }
  }


  // Get config, and setup internal structure.

  kConfig = kApp->getConfig();

  kSircConfig = new global_config;

  kConfig->setGroup("Colours");
  kSircConfig->colour_text = new QColor(kConfig->readColorEntry("text", &black));
  kSircConfig->colour_info = new QColor(kConfig->readColorEntry("info", &blue));
  kSircConfig->colour_chan = new QColor(kConfig->readColorEntry("chan", &green));
  kSircConfig->colour_error = new QColor(kConfig->readColorEntry("error", &red));

  kSircConfig->colour_background = 0;
  kSircConfig->filterKColour = kConfig->readNumEntry("kcolour", false);
  kSircConfig->filterMColour = kConfig->readNumEntry("mcolour", false);
  kSircConfig->nickFHighlight = kConfig->readNumEntry("nickfcolour", -1);
  kSircConfig->nickBHighlight = kConfig->readNumEntry("nickbcolour", -1);
  kSircConfig->usHighlight = kConfig->readNumEntry("uscolour", -1);
  kSircConfig->transparent = kConfig->readNumEntry("Transparent", false);

  kSircConfig->kdedir = getenv("KDEDIR");
  if(kSircConfig->kdedir.isEmpty()){
    kSircConfig->kdedir = "/usr/local/kde";
    //    kApp->kdedir() =  kSircConfig->kdedir;
    warning("KDEDIR information not found, online help will not work\n");
    //    cerr << "KDEDIR set to: " << kApp->kdedir() << endl;
  }
  QString ld_path = getenv("LD_LIBRARY_PATH");
  ld_path += ":" + kSircConfig->kdedir + "/share/apps/ksirc/:";
  ld_path.prepend("LD_LIBRARY_PATH=");
  putenv(ld_path.data());

  kConfig->setGroup("GlobalOptions");
  kSircConfig->defaultfont = kConfig->readFontEntry("MainFont", new QFont("fixed"));
  kConfig->setGroup("General");
  kSircConfig->MDIMode = kConfig->readNumEntry("MDIMode", false);


  if(kApp->isRestored()){
    int n = 1;
    while (servercontroller::canBeRestored(n)) {
      servercontroller *sc = new servercontroller(0, "servercontroller");
      CHECK_PTR(sc);
      sc->restore(n);
      kApp->setMainWidget(sc);
      n++;
    }
  }
  else{
    servercontroller *control = new servercontroller(0, "servercontroller");
    control->show();
    kApp->setMainWidget(control);
  }
  
  try {
      kApp->exec();
  }
  catch(...){
      cerr << "Caught Unkown Exception, uhoh!!!\n";
      cerr << "Dying!!!\n";
      exit(10);
  }

  kConfig->sync();
}















