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


#include <qapp.h>

#include <kapp.h>
#include <kiconloader.h>
#include <Kconfig.h>

#include "asker.h"
#include "groupdlg.h" 
#include "newsgroup.h"
#include "kdecode.h"
#include <mimelib/mimepp.h>



Groupdlg  *main_widget;
KConfig *conf;

KIconLoader *iconloader;

ArticleDict artSpool;

QString krnpath,cachepath,artinfopath,groupinfopath,pixpath;

KDecode *decoder;

void checkConf();

int main( int argc, char **argv )
{
    // Initialize the mime++ library
    DwInitialize();

    KApplication a( argc, argv, "krn" );

    conf=a.getConfig();

    checkConf();

    QString pixdef=a.kdedir() + QString("/lib/pics/krn/");
    conf->setGroup("KDE Setup");
    if(conf->hasKey("pixpath")) pixpath=conf->readEntry("pixpath",&pixdef);
    else pixpath=pixdef;

    decoder=new KDecode;
    iconloader=new KIconLoader ();
    
    // Create our directory. If it exists, no problem
    // Should do some checking, though
    
    QString c=getenv ("HOME");
    c=c+"/.kde";
    mkdir (c.data(),S_IREAD|S_IWRITE|S_IEXEC);
    krnpath=c+"/krn/";
    mkdir (krnpath.data(),S_IREAD|S_IWRITE|S_IEXEC);
    artinfopath=krnpath+"/artinfo/";
    mkdir (artinfopath.data(),S_IREAD|S_IWRITE|S_IEXEC);
    cachepath=krnpath+"/cache/";
    mkdir (cachepath.data(),S_IREAD|S_IWRITE|S_IEXEC);
    groupinfopath=krnpath+"/groupinfo/";
    mkdir (groupinfopath.data(),S_IREAD|S_IWRITE|S_IEXEC);

    Groupdlg k;
    main_widget = &k;
    
    
    a.setMainWidget( (QWidget *) &k );
    
    k.setMinimumSize( 250, 250 );
    k.setCaption( "KRN 0.0.3 - Group list" );
    
    k.show();
    
    return a.exec();
}

void checkConf()
// This checks that all necessary data exists and/or asks for it
// Should use Stefan's KIdentity someday
{
    Asker ask;
    QString data;
    conf->setGroup("Identity");
    
    data=conf->readEntry("Address");
    if (data.isEmpty())
    {
        ask.setCaption ("KRN-Missing Configuration info");
        ask.label->setText("Please enter your email adress");
        ask.entry->setText("");
        ask.exec();
        data=ask.entry->text();
        conf->writeEntry("Address",data);
    }


    data=conf->readEntry("RealName");
    if (data.isEmpty())
    {
        ask.setCaption ("KRN-Missing Configuration info");
        ask.label->setText("Please enter your real name");
        ask.entry->setText("");
        ask.exec();
        data=ask.entry->text();
        conf->writeEntry("RealName",data);
    }


    data=conf->readEntry("Organization");
    if (data.isEmpty())
    {
        ask.setCaption ("KRN-Missing Configuration info");
        ask.label->setText("Please enter your organization's name");
        ask.entry->setText("");
        ask.exec();
        data=ask.entry->text();
        conf->writeEntry("Organization",data);
    }

    conf->setGroup("NNTP");
    data=conf->readEntry("NNTPServer");
    if (data.isEmpty())
    {
        ask.setCaption ("KRN-Missing Configuration info");
        ask.label->setText("Please enter your NNTP server name");
        ask.entry->setText(getenv("NNTPSERVER"));
        ask.exec();
        data=ask.entry->text();
        conf->writeEntry("NNTPServer",data);
    }
    conf->sync();
}

