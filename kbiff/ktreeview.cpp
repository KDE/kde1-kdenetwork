/*
 * $Id$
 *
 * QListView fake COMPLETELY taken from KTreeView
 * 
 * Copyright (C) 1997 Johannes Sixt
 * 
 * based on KTreeList, which is
 * Copyright (C) 1996 Keith Brown and KtSoft
 *
 * Modified slightly by Kurt Granroth <granroth@kde.org> 10/98
 * 
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABLILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy
 * of the GNU General Public License along with this program; if not, write
 * to the Free Software Foundation, Inc, 675 Mass Ave, Cambridge, MA 02139,
 * USA.
 */

#include <ktreeview.h>

#if QT_VERSION < 140

#include <qapp.h>			/* used for QApplication::closingDown() */
#include <qkeycode.h>			/* used for keyboard interface */
#include <qpainter.h>			/* used to paint items */
#include <assert.h>

/*
 * -------------------------------------------------------------------
 * 
 * QListViewItem
 * 
 * -------------------------------------------------------------------
 */

// constructor
QListViewItem::QListViewItem(const QString& theText) :
	owner(0),
	numChildren(0),
	doExpandButton(true),
	expanded(false),
	delayedExpanding(false),
	doTree(true),
	doText(true),
	child(0),
	parent(0),
	sibling(0),
	deleteChildren(false)
{
    _text = theText;
}

QListViewItem::QListViewItem(QListView* parent, const QString& theText) :
	owner(0),
	numChildren(0),
	doExpandButton(true),
	expanded(false),
	delayedExpanding(false),
	doTree(true),
	doText(true),
	child(0),
	parent(0),
	sibling(0),
	deleteChildren(false)
{
    _text = theText;
	 parent->insertItem(this);
}
// constructor that takes a pixmap
QListViewItem::QListViewItem(const QString& theText,
			     const QPixmap& thePixmap) :
	owner(0),
	numChildren(0),
	doExpandButton(true),
	expanded(false),
	delayedExpanding(false),
	doTree(true),
	doText(true),
	child(0),
	parent(0),
	sibling(0),
	deleteChildren(false)
{
    _text = theText;
    pixmap = thePixmap;
}

// destructor
QListViewItem::~QListViewItem()
{
    if (deleteChildren) {
	// remove the children
	QListViewItem* i = child;
	while (i) {
	    QListViewItem* d = i;
	    i = i->getSibling();
	    delete d;
	}
    }
}

// appends a direct child
void QListViewItem::appendChild(QListViewItem* newChild)
{
    newChild->parent = this;
    newChild->owner = owner;
    if (!getChild()) {
	child = newChild;
    }
    else {
	QListViewItem* lastChild = getChild();
	while (lastChild->hasSibling()) {
	    lastChild = lastChild->getSibling();
	}
	lastChild->sibling = newChild;
    }
    newChild->sibling = 0;
    numChildren++;
}

// returns the bounding rectangle of the item content (not including indent
// and branches) for hit testing
QRect QListViewItem::boundingRect(int indent) const
{
    const QFontMetrics& fm = owner->fontMetrics();
    int rectX = indent;
    int rectY = 1;
    int rectW = width(indent, fm) - rectX;
    int rectH = height(fm) - 2;
    return QRect(rectX, rectY, rectW, rectH);
}

// returns the child item at the specified index
QListViewItem* QListViewItem::childAt(int index) const
{
    if (!hasChild())
	return 0;
    QListViewItem* item = getChild();
    while (index > 0 && item != 0) {
	item = item->getSibling();
	index--;
    }
    return item;
}

// returns the number of children this item has
uint QListViewItem::childCount() const
{
    return numChildren;
}

/* returns the index of the given child item in this item's branch */
int QListViewItem::childIndex(QListViewItem* searched) const
{
    assert(searched != 0);
    int index = 0;
    QListViewItem* item = getChild();
    while (item != 0 && item != searched) {
	item = item->getSibling();
	index++;
    }
    return item == 0  ?  -1  :  index;
}

// indicates whether mouse press is in expand button
inline bool QListViewItem::expandButtonClicked(const QPoint& coord) const
{
  return expandButton.contains(coord);
}

// returns a pointer to first child item
QListViewItem* QListViewItem::getChild() const
{
    return child;
}

// returns the parent of this item
QListViewItem* QListViewItem::getParent() const
{
    return parent;
}

// returns reference to the item pixmap
const QPixmap& QListViewItem::getPixmap() const
{
    return pixmap;
}

// returns the sibling below this item
QListViewItem* QListViewItem::getSibling() const
{
    return sibling;
}

// returns a pointer to the item text
const QString& QListViewItem::getText() const
{
    return _text;
}

// indicates whether this item has any children
bool QListViewItem::hasChild() const
{
    return child != 0;
}

// indicates whether this item has a parent
bool QListViewItem::hasParent() const
{
    return parent != 0;
}

// indicates whether this item has a sibling below it
bool QListViewItem::hasSibling() const
{
    return sibling != 0;
}

int QListViewItem::height() const
{
    assert(owner != 0);
    return height(owner->fontMetrics());
}

// returns the maximum height of text and pixmap including margins
// or -1 if item is empty -- SHOULD NEVER BE -1!
int QListViewItem::height(const QFontMetrics& fm) const
{
    int maxHeight = pixmap.height();
    int textHeight = fm.ascent() + fm.leading();
    maxHeight = textHeight > maxHeight ? textHeight : maxHeight;
    return maxHeight == 0 ? -1 : maxHeight + 2; // + 8;
}

// inserts child item at specified index in branch
void QListViewItem::insertChild(int index, QListViewItem* newChild)
{
    if (index < 0) {
	appendChild(newChild);
	return;
    }
    newChild->parent = this;
    newChild->owner = owner;
    if (index == 0) {
	newChild->sibling = getChild();
	child = newChild;
    }
    else {
	QListViewItem* prevItem = getChild();
	QListViewItem* nextItem = prevItem->getSibling();
	while (--index > 0 && nextItem) {
	    prevItem = nextItem;
	    nextItem = prevItem->getSibling();
	}
	prevItem->sibling = newChild;
	newChild->sibling = nextItem;
    }
    numChildren++;
}

// indicates whether this item is displayed expanded 
// NOTE: a TRUE response does not necessarily indicate the item 
// has any children
bool QListViewItem::isExpanded() const
{
    return expanded;
}

// returns true if all ancestors are expanded
bool QListViewItem::isVisible() const
{
    for (QListViewItem* p = getParent(); p != 0; p = p->getParent()) {
	if (!p->isExpanded())
	    return false;
    }
    return true;
}

// paint this item, including tree branches and expand button
void QListViewItem::paint(QPainter* p, int indent, const QColorGroup& cg,
			  bool highlighted) const
{
    assert(getParent() != 0);		/* can't paint root item */

    p->save();

    int cellHeight = height(p->fontMetrics());

    if (doTree) {
	paintTree(p, indent, cellHeight, cg);
    }

    /*
     * If this item has at least one child and expand button drawing is
     * enabled, set the rect for the expand button for later mouse press
     * bounds checking, and draw the button.
     */
    if (doExpandButton && (child || delayedExpanding)) {
	paintExpandButton(p, indent, cellHeight, cg);
    }

    // now draw the item pixmap and text, if applicable
    int pixmapX = indent;
    int pixmapY = (cellHeight - pixmap.height()) / 2;
    p->drawPixmap(pixmapX, pixmapY, pixmap);

    if (doText) {
	paintText(p, indent, cellHeight, cg, highlighted);
    }
    p->restore();
}

