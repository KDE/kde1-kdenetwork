#ifndef IRCLISTITEM_H
#define IRCLISTITEM_H

#include <qobject.h>
#include <qcolor.h>  
#include <qlistbox.h>
#include <qpainter.h>  
#include <qpixmap.h>
#include <qstrlist.h>


class ircListItem : public QObject, 
		    public QListBoxItem
{
 Q_OBJECT;
 public:
  ircListItem(QString s, const QColor *c, QListBox *lb, QPixmap *p=0, bool _WantColour = FALSE);

  virtual int row();

  QString getText(){
    return text;
  }

public slots:
  virtual void updateSize();
  
protected:
  virtual void paint(QPainter *);
  virtual int height ( const QListBox * ) const;
  virtual int width ( const QListBox * ) const;  
  
  virtual void setupPainterText();

  virtual void colourDrawText(QPainter *p, int startx, int starty, char *str);

private:

  QPixmap *pm;
  QColor *colour;
  QString text;

  int rows;

  int linewidth;
  int lineheight;

  QStrList *paint_text;
  QListBox *parent_lb;
  int yPos;
  int xPos;

  bool WantColour;
  static const int maxcolour;
  static const QColor num2colour[17];
  

};

#endif
