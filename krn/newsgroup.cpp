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
#include <stdlib.h>

#include "NNTP.h"
#include <qstrlist.h>
#include <qlist.h>
#include <qbuffer.h>

#include <gdbm.h>

#include <assert.h>

#include "artdlg.h"

#include "kfileio.h"

extern ArticleDict artSpool;

extern QString krnpath,cachepath,artinfopath,groupinfopath;

extern GDBM_FILE artdb;
extern GDBM_FILE old_artdb;

extern QDict <char> unreadDict;

////////////////////////////////////////////////////////////////////
// Article class. Represents an article
// Real docs soon.
////////////////////////////////////////////////////////////////////

Article::Article(void)
{
    isread=false;
    isavail=true;
    ismarked=false;
    refcount=0;
    threadDepth=0;
    expire=true;  // robert's cache stuff
    refsLoaded=false;
    Refs.setAutoDelete(true);
}

void Article::decref()
{
    refcount--;
    if (!refcount)
    {
        artSpool.remove(ID.data());
    }
};

void Article::incref()
{
    refcount++;
    if (refcount==1)
    {
        artSpool.insert(ID.data(),this);
    }
};

Article::~Article()
{
}


void Article::formHeader(QString *s)
// Builds a nice header to put in the header list, which properly
// Reflects the internal state of the message
{
    s->setStr(" ");
    QString ss;
    
    if (isRead())
    {
        ss.setStr("{R}");
    }
    else
    {
        ss.setStr("{N}");
    }
    if (!isAvailable())
    {
        ss.setStr("{T}");
    }
    if (isMarked())
    {
        ss.append("{M}");
//        ss.setStr("{M}");
    }
    if (!canExpire())
    {
        ss.append("{L}");
//        ss.setStr("{L}");
    }
    
    if (!From.isEmpty())
    {
        s->append(KMMessage::stripEmailAddr(From));
        s->append("\n");
    }
    else
    {
        s->append("Unkown Address\n");
    }
    
    
    QString tempbuf;
    if (Date.data())
    {
        DwDateTime date;
        date.FromString(Date.data());
        date.Parse();
        tempbuf.sprintf("%d/%d/%d",date.Day(),date.Month(),date.Year());
        s->append(tempbuf);
    }
    else
    {
        s->append("-/-/-");
    }
    s->append("\n");
    
    s->append(Lines);
    s->append(" ");
    s->append("\n");
    
    
    for (int i=0;i<threadDepth;i++)
        s->append("\t");
    s->append(ss.data());
    s->append(Subject);
}

void Article::save()
//stores the article info and data into the cache
{
    
    datum key;
    key.dptr=ID.data();
    key.dsize=ID.length()+1;
    
    QString _content;
    _content+=Subject+"\n";
    _content+=ID+"\n";
    _content+=Lines+"\n";
    _content+=From+"\n";
    _content+=Date+"\n";
    if (isRead())
        _content+="1\n";
    else
        _content+="0\n";
    
    //
    // robert's cache stuff
    
    if(canExpire())
        _content+="1\n";
    else
        _content+="0\n";
    
    // end robert;s cache stuff
    //
    
    for (char *iter=Refs.first();iter!=0;iter=Refs.next())
    {
        _content+=iter;
        _content+="\n";
    }
    
    datum content;
    content.dptr=_content.data();
    content.dsize=_content.length()+1;
    
    if (!isRead())
    {
        gdbm_store(artdb,key,content,GDBM_REPLACE);
        gdbm_delete(old_artdb,key);
    }
    else
    {
        gdbm_store(old_artdb,key,content,GDBM_REPLACE);
        gdbm_delete(artdb,key);
    }
    
}
void Article::load()
//gets the article info and data from the cache
{
    QStrList tl;
    tl.setAutoDelete(true);
    datum key;
    datum content;
    
    key.dptr=ID.data();
    key.dsize=ID.length() + 1;

    content=gdbm_fetch(old_artdb,key);
    if (!content.dptr)
    {
        content=gdbm_fetch(artdb,key);
    }
    if (!content.dptr)
    {
        debug ("couldn't load");
        return; //Couldn't load it
    }
    
    QString s=(char *)content.dptr;
    
    int index=0;
    
    QString t;
    while (1)
    {
        index=s.find("\n");
        if (index==-1)
        {
            tl.append(s);
            break;
        }
        t=s.left (index);
        s=s.right(s.length()-index-1);
        if (t.isEmpty())
            continue;
        tl.append (t.data());
    }
    Subject=tl.at(0);
    ID=tl.at(1);
    Lines=tl.at(2);
    From=tl.at(3);
    Date=tl.at(4);
    if (!strcmp(tl.at(5),"1"))
        isread=true;
    else
        isread=false;
    
    if (!strcmp(tl.at(6),"1"))
        expire=true;
    else
        expire=false;
    
    for (unsigned int i=7;i<tl.count();i++)
    {
        if(0<strlen(tl.at(i)))
        {
            Refs.append(tl.at(i));
        }
    }
    free (content.dptr);
    refsLoaded=true;
}

