/**********************************************************************

 Server Controller 

 $$Id$$

 Main Server Controller.  Displays server connection window, and makes 
 new server connection on demand.

 Signals: NONE

 Slots: 

   new_connection(): Creates popup asking for new connection

   new_ksircporcess(QString): 
      Args:
         QString: new server name or IP to connect to.
      Action:
	 Creates a new sirc process and window !default connected to the 
	 server.  Does nothing if a server connection already exists.
   
   add_toplevel(QString parent, QString child):
      Args:
	   parent: the server name that the new channel is being joined on
	   child: the new channel name
      Action:
         Adds "child" to the list of joined channles in the main 
	 window.  Always call this on new window creation!

   delete_toplevel(QString parent, QString child):
      Args:
         parent: the server name of which channel is closing
	         child: the channle that is closing. IFF Emtpy, parent is 
		 deleted.
      Action:
	 Deletes the "child" window from the list of connections.  If 
	 the child is Empty the whole tree is removed since it is assumed 
         the parent has disconnected and is closing.

   new_channel:  Creates popup asking for new channel name

   new_toplevel(QString str): 
      Args:
         str: name of the new channel to be created
      Action:
         Sends a signal to the currently selected server in the tree
         list and join the requested channel.  Does nothing if nothing
         is selected in the tree list.

   recvChangeChanel(QString parent, QString old, QString new):
      Args:
         parent: parent server connection
         old: the old name for the window
         new: the new name for the window
      Action:
          Changes the old window name to the new window name in the tree
          list box.  Call for all name change!
   reuse()
      Action:
          Toggles the Global option to reuse the !default window on each
          call.  Sets the menu item.   
 
 *********************************************************************/

#include <qpopmenu.h>

#include "servercontroller.h"
#include "open_ksirc.h"
#include "open_top.h"
#include "KSircColour.h"
#include "config.h"
#include "FilterRuleEditor.h"
#include <iostream.h>

#include <qkeycode.h>

#define Inherited servercontrollerData
extern KConfig *kConfig;
extern KApplication *kApp;
extern global_config *kSircConfig;

servercontroller::servercontroller
(
	QWidget* parent,
	const char* name
)
	:
	Inherited( parent, name )
{
	setCaption( "Server Control" );
	QPopupMenu *file = new QPopupMenu();
	file->insertItem("&Quit", kApp, SLOT(quit()), ALT + Key_F4);
	MenuBar->insertItem("&File", file);
	connections = new QPopupMenu();
	server_id = connections->insertItem("&New Server...", this, SLOT(new_connection()), CTRL + Key_N );
	join_id = connections->insertItem("&Join Channel...", this, SLOT(new_channel()), CTRL + Key_J);
	connections->setItemEnabled(join_id, FALSE);
	MenuBar->insertItem("&Connections", connections);
	
	
	kConfig->setGroup("GlobalOptions");
	options = new QPopupMenu();
	reuse_id = options->insertItem("Seperate Message Window", 
			    this, SLOT(reuse()));
	options->setItemChecked(reuse_id, 
				! kConfig->readNumEntry("Reuse", TRUE));
	auto_id = options->insertItem("Auto Create Windows", 
			    this, SLOT(autocreate()));
	options->insertSeparator();
	options->setItemChecked(auto_id, 
				kConfig->readNumEntry("AutoCreate", FALSE));
	kSircConfig->autocreate = kConfig->readNumEntry("AutoCreate", FALSE);
	options->insertItem("Colour Preferences...",
			    this, SLOT(colour_prefs()));
	options->insertItem("Filter Rule Editor...",
			    this, SLOT(filter_rule_editor()));
	MenuBar->insertItem("&Options", options);

	setMenu(MenuBar);
	setView(BaseBorder);
	
	ConnectionTree->setExpandLevel(2);
	open_toplevels = 0;

	pic_server = new QPixmap("img/mini-display.xpm");
	pic_channel = new QPixmap("img/mini-edit.xpm");

}


servercontroller::~servercontroller()
{
}

void servercontroller::new_connection()
{
  open_ksirc *w = new open_ksirc();              // Create new ksirc popup
  connect(w, SIGNAL(open_ksircprocess(QString)), // connected ok to process
          this, SLOT(new_ksircprocess(QString)));// start
  w->show();                                     // show the sucker!
}

