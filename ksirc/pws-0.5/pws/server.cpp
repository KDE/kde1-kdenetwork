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

    web = 0x0;
    error = 0x0;
    
    int iWebLog = open(logDir+"/WebLog", O_CREAT|O_RDONLY|O_NONBLOCK);
    if(iWebLog > 0){
        lseek(iWebLog, 0, SEEK_END);
        web = new QSocketNotifier(iWebLog, QSocketNotifier::Read, this, "PWSServer_web");
        connect(web, SIGNAL(activated(int)),
                this, SLOT(webLogData(int)));
        web->setEnabled(TRUE);
    }
    else {
        warning("Could not open WebLog");
        perror("Failed");
    }
    
    int iErrorLog = open(logDir+"/ErrorLog", O_CREAT|O_RDONLY|O_NONBLOCK);
    if(iErrorLog > 0){
        lseek(iErrorLog, 0, SEEK_END);
        error = new QSocketNotifier(iErrorLog, QSocketNotifier::Read, this, "PWSServer_error");
        connect(error, SIGNAL(activated(int)),
                this, SLOT(errorLogData(int)));
        error->setEnabled(TRUE);
    }
    else{
        warning("Could not open ErrorLog");
        perror("Failed");
    }


    /* Setup and create the acutal mathopd process */
    connect(&server, SIGNAL(processExited(KProcess *)),
            this, SLOT(serverDied(KProcess *)));
    connect(&server, SIGNAL(receivedStdout(KProcess *, char *, int)),
            this, SLOT(stdoutData(KProcess *, char *, int)));
    connect(&server, SIGNAL(receivedStderr(KProcess *, char *, int)),
            this, SLOT(stderrData(KProcess *, char *, int)));
    server << "mathpod" << "-n";
    server.start(KProcess::NotifyOnExit, KProcess::All);

    int config = open(script, O_RDONLY);
    if(config <= 0){
        warning("Couldn't open: %s", script.data());
        perror("Failed");
    }
    char buf[512];
    int bytes;
    while(bytes = read(config, buf, 511)){
        server.writeStdin(buf, bytes);
    }
    server.closeStdin();
}

PWSServer::~PWSServer()
{
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
    str.prepend("~4 ");
    logit(str);
}
void PWSServer::serverDied(KProcess *proc)
{
    logit("~0,4*** Server Exited ***");
}

void PWSServer::webLogData(int socket)
{
    char buf[1024];
    int bytes = read(socket, buf, 1023);
    while(bytes > 0){

        char *line = strtok(buf, "\n");
        while(line){
            logit(line);
            line = strtok(NULL, "\n");
        }
        bytes = read(socket, buf, 1023);
    }
    web->setEnabled(TRUE);
}

void PWSServer::errorLogData(int socket)
{
    char buf[1024];
    int bytes = read(socket, buf, 1023);
    while(bytes > 0){

        char *line = strtok(buf, "\n");
        while(line){
            QString sline = QString("~4 ") + line;
            logit(sline);
            line = strtok(NULL, "\n");
        }
        bytes = read(socket, buf, 1023);
    }
    error->setEnabled(TRUE);
        
}

void PWSServer::logit(QString txt)
{
    log->insertItem(new ircListItem(txt, &black, log, 0));
    log->scrollToBottom(TRUE);
}