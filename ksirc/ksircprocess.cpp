/*************************************************************************

 KSircProcess, sirc controller

 $$Id$$

 KSircProcess cerate and controls toplevel widgets and sirc process'.
 Each sirc process has 1 and only 1 KSircProcess to control it.  KSirc
 process passes all IO to IOController which is it's friend.

 Interface:

 public:
     KSircProcess(*server=0L, *parent=0, *name=0)
       server: is the name of the server to connect to.  It must be
       provided or else start sirc will barf. :(
       parent: parent window, this _should_ be null
       name: name, passed to QObject...

     ~KSirProcess: 
       kill the sirc process, and iocontrollller, emit delete_toplevel

     getWindowList:
       returns the TopList, see bellow.

   Signals:
     made_toplevel(server, window)
       made a new toplevel window for the "server" we are connected to
       with "window" as the title.

     dalete_toplevel(server, window)
       delete toplevel with server and called window.  If we emit null
       as the window name it means to destroy all info about the
       server and ksircprocess.

     changeChannel(server, old_name, new_name)
       toplevel with old_name has been changed to new_name and all
      future refrences will use new_name.

   public slots:
     new_toplevel(window):
       create a new window with name window.  This MAY only change the
       name of an existing window that's now idle.

     close_topevel(KsircTopLevel*, window):
       deletes all refrences to window and if needed finds a new
       default toplevel.

     default_window(KSricTopLevel*):
       KSircTopLevel is requesting change to !default.  Be carefull
       with this one.

     recvChangeChannel(old, new):
       window old is changing to new.  emit ChangeChannel with server
       name added.  Without server name we can uniqely id the window. :(

 Implementation:

   Bassic process is to create a new KSircProcess and it takes care of
   the rest.  It emits signals for each new window and everytime a
   window is delete so you can update external display (like
   servercontroller uses).

   Startup:

   1. Creates a case insensitive TopList.  This is a list of ALL
   KSircReceivers under control of this server, and includes such
   items as "!all" and "!default".  All !name are control windows.

   2. Forks off a KProcess for sirc and passes it over to IOController
   which grabs and control's it's IO.

   3. It then opens a "!default" window.  This will receive all
   initial input and such.  It WILL change it's name on the first
   join.

   4. The IO broadcast object is created and setup.

   5. everything is put into run mode.


   Operation, see code bellow for inline comments.

*************************************************************************/



#include "ksircprocess.h"
#include "servercontroller.h"
#include "toplevel.h"
#include "ioBroadcast.h"
#include "ioDiscard.h"
#include "ioDCC.h"
#include "ioLAG.h"
#include "ioNotify.h"
#include "iocontroller.h"
#include "control_message.h"
#include "config.h"

#include <qmsgbox.h>

#include <stdlib.h>
#include <time.h>
#include <iostream.h>

extern KApplication *kApp;
extern KConfig *kConfig;
extern global_config *kSircConfig;

KSircProcess::KSircProcess( char *_server=0L, QObject * parent=0, const char * name=0 )
  : QObject(parent, name)
{

  server = qstrdup(_server);

  QDict<KSircMessageReceiver> nTopList(17, FALSE);
  TopList = nTopList;
  //  TopList.setAutoDelete(TRUE);

  // Setup the environment for KSirc
  QString qsNick, qsRealname;
  kConfig->setGroup("StartUp");
  qsNick = kConfig->readEntry("Nick", "");
  qsRealname = kConfig->readEntry("RealName", "");
  // 
  // I use qstrdup, I'm SURE THIS IS NOT NEEDED, but doesn't work otherwise!
  //
  if((qsNick.isEmpty() == FALSE)){
    QString env = "SIRCNICK=" + qsNick;
    putenv(qstrdup(env.data()));
  }
  if((qsRealname.isEmpty() == FALSE)){
    QString env = "SIRCNAME=" + qsRealname;
    putenv(qstrdup(env.data()));
  }

  QString env = "SIRCLIB=" + kSircConfig->kdedir + "/share/apps/ksirc";
  putenv(qstrdup(env.data()));

  proc = new KProcess();

  proc->setExecutable("perl");
  *proc << kSircConfig->kdedir + QString("/bin/dsirc") << "-8" << "-r" << "-s" << server;
  proc->start(KProcess::NotifyOnExit, KProcess::All);

  iocontrol = new KSircIOController(proc, this);
  QString command = "/eval $version .= \"+4KSIRC\"\n";
  iocontrol->stdin_write(command);
  command = "/load " + kSircConfig->kdedir + "/share/apps/ksirc/filters.pl\n";
  iocontrol->stdin_write(command);
  command = "/load " + kSircConfig->kdedir + "/share/apps/ksirc/ksirc.pl\n";
  iocontrol->stdin_write(command);

  // Write default commands

  running_window = TRUE;        // True so we do create the default
  new_toplevel("!no_channel");  // 
  //  TopList.insert("!default", TopList["!no_channel"]);

  //  kConfig->setGroup("GlobalOptions");
  //  if(kConfig->readNumEntry("Reuse", TRUE) == TRUE){
  //  }
  //  else{
  //    running_window = TRUE;        // True so we do create the a new message
  //    new_toplevel("!messages");
  //  }

  running_window = FALSE;       // set false so next changes the first name
  default_follow_focus = TRUE;

  TopList.insert("!all", new KSircIOBroadcast(this));
  TopList.insert("!discard", new KSircIODiscard(this));
  KSircIODCC *dcc = new KSircIODCC(this);
  connect(dcc, SIGNAL(outputLine(QString&)),
	  iocontrol, SLOT(stdin_write(QString&)));	      
  TopList.insert("!dcc", dcc);
  KSircIOLAG *lag = new KSircIOLAG(this);
  connect(lag, SIGNAL(outputLine(QString&)),
	  iocontrol, SLOT(stdin_write(QString&)));
  TopList.insert("!lag", lag);
  KSircIONotify *notify = new KSircIONotify(this);
  connect(notify, SIGNAL(notify_online(QString)),
	  this, SLOT(notify_forw_online(QString)));
  connect(notify, SIGNAL(notify_offline(QString)),
	  this, SLOT(notify_forw_offline(QString)));
  TopList.insert("!notify", notify);

  filters_update();

  // We do this after filters_update() since filters_update loads the
  // require notify filters, etc.

  kConfig->setGroup("NotifyList");
  QString cindex, nick;
  int items = kConfig->readNumEntry("Number");
  command = "/notify ";
  for(int i = 0; i < items; i++){
    cindex.setNum(i);
    nick = "Notify-" + cindex;
    command += kConfig->readEntry(nick) + " ";
  }
  command += "\n";
  iocontrol->stdin_write(command);  

}

