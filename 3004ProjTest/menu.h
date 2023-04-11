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
    QString getName();
    QVector<QString> getMenuItems();
    Menu* getParentMenu();
    int getPosition();
    Menu* get(int);
    void addChildMenu(Menu*);
    int getLength();

    QVector<Menu*> getChildMenus();
    void addNewMenuItem(QString item);

    void removeMenuItem(int removeIndex);

private:
    QString menuName;
    QVector<QString> menuItems;
    int position;
    QVector<Menu*> childMenus;
    Menu* parent;
    int length;
};

#endif // MENU_H