void QListViewItem::paintExpandButton(QPainter* p, int indent, int cellHeight,
				      const QColorGroup& cg) const
{
    int parentLeaderX = indent - (22 / 2);
    int cellCenterY = cellHeight / 2;

    expandButton.setRect(parentLeaderX - 4, cellCenterY - 4, 9, 9);
    p->setBrush(cg.base());
    p->setPen(cg.foreground());
    p->drawRect(expandButton);
    if (expanded) {
	p->drawLine(parentLeaderX - 2, cellCenterY, 
		    parentLeaderX + 2, cellCenterY);
    } else {
	p->drawLine(parentLeaderX - 2, cellCenterY,
		    parentLeaderX + 2, cellCenterY);
	p->drawLine(parentLeaderX, cellCenterY - 2, 
		    parentLeaderX, cellCenterY + 2);
    }
    p->setBrush(NoBrush);
}

// paint the highlight 
void QListViewItem::paintHighlight(QPainter* p, int indent,
	const QColorGroup& colorGroup, bool hasFocus, GUIStyle style) const
{
	QColor fc;
	if (style == WindowsStyle)
		fc = QApplication::winStyleHighlightColor();
	else
		fc = colorGroup.text();

	QRect textRect = textBoundingRect(indent);
	int t,l,b,r;
	textRect.coords(&l, &t, &r, &b);
	QRect outerRect;
	outerRect.setCoords(l - 2, t - 2, r + 2, b + 2);

	if (style == WindowsStyle)	/* Windows style highlight */
	{
		if (hasFocus)
		{
			p->fillRect(outerRect, fc);	/* highlight background */
	//		p->setPen(colorGroup.text());
	//		p->setBrush(NoBrush);
	//		p->drawRect(textRect);
		}
		else
		{
			p->fillRect(textRect, fc);	/* highlight background */
		}
	}
	else
	{				/* Motif style highlight */
		if (hasFocus)
		{
			p->fillRect(textRect, fc);	/* highlight background */
			p->setPen(fc);
			p->drawRect(outerRect);
		}
		else
		{
			p->fillRect(outerRect, fc);	/* highlight background */
		}
	}
}

// draw the text, highlighted if requested
void QListViewItem::paintText(QPainter* p, int indent, int cellHeight,
			      const QColorGroup& cg, bool highlighted) const
{
	int textX = indent + pixmap.width() + 4;
	int textY = cellHeight - ((cellHeight - p->fontMetrics().ascent() - 
			p->fontMetrics().leading()) / 2);
	if (highlighted)
	{
		paintHighlight(p, indent, cg, owner->hasFocus(), owner->style());
		p->setPen(cg.base());
		p->setBackgroundColor(cg.text());
	}
	else
	{
		p->setPen(cg.text());
		p->setBackgroundColor(cg.base());
	}
	p->drawText(textX, textY, _text);
}

// paint the tree structure
void QListViewItem::paintTree(QPainter* p, int indent, int cellHeight,
			      const QColorGroup& cg) const
{
    int parentLeaderX = indent - (22 / 2);
    int cellCenterY = cellHeight / 2;
    int cellBottomY = cellHeight - 1;
    int itemLeaderX = indent - 3;

    p->setPen(cg.background());

    /*
     * If this is not the first item in the tree draw the line up
     * towards parent or sibling.
     */
    if (parent->parent != 0 || parent->getChild() != this)
	p->drawLine(parentLeaderX, 0, parentLeaderX, cellCenterY);

    // draw the line down towards sibling
    if (sibling) 
	p->drawLine(parentLeaderX, cellCenterY, parentLeaderX, cellBottomY);

    /*
     * If this item has children or siblings in the tree or is a child of
     * an item other than the root item then draw the little line from the
     * item out to the left.
     */
    if (sibling || (doExpandButton && (child || delayedExpanding)) ||
	parent->parent != 0 ||
	/*
	 * The following handles the case of an item at the end of the
	 * topmost level which doesn't have children.
	 */
	parent->getChild() != this)
    {
	p->drawLine(parentLeaderX, cellCenterY, itemLeaderX, cellCenterY);
    }

    /*
     * If there are siblings of ancestors below, draw our portion of the
     * branches that extend through this cell.
     */
    QListViewItem* prevRoot = parent;
    while (prevRoot->getParent() != 0) {  /* while not root item */
	if (prevRoot->hasSibling()) {
	    int sibLeaderX = owner->indentation(prevRoot) - (22 / 2);
	    p->drawLine(sibLeaderX, 0, sibLeaderX, cellBottomY);
	}
	prevRoot = prevRoot->getParent();
    }
}

// removes the given (direct) child from the branch
bool QListViewItem::removeChild(QListViewItem* theChild)
{
    // search item in list of children
    QListViewItem* prevItem = 0;
    QListViewItem* toRemove = getChild();
    while (toRemove && toRemove != theChild) {
	prevItem = toRemove;
	toRemove = toRemove->getSibling();
    }

    if (toRemove) {
	// found it!
	if (prevItem == 0) {
	    child = toRemove->getSibling();
	} else {
	    prevItem->sibling = toRemove->getSibling();
	}
	numChildren--;
	toRemove->owner = 0;
    }
    assert((numChildren == 0) == (child == 0));

    return toRemove != 0;
}

// sets the delayed-expanding flag
void QListViewItem::setDelayedExpanding(bool flag)
{
    delayedExpanding = flag;
}

// tells the item whether it shall delete child items
void QListViewItem::setDeleteChildren(bool flag)
{
    deleteChildren = flag;
}

// sets the draw expand button flag of this item
void QListViewItem::setDrawExpandButton(bool doit)
{
    doExpandButton = doit;
}

// sets the draw text flag of this item
void QListViewItem::setDrawText(bool doit)
{
    doText = doit;
}

// sets the draw tree branch flag of this item
void QListViewItem::setDrawTree(bool doit)
{
    doTree = doit;
}

// sets the expanded flag of this item
void QListViewItem::setExpanded(bool is)
{
    expanded = is;
}

// sets the item pixmap to the given pixmap
void QListViewItem::setPixmap(const QPixmap& pm)
{
    pixmap = pm;
}

void QListViewItem::setPixmap(int, const QPixmap& pm)
{
    pixmap = pm;

    // if repaint necessary, do it if visible and auto update
    // enabled
	 if (isVisible() || getParent()->childCount() == 1)
	 {
		 bool autoU = owner->autoUpdate();
		 owner->setAutoUpdate(FALSE);
		 owner->updateVisibleItems();
		 if(autoU && owner->isVisible())
			 owner->repaint();
		 owner->setAutoUpdate(autoU);
	 }
}

// sets the item text to the given string
void QListViewItem::setText(const QString& t)
{
    _text = t;
}

