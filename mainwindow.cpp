#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QVideoWidget>
#include <QMediaPlayer>
#include <QImage>
#include <QFile>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    createConnection();

    //showtab2();
    showbody();
    playvideo();

    connect(ui->query, &QPushButton::clicked, this, &MainWindow::showtab);
//    connect(ui->pushButton_2, &QPushButton::clicked, this, &MainWindow::select);
//    connect(ui->pushButton_2, &QPushButton::clicked, this, &MainWindow::isEquals);

    //show advice


}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::createConnection(){
    db.setHostName("127.0.0.1");
    db.setPort(3306);
    db.setDatabaseName("dc_db");
    db.setUserName("root");
    db.setPassword("17312767927");
    if (!db.open()) {
        QMessageBox::critical(0, QObject::tr("无法打开数据库"),"无法创建数据库连接！ ", QMessageBox::Cancel);
        return false;
        }
    else{
        statusBar()->showMessage(tr("数据库连接成功"), 5000);
        return true;
    }
};

void MainWindow::showtab(){
    QStringList trainResult;
    QString sql;
    trainResult << "记录视频" << "动作标准度" << "训练日期" <<"桨最后"<<"桨最前";
    QString name[5] = {"video", "std", "date", "p_back", "p_front"};

    sql = "select * from nonstd where (angle = 'right') order by date asc";
    QSqlQuery result = db.exec(sql);
    //setTable(ui->tableView);
    showTable(ui->tableView, result, trainResult, name);
}

void MainWindow::showtab2(){
    QStringList trainResult;
    QString sql;
    trainResult << "记录视频" << "动作标准度" << "训练日期" <<"桨最后"<<"桨最前";
    QString name[5] = {"video", "std", "date", "p_back", "p_front"};


    sql = "select * from nonstd where (angle = 'left') order by date asc";
    QSqlQuery result = db.exec(sql);
    //setTable(ui->tableView2);
    showTable(ui->tableView2, result, trainResult, name);
};

void MainWindow::showtab3(){
    QStringList trainResult;
    QString sql;
    trainResult << "记录视频" << "动作标准度" << "训练日期" <<"桨最后"<<"桨最前";
    QString name[5] = {"video", "std", "date", "p_back", "p_front"};

    sql = "select * from nonstd where (angle = 'front')";
    QSqlQuery result = db.exec(sql);
    //setTable(ui->tableView3);
    showTable(ui->tableView3, result, trainResult, name);
};


void MainWindow::playvideo(){
    //play video
    QMediaPlayer *player = new QMediaPlayer;
    QVideoWidget *videoWidget = ui->video_ns;

    videoWidget->show();

    player->setSource(QUrl::fromLocalFile("E:\\DBVideo\\1_NS_R\\1_1_NS_R\\1_1_NS_R_result.mp4"));
    QFile file("E:\\DBVideo\\NonStd\\Left\\1\\1_1_NS_L.mp4");
        if(!file.open(QIODevice::ReadOnly))
            qDebug() << "Could not open file";

    player->setVideoOutput(videoWidget);
    player->play();

};

void MainWindow::showbody(){
    QString filename=":/prefix1/resource/humanbody.png";
    QImage* img=new QImage,* scaledimg=new QImage;
    if(! ( img->load(filename) ) )
    {
        QMessageBox::information(this, tr("打开图像失败"), tr("打开图像失败!"));
        delete img;
        return;
    }
    int Fwidth,Fheight;

    Fwidth=170;
    Fheight=340;
    *scaledimg=img->scaled(Fwidth,Fheight,Qt::KeepAspectRatio);
    ui->image_body->setPixmap(QPixmap::fromImage(*scaledimg));

}




void MainWindow::setTable(QTableWidget *table)
{
    table->verticalHeader()->setVisible(false);
    QFont font = table->horizontalHeader()->font();
    font.setBold(true);
    table->horizontalHeader()->setFont(font);
    table->setFrameShape(QFrame::NoFrame);
}

void MainWindow::showTable(QTableWidget *table, QSqlQuery result, QStringList tableHead, QString tableName[])
{
    QWidget qw;
    table->setColumnCount(tableHead.size());
    table->setRowCount(0);
    table->setHorizontalHeaderLabels(tableHead);

    int i = 0;
    while(result.next()) {
        table->insertRow(i);
        for(int j = 0; j < tableHead.size(); j++) {
            table->setItem(i, j, new QTableWidgetItem(result.value(tableName[j]).toString()));
            table->item(i, j)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        }
        i++;
    }

    table->resizeColumnsToContents();
    table->resizeRowsToContents();
}

