#ifndef _PWS_SERVER_
#define _PWS_SERVER_

#include <kprocess.h>
#include <qsocketnotifier.h>
#include <qstring.h>
#include <qcolor.h>
#include <qpixmap.h>
#include "../../irclistbox.h"
#include "../../irclistitem.h"

class PWSServer : public QObject{
    Q_OBJECT
public:
    PWSServer(QObject *parent, QString script, QString LogDir);
    ~PWSServer();

    void showLogWindow(bool show=TRUE);

    void logit(QString txt);
    
private slots:
    void stdoutData(KProcess *proc, char *buf, int len);
    void stderrData(KProcess *proc, char *buf, int len);
    void serverDied(KProcess *proc);

    void webLogData(int socket);
    void errorLogData(int socket);
    

private:
    KProcess server;

    QSocketNotifier *web, *error;

    KSircListBox *log;
};

#endif