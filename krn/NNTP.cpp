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
#include <unistd.h>

#include "NNTP.h"

#include <kmsgbox.h>
#include <kconfig.h>
#include <kapp.h>

#include <qlist.h>
#include <qfile.h>
#include <qtstream.h>
#include <qapp.h>

#include <gdbm.h>

extern QString krnpath,cachepath,artinfopath;
extern GDBM_FILE artdb;

#include <mimelib/mimepp.h>

char debugbuf[1024];

#include "NNTP.moc"

extern KConfig *conf;

// NNTPObserver class. Used to get feedback from NNTP

NNTPObserver::NNTPObserver (NNTP *_client)
{
    client=_client;
}

int oldbytes;

void NNTPObserver::Notify()
{
    client->byteCounter+=client->mTextResponse.length();
    client->partialResponse+=client->mTextResponse;
    qApp->processEvents();
    if (client->reportBytes && (client->byteCounter - oldbytes)>1024 )
    {
        char *buffer=new char[100];
        sprintf (buffer,klocale->translate("Received %.2f Kb"),
                 ((double)client->byteCounter)/1024);
        emit client->newStatus(buffer);
        qApp->processEvents();
        delete[] buffer;
        oldbytes=client->byteCounter;
    }
}


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
//  NNTP class. Implements the NNTP protocol (I hope)
//
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////


NNTP::NNTP(char *host): DwNntpClient()
{
    hostname=host;
    Connected=false;
    Readonly=false;
    KDEBUG (KDEBUG_INFO,3300,hostname);
    reportBytes=false;
    reportCommands=false;
    byteCounter=0;
    commandCounter=0;
    extendPartialResponse=new NNTPObserver (this);
    SetObserver(extendPartialResponse);
    
}

void NNTP::PGetTextResponse()
{
    KDEBUG (KDEBUG_INFO,3300,"entered NNTP::PGetTextResponse()");
    partialResponse="";
    qApp->processEvents();
    SetObserver(extendPartialResponse);
    DwNntpClient::PGetTextResponse();
    mTextResponse=qstrdup(partialResponse.c_str());
    partialResponse="";
    SetObserver(NULL);
    KDEBUG (KDEBUG_INFO,3300,"exited NNTP::PGetTextResponse()");
}

void NNTP::resetCounters( bool byte,bool command)
{
    if (byte)
        byteCounter=0;
    if (command)
        commandCounter=0;
}
void NNTP::reportCounters (bool byte,bool command)
{
    reportBytes=byte;
    reportCommands=command;
}


NNTP::~NNTP()
{
}
bool NNTP::connect()
{
    if (Connected)
        return true;
    int status;
    conf->setGroup("NNTP");
    hostname=conf->readEntry("NNTPServer");

    if (!hostname) // no server in the env, nowhere to connect
        return false;
    else
    {
        KDEBUG (KDEBUG_INFO,3300,"Connecting to ...")
        KDEBUG (KDEBUG_INFO,3300,hostname.data());
        status=Open(hostname.data());
    }
    if (status!=200 && status!=201)
        return false;
    if (status==201)
        Readonly=true;
    
    Connected=true;
    
    //this is needed sometimes for some versions of INN
    status=setMode("reader");

    status=listOverview();

    return Connected;
}
bool NNTP::reConnect()
{
    return true;
}
bool NNTP::disconnect()
{
    int status=Quit();
    status=Close();
    Connected=false;
    return true;
}

int NNTP::authinfo(const char *username,const char *password)
{
    if (username)
    {
        sprintf (mSendBuffer,"authinfo user %s\r\n",username);
        cout << "C: " << mSendBuffer << endl;
        
        mReplyCode = -1;
        int bufferLen = strlen(mSendBuffer);
        int numSent = PSend(mSendBuffer, bufferLen);
        if (numSent == bufferLen)
        {
            PGetStatusResponse();
            cout <<"S: " << StatusResponse() << endl;
        }
    }
    if (password)
    {
        sprintf (mSendBuffer,"authinfo pass %s\r\n",password);
        cout << "C: " << mSendBuffer << endl;
        
        mReplyCode = -1;
        int bufferLen = strlen(mSendBuffer);
        int numSent = PSend(mSendBuffer, bufferLen);
        if (numSent == bufferLen)
        {
            PGetStatusResponse();
            cout <<"S: " << StatusResponse() << endl;
        }
    }
    return mReplyCode;
}

