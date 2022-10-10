#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QTableWidget>
#include <QAction>
#include <QMenu>
#include <QPlainTextEdit>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QSqlRecord>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    bool createConnection();
    
    void showtab();
    void showtab2();
    void showtab3();
    void showCombo();

    void showbody();
    void playvideo();
    void createMenu();
    void createActions();
    void createToolBar();

    void test();

    void setTable(QTableWidget *table);
    void showTable(QTableWidget *table, QSqlQuery result, QStringList tableHead, QString tableName[]);

private:
    Ui::MainWindow *ui;
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");

     //文本框
    QPlainTextEdit *textEdit;
    //file菜单
    QMenu * fileMenu;
    QMenu * helpMenu;
    //定义file的下拉菜单new open save saveAS exit
    QAction *newAthlete;
    QAction *open;
    QAction *Save;
    QAction *saveAs;
    QAction *exit;
    //定义help的下拉菜单about aboutQt
    QAction *about;
    QAction *aboutQt;

    QToolBar *fileToolBar;
    QToolBar *editToolBar;

};

#endif // MAINWINDOW_H
