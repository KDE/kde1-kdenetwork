#include "objFinder.h"
#include <qobjcoll.h>
#include <qwidcoll.h>
#include <stdlib.h>
#include <time.h>
#include <iostream.h>


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

  if(obj == 0x0){
    warning("objFinder: Passed Null Object");
    return;
  }
  
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
  
  emit objFind->inserted(obj);
}

QObject *objFinder::find(const char *name, const char *inherits){
  QObject *found;
  QDictIterator<QObject> it(*objList);
  uint len = strlen(name);
  while(it.current()){
    if(len == strlen(it.current()->name()) &&
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
  QWidgetList *all = QApplication::allWidgets();
  QWidgetListIt itW(*all);
  while(itW.current()){
    if(len == strlen(itW.current()->name()) &&
       strcmp(itW.current()->name(), name) == 0){
      if(inherits != 0x0 && itW.current()->inherits(inherits) == FALSE){
        ++itW;
        continue;
      }
      found = itW.current();
      delete all;
      return found;
    }
    ++itW;
  }

  
  return 0x0;
}

void objFinder::dumpTree(){
  QDictIterator<QObject> it(*objList);
  while(it.current()){
    it.current()->dumpObjectTree();
    ++it;
  }
  QWidgetList *all = QApplication::allWidgets();
  QWidgetListIt itW(*all);
  while(itW.current()){
    cerr << itW.current()->className() << "::" << itW.current()->name("unnamed") << endl;
    ++itW;
  }

}

QStrList objFinder::allObjects(){
  QStrList allNames;
  QDictIterator<QObject> it(*objList);
  while(it.current()){
    QObjectList *qobl = it.current()->queryList(); // Matches everything
    QObjectListIt itql( *qobl );
    while(itql.current()){
      QString name;
      name = itql.current()->className();
      name += "::";
      name += itql.current()->name("unnamed");
      allNames.append(name);
      ++itql;
    }
    delete qobl;
    ++it;
  }
  QWidgetList *all = QApplication::allWidgets();
  QWidgetListIt itW(*all);
  while(itW.current()){
    QString name;
    name = itW.current()->className();
    name += "::";
    name += itW.current()->name("unnamed");
    allNames.append(name);
    ++itW;
  }
  delete all;
  return allNames;
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
