#include "iocontroller.h"
#include "toplevel.h"
#include <iostream.h>
#include "ksircprocess.h"
#include "control_message.h"

extern KApplication *kApp;

int KSircIOController::counter = 0;

KSircIOController::KSircIOController(KProcess *_proc, KSircProcess *_ksircproc)
  : QObject()
{

  if(counter > 0){
    cerr << "Trying to open mode than one IOController!\n";
    counter++;
  }
  else{
    counter++;
  }

  proc = _proc;
  ksircproc = _ksircproc;

  proc->writeStdin("/eval $ssfe=1\n", 14);

  connect(proc, SIGNAL(receivedStdout(KProcess *, char *, int)),
          this, SLOT(stdout_read(KProcess*, char*, int))); 
                                              // Connect the data arrived
                                              // to sirc receive for adding
                                              // the main text window  
  connect(proc, SIGNAL(processExited(KProcess *)),
	  this, SLOT(sircDied(KProcess *)));
}

void KSircIOController::stdout_read(KProcess *, char *_buffer, int buflen)
{
  int pos,pos2,pos3;
  QString name, line, tmp;
  char buf[buflen+1];

  strncpy(buf, _buffer, buflen);

  //  buf[buflen] = 0;

  QString buffer(buf, buflen);

  name = "!default";

  if(buf[buflen-1] != '\n'){
    cerr << "End not Enter" << endl;
    pos2 = buffer.length();
    pos = buffer.findRev('\n', pos2);
    if(pos != -1){
      tmp = buffer.mid(pos+1, pos2-pos-1);
      buffer.truncate(pos);
      //	cerr << "Now Holding: " << tmp << endl;
    }
  }
  if(holder.length() > 0){
    //      cerr << "Prepending: " << holder << endl;
    buffer.prepend(holder);
    holder.truncate(0);
  }
  pos = pos2 = 0;
  QString control;
  control.setNum(STOP_UPDATES);
  ksircproc->TopList["!all"]->control_message(control);
  do{
    pos2 = buffer.find('\n', pos);
    //      cerr << "Pos1/2: " << pos << '/' << pos2 << endl;
    
    if(pos2 == -1)
      pos2 = buffer.length();
    
    line = buffer.mid(pos, pos2 - pos);
    if((line.length() > 0) && (line[0] == '~')){
      pos3 = line.find('~', 1);
      if(pos3 > 0){
	name = line.mid(1,pos3-1);
	name = name.lower();
	line.remove(0, pos3+1);
      }
      //	cerr << "Dest: " << name << endl;
    }
    if(!(ksircproc->TopList)[name]){
      name = "!default";
      if(line[0] == '`')
	line.prepend("*** ");
    }
    //    cerr << "Output: " << line << endl;
    ksircproc->TopList[name]->sirc_receive(line);
    pos = pos2+1;
  } while((uint) pos < buffer.length());

  holder = tmp;
  control.truncate(0);
  control.setNum(RESUME_UPDATES);
  ksircproc->TopList["!all"]->control_message(control);

}

KSircIOController::~KSircIOController()
{
}

void KSircIOController::stderr_read(KProcess *, char *, int)
{
}

void KSircIOController::stdin_write(QString s)
{

  proc->writeStdin(s.data(), s.length());
  //  write(sirc_stdin, s, s.length());

}

void KSircIOController::sircDied(KProcess *)
{

  cerr << "Sirc DIED!!!!!!!\n";
  cerr << "should do something....\n";
  
}
