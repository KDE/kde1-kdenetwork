//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// This file os part of KRN, a newsreader for the KDE project.              //
// KRN is distributed under the GNU General Public License.                 //
// Read the acompanying file COPYING for more info.                         //
//                                                                          //
// KRN wouldn't be possible without these libraries, whose authors have     //
// made free to use on non-commercial software:                             //
//                                                                          //
// MIME++ by Doug Sauder                                                    //
// Qt     by Troll Tech                                                     //
//                                                                          //
// This file is copyright 1997 by                                           //
// Roberto Alsina <ralsina@unl.edu.ar>                                      //
// Magnus Reftel  <d96reftl@dtek.chalmers.se>                               //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////
#include "groupdlg.h"

#define Inherited KTopLevelWidget

#include <qapp.h>
#include <qfile.h>
#include <qstring.h>
#include <qtstream.h>
#include <qevent.h>

#include <kmsgbox.h>
#include <Kconfig.h>
#include <kkeyconf.h>

#include "PostDialog.h"

#include "artdlg.h"
#include "identDlg.h"
#include "NNTPConfigDlg.h"
#include "rmbpop.h"
#include "aboutDlg.h"

#define CONNECT 1
#define DISCONNECT 5
#define GET_ACTIVE 3
#define SUBSCRIBE 4
#define LOAD_FILES 2
#define CHECK_UNREAD 6
#define CHANGE_IDENTITY 8
#define CONFIG_NNTP 9
#define EXIT 7
#define OPENGROUP 10
#define TAGGROUP 11
#define HELP_CONTENTS 12
#define HELP_ABOUT 13

extern QString krnpath,cachepath,artinfopath,pixpath;
extern KConfig *conf;


QPixmap *txt_xpm;
QPixmap *folder_xpm;
QPixmap *sub_xpm;
QPixmap *tag_xpm;

GroupList groups;
GroupList subscr;

bool checkPixmap(KTreeListItem *item,void *)
{
    QString name(item->getText());
    if (name.right(1)==".") //it's a folder
    {
        item->setPixmap(folder_xpm);
        return false;
    }
    int i=groups.find(&NewsGroup(name));
    if (i!=-1)
    {
        if (groups.at(i)->isTagged)
        {
            item->setPixmap(tag_xpm);
            return false;
        }
    }
    if (subscr.find(&NewsGroup(name))==-1) //it ain't subscribed
    {
        item->setPixmap(txt_xpm);
        return false;
    }
    
    item->setPixmap(sub_xpm);
    return false;
}

Groupdlg::Groupdlg
    (
     const char *name
    )
    :
    Inherited (name)
{
    groups.setAutoDelete(true);
    subscr.setAutoDelete(true);
    txt_xpm = new QPixmap (pixpath+"txt.xpm");
    folder_xpm = new QPixmap (pixpath+"folder.xpm");
    sub_xpm = new QPixmap (pixpath+"subscr.xpm");
    tag_xpm = new QPixmap (pixpath+"tagged.xpm");
    
    QPopupMenu *file = new QPopupMenu;
    file->insertItem("Connect to Server",CONNECT);
    file->insertItem("Disconnect From Server",DISCONNECT);
    file->insertItem("Get Active File",GET_ACTIVE);
    file->insertSeparator();
    file->insertItem("Exit",EXIT);
    connect (file,SIGNAL(activated(int)),SLOT(actions(int)));

    QPopupMenu *newsgroup = new QPopupMenu;
    newsgroup->insertItem("Open",OPENGROUP);
    newsgroup->insertItem("(un)Subscribe",SUBSCRIBE);
    newsgroup->insertItem("(Un)Tag",TAGGROUP);
    connect (newsgroup,SIGNAL(activated(int)),SLOT(actions(int)));
    
    QPopupMenu *options = new QPopupMenu;
    options->insertItem("Identity",CHANGE_IDENTITY);
    options->insertItem("NNTP Options",CONFIG_NNTP);
    connect (options,SIGNAL(activated(int)),SLOT(actions(int)));

    QPopupMenu *help = new QPopupMenu;
    help->insertItem("Contents",HELP_CONTENTS);
    help->insertSeparator();
    help->insertItem("About",HELP_ABOUT);
    connect (help,SIGNAL(activated(int)),SLOT(actions(int)));

    KMenuBar *menu = new KMenuBar (this, "menu");
    
    menu->insertItem ("&File", file);
    menu->insertItem ("&Newsgroup", newsgroup);
    menu->insertItem ("&Options", options);
    menu->insertSeparator();
    menu->insertItem ("&Help", help);
    
    setMenu (menu);
    
    QPixmap pixmap;
    
    KToolBar *tool = new KToolBar (this, "tool");
    QObject::connect (tool, SIGNAL (clicked (int)), this, SLOT (actions (int)));
    
    pixmap.load (pixpath+"connected.xpm");
    tool->insertButton (pixmap, CONNECT, true, "Connect to server");
    pixmap.load (pixpath+"disconnected.xpm");
    tool->insertButton (pixmap, DISCONNECT, false, "Disconnect from server");
    tool->insertSeparator ();
    
    pixmap.load (pixpath+"previous.xpm");
    tool->insertButton (pixmap, GET_ACTIVE, true, "Get list of active groups");
    tool->insertSeparator ();
    
    tool->insertButton (*sub_xpm, SUBSCRIBE, true, "(Un)Subscribe");
    addToolBar (tool);
    tool->insertSeparator ();
    pixmap.load (pixpath+"reload.xpm");
    tool->insertButton (pixmap, CHECK_UNREAD, true, "Check for Unread Articles");
    
    
    list = new KTreeList (this, "");
    QObject::connect (list, SIGNAL (selected (int)), this, SLOT (openGroup (int)));
    setView (list);
    RmbPop *filter=new RmbPop(list);
    delete (filter->pop);
    filter->pop=newsgroup;
    
    
    KStatusBar *status = new KStatusBar (this, "status");
    status->insertItem ("", 1);
    status->show ();
    setStatusBar (status);

    conf->setGroup("NNTP");
    QString sname=conf->readEntry("NNTPServer");
    server = new NNTP (sname.data());

    
    show ();
    resize (500, 400);
    actions (LOAD_FILES);

    if (conf->readNumEntry("ConnectAtStart"))
        actions(CONNECT);
    fillTree();
}

