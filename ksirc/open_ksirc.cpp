/**********************************************************************

	--- Qt Architect generated file ---

	File: open_ksirc.cpp
	Last generated: Wed Nov 26 22:18:02 1997

 *********************************************************************/

#include "open_ksirc.h"
#include "ksircprocess.h"
#include <iostream.h>

#define Inherited open_ksircData

extern KConfig *kConfig;

open_ksirc::open_ksirc
(
	QWidget* parent,
	const char* name
)
	:
	Inherited( parent, name )
{
	setCaption( "Connect to Server" );
	nameSLE->setFocus();
	okButton->setDefault(TRUE);
	okButton->setAutoDefault(TRUE);
	kConfig->setGroup("ServerList");
	int items = kConfig->readNumEntry("Number");
	QString server;
	QString cindex;
	for(int i = 0; i < items; i++){
	  cindex.setNum(i);
	  server = "Server-" + cindex;
	  nameSLE->insertItem(kConfig->readEntry(server), -1);
	}
	nameSLE->setCurrentItem(0);
}


open_ksirc::~open_ksirc()
{
  kConfig->setGroup("ServerList");
  int items = nameSLE->count();
  kConfig->writeEntry("Number", items);
  QString server;
  QString cindex;
  for(int i = 0; i < items; i++){
    cindex.setNum(i);
    server = "Server-" + cindex;
    kConfig->writeEntry(server, nameSLE->text(i));
  }
}

void open_ksirc::create_toplevel()
{
  hide();
  int item_found = 0;
  for(int i = 0; i < nameSLE->count() ; i++){
    if(strcmp(nameSLE->currentText(), nameSLE->text(i)) == 0){
      item_found = 1;
      //cerr << "Found one!\n";
    }
    //cerr << "Checking!\n";
  }
  if(item_found == 0){
    nameSLE->insertItem(nameSLE->currentText(), -1);
    //cerr << "Inserted!\n";
  }

  if(strlen(nameSLE->currentText()) > 0)
    emit open_ksircprocess( (QString) nameSLE->currentText());

  close(TRUE);

}

