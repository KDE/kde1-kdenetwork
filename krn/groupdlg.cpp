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

#include <unistd.h>

#include "groupdlg.h"

#define Inherited KTopLevelWidget

#include <qapp.h>
#include <qfile.h>
#include <qstring.h>
#include <qtstream.h>
#include <qevent.h>

#include <kapp.h>
#include <kmsgbox.h>
#include <Kconfig.h>
#include <kkeyconf.h>

#include "artdlg.h"
#include "identDlg.h"
#include "NNTPConfigDlg.h"
#include "rmbpop.h"
#include "aboutDlg.h"

#include "groupdlg.moc"

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
#define GET_SUBJECTS 14
#define GET_ARTICLES 15
#define CATCHUP 16

extern QString krnpath,cachepath,artinfopath,pixpath;
extern KConfig *conf;


GroupList groups;
GroupList subscr;

bool checkPixmap(KTreeListItem *item,void *)
{
    QString name(item->getText());
    if (name.right(1)==".") //it's a folder
    {
        item->setPixmap(&kapp->getIconLoader()->loadIcon("folder.xpm"));
        return false;
    }
    int i=groups.find(&NewsGroup(name));
    if (i!=-1)
    {
        if (groups.at(i)->isTagged)
        {
            item->setPixmap(&kapp->getIconLoader()->loadIcon("tagged.xpm"));
            return false;
        }
    }
    if (subscr.find(&NewsGroup(name))!=-1) //it's subscribed
    {
        item->setPixmap(&kapp->getIconLoader()->loadIcon("subscr.xpm"));
        return false;
    }
    item->setPixmap(&kapp->getIconLoader()->loadIcon("followup.xpm")); //it's plain
    return false;
}

