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

KSircListBox::KSircListBox(QWidget * parent, const char * name, WFlags f) : QListBox(parent,name,f) /*FOLD00*/
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
  waitForClear = FALSE;
}

KSircListBox::~KSircListBox() /*fold00*/
{
  delete vertScroll;
}

bool KSircListBox::scrollToBottom(bool force) /*fold00*/
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

void KSircListBox::updateScrollBars() /*fold00*/
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

void KSircListBox::resizeEvent(QResizeEvent *e) /*FOLD00*/
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

void KSircListBox::setTopItem(int index) /*fold00*/
{
  QListBox::setTopItem(index);
  updateScrollBars();
}

void KSircListBox::scrollTo(int index) /*fold00*/
{
   setYOffset(index + fudge);
   if((index + 100) > (totalHeight() - height()))
     ScrollToBottom = TRUE;
   else
     ScrollToBottom = FALSE;
   //   setYOffset(yoff);
}

void KSircListBox::pageUp() /*fold00*/
{
  setYOffset(QMAX(0, yOffset()-height()));
  updateScrollBars();
  ScrollToBottom = FALSE;
  
}

void KSircListBox::pageDown() /*fold00*/
{
  setYOffset(imin(totalHeight()-height()+fudge, yOffset()+height()));
  updateScrollBars();
}

void KSircListBox::lineUp() /*fold00*/
{
  setYOffset(QMAX(0, yOffset()-itemHeight(topItem())));
  updateScrollBars();
  ScrollToBottom = FALSE;
}

void KSircListBox::lineDown() /*fold00*/
{
  setYOffset(imin(totalHeight()-height()+fudge, yOffset()+itemHeight(topItem())));
  updateScrollBars();
}

//void KSircListBox::scrollBarUpdates(bool update)
//{
//  verticalScrollBar()->setUpdatesEnabled(update);
//}


int KSircListBox::imin(int max, int offset){ /*fold00*/
  if(max < offset){
    ScrollToBottom = TRUE;
    return max;
  }
  else{
    ScrollToBottom = FALSE;
    return offset;
  }
      
}

int KSircListBox::totalHeight ()  /*fold00*/
{
  if(thDirty == FALSE)
    return theightCache;
  
  thDirty = FALSE;
  theightCache = QListBox::totalHeight();
  return theightCache;
}

void KSircListBox::insertItem ( const QListBoxItem *lbi, int index ) /*fold00*/
{
  QListBox::insertItem(lbi, index);
  theightCache += lbi->height(this);
  //  thDirty = TRUE;
}

void KSircListBox::insertItem ( const char * text, int index ) /*fold00*/
{
  QListBox::insertItem(text, index);
  thDirty = TRUE;
}

void KSircListBox::insertItem ( const QPixmap & pixmap, int index ) /*fold00*/
{
  QListBox::insertItem(pixmap, index);
  thDirty = TRUE;
}

void KSircListBox::removeItem ( int index )  /*fold00*/
{
  theightCache -= item(index)->height(this);
  thDirty = TRUE;
  QListBox::removeItem(index);
  //  thDirty = TRUE;
}


void KSircListBox::clear() /*fold00*/
{
  thDirty = TRUE;
  QListBox::clear();
  
}

void KSircListBox::updateTableSize() /*fold00*/
{
  thDirty = TRUE;
  QTableView::updateTableSize();
}

void KSircListBox::mousePressEvent(QMouseEvent *me){ /*FOLD00*/

  if(me->button() == LeftButton){
    selectMode = FALSE;
    spoint.setX(me->x());
    spoint.setY(me->y());
    ScrollToBottom = FALSE; // Lock window
    if(waitForClear == TRUE)
      clearSelection();
  }
}

void KSircListBox::mouseReleaseEvent(QMouseEvent *me){ /*FOLD00*/

  if(me->button() == LeftButton){
    int erow; // End row

    ScrollToBottom = TRUE; // Unlock window THIS HAS TO BE FIRST SINCE THE WINDOW IS LOCKED ON A MOUSE CLICK

    //  cerr << "Mouse release event!\n";
    if(selectMode == FALSE)
      return;

    selectMode = FALSE;
    int row, line, rchar;
    ircListItem *it;
    if(!xlateToText(me->x(), me->y(), &row, &line, &rchar, &it)){
      erow = lrow;
      row = erow;
      it = (ircListItem *) item(erow);
      if(it == 0x0){
        warning("Row out of range: %d", row);
        return;
      }
    }
    else{
      erow = row;
    }
    if(erow == srow){
      debug("Selected: %s", it->getRev().data());
      kApp->clipboard()->setText(KSPainter::stripColourCodes(it->getRev()));
      updateItem(row, TRUE);
    }
    else {
      int trow, brow; // Top row, Bottom row
      QString selected;

      if(erow < srow){
        trow = erow;
        brow = srow;
      }
      else{
        trow = srow;
        brow = erow;
      }
      for(int crow = trow; crow <= brow; crow ++){
        ircListItem *cit = (ircListItem *) item(crow);
        if(cit == 0x0){
          warning("Row out of range: %d", crow);
          return;
        }
        selected.append(KSPainter::stripColourCodes(cit->getRev()));
        selected.append("\n");
      }
      selected.truncate(selected.length()-1); // Remove the last \n
      kApp->clipboard()->setText(selected);
      debug("selected: %s", selected.data());
    }
    waitForClear = TRUE;
  }
  else if(me->button() == MidButton){
    emit pasteReq();
  }
}

