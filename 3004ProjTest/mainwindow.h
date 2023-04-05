#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>

#include "menu.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    //menu related
    Menu* mainMenu;
    Menu* parentMenu;

    QListWidget *curQListWidget;
    void updateMenu(const QString, const QStringList);
    void initializeMainMenu(Menu*);

private slots:
    void upButton();
    void downButton();
    void select();
    void menu();
    void goBack();
//    void childMenu();
//    void mainMenu();
//    void backMenu();


};
#endif // MAINWINDOW_H
