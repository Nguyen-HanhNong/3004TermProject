#include "menu.h"

Menu::Menu(QString name, QStringList itemList, Menu* parentMenu)
{
    menuName = name;
    menuItems = itemList;
    parent = parentMenu;
    length = 0;
}

Menu::~Menu(){
    for(int i = 0; i<childMenus.length(); i++){
        delete childMenus[i];
    }
}

void Menu::addChildMenu(Menu* m){
    childMenus.push_back(m);
    length++;
}

QString Menu::getName(){
    return this->menuName;
}

QStringList Menu::getMenuItems(){
    return this->menuItems;
}

Menu* Menu::getParentMenu(){
    return this->parent;
}

int Menu::Menu::getPosition(){
    return this->position;
}

Menu* Menu::get(int i){
    return this->childMenus[i];
}

int Menu::getLength(){
    return this->length;
}
