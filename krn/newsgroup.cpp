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

#include <gdbm.h>

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
    datum key;
    datum content;

    key.dptr=ID.data();
    key.dsize=ID.length() + 1;

    content=gdbm_fetch(artdb,key);

    QString s;

    Subject=strtok ((char *)content.dptr,"\n");

    Lines=strtok(NULL,"\n");
    From=strtok(NULL,"\n");
    Date=strtok(NULL,"\n");
    s=strtok(NULL,"\n");
    if (s=="1")
        isread=true;

    //
    // robert's cache stuff

    QString s2;

    s2=strtok(NULL, "\n");

    if(s2 == "1")
      expire = true;
    else
      expire = false;

    // end  robert's cache stuff
    //

    char *p;
    while (1)
    {
        p=strtok(NULL,"\n");
        if (!p)
            break;
        Refs.append(p);
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
    isTagged=false;
    lastArticle=0;
}


NewsGroup::~NewsGroup()
{
}

void NewsGroup::getList()
{
    int c=0;
    QString ID;
    artList.clear();
    QString ac;
    ac=krnpath+data();
    QFile f(ac);
    if (f.open(IO_ReadOnly))
    {
        QTextStream st(&f);
        while (!st.eof())
        {
            c++;
            if (!(c%100))
                qApp->processEvents();
            ID=st.readLine();
            if (ID.isEmpty())
                break;
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
                artList.append(spart);
            }
        }
    }
}

void NewsGroup::updateList()
{
}

void NewsGroup::save()
{
    QString p=groupinfopath+data();
    QFile f(p.data());
    if(f.open (IO_WriteOnly))
    {
        QTextStream st(&f);
        st<<lastArticle;
        f.close();
    }
}

void NewsGroup::load()
{
    QString p=groupinfopath+data();
    QFile f(p.data());
    if(f.open (IO_ReadOnly))
    {
        QTextStream st(&f);
        lastArticle=st.readLine().toInt();
        f.close();
    }
}

void NewsGroup::getSubjects(NNTP *server)
{
    load();
    if (strcmp(server->group(),data()))
    {
        server->setGroup(data());
    }
    if (server->last>lastArticle)
    {
        debug ("xover from %d to %d",lastArticle+1,server->last+5);
        server->artList(lastArticle,server->last);
        lastArticle=server->last;
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

  if(server->last > lastArticle)
    count = server->last - lastArticle;

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


bool isParent(Article *parent,Article *child)
{
    if (child->Refs.contains(parent->ID.data()))
        return true;
    else
        return false;
}

void collectChildren(ArticleList *parentThread,QList<ArticleList> *children)
{
    QListIterator <ArticleList> it(*children);
    for (;it.current();++it)
    {
        if (it.current()->isEmpty())
            //thread has already been adopted
            continue;
        ArticleList *childThread=it.current();
        if (isParent(parentThread->first(),childThread->first()))
        {
            //It's parent's daughter, make it collect its own kids,
            collectChildren(childThread,children);
            //and then adopt it
            QListIterator <Article> it2(*childThread);
            it2.toFirst();
            for (;it2.current();++it2)
            {
                it2.current()->threadDepth++;
                parentThread->append(it2.current());
            }
            childThread->clear();
        }
    }
}


void ArticleList::thread(bool sortBySubject=false)
{
    if (count()<2)
    {
        return; //not much to thread
    }
    QList <ArticleList> threads;
    QListIterator <ArticleList> thread(threads);
    threads.setAutoDelete(false);
    Article *iter;
    //Make a thread for each article
    //And set their depth to 0
    for (iter=this->first();iter!=0;iter=this->next())
    {
        ArticleList *l=new ArticleList;
        l->append(iter);
        threads.append(l);
        iter->threadDepth=0;
    }

    thread.toFirst();
    //Now consolidate threads
    ArticleList *parentThread;
    thread.toFirst();
    for (;thread.current();++thread)
    {
        parentThread=thread.current();
        if (thread.current()->isEmpty())
            //thread has already been adopted
            continue;
        //look for current's children
        collectChildren(parentThread,&threads);
    }
    clear();

    //If requested, sort the threads by subject
    QList <ArticleList> sortedThreads;
    if (sortBySubject)
    {
        thread.toFirst();
        for (;thread.current();++thread)
        {
            if (thread.current()->isEmpty())
                continue;
            uint i;
            for (i=0;i<sortedThreads.count();i++)
            {
                if (noRe(sortedThreads.at(i)->first()->Subject)
                    >=noRe(thread.current()->first()->Subject).data())
                    //hijole with the condition!
                    break;
            }
            if (i<sortedThreads.count())
                sortedThreads.insert (i,thread.current());
            else
                sortedThreads.append (thread.current());
        }
        threads=sortedThreads;
    }

    //Now thread the subthreads
    //And rebuild the list from them
    thread.toFirst();
    for (;thread.current();++thread)
    {
        //The first one is a root for sure
        if (!thread.current()->isEmpty())
        {
            QListIterator <Article> it2(*thread.current());
            for (;it2.current();++it2)
            {
                append(it2.current());
            }
        }
    }
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
