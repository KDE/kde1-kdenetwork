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
#include <kfm.h>

#include <gdbm.h>
#include <mimelib/mimepp.h>

#include <assert.h>

#include "rules.h"
#include "artdlg.h"

#include "kfileio.h"

extern QString krnpath,cachepath,artinfopath,groupinfopath;

extern GDBM_FILE artdb;
extern GDBM_FILE old_artdb;
extern GDBM_FILE scoredb;

extern QDict <char> unreadDict;

extern QList <Rule> ruleList;

int k1;
int k2;
int k3;
int k4;



////////////////////////////////////////////////////////////////////
// Article class. Represents an article
// Real docs soon.
////////////////////////////////////////////////////////////////////

Article::Article (const char *_ID)
{
    lastScore=0;
    isread=false;
    isavail=true;
    ismarked=false;
    threadDepth=0;
    expire=true;  // robert's cache stuff
    refsLoaded=false;
    Refs.setAutoDelete(true);
    ID=_ID;
    load();
}


Article::Article(void)
{
    lastScore=0;
    isread=false;
    isavail=true;
    ismarked=false;
    threadDepth=0;
    expire=true;  // robert's cache stuff
    refsLoaded=false;
    Refs.setAutoDelete(true);
}

Article::~Article()
{
}

void Article::lookupAltavista()
{
    QString buffer(2048);
    QString urldata("http://ww2.altavista.digital.com/cgi-bin/news.cgi?id@");
    urldata+=ID.mid(1,ID.length()-2);
    KFM fm;
    fm.openURL(urldata);
    return;
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

    tempbuf.setNum(score());
    s->append(tempbuf);
    s->append("\n");
    
    for (int i=0;i<threadDepth;i++)
        s->append("\t");
    s->append(ss.data());
    while (Subject[0]=='{')
        Subject=Subject.right(Subject.length()-1);
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
    
    // end robert's cache stuff
    //
    
    if(isMarked())
        _content+="1\n";
    else
        _content+="0\n";
    
    QString tt;
    tt.setNum(time(NULL));
    _content+=tt;
    _content+="\n";
    
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
bool Article::load()
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
        return false; //Couldn't load it
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
    
    if (!strcmp(tl.at(7),"1"))
        ismarked=true;
    else
        ismarked=false;
    
    for (unsigned int i=9;i<tl.count();i++)
    {
        if(0<strlen(tl.at(i)))
        {
            Refs.append(tl.at(i));
        }
    }
    free (content.dptr);
    refsLoaded=true;
    return true;
}

int Article::score()
{
    datum key;
    datum content;
    int r=0;
    
    key.dptr=ID.data();
    key.dsize=ID.length() + 1;
    
    if (!gdbm_exists(scoredb,key))
        reScore(ruleList);
    content=gdbm_fetch(scoredb,key);
    r=atoi(content.dptr);
    free (content.dptr);
    return r;
}

