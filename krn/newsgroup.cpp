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
            setRead(true);
        if (st.readLine()=="1")
            setAvailable(true);
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
