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
#include <qfiledlg.h>
#include "artdlg.h"


#define Inherited KTopLevelWidget

#include <kapp.h>
#include <qfile.h>
#include <qstring.h>
#include <qtstream.h>
#include <qclipbrd.h>
#include <qregexp.h>

#include <kapp.h>
#include <kfm.h>

#include <kmsgbox.h>
#include <kkeyconf.h>
#include <html.h>
#include <kconfig.h>

#include <mimelib/mimepp.h>

#include "decoderDlg.h"
#include "kdecode.h"
#include "rmbpop.h"
#include "fontsDlg.h"
#include "findArtDlg.h"

#include "kmcomposewin.h"
#include "kmreaderwin.h"

#include "artdlg.moc"

#define REP_MAIL 1
#define FOLLOWUP 2
#define PREV 3
#define NEXT 4
#define ARTLIST 5
#define PAGE_DOWN_ARTICLE 6
#define PAGE_UP_ARTICLE 7
#define DECODE_ARTICLE 8
#define TAG_ARTICLE 9
#define SAVE_ARTICLE 10
#define SCROLL_UP_ARTICLE 11
#define SCROLL_DOWN_ARTICLE 12
#define DECODE_ONE_ARTICLE 13
#define NO_READ 14
#define PRINT_ARTICLE 15
#define CONFIG_FONTS 16
#define CATCHUP 17
#define TOGGLE_EXPIRE 18  // robert's cache stuff
#define NO_LOCKED 19
#define POSTANDMAIL 20
#define FORWARD 21
#define POST 22
#define FIND_ARTICLE 23
#define EXPUNGE 24
#define DOWNLOAD_ARTICLE 25
#define NO_CACHED 26

extern QString pixpath,cachepath;

extern ArticleDict artSpool;

extern KDecode *decoder;

extern KConfig *conf;

findArtDlg *FindDlg;