// counts the child items and stores the result in numChildren
void QListViewItem::synchNumChildren()
{
    numChildren = 0;
    QListViewItem* item = getChild();
    while (item != 0) {
	numChildren++;
	item = item->getSibling();
    }
}

/*
 * returns the bounding rect of the item text in cell coordinates couldn't
 * get QFontMetrics::boundingRect() to work right so I made my own
 */
QRect QListViewItem::textBoundingRect(int indent) const
{
    const QFontMetrics& fm = owner->fontMetrics();
    int cellHeight = height(fm);
    int rectX = indent + pixmap.width() + 3;
    int rectY = (cellHeight - fm.ascent() - fm.leading()) / 2 + 2;
    int rectW = fm.width(_text) + 1;
    int rectH = fm.ascent() + fm.leading();
    return QRect(rectX, rectY, rectW, rectH);
}

// returns the total width of text and pixmap, including margins, spacing
// and indent, or -1 if empty -- SHOULD NEVER BE -1!
int QListViewItem::width(int indent) const
{
    return width(indent, owner->fontMetrics());
}

int QListViewItem::width(int indent, const QFontMetrics& fm) const
{
    int maxWidth = pixmap.width();
    maxWidth += (4 + fm.width(_text));
    return maxWidth == 0  ?  -1  :  indent + maxWidth + 3;
}


/*
 * -------------------------------------------------------------------
 *
 * QListView
 *
 * -------------------------------------------------------------------
 */

// constructor
QListView::QListView(QWidget *parent,
		     const char *name,
		     WFlags f) :
	QTableView(parent, name, f),
	clearing(false),
	current(-1),
	drawExpandButton(true),
	drawTree(true),
	expansion(0),
	goingDown(false),
	itemIndent(0),
	showText(true),
	itemCapacity(500),
	visibleItems(0),
	rubberband_mode(false)
{
    initMetaObject();
    setCellHeight(0);
    setCellWidth(0);
    setNumRows(0);
    setNumCols(1);
    setTableFlags(Tbl_autoScrollBars | Tbl_clipCellPainting | Tbl_snapToVGrid);
    clearTableFlags(Tbl_scrollLastVCell | Tbl_scrollLastHCell | Tbl_snapToVGrid);
    switch(style()) {
    case WindowsStyle:
    case MotifStyle:
	setFrameStyle(QFrame::WinPanel | QFrame::Sunken);
	setBackgroundColor(colorGroup().base());
	break;
    default:
	setFrameStyle(QFrame::Panel | QFrame::Plain);
	setLineWidth(1);
    }
    setAcceptFocus(true);
    treeRoot = new QListViewItem;
    treeRoot->setExpanded(true);
    treeRoot->owner = this;

    visibleItems = new QListViewItem*[itemCapacity];
    // clear those pointers
    for (int j = itemCapacity-1; j >= 0; j--) {
	visibleItems[j] = 0;
    }
}

// destructor
QListView::~QListView()
{
    goingDown = true;
    clear();
    delete[] visibleItems;
    delete treeRoot;
}

// appends a child to the item at the given index with the given text
// and pixmap
void QListView::appendChildItem(const char* theText, const QPixmap& thePixmap,
				int index)
{
    QListViewItem* item = new QListViewItem(theText, thePixmap);
    item->setDeleteChildren(true);
    appendChildItem(item, index);
}

// appends a child to the item at the end of the given path with
// the given text and pixmap
void QListView::appendChildItem(const char* theText, const QPixmap& thePixmap,
				const KPath& thePath)
{
    QListViewItem* item = new QListViewItem(theText, thePixmap);
    item->setDeleteChildren(true);
    appendChildItem(item, thePath);
}

// appends the given item to the children of the item at the given index
void QListView::appendChildItem(QListViewItem* newItem, int index)
{                                                                  
    /* find parent item and append new item to parent's sub tree */
    QListViewItem* parentItem = itemAt(index);
    if (!parentItem)
	return;
    appendChildItem(parentItem, newItem);
}

// appends the given item to the children of the item at the end of the
// given path
void QListView::appendChildItem(QListViewItem* newItem, const KPath& thePath)
{                                
    /* find parent item and append new item to parent's sub tree */
    QListViewItem* parentItem = itemAt(thePath);
    if (!parentItem)
	return;
    appendChildItem(parentItem, newItem);
}
                                 
// indicates whether horizontal scrollbar appears only when needed
bool QListView::autoBottomScrollBar() const
{
  return testTableFlags(Tbl_autoHScrollBar);
}

// indicates whether vertical scrollbar appears only when needed
bool QListView::autoScrollBar() const
{
  return testTableFlags(Tbl_autoVScrollBar);
}

// indicates whether display updates automatically on changes
bool QListView::autoUpdate() const
{
  return QTableView::autoUpdate();
}

// indicates whether horizontal scrollbar is present
bool QListView::bottomScrollBar() const
{
  return testTableFlags(Tbl_hScrollBar);
}

// find item at specified index and change pixmap and/or text
void QListView::changeItem(const char *newText,
			      const QPixmap *newPixmap,
			      int index)
{
  QListViewItem *item = itemAt(index);
  if(item)
    changeItem(item, index, newText, newPixmap);
}

// find item at end of specified path, and change pixmap and/or text
void QListView::changeItem(const char* newText,
			   const QPixmap* newPixmap,
			   const KPath& thePath)
{
    QListViewItem* item = itemAt(thePath);
    if (item) {
	int index = itemRow(item);
	changeItem(item, index, newText, newPixmap);
    }
}

// clear all items from list and erase display
void QListView::clear()
{
    setCurrentItem(-1);	

    /* somewhat of a hack for takeItem so it doesn't update the current item... */
	clearing = TRUE;
	
	bool autoU = autoUpdate();
	setAutoUpdate(FALSE);
	QStack<QListViewItem> stack;
	stack.push(treeRoot);
	while(!stack.isEmpty()) {
		QListViewItem *item = stack.pop();
		if(item->hasChild()) {
			stack.push(item);
			stack.push(item->getChild());
		}
		else if(item->hasSibling()) {
			stack.push(item);
			stack.push(item->getSibling());
		}
		else if(item->getParent() != 0) {
			takeItem(item);
			delete item;
		}
	}
	clearing = FALSE;
  if(goingDown || QApplication::closingDown())
    return;
  if(autoU && isVisible())
    repaint();
  setAutoUpdate(autoU);
}

// return a count of all the items in the tree, whether visible or not
uint QListView::count()
{
  int total = 0;
  forEveryItem(&QListView::countItem, (void *)&total);
  return total;
}

// returns the index of the current (highlighted) item
int QListView::currentItemIndex() const
{
  return current;
}

// collapses the item at the specified row index.
void QListView::collapseItem(int index, bool emitSignal)
{
    QListViewItem* item = itemAt(index);
    if (item)
	collapseSubTree(item, emitSignal);
}

// expands the item at the specified row indes.
void QListView::expandItem(int index, bool emitSignal)
{
    QListViewItem* item = itemAt(index);
    if (item)
	expandSubTree(item, emitSignal);
}