void KSircListBox::clearSelection() { /*FOLD00*/
  for(int i = min; i <= max; i++){
    ircListItem *it = (ircListItem *) item(i);
    if(it == 0x0){
      warning("Row out of range: %d", i);
      continue;
    }
    it->setRevOne(-1);
    it->setRevTwo(-1);
    it->updateSize();
    updateItem(i, FALSE);
  }
  min = 1; // Turns off repeated clears
  max = 0;
  waitForClear = FALSE;
  cerr << "Got clear\n";
}

void KSircListBox::mouseMoveEvent(QMouseEvent *me){ /*FOLD00*/

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
    if(!xlateToText(spoint.x(), spoint.y(),&srow, &sline, &schar, &sit)){
      spoint.setX(me->x());
      spoint.setY(me->y());

      return;
    }
    max = min = sline; // start setting max and min info for clean up
    sit->setRevOne(schar);
    selectMode = TRUE;
    connect(kApp->clipboard(), SIGNAL(dataChanged()),
            this, SLOT(clearSelection()));
  }
  //  if(schar == rchar)
  //    rchar++;
  if(row == srow){
    it->setRevTwo(rchar);
    it->updateSize();
    updateItem(row, FALSE);
  }
  else if(row > srow){
    sit->setRevTwo(strlen(sit->text()));
    sit->updateSize();
    updateItem(srow, FALSE);
    for(int crow = srow + 1; crow < row; crow++){
      ircListItem *cit = (ircListItem *) item(crow);
      if(cit == 0x0){
        warning("Row out of range: %d", crow);
        return;
      }
      cit->setRevOne(0);
      cit->setRevTwo(strlen(cit->text()));
      cit->updateSize();
      updateItem(crow, FALSE);
    }
    it->setRevOne(0);
    it->setRevTwo(rchar);
    it->updateSize();
    updateItem(row, FALSE);
  }
  else if(row < srow){
    sit->setRevTwo(0);
    sit->updateSize();
    updateItem(srow, FALSE);
    for(int crow = srow - 1; crow > row; crow--){
      ircListItem *cit = (ircListItem *) item(crow);
      if(cit == 0x0){
        warning("Row out of range: %d", crow);
        return;
      }
      cit->setRevOne(0);
      cit->setRevTwo(strlen(cit->text()));
      cit->updateSize();
      updateItem(crow, FALSE);
    }
    it->setRevOne(rchar);
    it->setRevTwo(strlen(it->text()));
    it->updateSize();
    updateItem(row, FALSE);
  }
  if(lrow > row && lrow > srow){
    int trow = row > srow ? row : srow;
    for(int crow = lrow; crow > trow; crow --){
      ircListItem *cit = (ircListItem *) item(crow);
      if(cit == 0x0){
        warning("Row out of range: %d", crow);
        return;
      }
      cit->setRevOne(-1);
      cit->setRevTwo(-1);
      cit->updateSize();
      updateItem(crow, FALSE);
    }
  }
  else if(lrow < row && lrow < srow){
    int brow = row < srow ? row : srow;
    for(int crow = lrow; crow < brow; crow++){
      ircListItem *cit = (ircListItem *) item(crow);
      if(cit == 0x0){
        warning("Row out of range: %d", crow);
        return;
      }
      cit->setRevOne(-1);
      cit->setRevTwo(-1);
      cit->updateSize();
      updateItem(crow, FALSE);
    }
  }
  if(row > max)
    max = row;
  else if(row < min)
    min = row;

  lrow = row;
}

bool KSircListBox::xlateToText(int x, int y, /*fold00*/
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
    if(sline.isEmpty()){
      break; // End of line, don't give up!
    }
  }

  //  debug("cchar: %d", cchar);

  if(!c2noc.at(cchar))
    return FALSE;

  cchar = *(c2noc.at(cchar)); // Convert to character offset with colour codes.

  if(sline.isEmpty()) // Adjust for end of line
    cchar += 1;
    
  // Give abolute pos from start of the line
  for(int l = line-1;  l >= 0; l --){
    cchar += strlen(it->paintText()->at(l));
  }
  //  cerr << "On char: " << sline[0] << " Index: " << cchar << endl;
  *rrow = row;
  *rchar = cchar;
  *rline = line;
  *rit = it;
  return TRUE;
}