Artdlg::Artdlg (NewsGroup *_group, NNTP* _server)
    :Inherited (_group->name)
{
    group=0;
    server=0;
    FindDlg=new findArtDlg(0);
    connect (FindDlg,SIGNAL(FindThis(const char *,const char*)),
             this,SLOT(FindThis(const char *,const char*)));
    
    
    conf->setGroup("ArticleListOptions");
    unread=conf->readNumEntry("ShowOnlyUnread");
    showlocked=conf->readNumEntry("ShowLockedArticles");
    showcached=conf->readNumEntry("ShowCachedArticles");
    
    
    taggedArticle=new QPopupMenu;
    taggedArticle->insertItem(klocale->translate("Save"),SAVE_ARTICLE);
    taggedArticle->insertItem(klocale->translate("Download"),DOWNLOAD_ARTICLE);
    taggedArticle->insertSeparator();
    taggedArticle->insertItem(klocale->translate("Print"),PRINT_ARTICLE);
    taggedArticle->insertItem(klocale->translate("Decode"),DECODE_ARTICLE);
    taggedArticle->insertItem(klocale->translate("Untag"),TAG_ARTICLE);
    taggedArticle->insertItem(klocale->translate("Don't expire"), TOGGLE_EXPIRE);  // robert's cache stuff
    connect (taggedArticle,SIGNAL(activated(int)),SLOT(taggedActions(int)));
    
    
    article=new QPopupMenu;
    article->setCheckable(true);
    article->insertItem(klocale->translate("Save"),SAVE_ARTICLE);
    article->insertItem(klocale->translate("Download"),DOWNLOAD_ARTICLE);
    article->insertItem(klocale->translate("Find"),FIND_ARTICLE);
    article->insertSeparator();
    article->insertItem(klocale->translate("Print"),PRINT_ARTICLE);
    article->insertItem(klocale->translate("Post New Article"),POST);
    article->insertItem(klocale->translate("Reply by Mail"),REP_MAIL);
    article->insertItem(klocale->translate("Post Followup"),FOLLOWUP);
    article->insertItem(klocale->translate("Post & Reply"),POSTANDMAIL);
    article->insertItem(klocale->translate("Forward"),FORWARD);
    article->insertSeparator();
    article->insertItem(klocale->translate("Decode"),DECODE_ONE_ARTICLE);
    article->insertItem(klocale->translate("(Un)Tag"),TAG_ARTICLE);
    article->insertSeparator();
    article->insertItem(klocale->translate("Tagged"),taggedArticle);
    article->insertSeparator(); // robert
    article->insertItem(klocale->translate("Don't expire"), TOGGLE_EXPIRE);  // robert's cache stuff
    article->setItemChecked(TOGGLE_EXPIRE, false);
    connect (article,SIGNAL(activated(int)),SLOT(actions(int)));
    
    
    
    options=new QPopupMenu;
    options->setCheckable(true);
    options->insertItem(klocale->translate("Show Only Unread Messages"), NO_READ);
    options->setItemChecked(NO_READ,unread);
    options->insertItem(klocale->translate("Show Only Cached Messages"), NO_CACHED);
    options->setItemChecked(NO_CACHED,showcached);
    options->insertItem(klocale->translate("Show Locked Messages"), NO_LOCKED);
    options->setItemChecked(NO_LOCKED,showlocked);
    options->insertItem(klocale->translate("Expunge"), EXPUNGE);
    options->insertItem(klocale->translate("Appearance..."),CONFIG_FONTS);
    connect (options,SIGNAL(activated(int)),SLOT(actions(int)));
    
    menu = new KMenuBar (this, klocale->translate("menu"));
    menu->insertItem (klocale->translate("&Article"), article);
    menu->insertItem (klocale->translate("&Tagged"), taggedArticle);
    menu->insertItem (klocale->translate("&Options"), options);
    setMenu (menu);
    
    
    QPixmap pixmap;
    
    tool = new KToolBar (this, "tool");
    QObject::connect (tool, SIGNAL (clicked (int)), this, SLOT (actions (int)));
    
    pixmap=kapp->getIconLoader()->loadIcon("left.xpm");
    tool->insertButton (pixmap, PREV, true, klocale->translate("Previous Message"));
    
    pixmap=kapp->getIconLoader()->loadIcon("right.xpm");
    tool->insertButton (pixmap, NEXT, true, klocale->translate("Next Message"));
    
    tool->insertSeparator ();
    
    pixmap=kapp->getIconLoader()->loadIcon("save.xpm");
    tool->insertButton(pixmap,SAVE_ARTICLE,true,klocale->translate("Save Article"));
    
    pixmap=kapp->getIconLoader()->loadIcon("fileprint.xpm");
    tool->insertButton(pixmap,PRINT_ARTICLE,true,klocale->translate("Print Article"));

    pixmap=kapp->getIconLoader()->loadIcon("find.xpm");
    tool->insertButton(pixmap,FIND_ARTICLE,true,klocale->translate("Find Article"));
    tool->insertSeparator ();
    
    pixmap=kapp->getIconLoader()->loadIcon("filenew.xpm");
    tool->insertButton (pixmap, POST, true, klocale->translate("Post New Article"));
    
    pixmap=kapp->getIconLoader()->loadIcon("filemail.xpm");
    tool->insertButton (pixmap, REP_MAIL, true, klocale->translate("Reply by Mail"));
    
    pixmap=kapp->getIconLoader()->loadIcon("followup.xpm");
    tool->insertButton (pixmap, FOLLOWUP, true, klocale->translate("Post a Followup"));
    
    pixmap=kapp->getIconLoader()->loadIcon("mailpost.xpm");
    tool->insertButton (pixmap, POSTANDMAIL, true, klocale->translate("Post & Mail"));
    
    pixmap=kapp->getIconLoader()->loadIcon("fileforward.xpm");
    tool->insertButton (pixmap, FORWARD, true, klocale->translate("Forward"));
    
    tool->insertSeparator ();
    
    
    pixmap=kapp->getIconLoader()->loadIcon("previous.xpm");
    tool->insertButton (pixmap, ARTLIST, true, klocale->translate("Get Article List"));
    
    pixmap=kapp->getIconLoader()->loadIcon("tagged.xpm");
    tool->insertButton (pixmap, TAG_ARTICLE, true, klocale->translate("Tag Article"));
    
    pixmap=kapp->getIconLoader()->loadIcon("locked.xpm");
    tool->insertButton (pixmap, TOGGLE_EXPIRE, true, klocale->translate("Lock (keep in cache)"));
    
    pixmap=kapp->getIconLoader()->loadIcon("deco.xpm");
    tool->insertButton (pixmap, DECODE_ONE_ARTICLE, true, klocale->translate("Decode Article"));
    
    pixmap=kapp->getIconLoader()->loadIcon("catch.xpm");
    tool->insertButton (pixmap, CATCHUP, true, klocale->translate("Catchup"));
    
    addToolBar (tool);
    tool->setBarPos( KToolBar::Top );
    tool->show();
    
    if (conf->readNumEntry("VerticalSplit"))
        panner=new KPanner (this,"panner",KPanner::O_VERTICAL,33);
    else
        panner=new KPanner (this,"panner",KPanner::O_HORIZONTAL,33);
    panner->setSeparator(50);
    setView (panner);
    
    gl = new QGridLayout( panner->child0(), 1, 1 );
    list=new KTabListBox (panner->child0(),"",4);
    list->clearTableFlags(Tbl_hScrollBar);
    list->clearTableFlags(Tbl_autoHScrollBar);
    list->setTableFlags(Tbl_autoVScrollBar);
    list->setSeparator('\n');
    
    list->setColumn(0, klocale->translate("Sender"), 150);
    list->setColumn(1, klocale->translate("Date"), 75);
    list->setColumn(2, klocale->translate("Lines"), 50);
    list->setColumn(3, klocale->translate("Subject"), 50,KTabListBox::MixedColumn);
    
    list->dict().insert("N",new QPixmap(kapp->getIconLoader()->loadIcon("green-bullet.xpm")));  //Unread message
    list->dict().insert("R",new QPixmap(kapp->getIconLoader()->loadIcon("red-bullet.xpm")));    //Read message
    list->dict().insert("T",new QPixmap(kapp->getIconLoader()->loadIcon("black-bullet.xpm")));    //Read message
    list->dict().insert("M",new QPixmap(kapp->getIconLoader()->loadIcon("tagged.xpm")));    //Read message
    list->dict().insert("L",new QPixmap(kapp->getIconLoader()->loadIcon("locked.xpm")));    //Read message
    
    list->setTabWidth(25);
    
    gl->addWidget( list, 0, 0 );
    connect (list,SIGNAL(highlighted(int,int)),this,SLOT(loadArt(int,int)));
    connect (list,SIGNAL(midClick(int,int)),this,SLOT(markReadArt(int,int)));
    connect (list,SIGNAL(popupMenu(int,int)),this,SLOT(popupMenu(int,int)));
    
    RmbPop *filter=new RmbPop(list);
    delete (filter->pop);
    filter->pop=article;
    
    gl = new QGridLayout( panner->child1(), 1, 1 ); 
    messwin=new KMReaderWin(panner->child1(),"messwin");
    messwin->setMsg(0);
    gl->addWidget( messwin, 0, 0 );
    QObject::connect(messwin,SIGNAL(urlClicked(const char *,int)),this,SLOT(openURL(const char*)));
    
    RmbPop *filter2=new RmbPop(messwin);
    delete (filter2->pop);
    filter2->pop=article;
    
    
    status = new KStatusBar (this, "status");
    status->insertItem ("                 ", 1);
    status->insertItem ("", 2);
    status->show ();
    setStatusBar (status);
    
    
    acc=new QAccel (this);
    acc->insertItem(Key_N,NEXT);
    acc->insertItem(Key_P,PREV);
    acc->insertItem(Key_T,TAG_ARTICLE);
    acc->insertItem(Key_R,REP_MAIL);
    acc->insertItem(Key_F,FOLLOWUP);
    acc->insertItem(Key_G,ARTLIST);
    acc->insertItem(Key_C,CATCHUP);
    acc->insertItem(Key_Space,PAGE_DOWN_ARTICLE);
    acc->insertItem(Key_Next,PAGE_DOWN_ARTICLE);
    acc->insertItem(Key_Backspace,PAGE_UP_ARTICLE);
    acc->insertItem(Key_Prior,PAGE_UP_ARTICLE);
    acc->insertItem(ALT + Key_Up, PREV);
    acc->insertItem(ALT + Key_Down, NEXT);
    acc->insertItem(Key_Up, SCROLL_UP_ARTICLE);
    acc->insertItem(Key_Down, SCROLL_DOWN_ARTICLE);
    acc->insertItem(Key_X, EXPUNGE);
    acc->insertItem(Key_Slash, FIND_ARTICLE);
    acc->insertItem(Key_S, FIND_ARTICLE);
    acc->insertItem(CTRL+Key_F, FIND_ARTICLE);
    
    QObject::connect (acc,SIGNAL(activated(int)),this,SLOT(actions(int)));
    QObject::connect (messwin,SIGNAL(statusMsg(const char*)),this,SLOT(updateCounter(const char*)));
    conf->setGroup("Geometry");
    setGeometry(conf->readNumEntry("ArtX",100),
                conf->readNumEntry("ArtY",40),
                conf->readNumEntry("ArtW",400),
                conf->readNumEntry("ArtH",400));
    show();

    qApp->processEvents ();

    init(_group,_server);
}


