// kmsender.cpp

#include "kmsender.h"
#include "kmmessage.h"

#include <kconfig.h>
#include <kapp.h>
#include <kprocess.h>
#include <klocale.h>
#include <kmsgbox.h>

#include <assert.h>
#include <stdio.h>

#define SENDER_GROUP "sending mail"

extern KLocale *nls;
extern QString outpath;

//-----------------------------------------------------------------------------
//
// I do this because opening connections to remote NNTP servers can be very
// slow, and I already have one!
// I know it is against KMSender's design :-(

KMSender::KMSender(NNTP *_nntp)
{
    nntp=_nntp;
}

KMSender::KMSender(KMFolderMgr* /*aFolderMgr*/)
{
}


//-----------------------------------------------------------------------------
KMSender::~KMSender()
{
  if (mMailerProc) delete mMailerProc;
}


//-----------------------------------------------------------------------------
bool KMSender::sendQueued(void)
{
  return FALSE;
}


//-----------------------------------------------------------------------------
bool KMSender::send(KMMessage* aMsg, short sendNow)
{
    //Basically, queing is just placing the message in the
    //outgoing directory, no big deal.

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
    if (nntp)
    {
        if (sendNow)
        {
            if (nntp->postArticle(aMsg->id()))
                return true;
            else
                return false;
        }
    }
    else
    {
        return false;
    }
    return TRUE;
}


//-----------------------------------------------------------------------------
bool KMSender::sendSMTP(KMMessage* msg)
{
  // $markus: I just could not resist implementing smtp suppport
  // This code just must be stable. I checked every darn return code!
  // Date: 24. Sept. 97
  
  QString str;
  int replyCode;
  DwSmtpClient client;
  DwString dwString;
  DwString dwSrc;


  debug("Msg has %i parts\n",msg->numBodyParts());
  // Now we check if message is multipart.
  if(msg->numBodyParts() != 0) // If message is not a simple text message
    {
    }
  else
    {dwSrc = msg->body();
     DwToCrLfEol(dwSrc,dwString); // Convert to CRLF 
    }

  cout << mSmtpHost << endl;
  cout << mSmtpPort << endl;
  client.Open(mSmtpHost,mSmtpPort); // Open connection
  cout << client.Response().c_str();

  /*  printf("before kFailNoF\n");
  if(client.LastFailure != DwProtocolClient::kFailNoFailure)
    {KMsgBox::message(0,"Error opening connection",client.LastFailureStr());
    client.Close(); // Just to make sure
    return false;
    }*/

  replyCode = client.Helo(); // Send HELO command
  if(replyCode != 250 && replyCode != 0)
    {KMsgBox::message(0,"Error!",client.Response().c_str());
    if(client.Close() != 0)
      {KMsgBox::message(0,"Network Error!","Could not close connection to " +
		       mSmtpHost + "!");
      return false;
      }  
    return false;
    }
  else if(replyCode == 0 )
    {KMsgBox::message(0,"Network Error!",client.LastFailureStr());
    return false;
    }
  else  
    cout << client.Response().c_str();

  str = msg->from(); // Check if from is set.
  if(str.isEmpty())
    {KMsgBox::message(0,"?","How could you get this far without a from Field");
    if(client.Close() != 0)
      {KMsgBox::message(0,"Network Error!","Could not close connection to " +
		       mSmtpHost + "!");
      return false;
      }
    return false;
    }

  replyCode = client.Mail(msg->from());
  if(replyCode != 250 && replyCode != 0) // Send MAIL command
     {KMsgBox::message(0,"Error",client.Response().c_str());
     if(client.Close() != 0)
      {KMsgBox::message(0,"Network Error!","Could not close connection to " +
		       mSmtpHost + "!");
      return false;
      }
     return false;
     }
  else if(replyCode == 0 )
    {KMsgBox::message(0,"Network Error!",client.LastFailureStr());
    return false;
    }    
  else
    cout << client.Response().c_str();

  str = msg->to(); // Check if to is set.
  if(str.isEmpty())
    {KMsgBox::message(0,"?","How could you get this far without a to Field");
    if(client.Close() != 0)
      {KMsgBox::message(0,"Network Error!","Could not close connection to " +
		       mSmtpHost + "!");
      return false;
      }
    return false;
    }
  replyCode = client.Rcpt(msg->to()); // Send RCPT command
  if(replyCode != 250 && replyCode != 251 && replyCode != 0)
    {KMsgBox::message(0,"Error",client.Response().c_str());
    if(client.Close() != 0)
      {KMsgBox::message(0,"Network Error!","Could not close connection to " +
		       mSmtpHost + "!");
      return false;
      }
    return false;
    }  
  else if(replyCode == 0 )
    {KMsgBox::message(0,"Network Error!",client.LastFailureStr());
    return false;
    }    
  else
    cout << client.Response().c_str();

  str = msg->cc();
  if(!str.isEmpty())  // Check if cc is set.
    {replyCode = client.Rcpt(msg->cc()); // Send RCPT command
    if(replyCode != 250 && replyCode != 251 && replyCode != 0)
      {KMsgBox::message(0,"Error",client.Response().c_str());
      if(client.Close() !=0 )
	{KMsgBox::message(0,"Network Error!","Could not close connection to " +
		       mSmtpHost + "!");
	return false;
	}
      return false;
      }
    else if(replyCode == 0 )
      {KMsgBox::message(0,"Network Error!",client.LastFailureStr());
      return false;
      }    
    else
      cout << client.Response().c_str();
    }

  str = msg->bcc(); // Check if bcc ist set.
  if(!str.isEmpty())
    {replyCode = client.Rcpt(msg->bcc()); // Send RCPT command
    if(replyCode != 250 && replyCode != 251 && replyCode != 0)
      {KMsgBox::message(0,"Error",client.Response().c_str());
      if(client.Close() != 0)
	{KMsgBox::message(0,"Network Error!","Could not close connection to " +
		       mSmtpHost + "!");
	return false;
	}
      return false;
      }
    else if(replyCode == 0 )
      {KMsgBox::message(0,"Network Error!",client.LastFailureStr());
      return false;
      }    
    else
      cout << client.Response().c_str();
    }

  replyCode = client.Data();
  if(replyCode != 354 && replyCode != 0) // Send DATA command
    {KMsgBox::message(0,"Error!",client.Response().c_str());
    if(client.Close() != 0)
      {KMsgBox::message(0,"Network Error!","Could not close connection to " +
		       mSmtpHost + "!");
      return false;
      }
    return false;
    }
  else if(replyCode == 0 )
    {KMsgBox::message(0,"Network Error!",client.Response().c_str());
    return false;
    }    
  else
    cout << client.Response().c_str();

  replyCode = client.SendData(dwString);
  if(replyCode != 250 && replyCode != 0) // Send data.
    {KMsgBox::message(0,"Error!",client.Response().c_str());
    if(client.Close() != 0 )
      {KMsgBox::message(0,"Network Error!","Could not close connection to " +
		       mSmtpHost + "!");
      return false;
      }
    return false;
    }
  else if(replyCode == 0 )
    {KMsgBox::message(0,"Network Error!",client.LastFailureStr());
    return false;
    }    
  else
    cout << client.Response().c_str();

  replyCode = client.Quit(); // Send QUIT command
  if(replyCode != 221 && replyCode != 0)
    {KMsgBox::message(0,"Error!",client.Response().c_str());
    if(client.Close() != 0 )
      {KMsgBox::message(0,"Network Error!","Could not close connection to " +
		       mSmtpHost + "!");
      return false;
      }
    return false;
    }
  else if(replyCode == 0 )
    {KMsgBox::message(0,"Network Error!",client.LastFailureStr());
    return false;
    }    
  else
    cout << client.Response().c_str();

  return true;

}


