#ifndef KIODCCC_H
#define KIODCCC_H

#include <qobject.h>
#include <qstring.h>
#include <qdict.h>
#include <qprogdlg.h>

#include "KSProgress/ksprogress.h"

#include "messageReceiver.h"
#include "ksircprocess.h"
#include "dccDialog.h"

struct DCCInfo {
  QString nick;
  int LastSize;
  int PercentSize;
};

class KSircIODCC : public QObject,
		   public KSircMessageReceiver
{
  Q_OBJECT
public:
  KSircIODCC(KSircProcess *_proc) : QObject(),
    KSircMessageReceiver(_proc)
    {
      proc = _proc;
      pending = new dccDialog();
      connect(pending, SIGNAL(getFile()),
	      this, SLOT(getFile()));
      connect(pending, SIGNAL(forgetFile()),
	      this, SLOT(forgetFile()));
    }
  virtual ~KSircIODCC();

  virtual void sirc_receive(QString);
  virtual void control_message(QString);

protected slots:
  void cancelTransfer(QString);
  void getFile();
  void forgetFile();

signals:
  void outputLine(QString);

private:
//  QDict<QProgressDialog> DlgList;
  QDict<KSProgress> DlgList;
  QDict<DCCInfo> DCCStatus;
  
  dccDialog *pending;

  KSircProcess *proc;
};

#endif
