#include <qobject.h>
#include <qlistbox.h>
#include <qscrbar.h>

class KSircListBox : public QListBox
{
 Q_OBJECT;
 public:
  KSircListBox(QWidget * parent=0, const char * name=0, WFlags f=0);

  virtual void scrollToBottom();

//  virtual void scrollBarUpdates(bool);

 signals:
  void updateSize();

 public slots:
  virtual void updateScrollBars();
  virtual void setTopItem(int);

  virtual void scrollTo(int);

  virtual void pageDown();
  virtual void pageUp();
  virtual void lineUp();
  virtual void lineDown();

 protected:
  virtual void resizeEvent(QResizeEvent *);
//  virtual void paintEvent ( QPaintEvent * );
 private:

  QScrollBar *vertScroll;

};
