/*******************************************************************

 aListBox

 $$Id$$

 List box that outputs a right click mouse so I can popup a qpopupmenu.

 Does special sorting, and maintains a two part list, one for ops,
 other for users.

 nothing special.

*******************************************************************/

#include "alistbox.h"
#include "linelistitem.h"
#include "config.h"
#include <qscrbar.h>

void aListBox::mousePressEvent(QMouseEvent *e)
{

  QListBox::mousePressEvent(e);

  if(e->button() == RightButton){
    emit rightButtonPress(findItem(mapFromGlobal(cursor().pos()).y()));
  }
}


void aListBox::clear()
{
  QListBox::clear();
}


void aListBox::inSort ( nickListItem *lbi)
{
  int min = -1, max = count() - 1;
  int current, compare;

  bool top = lbi->op();

  int sep = findSep();

  // Max get's set to one more than we want since the binary search never
  // checks the last value.

  if(sep >= 0){
    if(top == TRUE){
      min = 0;
      max = sep;
    }
    else{
      min = sep;
      max = count() + 1;
    }
  }
  else
    current = -1;

  current = min;
  int last_current = -1;
  while(min != max){
    current = (max - min)/2 + min;
    if(current == last_current){
      current++;
      break; // we're looping, so stop
    }
    compare = strncasecmp(text(current), lbi->text(),
			  QMIN(strlen(text(current)),
			       strlen(lbi->text())));
    if(compare < 0)
      min = current;
    else if(compare > 0)
      max = current;
    else // We got a match?
      break;
    last_current = current;
  }

  insertItem(lbi, current);
}

void aListBox::inSort ( const char * text, bool top) 
{
  nickListItem *nli = new nickListItem();
  nli->setText(text);
  if(top == TRUE)
    nli->setOp(TRUE);
  inSort(nli);
}

int aListBox::findSep()
{
  uint i = 0;
  for(; i < count(); i++)
    if(item(i)->op() == FALSE)
      break; // stop now

  return i;

}

bool aListBox::isTop(int index)
{
  if(index >= findSep())
    return FALSE;
  else
    return TRUE;
}

void aListBox:: setPalette ( const QPalette &p )
{
  QListBox::setPalette(p);
  ((QScrollBar*) QTableView::verticalScrollBar())->setPalette(p_scroll);
  ((QScrollBar*) QTableView::horizontalScrollBar())->setPalette(p_scroll);
}

int aListBox::findNick(QString str)
{
  int min = 0;
  int max = findSep();
  int last_current = -1;
  int compare;
  int current = min;
  int loop = 0;

  do{
    while(min != max){
      current = (max - min)/2 + min;
      if(current == last_current){
	if(loop == 0)
	  break; // we're looping, so stop
	warning("alistbox: in findNick, looping on second loop for %s",
		str.data());
      }
      
      compare = strncasecmp(text(current), str,
			    QMIN(strlen(text(current)),
				 str.length()));
      if(compare < 0)
	min = current;
      else if(compare > 0)
	max = current;
      else // We got a match
	return current; // Return the match number
      last_current = current;
    }
    min = findSep() - 1;
    max = count();
    current = min;
    loop++;
  } while(loop <= 1);

  return -1; // we've failed to find a match, return -1

}

nickListItem *aListBox::item(int index){
  return (nickListItem *) QListBox::item(index);
}

nickListItem::nickListItem()
  : QListBoxItem()
{
  is_op = FALSE;
  is_voice = FALSE;
}

nickListItem::~nickListItem()
{
}

bool nickListItem::op()
{
  return is_op;
}

bool nickListItem::voice()
{
  return is_voice;
}

void nickListItem::setOp(bool _op)
{
  is_op = _op;
}

void nickListItem::setVoice(bool _voice)
{
  is_voice = _voice;
}

void nickListItem::paint(QPainter *p)
{
  QFontMetrics fm = p->fontMetrics();
  int yPos;                       // vertical text position
  QPen pen = p->pen();
  if(is_voice == TRUE)
    p->setPen(*kSircConfig->colour_chan);
  if(is_op == TRUE)
    p->setPen(*kSircConfig->colour_error);
  yPos = fm.ascent() + fm.leading()/2;
  p->drawText( 3, yPos, text() );
  p->setPen(pen);
}

int nickListItem::height(const QListBox *lb ) const
{
  return lb->fontMetrics().lineSpacing() + 1;
}

int nickListItem::width(const QListBox *lb ) const
{
  return lb->fontMetrics().width( 
				 text() 
				 ) + 6;
}

const char* nickListItem::text() const
{
  return string;
}

const QPixmap* nickListItem::pixmap() const
{
  return 0l;
}

void nickListItem::setText(const char *str)
{
  string = qstrdup(str);
}

nickListItem &nickListItem::operator= (const nickListItem &nli)
{
  string = nli.string;
  is_op = nli.is_op;
  is_voice = nli.is_voice;
  return (*this);
}