void Artdlg::init (NewsGroup *_group, NNTP* _server)
{
    if (group) //make old group know I'm not showing him
        group->isVisible=0;
        
    group=_group;
    group->isVisible=this;
    
    setCaption (group->name);
    groupname=group->name;
    statusBar()->changeItem("Reading Article List",2);
    qApp->processEvents ();
    group->getList(this);

    if (server)
    {
        disconnect (server,SIGNAL(newStatus(const char *)),
                          this,SLOT(updateCounter(const char *)));
    }
    
    server = _server;
    QObject::connect (server,SIGNAL(newStatus(const char *)),
                      this,SLOT(updateCounter(const char *)));

    if (server->isConnected())
    {
        actions(ARTLIST);
    }
    else
    {
        fillTree();
    }
}

void Artdlg::copyText(bool)
{
}

void Artdlg::closeEvent(QCloseEvent *)
{
    group->artList.clear();
    artList.clear();
    group->isVisible=0;
    delete this;
} 

Artdlg::~Artdlg ()
{
    conf->setGroup("Geometry");
    conf->writeEntry("ArtX",x());
    conf->writeEntry("ArtY",y());
    conf->writeEntry("ArtW",width());
    conf->writeEntry("ArtH",height());
    conf->sync();
}

void Artdlg::fillTree ()