void servercontroller::new_ksircprocess(QString str)
{

  if(str.isEmpty() == TRUE)  // nothing entered, nothing done
    return;
  if(proc_list[str.data()])   // if it already exists, quit
    return;

  ConnectionTree->insertItem(str.data(), pic_server, -1, FALSE); // Insert new base
                                                           // level parent
  add_toplevel(str, QString("default"));                   // Set a dflt chan
  KSircProcess *proc = new KSircProcess(qstrdup(str.data())); // Create proc
  proc_list.insert(str.data(), proc);                      // Add proc to hash
  connect(proc, SIGNAL(made_toplevel(QString, QString)),   // Connect new 
	  this, SLOT(add_toplevel(QString, QString)));     //
  connect(proc, SIGNAL(delete_toplevel(QString, QString)), // Connect delete
	  this, SLOT(delete_toplevel(QString, QString)));  //
  connect(proc, SIGNAL(changeChannel(QString, QString, QString)), //Name change
	  this, SLOT(recvChangeChannel(QString, QString, QString)));
  connect(this, SIGNAL(filters_update()),
	  proc, SLOT(filters_update()));
  
  if(!ConnectionTree->getCurrentItem()){   // If nothing's highlighted
    ConnectionTree->setCurrentItem(0);     // highlight it.
  }

  connections->setItemEnabled(join_id, TRUE);
}

void servercontroller::new_channel()
{
  open_top *w = new open_top();                // Create new channel popup
  connect(w, SIGNAL(open_toplevel(QString)),   // Connect ok to right slot
	  this, SLOT(new_toplevel(QString)));
  w->show();                                   // Show me baby!
}

void servercontroller::new_toplevel(QString str)
{
  KTreeListItem *citem = ConnectionTree->getCurrentItem(); // get item
  if(citem){ // if it exist, ie something is highlighted, continue
    if(proc_list[citem->getText()]){ // If it's a match with a server, ok
      proc_list[citem->getText()]->new_toplevel(str);
    }
    // Otherwise, check the parent to see it's perhaps a server.
    else if(proc_list[citem->getParent()->getText()]){
      proc_list[citem->getParent()->getText()]->new_toplevel(str);
    }
    //cerr << "Server is: " << citem->getText() << endl;
  }
}

void servercontroller::add_toplevel(QString parent, QString child)
{
  // Add new channel, first add the parent to the path
  KPath path;
  path.push(&parent);
  // add a new child item with parent as it's parent
  ConnectionTree->addChildItem(child.data(), pic_channel, &path);
  //cerr << "Added child for: " << parent << "->" << child << endl;
  open_toplevels++;
}

void servercontroller::delete_toplevel(QString parent, QString child)
{

  // Add parent to path
  KPath path;
  path.push(&parent);

  // If the child is emtpy, delete the whole tree, otherwise just the child
  if(child.isEmpty() == FALSE){
    if(child[0] == '!')
      child.remove(0, 1); // If the first char is !, it's control, remove it
    path.push(&child);    // Since it's not null, add the child to be deleted
  }
  else
    proc_list.remove(parent); // Remove process entry while we are at it

  ConnectionTree->removeItem(&path); // Remove the item
  //cerr << "Removed child for: " << parent << "->" << child << endl;
  open_toplevels--;
  if(open_toplevels <= 0)
    connections->setItemEnabled(join_id, FALSE);

}


void servercontroller::recvChangeChannel(QString parent, QString old_chan, QString new_chan)
{

  //  If the channel has a !, it's a control channel, remove the !

  if(old_chan[0] == '!')
    old_chan.remove(0, 1);
  if(new_chan[0] == '!')
    new_chan.remove(0, 1);

  // The path to the old one it parent->old_chan, create it
  KPath path;
  path.push(&parent);
  path.push(&old_chan);
  // Delete the old one
  //cerr << "Deleteing " << old_chan << endl;
  ConnectionTree->removeItem(&path);
  // Only create with the parent in the path
  path.pop();
  // Add new child.  Delete/creates wrecks the "random" sort order though.
  ConnectionTree->addChildItem(new_chan.data(), pic_channel, &path);
}

void servercontroller::reuse()
{
  kConfig->setGroup("GlobalOptions");
  if(kConfig->readNumEntry("Reuse", TRUE) == TRUE){
    options->setItemChecked(reuse_id, TRUE);
    kConfig->writeEntry("Reuse", FALSE);
  }
  else{
    options->setItemChecked(reuse_id, FALSE);
    kConfig->writeEntry("Reuse", TRUE);
  }
  kConfig->sync();
}

void servercontroller::autocreate()
{
  kConfig->setGroup("GlobalOptions");
  if(kConfig->readNumEntry("AutoCreate", FALSE) == FALSE){
    options->setItemChecked(auto_id, TRUE);
    kConfig->writeEntry("AutoCreate", TRUE);
    kSircConfig->autocreate = TRUE;
  }
  else{
    options->setItemChecked(auto_id, FALSE);
    kConfig->writeEntry("AutoCreate", FALSE);
    kSircConfig->autocreate = FALSE;
  }
  kConfig->sync();
}

void servercontroller::colour_prefs()
{
  (new KSircColour())->show();
}

void servercontroller::filter_rule_editor()
{
  FilterRuleEditor *fe = new FilterRuleEditor();
  connect(fe, SIGNAL(destroyed()), 
	  this, SIGNAL(filters_update()));
  fe->show();
}
