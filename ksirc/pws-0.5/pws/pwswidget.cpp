#include <pwswidget.h>
#include <pwswidget.moc>

#include <unistd.h>
#include <stdlib.h>
#include <sys/utsname.h>
#include <pwd.h>
#include <grp.h>
#include <sys/types.h>


#include <qlayout.h>
#include <qlistview.h>
#include <qwidgetstack.h>
#include <qlined.h>
#include <qpushbt.h>

#include <kapp.h>
#include <kiconloader.h>
#include <kconfig.h>
#include <kmsgbox.h>

#include <typelayout.h>
#include <pages.h>
#include <kfileio.h>
#include <addwizard.h>

static KConfig *conf;

PWSWidget::PWSWidget(QWidget *parent, const char *name)
	: QWidget(parent, name)
{

    increaser=0;
    server=0;

    conf = kapp->getConfig();
    parent->resize (QSize (580,440));
    
    //Put widgets all around the place
    
    QVBoxLayout *VLay=new QVBoxLayout(this,5);
    QHBoxLayout *HLay1=new QHBoxLayout(5);
    QHBoxLayout *HLay2=new QHBoxLayout(5);
    VLay->addLayout(HLay1,1);
    VLay->addLayout(HLay2,0);


    list=new QListView(this);
    list->setMinimumSize(QSize(100,200));

    // Now some pretty icons in the listview
    list->addColumn("");
    list->addColumn("Things");
    list->setSorting(0,false);
    list->setAllColumnsShowFocus(true);
    QObject::connect (list,SIGNAL(selectionChanged(QListViewItem*)),
                      this,SLOT(flipPage(QListViewItem *)));

    stack=new QWidgetStack(this);
    stack->setMinimumSize(QSize(300,200));
    stack->raiseWidget(1);
    stack->show();

    createGeneralPage();
    loadServers();

    list->setSorting(0,false);
    list->setAllColumnsShowFocus(true);
    list->setSelected(list->firstChild(),true);

    
    HLay1->addWidget(list,5);
    HLay1->addWidget(stack,10);
    
    QPushButton *b1=new QPushButton("Save Configuration",this);
    QObject::connect (b1,SIGNAL(clicked()),SLOT(accept()));
    QPushButton *b2=new QPushButton("(Re)Start Server",this);
    QObject::connect (b2,SIGNAL(clicked()),SLOT(restart()));
    QPushButton *b5=new QPushButton("Log Window",this);
    QObject::connect (b5,SIGNAL(clicked()),SLOT(logWindow()));
    QPushButton *b3=new QPushButton("Add Server",this);
    QObject::connect (b3,SIGNAL(clicked()),SLOT(addServer()));
    QPushButton *b4=new QPushButton("Quit",this);
    QObject::connect (b4,SIGNAL(clicked()),SLOT(quit()));

    b1->setFixedSize(b1->sizeHint());
    b2->setFixedSize(b2->sizeHint());
    b5->setFixedSize(b5->sizeHint());
    b3->setFixedSize(b3->sizeHint());
    b4->setFixedSize(b4->sizeHint());
    HLay2->addStretch(10);
    HLay2->addWidget(b1,0);
    HLay2->addWidget(b2,0);
    HLay2->addWidget(b5,0);
    HLay2->addWidget(b3,0);
    HLay2->addWidget(b4,0);
    VLay->activate();

    parent->resize (QSize (580,440));
}

PWSWidget::~PWSWidget()
{
}

void PWSWidget::quit()
{
    debug ("forgetting everything");
    hide();
    emit quitPressed(parent());
}