{
    //save current ID if there is one
    Article *currArt;
    currArt=0;
    int curr=list->currentItem();
    if (curr>-1)
        currArt=artList.at(curr);
    
    qApp->setOverrideCursor(waitCursor);
    statusBar()->changeItem("Reading Article List",2);
    qApp->processEvents ();

    list->setAutoUpdate(false);
    list->clear();
    artList.clear();
    
    Article *iter;
    bool thiscached;
    for (iter=group->artList.first();iter!=0;iter=group->artList.next())
    {
        thiscached=server->isCached(iter->ID.data());
        if(
           (((thiscached && showcached) || (!showcached)) &&
           (!(unread && iter->isRead()))) ||
            (showlocked && (!iter->canExpire())))
        {
            artList.append(iter);
        }
    }
    
    statusBar()->changeItem(klocale->translate("Threading..."),2);
    qApp->processEvents ();
    artList.thread(true);
    
    //had to split this in two loops because the order of articles is not
    //the same in both article lists
    
    statusBar()->changeItem(klocale->translate("Showing Article List"),2);
    qApp->processEvents ();
    int i=0;
    for (iter=artList.first();iter!=0;iter=artList.next(),i++)
    {
        QString formatted;
        iter->formHeader(&formatted);
        list->insertItem (formatted.data());
        if (server->isCached(iter->ID.data()))
            list->changeItemColor(QColor(0,0,255),i);
    }
    
    //restore current message
    if (curr>-1)
    {
        list->setCurrentItem(artList.find(currArt));
        list->setTopItem(artList.find(currArt));
    }
    
    
    list->setAutoUpdate(true);
    list->repaint();
    qApp->restoreOverrideCursor();
    statusBar()->changeItem("",2);
    QString s;
    s.sprintf ("%d/%d",list->currentItem()+1,artList.count());
    statusBar()->changeItem(s.data(),1);
    qApp->processEvents ();
}

bool Artdlg::taggedActions (int action)
{
    bool success=false;
    qApp->setOverrideCursor (waitCursor);
    int c=0;

    if (action!=PRINT_ARTICLE)
    {
        disconnect (list,SIGNAL(highlighted(int,int)),this,SLOT(loadArt(int,int)));
    }
    
    for (Article *iter=artList.first();iter!=0;iter=artList.next())
    {
        if (iter->isMarked())
        {
            list->setCurrentItem(c);
            success=actions(action);
        }
        c++;
    }
    
    if (action!=PRINT_ARTICLE)
    {
        connect (list,SIGNAL(highlighted(int,int)),this,SLOT(loadArt(int,int)));
    }
    
    qApp->restoreOverrideCursor ();
    switch (action)
    {
    case DECODE_ARTICLE:
        decoder->showWindow();
    }
    return success;
}

