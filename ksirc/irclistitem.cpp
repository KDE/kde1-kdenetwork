#include "irclistitem.h"
#include "iostream.h"

#include <stdlib.h> 

const int ircListItem::maxcolour = 17;
const QColor ircListItem::num2colour[17] = {  black,
					      darkRed,
					      darkGreen,
					      darkBlue,
					      darkMagenta,
					      darkCyan,
					      darkYellow,
					      lightGray,
					      darkGray,
					      red,
					      green,
					      blue,
					      magenta,
					      cyan,
					      yellow,
					      white };



ircListItem::ircListItem(QString s, const QColor *c, QListBox *lb, QPixmap *p = 0)
  : QObject(),
    QListBoxItem()
    
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
  QColor bc = p->backgroundColor();
  p->setPen(*colour);

  if(pm)
    p->drawPixmap(1,0,*pm);

  char *txt;
  int row = 0;
  for(txt = paint_text->first(); txt != 0; txt = paint_text->next(), row++){
    //    p->drawText(xPos,yPos+lineheight*row, txt);
    colourDrawText(p, xPos,yPos+lineheight*row, txt);
  }
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

void ircListItem::updateSize(){
  setupPainterText();
}

void ircListItem::colourDrawText(QPainter *p, int startx, int starty,
				 char *str)
{
  int offset = 0;
  int colour;
  char buf[3];
  int loc, i;
  buf[2] = 0;
  
  for(loc = 0; str[loc] != 0x00 ; loc++){
    if(str[loc] == 0x03 || str[loc] == '!'){
      i = loc;
      p->drawText(startx, starty, str + offset, i-offset);
      startx += p->fontMetrics().width(str + offset, i-offset);
      offset = i;
      //      lastp = i;
      if((str[i+1] >= 0x30) && (str[i+1] <= 0x39)){
	i++;
	buf[0] = str[i];
	i++;
	if((str[i] >= 0x30) && (str[i] <= 0x39)){
	  buf[1] = str[i];
	  i++;
	}
	else{
	  buf[1] = 0;
	}
	
	colour = atoi(buf);
	if(colour < maxcolour)
	  p->setPen(num2colour[atoi(buf)]);
	else
	  cerr << "Invalid colour: " << colour << endl;

	if(str[i] == ','){
	  i++;
	  if((str[i] >= 0x30) && (str[i] <= 0x39)){
	    buf[0] = str[i];
	    i++;
	    if((str[i] >= 0x30) && (str[i] <= 0x39)){
	      buf[1] = str[i];
	      i++;
	    }
	    else{
	      buf[1] = 0;
	    }
	    colour = atoi(buf);
	    if(colour < maxcolour){
	      p->setBackgroundColor(num2colour[colour]);
	      p->setBackgroundMode(OpaqueMode);
	    }
	    else
	      cerr << "Invalid colour: " << colour << endl;
	  }
	}
      }
      offset += i - loc;
    }
  }
  p->drawText(startx, starty, str + offset, loc-offset);
}

