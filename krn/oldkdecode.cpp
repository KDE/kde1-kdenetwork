#include "kdecode.h"

#include <uudeview.h>
#include <qfile.h>
#include <stdio.h>
#include <qstring.h>
#include <malloc.h>

KDecode::KDecode()
{
    switch(UUInitialize())
    {
        case UURET_NOMEM : fatal("Could not allocate memory for article decoding");
                           break;
        default          : break;
    }
    
    UUSetMsgCallback(NULL,NULL);
    UUSetBusyCallback(NULL,NULL,0);
    UUSetFileCallback(NULL,NULL);
    UUSetFNameFilter(NULL,NULL);
}

KDecode::~KDecode()
{
    UUCleanUp();
}


QByteArray *KDecode::decode(QString text, QString type)
{
    if(type=="some-type-that-krn-handles-itself")
    {
        //Do stuff here
    }
    else
    {
        QString name(tmpnam(NULL));
    
        QFile temp(name);
        temp.open(IO_WriteOnly);
        temp.writeBlock(text,text.length());
        temp.close();

        switch (UULoadFile(name.data(),NULL,1))
        {
            case UURET_OK   : break;
            case UURET_IOERR: warning("KDecode::decode:Could not write data to be "
                                      "decoded to a temporary file. Bad!");
                              return NULL;
                              break;
            default         : warning("KDecode::decode:The UULib function "
                                      "UULoadFile() failed unexpectedly.");
                              return NULL;
                              break;
        }

        QString dest(tmpnam(NULL));
        switch (UUDecodeFile(UUGetFileListItem(0), dest.data()))
        {
            case UURET_OK: break;
            default      : warning("KDecode::decode:The data could not be decoded!");
                           return NULL;
                           break;
        }   
        switch (UURemoveTemp(UUGetFileListItem(0)))
        {
            case UURET_OK: break;
            default      : warning("KDecode::decode:The temp file could not be deleted!");
                           break;
        }

        QFile temp2(dest);
        temp2.open(IO_ReadOnly);
        char *data= new char[temp2.size()];
        CHECK_PTR(data);
        temp2.readBlock(data,temp2.size());
        temp2.close();
    
        QByteArray *arr=new QByteArray(temp2.size());
        arr->setRawData(data,temp2.size());
        return arr;
    }
}

QDict<QByteArray> KDecode::decode(QList<DwMessage>)
{
    debug("Multiple part decoding is not done yet.");
    return QDict<QByteArray>();
}