KSircProcess::~KSircProcess()
{
  
  if(TopList["!default"]){
    TopList.remove("!default"); // remove default so we don't delete it twice.
  }

  QDictIterator<KSircMessageReceiver> it(TopList);
  while(it.current()){
    delete it.current();
    ++it;
  }

  emit ProcMessage(QString(server), ProcCommand::procClose, QString());

  delete proc;               // Delete process, seems to kill sirc, good.
  delete iocontrol;          // Take out io controller
  delete server;
}

void KSircProcess::new_toplevel(QString str)
{
  static time_t last_window_open = 0;
  static int number_open = 0;

  if(running_window == FALSE){ // If we're not fully running, reusing
			       // !default window for next chan.
    running_window = TRUE;
    TopList.insert(str, TopList["!no_channel"]); 
    TopList.remove("!no_channel"); // We're no longer !no_channel
    TopList[str]->control_message(CHANGE_CHANNEL, str);
  }
  else if(!TopList[str]){ // If the window doesn't exist, continue
    // If AutoCreate windows is on, let's make sure we're not being flooded.
    if(kSircConfig->autocreate == TRUE){
      time_t current_time = time(NULL);
      if((current_time - last_window_open) < 5){
	if(number_open > 4){
	  switch(QMessageBox::warning(0, "Flood warning",
				      "5 Channel windows were opened\n"
				      "in less than 5 seconds.  Someone\n"
				      "maybe trying to flood your X server\n"
				      "with windows.\n\n"
				      "Should I turn off AutoCreate windows?\n",
				      "Yes", "No", (char *) 0, 0, 0)){

          case 0:
	    emit ProcMessage(QString(server), ProcCommand::turnOffAutoCreate, QString());
	  }
	  last_window_open = current_time;
	  number_open = 0;
	}
	else{
	  number_open++;
	}
      }
      else{
	last_window_open = current_time;
      }
    }

    // Create a new toplevel, and add it to the toplist.  
    // TopList is a list of KSircReceivers so we still need wm.
    KSircTopLevel *wm = new KSircTopLevel(this, str.data());
    TopList.insert(str, wm);
    // Connect needed signals.  For a message window we never want it
    // becomming the default so we ignore focusIn events into it.
    connect(wm, SIGNAL(outputLine(QString&)), 
	    iocontrol, SLOT(stdin_write(QString&)));
    connect(wm, SIGNAL(open_toplevel(QString)),
	    this,SLOT(new_toplevel(QString)));
    connect(wm, SIGNAL(closing(KSircTopLevel *, char *)),
	    this,SLOT(close_toplevel(KSircTopLevel *, char *)));
    connect(wm, SIGNAL(currentWindow(KSircTopLevel *)),
	    this,SLOT(default_window(KSircTopLevel *)));
    connect(wm, SIGNAL(changeChannel(QString, QString)),
	    this,SLOT(recvChangeChannel(QString, QString)));
    emit ProcMessage(QString(server), ProcCommand::addTopLevel, str);
    wm->show(); // Pop her up
  }
  else{
    //    cerr << "Window " << str << " already exists\n";
  }
}

