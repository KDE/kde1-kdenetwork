#ifndef _KPOPUPMENU_H_
#define _KPOPUPMENU_H_

#define INCLUDE_MENUITEM_DEF 1

#include <qpopupmenu.h>
#include <qevent.h>
#include <qintdict.h>

class accelItem;
class KGAccel;
class KPopupMenu;

class accelItem
{
public:
  accelItem(int i, KPopupMenu *k){
      identifier = i;
      kp = k;
  }

  int id(){
    return identifier;
  }

  KPopupMenu *menu(){
      return kp;
  }
  
private:
    int identifier;
    KPopupMenu *kp;
};

class KGAccel : public QObject
{
    Q_OBJECT
public:
    KGAccel(QObject * parent = 0, const char * name=0 );
    virtual ~KGAccel();

    virtual bool eventFilter(QObject *, QEvent *);
    virtual void insertAccel(int key, int id, KPopupMenu *);
    virtual void removeAccel(int key);
    virtual void clearAccelForPopup(KPopupMenu *);

signals:
    void updateAccel();

private:
    QIntDict<accelItem> *accelDict;
    
};

class KPopupMenu : public QPopupMenu
{
    Q_OBJECT
public:
    KPopupMenu ( QWidget * parent=0, const char * name=0 );
    virtual ~KPopupMenu ();

    virtual void show();
    virtual void hide();

    static bool popVisible() { return visible; }

    virtual void activated(int id);
    virtual void clearAccel(int id);

protected slots:
  virtual void current(int id);
  virtual void updateAccel();

protected:
  virtual void keyPressEvent(QKeyEvent *);

  virtual QString keyToString(int);


private:
  int current_item;
  bool read_config;
  
  int ccount;

  bool ignoreAlt;

  static bool visible;
  static KGAccel *KGA;

};

#endif