void Article::reScore(RuleList rules)
{
    QString s;
    int sc=0;
    for (Rule *rule=rules.first();rule!=0;rule=rules.next())
    {
        if (rule->match(*this,0))
        {
            sc+=rule->value;
        }
    }
    s.setNum(sc);
    
    datum key;
    datum content;
    
    key.dptr=ID.data();
    key.dsize=ID.length() + 1;
    content.dptr=s.data();
    content.dsize=s.length() + 1;
    gdbm_store(scoredb,key,content,GDBM_REPLACE);
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

void Article::setMarked(bool b)
{
    if (!refsLoaded) load();
    ismarked = b;
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
    artList.setAutoDelete(true);
    isVisible=0;
    sconf=0;
    name=qstrdup(_name);
    dirty=false;
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

void NewsGroup::clean()
{
    debug ("cleaning %s",name);
    getList();
    
    Article *iter;
    QString s;
    if (artList.isEmpty())
        s="\n";
    else for (iter=artList.first();iter!=0;iter=artList.next())
    {
        s+=iter->ID;
        s+="\n";
    }
    QString ac=krnpath+name;
    unlink(ac);
    kStringToFile (s,ac,false,false);
    
    dirty=false;
}

void NewsGroup::addArticle(QString ID,bool onlyUnread)
{
    if (ID.isEmpty())
    {
        return;
    }
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
        if(art->load())
            artList.append(art);
        else
        {
            dirty=true;
            delete art;
        }
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
    
    int artCount=buffer.contains('\n');
    debug ("There are %d articles",artCount);
    while (1)
    {
        index=buffer.find ('\n',oldindex);
        ID=buffer.mid(oldindex,index-oldindex);
        if (ID.isEmpty())
            break;
        oldindex=index+1;
        addArticle (ID,onlyUnread);
        if (dialog && !(artList.count()%10))
        {
            status.sprintf ("Received %d articles",artList.count());
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
        if (!(server->isCached(art->ID.data())&PART_ALL))
            server->article(art->ID.data());
        qApp->processEvents();
    }
    server->resetCounters (true,true);
    server->reportCounters (true,false);
}
void NewsGroup::catchup()
{
    debug ("catching up");
    getList();
    int count=0;
    for (Article *art=artList.first();art!=0;art=artList.next())
    {
        count++;
        if (!art->isRead())
        {
            art->setRead();
            if (!(count%5))
            {
                qApp->processEvents();
            }
        }
    }
}

int NewsGroup::countNew(NNTP *server)
{
    int count = 0;
    
    getList();

    bool s=false;
    if(strcmp(server->group(), name))
        s=server->setGroup(name);

    if (s)
    {
        if(server->last > lastArticle(server))
            count = server->last - lastArticle(server);
    }
    for(Article *art=artList.first(); art!=0; art=artList.next())
        if(!art->isRead())
            count++;
    artList.clear();
    return(count);
}

////////////////////////////////////////////////////////////////////
// ArtList class. Represents a list of articles
// Real docs soon.
////////////////////////////////////////////////////////////////////


QString noRe(QString subject)
{
    if (subject.left(3)=="Re:")
        subject=subject.right(subject.length()-3);
    return subject.stripWhiteSpace();
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


int compareArticles (Article *a1,Article *a2, int key)
{
    int i=0;
    switch (key)
    {
    case KEY_NONE:
        {
            break;
        }
    case KEY_SUBJECT:
        {
            i=strcmp(noRe(a1->Subject).lower().data(),
                     noRe(a2->Subject).lower().data());
            break;
        }
    case KEY_SENDER:
        {
            i=strcmp(a1->From.lower().data(),
                     a2->From.lower().data());
            break;
        }
    case KEY_LINES:
        {
            int l1=a1->Lines.stripWhiteSpace().toInt();
            int l2=a2->Lines.stripWhiteSpace().toInt();
            if (l1<l2)
            {
                return -1;
            }
            if (l1>l2)
            {
                return 1;
            }
            return 0;
            break;
        }
    case KEY_DATE:
        {
            DwDateTime d1;
            d1.FromString(a1->Date.data());
            d1.Parse();
            DwDateTime d2;
            d2.FromString(a2->Date.data());
            d2.Parse();
            time_t t1=d1.AsUnixTime();
            time_t t2=d2.AsUnixTime();
            if (t1<t2)
            {
                i=-1;
                break;
            }
            if (t1>t2)
            {
                i=1;
                break;
            }
            break;
        }
    case KEY_SCORE:
        {
            int s1=0;
            int s2=0;
            s1=a1->score();
            s2=a2->score();
            if (s1<s2)
            {
                i=-1;
                break;
            }
            if (s2<s1)
            {
                i=1;
                break;
            }
            break;
        }
    }
    return i;
}


int compareThreads(const void *t1,const void *t2)
{
    int i=0;
    i=compareArticles(((ArticleList **)t1)[0]->first(),
                      ((ArticleList **)t2)[0]->first(),k1);
    if (i)
        return i;
    i=compareArticles(((ArticleList **)t1)[0]->first(),
                      ((ArticleList **)t2)[0]->first(),k2);
    if (i)
        return i;
    i=compareArticles(((ArticleList **)t1)[0]->first(),
                      ((ArticleList **)t2)[0]->first(),k3);
    if (i)
        return i;
    i=compareArticles(((ArticleList **)t1)[0]->first(),
                      ((ArticleList **)t2)[0]->first(),k4);
    return i;
}


void ArticleList::thread(bool threaded,int key1,int key2,int key3,int key4)
{
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
    
    ThreadList threads;
    threads.key1=key1;
    threads.key2=key2;
    threads.key3=key3;
    threads.key4=key4;
    
    QDictIterator <node> it(*d);
    
    if (threaded)
    {
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
    }
    else
    {
        while (it.current())
        {
            if (it.current()->art)
            {
                ArticleList *thr=new ArticleList();
                thr->append(it.current()->art);
                it.current()->art->threadDepth=0;
                threads.append(thr);
            }
            ++it;
        }
    }
    this->clear();
    QListIterator <ArticleList> thriter(threads);

    int count=threads.count();
    ArticleList **thrArr=(ArticleList **)malloc(count*sizeof(ArticleList *));
    thriter.toFirst();
    int c=0;
    while (c<count)
    {
        thrArr[c]=thriter.current();
        ++thriter;
        ++c;
    }

    threads.clear();

    k1=key1;
    k2=key2;
    k3=key3;
    k4=key4;

    
    qsort(thrArr,count,sizeof(ArticleList *),compareThreads);

    thriter.toFirst();
    c=0;
    while (c<count)
    {
        QListIterator <Article> artiter(*(thrArr[c]));
        while (artiter.current())
        {
            this->append(artiter.current());
            ++artiter;
        }
        thrArr[c]->clear();
        ++c;
    }

    free (thrArr);
    delete d;
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