// returns the depth the tree is automatically expanded to when
// items are added
int QListView::expandLevel() const
{
  return expansion;
}

// visits every item in the tree, visible or not and applies 
// the user supplied function with the item and user data passed as parameters
// if user supplied function returns true, traversal ends and function returns
bool QListView::forEveryItem(KForEveryFunc func, void* user, QListViewItem* item)
{
    if (item == 0) {
	item = treeRoot;
    }
    assert(item->owner == this);
    item = item->getChild();

    while (item != 0) {
	// visit the siblings
	if ((*func)(item, user)) {
	    return true;
	}
	// visit the children (recursively)
	if (item->hasChild()) {
	    if (forEveryItem(func, user, item))
		return true;
	}
	item = item->getSibling();
    }
    return false;
}

// visits every visible item in the tree in order and applies the 
// user supplied function with the item and user data passed as parameters
// if user supplied function returns TRUE, traversal ends and function
// returns
bool QListView::forEveryVisibleItem(KForEveryFunc func, void *user,
				    QListViewItem* item)
{
    if (item == 0) {
	item = treeRoot;
    } else {
	// children are invisible (hence, nothing to do)
	// if item is invisible or collapsed
	if (!item->isVisible() || !item->isExpanded()) {
	    return false;
	}
    }
    assert(item->owner == this);
    item = item->getChild();

    while (item != 0) {
	// visit the siblings
	if ((*func)(item, user)) {
	    return true;
	}
	// visit the children (recursively)
	if (item->hasChild() && item->isExpanded()) {
	    if (forEveryVisibleItem(func, user, item))
		return true;
	}
	item = item->getSibling();
    }
    return false;
}

// returns a pointer to the QListViewItem at the current index
// or 0 if no current item
QListViewItem *QListView::getCurrentItem()
{
  if(current == -1) return 0;
  return itemAt(current);
}

QListViewItem *QListView::currentItem()
{
  if(current == -1) return 0;
  return itemAt(current);
}

// returns the current indent spacing
int QListView::indentSpacing()
{
    return itemIndent;
}

// inserts a new item with the specified text and pixmap before
// or after the item at the given index, depending on the value
// of prefix
// if index is negative, appends item to tree at root level
bool QListView::insertItem(const char* theText, const QPixmap& thePixmap,
			   int row, bool prefix)
{
    QListViewItem* refItem = itemAt(row);

    QListViewItem* item = new QListViewItem(theText, thePixmap);
    item->setDeleteChildren(true);

    bool success = insertItem(refItem, item, prefix);
    if (!success)
	delete item;
    return success;
}

// inserts a new item with the specified text and pixmap before
// or after the item at the end of the given path, depending on the value
// of prefix
bool QListView::insertItem(const char* theText, const QPixmap& thePixmap,
			   const KPath& path, bool prefix)
{
    QListViewItem* refItem = itemAt(path);

    QListViewItem* item = new QListViewItem(theText, thePixmap);
    item->setDeleteChildren(true);

    bool success = insertItem(refItem, item, prefix);
    if (!success)
	delete item;
    return success;
}

// inserts the given item or derived object into the tree before
// or after the item at the given index, depending on the value
// of prefix
// if index is negative, appends item to tree at root level
bool QListView::insertItem(QListViewItem* newItem,
			   int index, bool prefix)
{
    // find the item currently at the index, if there is one
    QListViewItem* refItem = itemAt(index);

    // insert new item at the appropriate place
    return insertItem(refItem, newItem, prefix);
}

// inserts the given item or derived object into the tree before
// or after the item at the end of the given path, depending on the value
// of prefix
bool QListView::insertItem(QListViewItem* newItem,
			   const KPath& thePath, bool prefix)
{                              
    // find the item currently at the end of the path, if there is one
    QListViewItem* refItem = itemAt(thePath);

    // insert new item at appropriate place
    return insertItem(refItem, newItem, prefix);
}

/*
 * returns pointer to QListViewItem at the specifed row or 0 if row is out
 * of limits.
 */
QListViewItem* QListView::itemAt(int row)
{
    if (row < 0 || row >= numRows()) {
	return 0;
    }
    else {
	// lookup the item in the list of visible items
	assert(row < itemCapacity);
	QListViewItem* i = visibleItems[row];
	assert(i != 0);
	return i;
    }
}

// returns pointer to QListViewItem at the end of the
// path or 0 if not found
QListViewItem* QListView::itemAt(const KPath& path)
{
    if (path.isEmpty())
	return 0;

    // need a copy of the path because recursiveFind will destroy it
    KPath pathCopy;
    pathCopy.setAutoDelete(false);
    pathCopy = path;

    return recursiveFind(pathCopy);
}

// computes the path of the item at the specified index
// if index is invalid, nothing is done.
void QListView::itemPath(int row, KPath& path)
{
    QListViewItem* item = itemAt(row);
    if (item != 0) {
	itemPath(item, path);
    }
}

// returns the row in the visible tree of the given item or
// -1 if not found
int QListView::itemRow(QListViewItem* item)
{
    if (item->owner == this) {
	// search in list of visible items
	for (int i = numRows()-1; i >= 0; i--) {
	    if (visibleItems[i] == item) {
		return i;
	    }
	}
    }
    // not found
    return -1;
}

/*
 * move the subtree at the specified index up one branch level (make root
 * item a sibling of its current parent)
 */
void QListView::join(int index)
{
  QListViewItem *item = itemAt(index);
  if(item)
    join(item);
}

/*
 * move the subtree at the specified index up one branch level (make root
 * item a sibling of it's current parent)
 */
void QListView::join(const KPath& path)
{
    QListViewItem *item = itemAt(path);
    if (item)
	join(item);
}

/* move item at specified index one slot down in its parent's sub tree */
void QListView::lowerItem(int index)
{
  QListViewItem *item = itemAt(index);
  if(item)
    lowerItem(item);
}

/* move item at specified path one slot down in its parent's sub tree */
void QListView::lowerItem(const KPath& path)
{
    QListViewItem* item = itemAt(path);
    if (item)
	lowerItem(item);
}

/* move item at specified index one slot up in its parent's sub tree */
void QListView::raiseItem(int index)
{
  QListViewItem* item = itemAt(index);
    if (item)
	raiseItem(item);
}

/* move item at specified path one slot up in its parent's sub tree */
void QListView::raiseItem(const KPath& path)
{
    QListViewItem* item = itemAt(path);
    if (item)
	raiseItem(item);
}

// remove the item at the specified index and delete it
void QListView::removeItem(int index)
{
  QListViewItem *item = itemAt(index);
  if(item) { 
    takeItem(item);
    delete item;
  }
}

// remove the item at the end of the specified path and delete it
void QListView::removeItem(const KPath& thePath)
{
    QListViewItem* item = itemAt(thePath);
    if (item) {
	takeItem(item);
	delete item;
    }
}

// indicates whether vertical scrollbar is present
bool QListView::scrollBar() const
{
  return testTableFlags(Tbl_vScrollBar);
}

