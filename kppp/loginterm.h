#ifndef _LOGINTERM_H_
#define _LOGINTERM_H_

#include <qdialog.h>
#include <qmlined.h>
#include <qapp.h>
#include <qtimer.h>

class LoginMultiLineEdit : public QMultiLineEdit {

  Q_OBJECT

public:

  LoginMultiLineEdit(QWidget *parent, const char *name, const int fd);
  ~LoginMultiLineEdit();

  void startTimer();
  void stopTimer();

  void keyPressEvent(QKeyEvent *k);
  void insertChar(char c);
  void myreturn();
  void mynewline();

private:
  int modemfd;
  QTimer *readtimer;

public slots:
  void readtty();
};


class LoginTerm : public QDialog {
  Q_OBJECT
public:
  LoginTerm(QWidget *parent, const char *name, const int fd);

  bool pressedContinue();

public slots:
  void cancelbutton();
  void continuebutton();

private:
  LoginMultiLineEdit *text_window;
  QPushButton *cancel_b;
  QPushButton *continue_b;

  bool cont;
};

#endif