void PWSWidget::accept()
{
    debug ("accepted, saving configuration for pws");
    //dump all data to the KConfig
    ((GeneralPage *)pages.find("General"))->dumpData();
    bool needRoot=false;

    conf->setGroup("Servers");
    QStrList names;
    conf->readListEntry("ServerNames",names);
    for (char *name=names.first();name!=0;name=names.next())
    {
        ((ServerPage *)pages.find(name))->dumpData();
        if (conf->readBoolEntry("Enabled")!=false)
            if (conf->readNumEntry("Port")<1024)
                needRoot=true;
    }
    debug ("now saving configuration for mathopd");

    QString config;
    config+="DefaultName ";
    utsname u;
    uname (&u);
    config+=u.nodename;
    config+="\n";

    // General configuration
    
    conf->setGroup ("General");
    config+="Control {\n";
    config+="\tTypes {\n";
    config+=conf->readEntry("MIMETypes");
    config+="\t}\n";

    config+="\tIndexNames { ";
    QStrList indexes;
    conf->readListEntry("IndexNames",indexes);
    for (char *ex=indexes.first();ex!=0;ex=indexes.next())
    {
        config+=ex;
        config+=" ";
    }
    config+=+" }\n";
    
    config+="\tSpecials {\n";

    config+="\t\tCGI { ";
    QStrList extensions;
    conf->readListEntry("CGIext",extensions);
    for (char *ex=extensions.first();ex!=0;ex=extensions.next())
    {
        config+=ex;
        config+=" ";
    }
    config+=+" }\n";

    config+="\t}\n";
    config+="}\n";

    
    //Per server data
    
    for (char *name=names.first();name!=0;name=names.next())
    {
        conf->setGroup(name);
        if (conf->readBoolEntry("Enabled")==false)
            continue;
        config+="Server {\n";
        config+="\tPort ";
        config+=conf->readEntry("Port")+"\n";
        //Main html dir
        config+="\tControl {\n";
        config+="\t\tAlias /\n";
        config+="\t\tLocation ";
        config+=conf->readEntry("HTMLDir")+"\n";
        config+="\t}\n";
        
        QString alias,dir;
        
        //alias1
        alias=conf->readEntry("Alias1");
        dir=conf->readEntry("AliasDir1");
        if ((!alias.isEmpty()) && (!dir.isEmpty()))
        {
            config+="\tControl {\n";
            config+="\t\tAlias "+alias+"\n";
            config+="\t\tLocation ";
            config+=dir+"\n";
            config+="\t}\n";
        }
        //alias2
        alias=conf->readEntry("Alias2");
        dir=conf->readEntry("AliasDir2");
        if ((!alias.isEmpty()) && (!dir.isEmpty()))
        {
            config+="\tControl {\n";
            config+="\t\tAlias "+alias+"\n";
            config+="\t\tLocation ";
            config+=dir+"\n";
            config+="\t}\n";
        }
        //alias3
        alias=conf->readEntry("Alias3");
        dir=conf->readEntry("AliasDir3");
        if ((!alias.isEmpty()) && (!dir.isEmpty()))
        {
            config+="\tControl {\n";
            config+="\t\tAlias "+alias+"\n";
            config+="\t\tLocation ";
            config+=dir+"\n";
            config+="\t}\n";
        }
        config+="}\n";
    }
    
    
    //Logs and stuff
    
    conf->setGroup ("General");
    QString logdir=conf->readEntry("Logs");
    
    config+="Log  ";
    config+=logdir+"WebLog\n";
    config+="ErrorLog ";
    config+=logdir+"ErrorLog\n";
    config+="ChildLog ";
    config+=logdir+"ChildLog\n";
    config+="PIDFile ";
    config+=logdir+"PIDFile\n";
    config+="Umask 022\n";

    /*
     * There's not way I'm going to allow this to ever become root
     */
    
#undef NEEDROOT
#ifdef NEEDROOT
#warning I am not really sure about this uid/gid thing.
    if (needRoot)
    {
        QString usergrp;
        passwd *p=getpwuid(getuid());
        group *g=getgrgid(getgid());
        usergrp.sprintf ("user %s\ngroup %s\n",p->pw_name,g->gr_name);
        config+=usergrp;
    }
#endif
    
    kStringToFile(config,KApplication::localkdedir()+"/share/apps/pws/server-config");

}