Groupdlg::~Groupdlg ()
{
}

void Groupdlg::openGroup (QString name)
{
    if (name.find('/')==0)
        name=name.right(name.length()-1);
    debug ("Really spawning %s",name.data());
    int i=groups.find(&NewsGroup(name));
    if (groups.at(i)->isVisible)
    {
        debug ("well, no, I won't spawn it again");
        return;
    }
    if (i!=-1)
    {
        groups.at(i)->load();
        Artdlg *a = new Artdlg (groups.at(i),server);
        QObject::connect(a->messwin,SIGNAL(spawnGroup(QString)),this,SLOT(openGroup(QString)));
        QObject::connect(a,SIGNAL(needConnection()),this,SLOT(needsConnect()));
    }
    else
    {
        KMsgBox:: message (0, "Sorry!",
        "That newsgroup is not in the current active file\n Krn can't handle this gracefully right now", KMsgBox::INFORMATION);
    }
}



void Groupdlg::openGroup (int index)
{
    QString base;
    KTreeListItem *it=list->itemAt(index);
    debug ("clicked on %s",it->getText());
    if (it->getText()[strlen(it->getText())-1]!='.')
    {
        QString temp=it->getText();
        int i=temp.find(' ');
        if (i>0)
        {
            temp=temp.left(i);;
        };
        openGroup(temp);
    }
    else
    {
        if (!it->hasChild())
        {
            if (!strcmp("All Newsgroups.",it->getText()))
            {
                base="";
            }
            else
            {
                base=it->getText();
            }
            int l=base.length();
            int c=base.contains('.');
            char tc;
            QStrList bases;
            bases.setAutoDelete(true);
            for (NewsGroup *iter = groups.first(); iter != 0; iter = groups.next ())
            {
                //this group's name matches the base
                if (!strncmp(base.data(),iter->data(),l))
                {
                    QString gname=iter->data();
                    //Check the dot count.
                    //If it's the same, then it's a group
                    //Add it as a child
                    if (gname.contains('.')==c)
                    {
                        if (iter->isTagged)
                            list->addChildItem(iter->data(),tag_xpm,index);
                        else
                            list->addChildItem(iter->data(),txt_xpm,index);
                    }
                    
                    else  //It may be a new hierarchy
                    {
                        //take what the new base would be
                        char *nextdot=strchr(iter->data()+l+1,'.')+1;
                        tc=nextdot[0];
                        nextdot[0]=0;
                        if (bases.find(iter->data())==-1)
                        {
                            // It's new, so add it to the base list
                            // and insert it as a folder
                            bases.append(iter->data());
                            if (iter->isTagged)
                                list->addChildItem(iter->data(),tag_xpm,index);
                            else
                                list->addChildItem(iter->data(),folder_xpm,index);
                        }
                        nextdot[0]=tc;
                    }
                }
            }
            list->forEveryVisibleItem(checkPixmap,NULL);
            list->repaint();
            list->expandItem(index);
        }
        else
        {
            if (it->isExpanded())
                list->collapseItem(index);
            else
                list->expandItem(index);
        }
    }
}




