#ifndef PMENUDATA_H
#define PMENUDATA_H

class PMenuData;

#include <qobject.h>
#include <qmenudta.h>
#include "pmessage.h"
#include "pframe.h"
#include "pobject.h"
#include "controller.h"

// Init and setup code
extern "C" {
PObject *createWidget(CreateArgs &ca);
}

/**
 * Little helper class here gives us access to needed info inside
 * QMenuData
 */

class PSubMenuData : public QMenuData
{
    Q_OBJECT
public:
    PSubMenuData(const QMenuData &obj)
    {
        *this = obj;
    }
    ~PSubMenuData() {}
    operator =(const QMenuData &obj)
    {
        memcpy((QMenuData *) this, &obj, sizeof(QMenuData));
        return 1;
    }

    int active()
    {
        return QMenuData::actItem;
    }
    
private:
    PSubMenuData();
};

class PSubObject : public PObject
{
    Q_OBJECT
public:
    PSubObject(const PObject &obj)
    {
        *this = obj;
    }
    operator =(const PObject &obj)
    {
        memcpy((PObject *) this, &obj, sizeof(PObject));
        return 1;
    }


    void outputMessage(int fd, PukeMessage *pm)
    {
        emit outputMessage(fd, pm);
    }
private:
    PSubOjbect();
};

/**
 * We're subclased off QMenuData so we can access it's internal proteted vars
 * We do not initialize NOR create it!!!!
 */

class PMenuData
{
 public:
  PMenuData (PObject *_child);
  virtual ~PMenuData ();
  
  virtual bool messageHandler(int fd, PukeMessage *pm);

  virtual void setWidget(QMenuData *_qmd);
  virtual QMenuData *widget();

  virtual int activeItem()
  {
      PSubMenuData psub( *((QMenuData *) child->widget()));
      return psub.active();
  }

 private:
   PObject *child;
};

#endif
