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

extern ArticleDict artSpool;

extern QString krnpath,cachepath,artinfopath,groupinfopath;


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
    if (isRead())
    {
        s->setStr("R\n \n");
    }
    else
    {
        s->setStr("N\n \n");
    }
    if (!isAvailable())
    {
        s->setStr("T\n \n");
    }
    if (isMarked())
    {
        s->setStr("M\n \n");
    }

    char *tempbuf=new char[2048];
    if (From.data())
    {
        DwMailbox fromaddr;
        fromaddr.FromString (From.data());
        fromaddr.Parse();
        
        s1=fromaddr.FullName().c_str();
        s2=fromaddr.LocalPart().c_str();
        s3=fromaddr.Domain().c_str();
        sprintf (tempbuf,"%s <%s@%s>",s1,s2,s3);
        s->append(tempbuf);
    }
    else
    {
        s->append("Unkown Address");
    }
    s->append("\n \n");

    s->append(Lines);
    s->append("\n \n");

    QString t;
    t.setNum(score());
    s->append(t);
    s->append("\n \n");

    if (Date.data())
    {
        DwDateTime date;
        date.FromString(Date.data());
        date.Parse();
        sprintf(tempbuf,"%d/%d",date.Day(),date.Month());
        s->append(tempbuf);
    }
    else
    {
        s->append("0/0");
    }
    s->append("\n \n");
    delete[] tempbuf;

//    for (int i=0;i<threadDepth;i++)
//        s->append("\t");
    s->append(Subject);
}

void Article::save()
//stores the article info and data into the cache
{
    QString p=artinfopath+ID;
    QFile f(p.data());
    if(f.open (IO_WriteOnly))
    {
        QTextStream st(&f);
        st<<ID<<"\n";
        st<<Subject<<"\n";
        st<<Lines<<"\n";
        st<<From<<"\n";
        st<<ID<<"\n";
        st<<Date<<"\n";
        st<<isRead()<<"\n";
        st<<isAvailable()<<"\n";
        for (char *iter=Refs.first();iter!=0;iter=Refs.next())
        {
            st<<iter<<"\n";
        }
        f.close();
    }
}
void Article::load()
//gets the article info and data from the cache
{
    QString s;
    QString p=artinfopath+ID;
    QFile f(p.data());
    if(f.open (IO_ReadOnly))
    {
        QTextStream st(&f);
        ID=st.readLine();
        Subject=st.readLine();
        Lines=st.readLine();
        From=st.readLine();
        ID=st.readLine();
        Date=st.readLine();
        if (st.readLine()=="1")
            isread=true;
        else
            isread=false;
        if (st.readLine()=="1")
            isavail=true;
        else
            isavail=false;
        Refs.clear();
        while (1)
        {
            s=st.readLine();
            if (s.isEmpty())
                break;
            Refs.append(s.data());
        }
        f.close();
    }
}

int Article::score()
{
    return 100;
}


////////////////////////////////////////////////////////////////////
// NewsGroup class. Represents a newsgroup
// Real docs soon.
////////////////////////////////////////////////////////////////////


NewsGroup::NewsGroup(const char *name)
    :QString(name)
{
    isVisible=false;
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
        debug ("xover from %d to %d",lastArticle+1,server->last);
        server->artList(lastArticle,server->last);
        lastArticle=server->last;
        save();
    }
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
    if (subject.find("Re: ")==0)
    {
        return subject.right(subject.length()-4);
    }
    else if (subject.find("Re:")==0)
    {
        return subject.right(subject.length()-3);
    }
    return subject;
}


bool isParent(Article *parent,Article *child)
{
    if (child->Refs.contains(parent->ID.data()))
        return true;
    else
        return false;
}

/*
void ArticleList::thread(bool sortBySubject=false)
{
    //This isn't working
    return;
    if (count()<=1)
    {
        return; //not much to thread
    }
    
    QList <ArticleList> threads;
    QListIterator <ArticleList> thread(threads);
    threads.setAutoDelete(false);
    
    Article *iter;
    //Make a thread for each article
    //Sorted by Subjects, ignoring initial "Re:"'s
    for (iter=this->first();iter!=0;iter=this->next())
    {
        ArticleList *l=new ArticleList;
        l->append(iter);
        threads.append(l);
    }
    
    //Now consolidate threads
    bool breaking=false;
    bool dirty=false;
    ArticleList *parentThread;
    ArticleList *childThread;
    thread.toFirst();
    for (;thread.current();++thread)
    {
        dirty=false;
        childThread=thread.current();
        if (childThread->isEmpty())
            continue;
        //So I got a thread. What should I do with it?
        //Look for feasible parents in the thread list
        //Start with my freshest reference, and go backwards.
        Article *art=thread.current()->first();
        //Go over all threads until I find a parent.
        QListIterator <ArticleList> it(threads);
        for (;it.current();++it)
        {
            parentThread=it.current();
            QListIterator <Article> it2(*it.current());
            for (;it2.current();++it2)
            {
                if (isParent(it2.current(),art))
                {
                    //This is a parent, so I'll attach myself to this thread
                    //and jump out
                    breaking=true;
                    break;
                }
            }
            if (breaking)
                break;
        }
        if (breaking)
        {
            //I should put the current thread attached in the end of
            //parentThread

            //So, remove the child thread from the threads list
            //Now iterate over childThread, and move the articles to
            //parentThread
            QListIterator <Article> it2(*childThread);
            for (;it2.current();++it2)
            {
                parentThread->append(it2.current());
            }
            childThread->clear();
            //And start back from the first thread.
            //If no threads need reparenting, the outer loop will end
            dirty=true;
        }
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

    thread.toFirst();
    for (;thread.current();++thread)
    {
        debug (thread.current()->first()->Subject.data());
    }
    
    //Now thread the subthreads
    //And rebuild the list from them
    thread.toFirst();
    for (;thread.current();++thread)
    {
        //The first one is a root for sure
        if (thread.current()->first())
        {
            append(thread.current()->first());
            thread.current()->remove(0);
//            thread.current()->thread();
        }
        //And add all the articles back in.
        QListIterator <Article> it2(*thread.current());
        for (;it2.current();++it2)
        {
            append(it2.current());
        }
    }
    
    }
    */

