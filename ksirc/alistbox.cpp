/*******************************************************************

 aListBox

 $$Id$$

 List box that outputs a right click mouse so I can popup a qpopupmenu.

 nothing special.

*******************************************************************/

#include "alistbox.h"
#include "linelistitem.h"
#include <iostream.h>

// SEP has to be something that's never in a nick, !'s are not allowed in nicks

// Short so the strpcmp is quick

#define SEP "!!!"

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
  lineListItem *line = new lineListItem(this, red);
  line->setText(SEP);
  insertItem(line, 0);
}


void aListBox::inSort ( const QListBoxItem *lbi, bool top = FALSE)
{
  int min = -1, max = count() - 1;

  int sep = findSep();

  if(sep > 0){
    if(top == TRUE){
      min = 0;
      max = sep - 1;
    }
    else{
      min = sep + 1;
      max = count();
    }
  }
  if(min < (int) count())
    while((strncasecmp(text(min), lbi->text(), 
		       QMIN(strlen(text(min)), strlen(lbi->text()))) < 0) && 
	   (min < max))
      min++;
  else
    min = -1;

  insertItem(lbi, min);
}

void aListBox::inSort ( const char * text, bool top = FALSE)
{
  inSort(new QListBoxText(text), top);
}

int aListBox::findSep()
{
  bool found = FALSE;
  uint i = 0;
  for(; (i < count()) && (found == FALSE); i++){
    if(strcmp(text(i), SEP) == 0){
      found = TRUE;
    }
  }
  if(found == TRUE)
    return i;
  else
    return -1;

}

bool aListBox::isTop(int index)
{
  if(index >= findSep())
    return FALSE;
  else
    return TRUE;
}