Groupdlg::Groupdlg
    (
     const char *name
    )
    :
    Inherited (name)
{
    groups.setAutoDelete(false);
    subscr.setAutoDelete(false);

    QPopupMenu *file = new QPopupMenu;
    file->insertItem("Connect to Server",CONNECT);
    file->insertItem("Disconnect From Server",DISCONNECT);
    file->insertItem("Get Active File",GET_ACTIVE);
    file->insertSeparator();
    file->insertItem("Exit",EXIT);
    connect (file,SIGNAL(activated(int)),SLOT(currentActions(int)));

    setCaption ("KRN - Group List");

    QPopupMenu *subscribed = new QPopupMenu;
    subscribed->insertItem("Get Subjects",GET_SUBJECTS);
    subscribed->insertItem("Get Articles",GET_ARTICLES);
    subscribed->insertItem("Catchup",CATCHUP);
    connect (subscribed,SIGNAL(activated(int)),SLOT(subscrActions(int)));

    QPopupMenu *tagged = new QPopupMenu;
    tagged->insertItem("Get Subjects",GET_SUBJECTS);
    tagged->insertItem("Get Articles",GET_ARTICLES);
    tagged->insertItem("(un)Subscribe",SUBSCRIBE);
    tagged->insertItem("Untag",TAGGROUP);
    tagged->insertItem("Catchup",CATCHUP);
    connect (tagged,SIGNAL(activated(int)),SLOT(taggedActions(int)));

    QPopupMenu *newsgroup = new QPopupMenu;
    newsgroup->insertItem("Open",OPENGROUP);
    newsgroup->insertItem("(un)Subscribe",SUBSCRIBE);
    newsgroup->insertItem("(Un)Tag",TAGGROUP);
    newsgroup->insertItem("Catchup",CATCHUP);
    newsgroup->insertSeparator();
    newsgroup->insertItem ("&Subscribed", subscribed);
    newsgroup->insertItem ("&Tagged", tagged);
    
    connect (newsgroup,SIGNAL(activated(int)),SLOT(currentActions(int)));
    
    QPopupMenu *options = new QPopupMenu;
    options->insertItem("Identity",CHANGE_IDENTITY);
    options->insertItem("NNTP Options",CONFIG_NNTP);
    connect (options,SIGNAL(activated(int)),SLOT(currentActions(int)));

    QPopupMenu *help = new QPopupMenu;
    help->insertItem("Contents",HELP_CONTENTS);
    help->insertSeparator();
    help->insertItem("About",HELP_ABOUT);
    connect (help,SIGNAL(activated(int)),SLOT(currentActions(int)));

    KMenuBar *menu = new KMenuBar (this, "menu");
    
    menu->insertItem ("&File", file);
    menu->insertItem ("&Newsgroup", newsgroup);
    menu->insertItem ("&Options", options);
    menu->insertSeparator();
    menu->insertItem ("&Help", help);
    
    setMenu (menu);
    
    QPixmap pixmap;
    
    KToolBar *tool = new KToolBar (this, "tool");
    QObject::connect (tool, SIGNAL (clicked (int)), this, SLOT (currentActions (int)));
    
    pixmap=kapp->getIconLoader()->loadIcon("connected.xpm");
    tool->insertButton (pixmap, CONNECT, true, "Connect to server");

    pixmap=kapp->getIconLoader()->loadIcon("disconnected.xpm");
    tool->insertButton (pixmap, DISCONNECT, false, "Disconnect from server");
    tool->insertSeparator ();
    
    pixmap=kapp->getIconLoader()->loadIcon("previous.xpm");
    tool->insertButton (pixmap, GET_ACTIVE, true, "Get list of active groups");
    tool->insertSeparator ();
    
    tool->insertButton (kapp->getIconLoader()->loadIcon("subscr.xpm"), SUBSCRIBE, true, "(Un)Subscribe");
    addToolBar (tool);
    tool->insertSeparator ();
    
    pixmap=kapp->getIconLoader()->loadIcon("reload.xpm");
    tool->insertButton (pixmap, CHECK_UNREAD, true, "Check for Unread Articles");
    
    
    list = new KTreeList (this, "");
    QObject::connect (list, SIGNAL (selected (int)), this, SLOT (openGroup (int)));
    setView (list);
    RmbPop *filter=new RmbPop(list);
    delete (filter->pop);
    filter->pop=newsgroup;
    
    
    KStatusBar *status = new KStatusBar (this, "status");
    status->insertItem ("Received 00000000 bytes", 1);
    status->insertItem ("", 2);
    status->show ();
    setStatusBar (status);

    conf->setGroup("NNTP");
    QString sname=conf->readEntry("NNTPServer");
    server = new NNTP (sname.data());
    server->reportCounters (true,false);

    actions (LOAD_FILES);
    fillTree();
    connect (server,SIGNAL(newStatus(char *)),this,SLOT(updateCounter(char *)));

    conf->setGroup("NNTP");
    if (conf->readNumEntry("ConnectAtStart"))
        actions(CONNECT);

    readProperties();

    show();
    qApp->processEvents();
    //Open group windows
}

Groupdlg::~Groupdlg ()
{
    debug ("saving groupdlg's properties");
    saveProperties(false);
    QStrList openwin;
    //check for all open groups, and close them
    for (NewsGroup *g=groups.first();g!=0;g=groups.next())
    {
        if (g->isVisible)
        {
            debug ("%s is open, closing it",g->data());
            openwin.append (g->data());
            delete g->isVisible;
        }
    }
    conf->setGroup("ArticleListOptions");
    conf->writeEntry("OpenWindows",openwin);
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
            QListIterator <NewsGroup> it(groups);
            NewsGroup *iter;
            for (;it.current();++it)
            {
                iter=it.current();
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
                            list->addChildItem(iter->data(),&kapp->getIconLoader()->loadIcon("tagged.xpm"),index);
                        else
                            list->addChildItem(iter->data(),&kapp->getIconLoader()->loadIcon("followup.xpm"),index);
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
                                list->addChildItem(iter->data(),&kapp->getIconLoader()->loadIcon("tagged.xpm"),index);
                            else
                                list->addChildItem(iter->data(),&kapp->getIconLoader()->loadIcon("folder.xpm"),index);
                        }
                        nextdot[0]=tc;
                    }
                }
            }
            list->expandItem(index);
            list->forEveryVisibleItem(checkPixmap,NULL);
            list->repaint();
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