void KSircProcess::close_toplevel(KSircTopLevel *wm, char *name)
{

  bool is_default = FALSE; // Assume it's no default

  if(TopList.count() <= 7){ // If this is the last window shut down
    QString command = "/quit\n";
    iocontrol->stdin_write(command); // kill sirc
    delete this; // Delete ourself, WARNING MUST RETURN SINCE WE NO
		 // LONGER EXIST!!!!
    return;      // ^^^^^^^^^^^^^^^
  }
  else if(wm == TopList["!default"]){ // Are we the current default?
    is_default = TRUE;
  }

  if(TopList[name]){  // Delete window
    delete TopList[name];
    TopList.remove(name);
  }
  else // If the window wasn't found warn, something's going wrong.
    cerr << "DID NOT DELETE THE WINDOW: " << name << " !!!!\n";

  //
  // Ok, now if we just deleted the default we have a problem, we need
  // a new default.  BUT don't make the default "!all" or !message.
  // So let's go grab a default, and make sure it's not "!" control
  // object.
  //

  if(is_default == TRUE){
    QDictIterator<KSircMessageReceiver> it(TopList);
    it.toFirst();
    char *key = (char*) it.currentKey();
    if(key[0] == '!')
      for(;(key[0] == '!') && it.current(); ++it)
	key = (char*) it.currentKey();

    if(it.current())
      TopList.replace("!default", it.current());
    else{
      cerr << "NO MORE WINDOWS?\n"; // We're out of windows with > 3
				    // huh open, huh?
      TopList.remove("!default");   // let's not blow up to badly
      QString command = "/quit\n";  // close this server connetion then
      iocontrol->stdin_write(command); // kill sirc
      delete this; // Delete ourself, WARNING MUST RETURN SINCE WE NO
                   // LONGER EXIST!!!!
      return;      // ^^^^^^^^^^^^^^^

    }
  }
  // Let's let em know she's deleted!
  emit ProcMessage(QString(server), ProcCommand::deleteTopLevel, 
		   QString(name));
}

void KSircProcess::default_window(KSircTopLevel *w)
{

  //
  // If we want to track the default as it goes arround, change the
  // window on focus changes.
  //

  if(w && (default_follow_focus == TRUE))
    TopList.replace("!default", w);

}

void KSircProcess::recvChangeChannel(QString old_chan, QString
				     new_chan)
{
  //
  // Channel changed name, add our own name and off we go.
  // ServerController needs our name so it can have a uniq handle for
  // the window name.
  //
  emit ProcMessage(QString(server), ProcCommand::changeChannel,
		   old_chan + " " + new_chan);
}

void KSircProcess::filters_update()
{
  QString command, next_part, key, data;
  command = "/crule\n";
  iocontrol->stdin_write(command);
  QDictIterator<KSircMessageReceiver> it(TopList);
  filterRuleList *frl;
  filterRule *fr;
  while(it.current()){
    frl = it.current()->defaultRules();
    for ( fr=frl->first(); fr != 0; fr=frl->next() ){
      command.truncate(0);
      command += "/ksircappendrule DESC==";
      command += fr->desc;
      command += " !!! SEARCH==";
      command += fr->search;
      command += " !!! FROM==";
      command += fr->from;
      command += " !!! TO==\"";
      command += fr->to;
      command += "\"\n";
      iocontrol->stdin_write(command);
    }
    delete frl;
    ++it;
  }
  kConfig->setGroup("FilterRules");
  int max = kConfig->readNumEntry("Rules", 0);
  for(int number = 1; number <= max; number++){
    command.truncate(0);
    key.sprintf("name-%d", number);
    next_part.sprintf("/ksircappendrule DESC==%s !!! ", kConfig->readEntry(key).data());
    command += next_part;
    key.sprintf("search-%d", number);
    next_part.sprintf("SEARCH==%s !!! ", kConfig->readEntry(key).data());
    command += next_part;
    key.sprintf("from-%d", number);
    next_part.sprintf("FROM==%s !!! ", kConfig->readEntry(key).data());
    command += next_part;
    key.sprintf("to-%d", number);
    next_part.sprintf("TO==\"%s\"\n", kConfig->readEntry(key).data());
    command += next_part;
    iocontrol->stdin_write(command);
  }
}


void KSircProcess::notify_forw_online(QString nick)
{
  emit ProcMessage(QString(server), ProcCommand::nickOnline, nick);
}

void KSircProcess::notify_forw_offline(QString nick)
{
  emit ProcMessage(QString(server), ProcCommand::nickOffline, nick);
}

void KSircProcess::ServMessage(QString dst_server, int command, QString args)
{
  if(dst_server.isEmpty() || (dst_server == QString(server))){
    switch(command){
    case ServCommand::updateFilters:
      filters_update();
      break;
    default:
      cerr << "Unkown command: " << command << " to " << command << " args " << args << endl;
      break;
    }
  }
}
