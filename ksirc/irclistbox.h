#include <qobject.h>
#include <qlistbox.h>
#include <qscrbar.h>

class KSircListBox : public QListBox
{
 Q_OBJECT;
 public:
  KSircListBox(QWidget * parent=0, const char * name=0, WFlags f=0);
  ~KSircListBox();
  /**
    * Scrolls list box to bottom.
    * Returns true if it could have scrolled, false if not.
    */
  virtual bool scrollToBottom(bool force = FALSE);

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

  bool ScrollToBottom;
  /**
    * Find the minimum of the two int.  Order is important.
    * Arg1 is the maxium allowed.
    * Arg2 is the wanted to scroll to.
    * If Arg2 < Arg1 then we're scrolling up at the top, and we 
    * set ScrollToBottom false.  Otherwise we're at the bottom, so 
    * set scroll to bottom true.
    */
  int imin(int, int);

};
