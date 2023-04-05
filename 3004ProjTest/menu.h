#ifndef MENU_H
#define MENU_H

#include <QString>
#include <QStringList>
#include <QVector>


class Menu
{
public:
    explicit Menu(QString, QStringList, Menu*);
    ~Menu();
    QString getName();
    QStringList getMenuItems();
    Menu* getParentMenu();
    int getPosition();
    Menu* get(int);
    void addChildMenu(Menu*);
    int getLength();

private:
    QString menuName;
    QStringList menuItems;
    int position;
    QVector<Menu*> childMenus;
    Menu* parent;
    int length;
};

#endif // MENU_H
