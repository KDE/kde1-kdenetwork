/**********************************************************************

	--- Qt Architect generated file ---

	File: UserMenuRef.cpp
	Last generated: Fri Nov 21 13:01:21 1997

 *********************************************************************/

#include "UserMenuRef.h"

#define Inherited UserMenuRefData

UserMenuRef::UserMenuRef
(
        QList<UserControlMenu> *_user_menu,
	QWidget* parent,
	const char* name
)
	:
	Inherited( parent, name )
{
  user_menu = _user_menu;

  updateMainListBox();

  MainListBox->setCurrentItem(0);
  newHighlight(0);

}


UserMenuRef::~UserMenuRef()
{
}

void UserMenuRef::newHighlight(int index)
{

  UserControlMenu *ucm;

  ucm = user_menu->at(index);

  if(ucm->type == UserControlMenu::Text){
    MenuName->setEnabled(TRUE);
    MenuCommand->setEnabled(TRUE);
    MenuOpOnly->setEnabled(TRUE);
    MenuName->setText(ucm->title);
    MenuCommand->setText(ucm->action);
    MenuType->setCurrentItem(UserControlMenu::Text);
    MenuOpOnly->setChecked(ucm->op_only);
  }
  else if(ucm->type == UserControlMenu::Seperator){
    MenuName->setText("");
    MenuCommand->setText("");
    MenuType->setCurrentItem(UserControlMenu::Seperator);
    MenuOpOnly->setChecked(FALSE);

    MenuName->setEnabled(FALSE);
    MenuCommand->setEnabled(FALSE);
    MenuOpOnly->setEnabled(FALSE);
  }
      

}

void UserMenuRef::insertMenu()
{

  int newitem = MainListBox->currentItem() + 1;

  if(MenuType->currentItem() == UserControlMenu::Text){
    user_menu->insert(newitem,
		      new UserControlMenu(qstrdup(MenuName->text()),
					  qstrdup(MenuCommand->text()),
					  -1,
					  (int) UserControlMenu::Text,
					  MenuOpOnly->isChecked()));
  }
  else if(MenuType->currentItem() == UserControlMenu::Seperator){
    user_menu->insert(newitem,
			  new UserControlMenu);
  }
      
  updateMainListBox();
  MainListBox->setCurrentItem(newitem);
  newHighlight(newitem);
}

void UserMenuRef::updateMainListBox()
{
  UserControlMenu *ucm;

  MainListBox->setAutoUpdate(FALSE);
  MainListBox->clear();

  for(ucm = user_menu->first(); ucm != 0; ucm = user_menu->next()){
    if(ucm->type == UserControlMenu::Text)
      MainListBox->insertItem(ucm->title);
    else if(ucm->type == UserControlMenu::Seperator)
      MainListBox->insertItem("--------");
  }

  MainListBox->setAutoUpdate(TRUE);
  MainListBox->repaint(TRUE);

  emit updateMenu();

}

void UserMenuRef::typeSetActive(int index)
{


  if(index == UserControlMenu::Text){
    MenuName->setEnabled(TRUE);
    MenuCommand->setEnabled(TRUE);
    MenuOpOnly->setEnabled(TRUE);
  }
  else if(index == UserControlMenu::Seperator){
    MenuName->setText("");
    MenuCommand->setText("");
    MenuOpOnly->setChecked(FALSE);
    MenuName->setEnabled(FALSE);
    MenuCommand->setEnabled(FALSE);
    MenuOpOnly->setEnabled(FALSE);
  }
      
}

void UserMenuRef::deleteMenu()
{

  int currentitem = MainListBox->currentItem();

  user_menu->remove(currentitem);

  updateMainListBox();

  MainListBox->setCurrentItem(currentitem);


}

void UserMenuRef::modifyMenu()
{
  int newitem = MainListBox->currentItem();

  user_menu->remove(newitem);

  if(MenuType->currentItem() == UserControlMenu::Text){
    user_menu->insert(newitem,
		      new UserControlMenu(qstrdup(MenuName->text()),
					  qstrdup(MenuCommand->text()),
					  -1,
					  (int) UserControlMenu::Text,
					  MenuOpOnly->isChecked()));
  }
  else if(MenuType->currentItem() == UserControlMenu::Seperator){
    user_menu->insert(newitem,
			  new UserControlMenu);
  }
      
  updateMainListBox();
  MainListBox->setCurrentItem(newitem);
}
