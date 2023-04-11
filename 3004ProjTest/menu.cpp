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

QVector<Menu*> Menu::getChildMenus(){
    return this->childMenus;
}

void Menu::addNewMenuItem(QString item){
    menuItems.push_back(item);

    qDebug("size of menuItems: %d", menuItems.length());
    qDebug("What are all elements inside menuItems: %s", menuItems.join(" ").toStdString().c_str());
}

void Menu::removeMenuItem(QString removedElement){
    //none of thsi works for some reason
    // qDebug("size of menuItems: %d", menuItems.length());
    // qDebug("made it past first lenght call");

    // for (int index = 0; index < menuItems.length(); index++)  {
    //     if (menuItems[index] == removedElement){
    //         menuItems.removeAt(index);
    //         return;
    //     }
    // }
}
