#ifndef WIDGET_RUNNER_H
#define WIDGET_RUNNER_H

class WidgetRunner;

#include <qwidget.h>
#include <qintdict.h>
#include "controller.h"

// PWidget definitions

#include "pwidget.h"
#include "pframe.h"

typedef struct {
  PWidget *pwidget; // The widget
  int type;        // The type so casting is "safer"
} WidgetS;         // WidgetStruct

typedef struct {
  PWidget *(*wc)(widgetId *wI, PWidget *parent);
} widgetCreate;


class WidgetRunner : public QObject
{
 Q_OBJECT
 public:
 WidgetRunner(QObject * parent=0, const char * name=0);
 ~WidgetRunner();

 public slots:
 void inputMessage(int fd, PukeMessage *pm);

 void closefd(int fd);

 signals:
 void outputMessage(int fd, PukeMessage *pm);
 
 private:
 QIntDict<QIntDict<WidgetS> > WidgetList;

 QIntDict<widgetCreate> widgetCF; // widgetCreatingFuntion List

 // WinId comes from a static unsigned int we increment for each new window
 static uint uiBaseWinId;

 /**
  * Verifies the widgetId exists and is a valid widget.
  * True is valid, false if invalid.
  */
 bool checkWidgetId(widgetId *pwI);

 /**
  * Create new Widget, returns new iWinId for it.
  * Takes server fd and parent winid, and type as arguments
  */
 widgetId createWidget(widgetId wI, int iType);

};



#endif