bool Artdlg::actions (int action)
{
    setEnabled (false);
    acc->setEnabled(false);
    list->setEnabled(false);
    messwin->setEnabled(false);
    bool success=false;
    qApp->setOverrideCursor (waitCursor);
    switch (action)
    {
    case CONFIG_FONTS:
        {
            qApp->setOverrideCursor (arrowCursor);
            fontsDlg dlg;
            if(dlg.exec()==1)
            {
                messwin->readConfig();
            }
            qApp->restoreOverrideCursor ();
            break;
        }
    case PRINT_ARTICLE:
        {
            qApp->setOverrideCursor (arrowCursor);
            messwin->printMsg();
            qApp->restoreOverrideCursor ();
            break;
        }
    case ARTLIST:
        {
            emit needConnection();
            if (server->isConnected())
            {
                getSubjects();
                fillTree();
                success=true;
            }
            break;
        }
    case PREV:
        {
            int i=list->currentItem();
            if (i>0)
            {
                i--;
                list->setCurrentItem(i);
            }
            success=true;
            break;
        }
    case NEXT:
        {
            unsigned int i=list->currentItem();
            if (i<list->count())
            {
                i++;
                list->setCurrentItem(i);
            }
            success=true;
            break;
        }
    case DECODE_ARTICLE:
        {
            decArt(list->currentItem(),0);
            break;
        }
    case DECODE_ONE_ARTICLE:
        {
            actions(DECODE_ARTICLE);
            decoder->showWindow();
            break;
        }
    case TAG_ARTICLE:
        {
            markArt(list->currentItem(),0);
            break;
        }
    case SAVE_ARTICLE:
        {
            int index=list->currentItem();
            if (index<0)
                break;
            Article *art=artList.at(index);
            saveArt(art->ID);
            break;
        }
    case NO_READ:
        {
            unread = !unread;
            conf->setGroup("ArticleListOptions");
            conf->writeEntry("ShowOnlyUnread",unread);
            conf->sync();
            options->setItemChecked(NO_READ, unread);
            if (unread==false)
                group->getList(this);
            fillTree();
            success = true;
            break;
        }
    case NO_CACHED:
        {
            showcached = !showcached;
            conf->setGroup("ArticleListOptions");
            conf->writeEntry("ShowCachedArticles",showcached);
            conf->sync();
            options->setItemChecked(NO_CACHED, showcached);
            fillTree();
            success = true;
            break;
        }
    case NO_LOCKED:
        {
            showlocked = !showlocked;
            conf->setGroup("ArticleListOptions");
            conf->writeEntry("ShowLockedArticles",showlocked);
            conf->sync();
            options->setItemChecked(NO_LOCKED, showlocked);
            fillTree();
            success = true;
            break;
        }
    case PAGE_UP_ARTICLE:
        {
            messwin->slotScrollPrior();
            break;
        }
    case SCROLL_UP_ARTICLE:
        {
            messwin->slotScrollUp();
            break;
        }
    case PAGE_DOWN_ARTICLE:
        {
            messwin->slotScrollNext();
            break;
        }
    case SCROLL_DOWN_ARTICLE:
        
        {
            messwin->slotScrollDown();
            break;
        }
    case POST:
        {
            int mShowHeaders = 0xe0;
            conf->setGroup("Composer");
            conf->writeEntry("headers",mShowHeaders);

            KMMessage *m=new KMMessage();
            m->initHeader();
            m->setGroups(groupname);

            KMComposeWin *comp=new KMComposeWin(m);
            comp->show();
            break;
        }
    case FOLLOWUP:
        {
            int index = list->currentItem();
            
            if(index < 0)
                break;
            
            int mShowHeaders = 0xe0;
            conf->setGroup("Composer");
            conf->writeEntry("headers",mShowHeaders);

            Article *art=artList.at(index);
            KMMessage *mm=new KMMessage();
            QString *ts=server->article(art->ID.data());
            mm->fromString(ts->data());
            delete ts;
            KMMessage *m=mm->createReply(true);
//            m->initHeader();
            debug ("id1-->%s<--id2-->%s<--",
                   m->id().data(),
                   mm->id().data());
            QString refs=mm->references();
            refs+=" ";
            refs+=mm->id();
            m->setReferences(refs);
            m->setGroups(mm->groups());
            m->setTo("");
            delete mm;
            
            KMComposeWin *comp=new KMComposeWin(m);
            comp->show();
            break;
        }
    case REP_MAIL:
        {
            int index = list->currentItem();
            
            if(index < 0)
                break;
            
            int mShowHeaders  = 0x2c;
            conf->setGroup("Composer");
            conf->writeEntry("headers",mShowHeaders);

            Article *art=artList.at(index);
            KMMessage *m=new KMMessage();
            QString *ts=server->article(art->ID.data());
            m->fromString(ts->data());
            delete ts;
            KMMessage *mm=m->createReply();
            m->initHeader();
            mm->setGroups("");
            
            KMComposeWin *comp=new KMComposeWin(mm);
            comp->show();
            break;
        }
    case FORWARD:
        {
            int index = list->currentItem();
            
            if(index < 0)
                break;

            int mShowHeaders = 0x2c;
            conf->setGroup("Composer");
            conf->writeEntry("headers",mShowHeaders);
            
            Article *art=artList.at(index);
            KMMessage *m=new KMMessage();
            QString *ts=server->article(art->ID.data());
            m->fromString(ts->data());
            delete ts;
            
            KMMessage *mm=m->createForward();
            m->initHeader();
            mm->setGroups("");
            
            KMComposeWin *comp=new KMComposeWin(mm);
            comp->show();
            break;
        }
    case POSTANDMAIL:
        {
            int index = list->currentItem();
            
            if(index < 0)
                break;
            Article *art=artList.at(index);
            
            int mShowHeaders=0x6c;
            conf->setGroup("Composer");
            conf->writeEntry("headers",mShowHeaders);

            KMMessage *mm=new KMMessage();
            QString *ts=server->article(art->ID.data());
            mm->fromString(ts->data());
            delete ts;
            KMMessage *m=mm->createReply(true);
            QString refs=mm->references();
            refs+=" ";
            refs+=mm->id();
            m->setReferences(refs);
            m->setGroups(mm->groups());
            delete mm;
            
            KMComposeWin *comp=new KMComposeWin(m);
            comp->show();
            break;
            
        }
        
    case CATCHUP:
        {
            QListIterator <Article> iter(artList);
            for (;iter.current();++iter)
            {
                if (!iter.current()->isRead())
                {
                    iter.current()->setRead();
                }
            }
            fillTree();
            break;
        }     
    case EXPUNGE:
        {
            unread=false;
            actions(NO_READ);
            break;
        }     
        
        //
        // robert's cache stuff
        
    case TOGGLE_EXPIRE:
        {
            int index = list->currentItem();
            
            if(index < 0)
                break;
            
            Article *art=artList.at(index);
            
            if(art->canExpire()) {
                article->setItemChecked(TOGGLE_EXPIRE, true);
                art->toggleExpire();
            } else {
                article->setItemChecked(TOGGLE_EXPIRE, false);
                art->toggleExpire();
            }
            QString formatted;
            art->formHeader(&formatted);
            list->changeItem (formatted.data(),index);
            if (server->isCached(art->ID.data()))
                list->changeItemColor(QColor(0,0,255),index);
            
            break;
        }
    case FIND_ARTICLE:
        {
            FindDlg->show();
            break;
        }
    case DOWNLOAD_ARTICLE:
        {
            int index = list->currentItem();
            if(index < 0)
                break;
            Article *art=artList.at(index);
            QString id=art->ID;
            if (!server->isConnected())
            {
                if (!server->isCached(id.data()))
                {
                    emit needConnection();
                    if (!server->isConnected())
                    {
                        qApp->restoreOverrideCursor ();
                        return false;
                    }
                }
            }
            server->article(id.data());
            list->changeItemColor(QColor(0,0,255),index);
        }
    }
    qApp->restoreOverrideCursor ();
    setEnabled (true);
    acc->setEnabled(true);
    list->setEnabled(true);
    messwin->setEnabled(true);
    return success;
}

