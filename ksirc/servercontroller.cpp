/**********************************************************************

	--- Qt Architect generated file ---

	File: servercontroller.cpp
	Last generated: Sat Nov 29 08:50:19 1997

 *********************************************************************/

#include <qpopmenu.h>

#include "servercontroller.h"
#include "open_ksirc.h"
#include "open_top.h"
#include <iostream.h>

#include <qkeycode.h>

#define Inherited servercontrollerData

servercontroller::servercontroller
(
	QWidget* parent,
	const char* name
)
	:
	Inherited( parent, name )
{
	setCaption( "Server Control" );
	QPopupMenu *connections = new QPopupMenu();
	connections->insertItem("New Server...", this, SLOT(new_connection()), CTRL + Key_N );
	connections->insertSeparator();
	connections->insertItem("Join Channel...", this, SLOT(new_channel()), CTRL + Key_J);
	MenuBar->insertItem("Connections", connections);
	setMenu(MenuBar);
	
	ConnectionTree->setExpandLevel(2);
}


servercontroller::~servercontroller()
{
}

void servercontroller::new_connection()
{
  open_ksirc *w = new open_ksirc();
  connect(w, SIGNAL(open_ksircprocess(QString)),
          this, SLOT(new_ksircprocess(QString)));
  w->show();
}

void servercontroller::new_ksircprocess(QString str)
{
  if(proc_list[str.data()])
    return;
  ConnectionTree->insertItem(str.data(), NULL, -1, FALSE);
  add_toplevel(str, QString("default"));
  KSircProcess *proc = new KSircProcess(qstrdup(str.data()));
  proc_list.insert(str.data(), proc);
  connect(proc, SIGNAL(made_toplevel(QString, QString)),
	  this, SLOT(add_toplevel(QString, QString)));
  connect(proc, SIGNAL(delete_toplevel(QString, QString)),
	  this, SLOT(delete_toplevel(QString, QString)));
  connect(proc, SIGNAL(changeChannel(QString, QString, QString)),
	  this, SLOT(recvChangeChannel(QString, QString, QString)));
}

void servercontroller::new_channel()
{
  open_top *w = new open_top();
  connect(w, SIGNAL(open_toplevel(QString)),
	  this, SLOT(new_toplevel(QString)));
  w->show();
}

void servercontroller::new_toplevel(QString str)
{
  KTreeListItem *citem = ConnectionTree->getCurrentItem();
  if(citem){
    if(proc_list[citem->getText()]){
      proc_list[citem->getText()]->new_toplevel(str);
    }
    else if(proc_list[citem->getParent()->getText()]){
      proc_list[citem->getParent()->getText()]->new_toplevel(str);
    }
    cerr << "Server is: " << citem->getText() << endl;
  }
}

void servercontroller::add_toplevel(QString parent, QString child)
{
  KPath path;
  path.push(&parent);
  ConnectionTree->addChildItem(child.data(), NULL, &path);
  cerr << "Added child for: " << parent << "->" << child << endl;

}

void servercontroller::delete_toplevel(QString parent, QString child)
{
  KPath path;
  path.push(&parent);

  if(child.isEmpty() == FALSE){
    if(child[0] == '!')
      child.remove(0, 1);
    path.push(&child);
  }
  else
    proc_list.remove(parent);

  ConnectionTree->removeItem(&path);
  cerr << "Removed child for: " << parent << "->" << child << endl;

}


void servercontroller::recvChangeChannel(QString parent, QString old_chan, QString new_chan)
{

  if(old_chan[0] == '!')
    old_chan.remove(0, 1);
  
  KPath path;
  path.push(&parent);
  path.push(&old_chan);
  cerr << "Deleteing " << old_chan << endl;
  ConnectionTree->removeItem(&path);
  path.pop();
  ConnectionTree->addChildItem(new_chan.data(), NULL, &path);
}
