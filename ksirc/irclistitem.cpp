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
  totalheight = 0;

  paint_text = new QStrList();

  dbuffer = new QPixmap();
  need_update = TRUE;

  old_height = old_width = 0;
  
  setupPainterText();

}

ircListItem::~ircListItem()
{
  delete paint_text;
  delete dbuffer;
  itext.truncate(0);
  
}

void ircListItem::paint(QPainter *p)
{
  if(need_update == TRUE)
    setupPainterText();
  p->drawPixmap(0,0, *dbuffer);
}

int ircListItem::height(const QListBox *) const
{
  return  totalheight;
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
    //    linewidth = fm.width(itext);
    paint_text->append(itext);
  }
  linewidth = parent_lb->width()-35; // set out width to the parent width.
  totalheight =  rows*(lineheight) + 2;

  // Setup the QPixmap's size and colours if it's null.  When sizing
  // changes, QPixmap is changed in updateSize().  rows which is used
  // by width() and height() is set in the prior line sizing, so NEVER
  // EVER call width and height before this point.

  if((old_width != width(0)) ||
     (old_height != height(0))){
    old_width = width(0);
    old_height = height(0);
    dbuffer->resize(old_width + 35, old_height);
    dbuffer->fill(parent_lb->backgroundColor());
  }
  
  // Print everything to the pixmap so when a paint() comes along
  // we just spit the pixmap out.

  QPainter p;
  if((dbuffer->isNull() == FALSE) && 
     (p.begin(dbuffer) == TRUE)){
    need_update = FALSE;

    p.setFont(parent_lb->font());
    p.setPen(*colour);
    
    if(pm)
      p.drawPixmap(1,0,*pm);
    
    char *txt;
    int row = 0;
    for(txt = paint_text->first(); txt != 0; txt = paint_text->next(), row++){
      KSPainter::colourDrawText(&p, xPos,yPos+lineheight*row, txt);
    }
    
    p.end();
  }
  else{
    cerr << "Start failed!\n";
    if(dbuffer->isNull())
      cerr << "dbuffer is NULL!!!!!!\n";
    dbuffer->resize(1,1);
    need_update = TRUE;
  }
    
    
}

void ircListItem::updateSize(){
  setupPainterText();
}

void ircListItem::setWrapping(bool _wrap){
  Wrapping = _wrap;
  setupPainterText();
}

inline bool ircListItem::wrapping(){
  return Wrapping;
}
