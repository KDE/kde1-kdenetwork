
#include <qfiledlg.h>
#include <qfile.h>
#include <stdio.h>
#include <qstring.h>
#include <qstrlist.h>
#include <malloc.h>
#include <uudeview.h>
#include "kdecode.h"
#include <mimelib/string.h>
#include <mimelib/mimepp.h>

void MsgCallBack(void *,char *msg, int )
{
    debug("UULIB message-->%s",msg);
}

KDecode::KDecode()
{
    dialog=new decoderDlg ("KRN-Decoder");

    connect (dialog->list,SIGNAL(selected(int,int)),this,SLOT(decode(int,int)));
}

KDecode::~KDecode()
{
    delete dialog;
}


void KDecode::load(char *filename)
{

    filenames.append (filename);
    
}

void KDecode::showWindow()
{

    switch(UUInitialize())
    {
    case UURET_NOMEM :
        fatal("Could not allocate memory for article decoding");
        break;
    default:
        break;
    } 

    UUSetMsgCallback(NULL,MsgCallBack);
    UUSetBusyCallback(NULL,NULL,0);
    UUSetFileCallback(NULL,NULL);
    UUSetFNameFilter(NULL,NULL);

    UUSetOption (UUOPT_DESPERATE,1);

    for (char *iter=filenames.first();iter!=0;iter=filenames.next())
    {
        UULoadFile(iter,NULL,0);
    }

    

    dialog->list->clear();
    int c=0;
    uulist *l;
    bool flag=false;
    while (1)
    {
        QString t;
        l=UUGetFileListItem(c);
        if (l)
        {
            c++;
            flag=true;
            QString formatted;
            
            if (l->state&UUFILE_MISPART || l->state&UUFILE_NOEND)
            {
                debug ("Found file with missing parts");
                formatted="B\t";
            }
            else if (l->state&UUFILE_OK)
            {
                debug ("Found ok file");
                formatted="G\t";
            }
            else if (l->state&UUFILE_NOBEGIN || l->state&UUFILE_NODATA)
            {
                debug ("Found very broken file");
                continue;
            }
            debug ("size is %ld",l->size);
            t.setNum(l->size);
            formatted+=t+"\t";
            
            debug ("called %s",l->filename);
            formatted=formatted+l->filename+"\t";
            t.setNum(c);
            formatted=t+"\t"+formatted;
            
            dialog->list->insertItem(formatted.data());
        }
        else
            break;
    }
    qApp->setOverrideCursor(arrowCursor);
    dialog->exec();
    qApp->restoreOverrideCursor();

    UUCleanUp();
    filenames.clear();
}

void KDecode::decode(int line,int)
{
    KTabListBoxItem *item=dialog->list->getItem(line);
    uulist *l;
    int i=QString(item->text(0)).toInt()-1;
    debug ("Decoding ID-->%d",i);
    l=UUGetFileListItem(i);
    QString f= QFileDialog::getSaveFileName(0,0,0,l->filename);
    debug ("saving to-->%s",f.data());
    if (!f.isEmpty())
    {
        i=UUDecodeFile (l,f.data());
        debug ("return code-->%d",i);
        switch (i)
        {
        case UURET_OK:
            debug ("decoding ok");
            break;
        case UURET_IOERR:
            warning ("IO error while decoding");
            warning(strerror(UUGetOption(UUOPT_ERRNO,NULL,NULL,0)));
            break;
        case UURET_NOMEM:
            warning ("run out of memory");
            break;
        case UURET_NODATA:
            warning ("No data in file");
            break;
                                                                                                                                            
        case UURET_NOEND:
            warning ("No end of file found");
            break;
        case UURET_EXISTS:
            warning ("File already exists");
            break;
        }
    }
}

const char* KDecode::decodeString(const char* data, QString type)
{
    type=type.lower();
    if(type=="base64") DwDecodeBase64(data,data);
    else if(type=="quoted-printable") DwDecodeQuotedPrintable(data,data);
    else if(type!="7bit")
    {
        warning("%s: Unsupported encoding type: %s",
                __FUNCTION__,type.data());
        return NULL;
    }
    return data;
}
