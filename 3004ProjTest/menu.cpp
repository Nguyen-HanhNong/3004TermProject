#include "menu.h"

Menu::Menu(QString name, QVector<QString> itemList, Menu* parentMenu)
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

QVector<QString> Menu::getMenuItems(){
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

QVector<Menu*> Menu::getChildMenus(){
    return this->childMenus;
}

void Menu::addNewMenuItem(QString item){
    menuItems.push_back(item);
}

void Menu::removeMenuItem(int removeIndex){
    QVector<QString> newMenuItems = QVector<QString>();
    
    for(int i = 0; i<menuItems.length(); i++){
        if(i != removeIndex){
            newMenuItems.push_back(menuItems[i]);
        }
    }

    menuItems = newMenuItems;
}