void Groupdlg::subscribe (NewsGroup *group)
{
    debug ("subscribing to %s",group->data());
    KPath path;
    int index=subscr.find (group);
    if (-1 != index)
    {
        subscr.remove ();
        path.push (new QString ("Subscribed Newsgroups."));
        path.push (new QString (group->data()));
        int l=list->currentItem();
        list->setCurrentItem(0);
        list->removeItem (&path);
        if (list->itemAt(0)->isExpanded() &&
            ((unsigned int)l>list->itemAt(0)->childCount()+1))
            list->setCurrentItem(l-1);
        else
            list->setCurrentItem(l);
    }
    else
    {
        if (-1 != groups.find (group))
        {
            list->addChildItem (group->data(), &kapp->getIconLoader()->loadIcon("subscr.xpm"), 0);
            subscr.append (group);
        if (list->itemAt(0)->isExpanded() &&
            ((unsigned int)list->currentItem()>list->itemAt(0)->childCount()+1))
            list->setCurrentItem(list->currentItem()+1);
        }
    };
    list->forEveryVisibleItem(checkPixmap,NULL);
    list->repaint();
    saveSubscribed();
}

void Groupdlg::tag (NewsGroup *group)
{
    if(group->isTagged)
        group->isTagged=false;
    else
        group->isTagged=true;
    list->forEveryVisibleItem(checkPixmap,NULL);
    list->repaint();
}


void Groupdlg::offline()
{
    toolBar()->setItemEnabled (DISCONNECT,false);
    server->disconnect();
    toolBar()->setItemEnabled (CONNECT,true);
    statusBar ()->changeItem ("Disconnected", 2);
    qApp->processEvents ();
}

void Groupdlg::online()
{
    toolBar()->setItemEnabled (CONNECT,false);
    statusBar ()->changeItem ("Connecting to server", 2);
    qApp->processEvents ();
    if (server->connect ())
    {
        if (server->isReadOnly ())
            statusBar ()->changeItem ("Connected to server - Posting not allowed", 2);
        else
            statusBar ()->changeItem ("Connected to server - Posting allowed", 2);
    }
    else
    {
        qApp->setOverrideCursor (arrowCursor);
        KMsgBox:: message (0, "Error", "Can't connect to server", KMsgBox::INFORMATION);
        qApp->restoreOverrideCursor ();
        statusBar ()->changeItem ("Connection to server failed", 2);
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
            statusBar ()->changeItem ("Connection to server failed: Authentication problem", 2);
            qApp->processEvents ();
            toolBar()->setItemEnabled (CONNECT,true);
        }
    }
    toolBar()->setItemEnabled (DISCONNECT,true);
}