int Article::score()
{
    return 100;
}

void Article::setRead(bool b)
{
    if (!refsLoaded) load();
    if (b)
        unreadDict.remove(ID.data());
    else
        unreadDict.replace(ID.data(),ID.data());
    isread = b;
    save();
}

void Article::setAvailable(bool b)
{
    if (!refsLoaded) load();
    isavail = b;
    save();
}

bool Article::canExpire()  // robert's cache stuff
{
    return(expire);
}

void Article::setExpire(bool b)   // robert's cache stuff
{
    if (!refsLoaded) load();
    expire = b;
    save();
}

void Article::toggleExpire()   // robert's cache stuff
{
    if (!refsLoaded) load();
    if(expire)
        expire = false;
    else
        expire = true;
    save();
}

KMMessage *Article::createMessage ()
{
    KMMessage *m=new KMMessage();
    return m;
}

////////////////////////////////////////////////////////////////////
// NewsGroup class. Represents a newsgroup
// Real docs soon.
////////////////////////////////////////////////////////////////////


NewsGroup::NewsGroup(const char *_name)
{
    isVisible=0;
    sconf=0;
    name=qstrdup(_name);
}


NewsGroup::~NewsGroup()
{
    if (sconf)
    {
        save();
        delete sconf;
    }
    free (name);
}
void NewsGroup::addArticle(QString ID,bool onlyUnread)
{
    if (ID.isEmpty())
    {
        return;
    }
    Article *spart=artSpool.find(ID.data());
    if (spart==NULL)
    {
        if (onlyUnread)
        {
            if (unreadDict.find(ID.data()))
            {
                Article *art=new Article();
                art->ID=ID;
                art->load();
                artList.append(art);
            }
        }
        else
        {
            Article *art=new Article();
            art->ID=ID;
            art->load();
            artList.append(art);
        }
    }
    else
    {
        if (artList.findRef (spart)==-1)
            artList.append(spart);
    }
}

void NewsGroup::getList(Artdlg *dialog)
{
    QString ID;
    QString ac;
    QString status;
    ac=krnpath+name;
    bool onlyUnread=false;
    if (dialog)
        onlyUnread=(dialog->unread);
    if (!QFile::exists(ac))
        return;
    
    QString buffer=kFileToString (ac,true,true);
    if (buffer.isNull())
        return;
    
    int index=0;
    int oldindex=0;
    int counter=0;

    int artCount=buffer.contains('\n');
    debug ("There are %d articles",artCount);
    while (1)
    {
        index=buffer.find ('\n',oldindex);
        ID=buffer.mid(oldindex,index-oldindex);
        if (ID.isEmpty())
            break;
        counter++;
        oldindex=index+1;
        addArticle (ID,onlyUnread);
        if (dialog && !(counter%50))
        {
            status.sprintf ("Received %d articles",counter);
            dialog->updateCounter(status);
        }
    }
}

void NewsGroup::updateList()
{
}

void NewsGroup::save()
{
}

void NewsGroup::load()
{
}

int NewsGroup::lastArticle(NNTP *server)
{
    QString p=groupinfopath+name;
    if (!sconf)
    {
        QString q=p+".conf";
        sconf=new KSimpleConfig(q);
    }
    sconf->setGroup(server->hostname.data());
    return sconf->readNumEntry("LastArticle",0);
    
}

void NewsGroup::saveLastArticle(NNTP *server,int i)
{
    QString p=groupinfopath+name;
    if (!sconf)
    {
        QString q=p+".conf";
        sconf=new KSimpleConfig(q);
    }
    sconf->setGroup(server->hostname.data());
    sconf->writeEntry("LastArticle",i);
    sconf->sync();
    
}

void NewsGroup::getSubjects(NNTP *server)
{
    load();
    server->setGroup(name);
    if (server->last>lastArticle(server))
    {
        debug ("xover from %d to %d",lastArticle(server)+1,server->last+5);
        server->artList(lastArticle(server),server->last,this);
        saveLastArticle(server,server->last);
        save();
    }
}

void NewsGroup::getMessages(NNTP *server)
{
    debug ("getting articles in %s",name);
    load();
    getSubjects(server);
    getList();
    server->resetCounters (true,true);
    server->reportCounters (false,true);
    for (Article *art=artList.first();art!=0;art=artList.next())
    {
        if (!server->isCached(art->ID.data()))
            server->article(art->ID.data());
        qApp->processEvents();
    }
    server->resetCounters (true,true);
    server->reportCounters (true,false);
}
void NewsGroup::catchup()
{
    debug ("catching up");
    load();
    getList();
    for (Article *art=artList.first();art!=0;art=artList.next())
    {
        if (!art->isRead())
        {
            art->setRead();
        }
    }
}

