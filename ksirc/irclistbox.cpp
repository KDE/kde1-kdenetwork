#include "irclistbox.h"
#include <iostream.h>

static const int fudge = 5;

KSircListBox::KSircListBox(QWidget * parent=0, const char * name=0, WFlags f=0) : QListBox(parent,name,f)
{
  setAutoScrollBar(FALSE);
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
}

void KSircListBox::scrollToBottom()
{
 setYOffset(totalHeight() - height() + fudge);
 updateScrollBars();
}

void KSircListBox::updateScrollBars()
{
  if((int) count() > numItemsVisible()){
    vertScroll->setRange(0, count() - numItemsVisible());
    int scroll = (count()-numItemsVisible())*yOffset()/(totalHeight() - height());
    vertScroll->setValue(scroll);
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
  //  QRect frame = frameRect();
  //  frame.setRight(frame.right() - 17);
  //  setFrameRect(frame);
}

void KSircListBox::setTopItem(int index)
{
  QListBox::setTopItem(index);
  updateScrollBars();
}

void KSircListBox::scrollTo(int index)
{
   int yoff = index*(totalHeight() - height()) / 
     (count() - numItemsVisible() - 2) + fudge;
   setYOffset(QMIN(yoff, totalHeight()-height()+fudge));
   //   setYOffset(yoff);
}

void KSircListBox::pageUp()
{
  setYOffset(QMAX(0, yOffset()-height()));
  updateScrollBars();
}

void KSircListBox::pageDown()
{
  setYOffset(QMIN(totalHeight()-height()+fudge, yOffset()+height()));
  updateScrollBars();
}

void KSircListBox::lineUp()
{
  setYOffset(QMAX(0, yOffset()-itemHeight(topItem())));
  updateScrollBars();
}

void KSircListBox::lineDown()
{
  setYOffset(QMIN(totalHeight()-height()+fudge, yOffset()+itemHeight(topItem())));
  updateScrollBars();
}

//void KSircListBox::scrollBarUpdates(bool update)
//{
//  verticalScrollBar()->setUpdatesEnabled(update);
//}