void Groupdlg::fillTree ()
{
    list->insertItem ("Subscribed Newsgroups.", &kapp->getIconLoader()->loadIcon("folder.xpm"));
    QListIterator <NewsGroup> it(subscr);
    it.toFirst();
    NewsGroup *g;
    for (;it.current();++it)
    {
        g=it.current();
        debug ("adding %s",g->data());
        list->addChildItem (g->data(), &kapp->getIconLoader()->loadIcon("subscr.xpm"), 0);
    }

    list->insertItem ("All Newsgroups.", &kapp->getIconLoader()->loadIcon("folder.xpm"));
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


bool Groupdlg::actions (int action,NewsGroup *group=0)
{
    bool success=false;
    qApp->setOverrideCursor (waitCursor);
    switch (action)
    {
    case GET_SUBJECTS:
        {
            if (!group)
                break;
            getSubjects(group);
            break;
        }
    case GET_ARTICLES:
        {
            if (!group)
                break;
            getArticles(group);
            break;
        }
    case OPENGROUP:
        {
            openGroup (group->data());
            break;
        }
    case TAGGROUP:
        {
            if (!group)
                break;
            tag(group);
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
                conf->writeEntry("SMTPServer",dlg.smtpserver->text());
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
                statusBar ()->changeItem ("Getting active list from server", 2);
                qApp->processEvents ();

                server->resetCounters (true,true);
                server->reportCounters (true,false);
                server->groupList (&groups,true);
                server->resetCounters (true,true);
                server->reportCounters (false,false);
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
            if (!group)
                break;
            subscribe (group);
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
    case CATCHUP:
        {
            if (!group)
                break;
            group->catchup();
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
    subscr.clear();
    if (f.open (IO_ReadOnly))
    {
        QTextStream st(&f);
        while (1)
        {
            ac=st.readLine ();
            if (st.eof())
                break;
            subscr.append (new NewsGroup(ac.data()));
            debug ("subscr-->%s",ac.data());
        };
        f.close ();
        return true;
    }
    else
    {
        warning("Can't open subscribed file for reading!");
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
        QListIterator <NewsGroup>it(subscr);
        for (;it.current();++it)
        {
            f.writeBlock(it.current()->data(),it.current()->length());
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
        statusBar ()->changeItem ("Listing active newsgroups", 2);
        qApp->processEvents ();
        server->groupList (&groups,false);
    };
    statusBar ()->changeItem ("", 2);
    return success;
};

bool Groupdlg::currentActions(int action)
{
    bool success=true;
    //Lets try to find the newsgroup
    if (action==OPENGROUP)
    {
        openGroup(list->currentItem());
        return success;
    }
    else
    {
        int index=-1;
        NewsGroup *g=0;
        if (list->getCurrentItem())
        {
            const char *text = list->getCurrentItem ()->getText ();
            index=groups.find (&NewsGroup(text));
        }
        debug ("index-->%d",index);
        if (index!=-1)
            g=groups.at(index);
        actions(action,g);
    }
    return success;
}

bool Groupdlg::taggedActions(int action)
{
    int i=0;
    bool success=true;
    QListIterator <NewsGroup> it(groups);
    for (;it.current(); ++it)
    {
        i++;
        if (it.current()->isTagged)
        {
            debug ("acting");
            actions(action,it.current());
        }
    }
    debug ("groups-->%d",groups.count());
    return success;
}

bool Groupdlg::subscrActions(int action)
{
    bool success=true;
    QListIterator <NewsGroup> it(subscr);
    for (;it.current();++it)
    {
        debug ("doing action in group %s",it.current()->data());
        actions(action,it.current());
    }
    statusBar ()->changeItem ("Done", 2);
    return success;
}

void Groupdlg::getArticles(NewsGroup *group)
{
    if (needsConnect())
    {
        QString s;
        s="Getting messages in ";
        s+=group->data();
        statusBar ()->changeItem (s.data(), 2);
        qApp->processEvents();
        debug ("Getting messages in %s",group->data());
        group->getMessages(server);
    }
}

void Groupdlg::getSubjects(NewsGroup *group)
{
    if (needsConnect())
    {
        QString s;
        s="Getting list of messages in ";
        s+=group->data();
        statusBar ()->changeItem (s.data(), 2);
        qApp->processEvents();
        debug ("Getting subjects in %s",group->data());
        group->getSubjects(server);
    }
}

void Groupdlg::checkUnread()
{
    QString l,status,howmany,first,last,gname;
    KPath p;
    KTreeListItem *base=list->itemAt(0); //The "Subscribed Newsgroups" item.
    p.push(new QString(base->getText()));
    char countmessage[255];

    int c=base->childCount();
    for (int i=0;i<c;i++)
    {
        const char *it=base->childAt(i)->getText();
        gname=it;
        p.push(new QString(it));
        gname=gname.left(gname.find(' '));
	//        server->setGroup(gname.data());
	//        l=server->StatusResponse().c_str();
	//        debug ("----->%s",l.data());
	sprintf(countmessage, " [%d]", subscr.at(i)->countNew(server));

        l=gname+countmessage;
        // Updating the test this way doesn't repaint properly,
        // so I have to do do the path hack.
        //        base->childAt(i)->setText(l.data());
        list->changeItem(l.data(),0,&p);
        delete p.pop();
    }
    list->repaint();

    //    int groupcnt = subscr.count();

    //    for(int i=0; i < groupcnt-1; i++)
    //debug("%d", subscr.at(i)->countNew(server));
}


void Groupdlg::updateCounter(char *s)
{
    statusBar()->changeItem (s, 1);
    qApp->processEvents();
}