void QListView::scrollVisible(QListViewItem* item, bool children)
{
    if (item == 0)
	return;
    int row = itemRow(item);
    if (row < 0)
	return;				/* do nothing if invisible */

    if (children && item->isExpanded()) {
	// we are concerned about children
	if (!rowIsVisible(row)) {
	    // just move to the top
	    setTopCell(row);
	} else {
	    // this is the complicated part
	    // count the visible children (including grandchildren)
	    int numVisible = 0;
	    forEveryVisibleItem(countItem, &numVisible, item);
	    // if the last child is visible, do nothing
	    if (rowIsVisible(row + numVisible))
		return;
	    /*
	     * Basically, item will become the top cell; but if there are
	     * more visible rows in the widget than we have children, then
	     * we don't move that far.
	     */
	    int remain = lastRowVisible()-topCell()-numVisible;
	    if (remain <= 0) {
		setTopCell(row);
	    } else {
		setTopCell(QMAX(0,row-remain));
	    }
	}
    } else {
	// we are not concerned about children
	if (rowIsVisible(row))
	    return;
	// just move the item to the top
	setTopCell(row);
    }
}

// enables/disables auto update of display
void QListView::setAutoUpdate(bool enable)
{
  QTableView::setAutoUpdate(enable);
}

// enables/disables horizontal scrollbar
void QListView::setBottomScrollBar(bool enable)
{
  enable ? setTableFlags(Tbl_hScrollBar) :
    clearTableFlags(Tbl_hScrollBar);
}

// sets the current item and hightlights it, emitting signals
void QListView::setCurrentItem(int row)
{
    if (row == current)
	return;
    int numVisible = numRows();
    if (row > numVisible)
	return;
    int oldCurrent = current;
    current = row;
    if (oldCurrent < numVisible)
	updateCell(oldCurrent, 0);
	 if (current > -1) {
		 updateCell(current, 0, false);
		 emit highlighted(current);
		 emit selectionChanged(currentItem());
	 }
}

// enables/disables drawing of expand button
void QListView::setExpandButtonDrawing(bool enable)
{
    if (drawExpandButton == enable)
	return;
    drawExpandButton = enable;

    // the user parameter is cast to a bool in setItemExpandButtonDrawing
    forEveryItem(&QListView::setItemExpandButtonDrawing, enable ? &enable : 0);

    if (autoUpdate() && isVisible())
	repaint();
}

// sets depth to which subtrees are automatically expanded, and
// redraws tree if auto update enabled
void QListView::setExpandLevel(int level)
{
    if (expansion == level)
	return;
    expansion = level;
    QListViewItem* item = getCurrentItem();
    forEveryItem(&QListView::setItemExpandLevel, 0);
    while (item != 0) {
	if (item->getParent()->isExpanded())
	    break;
	item = item->getParent();
    }
    if (item != 0)
	setCurrentItem(itemRow(item));
    if (autoUpdate() && isVisible())
	repaint();
}

// sets the indent margin for all branches and repaints if auto update enabled
void QListView::setIndentSpacing(int spacing)
{
    if (itemIndent == spacing)
	return;
    itemIndent = spacing;
    updateCellWidth();
    if (autoUpdate() && isVisible())
	repaint();
}

// enables/disables vertical scrollbar
void QListView::setScrollBar(bool enable)
{
  enable ? setTableFlags(Tbl_vScrollBar) :
    clearTableFlags(Tbl_vScrollBar);
}

// enables/disables display of item text (keys)
void QListView::setShowItemText(bool enable)
{
    if (showText == enable)
	return;
    showText = enable;

    // the user parameter is cast to a bool in setItemShowText
    forEveryItem(&QListView::setItemShowText, enable ? &enable : 0);

    if (autoUpdate() && isVisible())
	repaint();
}

// indicates whether vertical scrolling is by pixel or row
void QListView::setSmoothScrolling(bool enable)
{
  enable ? setTableFlags(Tbl_smoothVScrolling) :
    clearTableFlags(Tbl_smoothVScrolling);
}

// enables/disables tree branch drawing
void QListView::setTreeDrawing(bool enable)
{
    if (drawTree == enable)
	return;
    drawTree = enable;

    // the user parameter is cast to a bool in setItemTreeDrawing
    forEveryItem(&QListView::setItemTreeDrawing, enable ? &enable : 0);

    if (autoUpdate() && isVisible())
	repaint();
}
    
// indicates whether item text keys are displayed
bool QListView::showItemText() const
{
  return showText;
}

// indicates whether scrolling is by pixel or row
bool QListView::smoothScrolling() const
{
  return testTableFlags(Tbl_smoothVScrolling);
}

// indents the item at the given index, splitting the tree into
// a new branch
void QListView::split(int index)
{
  QListViewItem *item = itemAt(index);
  if(item)
    split(item);
}

// indents the item at the given path, splitting the tree into
// a new branch
void QListView::split(const KPath& path)
{
    QListViewItem* item = itemAt(path);
    if (item)
	split(item);
}

// removes item at specified index from tree but does not delete it
// returns pointer to the item or 0 if not succesful
QListViewItem *QListView::takeItem(int index)
{
  QListViewItem *item = itemAt(index);
  if(item)
    takeItem(item);
  return item;
}

// removes item at specified path from tree but does not delete it
// returns pointer to the item or 0 if not successful
QListViewItem* QListView::takeItem(const KPath& path)
{
    QListViewItem* item = itemAt(path);
    if (item)
	takeItem(item);
    return item;
}

// indicates whether tree branches are drawn
bool QListView::treeDrawing() const
{
  return drawTree;
}


// appends a child to the specified parent item (note: a child, not a sibling, is added!)
void QListView::appendChildItem(QListViewItem* theParent,
				QListViewItem* newItem)
{
    theParent->appendChild(newItem);

    // set item state
    newItem->setDrawExpandButton(drawExpandButton);
    newItem->setDrawTree(drawTree);
    newItem->setDrawText(showText);
    if (level(newItem) < expansion) {
	newItem->setExpanded(true);
    }

    // fix up branch levels of any children that the new item may already have
    if(newItem->hasChild()) {
	fixChildren(newItem);
    }

    // if necessary, adjust cell width, number of rows and repaint
    if (newItem->isVisible() || theParent->childCount() == 1) {
	bool autoU = autoUpdate();
	setAutoUpdate(false);
	updateVisibleItems();
	if(autoU && isVisible())
	    repaint();
	setAutoUpdate(autoU);
    }
}

// returns the height of the cell(row) at the specified row (index)
int QListView::cellHeight(int row)
{
  return itemAt(row)->height(fontMetrics());
}

// returns the width of the cells. Note: this is mostly for derived classes
// which have more than 1 column
int QListView::cellWidth(int /*col*/)
{
  return maxItemWidth;
}

// changes the given item with the new text and/or pixmap
void QListView::changeItem(QListViewItem* toChange, int itemRow,
			   const char* newText, const QPixmap* newPixmap)
{
    int indent = indentation(toChange);
    int oldWidth = toChange->width(indent);
    if(newText)
	toChange->setText(newText);
    if (newPixmap)
	toChange->setPixmap(*newPixmap);
    if(oldWidth != toChange->width(indent))
	updateCellWidth();
    if(itemRow == -1)
	return;
    if(autoUpdate() && rowIsVisible(itemRow))
	updateCell(itemRow, 0);
}

