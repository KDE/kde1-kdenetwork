#include "ksircprocess.h"
#include "toplevel.h"
#include "ioBroadcast.h"
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

  running_window = TRUE;        // True so we do create the default
  new_toplevel("!default");     // 

  kConfig->setGroup("GlobalOptions");
  if(kConfig->readNumEntry("Reuse", TRUE) == TRUE){
  }
  else{
    running_window = TRUE;        // True so we do create the a new message
    new_toplevel("!messages");
    //    QObject::disconnect((QObject *) TopList["!messages"],
    //	       SIGNAL(currentWindow(KSircTopLevel *)));
    //    QObject::disconnect((QObject *) TopList["!messages"], 
    //    	       SIGNAL(changeChannel(QString, QString)));
  }

  running_window = FALSE;       // set false so next changes the first name
  default_follow_focus = TRUE;

  TopList.insert("!all", new KSircIOBroadcast(this));
  
  //  wm->show();

}

KSircProcess::~KSircProcess()
{
  delete proc;
  delete iocontrol;
  emit delete_toplevel(QString(server), QString());
}

void KSircProcess::new_toplevel(QString str)
{
  if(running_window == FALSE){
    running_window = TRUE;
    TopList.insert(str, TopList["!default"]);
    TopList[str]->control_message(QString("001") + str);
  }
  else if(!TopList[str]){
    KSircTopLevel *wm = new KSircTopLevel(this, qstrdup(str.data()));
    TopList.insert(str, wm);
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
    wm->show();
  }
  else{
    cerr << "Window " << str << " already exists\n";
  }
}

//void KSircProcess::new_ksircprocess(QString str)
//{
//  new KSircProcess(qstrdup(str.data()));
//
//}

void KSircProcess::close_toplevel(KSircTopLevel *wm, char *name)
{

  bool is_default = FALSE;

  if(TopList.count() <= 3){
    iocontrol->stdin_write(QString("/quit\n"));
    delete this;
    return;
  }
  else if(wm == TopList["!default"]){
    is_default = TRUE;
  }

  if(TopList[name]){
    TopList.remove(name);
    cerr << "DELETE THE WINDOW: " << name << " !!!!\n";
  }
  else
    cerr << "DID NOT DELETE THE WINDOW: " << name << " !!!!\n";

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
      cerr << "NO MORE WINDOWS?\n";
  }

  emit delete_toplevel(QString(server), QString(name));
}

void KSircProcess::default_window(KSircTopLevel *w){

  if(w && (default_follow_focus == TRUE))
    TopList.replace("!default", w);

}

void KSircProcess::recvChangeChannel(QString old_chan, QString new_chan){
  emit changeChannel(QString(server), old_chan, new_chan);
}
