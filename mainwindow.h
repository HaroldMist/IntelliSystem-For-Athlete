#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QTableWidget>
#include <QAction>
#include <QMenu>
#include <QPlainTextEdit>

#include <QVideoWidget>
#include <QMediaPlayer>
#include <QImage>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QSqlRecord>

#include <QProcess>
#include<QRandomGenerator>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void test(); // empty function
    bool createConnection();
    void GetnameList();

    void createMenu();
    void createActions();
    void createToolBar();

    void showNameHead();
    void PlayandShow(QSqlQuery sql_result, int row);
    void showtab(QTableWidget *table, QSqlQuery &sql_table, QString angle);
    void showCombo();

    void showImages();
    void showImage_y(QString filename);

    void playvideo(QString path, QVideoWidget *videoWidget, int pos = 1);

    void setTable(QTableWidget *table);
    void showTable(QTableWidget *table, QSqlQuery result, QStringList tableHead, QString tableName[]);

    void analysisDate(QVector<double> &std);
    void drawGraphic();

    void startDocker();

private slots:
    void on_pauseButton_clicked();
    void on_playButton_clicked();

    void on_query_clicked();
    void on_query2_clicked();

    // 选择视频文件
    void on_pushButton_3_clicked();

    void on_tableView_cellDoubleClicked(int row, int column);
    void on_tableView2_cellDoubleClicked(int row, int column);
    void on_tableView3_cellDoubleClicked(int row, int column);

    void on_tabWidget_2_tabBarClicked(int index);

    void on_pushButton_6_clicked();

    void on_pushButton_5_clicked();

private:
    Ui::MainWindow *ui;
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");

    QStringList nameList;
    QString ComboN;
    QSqlQuery sql_table;
    QSqlQuery sql_table2;
    QSqlQuery sql_table3;

    QMediaPlayer *player = nullptr;
    QMediaPlayer *player1 = nullptr;
    QMediaPlayer *player3_std = nullptr;
    QMediaPlayer *player3_nstd = nullptr;

    QPlainTextEdit *textEdit;
    // file菜单
    QMenu *fileMenu;
    QMenu *helpMenu;
    // 定义file的下拉菜单new open save saveAS exit
    QAction *newAthlete;
    QAction *open;
    QAction *Save;
    QAction *saveAs;
    QAction *exit;
    // 定义help的下拉菜单about aboutQt
    QAction *about;
    QAction *aboutQt;

    QToolBar *fileToolBar;
    QToolBar *editToolBar;

    QFileInfo fileinfo;
    QString filePath;
    QString fileName;
};

#endif // MAINWINDOW_H
