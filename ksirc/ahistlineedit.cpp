#include "ahistlineedit.h"
#include <iostream.h>

aHistLineEdit::aHistLineEdit(QWidget *parent=0, const char *name=0)
  : QLineEdit(parent, name)
{
  current = 0;
  hist.append(""); // Set the current as blank
}

void aHistLineEdit::keyPressEvent ( QKeyEvent *e )
{
  switch(e->key()){
  case Key_Up:
    if(hist.at() == current){ // same as Key_Down
      hist.remove(current);
      hist.insert(current, text());
    }
    if(hist.at() < 1){ // Same idea as Key_Down so look there for an
      hist.last();     // explanation
      setText(hist.current());
    }
    else
      setText(hist.prev()); // Set text to prev item
    break;
  case Key_Down:
    if(hist.at() == current){ // If we're leaving the active line, save it
      hist.remove(current);   // remove last line
      hist.insert(current, text()); // update it to the current line
    }
    if(hist.at() >= (int) hist.count()-1){ // If we're moving byond the end
      hist.first();                        // roll to first one
      setText(hist.current());             // set text to first line
    }
    else
      setText(hist.next());                // Set text to the next one
    break;
  case Key_Return:
  case Key_Enter:
    if(hist.count() > 20){
      hist.removeFirst(); // more than 20 entries? ick
      current--;          // backup counter
    }
    if(text() != ""){
      hist.remove(current); // remove the "dummy" entry
      hist.append(text()); // add the current text to the end
      hist.append(""); // set the next line to blank
      current++; // move ahead one.
    }
  default:
    QLineEdit::keyPressEvent(e);
  }
}

void aHistLineEdit::focusInEvent(QFocusEvent *)
{
  emit gotFocus();
}

void aHistLineEdit::focusOutEvent(QFocusEvent *)
{
  emit lostFocus();
}
