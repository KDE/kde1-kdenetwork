/**********************************************************************
 
 The IO DCC Window

 $$Id$$

 Display DCC progress, etc.  This in the future should be expanded.

**********************************************************************/
 

#include "ioDCC.h"
#include <qregexp.h> 
#include <iostream.h>

KSircIODCC::~KSircIODCC()
{
}

void KSircIODCC::sirc_receive(QString str)
{
  // Parse the string to find out what type it is.
  //  cerr << "Got: " << str << endl;
  if(str.find("DCC SEND (", 0) != -1){
    cerr << "Startig new dialog\n";
    int pos1 = str.find("SEND") + 6;
    int pos2 = str.find(")", pos1);
    if((pos1 < 0) || (pos2 < 0)){
      cerr << "FILENAME Pos1/Pos2 " << pos1 << '/' << pos2 << endl;
      return;
    }
    QString filename = str.mid(pos1, pos2 - pos1);
    pos1 = str.find("(size: ") + 7;
    pos2 = str.find(")", pos1);
    if((pos1 < 0) || (pos2 < 0)){
      cerr << "SIZE Pos1/Pos2 " << pos1 << '/' << pos2 << endl;
      return;
    }
    QString size = str.mid(pos1, pos2-pos1);
    int fileSize = size.toInt(); // Bytes per step
    if(fileSize >= 100){
      fileSize /= 100;
    }
    else{
      fileSize = 1;
    }
    pos1 = str.find(" from ") + 6;
    pos2 = str.find(" ", pos1);
    if((pos1 < 0) || (pos2 < 0)){
      cerr << "FROM Pos1/Pos2 " << pos1 << '/' << pos2 << endl;
      return;
    }
    QString nick = str.mid(pos1, pos2 - pos1);
    DlgList.insert(filename, new KSProgress());
    DlgList[filename]->setID(filename);
    DlgList[filename]->setRange(0,100);
    DlgList[filename]->setTopText("DCC Receving: " + filename);
    DlgList[filename]->setBotText("Size: " + size);
    connect(DlgList[filename], SIGNAL(cancel(QString)),
	    this, SLOT(cancelTransfer(QString)));

    //    "DCC File Xfer: " +
    //      filename,
    //      "Cancel",
    //      100));
//    DlgList[filename]->setProgress(0);
    DCCInfo *stat = new DCCInfo;
    stat->LastSize = 0;
    stat->PercentSize = fileSize;
    stat->nick = nick;
    DCCStatus.insert(filename, stat);
    pending->fileListing()->insertItem(nick + " offered " + filename);
    pending->fileListing()->setCurrentItem(pending->fileListing()->count()-1);
    if(pending->isVisible() == FALSE)
      pending->show();
  }
  else if(str.find("DCC GET read:", 0) != -1){
    //    cerr << "Startig dialog update\n";
    int pos1 = str.find("read: ") + 6;
    int pos2 = str.find(" ", pos1);
    if((pos1 < 0) || (pos2 < 0))
      return;
    QString filename = str.mid(pos1, pos2 - pos1);
    pos1 = str.find("bytes: ") + 7;
    pos2 = str.length();
    if((pos1 < 0) || (pos2 < 0))
      return;
    int bytesXfer = str.mid(pos1, pos2-pos1).toInt();
    DCCInfo *stat = DCCStatus[filename];
    ASSERT(stat->PercentSize > 0); // We devide by this!
    ASSERT(bytesXfer > 0); // Setting progress back to 0 screws it up
    if(bytesXfer > (stat->LastSize + stat->PercentSize)){
      DlgList[filename]->setValue(bytesXfer/(stat->PercentSize));
      stat->LastSize = bytesXfer;
    }
  }
  else if(str.find("DCC transfer with") != -1){
    int pos1 = str.find(" (", 0)+2;
    int pos2 = str.find(")", pos1);
    QString filename = str.mid(pos1, pos2-pos1);
    delete DlgList[filename];
    DlgList.remove(filename);
    delete DCCStatus[filename];
    DCCStatus.remove(filename);
  }
  else{
    proc->getWindowList()["!default"]->sirc_receive(str);
  }
  
}

void KSircIODCC::control_message(QString)
{
}

void KSircIODCC::cancelTransfer(QString filename)
{
  if(DlgList[filename]){
    emit outputLine("/dcc close get " + DCCStatus[filename]->nick + " " + filename + "\n");
    delete DlgList[filename];
    DlgList.remove(filename);
    delete DCCStatus[filename];
    DCCStatus.remove(filename);
  }
}

void KSircIODCC::getFile()
{
  QString text = pending->fileListing()->text(pending->fileListing()->currentItem());
  int pos = text.find(" ", 0);
  QString nick = text.mid(0, pos);
  pos = text.find(" ", pos+1) + 1;
  QString filename = text.mid(pos, text.length() - pos);
  if(DlgList[filename]->isVisible() == FALSE)
    DlgList[filename]->show();
  emit outputLine("/dcc get " + nick + " " + filename + "\n");
  for(uint i = 0; i < pending->fileListing()->count(); i++)
    if(QString(pending->fileListing()->text(i)) == (nick + " offered " + filename))
      pending->fileListing()->removeItem(i);

  if(pending->fileListing()->count() == 0)
     pending->hide();
}

void KSircIODCC::forgetFile()
{
  QString text = pending->fileListing()->text(pending->fileListing()->currentItem());
  int pos = text.find(" ", 0);
  QString nick = text.mid(0, pos);
  pos = text.find(" ", pos+1) + 1;
  QString filename = text.mid(pos, text.length() - pos);
  emit outputLine("/dcc close get " + nick + " " + filename + "\n");
  for(uint i = 0; i < pending->fileListing()->count(); i++)
    if(QString(pending->fileListing()->text(i)) == (nick + " offered " + filename))
      pending->fileListing()->removeItem(i);

  if(pending->fileListing()->count() == 0)
     pending->hide();

}
