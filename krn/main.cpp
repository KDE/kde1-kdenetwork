//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// This file os part of KRN, a newsreader for the KDE project.              //
// KRN is distributed under the GNU General Public License.                 //
// Read the acompanying file COPYING for more info.                         //
//                                                                          //
// KRN wouldn't be possible without these libraries, whose authors have     //
// made free to use on non-commercial software:                             //
//                                                                          //
// MIME++ by Doug Sauder                                                    //
// Qt     by Troll Tech                                                     //
//                                                                          //
// This file is copyright 1997 by                                           //
// Roberto Alsina <ralsina@unl.edu.ar>                                      //
// Magnus Reftel  <d96reftl@dtek.chalmers.se>                               //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <pwd.h>

#include <qapp.h>
#include <qfile.h>
#include <qdir.h>

#include <kapp.h>
#include <kconfig.h>
#include <kkeyconf.h>
#include <kstdaccel.h> 
#include <kmsgbox.h>

#include "asker.h"
#include "groupdlg.h" 
#include "NNTP.h"
#include "kdecode.h"
#include "krnsender.h"
#include "kmidentity.h"
#include "kbusyptr.h"
#include "kmmessage.h"
#include <mimelib/mimepp.h>

#include <gdbm.h>

// robert's cache stuff
#include <dirent.h>
#include <time.h>
#include <stdlib.h>

#define SECOND 1
#define MINUTE SECOND*60
#define HOUR   MINUTE*60
#define DAY    HOUR*24

KApplication *app;
Groupdlg  *main_widget;
KConfig *conf;
KLocale *nls;
KRNSender *msgSender;
KMIdentity *identity;
KStdAccel* keys;
KBusyPtr *kbp;

ArticleDict artSpool;

QString krnpath,cachepath,artinfopath,groupinfopath,pixpath,dbasepath,outpath;

KDecode *decoder;

GDBM_FILE artdb;
GDBM_FILE old_artdb;

void checkConf();
void expireCache();

void testDir( const char *_name )
{
    DIR *dp;
    QString c = getenv( "HOME" );
    c += _name;
    dp = opendir( c.data() );
    if ( dp == NULL )
	::mkdir( c.data(), S_IRWXU );
    else
	closedir( dp );
}

int main( int argc, char **argv )
{
    msgSender=0;
    // Initialize the mime++ library
    DwInitialize();

    KApplication a( argc, argv, "krn" );
    a.enableSessionManagement();

    app=&a;
    conf=a.getConfig();

    checkConf();
    nls=a.getLocale();
    keys = new KStdAccel(conf);
    kbp=new KBusyPtr();
    msgSender=new KRNSender();
    msgSender->setMethod(KMSender::smSMTP);
    KMMessage::readConfig();

    QString pixdef=a.kdedir() + QString("/lib/pics/krn/");
    conf->setGroup("KDE Setup");
    if(conf->hasKey("pixpath")) pixpath=conf->readEntry("pixpath",pixdef);
    else pixpath=pixdef;

    decoder=new KDecode;
    
    // Create our directory. If it exists, no problem
    // Should do some checking, though

    testDir( "/.kde" );
    testDir( "/.kde/share" );    
    testDir( "/.kde/share/config" );
    testDir( "/.kde/share/apps" );
    testDir( "/.kde/share/apps/krn" );
    testDir( "/.kde/share/apps/krn/cache" );    
    testDir( "/.kde/share/apps/krn/groupinfo" );    
    testDir( "/.kde/share/apps/krn/outgoing" );    

    QString c = getenv ("HOME");
    krnpath=c+"/.kde/share/apps/krn/";
    mkdir (krnpath.data(),S_IREAD|S_IWRITE|S_IEXEC);
    cachepath=krnpath+"/cache/";
    mkdir (cachepath.data(),S_IREAD|S_IWRITE|S_IEXEC);
    groupinfopath=krnpath+"/groupinfo/";
    mkdir (groupinfopath.data(),S_IREAD|S_IWRITE|S_IEXEC);
    outpath=krnpath+"/outgoing/";
    mkdir (outpath.data(),S_IREAD|S_IWRITE|S_IEXEC);

    //Check for a lock file before I break things
    if (QFile::exists(krnpath+"krn_lock"))
    {
        KMsgBox::message(0,"KRN - Error",
                         "I have detected another Krn running\n"
                         "If you are sure there isn't one\n"
                         "remove ~/.kde/share/apps/krn/krn_lock\n"
                         "and restart Krn");
        exit(1);
    }
    else
    {
        QFile lock(krnpath+"krn_lock");
        lock.open(IO_WriteOnly);
        lock.writeBlock("locked",7);
        lock.close();
    }
    
    // Create the articles database

    artinfopath=krnpath+"/artinfo.db";
    artdb=gdbm_open(artinfopath.data(),0,GDBM_WRCREAT | GDBM_FAST,448,0);
    artinfopath=krnpath+"/old_artinfo.db";
    old_artdb=gdbm_open(artinfopath.data(),0,GDBM_WRCREAT | GDBM_FAST,448,0);
    
    Groupdlg k;
    main_widget = &k;
    
    a.setMainWidget( (QWidget *) &k );
    
    k.setMinimumSize( 250, 250 );
    k.show();
    
    a.exec();
    expireCache();

    debug ("flag0");
    gdbm_reorganize(artdb);
    debug ("flag1");
    gdbm_reorganize(old_artdb);
    debug ("flag2");
    gdbm_close(artdb);
    debug ("flag3");
    gdbm_close(old_artdb);
    debug ("flag4");
    unlink((krnpath+"krn_lock").data());
}

