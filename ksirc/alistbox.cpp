/*******************************************************************

 aListBox

 $$Id$$

 List box that outputs a right click mouse so I can popup a qpopupmenu.

 nothing special.

*******************************************************************/

#include "alistbox.h"
#include <iostream.h>

void aListBox::mousePressEvent(QMouseEvent *e)
{

  QListBox::mousePressEvent(e);

  if(e->button() == RightButton){
    emit rightButtonPress(findItem(mapFromGlobal(cursor().pos()).y()));
  }
}
