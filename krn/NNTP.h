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

#include <ksock.h>
#include <ktreelist.h>

#include "newsgroup.h"

#include <mimelib/nntp.h>

class NNTP: public DwNntpClient
{
public:
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
    int     first;
    int     last;
    int     howmany;
    
    QString    	hostname;
    char    *lastStatusResponse() { return Laststatus.data();};
    bool    reConnect();
        
private:
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
    
};

#endif
