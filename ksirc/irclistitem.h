#ifndef IRCLISTITEM_H
#define IRCLISTITEM_H

#include <qcolor.h>  
#include <qlistbox.h>
#include <qpainter.h>  
#include <qpixmap.h>
#include <qstrlist.h>


class ircListItem : public QListBoxItem
{
 public:
  ircListItem(QString s, const QColor c, QListBox *lb, QPixmap *p=0);

  virtual int row();
  
protected:
  virtual void paint(QPainter *);
  virtual int height ( const QListBox * ) const;
  virtual int width ( const QListBox * ) const;  
  
  virtual void setupPainterText();

private:

  QPixmap *pm;
  QColor colour;
  QString text;

  int rows;

  int linewidth;
  int lineheight;

  QStrList *paint_text;
  QListBox *parent_lb;
  int yPos;
  int xPos;

};

#endif
