/*
 * Main controller for:
 *
 * PUKE = Perl gUi Kontrol Environment
 *
 */

#ifndef PUKE_CONTROLLER_H
#define PUKE_CONTROLLER_H

class PukeController;

#include <qobject.h>
#include <qsocknot.h>
#include <qstring.h>
#include <qintdict.h>

#include "pmessage.h"

#include "../servercontroller.h"

#include "commands.h"
#include "widget.h"
#include "layout.h"

typedef struct {
  QString server;
  bool writeable;
  QSocketNotifier *sr,*sw;
} fdStatus;

typedef struct {
  void (PukeController::*cmd)(int, PukeMessage*);
  void *dlhandle;
} commandStruct;

class PukeController : public QObject {
  Q_OBJECT;
public:
  PukeController(QString socket = "", QObject *parent=0, const char *name=0);
  ~PukeController();
  bool running;

signals:
  void PukeMessages(QString server, int command, QString args);

public slots:
  void ServMessage(QString, int, QString);

protected slots:
  void Traffic(int);
  void Writeable(int);
  void NewConnect(int);

  /**
   * Fd to write to
   * PukeMessage message  to be written, if null buffer is flushed.
   */
  void writeBuffer(int fd, PukeMessage *message);


private:
  QString qsPukeSocket;
  int iListenFd;
  QSocketNotifier *qsnListen;
  QIntDict<fdStatus> qidConnectFd;

  WidgetRunner *wrControl;
  LayoutRunner *lrControl;
  
  QIntDict<commandStruct> qidCommandTable;

  void initHdlr();

  void closefd(int fd);

  void MessageDispatch(int fd, PukeMessage *pm);

  // Message handlers
  void hdlrPukeSetup(int fd, PukeMessage *pm);
  void hdlrPukeInvalid(int fd, PukeMessage *pm);
  void hdlrPukeEcho(int fd, PukeMessage *pm);

};

#endif
