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

#include <kapp.h>
#include <qfile.h>
#include <qstring.h>
#include <qtstream.h>
#include <qfiledlg.h>
#include <qclipbrd.h>

#include <kapp.h>

#include <kmsgbox.h>
#include <kkeyconf.h>
#include <html.h>
#include <kconfig.h>

#include <mimelib/mimepp.h>

#include "decoderDlg.h"
#include "kdecode.h"
#include "rmbpop.h"
#include "fontsDlg.h"

#include "kmcomposewin.h"

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

extern QString pixpath,cachepath;

extern ArticleDict artSpool;

extern KDecode *decoder;

extern KConfig *conf;

Artdlg::Artdlg (NewsGroup *_group, NNTP* _server)
    :Inherited (_group->data())
{
    group=_group;
    group->isVisible=this;
    setCaption (group->data());
    groupname=group->data();

    conf->setGroup("ArticleListOptions");
    unread=conf->readNumEntry("ShowOnlyUnread");
    showlocked=conf->readNumEntry("ShowLockedArticles");
    
    server = _server;
    
    taggedArticle=new QPopupMenu;
    taggedArticle->insertItem("Save",SAVE_ARTICLE);
    taggedArticle->insertSeparator();
    taggedArticle->insertItem("Decode",DECODE_ARTICLE);
    taggedArticle->insertItem("Untag",TAG_ARTICLE);
    connect (taggedArticle,SIGNAL(activated(int)),SLOT(taggedActions(int)));


    article=new QPopupMenu;
    article->setCheckable(true);
    article->insertItem("Save",SAVE_ARTICLE);
    article->insertSeparator();
    article->insertItem("Print",PRINT_ARTICLE);
    article->insertItem("Post New Article",POST);
    article->insertItem("Reply by Mail",REP_MAIL);
    article->insertItem("Post Followup",FOLLOWUP);
    article->insertItem("Post & Reply",POSTANDMAIL);
    article->insertItem("Forward",FORWARD);
    article->insertSeparator();
    article->insertItem("Decode",DECODE_ONE_ARTICLE);
    article->insertItem("(Un)Tag",TAG_ARTICLE);
    article->insertSeparator();
    article->insertItem("Tagged",taggedArticle);
    article->insertSeparator(); // robert
    article->insertItem("Don't expire", TOGGLE_EXPIRE);  // robert's cache stuff
    article->setItemChecked(TOGGLE_EXPIRE, false);
    connect (article,SIGNAL(activated(int)),SLOT(actions(int)));

    

    options=new QPopupMenu;
    options->setCheckable(true);
    options->insertItem("Show Only Unread Messages", NO_READ);
    options->setItemChecked(NO_READ,unread);
    options->insertItem("Show Locked Messages", NO_LOCKED);
    options->setItemChecked(NO_LOCKED,showlocked);
    options->insertItem("Appearance",CONFIG_FONTS);
    connect (options,SIGNAL(activated(int)),SLOT(actions(int)));
    
    menu = new KMenuBar (this, "menu");
    menu->insertItem ("&Article", article);
    menu->insertItem ("&Tagged", taggedArticle);
    menu->insertItem ("&Options", options);
    setMenu (menu);
    
    
    QPixmap pixmap;
    
    tool = new KToolBar (this, "tool");
    QObject::connect (tool, SIGNAL (clicked (int)), this, SLOT (actions (int)));
    
    pixmap=kapp->getIconLoader()->loadIcon("left.xpm");
    tool->insertButton (pixmap, PREV, true, "Previous Message");
    
    pixmap=kapp->getIconLoader()->loadIcon("right.xpm");
    tool->insertButton (pixmap, NEXT, true, "Next Message");
    
    tool->insertSeparator ();
    
    pixmap=kapp->getIconLoader()->loadIcon("save.xpm");
    tool->insertButton(pixmap,SAVE_ARTICLE,true,"Save Article");
    
    pixmap=kapp->getIconLoader()->loadIcon("fileprint.xpm");
    tool->insertButton(pixmap,PRINT_ARTICLE,true,"Print Article");
    tool->insertSeparator ();
    
    pixmap=kapp->getIconLoader()->loadIcon("filenew.xpm");
    tool->insertButton (pixmap, POST, true, "Post New Article");

    pixmap=kapp->getIconLoader()->loadIcon("filemail.xpm");
    tool->insertButton (pixmap, REP_MAIL, true, "Reply by Mail");

    pixmap=kapp->getIconLoader()->loadIcon("followup.xpm");
    tool->insertButton (pixmap, FOLLOWUP, true, "Post a Followup");

    pixmap=kapp->getIconLoader()->loadIcon("mailpost.xpm");
    tool->insertButton (pixmap, POSTANDMAIL, true, "Post & Mail");

    pixmap=kapp->getIconLoader()->loadIcon("fileforward.xpm");
    tool->insertButton (pixmap, FORWARD, true, "Forward");

    tool->insertSeparator ();
    

    pixmap=kapp->getIconLoader()->loadIcon("previous.xpm");
    tool->insertButton (pixmap, ARTLIST, true, "Get Article List");
    
    pixmap=kapp->getIconLoader()->loadIcon("tagged.xpm");
    tool->insertButton (pixmap, TAG_ARTICLE, true, "Tag Article");
    
    pixmap=kapp->getIconLoader()->loadIcon("locked.xpm");
    tool->insertButton (pixmap, TOGGLE_EXPIRE, true, "Lock (keep in cache)");

    pixmap=kapp->getIconLoader()->loadIcon("deco.xpm");
    tool->insertButton (pixmap, DECODE_ONE_ARTICLE, true, "Decode Article");

    pixmap=kapp->getIconLoader()->loadIcon("catch.xpm");
    tool->insertButton (pixmap, CATCHUP, true, "Catchup");
    
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
    list=new KTabListBox (panner->child0());
    list->clearTableFlags(Tbl_hScrollBar);
    list->clearTableFlags(Tbl_autoHScrollBar);
    list->setTableFlags(Tbl_autoVScrollBar);
    list->setSeparator('\n');
    list->setNumCols(4);
    list->setColumn(0, "Sender", 150);
    list->setColumn(1, "Date", 75);
    list->setColumn(2, "Lines", 50);
    list->setColumn(3, "Subject", 50,KTabListBox::MixedColumn);
    
    list->dict().insert("N",new QPixmap(kapp->getIconLoader()->loadIcon("green-bullet.xpm")));  //Unread message
    list->dict().insert("R",new QPixmap(kapp->getIconLoader()->loadIcon("red-bullet.xpm")));    //Read message
    list->dict().insert("T",new QPixmap(kapp->getIconLoader()->loadIcon("black-bullet.xpm")));    //Read message
    list->dict().insert("M",new QPixmap(kapp->getIconLoader()->loadIcon("tagged.xpm")));    //Read message
    list->dict().insert("L",new QPixmap(kapp->getIconLoader()->loadIcon("locked.xpm")));    //Read message

    list->setTabWidth(25);
    
    gl->addWidget( list, 0, 0 );
    connect (list,SIGNAL(selected(int,int)),this,SLOT(loadArt(int,int)));
    
    RmbPop *filter=new RmbPop(list);
    delete (filter->pop);
    filter->pop=article;
    
    gl = new QGridLayout( panner->child1(), 1, 1 ); 
    messwin=new Kmessage(panner->child1(),"messwin");
    gl->addWidget( messwin, 0, 0 );
    QObject::connect(messwin,SIGNAL(textSelected(bool)),this,SLOT(copyText(bool)));
    
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
    acc->insertItem(Key_Up, SCROLL_UP_ARTICLE);
    acc->insertItem(Key_Down, SCROLL_DOWN_ARTICLE);
        
    QObject::connect (acc,SIGNAL(activated(int)),this,SLOT(actions(int)));
    QObject::connect (messwin,SIGNAL(spawnArticle(QString)),this,SLOT(loadArt(QString)));
    readProperties();
    show();
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

void Artdlg::copyText(bool b)
{
    debug ("copyText");
    if (b)
    {
        //this is copied from kdehelp
        QString text;
        messwin->getKHTMLWidget()->getSelectedText( text );
        debug ("copying %s",text.data());
        QClipboard *cb = kapp->clipboard();
        cb->setText( text ); 
    }
}

void Artdlg::closeEvent(QCloseEvent *)
{
    debug ("close event");
    group->artList.clear();
    artList.clear();
    group->isVisible=0;
    delete this;
} 

Artdlg::~Artdlg ()
{
    debug ("destroying articles window");
    saveProperties (false);
    conf->sync();
}

void Artdlg::fillTree ()

{
    qApp->setOverrideCursor(waitCursor);
    statusBar()->changeItem("Reading Article List",1);
    qApp->processEvents ();
    group->getList();
    
    list->setAutoUpdate(false);
    list->clear();
    artList.clear();

    Article *iter;
    for (iter=group->artList.first();iter!=0;iter=group->artList.next())
    {
        if( (!(unread && iter->isRead())) ||
            (showlocked && (!iter->canExpire())) )
        {
                artList.append(iter);
        }
    }

    debug ("count1-->%d",artList.count());
    statusBar()->changeItem("Threading...",1);
    qApp->processEvents ();
    artList.thread(true);
    debug ("count2-->%d",artList.count());

    //had to split this in two loops because the order of articles is not
    //the same in both article lists
    
    statusBar()->changeItem("Showing Article List",1);
    qApp->processEvents ();
    for (iter=artList.first();iter!=0;iter=artList.next())
    {
        QString formatted;
        iter->formHeader(&formatted);
        list->insertItem (formatted.data());
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
    case CONFIG_FONTS:
        {
            debug ("configuring fonts");
            qApp->setOverrideCursor (arrowCursor);
            fontsDlg dlg;
            if(dlg.exec()==1)
            {
                messwin->loadSettings();
            }
            qApp->restoreOverrideCursor ();
            break;
        }
    case PRINT_ARTICLE:
        {
            qApp->setOverrideCursor (arrowCursor);
            debug ("printing");
            messwin->getKHTMLWidget()->print();
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
            messwin->slotVertSubtractPage();
            break;
        }
    case SCROLL_UP_ARTICLE:
        {
            messwin->slotVertSubtractLine();
            break;
        }
    case PAGE_DOWN_ARTICLE:
        {
            messwin->slotVertAddPage();
            break;
        }
    case SCROLL_DOWN_ARTICLE:

        {
            messwin->slotVertAddLine();
            break;
        }
    case POST:
        {
            KMComposeWin *comp=new KMComposeWin(0,"","",0,actFollowup,true,group->data(),false);
            comp->show();
            break;
        }
    case FOLLOWUP:
        {
            int index = list->currentItem();
            
            if(index < 0)
                break;
            
            Article *art=artList.at(index);
            DwMessage *m=new DwMessage();
            QString *ts=server->article(art->ID.data());
            m->FromString(ts->data());
            delete ts;
            m->Parse();

            KMMessage *mm=new KMMessage(m);
            KMComposeWin *comp=new KMComposeWin(0,"","",mm,actFollowup,true,"",false);
            comp->show();
            break;
        }
    case REP_MAIL:
        {
            int index = list->currentItem();
            
            if(index < 0)
                break;
            
            Article *art=artList.at(index);
            DwMessage *m=new DwMessage();
            QString *ts=server->article(art->ID.data());
            m->FromString(ts->data());
            delete ts;
            m->Parse();

            KMMessage *mm=new KMMessage(m);
            KMComposeWin *comp=new KMComposeWin(0,"","",mm,actReply);
            comp->show();
            break;
        }
    case FORWARD:
        {
            int index = list->currentItem();
            
            if(index < 0)
                break;
            
            Article *art=artList.at(index);
            DwMessage *m=new DwMessage();
            QString *ts=server->article(art->ID.data());
            m->FromString(ts->data());
            delete ts;
            m->Parse();

            KMMessage *mm=new KMMessage(m);
            KMComposeWin *comp=new KMComposeWin(0,"","",mm,actForward);
            comp->show();
            break;
        }
    case POSTANDMAIL:
        {
            int index = list->currentItem();
            
            if(index < 0)
                break;
            
            Article *art=artList.at(index);
            DwMessage *m=new DwMessage();
            QString *ts=server->article(art->ID.data());
            m->FromString(ts->data());
            delete ts;
            m->Parse();

            KMMessage *mm=new KMMessage(m);
            KMComposeWin *comp=new KMComposeWin(0,"",mm->from(),mm,actFollowup,true,"");
            comp->show();
            break;
        }

    case CATCHUP:
      {
        group->catchup();
        this->close(FALSE);
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

	break;
      }

      // end robert's cache stuff
      //
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
        else
        {
            messwin->getFromWeb(id);
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
    group->getSubjects(server);
    
    qApp->setOverrideCursor(waitCursor);
    statusBar ()->changeItem ("Getting Article List", 1);
    qApp->processEvents ();

    group->getSubjects(server);

    statusBar ()->changeItem ("", 1);
    qApp->processEvents ();
    qApp->restoreOverrideCursor();
}
