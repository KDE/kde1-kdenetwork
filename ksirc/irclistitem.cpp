#include "irclistitem.h"
#include "iostream.h"

ircListItem::ircListItem(QString s, const QColor c, QPixmap *p = 0, QListBox *lb = 0)
  : QListBoxItem()
{

  text = s;
  colour = c;
  pm = p;

  setText(s);

  rows = 1;
  linewidth = 0;


  if(lb){
    rows = 0;
    uint length = (lb->width()-20) / lb->fontMetrics().width("X");
    uint sChar = 0;
    uint eChar = length;
    uint eChar_h;
    uint tlength = text.length();

    while(sChar < tlength){
      if(eChar < tlength){
	eChar_h = text.findRev(' ', eChar);
	if( !((eChar_h == (uint) -1) || (eChar_h < sChar)))
	  eChar = eChar_h;
      }
      else{
	eChar = tlength;
      }
      if((int) (eChar - sChar) > linewidth)
	linewidth = eChar - sChar;
      sChar = eChar + 1;
      eChar += length;
      rows++;
    }
    linewidth = lb->fontMetrics().width("X")*linewidth;
    lineheight = lb->fontMetrics().lineSpacing();
  }
  else{
    lineheight = 0;
  }
  //  cerr << linewidth << endl;

}

void ircListItem::paint(QPainter *p)
{
  QPen pen = p->pen();
  p->setPen(colour);
  QFontMetrics fm = p->fontMetrics();
  int yPos;                       // vertical text position
  int xPos;

  if(pm){
    if ( pm->height() < fm.height() )
      yPos = fm.ascent() + fm.leading()/2;
    else
      yPos = pm->height()/2 - fm.height()/2 + fm.ascent(); 

    p->drawPixmap(1,0,*pm);
    xPos = pm->width()+5;
  }
  else{
    yPos = fm.ascent() + fm.leading()/2;
    xPos = 3;
  }

  if(!lineheight){
     lineheight = p->fontMetrics().lineSpacing();
  }

  if(fm.width(text) > (p->window().width()-20)){
    uint length = (p->window().width()-20) / fm.width("X");
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
      //      cerr << sChar << '-' << eChar << '-' << eChar - sChar << endl;
      p->drawText(xPos,yPos+lineheight*rows, text.mid(sChar, eChar - sChar));
      sChar = eChar + 1;
      eChar += length;
      rows++;
    }
  }
  else{
    rows = 1;
    p->drawText(xPos,yPos, text);
  }
  //    p->drawText(pm->width()+5,yPos,text, text.length());
  //    cerr << pm->width() << endl;
  //  cerr << text << endl;
  p->setPen(pen);
}

int ircListItem::height(const QListBox *lb) const
{
  //  if(pm)
  //    return QMAX( pm->height(), lb->fontMetrics().lineSpacing() + 1 );
  //  else

  if(!lineheight)
    return rows*(lb->fontMetrics().lineSpacing())+2;
  else
    return rows*(lineheight) + 2;
}

int ircListItem::width(const QListBox *lb) const
{
  //  if(pm)
  //    return pm->width() + lb->fontMetrics().width(text) + 1;
  //  else

  if(linewidth > 0)
    return linewidth;
  else
    return (lb->fontMetrics().width(text) + 1);
}

int ircListItem::row()
{
  return rows;
}
