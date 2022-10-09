#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "show_db.h"
#include <QVideoWidget>
#include <QMediaPlayer>
#include <QFile>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    createConnection();

    //set column name
    QSqlTableModel *model = new QSqlTableModel;
        model->setTable("nonstd");
        model->setEditStrategy(QSqlTableModel::OnManualSubmit);
        model->select();
        model->setHeaderData(1, Qt::Horizontal, tr("分析角度"));
        model->setHeaderData(2, Qt::Horizontal, tr("记录视频"));
        model->setHeaderData(7, Qt::Horizontal, tr("动作标准度"));

    //set column to show
    QTableView *view = ui->tableView;
        view->setModel(model);

        view->hideColumn(0);    //name
        view->hideColumn(1);    //angle
        view->hideColumn(2);    //video
        view->hideColumn(3);    //o_video
        view->hideColumn(4);    //p_back
        view->hideColumn(5);    //p_front
        view->hideColumn(6);    //p_high
        // view->hideColumn(7);   //std
        view->hideColumn(8);   //j_ru
        view->hideColumn(9);   //j_chu
        view->hideColumn(10);   //j_high
        view->hideColumn(11);   //p_left_wrist

        view->show();

    //play video
    QMediaPlayer *player = new QMediaPlayer;
    QVideoWidget *videoWidget = ui->video_ns;

    videoWidget->show();

    player->setSource(QUrl::fromLocalFile("E:\\DBVideo\\NonStd\\Left\\1\\1_1_NS_L.mp4"));
    QFile file("E:\\DBVideo\\NonStd\\Left\\1\\1_1_NS_L.mp4");
        if(!file.open(QIODevice::ReadOnly))
            qDebug() << "Could not open file";

    player->setVideoOutput(videoWidget);
    player->play();

    //show advice


}

MainWindow::~MainWindow()
{
    delete ui;
}



