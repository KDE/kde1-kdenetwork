#include "kdecode.h"
#include <malloc.h>
#include <stdio.h>
#undef Unsorted
#include <qfiledlg.h>
#include <qfile.h>
#include <qstring.h>
#include <qstrlist.h>
#include <Kconfig.h>
#include <kapp.h>
#include <uudeview.h>
#include <mimelib/mimepp.h>
#include "ktempfile.h"

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
                formatted="B\n";
            }
            else if (l->state&UUFILE_OK)
            {
                debug ("Found ok file");
                formatted="G\n";
            }
            else if (l->state&UUFILE_NOBEGIN || l->state&UUFILE_NODATA)
            {
                debug ("Found very broken file");
                continue;
            }
            debug ("size is %ld",l->size);
            t.setNum(l->size);
            formatted+=t+"\n";
            
            debug ("called %s",l->filename);
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
    uulist *l;
    int i=dialog->list->text(line,0).toInt()-1;
    debug ("Decoding ID-->%d",i);
    l=UUGetFileListItem(i);
    QString f= QFileDialog::getSaveFileName(0,0,0,l->filename);
    if (!f.isEmpty())
    {
        debug ("saving to-->%s",f.data());
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
    debug("decoding %s",type.data());
    if(type=="base64") DwDecodeBase64(data,data);
    else if(type=="quoted-printable") DwDecodeQuotedPrintable(data,data);
    else if(type=="8bit") warning("Raw 8 bit data read. Thins may look strange");
    else if(type!="7bit")
    {
        KConfig* conf=kapp->getConfig();
        conf->setGroup("Decoders");
        if(conf->hasKey(type.data()))
        {
            KTempFile tempfile;
            QString plugin=conf->readEntry(type);
            debug("Plug-in found: %s",plugin.data());
            int i=tempfile.create("decode_in","");
            int o=tempfile.create("decode_out","");
            QFile* f=tempfile.file(i);
            f->open(IO_WriteOnly);
            f->writeBlock(data,strlen(data));
            f->close();
            system(plugin+" <"+tempfile.file(i)->name()+" >"+
            tempfile.file(o)->name());
            f=tempfile.file(o);
            f->open(IO_ReadOnly);
            char* ndata=(char*)malloc(f->size());
            f->readBlock(ndata,f->size());
            f->close();
            tempfile.remove(i);
            tempfile.remove(o);
            return ndata;
        }
                                                                                                                                                                                                                                                                                                                 
        warning("KDecode::decodeString(): Unsupported encoding type: %s.",
                type.data() );
        return NULL;
    }
    debug("decoded data: %s",data);
    return data;
}