// collapses the subtree at the specified item
void QListView::collapseSubTree(QListViewItem* subRoot, bool emitSignal)
{
    assert(subRoot->owner == this);
    if (!subRoot->isExpanded())
	return;

    // must move the current item if it is visible
    QListViewItem* cur = current >= 0  ?  itemAt(current)  :  0;

    subRoot->setExpanded(false);
    if (subRoot->isVisible()) {
	bool autoU = autoUpdate();
	setAutoUpdate(false);
	updateVisibleItems();
	// re-seat current item
	if (cur != 0) {
	    setCurrentItem(itemRow(cur));
	}
	if (emitSignal) {
	    emit collapsed(itemRow(subRoot));
	}
	if (autoU && isVisible())
	    repaint();
	setAutoUpdate(autoU);
    }
}

// used by count() with forEach() function to count total number
// of items in the tree
bool QListView::countItem(QListViewItem*, void* total)
{
    int* t = static_cast<int*>(total);
    (*t)++;
    return false;
}

// expands the subtree at the given item
void QListView::expandSubTree(QListViewItem* subRoot, bool emitSignal)
{
    assert(subRoot->owner == this);
    if (subRoot->isExpanded())
	return;

    // must move the current item if it is visible
    QListViewItem* cur = current >= 0  ?  itemAt(current)  :  0;

    bool allow = true;

    if (subRoot->delayedExpanding) {
	emit expanding(subRoot, allow);
    }
    if (!allow)
	return;

    subRoot->setExpanded(true);

    if (subRoot->isVisible()) {
	bool autoU = autoUpdate();
	setAutoUpdate(false);
	updateVisibleItems();
	// re-seat current item
	if (cur != 0) {
	    setCurrentItem(itemRow(cur));
	}
	if (emitSignal) {
	    emit expanded(itemRow(subRoot));
	}
	if (autoU && isVisible())
	    repaint();
	setAutoUpdate(autoU);
    }
}

// fix up branch levels out of whack from split/join operations on the tree
void QListView::fixChildren(QListViewItem *parentItem)
{
    QListViewItem* childItem = 0;
    QListViewItem* siblingItem = 0;
//    int childBranch = parentItem->getBranch() + 1;
    if(parentItem->hasChild()) {
	childItem = parentItem->getChild(); 
//	childItem->setBranch(childBranch);
	childItem->owner = parentItem->owner;
	fixChildren(childItem);
    }
    while(childItem && childItem->hasSibling()) {
	siblingItem = childItem->getSibling();
//	siblingItem->setBranch(childBranch);
	siblingItem->owner = parentItem->owner;
	fixChildren(siblingItem);
	childItem = siblingItem;
    }	
}

// handles QFocusEvent processing by setting current item to top
// row if there is no current item, and updates cell to add or
// delete the focus rectangle on the highlight bar
void QListView::focusInEvent(QFocusEvent *)
{
  if(current < 0 && numRows() > 0)
    setCurrentItem(topCell());
  updateCell(current, 0);
}

// called by updateCellWidth() for each item in the visible list
bool QListView::getMaxItemWidth(QListViewItem* item, void* user)
{
    assert(item->owner != 0);
    int indent = item->owner->indentation(item);
    int* maxW = static_cast<int*>(user);
    int w = item->width(indent);
    if (w > *maxW)
	*maxW = w;
    return false;
}

int QListView::indentation(QListViewItem* item) const
{
    return level(item) * itemIndent + itemIndent + 3;
}

// inserts the new item before or after the reference item, depending
// on the value of prefix
bool QListView::insertItem(QListViewItem* referenceItem,
			   QListViewItem* newItem,
			   bool prefix)
{
    assert(newItem != 0);
    assert(referenceItem == 0 || referenceItem->owner == this);

    /* set the new item's state */
    newItem->setDrawExpandButton(drawExpandButton);
    newItem->setDrawTree(drawTree);
    newItem->setDrawText(showText);
    QListViewItem* parentItem;
    if (referenceItem) {
	parentItem = referenceItem->getParent(); 
	int insertIndex = parentItem->childIndex(referenceItem);
	if (!prefix)
	    insertIndex++;
	parentItem->insertChild(insertIndex, newItem);
    }
    else {
	// no reference item, append at end of visible tree
	// need to repaint
	parentItem = treeRoot;
	parentItem->appendChild(newItem);
    }

    // set item expansion
    if (level(newItem) < expansion)
	newItem->setExpanded(true);

    // fix up branch levels of any children
    if (newItem->hasChild())
	fixChildren(newItem);

    // if repaint necessary, do it if visible and auto update
    // enabled
    if (newItem->isVisible() || parentItem->childCount() == 1) {
	bool autoU = autoUpdate();
	setAutoUpdate(FALSE);
	updateVisibleItems();
	if(autoU && isVisible())
	    repaint();
	setAutoUpdate(autoU);
    }
    return true;
}

/*
 * returns pointer to item's path
 */
void QListView::itemPath(QListViewItem* item, KPath& path) const
{
    assert(item != 0);
    assert(item->owner == this);
    if (item != treeRoot) {
	itemPath(item->getParent(), path);
	path.push(new QString(item->getText()));
    }
}

/*
 * joins the item's branch into the tree, making the item a sibling of its
 * parent
 */
void QListView::join(QListViewItem *item)
{
  QListViewItem *itemParent = item->getParent();
  if(itemParent->hasParent()) {
    bool autoU = autoUpdate();
    setAutoUpdate(FALSE);
    takeItem(item);
    insertItem(itemParent, item, FALSE);
    if(autoU && isVisible())
      repaint();
    setAutoUpdate(autoU);
  }
}

