#include "irclistitem.h"
#include "iostream.h"

#include <stdlib.h> 

ircListItem::ircListItem(QString s, const QColor *c, QListBox *lb, QPixmap *p = 0)
  : QObject(),
    QListBoxItem()
    
{

  itext = s.data();
  setText(s);
  colour = c;
  pm = p;
  parent_lb = lb;


  Wrapping = TRUE;

  rows = 1;
  linewidth = 0;

  paint_text = new QStrList();
  
  setupPainterText();

}

ircListItem::~ircListItem()
{
  delete paint_text;
  itext.truncate(0);
  
}

void ircListItem::paint(QPainter *p)
{
  QPen pen = p->pen();
  QColor bc = p->backgroundColor();
  QFont font = p->font();
  p->setPen(*colour);

  if(pm)
    p->drawPixmap(1,0,*pm);

  char *txt;
  int row = 0;
  for(txt = paint_text->first(); txt != 0; txt = paint_text->next(), row++){
    //    p->drawText(xPos,yPos+lineheight*row, txt);
    KSPainter::colourDrawText(p, xPos,yPos+lineheight*row, txt);
  }
  p->setFont(font);
  p->setBackgroundMode(TransparentMode);
  p->setPen(pen);
  p->setBackgroundColor(bc);
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

  // Wrapping code is a little slow, and a little silly, but it works.

  // Main idea is this:
  // Go through each character, find it's width and add it, when it goes
  // beyond the max widith, time for new line.
  //
  // We skip over all !<>,<> time constructs.
  

  paint_text->clear();
  int max_width = parent_lb->width()-35;
  if((fm.width(itext) > max_width) && (Wrapping == TRUE)){
    int lastp = 0;
    int width = xPos - fm.width(itext[0]);
    uint i;
    int ig = 0;
    for(i = 0; i < itext.length() ; i++){
      if((itext[i] == '~') || (itext[i] == 0x03) &&
	 (((itext[i+1] >= 0x30) && (itext[i+1] <= 0x39)  ||
	   (itext[i] == '~') && ((itext[i+1] >= 0x61) || (itext[i+1] <= 0x7a))))){ // a->z
	if((itext[i+1] >= 0x30) && (itext[i+1] <= 0x39)){
	  i += 2; 
	  ig += 2;
	  if((itext[i] >= 0x30) && (itext[i] <= 0x39)){
	    i++;
	    ig++;
	  }
	  if((itext[i] == ',') && ((itext[i+1] >= 0x30) && (itext[i+1] <= 0x39))){
	    i+=2;
	    ig+=2;
	    if((itext[i] >= 0x30) && (itext[i] <= 0x39)){
	      i++;
	      ig++;
	    }
	  }
	  i--; // Move back on since the i++ moves ahead one.
	}
	else if((itext[i] == '~') && ((itext[i+1] >= 0x61) || (itext[i+1] <= 0x7a))){
	  i += 1;   // Implicit step forward in for loop
	  ig += 2;
	}
      }
      else{
	width += fm.width(itext[i]);
	if(width >= max_width){
	  int newi = i;
	  for(; (newi > 0) &&
		(itext[newi] != ' ') &&
		(itext[newi] != '-') &&
		(itext[newi] != '\\'); newi--);
	  if(newi > lastp)
	    i = newi+1;
	  paint_text->append(itext.mid(lastp, i-lastp));
	  ig = 0;
	  width = xPos;
	  lastp = i;
	}
      }
    }
    paint_text->append(itext.mid(lastp, i-lastp));
    rows = paint_text->count();
  }
  else{
    rows = 1;
    linewidth = fm.width(itext);
    paint_text->append(itext);
  }
}

void ircListItem::updateSize(){
  setupPainterText();
}

void ircListItem::setWrapping(bool _wrap){
  Wrapping = _wrap;
  setupPainterText();
}

bool ircListItem::wrapping(){
  return Wrapping;
}
