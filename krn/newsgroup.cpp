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

extern ArticleDict artSpool;

extern QString krnpath,cachepath,artinfopath,groupinfopath;

extern GDBM_FILE artdb;

////////////////////////////////////////////////////////////////////
// Article class. Represents an article
// Real docs soon.
////////////////////////////////////////////////////////////////////

Article::Article(void)
{
    setRead(false);
    setAvailable(true);
    setMarked (false);
    refcount=0;
    Refs.setAutoDelete(true);
    threadDepth=0;
    setExpire(true);  // robert's cache stuff
}

void Article::decref()
{
    refcount--;
    if (!refcount)
    {
        artSpool.remove(ID.data());
    }
};

Article::~Article()
{
}


void Article::formHeader(QString *s)
// Builds a nice header to put in the header list, which properly
// Reflects the internal state of the message
{
    const char *s1, *s2, *s3;
    s->setStr(" ");
    QString ss;
    
    if (isRead())
   {
        ss.setStr("{R} ");
    }
    else
    {
        ss.setStr("{N} ");
    }
    if (!isAvailable())
    {
        ss.setStr("{T} ");
    }
    if (isMarked())
    {
        ss.setStr("{M} ");
    }
    if (!canExpire())
    {
        ss.setStr("{L} ");
    }

    ss.append(" ");
    char *tempbuf=new char[2048];
    if (From.data())
    {
        DwMailbox fromaddr;
        fromaddr.FromString (From.data());
        fromaddr.Parse();
        
        s1=fromaddr.FullName().c_str();
        s2=fromaddr.LocalPart().c_str();
        s3=fromaddr.Domain().c_str();
        if (strlen(s1))
            sprintf (tempbuf,"%s\n",s1);
        else
            sprintf (tempbuf,"<%s@%s>\n",s2,s3);
        s->append(tempbuf);
    }
    else
    {
        s->append("Unkown Address\n");
    }


    if (Date.data())
    {
        DwDateTime date;
        date.FromString(Date.data());
        date.Parse();
        sprintf(tempbuf,"%d/%d/%d",date.Day(),date.Month(),date.Year());
        s->append(tempbuf);
    }
    else
    {
        s->append("-/-/-");
    }
    s->append("\n");
    delete[] tempbuf;

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
    gdbm_store(artdb,key,content,GDBM_REPLACE);
    
}
void Article::load()
//gets the article info and data from the cache
{
    QStrList tl;
    datum key;
    datum content;

    key.dptr=ID.data();
    key.dsize=ID.length() + 1;

    content=gdbm_fetch(artdb,key);

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
}

int Article::score()
{
    return 100;
}

bool Article::canExpire()  // robert's cache stuff
{
  return(expire);
}

void Article::setExpire(bool b)   // robert's cache stuff
{
  expire = b;
  save();
}

void Article::toggleExpire()   // robert's cache stuff
{
  if(expire)
    expire = false;
  else
    expire = true;

  save();
}



////////////////////////////////////////////////////////////////////
// NewsGroup class. Represents a newsgroup
// Real docs soon.
////////////////////////////////////////////////////////////////////


NewsGroup::NewsGroup(const char *name)
    :QString(name)
{
    isVisible=0;
    sconf=0;
}


NewsGroup::~NewsGroup()
{
    if (sconf)
    {
        save();
        delete sconf;
    }
}
void NewsGroup::addArticle(QString ID)
{
    if (ID.isEmpty())
    {
        return;
    }
    Article *spart=artSpool.find(ID.data());
    if (spart==NULL)
    {
        Article *art=new Article();
        art->ID=ID;
        art->load();
        artSpool.insert(ID.data(),art);
        artList.append(art);
    }
    else
    {
        if (artList.findRef (spart)==-1)
            artList.append(spart);
    }
}

void NewsGroup::getList()
{
    int c=0;
    QString ID;
//    artList.clear();
    QString ac;
    ac=krnpath+data();
    QFile f(ac);
    if (!f.exists())
        return;
    QByteArray arr(f.size());
    
    if (f.open(IO_ReadOnly))
    {
        f.readBlock(arr.data(),f.size());
        f.close();
        QBuffer b(arr);
        b.open(IO_ReadOnly);
        QTextStream st(&b);
        while (!st.eof())
        {
            c++;
            if (!(c%100))
                qApp->processEvents();
            ID=st.readLine();
            addArticle (ID);
        }
        b.close();
        arr.resize(0);
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
    QString p=groupinfopath+data();
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
    QString p=groupinfopath+data();
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
    server->setGroup(data());
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
    debug ("getting articles in %s",data());
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

  if(strcmp(server->group(), data()))
    server->setGroup(data());

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

void do_insert(QString id,Article *art)
{
    node *a=0;
    a=d->find(id);
    if (a) //article is in the dict
    {
        if (a->art==0)
        {
            a->art=art;
        }
    }
    else //article is not in the dict
    {
        a=new node;
        a->art=art;
        a->parent=0;
        d->insert (id,a);
    }
assert (a->art==art);
    if (!art->Refs.count())
        return;
    node *last=a;
    node *b=0;
    for (char *ref=art->Refs.last();ref!=0;ref=art->Refs.prev())
    {
        b=0;
        b=d->find(ref);
        if (b==0)
        {
            b=new node;
            b->art=0;
            b->parent=0;
            d->insert (ref,b);
            last->parent=b;
            b->children.append(last);
            last=b;
        }
        else
        {
            b->children.append(last);
            last->parent=b;
            break;
        }
    }
}

void addToList(node *n,int dep,ArticleList *l)
{
    for (node *child=(node *)n->children.first();child!=0;child=(node *)n->children.next())
    {
        if (child->art)
        {
            l->append(child->art);
            child->art->threadDepth=dep;
            addToList (child,dep+1,l);
        }
        else
            addToList (child,dep,l);
    }
}

void ArticleList::thread(bool)
{
    d=new QDict <node>;
    d->setAutoDelete(true);
    for (Article *iter=this->first();iter!=0;iter=this->next())
    {
        do_insert(iter->ID,iter);
    }
    node *n=new node;
    n->art=0;
    n->parent=0;
    QDictIterator <node> it(*d);
    
    while (it.current())
    {
        if (!(it.current()->parent))
        {
            it.current()->parent=n;
            n->children.append(it.current());
        }
        ++it;
    }
    this->clear();
    addToList (n,0,this);
    delete d;
}

ArticleList::ArticleList()
{
    visited=false;
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
}

GroupList::~GroupList()
{
}
int GroupList::compareItems(GCI item1,GCI item2)
{
    return strcmp(((NewsGroup *)item1)->data(),((NewsGroup *)item2)->data());
}


ArticleDict::ArticleDict ()
{
    setAutoDelete(true);
};