void PWSWidget::restart()
{
    conf->setGroup("Servers");
    QStrList names;
    conf->readListEntry("ServerNames",names);
    bool needRoot=false;
    for (char *name=names.first();name!=0;name=names.next())
    {
        conf->setGroup(name);
        if (conf->readBoolEntry("Enabled")!=false)
            if (conf->readNumEntry("Port")<1024)
                needRoot=true;
    }
    debug ("now saving configuration for mathopd");

    if (geteuid()==0) //I am root already
        needRoot=false;
    if (needRoot)
        debug ("need root");
    else
        debug ("don't need root");
    debug ("trying to kill old mathopd");
    debug ("and starting new mathopd with new configuration");
    QString script,pid;
    conf->setGroup("General");
    QString pidfile=conf->readEntry("Logs")+"PIDFile";
    if (QFile::exists(pidfile))
        pid=kFileToString(pidfile);
    if (!pid.isEmpty())
    {
        script+="echo \"Killing old server, with PID "+pid+"\"\n";
        script+="kill "+pid+"\n";
        script+="echo Done\n";
    }
    script+="echo Starting new server\n";
    if (needRoot)
    {
        script+="echo Please enter root password, so I can start\n";
        script+="echo the new daemon in the low port you selected.\n";
        script+="su -c ";
    }
    script+="mathopd <";
    script+=KApplication::localkdedir()+"/share/apps/pws/server-config\n";
    script+="echo Done\n";
    script+="echo Any errors should have appeared above\n";
    script+="echo \"Press return to close this window\"\nread\n";

    kStringToFile(script,KApplication::localkdedir()+"/share/apps/pws/server-script");

    QString command;
    command="kvt -e sh ";
    command+=KApplication::localkdedir()+"/share/apps/pws/server-script";

    conf->setGroup("General");
    QString logdir=conf->readEntry("Logs");

    debug ("command->%s",command.data());

    if(server != 0)
        delete server;
    server = new PWSServer(this, KApplication::localkdedir()+"/share/apps/pws/server-config", logdir);
    
//    system(command.data());
}
void PWSWidget::addServer()
{

    AddServerWizard *wiz=new AddServerWizard();
    int i=wiz->exec();
    qApp->processEvents();
    debug ("done wizard-->%d",i);
    if (i==1) //pressed OK
    {
        conf->setGroup("Servers");
        QStrList names;
        conf->readListEntry("ServerNames",names);
        debug ("name=%s",wiz->data.at(0));
        names.append (wiz->data.at(0));
        conf->writeEntry("ServerNames",names);
        conf->setGroup(wiz->data.at(0));
        debug ("htmldir=%s",wiz->data.at(1));
        conf->writeEntry("HTMLDir",wiz->data.at(1));
        debug ("port=%d",atoi(wiz->data.at(2)));
        conf->writeEntry("Port",atoi(wiz->data.at(2)));
        conf->sync();
    }
}

void PWSWidget::loadServers()
{
    conf->setGroup("Servers");
    QStrList names;
    conf->readListEntry("ServerNames",names);

    if (names.count()==0)
    {
        addServer();
    }
    conf->setGroup("Servers");
    conf->readListEntry("ServerNames",names);
    for (char *name=names.first();name!=0;name=names.next())
        createServerPage(name);
}

void PWSWidget::createGeneralPage()
{
    //list stuff
    QListViewItem *item=new QListViewItem(list);
    item->setPixmap(0,Icon("pws_general.xpm"));
    item->setText(1,"General");

    //stack stuff
    GeneralPage *w=new GeneralPage(stack);
    w->show();
    stack->addWidget(w,increaser);
    increaser++;
    pages.insert("General",w);
}


void PWSWidget::createServerPage(const char *name)
{
    //list stuff
    QListViewItem *item=new QListViewItem(list);
    item->setPixmap(0,Icon("pws_www.xpm"));
    item->setText(1,name);
    list->setSorting(0,false);

    //stack stuff
    ServerPage *w=new ServerPage(stack,name);
    stack->addWidget(w,increaser);
    w->resize(stack->size());
    w->show();
    list->setSelected(item,true);
    increaser++;
    pages.insert(name,w);
}

void PWSWidget::flipPage(QListViewItem *item)
{
    stack->raiseWidget (pages.find(item->text(1)));
}

void PWSWidget::logWindow()
{
    if(server != 0){
        server->showLogWindow(TRUE);
    }
}