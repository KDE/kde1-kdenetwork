#include "kdecode.h"
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#undef Unsorted
#include <qfiledlg.h>
#include <qfile.h>
#include <qstring.h>
#include <qstrlist.h>
#include <kconfig.h>
#include <kapp.h>
#include <uudeview.h>
#include <mimelib/mimepp.h>
#include "ktempfile.h"
#include <kapp.h>

#include "kdecode.moc"

void MsgCallBack(void *,char *, int )
{
}

KDecode::KDecode()
{
    dialog=new decoderDlg (klocale->translate("KRN-Decoder"));

    //XXXXX
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
                KDEBUG (KDEBUG_INFO,3300,"Found file with missing parts");
                formatted="B\n";
            }
            else if (l->state&UUFILE_OK)
            {
                KDEBUG (KDEBUG_INFO,3300,"Found ok file");
                formatted="G\n";
            }
            else if (l->state&UUFILE_NOBEGIN || l->state&UUFILE_NODATA)
            {
                KDEBUG (KDEBUG_INFO,3300,"Found very broken file");
                continue;
            }
            t.setNum(l->size);
            formatted+=t+"\n";
            
            formatted=formatted+l->filename+"\n";
            t.setNum(c);
            formatted=t+"\n"+formatted;
            
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
    decode(line,(char *)0);
}

void KDecode::decode(int line,char *destName)
{
    uulist *l;
    int i=dialog->list->text(line,0).toInt()-1;
    l=UUGetFileListItem(i);
    QString f;
    if (!destName)
        f= QFileDialog::getSaveFileName(0,0,0,l->filename);
    else
    {
        f=destName;
        f+="/";
        f+=l->filename;
    }
    if (!f.isEmpty())
    {
        i=UUDecodeFile (l,f.data());
        switch (i)
        {
        case UURET_OK:
                KDEBUG (KDEBUG_INFO,3300,"decoding ok");
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

DwString* KDecode::decodeString(const char* data, QString type)
{
    type=type.lower();
    DwString idata=data;
    DwString* odata=new DwString;
    if(type=="base64") DwDecodeBase64(idata,*odata);
    else if(type=="quoted-printable") DwDecodeQuotedPrintable(idata,*odata);
    else if(type=="8bit") *odata=idata;
    else if(type=="7bit") *odata=idata;
    else if(type!="7bit")  
    {
        KConfig* conf=kapp->getConfig();
        conf->setGroup("Decoders");
        if(conf->hasKey(type.data()))
        {
            KTempFile tempfile;
            QString plugin=conf->readEntry(type);
            int i=tempfile.create("decode_in","");
            int o=tempfile.create("decode_out","");
            QFile* f=tempfile.file(i);
            f->open(IO_WriteOnly);
            f->writeBlock(idata.c_str(),idata.length());
            f->close();
            system((plugin+" <"+tempfile.file(i)->name()+" >"+
            tempfile.file(o)->name()).data());
            f=tempfile.file(o);
            f->open(IO_ReadOnly);
            char* ndata=(char*)malloc(f->size());
            f->readBlock(ndata,f->size());
            f->close();
            tempfile.remove(i);
            tempfile.remove(o);
            *odata=ndata;
        }
        else
        {

            warning(klocale->translate("Unsupported encoding type: %s."),
                    type.data() );
            *odata=idata;
        }
    }
    return odata;
}          
