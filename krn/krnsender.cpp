#include "krnsender.h"

#include "krnsender.moc"

extern QString outpath;

void KRNSender::setNNTP(NNTP *_server)
{
    server=_server;
}

bool KRNSender::send(KMMessage *aMsg, short sendnow=-1)
{
    debug ("KRNSender::send");
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
    
    msgStr = prepareStr(msg->asString(), TRUE);
    int errcode=server->Post();
    debug ("post/errcode-->%d",errcode);
    if (!errcode)
    {
        warning ("The server closed the connection!");
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
        success=KMSender::send(aMsg,1);
        if (!success)
        {
            warning ("failed to send by SMTP");
            return success;
        }
    }
    if (!aMsg->groups().isEmpty())//It has destination groups
    {
        doSendNNTP(aMsg);
    }
    return true;
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
