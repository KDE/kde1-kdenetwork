#include <qobject.h>
#include <qlistbox.h>
#include <qscrbar.h>

#include "irclistitem.h"

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

  void insertItem ( const QListBoxItem *, int index=-1 );
  void insertItem ( const char * text, int index=-1 );
  void insertItem ( const QPixmap & pixmap, int index=-1 );
  void removeItem ( int index );

  void clear();

//  virtual void scrollBarUpdates(bool);

 signals:
  void updateSize();

 public slots:
  virtual void updateScrollBars();
  virtual void updateTableSize();
  virtual void setTopItem(int);

  virtual void scrollTo(int);

  virtual void pageDown();
  virtual void pageUp();
  virtual void lineUp();
  virtual void lineDown();

 protected:
  virtual void resizeEvent(QResizeEvent *);
  virtual int totalHeight ();

  virtual void mousePressEvent(QMouseEvent *);
  virtual void mouseReleaseEvent(QMouseEvent *);
  virtual void mouseMoveEvent(QMouseEvent *);

  bool xlateToText(int x, int y, int *rrow, int *rline, int *rchar, ircListItem **);


//  virtual void paintEvent ( QPaintEvent * );

 private:

  QScrollBar *vertScroll;

  bool ScrollToBottom;
  /**
   * If the total height has been changed and needs to be recalced this is 
   * true
   */
  bool thDirty;
  /**
   * The Total height of all the irclist items.  This cached since it's
   * used frequently.
   */
  int theightCache;
  /**
    * Find the minimum of the two int.  Order is important.
    * Arg1 is the maxium allowed.
    * Arg2 is the wanted to scroll to.
    * If Arg2 < Arg1 then we're scrolling up at the top, and we 
    * set ScrollToBottom false.  Otherwise we're at the bottom, so 
    * set scroll to bottom true.
    */
  int imin(int, int);

  /**
   * If we are selection this is true. otherwise it's false
   */
  bool selectMode;

};