bool Artdlg::loadArt (QString id)
{
    disconnect (list,SIGNAL(highlighted(int,int)),this,SLOT(loadArt(int,int)));
    setEnabled (false);
    acc->setEnabled(false);
    list->setEnabled(false);
    messwin->setEnabled(false);
    qApp->setOverrideCursor (waitCursor);

    int i=list->currentItem();

    if (artList.at(i)->ID!=id)
    {
        int index=0;
        QListIterator <Article> iter(artList);
        for (;iter.current();++iter,++index)
        {
            if (iter.current()->ID==id)
            {
                list->setCurrentItem(index);
                qApp->restoreOverrideCursor ();
                setEnabled (true);
                acc->setEnabled(true);
                list->setEnabled(true);
                messwin->setEnabled(true);
                QString s;
                s.sprintf ("%d/%d",list->currentItem()+1,artList.count());
                statusBar()->changeItem(s.data(),1);
                connect (list,SIGNAL(highlighted(int,int)),this,SLOT(loadArt(int,int)));
                return true;
                break;
            }
        }
    }
    
    if (artList.at(i)->ID==id)
    {
        goTo(i);
    }
    
    if (!server->isConnected())
    {
        if (!server->isCached(id.data()))
        {
            emit needConnection();
            if (!server->isConnected())
            {
                qApp->restoreOverrideCursor ();
                connect (list,SIGNAL(highlighted(int,int)),this,SLOT(loadArt(int,int)));
                return false;
            }
        }
    }
    QString *s;
    s=server->article(id.data());
    if (s->isEmpty())
    {
        debug ("entered get from web");
        QString buffer(2048);
        QString urldata("http://ww2.altavista.digital.com/cgi-bin/news.cgi?id@");
        id=id.mid(1,id.length()-2);
        //    KURL::encodeURL(id);
        urldata+=id;
        KURL url(urldata.data());
        buffer.sprintf("From: KRN\n"
                       "To: You\n"
                       "Date: now\n"
                       "Subject: Problem getting article\n"
                       "ID: <00@00>\n"
                       "\n"
                       "This article seems to have expired or be missing from both"
                       "your news server and Krn's local cache\n"
                       "However, if you have a functional Internet connection, you may"
                       "be able to find it at Altavista following this link:\n"
                       "%s\n\n\n",url.url().data());

        //Now, lets create a phony article with this data.
        KMMessage *m=new KMMessage();
        m->fromString(qstrdup(buffer));
        messwin->setMsg(m);
        debug ("exited get from web");
    }
    else
    {
        KMMessage *m=new KMMessage();
        m->fromString(QString(s->data()));
        messwin->setMsg(m);
    }
    delete s;

    qApp->restoreOverrideCursor ();
    setEnabled (true);
    acc->setEnabled(true);
    list->setEnabled(true);
    messwin->setEnabled(true);
    QString sb;
    sb.sprintf ("%d/%d",list->currentItem()+1,artList.count());
    statusBar()->changeItem(sb.data(),1);
    connect (list,SIGNAL(highlighted(int,int)),this,SLOT(loadArt(int,int)));
    return true;
}

