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

#include "newsgroup.h"
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
    if (isRead())
    {
        s->setStr("R\t \t");
    }
    else
    {
        s->setStr("N\t \t");
    }
    if (!isAvailable())
    {
        s->setStr("T\t \t");
    }
    if (isMarked())
    {
        s->setStr("M\t \t");
    }

    char *tempbuf=new char[2048];
    if (From.data())
    {
        DwMailbox fromaddr;
        fromaddr.FromString (From.data());
        fromaddr.Parse();
        sprintf (tempbuf,"%s <%s@%s>",
                 fromaddr.FullName().data(),
                 fromaddr.LocalPart().data(),
                 fromaddr.Domain().data());
        s->append(tempbuf);
    }
    else
    {
        s->append("Unkown Address");
    }
    s->append("\t \t");

    s->append(Lines);
    s->append("\t \t");

    QString t;
    t.setNum(score());
    s->append(t);
    s->append("\t \t");

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
    s->append("\t \t");
    delete[] tempbuf;

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

void ArticleList::thread()
{
    if (count()<=1)
    {
        return; //not much to thread
    }
    
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
    debug ("this should be 0-->%d or I'm losing articles",count());
    clear();
    debug ("found %d threads",roots.count());
    
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
