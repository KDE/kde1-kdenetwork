#include "irclistbox.h"
#include <iostream.h>

#include <qevent.h>
#include <qregexp.h>
#include <kapp.h>
#include <qclipboard.h>

#include "KSCutDialog/KSCutDialog.h"
#include "irclistitem.h"

extern KApplication *kApp;

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
  selectMode = FALSE;
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
  thDirty = TRUE;
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

void KSircListBox::mousePressEvent(QMouseEvent *me){
  selectMode = FALSE;
  spoint.setX(me->x());
  spoint.setY(me->y());
//  cerr << "Mouse press event!\n";
  ScrollToBottom = FALSE; // Lock window
}

void KSircListBox::mouseReleaseEvent(QMouseEvent *me){
  ScrollToBottom = TRUE; // Unlock window
//  cerr << "Mouse release event!\n";
  if(selectMode == FALSE)
    return;
  selectMode = FALSE;
  int row, line, rchar;
  ircListItem *it;
  if(!xlateToText(me->x(), me->y(), &row, &line, &rchar, &it)){
    it->setRevOne(-1);
    it->setRevTwo(-1);
    kApp->beep();
    return;
  }
  debug("Selected: %s", it->getRev().data());
  kApp->clipboard()->setText(it->getRev());
  it->setRevOne(-1);
  it->setRevTwo(-1);
  it->updateSize();
  updateItem(row, TRUE);

}

void KSircListBox::mouseMoveEvent(QMouseEvent *me){
  int srow = -1, sline = -1, schar = -1;
  int row = -2, line = -2, rchar = -2;
  ircListItem *it;
  if(!xlateToText(me->x(), me->y(), &row, &line, &rchar, &it))
    return;
//  debug("rchar: %d", rchar);
  if(selectMode == FALSE){
    int xoff, yoff;
    xoff = me->x() - spoint.x() > 0 ? me->x() - spoint.x() : spoint.x() - me->x();
    yoff = me->y() - spoint.y() > 0 ? me->y() - spoint.y() : spoint.y() - me->y();
    if(!(xoff > 5 || yoff > 5))
      return;
    ircListItem *sit;
    if(!xlateToText(spoint.x(), spoint.y(),&srow, &sline, &schar, &sit)){
      spoint.setX(me->x());
      spoint.setY(me->y());
      
      return;
    }
    sit->setRevOne(schar);
    selectMode = TRUE;
  }
//  if(schar == rchar)
//    rchar++;
  it->setRevTwo(rchar);
  it->updateSize();
  updateItem(row, FALSE);
}

bool KSircListBox::xlateToText(int x, int y,
                               int *rrow, int *rline, int *rchar, ircListItem **rit){
  int row, line;
  int top; // Index of top item in list box.
  int lineheight; // Height in Pixels for each line
  QString sline; // s line == sample line
  QList<int> c2noc; // Conversion table for colour numbers to "no colour" numbers
  
  if(x < 0)
    x = 0;
  else if(x > width())
    x = width();
  if(y < 0)
    y = 0;
  else if(y > height())
    y = height();
  int ttotal = 0;
  for(top = topItem()-1; top >= 0; top--){
    ttotal += item(top)->height(this);
  }
//  debug("Total height: %d, yOffset: %d, diffrence: %d", ttotal, yOffset(), yOffset() - ttotal);
  
//  cerr << "Selected: " << selectMode << " x: " << x << " y: " << y << endl;
  top = topItem();
//  setTopItem(top);
  lineheight = fontMetrics().lineSpacing();
  int yoff = y + (yOffset() - ttotal);
  if(item(top) == 0x0)
    return FALSE;
  for(row = top; yoff > item(row)->height(this); row++) {
    yoff -= item(row)->height(this);
    if(item(row+1) == 0x0)
      return FALSE;
  }
  for(line = 0; yoff > lineheight; line++) {
    yoff -= lineheight;
  }
//  cerr << "Row: " << row << " Line: " << line << endl;
  ircListItem *it = (ircListItem *) item(row);
  if(it == 0x0){
    warning("Row out of range: %d", row);
    return FALSE;
  }
//  cerr << "Line: " << it->paintText()->at(line) << endl;
  
  sline = KSPainter::stripColourCodes(it->paintText()->at(line), &c2noc);
  if(sline.isNull()){
    warning("No such line: %d", line);
    return FALSE;
  }
  
  QFontMetrics fm = fontMetrics();
  int xoff = x, cchar = 0;
  if(it->pixmap() != 0x0)
    xoff -= (it->pixmap()->width() + 5);

//  cerr << "Line: " << sline << endl;
  for(;  xoff > fm.width(sline[0]); cchar++){
    xoff -= fm.width(sline[0]);
    sline.remove(0, 1);
    if(sline.isEmpty())
      return FALSE;
  }
  if(!c2noc.at(cchar))
    return FALSE;
  cchar = *(c2noc.at(cchar)); // Convert to character offset with colour codes.
  
  // Give abolute pos from start of the line
  for(int l = line-1;  l >= 0; l --){
    cchar += KSPainter::stripColourCodes(it->paintText()->at(l)).length();
  }
  //  cerr << "On char: " << sline[0] << " Index: " << cchar << endl;
  *rrow = row;
  *rchar = cchar;
  *rline = line;
  *rit = it;
  return TRUE;
}

