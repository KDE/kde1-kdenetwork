
#include "kpopupmenu.h"

#include <kapp.h>
#include <qkeycode.h>

static int dummy = 0;

KPopupMenu::KPopupMenu ( QWidget * parent=0, const char * name=0 )
    : QPopupMenu(parent, name)
{
  connect(this, SIGNAL(highlighted(int)),
	  this, SLOT(current(int)));
  connect(this, SIGNAL(aboutToShow()),
	  this, SLOT(updateAccel()));
  current_item = -99;
  read_config = false;

  accel = 0x0;
  
//  dummy = insertItem("Dummy Item",
//			 this, SLOT(updateAccel()), CTRL + Key_L);
}

KPopupMenu::~KPopupMenu()
{
}

void KPopupMenu::current(int item)
{
  current_item = item;
}

void KPopupMenu::keyPressEvent(QKeyEvent *e)
{
  if(e->state() & ControlButton){
    e->accept();
    int accel_key = CTRL + e->key();
    setAccel(accel_key, current_item);
    findItem(current_item)->setDirty(true);
//    hide(); show();
    setActiveItem(0);
    setActiveItem(current_item);
    

    KConfig *cnf = kapp->getConfig();
    cnf->setGroup("UserDefinedAccel");
    QString text_id = text(current_item);
    int spaces = text_id.find("\t"); // Search for accel key starts
    if(spaces != -1)
      text_id.truncate(spaces); // and remove it
    QString key_id = text_id + "_" + name("NoName");
    cnf->writeEntry(key_id, accel_key);
  }
  else{
    QPopupMenu::keyPressEvent(e);
  }
}

void KPopupMenu::updateAccel()
{
  if(read_config == false){
    KConfig *cnf = kapp->getConfig();
    cnf->setGroup("UserDefinedAccel");

    for(int i = 0; i < count(); i++){
      int item = idAt(i);
      QString text_id = text(item);
      int spaces = text_id.find("\t"); // Search for accel key starts
      if(spaces != -1)
	text_id.truncate(spaces); // and remove it
      QString key_id = text_id + "_" + name("NoName");
      int key = cnf->readNumEntry(key_id, -1);
      if(key != -1){
	setAccel(key, item);
	QMenuItem *mi = findItem(item);
	mi->setDirty(true);
      }
    }

    
    read_config = true;
  }
}