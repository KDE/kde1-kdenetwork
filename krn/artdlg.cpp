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
#include "artdlg.h"
#define Inherited KTopLevelWidget

#include <qapp.h>
#include <qfile.h>
#include <qstring.h>
#include <qtstream.h>
#include <qfiledlg.h>

#include <kmsgbox.h>

#include <mimelib/mimepp.h>

#include "decoderDlg.h"
#include "kdecode.h"
#include "rmbpop.h"

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


extern KIconLoader *iconloader;

extern QString pixpath,cachepath;

extern ArticleDict artSpool;

extern KDecode *decoder;


Artdlg::Artdlg (NewsGroup *_group, NNTP* _server)
    :Inherited (_group->data())
{
    group=_group;
    group->isVisible=true;
    setCaption (group->data());
    groupname=group->data();
    unread = false;
    
    server = _server;
    
    
    article=new QPopupMenu;
    article->setCheckable(true);
    article->insertItem("Save",SAVE_ARTICLE);
    article->insertSeparator();
    article->insertItem("Reply by Mail",REP_MAIL);
    article->insertItem("Post Followup",FOLLOWUP);
    article->insertSeparator();
    article->insertItem("Decode",DECODE_ONE_ARTICLE);
    article->insertItem("Tag",TAG_ARTICLE);
    article->insertSeparator();
    article->insertItem("Only unread messages", NO_READ);
    connect (article,SIGNAL(activated(int)),SLOT(actions(int)));
    
    QPopupMenu *taggedArticle=new QPopupMenu;
    taggedArticle->insertItem("Save",SAVE_ARTICLE);
    taggedArticle->insertSeparator();
    taggedArticle->insertItem("Reply by Mail",REP_MAIL);
    taggedArticle->insertItem("Post Followup",FOLLOWUP);
    taggedArticle->insertSeparator();
    taggedArticle->insertItem("Decode",DECODE_ARTICLE);
    taggedArticle->insertItem("Untag",TAG_ARTICLE);
    connect (taggedArticle,SIGNAL(activated(int)),SLOT(taggedActions(int)));
    
    menu = new KMenuBar (this, "menu");
    menu->insertItem ("&Article", article);
    menu->insertItem ("&Tagged", taggedArticle);
    setMenu (menu);
    
    
    QPixmap pixmap;
    
    tool = new KToolBar (this, "tool");
    QObject::connect (tool, SIGNAL (clicked (int)), this, SLOT (actions (int)));
    
    
    pixmap.load(pixpath+"save.xpm");
    tool->insertItem(pixmap,SAVE_ARTICLE,true,"Save file");
    tool->insertSeparator ();
    
    //    pixmap=iconloader->loadIcon("filemail.xpm");
    pixmap.load(pixpath+"filemail.xpm");
    tool->insertItem (pixmap, REP_MAIL, true, "Reply by Mail");
    pixmap.load (pixpath+"txt.xpm");
    tool->insertItem (pixmap, FOLLOWUP, true, "Post a Followup");
    tool->insertSeparator ();
    
    //    pixmap=iconloader->loadIcon("left.xpm");
    pixmap.load(pixpath+"left.xpm");
    tool->insertItem (pixmap, PREV, true, "Previous Message");
    
    //    pixmap=iconloader->loadIcon ("right.xpm");
    pixmap.load(pixpath+"right.xpm");
    tool->insertItem (pixmap, NEXT, true, "Next Message");
    
    tool->insertSeparator ();
    //    pixmap=iconloader->loadIcon ("previous.xpm");
    pixmap.load(pixpath+"previous.xpm");
    tool->insertItem (pixmap, ARTLIST, true, "Get Article List");
    
    pixmap.load(pixpath+"tagged.xpm");
    tool->insertItem (pixmap, TAG_ARTICLE, true, "Tag Article");
    
    pixmap.load(pixpath+"deco.xpm");
    tool->insertItem (pixmap, DECODE_ONE_ARTICLE, true, "Decode Article");
    
    
    addToolBar (tool);
    tool->setPos( KToolBar::Top );
    tool->show();
    
    panner=new KPanner (this,"panner",KPanner::O_HORIZONTAL,33);
    panner->setSeparator(50);
    setView (panner);
    
    gl = new QGridLayout( panner->child0(), 1, 1 );
    list=new KTabListBox (panner->child0());
    list->setNumCols(11);
    list->setColumn(0, "",20,KTabListBox::PixmapColumn);
    list->setColumn(1, "",5);
    list->setColumn(2, "Sender", 150);
    list->setColumn(3, "",5);
    list->setColumn(4, "Lines", 50);
    list->setColumn(5, "",5);
    list->setColumn(6, "Score", 50);
    list->setColumn(7, "",5);
    list->setColumn(8, "Date", 50);
    list->setColumn(9, "",5);
    list->setColumn(10, "Subject", 50);
    
    list->dict().insert("N",new QPixmap(pixpath+"green-bullet.xpm"));  //Unread message
    list->dict().insert("R",new QPixmap(pixpath+"red-bullet.xpm"));    //Read message
    list->dict().insert("T",new QPixmap(pixpath+"black-bullet.xpm"));  //Unav. message
    list->dict().insert("M",new QPixmap(pixpath+"tagged.xpm"));  //Marked message
    
    gl->addWidget( list, 0, 0 );
    connect (list,SIGNAL(selected(int,int)),this,SLOT(loadArt(int,int)));
    
    RmbPop *filter=new RmbPop(list);
    delete (filter->pop);
    filter->pop=article;
    
    gl = new QGridLayout( panner->child1(), 1, 1 ); 
    messwin=new Kmessage(panner->child1(),"messwin");
    gl->addWidget( messwin, 0, 0 );
    
    RmbPop *filter2=new RmbPop(messwin);
    delete (filter2->pop);
    filter2->pop=article;
    
    
    status = new KStatusBar (this, "status");
    status->insertItem ("", 1);
    status->show ();
    setStatusBar (status);
    
    acc=new QAccel (this);
    acc->insertItem(Key_N,NEXT);
    acc->insertItem(Key_P,PREV);
    acc->insertItem(Key_T,TAG_ARTICLE);
    acc->insertItem(Key_R,REP_MAIL);
    acc->insertItem(Key_F,FOLLOWUP);
    acc->insertItem(Key_G,ARTLIST);
    acc->insertItem(Key_Space,PAGE_DOWN_ARTICLE);
    acc->insertItem(Key_Next,PAGE_DOWN_ARTICLE);
    acc->insertItem(Key_Backspace,PAGE_UP_ARTICLE);
    acc->insertItem(Key_Prior,PAGE_UP_ARTICLE);
    acc->insertItem(ALT + Key_Up, PREV);
    acc->insertItem(ALT + Key_Down, NEXT);
    acc->insertItem(Key_Up, SCROLL_DOWN_ARTICLE);
    acc->insertItem(Key_Down, SCROLL_UP_ARTICLE);
    
    QObject::connect (acc,SIGNAL(activated(int)),this,SLOT(actions(int)));
    QObject::connect (messwin,SIGNAL(spawnArticle(QString)),this,SLOT(loadArt(QString)));
    resize(600,400);
    qApp->processEvents ();
    show ();
    resize(600,400);
    qApp->processEvents ();
    
    if (server->isConnected())
    {
        actions(ARTLIST);
    }
    else
    {
        fillTree();
    }
}

