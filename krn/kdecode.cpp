
#include <qfiledlg.h>
#include <qfile.h>
#include <stdio.h>
#include <qstring.h>
#include <qstrlist.h>
#include <malloc.h>
#include <uudeview.h>
#include "kdecode.h"


void MsgCallBack(void *,char *msg, int )
{
    debug("UULIB message-->%s",msg);
}

KDecode::KDecode()
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
    
    dialog=new decoderDlg ("KRN-Decoder");

    connect (dialog->list,SIGNAL(selected(int,int)),this,SLOT(decode(int,int)));
}

KDecode::~KDecode()
{
    UUCleanUp();
    delete dialog;
}


void KDecode::load(char *filename)
{

    UULoadFile(filename,NULL,0);
    char *fname=new char [strlen(filename)];
    strcpy (fname,filename);
    debug ("adding %s",fname);
}

void KDecode::showWindow()
{
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
    dialog->show();
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


