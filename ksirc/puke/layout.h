#ifndef LAYOUT_RUNNER_H
#define LAYOUT_RUNNER_H

class LayoutRunner;

#include <qwidget.h>
#include <qintdict.h>
#include <qlayout.h>
#include "controller.h"
#include "widget.h"

class LayoutRunner : public QObject
{
 Q_OBJECT
 public:
 LayoutRunner(WidgetRunner *_wr, QObject * parent=0, const char * name=0);
 ~LayoutRunner();

 QBoxLayout *id2layout(widgetId *pwi);
 bool checkLayoutId(widgetId *pwi);

 public slots:
 void inputMessage(int fd, PukeMessage *pm);

 signals:
 void outputMessage(int fd, PukeMessage *pm);
 
 private:
 QIntDict<QBoxLayout> LayoutList;
 WidgetRunner *wrControl;

 // WinId comes from a static unsigned int we increment for each new window
 static uint uiBaseLayoutId;

};



#endif
