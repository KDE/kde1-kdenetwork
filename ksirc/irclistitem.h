#ifndef IRCLISTITEM_H
#define IRCLISTITEM_H

#include <qcolor.h>  
#include <qlistbox.h>
#include <qpainter.h>  
#include <qpixmap.h>

class ircListItem : public QListBoxItem
{
 public:
  ircListItem(QString s, const QColor c, QPixmap *p=0, QListBox *lb=0);

  virtual int row();
  
protected:
  virtual void paint(QPainter *);
  virtual int height ( const QListBox * ) const;
  virtual int width ( const QListBox * ) const;  

 private:

  QPixmap *pm;
  QColor colour;
  QString text;

  int rows;

  int linewidth;
  int lineheight;

};

#endif
