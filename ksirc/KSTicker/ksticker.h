#ifndef KSTICKER_H
#define KSTICKER_H

#include <qobject.h>
#include <qframe.h>
#include <qstring.h>
#include <qlist.h>

struct SInfo {
  int length;
};

class KSTicker : public QFrame
{
  Q_OBJECT

public:
  KSTicker(QWidget * parent=0, const char * name=0, WFlags f=0);
  virtual ~KSTicker();

  void setString(QString);
  void mergeString(QString);

  virtual void show();
  virtual void hide();

signals:
  void doubleClick();
  void closing();

protected slots:
  virtual void timerEvent ( QTimerEvent * );
  virtual void paintEvent ( QPaintEvent * );
  virtual void resizeEvent( QResizeEvent * );
  virtual void closeEvent( QCloseEvent * );
  virtual void mouseDoubleClickEvent( QMouseEvent * );
  virtual void iconify();



private: 
  QString ring; 
  QString display;
  QString display_old;
  QList<SInfo> StrInfo;

  int onechar;
  int chars;
  
  int tickStep;
  int cOffset;

  int pHeight;
  
  int currentChar;

  void startTicker();

};

#endif KSTICKER_H
