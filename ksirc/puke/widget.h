#ifndef WIDGET_RUNNER_H
#define WIDGET_RUNNER_H

class WidgetRunner;

#include <qwidget.h>
#include <qintdict.h>
#include "controller.h"

// PWidget definitions

#include "pwidget.h"
//#include "pframe.h"


class WidgetRunner : public QObject
{
 Q_OBJECT
 public:
 WidgetRunner(QObject * parent=0, const char * name=0);
 ~WidgetRunner();

 PWidget *id2pwidget(widgetId *pwi);

 public slots:
 void inputMessage(int fd, PukeMessage *pm);
 void closefd(int fd);
 void closeWidget(widgetId);

 signals:
 void outputMessage(int fd, PukeMessage *pm);
 
 private:

};



#endif