void Groupdlg::subscribe ()
{
    const char *text = list->getCurrentItem ()->getText ();
    KPath path;
    int index=subscr.find (&NewsGroup(text));
    if (-1 != index)
    {
        subscr.remove ();

        path.push (new QString ("Subscribed Newsgroups."));
        path.push (new QString (text));
        list->removeItem (&path);
        if (list->itemAt(0)->isExpanded() &&
            ((unsigned int)list->currentItem()>list->itemAt(0)->childCount()+1))
            list->setCurrentItem(list->currentItem()-1);
    }
    else
    {
        NewsGroup *n=new NewsGroup(text);
        if (-1 != groups.find (n))
        {
            list->addChildItem (text, sub_xpm, 0);
            subscr.append (new NewsGroup(text));
        if (list->itemAt(0)->isExpanded() &&
            ((unsigned int)list->currentItem()>list->itemAt(0)->childCount()+1))
            list->setCurrentItem(list->currentItem()+1);
        }
        delete n;
    };
    list->forEveryVisibleItem(checkPixmap,NULL);
    list->repaint();
    saveSubscribed();
}

void Groupdlg::tag ()
{
    const char *text = list->getCurrentItem ()->getText ();
    int index=groups.find (&NewsGroup(text));
    NewsGroup *g=groups.at(index);
    if (-1 != index)
    {
        if(g->isTagged)
            g->isTagged=false;
        else
            g->isTagged=true;
    }
    list->forEveryVisibleItem(checkPixmap,NULL);
    list->repaint();
}


void Groupdlg::offline()
{
    toolBar()->setItemEnabled (DISCONNECT,false);
    server->disconnect();
    toolBar()->setItemEnabled (CONNECT,true);
    statusBar ()->changeItem ("Disconnected", 1);
    qApp->processEvents ();
}

void Groupdlg::online()
{
    toolBar()->setItemEnabled (CONNECT,false);
    statusBar ()->changeItem ("Connecting to server", 1);
    qApp->processEvents ();
    if (server->connect ())
    {
        if (server->isReadOnly ())
            statusBar ()->changeItem ("Connected to server - Posting not allowed", 1);
        else
            statusBar ()->changeItem ("Connected to server - Posting allowed", 1);
    }
    else
    {
        qApp->setOverrideCursor (arrowCursor);
        KMsgBox:: message (0, "Error", "Can't connect to server", KMsgBox::INFORMATION);
        qApp->restoreOverrideCursor ();
        statusBar ()->changeItem ("Connection to server failed", 1);
        qApp->processEvents ();
        toolBar()->setItemEnabled (CONNECT,true);
    }
    if (conf->readNumEntry("Authenticate")!=0)
    {
        debug ("authenticating");
        if (299<server->authinfo(conf->readEntry("Username"),conf->readEntry("Password")))
        {
            qApp->setOverrideCursor (arrowCursor);
            KMsgBox:: message (0, "Error", "Authentication Failed", KMsgBox::INFORMATION);
            qApp->restoreOverrideCursor ();
            actions(DISCONNECT);
            statusBar ()->changeItem ("Connection to server failed: Authentication problem", 1);
            qApp->processEvents ();
            toolBar()->setItemEnabled (CONNECT,true);
        }
    }
    toolBar()->setItemEnabled (DISCONNECT,true);
}

void Groupdlg::fillTree ()
{
    list->insertItem ("Subscribed Newsgroups.", folder_xpm);
    for (NewsGroup *iter = subscr.first (); iter != 0; iter = subscr.next ())
        list->addChildItem (iter->data(), sub_xpm, 0);

    list->insertItem ("All Newsgroups.", folder_xpm);
}

bool Groupdlg::needsConnect()
{
    bool success=false;
    qApp->setOverrideCursor (arrowCursor);
    debug ("asking to connect");
    if (server->isConnected())
    {
        success=true;
    }
    else
    {
        if (1==KMsgBox::yesNo(0,"Krn-Question","The operation you requested needs a connection to the News server\nShould I attempt one?"))
        {
            actions(CONNECT);
            success=true;
        }
    }
    qApp->restoreOverrideCursor ();
    return success;
}


