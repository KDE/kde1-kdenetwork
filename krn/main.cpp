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
#include <qdict.h>

#include <kapp.h>
#include <kconfig.h>
#include <kkeyconf.h>
#include <kstdaccel.h> 
#include <kmsgbox.h>

#include "rules.h"
#include "asker.h"
#include "krnsender.h"
#include "kmidentity.h"
#include "kbusyptr.h"
#include "kmmessage.h"
#include "kmaddrbook.h"
#include "kfileio.h"
#include "groupdlg.h" 
#include "artdlg.h"
#include "expirestatusdlg.h"
#include "NNTP.h"
#include <mimelib/mimepp.h>

#include <gdbm.h>

// robert's cache stuff
#include <dirent.h>
#include <time.h>
#include <stdlib.h>


KApplication *app;
Groupdlg  *main_widget;
KConfig *conf;
KLocale *nls;
KRNSender *msgSender;
KMIdentity *identity;
KStdAccel* keys;
KBusyPtr *kbp;
KMAddrBook *addrBook;
QDict <char> unreadDict(17,TRUE);
QDict <NewsGroup> groupDict(17,TRUE);

QList <Rule> ruleList;
KSimpleConfig *ruleFile=0;

QString krnpath,cachepath,artinfopath,groupinfopath,dbasepath,outpath;

GDBM_FILE artdb;
GDBM_FILE refsdb;
GDBM_FILE scoredb;

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
    msgSender->readConfig();
    msgSender->setMethod(KMSender::smSMTP);
    KMMessage::readConfig();

    addrBook=new KMAddrBook();
    addrBook->readConfig();
    addrBook->load();

    
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
    outpath=krnpath+"outgoing/";
    mkdir (outpath.data(),S_IREAD|S_IWRITE|S_IEXEC);

    //Check for a lock file before I break things
    if (QFile::exists(krnpath+"krn_lock"))
    {
        int i=KMsgBox::yesNo(0,"KRN - Error",
                             "I have detected another Krn running\n"
                             "Do you REALLY want to continue?\n"
                             "If you are sure there isn't one, press \"Yes\"\n"
                             "But if there *is* another one, it's going to be UGLY\n");
        if (i!=1) exit(1);
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
    artinfopath=krnpath+"/refs.db";
    refsdb=gdbm_open(artinfopath.data(),0,GDBM_WRCREAT | GDBM_FAST,448,0);
    artinfopath=krnpath+"/scores.db";
    scoredb=gdbm_open(artinfopath.data(),0,GDBM_WRCREAT | GDBM_FAST,448,0);

    // Fill the unreadDict
    datum key=gdbm_firstkey ( artdb );
    datum nextkey;


    while ( key.dptr )
    {
        unreadDict.insert(key.dptr,key.dptr);
        nextkey = gdbm_nextkey ( artdb, key );
        free (key.dptr);
        key = nextkey;
    };

    // Load the rules
    ruleFile=new KSimpleConfig(krnpath+"/rules");
    Rule::updateGlobals();


    Groupdlg k;
    main_widget = &k;
    
    a.setMainWidget( &k );
    
    k.setMinimumSize( 250, 250 );
    k.show();

    a.exec();

    
    expireCache();

    k.close();
    gdbm_sync(artdb);
    gdbm_reorganize(artdb);
    gdbm_sync(artdb);
    gdbm_close(artdb);
    unlink((krnpath+"krn_lock").data());
    exit(0);
}

void checkConf()
// This checks that all necessary data exists and/or asks for it
{

    identity=new KMIdentity();
    identity->setSignatureFile(QString(getenv("HOME"))+"/.signature");
    
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
        if (data.isEmpty())
            data="-";
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

    main_widget->hide();
    QDictIterator <NewsGroup> it(groupDict);
    
    NewsGroup *iter;

    for (it.toFirst();it.current(); ++it)
    {
        iter=it.current();
        if (iter->isVisible)
            iter->isVisible->hide();
    }
    ExpireStatusDlg *dlg=new ExpireStatusDlg();
}


