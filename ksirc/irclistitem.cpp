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



ircListItem::ircListItem(QString s, const QColor *c, QListBox *lb, QPixmap *p = 0, bool _WantColour = FALSE)
  : QObject(),
    QListBoxItem()
    
{

  text = s;
  colour = (QColor *) c;
  pm = p;
  parent_lb = lb;

  WantColour = _WantColour;

  Wrapping = TRUE;

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
  QFont font = p->font();
  p->setPen(*colour);

  if(pm)
    p->drawPixmap(1,0,*pm);

  char *txt;
  int row = 0;
  for(txt = paint_text->first(); txt != 0; txt = paint_text->next(), row++){
    //    p->drawText(xPos,yPos+lineheight*row, txt);
    colourDrawText(p, xPos,yPos+lineheight*row, txt);
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
  if((fm.width(text) > max_width) && (Wrapping == TRUE)){
    int lastp = 0;
    int width = xPos - fm.width(text[0]);
    uint i;
    int ig = 0;
    for(i = 0; i < text.length() ; i++){
      if((text[i] == '~') || (text[i] == 0x03) &&
	 (((text[i+1] >= 0x30) && (text[i+1] <= 0x39)  ||
	   (text[i] == '~') && ((text[i+1] >= 0x61) || (text[i+1] <= 0x7a))))){ // a->z
	if((text[i+1] >= 0x30) && (text[i+1] <= 0x39)){
	  i += 2; 
	  ig += 2;
	  if((text[i] >= 0x30) && (text[i] <= 0x39)){
	    i++;
	    ig++;
	  }
	  if((text[i] == ',') && ((text[i+1] >= 0x30) && (text[i+1] <= 0x39))){
	    i+=2;
	    ig+=2;
	    if((text[i] >= 0x30) && (text[i] <= 0x39)){
	      i++;
	      ig++;
	    }
	  }
	  i--; // Move back on since the i++ moves ahead one.
	}
	else if((text[i] == '~') && ((text[i+1] >= 0x61) || (text[i+1] <= 0x7a))){
	  i += 1;   // Implicit step forward in for loop
	  ig += 2;
	}
      }
      else{
	width += fm.width(text[i]);
	if(width >= max_width){
	  int newi = i;
	  for(; (newi > 0) &&
		(text[newi] != ' ') &&
		(text[newi] != '-') &&
		(text[newi] != '\\'); newi--);
	  if(newi > lastp)
	    i = newi+1;
	  paint_text->append(text.mid(lastp, i-lastp));
	  ig = 0;
	  width = xPos;
	  lastp = i;
	}
      }
    }
    paint_text->append(text.mid(lastp, i-lastp));
    rows = paint_text->count();
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

void ircListItem::setWrapping(bool _wrap){
  Wrapping = _wrap;
  setupPainterText();
}

bool ircListItem::wrapping(){
  return Wrapping;
}

void ircListItem::colourDrawText(QPainter *p, int startx, int starty,
				 char *str)
{
  int offset = 0;
  int pcolour;
  char buf[3];
  int loc = 0, i;
  buf[2] = 0;

  for(loc = 0; str[loc] != 0x00 ; loc++){
    if(str[loc] == 0x03 || str[loc] == '~'){
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
	
	pcolour = atoi(buf);
	if(pcolour < maxcolour)
	  p->setPen(num2colour[pcolour]);
	else
	  i = loc;
	
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
	    int bcolour = atoi(buf);
	    if(pcolour == bcolour){
	      if(bcolour + 1 < maxcolour)
		bcolour += 1;
	      else
		bcolour -= 1;
	    }
	    if(bcolour < maxcolour){
	      p->setBackgroundColor(num2colour[bcolour]);
	      p->setBackgroundMode(OpaqueMode);
	    }
	    else
	      i = loc;
	  }
	}
      }
      else if(str[i] == 0x03){
	i++;
	p->setPen(*colour);
	p->setBackgroundMode(TransparentMode);
      }
      else if((str[i] == '~') && ((str[i+1] >= 0x61) || (str[i+1] <= 0x7a))){
	QFont fnt = p->font();
	switch(str[i+1]){
	case 'c':
	  p->setPen(*colour);
	  p->setBackgroundMode(TransparentMode);
	  break;
	case 'b':
	  if(fnt.bold() == TRUE)
	    fnt.setBold(FALSE);
	  else
	    fnt.setBold(TRUE);
	  break;
	case 'i':
	  if(fnt.italic() == TRUE)
	    fnt.setItalic(FALSE);
	  else
	    fnt.setItalic(TRUE);
	  break;
	case 'u':
	  if(fnt.underline() == TRUE)
	    fnt.setUnderline(FALSE);
	  else
	    fnt.setUnderline(TRUE);
	  break;
	default:
	  i-=1;
	  offset -= 1;
	}
	p->setFont(fnt);
	i += 2;
      }
      offset += i - loc;
    }
  }
  p->drawText(startx, starty, str + offset, loc-offset);
}