// handles keyboard interface
void QListView::keyPressEvent(QKeyEvent* e)
{
    if (numRows() == 0)
	return;				/* nothing to do */

    /* if there's no current item, make the top item current */
    if (currentItemIndex() < 0)
	setCurrentItem(topCell());
    assert(currentItemIndex() >= 0);		/* we need a current item */
    assert(itemAt(currentItemIndex()) != 0);	/* we really need a current item */

    int pageSize, delta;
    QListViewItem* item;
    int key = e->key();
repeat:
    switch (key) {
    case Key_Up:
	// make previous item current, scroll up if necessary
	if (currentItemIndex() > 0) {
	    setCurrentItem(currentItemIndex() - 1);
	    scrollVisible(itemAt(currentItemIndex()), false);
	}
	break;
    case Key_Down:
	// make next item current, scroll down if necessary
	if (currentItemIndex() < numRows() - 1) {
	    setCurrentItem(currentItemIndex() + 1);
	    if (currentItemIndex() > lastRowVisible()) {
		// scrollVisible is not feasible here because
		// it scrolls the item to the top
		setTopCell(topCell() + currentItemIndex() - lastRowVisible());
	    } else if (currentItemIndex() < topCell()) {
		setTopCell(currentItemIndex());
	    }
	}
	break;
    case Key_Next:
	// move highlight one page down and scroll down
	delta = currentItemIndex() - topCell();
	pageSize = lastRowVisible() - topCell();
	setTopCell(QMIN(topCell() +  pageSize, numRows() - 1));
	setCurrentItem(QMIN(topCell() + delta, numRows() - 1));
	break;
    case Key_Prior:
	// move highlight one page up and scroll up
	delta = currentItemIndex() - topCell();
	pageSize = lastRowVisible() - topCell();
	setTopCell(QMAX(topCell() - pageSize, 0));
	setCurrentItem(QMAX(topCell() + delta, 0));
	break;
    case Key_Plus:
    case Key_Right:
	// if current item has subtree and is collapsed, expand it
	item = itemAt(currentItemIndex());
	if (item->isExpanded() && item->hasChild() && key == Key_Right) {
	    // going right on an expanded item is like going down
	    key = Key_Down;
	    goto repeat;
	} else {
	    expandSubTree(item, true);
	    scrollVisible(item, true);
	}
	break;
    case Key_Minus:
    case Key_Left:
	// if current item has subtree and is expanded, collapse it
	item = itemAt(currentItemIndex());
	if ((!item->isExpanded() || !item->hasChild()) && key == Key_Left) {
	    // going left on a collapsed item goes to its parent
	    item = item->getParent();
	    if (item == treeRoot)
		break;			/* we're already at the top */
	    assert(item->isVisible());
	    setCurrentItem(itemRow(item));
	} else {
	    collapseSubTree(item, true);
	}
	scrollVisible(item, false);
	break;
    case Key_Return:
    case Key_Enter:
	// select the current item
	if (currentItemIndex() >= 0)
	    emit selected(currentItemIndex());
	break;
    default:
	break;
    }
}

int QListView::level(QListViewItem* item) const
{
    assert(item != 0);
    assert(item->owner == this);
    assert(item != treeRoot);
    int l = 0;
    item = item->parent->parent;	/* since item != treeRoot, there is a parent */
    while (item != 0) {
	item = item->parent;
	l++;
    }
    return l;
}

/* move specified item down one slot in parent's subtree */
void QListView::lowerItem(QListViewItem *item)
{
  QListViewItem *itemParent = item->getParent();
  uint itemChildIndex = itemParent->childIndex(item);
  if(itemChildIndex < itemParent->childCount() - 1) {
    bool autoU = autoUpdate();
    setAutoUpdate(FALSE);
    takeItem(item);
    insertItem(itemParent->childAt(itemChildIndex), item, FALSE);
    if(autoU && isVisible())
      repaint();
    setAutoUpdate(autoU);
  }
}

// handle mouse double click events by selecting the clicked item
// and emitting the signal
void QListView::mouseDoubleClickEvent(QMouseEvent *e)
{
  // find out which row has been clicked
	
  QPoint mouseCoord = e->pos();
  int itemClicked = findRow(mouseCoord.y());
  
  // if a valid row was not clicked, do nothing
  
  if(itemClicked == -1) 
    return;

  QListViewItem *item = itemAt(itemClicked);
  if(!item) return;
  
  // translate mouse coord to cell coord
  
  int  cellX, cellY;
  colXPos(0, &cellX);
  rowYPos(itemClicked, &cellY);
  QPoint cellCoord(mouseCoord.x() - cellX, mouseCoord.y() - cellY);
  
  // hit test item

    int indent = indentation(item);
  if(item->boundingRect(indent).contains(cellCoord))
    emit selected(itemClicked);
}

// handle mouse movement events
void QListView::mouseMoveEvent(QMouseEvent *e)
{
  // in rubberband_mode we actually scroll the window now
  if (rubberband_mode) 
	{
	  move_rubberband(e->pos());
	}
}


// handle single mouse presses
void QListView::mousePressEvent(QMouseEvent *e)
{
    // first: check which button was pressed

    if (e->button() == MidButton) 
    {
	// RB: the MMB is hardcoded to the "rubberband" scroll mode
	if (!rubberband_mode) {
	    start_rubberband(e->pos());
	}
	return;
    } 
    else if (rubberband_mode) 
    {
	// another button was pressed while rubberbanding, stop the move.
	// RB: if we allow other buttons while rubberbanding the tree can expand
	//     while rubberbanding - we then need to reclaculate and resize the
	//     rubberband rect and show the new size
	end_rubberband();
	return;  // should we process the button press?
    }

    // find out which row has been clicked
    QPoint mouseCoord = e->pos();
    int itemClicked = findRow(mouseCoord.y());

    // nothing to do if not on valid row  
    if (itemClicked == -1)
	return;

    QListViewItem* item = itemAt(itemClicked);
    if (!item)
	return;

    // translate mouse coord to cell coord
    int  cellX, cellY;
    colXPos(0, &cellX);
    rowYPos(itemClicked, &cellY);
    QPoint cellCoord(mouseCoord.x() - cellX, mouseCoord.y() - cellY);

    /* hit test expand button (doesn't set currentItem) */
    if (item->expandButtonClicked(cellCoord)) {
	if (item->isExpanded()) {
	    collapseSubTree(item, true);
	} else {
	    expandSubTree(item, true);
	    scrollVisible(item, true);	/* make children visible */
	}
    }
    // hit test item
    else if (item->boundingRect(indentation(item)).contains(cellCoord)) {
	setCurrentItem(itemClicked);
    }
}

// handle mouse release events
void QListView::mouseReleaseEvent(QMouseEvent *e)
{
  /* if it's the MMB end rubberbanding */
  if (rubberband_mode && e->button()==MidButton) 
	{
	  end_rubberband();
	}
}

// rubberband move: draw the rubberband
void QListView::draw_rubberband()
{
    /*
     * RB: I'm using a white pen because of the XorROP mode. I would prefer
     * to draw the rectangle in red but I don't now how to get a pen which
     * draws red in XorROP mode (this depends on the background). In fact
     * the color should be configurable.
     */

  if (!rubberband_mode) return;
  QPainter paint(this);
  paint.setPen(white);
  paint.setRasterOp(XorROP);
  paint.drawRect(xOffset()*viewWidth()/totalWidth(),
                 yOffset()*viewHeight()/totalHeight(),
                 rubber_width+1, rubber_height+1);
  paint.end();
}

// rubberband move: start move
void QListView::start_rubberband(const QPoint& where)
{
  if (rubberband_mode) { // Oops!
    end_rubberband();
  }
    /* RB: Don't now, if this check is necessary */
  if (!viewWidth() || !viewHeight()) return; 
  if (!totalWidth() || !totalHeight()) return;

  // calculate the size of the rubberband rectangle
  rubber_width = viewWidth()*viewWidth()/totalWidth();
  if (rubber_width > viewWidth()) rubber_width = viewWidth();
  rubber_height = viewHeight()*viewHeight()/totalHeight();
  if (rubber_height > viewHeight()) rubber_height = viewHeight();

  // remember the cursor position and the actual offset
  rubber_startMouse = where;
  rubber_startX = xOffset();
  rubber_startY = yOffset();
  rubberband_mode=TRUE;
  draw_rubberband();
}

// rubberband move: end move
void QListView::end_rubberband()
{
  if (!rubberband_mode) return;
  draw_rubberband();
  rubberband_mode = FALSE;
}

