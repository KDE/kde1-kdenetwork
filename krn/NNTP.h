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
#ifndef NNTP_H
#define NNTP_H

#include <stdio.h>

#include <qlist.h>
#include <qstring.h>
#include <qstrlist.h>
#include <qdict.h>
#include <qobject.h>

#include <ksock.h>
#include <ktreelist.h>
#include <Kconfig.h>


#include <mimelib/mimepp.h>

#include "kmmessage.h"

class NNTP;

class Article
{
public:
    Article ();
    ~Article ();
    QString Number;
    QString Subject;
    QString Lines;
    QString From;
    QString ID;
    QString Date;
    QStrList Refs;
    void formHeader(QString *s);
    bool isRead() {return isread;};
    void setRead(bool b=true) {isread=b;save();};
    bool isAvailable() {return isavail;};
    void setAvailable(bool b=true) {isavail=b;save();};
    void setMarked(bool b=true) {ismarked=b;};
    bool isMarked(){return ismarked;};
    void incref(){refcount++;};
    void decref();
    void save();
    void load();
    int  score();
    
    //This value is valid only for a few moments after running
    //thread() over an article list. It's not really meant to be used...
    int  threadDepth;
    
private:
    bool isread;
    bool isavail;
    bool ismarked;
    int refcount;
};

typedef QListT<Article> ArticleListBase;

class ArticleList: public ArticleListBase
{
public:
    ~ArticleList();
    void append(Article *item);
    bool remove(uint index);
    bool remove();
    void thread(bool sortBySubject=false);
    virtual void clear();
    
};

typedef QDictT<Article>ArticleDictBase;

class ArticleDict: public ArticleDictBase
{
public:
    ArticleDict::ArticleDict ();
};

class NewsGroup: public QString
{
public:
    NewsGroup(const char *name);
    ~NewsGroup();
    void save();
    void load();
    
    // Last article number of which the group has info.
    int lastArticle;
    
    //Is this group's window visible?
    bool isVisible;
    //Is it tagged?
    bool isTagged;
    
    //List of articles in the newsgroup.
    ArticleList artList;
    void getList();
    void updateList();
    void getSubjects(NNTP *server);
    void getMessages(NNTP *server);
    void catchup();
private:
};

typedef QListT<NewsGroup> GroupListBase;

class GroupList: public GroupListBase
{
public:
    GroupList();
    ~GroupList();
protected:
    virtual int compareItems(GCI item1,GCI item2);
};


class NNTPObserver:public DwObserver
{
public:
    NNTPObserver (NNTP *_client);
    virtual void Notify();
private:
    NNTP *client;
};



class NNTP: public QObject, public DwNntpClient
{
    
    Q_OBJECT
        
public:
    friend class NNTPObserver;
    NNTP(char *hostname=0);
    ~NNTP();
    
    bool    connect();
    bool    disconnect();
    bool    isConnected() {return Connected;};
    bool    isReadOnly() {return Readonly;};
    void    groupList(QList <NewsGroup> *grouplist, bool fromserver);
    bool    setGroup( const char *groupname);
    char    *group() {return GroupName.data();};
    bool    artList(int from=0,int to=0);
    QString *article(char *id);
    bool    isCached(char *id);
    int     authinfo(const char *username,const char *password);
    int     setMode (char *mode);
    QString    	hostname;
    int     first;
    int     last;
    int     howmany;
    char    *lastStatusResponse() { return Laststatus.data();};
    bool    reConnect();
    void    resetCounters( bool byte=true,bool command=true);
    void    reportCounters (bool byte=true,bool command=true);
    int     byteCounter;
    int     commandCounter;
signals:
    void newStatus(char *status);
    
private:
    bool    reportBytes;
    bool    reportCommands;
    int         listOverview();
    int         listXover(int from=0,int to=0);
    
    QString 	Laststatus;
    QString 	GroupName;
    bool    	Connected;
    bool       	Readonly;
    bool       	checkStatus( QString start);
    void       	getResponse( QString *r);
    bool       	connected;
    void       	overviewFmt( QString of );
    int		OffsetArticleNumber;
    int		OffsetSubject;
    int		OffsetFrom;
    int		OffsetLines;
    int         OffsetID;
    int		OffsetDate;
    int         OffsetRef;
    FILE      	*server;
    KSocket    	*sock;
    QString     partialResponse;
    NNTPObserver  *extendPartialResponse;
    void        PGetTextResponse();
};

#endif