//-----------------------------------------------------------------------------
bool KMSender::sendMail(KMMessage* aMsg)
{
  char  msgFileName[1024];
  FILE* msgFile;
  const char* msgstr = aMsg->asString();
  QString sendCmd;

  // write message to temporary file such that mail can
  // process it afterwards easily.
  tmpnam(msgFileName);
  msgFile = fopen(msgFileName, "w");
  fwrite(msgstr, strlen(msgstr), 1, msgFile);
  fclose(msgFile);

  if (!mMailerProc) mMailerProc = new KProcess;

  if (mMailer.isEmpty())
  {
    warning(nls->translate("Please specify a mailer program\nin the settings."));
    return FALSE;
  }

  sendCmd = mMailer.copy();
  sendCmd += " \"";
  sendCmd += aMsg->to();
  sendCmd += "\" < ";
  sendCmd += msgFileName;

  mMailerProc->setExecutable("/bin/sh");
  *mMailerProc << "-c" << (const char*)sendCmd;

  debug("sending message with command: "+sendCmd);
  mMailerProc->start(KProcess::Block);
  debug("sending done");

  unlink(msgFileName);
  return true;
}


//-----------------------------------------------------------------------------
void KMSender::setMethod(Method aMethod)
{
  mMethod = aMethod;
  mCfg->setGroup(SENDER_GROUP);
  mCfg->writeEntry("method", (int)mMethod);
  mCfg->sync();
}


//-----------------------------------------------------------------------------
void KMSender::setSendImmediate(bool aSendImmediate)
{
  mSendImmediate = aSendImmediate;
  mCfg->setGroup(SENDER_GROUP);
  mCfg->writeEntry("immediate", (int)mSendImmediate);
}


//-----------------------------------------------------------------------------
void KMSender::setMailer(const QString& aMailer)
{
  mMailer = aMailer;
  mCfg->setGroup(SENDER_GROUP);
  mCfg->writeEntry("mailer", mMailer);
}


//-----------------------------------------------------------------------------
void KMSender::setSmtpHost(const QString& aSmtpHost)
{
  mSmtpHost = aSmtpHost;
  mCfg->setGroup(SENDER_GROUP);
  mCfg->writeEntry("smtphost", mSmtpHost);
  mCfg->sync();
}


//-----------------------------------------------------------------------------
void KMSender::setSmtpPort(int aSmtpPort)
{
  mSmtpPort = aSmtpPort;
  mCfg->setGroup(SENDER_GROUP);
  mCfg->writeEntry("smtpport", mSmtpPort);
  mCfg->sync();
}
