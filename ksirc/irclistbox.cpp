#include "irclistbox.h"
#include <iostream.h>

#include <qevent.h>

#include "KSCutDialog/KSCutDialog.h"

static const int fudge = 5;

KSircListBox::KSircListBox(QWidget * parent, const char * name, WFlags f) : QListBox(parent,name,f)
{
  setAutoScrollBar(FALSE);
  setAutoBottomScrollBar(FALSE);
  thDirty = TRUE;
  vertScroll = new QScrollBar(this, "VertScrollBar");
  vertScroll->setOrientation(QScrollBar::Vertical);
  vertScroll->resize(16, this->height());
  vertScroll->move(this->width() - vertScroll->width(), 0);
  //  QRect frame = frameRect();
  //  frame.setRight(frame.right() - 17);
  //  setFrameRect(frame);
  updateScrollBars();
  connect(vertScroll, SIGNAL(sliderMoved(int)),
          SLOT(scrollTo(int)));
  connect(vertScroll, SIGNAL(nextLine()),
	  SLOT(lineDown()));
  connect(vertScroll, SIGNAL(prevLine()),
	  SLOT(lineUp()));
  connect(vertScroll, SIGNAL(nextPage()),
	  SLOT(pageDown()));
  connect(vertScroll, SIGNAL(prevPage()),
	  SLOT(pageUp()));
  vertScroll->show();
  ScrollToBottom = TRUE;
}

KSircListBox::~KSircListBox()
{
  delete vertScroll;
}

bool KSircListBox::scrollToBottom(bool force)
{
  if(force == TRUE)
    ScrollToBottom = TRUE;

  int th = totalHeight();
  if(th > height()){
    if(ScrollToBottom == TRUE)
      setYOffset(th - height() + fudge);
    updateScrollBars();
  }
  else{
    if(ScrollToBottom == TRUE)
      setYOffset(0);
    vertScroll->setRange(0, 0);
    repaint(FALSE);
  }

  if(ScrollToBottom == TRUE)
    return TRUE;
  else
    return FALSE;
}

void KSircListBox::updateScrollBars()
{
  int wheight = height();
  int theight = totalHeight();
  if(wheight < theight){
    vertScroll->setRange(0, (theight - wheight));
    vertScroll->setValue(yOffset());
  }
  else{
    vertScroll->setRange(0, 0);
  }
}

void KSircListBox::resizeEvent(QResizeEvent *e)
{
  QListBox::resizeEvent(e);
  vertScroll->resize(16, this->height());
  vertScroll->move(this->width() - vertScroll->width(), 0);

  // Dirty the buffer

  thDirty = TRUE;
  emit updateSize();
  
  updateTableSize();
  thDirty = TRUE;
  scrollToBottom(TRUE);
}

void KSircListBox::setTopItem(int index)
{
  QListBox::setTopItem(index);
  updateScrollBars();
}

void KSircListBox::scrollTo(int index)
{
   setYOffset(index + fudge);
   if((index + 100) > (totalHeight() - height()))
     ScrollToBottom = TRUE;
   else
     ScrollToBottom = FALSE;
   //   setYOffset(yoff);
}

void KSircListBox::pageUp()
{
  setYOffset(QMAX(0, yOffset()-height()));
  updateScrollBars();
  ScrollToBottom = FALSE;
  
}

void KSircListBox::pageDown()
{
  setYOffset(imin(totalHeight()-height()+fudge, yOffset()+height()));
  updateScrollBars();
}

void KSircListBox::lineUp()
{
  setYOffset(QMAX(0, yOffset()-itemHeight(topItem())));
  updateScrollBars();
  ScrollToBottom = FALSE;
}

void KSircListBox::lineDown()
{
  setYOffset(imin(totalHeight()-height()+fudge, yOffset()+itemHeight(topItem())));
  updateScrollBars();
}

//void KSircListBox::scrollBarUpdates(bool update)
//{
//  verticalScrollBar()->setUpdatesEnabled(update);
//}


int KSircListBox::imin(int max, int offset){
  if(max < offset){
    ScrollToBottom = TRUE;
    return max;
  }
  else{
    ScrollToBottom = FALSE;
    return offset;
  }
      
}

int KSircListBox::totalHeight () 
{
  if(thDirty == FALSE)
    return theightCache;
  
  thDirty = FALSE;
  theightCache = QListBox::totalHeight();
  return theightCache;
}

void KSircListBox::insertItem ( const QListBoxItem *lbi, int index )
{
  QListBox::insertItem(lbi, index);
  theightCache += lbi->height(this);
  //  thDirty = TRUE;
}

void KSircListBox::insertItem ( const char * text, int index )
{
  QListBox::insertItem(text, index);
  thDirty = TRUE;
}

void KSircListBox::insertItem ( const QPixmap & pixmap, int index )
{
  QListBox::insertItem(pixmap, index);
  thDirty = TRUE;
}

void KSircListBox::removeItem ( int index ) 
{
  theightCache -= item(index)->height(this);
  QListBox::removeItem(index);
  //  thDirty = TRUE;
}


void KSircListBox::clear()
{
  thDirty = TRUE;
  QListBox::clear();
  
}

void KSircListBox::updateTableSize()
{
  thDirty = TRUE;
  QTableView::updateTableSize();
}