// rubberband move: hanlde mouse moves
void QListView::move_rubberband(const QPoint& where)
{
  if (!rubberband_mode) return;

  // look how much the mouse moved and calc the new scroll position
  QPoint delta = where - rubber_startMouse;
  int nx = rubber_startX + delta.x() * totalWidth() / viewWidth();
  int ny = rubber_startY + delta.y() * totalHeight() / viewHeight();

  // check the new position (and make it valid)
  if (nx < 0) nx = 0;
  else if (nx > maxXOffset()) nx = maxXOffset();
  if (ny < 0) ny = 0;
  else if (ny > maxYOffset()) ny = maxYOffset();

  // redraw the rubberband at the new position
  draw_rubberband();
  setOffset(nx,ny);
  draw_rubberband();
}


// paints the cell at the specified row and col
// col is ignored for now since there is only one
void QListView::paintCell(QPainter* p, int row, int)
{
    QListViewItem* item = itemAt(row);
    if (item == 0)
	return;

    QColorGroup cg = colorGroup();
    int indent = indentation(item);
    item->paint(p, indent, cg,
		current == row);		/* highlighted */
}


/* This is needed to make the kcontrol's color setup working (Marcin Dalecki) */
void QListView::paletteChange(const QPalette &)
{
    setBackgroundColor(colorGroup().base());
    repaint(true);
}


/* raise the specified item up one slot in parent's subtree */
void QListView::raiseItem(QListViewItem *item)
{
  QListViewItem *itemParent = item->getParent();
  int itemChildIndex = itemParent->childIndex(item);
  if(itemChildIndex > 0) {
    bool autoU = autoUpdate();
    setAutoUpdate(FALSE);
    takeItem(item);
    insertItem(itemParent->childAt(--itemChildIndex), item, TRUE);
    if(autoU && isVisible())
      repaint();
    setAutoUpdate(autoU);
  }
}

// find the item at the path
QListViewItem* QListView::recursiveFind(KPath& path)
{
    if (path.isEmpty())
	return treeRoot;

    // get the next key
    QString* searchString = path.pop();

    // find the parent item
    QListViewItem* parent = recursiveFind(path);
    if (parent == 0)
	return 0;

    /*
     * Iterate through the parent's children searching for searchString.
     */
    QListViewItem* sibling = parent->getChild();
    while (sibling != 0) {
	if (*searchString == sibling->getText()) {
	    break;			/* found it! */
	}
	sibling = sibling->getSibling();
    }
    return sibling;
}

void QListView::setItemExpanded(QListViewItem* item)
{
    if (level(item) < expansion) {
	expandSubTree(item, true);
    } else {
	collapseSubTree(item, true);
    }
}

// called by setExpandLevel for each item in tree
bool QListView::setItemExpandLevel(QListViewItem* item, void*)
{
    assert(item->owner != 0);
    item->owner->setItemExpanded(item);
    return false;
}

// called by setExpandButtonDrawing for every item in tree
// the parameter drawButton is used as (and implicitly cast to) a bool
bool QListView::setItemExpandButtonDrawing(QListViewItem* item,
					   void* drawButton)
{
    item->setDrawExpandButton(drawButton);
    return false;
}

// called by setShowItemText for every item in tree
// the parameter newDrawText is used as (and implicitly cast to) a bool
bool QListView::setItemShowText(QListViewItem* item, 
				void* newDrawText)
{
    item->setDrawText(newDrawText);
    return false;
}

// called by setTreeDrawing for every item in tree
// the parameter drawTree is used as (and implicitly cast to) a bool
bool QListView::setItemTreeDrawing(QListViewItem* item, void* drawTree)
{
    item->setDrawTree(drawTree);
    return false;
}

// makes the item a child of the item above it, splitting
// the tree into a new branch
void QListView::split(QListViewItem *item)
{
  QListViewItem *itemParent = item->getParent();
  int itemChildIndex = itemParent->childIndex(item);
  if(itemChildIndex == 0)
    return;
  bool autoU = autoUpdate();
  setAutoUpdate(FALSE);
  takeItem(item);
  appendChildItem(itemParent->childAt(--itemChildIndex), item);
  if(autoU && isVisible())
    repaint();
  setAutoUpdate(autoU);
}

// removes the item from the tree without deleting it
void QListView::takeItem(QListViewItem* item)
{
    assert(item->owner == this);

    // TODO: go over this function again

    bool wasVisible = item->isVisible();
    /*
     * If we have a current item, make sure that it is not in the subtree
     * that we are about to remove. If the current item is in the part
     * below the taken-out subtree, we must move it up a number of rows if
     * the taken-out subtree is at least partially visible.
     */
    QListViewItem* cur = current >= 0  ?  itemAt(current)  :  0;
    if (wasVisible && cur != 0) {
	QListViewItem* c = cur;
	while (c != 0 && c != item) {
	    c = c->getParent();
	}
	if (c != 0) {
	    // move current item to parent
	    cur = item->getParent();
	    if (cur == treeRoot)
		cur = 0;
	}
    }
    QListViewItem* parentItem = item->getParent();
    parentItem->removeChild(item);
    item->sibling = 0;
    if (wasVisible || parentItem->childCount() == 0) {
	bool autoU = autoUpdate();
	setAutoUpdate(FALSE);
	updateVisibleItems();

	if (autoU && isVisible())
	    repaint();
	setAutoUpdate(autoU);
    }

    // re-seat the current item
    setCurrentItem(cur != 0  ?  itemRow(cur)  :  -1);
}

// visits each item, calculates the maximum width  
// and updates QTableView
void QListView::updateCellWidth()
{
    // make cells at least 1 pixel wide to avoid singularities (division by zero)
    int maxW = 1;
    forEveryVisibleItem(&QListView::getMaxItemWidth, &maxW);
    maxItemWidth = maxW;
    updateTableSize();
}

void QListView::updateVisibleItems()
{
    int index = 0;
    int count = 0;
    updateVisibleItemRec(treeRoot, index, count);
    assert(index == count);
    setNumRows(count);
    updateCellWidth();
}

void QListView::updateVisibleItemRec(QListViewItem* item, int& index, int& count)
{
    if (!item->isExpanded()) {
	// no visible items if not expanded
	return;
    }

    /*
     * Record the children of item in the list of visible items.
     *
     * Don't register item itself, it's already in the list. Also only
     * allocate new space for children.
     */
    count += item->childCount();
    if (count > itemCapacity) {
	// must reallocate
	int newCapacity = itemCapacity;
	do {
	    newCapacity += newCapacity;
	} while (newCapacity < count);
	QListViewItem** newItems = new QListViewItem*[newCapacity];
	// clear the unneeded space
	for (int i = index; i < newCapacity; i++) {
	    newItems[i] = 0;
	}
	// move already accumulated items over
	for (int i = index-1; i >= 0; i--) {
	    newItems[i] = visibleItems[i];
	}
	delete[] visibleItems;
	visibleItems = newItems;
	itemCapacity = newCapacity;
    }
    // insert children
    for (QListViewItem* i = item->getChild(); i != 0; i = i->getSibling()) {
	visibleItems[index++] = i;
	updateVisibleItemRec(i, index, count);
    }
}
#endif