void Artdlg::closeEvent(QCloseEvent *)
{
    debug ("close event");
    group->isVisible=false;
    group->artList.clear();
    artList.clear();
    delete this;
} 

Artdlg::~Artdlg ()
{
    debug ("destroying articles window");
}

void Artdlg::fillTree ()

{
    qApp->setOverrideCursor(waitCursor);
    statusBar()->changeItem("Reading Article List",1);
    qApp->processEvents ();
    group->getList();
    
    //Here is where the filtering should happen
    //Only put messages we want.
    
    statusBar()->changeItem("Showing Article List",1);
    qApp->processEvents ();
    list->setAutoUpdate(false);
    list->clear();
    
    for (Article *iter=group->artList.first();iter!=0;iter=group->artList.next())
    {
        QString formatted;
        
        artList.append(iter);
        iter->formHeader(&formatted);
        if (!(iter->isRead() && unread)) // We want to see only the unread messages
        {
            list->insertItem (formatted.data());
        }
    }
    
    list->setAutoUpdate(true);
    list->repaint();
    qApp->restoreOverrideCursor();
    statusBar()->changeItem("",1);
    qApp->processEvents ();
}

bool Artdlg::taggedActions (int action)
{
    bool success=false;
    qApp->setOverrideCursor (waitCursor);
    int c=0;
    for (Article *iter=artList.first();iter!=0;iter=artList.next())
    {
        if (iter->isMarked())
        {
            list->setCurrentItem(c);
            success=actions(action);
        }
        c++;
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
    bool success=false;
    qApp->setOverrideCursor (waitCursor);
    switch (action)
    {
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
                i=list->currentItem();
                list->setTopItem(i);
                loadArt(i,0);
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
                i=list->currentItem();
                list->setTopItem(i);
                loadArt(i,0);
            }
            success=true;
            break;
        }
    case PAGE_DOWN_ARTICLE:
        {
            messwin->pageUp();
            success=true;
            break;
        }
    case PAGE_UP_ARTICLE:
        {
            messwin->pageDown();
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
        }
    case TAG_ARTICLE:
        {
            markArt(list->currentItem(),0);
            break;
        }
    case FOLLOWUP:
    case REP_MAIL:
        KMsgBox::message (0,"Sorry!","Not implemented");
    case SCROLL_UP_ARTICLE:
        {
            messwin->scrollUp();
            success=true;
            break;
        }
    case SCROLL_DOWN_ARTICLE:
        {
            messwin->scrollDown();
            success=true;
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
            article->setItemChecked(NO_READ, unread);
            list->setAutoUpdate(false);
            list->clear();
            artList.clear();
            for (Article *iter=group->artList.first();iter!=NULL;iter=group->artList.next())
            {
                QString formatted;
                iter->formHeader(&formatted);
                if (!(iter->isRead() && unread))		// We want to see only read messages.
                {
                    artList.append(iter);
                    list->insertItem (formatted.data());
                }
            }
            list->setAutoUpdate(true);
            list->repaint();
            success = true;
            break;
        }
    }
    qApp->restoreOverrideCursor ();
    return success;
}

