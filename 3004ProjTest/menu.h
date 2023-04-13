/*
    Contributor: Shreya Voore, Nguyen-Hanh Nong
    File name: menu.cpp
    Purpose: Tree object/data structure that is used to store menu information to be displayed and manipulated on the main window
    Source: Adapted from the Denas Case Study Menu class, from Brightspace (COMP3004)
*/

#ifndef MENU_H
#define MENU_H

#include <QString>
#include <QStringList>
#include <QVector>
#include <QDebug>

class Menu
{
public:
    explicit Menu(QString, QVector<QString> itemList, Menu*);
    ~Menu();

    //getters
    QString getName(); //gets the name of the menu
    QVector<QString> getMenuItems(); //returns the menuItems of a menu as QStrings
    Menu* getParentMenu(); //gets the parent menu of the menu
    int getPosition(); //gets the position of the menu in the menu list
    Menu* get(int); //gets the menu at that index in the list
    int getLength(); //gets the number of child menus in the menu
    QVector<Menu*> getChildMenus(); //returns all the menu items as Menu* objects

    //add and removes
    void addChildMenu(Menu*); //adds a child menu as a Menu*
    void addNewMenuItem(QString item); //adds a new menu item from a QString
    void removeMenuItem(int removeIndex); //removes a menu item at the given index

private:
    QString menuName; //name of the menu
    QVector<QString> menuItems; //the child menus of the menu, as a QString list of their names
    int position; //the position of the menu
    QVector<Menu*> childMenus; //the child menus as a Menu* list
    Menu* parent; //the parent menu of the menu
    int length; // the number of child menus in the menu
};

#endif // MENU_H