int NNTP::listOverview()
{
    strcpy(mSendBuffer, "LIST overview.fmt\r\n");
    
    // for debugging
    cout << "C: " << mSendBuffer << endl;
    
    mReplyCode = -1;
    int bufferLen = strlen(mSendBuffer);
    int numSent = PSend(mSendBuffer, bufferLen);
    if (numSent == bufferLen) {
        PGetStatusResponse();
        cout <<"S: " << StatusResponse() << endl;
        if (mReplyCode/100%10 == 2)
        {
            PGetTextResponse();
            QString of=TextResponse().data();
            int index;
            
            index=of.find ("Subject");
            if (index!=-1)
                OffsetSubject=of.left(index).contains('\n')+1;
            else
                OffsetSubject=0;
            
            index=of.find ("From");
            if (index!=-1)
                OffsetFrom=of.left(index).contains('\n')+1;
            else
                OffsetFrom=0;
            
            index=of.find ("Lines");
            if (index!=-1)
                OffsetLines=of.left(index).contains('\n')+1;
            else
                OffsetLines=0;
            
            index=of.find ("Message-ID");
            if (index!=-1)
                OffsetID=of.left(index).contains('\n')+1;
            else
                OffsetID=0;
            
            index=of.find ("Date");
            if (index!=-1)
                OffsetDate=of.left(index).contains('\n')+1;
            else
                OffsetDate=0;
            
            index=of.find ("References");
            if (index!=-1)
                OffsetRef=of.left(index).contains('\n')+1;
            else
                OffsetRef=0;
        }
        else
        {
            OffsetSubject=1;
            OffsetFrom=2;
            OffsetDate=3;
            OffsetID=4;
            OffsetRef=5;
            OffsetLines=7;
        }
        sprintf (debugbuf,"Offsets:%d,%d,%d,%d,%d,%d",OffsetSubject,OffsetFrom,OffsetLines,OffsetID,OffsetDate,OffsetRef);
        KDEBUG(KDEBUG_INFO,3300,debugbuf);
    }
    return mReplyCode;
}


int NNTP::setMode (char *mode)
{
    sprintf (mSendBuffer,"mode %s\r\n",mode);
    cout << "C: " << mSendBuffer << endl;

    mReplyCode = -1;
    int bufferLen = strlen(mSendBuffer);
    int numSent = PSend(mSendBuffer, bufferLen);
    if (numSent == bufferLen)
    {
        PGetStatusResponse();
        cout <<"S: " << StatusResponse() << endl;
    }
    return mReplyCode;
}

int NNTP::listXover(int from,int to)
{
    datum key;
    char buffer[1024];
    int counter=0;
    DwString gi;
    reportCounters (true,false);
    from=from >? first;
    to=to <? last;
    if (to)
        sprintf(mSendBuffer, "xover %d-%d\r\n",from,to);
    else
        sprintf(mSendBuffer, "xover %d-\r\n",from);
    
    // for debugging
    cout << "C: " << mSendBuffer << endl;
    
    mReplyCode = -1;
    int bufferLen = strlen(mSendBuffer);
    int numSent = PSend(mSendBuffer, bufferLen);
    if (numSent == bufferLen)
    {
        PGetStatusResponse();
        cout <<"S: " << StatusResponse() << endl;
        if (mReplyCode/100%10 == 2)
        {
            PGetTextResponse();
            
            QString p,qit;
            p=krnpath+GroupName;
            QFile f(p);
            if (!f.open(IO_Append | IO_WriteOnly))
            {
                printf ("Can't open file %s for writing!\n",p.data());
            }
            else
            {
                QString resp=TextResponse().data();
                if (resp.isEmpty())
                {
                    f.close();
                }
                //First break it up in an article list
                class Article art;
                char *tok=strtok(resp.data(),"\n");
                while (tok)
                {
                    QStrList templ;
                    templ.setAutoDelete (true);
                    qit=tok;
                    int index;
                    do
                    {
                        index=qit.find('\t');
                        if (index==-1)
                        {
                            templ.append(qit);
                            break;
                        }
                        templ.append(qit.left(index));
                        qit=qit.right(qit.length()-index-1);
                    }
                    while (!qit.isEmpty());
                    
                    //Write the article ID to the newsgroup file
                    gi+=templ.at(OffsetID);
                    gi+="\n";
                    
                    art.Subject=templ.at(OffsetSubject);
                    art.From=templ.at(OffsetFrom);
                    art.Date=templ.at(OffsetDate);
                    art.Lines=templ.at(OffsetLines);
                    art.ID=templ.at(OffsetID);
                    
                    key.dptr=art.ID.data();
                    key.dsize=art.ID.length()+1;
                    
                    if (!gdbm_exists(artdb,key))
                    {
                        
                        //convert Refs to a strlist
                        art.Refs.clear();
                        QString refsdata=templ.at(OffsetRef);
                        
                        if (!refsdata.isEmpty())
                        {
                            while (1)
                            {
                                int index=refsdata.find(' ');
                                if (index==-1)
                                {
                                    art.Refs.append(refsdata.data());
                                    break;
                                }
                                else
                                {
                                    art.Refs.append(refsdata.left(index));
                                    refsdata=refsdata.right(refsdata.length()-index-1);
                                }
                            }
                        }
                        sprintf (buffer,"Stored %d articles",counter);
                        emit newStatus(buffer);
                        counter++;
                        
                        art.save();
                    }
                    tok=strtok(NULL,"\n");
                }
                f.writeBlock(gi.data(),gi.length());
                f.close();
                gdbm_sync(artdb);
            }
        }
        else
        {
            warning ("Can't get XOVER data from your server");
            warning ("Server said %s",StatusResponse().data());
        }
    }
    resetCounters (true,true);
    return mReplyCode;
}