void ArticleList::thread()
{
    if (count()<=1)
    {
        return; //not much to thread
    }
    //this can take a while
    qApp->processEvents();
    
    QDict <ArticleList> threads;
    QStrList IDs;
    QStrList roots;
    
    //Make a list of article IDs
    Article *iter;
    for (iter=this->first();iter!=0;iter=this->next())
    {
        IDs.append(iter->ID.data());
    }
    
    for (iter=this->first();iter!=0;iter=this->next())
    {
        bool root=true;
        
        //For each reference
        
        for (char *ref=iter->Refs.first();ref!=0;ref=iter->Refs.next())
        {
            //See if it refers to any of these articles
            int index=IDs.find(ref);
            if (index!=-1)
            {
                //if it is a loop
                if (artSpool[IDs.at(index)]->Refs.contains(ref))
                {
                    //hate it, and then skip the reference.
                    debug ("breaking reference loop! (maybe)");
                    continue;
                }
                root=false;
                break;
            }
        }
        //If it's a root item, start a thread with it
        if (root)
        {
            ArticleList *list=new ArticleList();
            list->append(iter);
            threads.insert(iter->ID.data(),list);
            //Add it to the list of roots, sorted by order of subject!
            int i=0;
            for (char *_root=roots.first();_root!=0;_root=roots.next())
            {
                if (noRe(threads.find(_root)->first()->Subject) >=
                    noRe(iter->Subject).data())
                {
                    break;
                }
                i++;
            }
            roots.insert(i,iter->ID.data());
        }
    }

    //Remove all roots from the list
    for (char *s=roots.first();s!=0;s=roots.next())
    {
        //Ain't this a fine statement?
        this->remove(this->findRef(threads[s]->first()));
    }

    //This shouldn't exist: an article that makes a reference to another
    //but not to it's ancesters and/or our root article in the
    //thread of the second.
    //But.... it happens about 10% of the time

    ArticleList brokenlist;
    ArticleList notbrokenlist;
    
    for (iter=this->first();iter!=0;iter=this->next())
    {
        bool broken=true;
        
        //For each reference
        for (char *ref=iter->Refs.first();ref!=0;ref=iter->Refs.next())
        {
            //See if it refers to any of these articles
            if (roots.find(ref)!=-1)
            {
                broken=false;
                break;
            }
        }
        //If it's a broken item, complain
        if (broken)
        {
            debug ("Broken reference, fixing it");
            //put it in the broken list
            brokenlist.append(iter);
        }
    }

    //add the broken references as roots. this sucks, but inserting
    //them properly looks like work
    for (Article *art=brokenlist.first();art!=0;art=brokenlist.next())
    {
        ArticleList *list=new ArticleList();
        list->append(art);
        threads.insert(art->ID.data(),list);
        //remove it from the current list
        this->removeRef(art);
        //Add it to the list of roots, sorted by order of subject!
        int i=0;
        for (char *_root=roots.first();_root!=0;_root=roots.next())
        {
            if (noRe(threads.find(_root)->first()->Subject) >=
                noRe(art->Subject).data())
            {
                break;
            }
            i++;
        }
        roots.insert(i,art->ID.data());
    }

    
    for (iter=this->first();iter!=0;)
    {
        bool inserted=false;
        for (char *ref=iter->Refs.first();ref!=0;ref=iter->Refs.next())
        {
            //If this ref is to an existing root,
            //I'll place this article in that thread.
            int i=roots.find(ref);
            if (i!=-1)
            {
                threads[ref]->append(iter);
                inserted=true;
            }
        }
        this->remove();
        iter=this->first();
    }

    //whack the list
    clear();
    //And now reassemble the list from the threads
    //Iterate all the threads
    for (char *s=roots.first();s!=0;s=roots.next())
    {
        ArticleList *l=threads.find(s);
        // Thread the thread
        // I know the first one is the root, but the rest is suspect.
        // So, I'll take the root, thread the rest, and then add them.
        if (l->first())
            this->append(l->first());
        else
        {
            debug ("huh? bad pointer in athread with %d articles!",l->count());
            continue;
        }
        l->remove(0);
        l->thread();
        //Add all articles in the thread back to the list
        for (Article *art=l->first();art!=0;art=l->next())
        {
            this->append(art);
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
