#include "objFinder.h"
#include <qobjcoll.h>
#include <stdlib.h>
#include <time.h>

QDict<QObject> *objFinder::objList = new QDict<QObject>;

/*
 * So we can connect to the slots, etc
 */
objFinder *objFind = new objFinder();

objFinder::objFinder()
  : QObject()
{
}

objFinder::~objFinder()
{
}

void objFinder::insert(QObject *obj, const char *key = 0){
  const char *name;

  if(key != 0){
    name = key;
  }
  else {
    name = obj->name();
    if(name == 0){
      name = randString();
    }
  }
  objList->insert(name, obj);
  connect(obj, SIGNAL(destroyed()),
          objFind, SLOT(objDest()));
}

QObject *objFinder::find(const char *name, const char *inherits){
  QObject *found;
  QDictIterator<QObject> it(*objList);
  while(it.current()){
    if(strlen(name) == strlen(it.current()->name()) &&
       strcmp(it.current()->name(), name) == 0)
      return it.current();
    QObjectList *qobl = it.current()->queryList(inherits, name, FALSE);
    QObjectListIt itql( *qobl );
    if(itql.current() != 0x0){
      found = itql.current();
      delete qobl;
      return found;
    }
    delete qobl;
    ++it;
  }
  return 0x0;
}

void objFinder::dumpTree(){
  QDictIterator<QObject> it(*objList);
  while(it.current()){
    it.current()->dumpObjectTree();
    ++it;
  }
}

QString objFinder::randString(){
  static bool runSrand = 0;
  QString str = "";
  if(runSrand == 0){
    srand(time(NULL));
  }
  for(int i = 0; i <= 8; i++){
    str.insert(0, (char) (1+(int) (94.0*rand()/(RAND_MAX+1.0))) + 0x20);
  }
  return str;
}

void objFinder::objDest(){
  if(sender() == 0x0){
    return;
  }
  debug("Destroyed: %s", sender()->name());
  QDictIterator<QObject> it(*objList);
  while(it.current()){
    if(it.current() == sender()){
      objList->remove(it.currentKey());
    }
    ++it;
  }
}
