/*
 *            kPPP: A pppd front end for the KDE project
 *
 * $Id$
 * 
 *            Copyright (C) 1997 Bernd Johannes Wuebben 
 *                   wuebben@math.cornell.edu
 *
 * based on EzPPP:
 * Copyright (C) 1997  Jay Painter
 *
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this program; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "pppdargs.h"

PPPdArguments::PPPdArguments(QWidget *parent, const char *name)
  : QDialog(parent, name, TRUE)
{
  setCaption(klocale->translate("Customize pppd Arguments"));

  argument = new QLineEdit(this);
  argument->setGeometry(95, 15, 170, 25);
  connect(argument, SIGNAL(returnPressed()), SLOT(addbutton()));

  argument_label = new QLabel(this);
  argument_label->setGeometry(15,15,65,25);
  argument_label->setText(klocale->translate("Argument:"));

  add = new QPushButton(klocale->translate("Add"), this);
  add->setGeometry(15, 55, 110, add->sizeHint().height());
  connect(add, SIGNAL(clicked()), SLOT(addbutton()));

  remove = new QPushButton(klocale->translate("Remove"), this);
  remove->setGeometry(155, 55, 110, remove->sizeHint().height());
  connect(remove, SIGNAL(clicked()), SLOT(removebutton()));

  arguments = new QListBox(this);
  arguments->setGeometry(15, 105, 250, 100);

  closebtn = new QPushButton(klocale->translate("Close"), this);
  closebtn->setGeometry(15, 225, 110, closebtn->sizeHint().height());
  connect(closebtn, SIGNAL(clicked()), SLOT(closebutton()));

  defaults = new QPushButton(klocale->translate("Restore Defaults"), this);
  defaults->setGeometry(155, 225, 110, defaults->sizeHint().height());
  connect(defaults, SIGNAL(clicked()), SLOT(defaultsbutton()));

  setFixedSize(280,270);
  //load info from gpppdata
  init();
}



//
//private slots
//
void PPPdArguments::addbutton() {
  if(strcmp(argument->text(), "") != 0 &&
     arguments->count() < MAX_PPPD_ARGUMENTS) {
    arguments->insertItem(argument->text());
    argument->setText("");
  }
}


void PPPdArguments::removebutton() {
  if(arguments->currentItem() >= 0)
    arguments->removeItem(arguments->currentItem());
}


void PPPdArguments::defaultsbutton() {
  gpppdata.setpppdArgumentDefaults();
  init();
}


void PPPdArguments::closebutton() {
  if(arguments->count() > 0) {
    for(uint i=0; i <= arguments->count()-1; i++)
      gpppdata.setpppdArgument(i, arguments->text(i));
  }
  else {
    gpppdata.setpppdArgument(0, 0L);
  }

  done(0);
}

//
//private funcitons
//

void PPPdArguments::init() {
  arguments->clear();

  for(int i=0; gpppdata.pppdArgument(i) && 
	i <= MAX_PPPD_ARGUMENTS-1; i++) {
    arguments->insertItem(gpppdata.pppdArgument(i));
  }
}

#include "pppdargs.moc"
