#ifndef IRCLISTITEM_H
#define IRCLISTITEM_H

#include <qobject.h>
#include <qcolor.h>  
#include <qlistbox.h>
#include <qpainter.h>  
#include <qpixmap.h>
#include <qstrlist.h>

#include "kspainter.h"

class ircListItem : public QObject, 
		    public QListBoxItem
{
 Q_OBJECT;
 public:
  ircListItem(QString s, const QColor *c, QListBox *lb, QPixmap *p=0);
  ~ircListItem();

  virtual int row();

  QString getText(){
    return itext;
  }

  void setWrapping(bool);
  inline bool wrapping();

  const QColor *defcolour(){
    return colour;
  }

public slots:
  virtual void updateSize();
  virtual void freeze(bool);
  
protected:
  virtual void paint(QPainter *);
  virtual int height ( const QListBox * ) const;
  virtual int width ( const QListBox * ) const;  
  
  virtual void setupPainterText();

private:

  QPixmap *pm;
  QPixmap *dbuffer;
  bool need_update;
  int old_height, old_width;
  const QColor *colour;
  QString itext;

  int rows;

  int linewidth;
  int lineheight;
  int totalheight;

  QStrList *paint_text;
  QListBox *parent_lb;
  int yPos;
  int xPos;

  bool Wrapping;
  bool frozen;


};

#endif
