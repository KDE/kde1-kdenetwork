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

extern QString krnpath,cachepath,artinfopath;

#include <mimelib/mimepp.h>


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
    if (client->reportBytes && (client->byteCounter - oldbytes)>1024 )
    {
        char *buffer=new char[100];
        sprintf (buffer,klocale->translate("Received %d bytes"),client->byteCounter);
        emit client->newStatus(buffer);
        delete[] buffer;
        qApp->processEvents();
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
    debug (hostname);
    reportBytes=false;
    reportCommands=false;
    byteCounter=0;
    commandCounter=0;
    extendPartialResponse=new NNTPObserver (this);
    SetObserver(extendPartialResponse);
    
}

void NNTP::PGetTextResponse()
{
    debug ("entered NNTP::PGetTextResponse()");
    partialResponse="";
    qApp->processEvents();
    SetObserver(extendPartialResponse);
    DwNntpClient::PGetTextResponse();
    mTextResponse=qstrdup(partialResponse.c_str());
    partialResponse="";
    SetObserver(NULL);
    debug ("exited NNTP::PGetTextResponse()");
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
        debug ("Connecting to %s",hostname.data());
        status=Open(hostname.data());
        debug ("status-->%d",status);
    }
    if (status!=200 && status!=201)
        return false;
    if (status==201)
        Readonly=true;
    
    Connected=true;
    
    //this is needed sometimes for some versions of INN
    status=setMode("reader");
    debug ("status-->%d",status);

    status=listOverview();
    debug ("status-->%d",status);

    return Connected;
}
bool NNTP::reConnect()
{
    return true;
}
bool NNTP::disconnect()
{
    int status=Quit();
    debug ("status-->%d",status);
    status=Close();
    debug ("status-->%d",status);
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
        debug ("Offsets:%d,%d,%d,%d,%d,%d",OffsetSubject,OffsetFrom,OffsetLines,OffsetID,OffsetDate,OffsetRef);
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
                    f.writeBlock(templ.at(OffsetID),strlen(templ.at(OffsetID)));
                    f.writeBlock("\n",1);
                    
                    class Article art;
                    art.Subject=templ.at(OffsetSubject);
                    art.From=templ.at(OffsetFrom);
                    art.Date=templ.at(OffsetDate);
                    art.Lines=templ.at(OffsetLines);
                    art.ID=templ.at(OffsetID);
                    //convert Refs to a strlist
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
                    art.save();
                    tok=strtok(NULL,"\n");
                }
                f.close();
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
        debug ("status-->%d",status);
        if (status!=215)
        {
            printf ("error getting group list\nServer said %s\n",StatusResponse().data());
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
    debug ("status-->%d",status);
    
    if (status!=211)
    {
        printf ("can't change group!\n");
        printf ("server said: %s\n",StatusResponse().data());
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
    debug ("status-->%d",status);
    return true;
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
    QString *data=new QString(klocale->translate("\n\nError reading Article!"));
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
            data->setStr(TextResponse().data());
            f.writeBlock(data->data(),data->length());
            f.close();
            return article(id);
        }
        else
        {
            warning ("error getting data\nserver said %s\n",StatusResponse().data());
            f.close();
            unlink (p.data());
            return new QString("");
        }
    }
    commandCounter++;
    char *buffer=new char[100];
    sprintf (buffer,klocale->translate("received %d article(s)"),commandCounter);
    emit newStatus(buffer);
    delete buffer;
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
    debug ("sending now");
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
            debug ("error posting, I said post, and the server said:\n%s",
                   StatusResponse().data());
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