bool Groupdlg::actions (int action)
{
    bool success=false;
    qApp->setOverrideCursor (waitCursor);
    switch (action)
    {
    case OPENGROUP:
        {
            openGroup (list->currentItem());
            break;
        }
    case TAGGROUP:
        {
            tag();
            break;
        }
    case CHANGE_IDENTITY:
        {
            qApp->setOverrideCursor (arrowCursor);
            IdentDlg id;
            if (id.exec())
            {
                debug ("Changing identity.... go undercover!");
                conf->setGroup("Identity");
                conf->writeEntry("Address",id.address->text());
                conf->writeEntry("RealName",id.realname->text());
                conf->writeEntry("Organization",id.organization->text());
                conf->sync();
            }
            qApp->restoreOverrideCursor ();
            success = true;
            break;
        }

    case CONFIG_NNTP:
        {
            qApp->setOverrideCursor (arrowCursor);
            NNTPConfigDlg dlg;
            if (dlg.exec())
            {
                debug ("Configuring NNTP");
                conf->setGroup("NNTP");
                conf->writeEntry("NNTPServer",dlg.servername->text());
                conf->writeEntry("ConnectAtStart",dlg.connectatstart->isChecked());
                conf->writeEntry("Authenticate",dlg.authenticate->isChecked());
                conf->writeEntry("Username",dlg.username->text());
                conf->writeEntry("Password",dlg.password->text());
                conf->sync();
            }
            qApp->restoreOverrideCursor ();
            success = true;
            break;
        }
        
    case EXIT:
        {
            qApp->setOverrideCursor (arrowCursor);
            int i = KMsgBox::yesNo (0, "Question", "Do you REALLY wanna exit KRN?");
            if (i==1)
                qApp->exit();
            qApp->restoreOverrideCursor ();
            success = true;
            break;
        }
    case CONNECT:
        {
            online();
            break;
        }
    case DISCONNECT:
        {
            offline();
            break;
        }
    case LOAD_FILES:
        {
            loadSubscribed();
            loadActive();
            break;
        }
    case GET_ACTIVE:
        {
            if (needsConnect())
            {
                statusBar ()->changeItem ("Getting active list from server", 1);
                qApp->processEvents ();
                server->groupList (&groups,true);
                loadActive();
            }
            else
            {
                success = false;
            };
            break;
        }
    case SUBSCRIBE:
        {
            subscribe ();
            break;
        }
    case CHECK_UNREAD:
        {
            if (needsConnect())
            {
                printf ("check_unread\n");
                checkUnread();
            }
            break;
        }
    case HELP_ABOUT:
        {
            qApp->setOverrideCursor (arrowCursor);
            debug ("about krn");
            aboutDlg ab;
            ab.exec();
            qApp->restoreOverrideCursor ();
            success = true;
            break;
        }
    };
    
    
    qApp->restoreOverrideCursor ();
    return success;
}

bool Groupdlg::loadSubscribed()
{
    QString ac;
    ac=krnpath+"subscribed";
    QFile f(ac.data ());
    if (f.open (IO_ReadOnly))
    {
        subscr.clear();
        QTextStream st(&f);
        while (1)
        {
            ac=st.readLine ();
            if (st.eof())
                break;
            subscr.append (new NewsGroup(ac.data()));
        };
        f.close ();
        return true;
    }
    else
    {
        warning("Can't open subscribed file for writing!");
        return false;
    }
}

bool Groupdlg::saveSubscribed()
{
    QString ac;
    ac=krnpath+"/subscribed";
    unlink(ac.data());
    QFile f(ac.data ());
    if (f.open (IO_WriteOnly))
    {
        for (NewsGroup *it = subscr.first (); it != 0; it = subscr.next ())
        {
            f.writeBlock(it->data(),it->length());
            f.writeBlock("\n",1);
        }
        f.close ();
        return true;
    }
    else
    {
        printf ("Can't save subscribed file\n");
        return false;
    }
}

bool Groupdlg::loadActive()
{
    bool success=false;
    QString ac;
    //check if the "active" file exists
    ac=krnpath+"/active";
    QFile f (ac.data ());
    if (!f.open (IO_ReadOnly))	//can't open file
        
    {
        qApp->setOverrideCursor (arrowCursor);
        int i = KMsgBox::yesNo (0, "Error", "You don't have an active groups list.\n get it from server?");
        qApp->restoreOverrideCursor ();
        if (1 == i)
        {
            if (actions (GET_ACTIVE))
                success = true;
            else
                success = false;
        }
    }
    else			// active file opens
    {
        f.close ();
        debug ("loading %s\n",ac.data());
        statusBar ()->changeItem ("Listing active newsgroups", 1);
        qApp->processEvents ();
        server->groupList (&groups,false);
    };
    statusBar ()->changeItem ("", 1);
    return success;
};

void Groupdlg::checkUnread()
{
    QString l,status,howmany,first,last,gname;
    KPath p;
    KTreeListItem *base=list->itemAt(0); //The "Subscribed Newsgroups" item.
    p.push(new QString(base->getText()));
    int c=base->childCount();
    for (int i=0;i<c;i++)
    {
        const char *it=base->childAt(i)->getText();
        gname=it;
        p.push(new QString(it));
        gname=gname.left(gname.find(' '));
        server->setGroup(gname.data());
        l=server->lastStatusResponse();

        l=gname+" [ BROKEN unread]";
        // Updating the test this way doesn't repaint properly,
        // so I have to do do the path hack.
        //        base->childAt(i)->setText(l.data());
        list->changeItem(l.data(),0,&p);
        delete p.pop();
    }
    list->repaint();
    
}

