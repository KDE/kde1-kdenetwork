/**********************************************************************

	--- Qt Architect generated file ---

	File: servercontroller.h
	Last generated: Sat Nov 29 08:50:19 1997
	
 Now Under CVS control.

 $$Id$$

 *********************************************************************/

#ifndef servercontroller_included
#define servercontroller_included

#include <qdict.h>
#include <qpixmap.h> 

#include "servercontrollerData.h"

#include "ksircprocess.h"

class servercontroller : public servercontrollerData
{
    Q_OBJECT

public:

    servercontroller
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~servercontroller();

signals:
    /**
      * Filter rules have changed, need to re-read and update.
      */
    virtual void filters_update();

public slots:
    // All slots are described in servercontroll.cpp file
    /**
      * Creates popup asking for new connection
      */
    virtual void new_connection(); 
    /**
      *  Args:
      *    QString: new server name or IP to connect to.
      *  Action:
      *	 Creates a new sirc process and window !default connected to the 
      *	 server.  Does nothing if a server connection already exists.
      */
    virtual void new_ksircprocess(QString);
    /**
      *  Args:
      *	   parent: the server name that the new channel is being joined on
      *    child: the new channel name
      *  Action:
      *    Adds "child" to the list of joined channles in the main 
      *    window.  Always call this on new window creation!
      */
    virtual void add_toplevel(QString, QString); 
    /**
      *  Args:
      *    parent: the server name of which channel is closing
      *	   child: the channle that is closing. IFF Emtpy, parent is 
      *	   deleted.
      *  Action:
      *	   Deletes the "child" window from the list of connections.  If 
      *	   the child is Empty the whole tree is removed since it is assumed 
      *    the parent has disconnected and is closing.
      */
    virtual void delete_toplevel(QString, QString); 
    /**
      * Creates popup asking for new channel name
      */
    virtual void new_channel();
    /**
      *  Args:
      *    str: name of the new channel to be created
      *  Action:
      *    Sends a signal to the currently selected server in the tree
      *    list and join the requested channel.  Does nothing if nothing
      *    is selected in the tree list.
      */
    virtual void new_toplevel(QString str);
    /**
      *  Args:
      *    parent: parent server connection
      *    old: the old name for the window
      *    new: the new name for the window
      *  Action:
      *    Changes the old window name to the new window name in the tree
      *    list box.  Call for all name change!
      */
    virtual void recvChangeChannel(QString, QString, QString);
    /**
      *  Action:
      *     Toggles the Global option to reuse the !default window on each
      *     call.  Sets the menu item.   
      */
    virtual void reuse();
    virtual void autocreate();
    virtual void colour_prefs();
    virtual void font_prefs();
    virtual void font_update(const QFont&);
    virtual void filter_rule_editor();
    virtual void configChange();
    virtual void nickcompletion();
    virtual void help_general();
    virtual void help_colours();
    virtual void help_filters();
    virtual void help_keys();
    virtual void about_ksirc();

private:
    // Hold a list of all KSircProcess's for access latter.  Index by server 
    // name
  QDict<KSircProcess> proc_list;
  QPopupMenu *options, *connections;
  int reuse_id, join_id, server_id, auto_id, nickc_id;

  int open_toplevels;

  QPixmap *pic_channel;
  QPixmap *pic_server;

};
#endif // servercontroller_included