void Artdlg::goTo (int i)
{
    if ((list->lastRowVisible()-(list->height()/list->cellHeight(i)))
        >(i-2))
    {
        list->setTopItem(i);
    }
    else
    {
        if (list->lastRowVisible()<(i+1))
        {
            list->setTopItem(i+2-(list->height()/list->cellHeight(i)));
        }
    }
}


void Artdlg::saveArt (QString id)
{
    if (!server->isConnected())
    {
        if (!server->isCached(id.data()))
        {
            emit needConnection();
            if (!server->isConnected())
            {
                qApp->restoreOverrideCursor ();
                return;
            }
        }
    }
    qApp->setOverrideCursor (waitCursor);
    QString *s;
    s=server->article(id.data());
    if (s)
    {
        if (!s->isEmpty())
        {
            qApp->setOverrideCursor (arrowCursor);
            QString f=QFileDialog::getSaveFileName(0,"*",this);
            qApp->restoreOverrideCursor ();
            if (!f.isEmpty())
            {
                QFile fi(f);
                if (fi.open(IO_WriteOnly))
                {
                    fi.writeBlock(s->data(),s->length());
                    fi.close();
                }
                else
                {
                    warning ("Can't open file for writing");
                }
            }
        }
        delete s;
    }
    else
    {
        Article *art;
        art=artSpool.find(id.data());
        if (art)
            art->setAvailable(false);
        s=new QString(klocale->translate("\nError getting article.\nServer said:\n"));
        s->append(server->lastStatusResponse());
        KMMessage *m=new KMMessage();
        m->fromString(s->data());
        messwin->setMsg(m);
        delete s;
        qApp->restoreOverrideCursor ();
        return;
    }
    qApp->restoreOverrideCursor ();
    return;
}


//column is useless right now.
void Artdlg::loadArt (int index,int)
{
    if (index<0) return;
    Article *art=artList.at(index);
    if (loadArt(art->ID))
    {
        art->setRead(true);
        QString formatted;
        art->formHeader(&formatted);
        list->changeItem (formatted.data(),index);
        if (server->isCached(art->ID.data()))
            list->changeItemColor(QColor(0,0,255),index);
        
        article->setItemChecked(TOGGLE_EXPIRE, !art->canExpire());  // robert's cache stuff
    }
}

void Artdlg::markArt (int index,int)
{
    if (index<0) return;
    Article *art=artList.at(index);
    if (art->isMarked())
    {
        art->setMarked(false);
    }
    else
    {
        art->setMarked(true);
    }
    QString formatted;
    art->formHeader(&formatted);
    list->changeItem (formatted.data(),index);
    if (server->isCached(art->ID.data()))
        list->changeItemColor(QColor(0,0,255),index);
}