bool Artdlg::loadArt (QString id)
{
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
    qApp->setOverrideCursor (waitCursor);
    QString *s;
    s=server->article(id.data());
    if (s)
    {
        if (!s->isEmpty())
        {
            messwin->loadMessage(*s);
        }
        delete s;
    }
    else
    {
        Article *art;
        art=artSpool.find(id.data());
        if (art)
            art->setAvailable(false);
        s=new QString("\nError getting article.\nServer said:\n");
        s->append(server->lastStatusResponse());
        warning (s->data());
        messwin->loadMessage(*s);
        delete s;
        qApp->restoreOverrideCursor ();
        return false;
    }
    qApp->restoreOverrideCursor ();
    return true;
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
            debug ("saving article");
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
        s=new QString("\nError getting article.\nServer said:\n");
        s->append(server->lastStatusResponse());
        warning (s->data());
        messwin->loadMessage(*s);
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
}


void Artdlg::getSubjects()
{
    qApp->setOverrideCursor(waitCursor);
    statusBar ()->changeItem ("Getting Article List", 1);
    qApp->processEvents ();
    if (strcmp(server->group(),groupname))
    {
        server->setGroup(groupname);
    }
    if (server->last>group->lastArticle)
    {
        debug ("xover from %d to %d",group->lastArticle+1,server->last);
        server->artList(group->lastArticle,server->last);
        group->lastArticle=server->last;
        group->save();
    }
    statusBar ()->changeItem ("", 1);
    qApp->processEvents ();
    qApp->restoreOverrideCursor();
}
