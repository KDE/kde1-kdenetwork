#ifndef _KPOPUPMENU_H_
#define _KPOPUPMENU_H_

#define INCLUDE_MENUITEM_DEF 1

#include <qpopupmenu.h>
#include <qaccel.h> 

class KPopupMenu : public QPopupMenu
{
    Q_OBJECT
public:
    KPopupMenu ( QWidget * parent=0, const char * name=0 );
    virtual ~KPopupMenu ();

protected slots:
  virtual void current(int id);
  virtual void updateAccel();

protected:
    virtual void keyPressEvent(QKeyEvent *);


private:
  int current_item;
  bool read_config;

  QAccel *accel;
};

#endif