void Artdlg::decArt (int index,int)
{
    if (index<0) return;
    QString *s;
    Article *art=artList.at(index);
    
    if (!server->isConnected())
    {
        if (!server->isCached(art->ID.data()))
        {
            emit needConnection();
            if (!server->isConnected())
            {
                qApp->restoreOverrideCursor ();
                return;
            }
        }
    }
    
    art->setRead(true);
    s=server->article(art->ID.data());
    if (s)
    {
        if (!s->isEmpty())
        {
            QString p;
            p=cachepath+"/"+art->ID;
            decoder->load (p.data());
            art->setAvailable(true);
        }
        delete s;
    }
    else
    {
        art->setAvailable(false);
    }
    QString formatted;
    art->formHeader(&formatted);
    list->changeItem (formatted.data(),index);
    if (server->isCached(art->ID.data()))
        list->changeItemColor(QColor(0,0,255),index);
}


void Artdlg::getSubjects()
{
    qApp->setOverrideCursor(waitCursor);
    statusBar ()->changeItem (klocale->translate("Getting Article List"), 2);
    qApp->processEvents ();
    
    group->getSubjects(server);
    
    statusBar ()->changeItem ("", 2);
    qApp->processEvents ();
    qApp->restoreOverrideCursor();
}
void Artdlg::updateCounter(const char *s)
{
    statusBar()->changeItem (s, 2);
    qApp->processEvents();
}

void Artdlg::popupMenu(int index,int)
{
    markArt(index,0);
}


void Artdlg::FindThis (const char *expr,const char *field)
{
    static int lastfound=-1;
    static QString lastexpr="";
    static QString lastfield="";

    bool sameQuery=false;
    
    QRegExp regex(expr,false);
    QListIterator <Article> iter(artList);
    
    int index=list->currentItem();
    if (index>0)
    {
        iter+=index;
        ++iter;
        ++index;
    }
    else
    {
        index=0;
    }

    sameQuery=(lastexpr==expr) && (lastfield==field);
    if (sameQuery)
    {
        index=lastfound+1;
        iter.toFirst();
        iter+=index;
    }

    lastexpr=expr;
    lastfield=field;
    
    
    if (!strcmp(field,"Subject"))
    {
        for (;iter.current();++iter,++index)
        {
            if (regex.match(iter.current()->Subject.data())>-1)
            {
                list->changeItemColor(QColor(0,0,0),lastfound);
                list->changeItemColor(QColor(255,0,0),index);
                goTo(index);
                lastfound=index;
                break;
            }
        }
        return;
    }
    if (!strcmp(field,"Sender"))
    {
        for (;iter.current();++iter,++index)
        {
            if (regex.match(iter.current()->From.data())>-1)
            {
                list->changeItemColor(QColor(0,0,0),lastfound);
                list->changeItemColor(QColor(255,0,0),index);
                goTo(index);
                lastfound=index;
                break;
            }
        }
        return;
    }
}
void Artdlg::markReadArt (int index,int)
{
    if (index<0) return;
    Article *art=artList.at(index);
    if (art->isRead())
    {
        art->setRead(false);
    }
    else
    {
        art->setRead(true);
    }
    QString formatted;
    art->formHeader(&formatted);
    list->changeItem (formatted.data(),index);
    if (server->isCached(art->ID.data()))
        list->changeItemColor(QColor(0,0,255),index);
}

void Artdlg::openURL (const char *s)
{
    KURL url(s);
    if( url.isMalformed() )
    {
        warning("Invalid URL clicked!");
        return;
    };
    if(strcmp(url.protocol(),"news")==0)
    {
        if(strchr(url.path(),'@')!=NULL)
        {
            QString s=url.path();
            s="<"+s.right(s.length()-1)+">";
            loadArt(s);
        }
        else emit spawnGroup(url.path());
        return;
    }
    if(strcmp(url.protocol(),"http")==0)
    {
        KFM fm;
        fm.openURL(s);
        return;
    }
    if(strcmp(url.protocol(),"ftp")==0)
    {
        KFM fm;
        fm.openURL(s);
        return;
    }
    if(strcmp(url.protocol(),"mailto")==0)
    {
        QString address(url.path());
        KMMessage *m=new KMMessage();
        QString buffer(2048);
        buffer.sprintf ("To: %s\n\n\n",address.data());
        m->fromString (buffer);
        KMComposeWin *comp=new KMComposeWin(m);
        comp->show();
    }
}