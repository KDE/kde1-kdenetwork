#include "irclistitem.h"
#include "iostream.h"

ircListItem::ircListItem(QString s, const QColor *c, QListBox *lb, QPixmap *p = 0)
  : QListBoxItem()
{

  text = s;
  colour = (QColor *) c;
  pm = p;
  parent_lb = lb;

  setText(s);

  rows = 1;
  linewidth = 0;

  paint_text = new QStrList();
  
  setupPainterText();

}

void ircListItem::paint(QPainter *p)
{
  QPen pen = p->pen();
  p->setPen(*colour);

  if(pm)
    p->drawPixmap(1,0,*pm);

  char *txt;
  int row = 0;
  for(txt = paint_text->first(); txt != 0; txt = paint_text->next(), row++){
    p->drawText(xPos,yPos+lineheight*row, txt);
  }
  p->setPen(pen);
}

int ircListItem::height(const QListBox *) const
{
  return rows*(lineheight) + 2;
}

int ircListItem::width(const QListBox *) const
{
  return linewidth;
}

int ircListItem::row()
{
  return rows;
}

void ircListItem::setupPainterText()
{

  lineheight = parent_lb->fontMetrics().lineSpacing();
  QFontMetrics fm = parent_lb->fontMetrics();

  if(pm){
    if ( pm->height() < fm.height() )
      yPos = fm.ascent() + fm.leading()/2;
    else
      yPos = pm->height()/2 - fm.height()/2 + fm.ascent(); 

    xPos = pm->width()+5;
  }
  else{
    yPos = fm.ascent() + fm.leading()/2;
    xPos = 3;
  }

  paint_text->clear();

  if(fm.width(text) > (parent_lb->width()-35)){
    uint length = (parent_lb->width()-35) / fm.width("X");
    linewidth = length;
    uint sChar = 0;
    uint eChar = length;
    uint eChar_h;
    rows = 0;
    while(sChar < text.length()){
      if(eChar < text.length()){
	eChar_h = text.findRev(' ', eChar);
	if( !((eChar_h == (uint) -1) || (eChar_h < sChar)))
	  eChar = eChar_h;
      }
      else{
	eChar = text.length();
      }
      paint_text->append(text.mid(sChar, eChar - sChar));
      sChar = eChar + 1;
      eChar += length;
      rows++;
    }
  }
  else{
    rows = 1;
    linewidth = fm.width(text);
    paint_text->append(text);
  }
}
