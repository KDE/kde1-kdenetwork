#ifndef AHISTLINEEDIT_H
#define AHISTLINEEDIT_H

#include <qlined.h>
#include <qevent.h>
#include <qstrlist.h>
#include <qkeycode.h>

class aHistLineEdit : public QLineEdit
{
Q_OBJECT;
public:
  aHistLineEdit(QWidget *parent = 0, const char *name = 0);
signals:
  void gotFocus();
  void lostFocus();
protected:
  virtual void keyPressEvent ( QKeyEvent * );
  virtual void focusInEvent ( QFocusEvent * );
  virtual void focusOutEvent ( QFocusEvent * );
private:
  QStrList hist;
  int current;
};

#endif
