#include "server.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h> 

#include "../../irclistitem.h"

#include "server.moc"

PWSServer::PWSServer(QObject *parent, QString script, QString logDir)
    : QObject(parent)
{
    log = new KSircListBox();
    log->setCaption("Web Server Event Log");
    log->resize(400,100);

    web = 0x0;
    error = 0x0;
    
    fdWeb = open(logDir+"/WebLog", O_CREAT|O_RDONLY|O_NONBLOCK);
    if(fdWeb > 0){
        lseek(fdWeb, 0, SEEK_END);
         web = new QTimer(this, "web_timer");
         connect(web, SIGNAL(timeout()),
                 this, SLOT(webLogData()));
         web->start(1000, FALSE);
    }
    else {
        warning("Could not open WebLog");
        perror("Failed");
    }
    
    int fdError = open(logDir+"/ErrorLog", O_CREAT|O_RDONLY|O_NONBLOCK);
    if(fdError > 0){
        lseek(fdError, 0, SEEK_END);
        error = new QTimer(this, "error_timer");
        connect(error, SIGNAL(timeout()),
                this, SLOT(errorLogData()));
        error->start(1000, FALSE);
    }
    else{
        warning("Could not open ErrorLog");
        perror("Failed");
    }


    server = new KProcess();
    /* Setup and create the acutal mathopd process */
    connect(server, SIGNAL(processExited(KProcess *)),
            this, SLOT(serverDied(KProcess *)));
    connect(server, SIGNAL(receivedStdout(KProcess *, char *, int)),
            this, SLOT(stdoutData(KProcess *, char *, int)));
    connect(server, SIGNAL(receivedStderr(KProcess *, char *, int)),
            this, SLOT(stderrData(KProcess *, char *, int)));

    *server << "mathpod" << "-n";
    server->start(KProcess::NotifyOnExit, KProcess::All);

    int config = open(script, O_RDONLY);
    if(config <= 0){
        warning("Couldn't open: %s", script.data());
        perror("Failed");
    }
    char buf[512];
    int bytes;
    while(bytes = read(config, buf, 511)){
        server->writeStdin(buf, bytes);
    }
    server->closeStdin();
}

PWSServer::~PWSServer()
{
    server->kill();
    disconnect(server, 0, this, 0);
    // I don't delete the server controller since it only gets created once and ends once and It's segfaulting and I'm not sure why
//    delete server;

    log->hide();
    delete web;
    delete error;
    delete log;
}

void PWSServer::showLogWindow(bool show)
{
    if(show == TRUE)
        log->show();
    else
        log->hide();
}

void PWSServer::stdoutData(KProcess *proc, char *buf, int len)
{
    QString str(buf, len);
    logit(str);
}

void PWSServer::stderrData(KProcess *proc, char *buf, int len)
{
    QString str(buf, len);
    str.prepend("~4");
    logit(str);
    showLogWindow(TRUE);
}
void PWSServer::serverDied(KProcess *proc)
{
    logit("~0,4*** Server Exited ***");
}

void PWSServer::webLogData()
{
    char buf[1024];
    int bytes = read(fdWeb, buf, 1023);
    while(bytes > 0){
        if(bytes <= 1024)
            buf[bytes+1] = 0x0;
        char *line = strtok(buf, "\n");
        while(line){
            logit(line);
            line = strtok(NULL, "\n");
        }
        bytes = read(fdWeb, buf, 1023);
    }
}

void PWSServer::errorLogData()
{
    char buf[1024];
    int bytes = read(fdError, buf, 1023);
    while(bytes > 0){
        if(bytes <= 1024)
            buf[bytes+1] = 0x0;
        char *line = strtok(buf, "\n");
        while(line){
            QString sline = QString("~4") + line;
            logit(sline);
            line = strtok(NULL, "\n");
        }
        bytes = read(fdError, buf, 1023);
    }
        
}

void PWSServer::logit(QString txt)
{
    log->insertItem(new ircListItem(txt, &black, log, 0));
    log->scrollToBottom(TRUE);
}