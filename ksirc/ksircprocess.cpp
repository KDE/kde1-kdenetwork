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
#include "toplevel.h"
#include "ioBroadcast.h"
#include "ioDiscard.h"
#include "iocontroller.h"
#include <iostream.h>

extern KApplication *kApp;
extern KConfig *kConfig;

KSircProcess::KSircProcess( char *_server=0L, QObject * parent=0, const char * name=0 )
  : QObject(parent, name)
{

  server = _server;

  QDict<KSircMessageReceiver> nTopList(17, FALSE);
  TopList = nTopList;

  proc = new KProcess();

  proc->setExecutable("perl");
  *proc << "dsirc" << "-l" << "/dev/null" << "-8" << "-r" << "-s" << server;
  proc->start(KProcess::NotifyOnExit, KProcess::All);

  iocontrol = new KSircIOController(proc, this);
  iocontrol->stdin_write("/load filters.pl\n");
  filters_update();

  running_window = TRUE;        // True so we do create the default
  new_toplevel("!default");     // 

  kConfig->setGroup("GlobalOptions");
  if(kConfig->readNumEntry("Reuse", TRUE) == TRUE){
  }
  else{
    running_window = TRUE;        // True so we do create the a new message
    new_toplevel("!messages");
  }

  running_window = FALSE;       // set false so next changes the first name
  default_follow_focus = TRUE;

  TopList.insert("!all", new KSircIOBroadcast(this));
  TopList.insert("!discard", new KSircIODiscard(this));
  
  //  wm->show();

}

KSircProcess::~KSircProcess()
{
  delete proc;               // Delete process, seems to kill sirc, good.
  delete iocontrol;          // Take out io controller
  emit delete_toplevel(QString(server), QString()); // Say we're closing.
}

void KSircProcess::new_toplevel(QString str)
{
  if(running_window == FALSE){ // If we're not fully running, reusing
			       // !default window for next chan.
    running_window = TRUE;
    TopList.insert(str, TopList["!default"]); 
    TopList[str]->control_message(QString("001") + str);
  }
  else if(!TopList[str]){  // If the window doesn't exist, continue
    // Create a new toplevel, and add it to the toplist.  
    // TopList is a list of KSircReceivers so we still need wm.
    KSircTopLevel *wm = new KSircTopLevel(this, qstrdup(str.data()));
    TopList.insert(str, wm);
    // Connect needed signals.  For a !message window we never want it
    // becomming the default so we ignore focusIn events into it.
    connect(wm, SIGNAL(outputLine(QString)), 
	    iocontrol, SLOT(stdin_write(QString)));
    connect(wm, SIGNAL(open_toplevel(QString)),
	    this,SLOT(new_toplevel(QString)));
    connect(wm, SIGNAL(closing(KSircTopLevel *, char *)),
	  this,SLOT(close_toplevel(KSircTopLevel *, char *)));
    if(str != QString("!messages")){
      connect(wm, SIGNAL(currentWindow(KSircTopLevel *)),
	      this,SLOT(default_window(KSircTopLevel *)));
      connect(wm, SIGNAL(changeChannel(QString, QString)),
	      this,SLOT(recvChangeChannel(QString, QString)));
    }
    emit made_toplevel(QString(server), str);
    wm->show(); // Pop her up
  }
  else{
    cerr << "Window " << str << " already exists\n";
  }
}

void KSircProcess::close_toplevel(KSircTopLevel *wm, char *name)
{

  bool is_default = FALSE; // Assume it's no default

  if(TopList.count() <= 4){ // If this is the last window shut down
    iocontrol->stdin_write(QString("/quit\n")); // kill sirc
    delete this; // Delete ourself, WARNING MUST RETURN SINCE WE NO
		 // LONGER EXIST!!!!
    return;      // ^^^^^^^^^^^^^^^
  }
  else if(wm == TopList["!default"]){ // Are we the current default?
    is_default = TRUE;
  }

  if(TopList[name]){  // Delete window
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
    else
      cerr << "NO MORE WINDOWS?\n"; // We're out of windows with > 3
				    // huh open, huh?
  }
  // Let's let em know she's deleted!
  emit delete_toplevel(QString(server), QString(name));
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
  emit changeChannel(QString(server), old_chan, new_chan);
}

void KSircProcess::filters_update()
{
  iocontrol->stdin_write(QString("/crule\n"));
  kConfig->setGroup("FilterRules");
  int max = kConfig->readNumEntry("Rules", 0);
  QString command;
  QString next_part;
  QString key;
  QString data;
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
