#include "krnsender.h"

#include <unistd.h>
#include <kmsgbox.h>
#include <kapp.h>

#include "kfileio.h"

#include "krnsender.moc"

extern QString outpath;

void KRNSender::setNNTP(NNTP *_server)
{
    server=_server;
    setMethod(smSMTP);
}

bool KRNSender::send(KMMessage *aMsg, short sendnow=-1)
{
    readConfig();
    debug ("KRNSender::send");
    aMsg->cleanupHeader();
    if (sendnow==-1 && server->isConnected())
        sendnow=1;
    else if (sendnow==-1)
        sendnow=0;

    if (sendnow)
        return sendNow(aMsg);
    else
        return queue(aMsg);
}

bool KRNSender::doSendNNTP (KMMessage *msg)
{
    QString str, msgStr;
    assert(msg != NULL);

    //Empty bodies make news servers and readers go insane
    if (msg->body().isEmpty())
        msg->setBody("\n\n\n");
    
    msgStr = msg->asString();
    if (server->isReadOnly())
    {
        KMsgBox::message(0,"KRN - Error",
                         klocale->translate("This server is read only"
                                            "you can't post here!"));
        return false;
    }
    int errcode=server->myPost();
    debug ("post/errcode-->%d",errcode);
    if (!errcode)
    {
        warning ("The server closed the connection!");
        return false;
    }
    if (errcode!=340)
    {
        warning("error posting, I said POST, and the server said:\n%s",
                 server->StatusResponse().data());
        return false;
    }
    errcode=server->SendData((const char *)msgStr);
    debug ("senddata/errcode-->%d",errcode);
    if (!errcode)
    {
        warning ("The server closed the connection!");
        return false;
    }
    if (errcode>240)
    {
        warning("error posting, I said DATA, and the server said:\n%s",
                 server->StatusResponse().data());
        return false;
    }
    return true;
}

bool KRNSender::sendNow(KMMessage *aMsg)
{
    bool success=false;
    debug ("sending now!!!");
    if ((!aMsg->to().isEmpty()) ||
        (!aMsg->cc().isEmpty()) ||
        (!aMsg->bcc().isEmpty())) //It has an email recipient
    {
        debug ("sending by SMTP");
        KMSendSMTP *realSender = new KMSendSMTP(this);
        success=realSender->start();
        if (!success)
        {
            warning ("failed to send by SMTP");
            return success;
        }
        success=realSender->send(aMsg);
        if (!success)
        {
            warning ("failed to send by SMTP");
            return success;
        }
        success=realSender->finish();
        if (!success)
        {
            warning ("failed to send by SMTP");
            return success;
        }
        delete realSender;
    }
    if (!aMsg->groups().isEmpty())//It has destination groups
    {
        success=doSendNNTP(aMsg);
    }
    return success;
}

bool KRNSender::queue(KMMessage *aMsg)
{
    debug ("queuing now!!!");
    QFile f(outpath+aMsg->id());
    if (f.exists())
    {
        warning ("There exists another message with this ID!\n That shouldn't happen!!!");
        return FALSE;
    }
    if (!f.open (IO_WriteOnly))
    {
        warning ("I can't open for writing, so I can't spool this thing");
        return FALSE;
    }
    debug ("Spooling the message");
    f.writeBlock(aMsg->asString(),strlen(aMsg->asString()));
    f.close();
    
    return true;
}


bool KRNSender::sendQueued(const char *fname)
{
    bool success=false;
    KMMessage *mm=new KMMessage;
    mm->fromString(kFileToString(outpath+fname,TRUE,TRUE));
    success=sendNow(mm);
    delete mm;
    unlink(QString(outpath+fname).data());
    return success;
}