void NNTP::groupList(QList <NewsGroup> *grouplist, bool fromserver)
{
    reportCounters (true,false);
    grouplist->clear();
    QString ac;
    ac=krnpath+"/active";
    QFile f(ac.data());
    
    QString groups;
    
    if (fromserver)
    {
        int status=List();
        if (status!=215)
        {
            sprintf (debugbuf,"error getting group list\nServer said %s\n",
                     StatusResponse().data());
            KDEBUG (KDEBUG_ERROR,3300,debugbuf);
            grouplist->clear();
            return;
        };
        groups=TextResponse().c_str();
        if (groups.isEmpty())
        {
            grouplist->clear();
            return;
        }
        if(f.open (IO_WriteOnly))
        {
            f.writeBlock(groups.data(),groups.length());
            f.close();
            QString command="sort <";
            command=command+ac+">"+ac+"1; mv "+ac+"1 "+ac;
            system (command.data());
        }
        groupList(grouplist,false);
    }
    else //read it from the active file
    {
        static char buffer[2048]; // I hope no group has over 2000 chars in it's name ;-)
        if(f.open (IO_ReadOnly))
        {
            while (1)
            {
                if (!f.readLine(buffer,2040))
                    break;
                QString s(buffer);
                int t=s.find(' ');
                NewsGroup *gr=new NewsGroup(s.left(t).data());
                grouplist->append(gr);
            };
            f.close();
        }
    };
    resetCounters (true,true);
}

bool NNTP::setGroup(const char *groupname)
{
    bool success=false;
    GroupName=groupname;
    
    int status=Group(GroupName.data());
    
    if (status!=211)
    {
        sprintf (debugbuf,"can't change group!\nserver said: %s\n",
                 StatusResponse().data());
        KDEBUG(KDEBUG_ERROR,3300,debugbuf);
        GroupName="";
    }
    else
    {
        int j;
        QString l=StatusResponse().data();
        j=l.find(' ');
        l=l.right(l.length()-j-1);
        j=l.find(' ');
        howmany=l.left(j).toInt();
        l=l.right(l.length()-j-1);
        
        j=l.find(' ');
        first=l.left(j).toInt();
        l=l.right(l.length()-j-1);
        
        j=l.find(' ');
        last=l.left(j).toInt();
        success=true;
    }
    return success;
}



bool NNTP::artList(int from,int to)
{
    int status=listXover(from,to);
    return (status>199);
}

bool NNTP::isCached (char *id)
{
    QString path=cachepath+"/"+id;
    if (QFile::exists(path.data()))
        return true;
    else
        return false;
}

QString *NNTP::article(char *id)
{
    QString p=cachepath;
    QString *data=new QString("");
    p=p+id;
    QFile f(p.data());
    if (isCached (id))//it exists so it's cached
    {
        if(f.open (IO_ReadOnly))
        {
            char *buffer=new char[f.size()+1];
            f.readBlock(buffer,f.size());
            buffer[f.size()]=0;
            data->setStr(buffer);
            f.close();
        }
    }
    else if(f.open (IO_WriteOnly))//get it, write it and return it
    {
        int status=Article (id);
        if (status==220)
        {
            f.writeBlock(TextResponse().data(),TextResponse().length());
            f.close();
            delete data;
            data=article(id);
        }
        else
        {
            warning ("error getting data\nserver said %s\n",StatusResponse().c_str());
            f.close();
            unlink (p.data());
        }
    }
    return data;
}

bool NNTP::checkStatus( QString start)
{
    return true;
    bool success;
    if (Laststatus.left(start.length())!=start)
    {
        success=false;
    }
    else
        success=true;
    return success;
}

bool NNTP::postArticle (QString ID)
{
    QString p;
    p=krnpath+"/outgoing/";
    p+=ID;
    QFile f(p);
    if (f.open (IO_ReadOnly))
    {
        char *buffer=new char[f.size()+1];
        f.readBlock(buffer,f.size());
        buffer[f.size()]=0;
        int errcode=Post();
        if (errcode!=340)
        {
            sprintf (debugbuf,"error posting, I said post, and the server said:\n%s",
                     StatusResponse().data());
            KDEBUG(KDEBUG_ERROR,3300,debugbuf);
            return false;
        }
        SendData(buffer, f.size());
        SendData("\r\n.\r\n",5);
        f.close();
        unlink (p.data());
        delete buffer;
        return true;
    }
    else
    {
        warning ("Can't open the file I am supposed to post!");
    }
    return false;
}