int NewsGroup::countNew(NNTP *server)
{
    int count = 0;
    
    load();
    getList();
    
    if(strcmp(server->group(), name))
        server->setGroup(name);
    
    if(server->last > lastArticle(server))
        count = server->last - lastArticle(server);
    
    for(Article *art=artList.first(); art!=0; art=artList.next()) {
        if(!art->isRead())
            count++;
    }
    
    return(count);
}

////////////////////////////////////////////////////////////////////
// ArtList class. Represents a list of articles
// Real docs soon.
////////////////////////////////////////////////////////////////////

void ArticleList::append(Article *item)
{
    item->incref();
    ArticleListBase::append((const Article *)item);
}

bool ArticleList::remove(uint index)
{
    Article *art=at(index);
    bool b=ArticleListBase::remove(index);
    if (b)
        art->decref();
    return b;
}

bool ArticleList::remove()
{
    return remove(at());
}

void ArticleList::clear()
{
    while (!isEmpty())
        remove(0);
}

QString noRe(QString subject)
{
    if (subject.left(3)=="Re:")
        subject=subject.right(subject.length()-3);
    if (subject.left(1)==" ")
        subject=subject.right(subject.length()-1);
    return subject;
}


int vc=0;

struct node
{
    Article *art;
    void *parent;
    QList <void> children;
};

QDict <node> *d;

void do_insert(Article *art)
{
    node *a=0;
    a=d->find(art->ID.data());
    if (a) //article is in the dict
    {
        if (a->art==0)
        {
            a->art=art;
            if (a->parent)
            {
                debug ("marker 1");
                debug ("ID-->%s",art->ID.data());
//                ((node *)a->parent)->children.removeRef(a);
//                a->parent=0;
//                return;
            }
        }
        else
        {
            return;
        }
    }
    else //article is not in the dict
    {
        a=new node;
        a->art=art;
        a->parent=0;
        a->children.clear();
        d->insert (art->ID.data(),a);
    }
    assert (a->art==art);
    node *last=a;
    node *b=0;
    QListIterator <char> iter(art->Refs);
    iter.toLast();
    char *ref;
    for (;iter.current();--iter)
    {
        ref=iter.current();
        b=d->find(ref);
        if (b==0)
        {
            b=new node;
            b->art=0;
            b->parent=0;
            d->insert (ref,b);
            last->parent=b;
            b->children.clear();
            b->children.append(last);
            last=b;
        }
        else
        {
//            if (-1==b->children.findRef(last))
                b->children.append(last);
            last->parent=b;
            break;
        }
    }
}

void addToList(node *n,int dep,ArticleList *l)
{
    node *child;
    QListIterator <void> childIt(n->children);
    for (;childIt.current();++childIt)
    {
        child=(node *)childIt.current();
        if (child->art)
        {
            l->append(child->art);
            child->art->Refs.clear();
            child->art->refsLoaded=false;
            child->art->threadDepth=dep;
            addToList (child,dep+1,l);
        }
        else
            addToList (child,dep,l);
    }
}

void ArticleList::thread(bool)
{
    debug ("entered with-->%d",count());
    d=new QDict <node> (10271);
    d->setAutoDelete(true);
    QListIterator <Article> artit(*this);
    Article *iter;
    for (;artit.current();++artit)
    {
        iter=artit.current();
        if (!(iter->refsLoaded))
            iter->load();
        iter->threadDepth=0;
        do_insert(iter);
    }

    QList <ArticleList> threads;
    QDictIterator <node> it(*d);

    while (it.current())
    {
        if (!(it.current()->parent))
        {
            ArticleList *thr=new ArticleList();
            if (it.current()->art)
                thr->append(it.current()->art);
            addToList(it.current(),0,thr);
            threads.append(thr);
        }
        ++it;
    }
    debug ("has %d threads",threads.count());
    this->clear();


    QListIterator <ArticleList> thriter(threads);
    while (thriter.current())
    {
        QListIterator <Article> artiter(*thriter.current());
        while (artiter.current())
        {
//            if(-1==findRef(artiter.current()))
                this->append(artiter.current());
//            else
//                debug("repeated article!!! %s",artiter.current()->ID.data());
            ++artiter;
        }
        thriter.current()->clear();
        ++thriter;
    }


    delete d;
    debug ("exited with-->%d",count());
}

ArticleList::ArticleList()
{
}

ArticleList::~ArticleList()
{
    clear();
}

////////////////////////////////////////////////////////////////////
// GroupList class. Represents a list of newsgroup
// Real docs soon.
////////////////////////////////////////////////////////////////////


GroupList::GroupList()
{
    setAutoDelete(true);
}

GroupList::~GroupList()
{
}
int GroupList::compareItems(GCI item1,GCI item2)
{
    return strcmp(((NewsGroup *)item1)->name,((NewsGroup *)item2)->name);
}


ArticleDict::ArticleDict ()
{
    setAutoDelete(true);
};
