#ifndef ALISTBOX_H
#define ALISTBOX_H

#include <qobject.h>
#include <qwidget.h>
#include <qlistbox.h>
#include <qevent.h>

class aListBox : public QListBox
{
  Q_OBJECT;

public:
  aListBox(QWidget *parent = 0, const char *name = 0) : QListBox(parent,name)
    {}

signals:
   void rightButtonPress(int index);

protected:
  virtual void mousePressEvent ( QMouseEvent * );

};

#endif
