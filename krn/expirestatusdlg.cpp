#include "expirestatusdlg.h"

#include <time.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <gdbm.h>

#include <qlabel.h>
#include <qapp.h>
#include <qdir.h>
#include <qtimer.h>
#include <qstring.h>
#include <kconfig.h>

#include "NNTP.h"

#include "expirestatusdlg.moc"

#define SECOND 1
#define MINUTE SECOND*60
#define HOUR   MINUTE*60
#define DAY    HOUR*24

extern GDBM_FILE artdb;
extern GDBM_FILE old_artdb;
extern KConfig *conf;

extern QString krnpath,cachepath,artinfopath,groupinfopath,dbasepath,outpath;

ExpireStatusDlg::ExpireStatusDlg()
    :QDialog (0,0,true)
{
    QLabel *l=new QLabel ("Please wait a minute, I am expiring!",
                          this);
    l->adjustSize();
    adjustSize();
    QTimer::singleShot(2000,this,SLOT(doExpire()));
    show();
}

void ExpireStatusDlg::doExpire()
{
    conf->setGroup("Cache");
    int expireTime=conf->readNumEntry("ExpireBodies",5);
    
    QDir d(cachepath.data());
    d.setFilter(QDir::Files);
    QStrList *files=new QStrList (*d.entryList("*"));
    
    struct stat st;
    time_t currenttime = time(NULL);
    char filename[255];
    
    for (char *fname=files->first();fname!=0;fname=files->next())
    {
        sprintf(filename, "%s%s", cachepath.data(), fname);
        
        
        if(stat(filename, &st))
        {
            debug("couldn't stat %s", filename);
        } else {
            if((currenttime-st.st_atime) > DAY*expireTime)
            {
                debug(filename);
                Article *art = new Article();
                
                art->ID = fname;
                
                art->load();
                
                if(art->canExpire())
                    unlink(filename);
                
                delete art;
            }
        }
    }
    delete files;
    
    datum key,nextkey;
    datum content;
    int index;
    
    QStrList tl;
    tl.setAutoDelete(true);
    QString t,s;
    
    time_t threshold;
    
    // Expire old articles
    conf->setGroup("Cache");
    threshold=time(NULL)-DAY*conf->readNumEntry("ExpireReadHeaders",5);
    debug ("threshold->%ld",threshold);
    key= gdbm_firstkey(old_artdb);
    while (key.dptr)
    {
        nextkey=gdbm_nextkey (old_artdb,key);
        
        content=gdbm_fetch(old_artdb,key);
        s=(char *)content.dptr;
        
        while (1)
        {
            index=s.find("\n");
            if (index==-1)
            {
                tl.append(s);
                break;
            }
            t=s.left (index);
            s=s.right(s.length()-index-1);
            if (t.isEmpty())
                continue;
            tl.append (t.data());
        }
        
        time_t lastAccess=atol(tl.at(8));
        
        if (threshold>lastAccess)
        {
            debug ("deleting %s",key.dptr);
            gdbm_delete(old_artdb,key);
            free (key.dptr);
        }
        
        tl.clear();
        key=nextkey;
    }
    
    // Expire new articles
    
    conf->setGroup("Cache");
    threshold=time(NULL)-DAY*conf->readNumEntry("ExpireUnreadHeaders",5);
    debug ("threshold->%ld",threshold);
    key= gdbm_firstkey(artdb);
    while (key.dptr)
    {
        nextkey=gdbm_nextkey (artdb,key);
        
        content=gdbm_fetch(artdb,key);
        s=(char *)content.dptr;
        
        while (1)
        {
            index=s.find("\n");
            if (index==-1)
            {
                tl.append(s);
                break;
            }
            t=s.left (index);
            s=s.right(s.length()-index-1);
            if (t.isEmpty())
                continue;
            tl.append (t.data());
        }
        
        time_t lastAccess=atol(tl.at(8));
        
        if (threshold>lastAccess)
        {
            debug ("deleting");
            gdbm_delete(artdb,key);
            free (key.dptr);
        }
        tl.clear();
        key=nextkey;
    }
    qApp->quit();
}

ExpireStatusDlg::~ExpireStatusDlg()
{
}
