#ifndef ALISTBOX_H
#define ALISTBOX_H

#include <qobject.h>
#include <qwidget.h>
#include <qlistbox.h>
#include <qevent.h>

#include "irclistitem.h"

class aListBox : public QListBox
{
  Q_OBJECT;

public:
  aListBox(QWidget *parent = 0, const char *name = 0) : QListBox(parent,name)
    {
      clear();
      p_scroll = palette().copy();
    }

  void clear();

  void inSort ( const QListBoxItem *, bool top = false);
  void inSort ( const char * text, bool top = false);

  bool isTop(int index);

  virtual void setPalette ( const QPalette & );

signals:
   void rightButtonPress(int index);

protected:
  virtual void mousePressEvent ( QMouseEvent * );
  virtual int findSep();

private:
  QPalette p_scroll;


};

#endif