void checkConf()
// This checks that all necessary data exists and/or asks for it
{

    identity=new KMIdentity();
    
    Asker ask;
    QString data;
    conf->setGroup("Identity");
    
    data=conf->readEntry("Email Address");
    if (data.isEmpty())
    {
        ask.setCaption (klocale->translate("KRN-Missing Configuration info"));
        ask.label->setText(klocale->translate("Please enter your email adress"));
        ask.entry->setText(identity->emailAddr());
        ask.exec();
        data=ask.entry->text();
        identity->setEmailAddr(data);
    }


    data=conf->readEntry("Name");
    if (data.isEmpty())
    {
        ask.setCaption (klocale->translate("KRN-Missing Configuration info"));
        ask.label->setText(klocale->translate("Please enter your real name"));
        ask.entry->setText(identity->fullName());
        ask.exec();
        data=ask.entry->text();
        identity->setFullName(data);
    }


    data=identity->organization();
    if (data.isEmpty())
    {
        ask.setCaption (klocale->translate("KRN-Missing Configuration info"));
        ask.label->setText(klocale->translate("Please enter your organization's name"));
        ask.entry->setText(getenv("ORGANIZATION"));
        ask.exec();
        data=ask.entry->text();
        conf->writeEntry("Organization",data);

        identity->setOrganization(data);
    }

    conf->setGroup("NNTP");
    data=conf->readEntry("NNTPServer");
    if (data.isEmpty())
    {
        ask.setCaption (klocale->translate("KRN-Missing Configuration info"));
        ask.label->setText(klocale->translate("Please enter your NNTP server name"));
        ask.entry->setText(getenv("NNTPSERVER"));
        ask.exec();
        data=ask.entry->text();
        conf->writeEntry("NNTPServer",data);
    }

    conf->setGroup("sending mail");
    data=conf->readEntry("Smtp Host");
    if (data.isEmpty())
    {
        ask.setCaption (klocale->translate("KRN-Missing Configuration info"));
        ask.label->setText(klocale->translate("Please enter your SMTP server name"));
        QString add=identity->emailAddr();
        add=add.right(add.length()-add.find('@')-1);
        ask.entry->setText(add.data());
        ask.exec();
        data=ask.entry->text();
        conf->writeEntry("Smtp Host",data);
    }

    conf->sync();
    identity->writeConfig();
}

void expireCache()   // robert's cache stuff
{
    
    conf->setGroup("Cache");
    int expireTime=conf->readNumEntry("ExpireDays",5);
    
    QDir d(cachepath.data());
    QStrList files=d.entryList();
    
    struct stat st;
    time_t currenttime = time(NULL);
    char filename[255];
    
    for (char *fname=files.first();fname!=0;fname=files.next())
    {
        sprintf(filename, "%s%s", cachepath.data(), fname);
        
        debug(filename);
        
        if(stat(filename, &st))
        {
            debug("couldn't stat %s", filename);
        } else {
            if(((currenttime-st.st_atime) > DAY*expireTime) && !strcmp(fname, "."))
            {
                Article *art = new Article();
                
                art->ID = fname;
                
                art->load();
                
                if(art->canExpire())
                    unlink(filename);
                
                delete art;
            }
        }
    }
}


