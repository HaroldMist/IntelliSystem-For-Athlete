#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    createConnection();

    GetnameList(); // get the namelist in database

    createActions();
    createMenu();

    ui->label->setText("未选择");
    showCombo();

    showImages();

    connect(ui->query, &QPushButton::clicked, this, &MainWindow::showtab);
    connect(ui->query, &QPushButton::clicked, this, &MainWindow::showtab2);
    connect(ui->query, &QPushButton::clicked, this, &MainWindow::showtab3);

    connect(ui->analysis, &QPushButton::clicked, this, &MainWindow::analysisDate);

    connect(ui->tableView, &QAbstractItemView::doubleClicked, this, [=](const QModelIndex &idx) -> void
            {
                int RowIdx = idx.row();
                int ColIdx = idx.column();
                //移动到指定行
                this->sql_table.first();
                for (int i = 0; i < RowIdx; i++)
                {
                    this->sql_table.next();
                }

                QString video = this->sql_table.value(2).toString();
                playvideo(video, ui->video_ns);

                QString video2 = this->sql_table.value(3).toString();
                playvideo(video2, ui->video_o, 2);

                QString j_ru = this->sql_table.value(8).toString();
                ui->label_ru->setText(j_ru);

                QString j_chu = this->sql_table.value(9).toString();
                ui->label_chu->setText(j_chu);

                QString j_high = this->sql_table.value(10).toString();
                ui->label_high->setText(j_high);

                QString p_y = this->sql_table.value(11).toString();
                showImage_y(p_y);
            });

    connect(ui->tableView2, &QAbstractItemView::doubleClicked, this, [=](const QModelIndex &idx) -> void
            {
                int RowIdx = idx.row();
                int ColIdx = idx.column();
                //移动到指定行
                this->sql_table2.first();
                for (int i = 0; i < RowIdx; i++)
                {
                    this->sql_table2.next();
                }

                QString video = this->sql_table2.value(2).toString();
                playvideo(video, ui->video_ns);

                QString video2 = this->sql_table2.value(3).toString();
                playvideo(video2, ui->video_o, 2);

                QString j_ru = this->sql_table2.value(8).toString();
                ui->label_ru->setText(j_ru);

                QString j_chu = this->sql_table2.value(9).toString();
                ui->label_chu->setText(j_chu);

                QString j_high = this->sql_table2.value(10).toString();
                ui->label_high->setText(j_high);

                QString p_y = this->sql_table2.value(11).toString();
                showImage_y(p_y);
            });

    connect(ui->tableView3, &QAbstractItemView::doubleClicked, this, [=](const QModelIndex &idx) -> void
            {
                int RowIdx = idx.row();
                int ColIdx = idx.column();
                //移动到指定行
                this->sql_table3.first();
                for (int i = 0; i < RowIdx; i++)
                {
                    this->sql_table3.next();
                }

                QString video = this->sql_table3.value(2).toString();
                playvideo(video, ui->video_ns);

                QString video2 = this->sql_table3.value(3).toString();
                playvideo(video2, ui->video_o, 2);

                QString j_ru = this->sql_table3.value(8).toString();
                ui->label_ru->setText(j_ru);

                QString j_chu = this->sql_table3.value(9).toString();
                ui->label_chu->setText(j_chu);

                QString j_high = this->sql_table3.value(10).toString();
                ui->label_high->setText(j_high);

                QString p_y = this->sql_table3.value(11).toString();
                showImage_y(p_y);
            });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::test()
{
}

bool MainWindow::createConnection()
{
    db.setHostName("127.0.0.1");
    db.setPort(3306);
    db.setDatabaseName("dc_db");
    db.setUserName("root");
    db.setPassword("15897933683");
//    db.setPassword("17312767927");
    if (!db.open())
    {
        QMessageBox::critical(0, QObject::tr("无法打开数据库"), "无法创建数据库连接！ ", QMessageBox::Cancel);
        return false;
    }
    else
    {
        statusBar()->showMessage(tr("数据库连接成功"), 5000);
        return true;
    }
};

void MainWindow::showCombo()
{
    ui->combo_p->addItems(nameList);
    ui->combo_p->setCurrentIndex(0);
}

void MainWindow::GetnameList()
{
    QString sql = "select distinct name from nonstd order by name asc";
    QSqlQuery sql_name = db.exec(sql);
    this->nameList.clear();
    while (sql_name.next())
    {
        this->nameList << sql_name.value(0).toString();
    }
}

void MainWindow::showtab()
{
    int index = ui->combo_p->currentIndex();
    ComboN = nameList[index];
    ui->label->setText(ComboN);

    QString filename = QString(":/prefix1/resource/p%1.png").arg(index + 1);
    QImage *img = new QImage;
    if (!(img->load(filename)))
    {
        QMessageBox::information(this, tr("加载图像失败"), tr("加载图像失败!"));
        delete img;
        return;
    }
    *img = img->scaled(100, 110, Qt::KeepAspectRatio);
    ui->label_p->setPixmap(QPixmap::fromImage(*img));

    QStringList trainResult;
    QString sql;
    trainResult << "记录视频"
                << "动作标准度"
                << "训练日期"
                << "桨最后"
                << "桨最前";
    QString name[5] = {"video", "std", "date", "p_back", "p_front"};

    sql = "select * from nonstd where (angle = 'right') and (name = '" + ComboN + "') order by video desc";
    this->sql_table = db.exec(sql);
    showTable(ui->tableView, this->sql_table, trainResult, name);
}

void MainWindow::showtab2()
{
    int index = ui->combo_p->currentIndex();
    ComboN = nameList[index];
    ui->label->setText(ComboN);

    QString filename = QString(":/prefix1/resource/p%1.png").arg(index + 1);
    QImage *img = new QImage;
    if (!(img->load(filename)))
    {
        QMessageBox::information(this, tr("加载图像失败"), tr("加载图像失败!"));
        delete img;
        return;
    }
    *img = img->scaled(100, 110, Qt::KeepAspectRatio);
    ui->label_p->setPixmap(QPixmap::fromImage(*img));

    QStringList trainResult;
    QString sql;
    trainResult << "记录视频"
                << "动作标准度"
                << "训练日期"
                << "桨最后"
                << "桨最前";
    QString name[5] = {"video", "std", "date", "p_back", "p_front"};

    sql = "select * from nonstd where (angle = 'left') and (name = '" + ComboN + "') order by video asc";
    this->sql_table2 = db.exec(sql);
    // setTable(ui->tableView2);
    showTable(ui->tableView2, this->sql_table2, trainResult, name);
}

void MainWindow::showtab3()
{
    int index = ui->combo_p->currentIndex();
    ComboN = nameList[index];
    ui->label->setText(ComboN);

    QString filename = QString(":/prefix1/resource/p%1.png").arg(index + 1);
    QImage *img = new QImage;
    if (!(img->load(filename)))
    {
        QMessageBox::information(this, tr("加载图像失败"), tr("加载图像失败!"));
        delete img;
        return;
    }
    *img = img->scaled(100, 110, Qt::KeepAspectRatio);
    ui->label_p->setPixmap(QPixmap::fromImage(*img));

    QStringList trainResult;
    QString sql;
    trainResult << "记录视频"
                << "动作标准度"
                << "训练日期"
                << "桨最后"
                << "桨最前";
    QString name[5] = {"video", "std", "date", "p_back", "p_front"};

    sql = "select * from nonstd where (angle = 'front') and (name = '" + ComboN + "') order by video asc";
    this->sql_table3 = db.exec(sql);
    // setTable(ui->tableView2);
    showTable(ui->tableView3, this->sql_table3, trainResult, name);
}

void MainWindow::playvideo(QString path, QVideoWidget *videoWidget, int pos)
{
    // play video
    if(pos==1){
        this->player = new QMediaPlayer;
    }else{
        this->player1 = new QMediaPlayer;
    }

    videoWidget->show();

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Could not open video file";
        return;
    }

    if(pos == 1){
        player->setSource(QUrl::fromLocalFile(path));
        player->setVideoOutput(videoWidget);
        player->play();
    }else{
        player1->setSource(QUrl::fromLocalFile(path));
        player1->setVideoOutput(videoWidget);
        player1->play();
    }
}

void MainWindow::showImages()
{
    QImage *img = new QImage;
    QString filename = ":/prefix1/resource/boat.png";

    if (!(img->load(filename)))
    {
        QMessageBox::information(this, tr("加载图像失败"), tr("加载图像失败!"));
        delete img;
        return;
    }
    *img = img->scaled(599, 153, Qt::KeepAspectRatio);
    ui->image_body->setPixmap(QPixmap::fromImage(*img));

    QString testText = "选择单次\n训练数据\n进行分析";
    ui->label_high->setText(testText);
    ui->label_chu->setText(testText);
    ui->label_ru->setText(testText);
}

void MainWindow::showImage_y(QString filename)
{
    QImage *img = new QImage;
    if (!(img->load(filename)))
    {
        QMessageBox::information(this, tr("加载图像失败"), tr("加载图像失败!"));
        delete img;
        return;
    }
    *img = img->scaled(280, 210, Qt::KeepAspectRatio);
    ui->label_y->setPixmap(QPixmap::fromImage(*img));
}

void MainWindow::analysisDate()
{
    QDate date_s = ui->date_start->date();
    QDate date_e = ui->date_end->date();

    QString ds = date_s.toString("yyyy-MM-dd");
    QString de = date_e.toString("yyyy-MM-dd");

    int index = ui->combo_p->currentIndex();
    QMap<QString,QString> map;
    map["右侧"] = "right";
    map["左侧"] = "left";
    map["前侧"] = "front";

    QString sql = "select std from nonstd where date > DATE_ADD('" +ds +"', INTERVAL -1 DAY) "
            + "and date<DATE_ADD('" + de +"', INTERVAL +1 DAY) "
            + "and name ='" + this->nameList[index] +"' "
            + "and angle ='" + map[ui->comboAngle->currentText()] + "'";
    QSqlQuery result = db.exec(sql);
    QVector<double> std;
    while(result.next()){
        std.append(result.value("std").toDouble());
    }

}

void MainWindow::createActions()
{
    newAthlete = new QAction(tr("&新建"), this);
    newAthlete->setShortcuts(QKeySequence::New);

    newAthlete->setStatusTip(tr("Create a new file"));
    connect(newAthlete, SIGNAL(triggered()), this, SLOT(test()));

    open = new QAction(tr("&打开"), this);
    open->setShortcuts(QKeySequence::Open);
    open->setStatusTip(tr("open a new file"));
    connect(open, SIGNAL(triggered()), this, SLOT(test()));

    Save = new QAction(tr("&保存"), this);
    Save->setShortcuts(QKeySequence::Save);
    Save->setStatusTip(tr("Save a new file"));
    connect(Save, SIGNAL(triggered()), this, SLOT(test()));

    saveAs = new QAction(tr("&另存为"), this);
    saveAs->setShortcuts(QKeySequence::SaveAs);
    saveAs->setStatusTip(tr("Save As a new file"));
    connect(saveAs, SIGNAL(triggered()), this, SLOT(test()));

    exit = new QAction(tr("&退出"), this);
    exit->setShortcuts(QKeySequence::Close);
    exit->setStatusTip(tr("exit system"));
    connect(exit, SIGNAL(triggered()), this, SLOT(close()));

    about = new QAction(tr("&关于"), this);
    about->setStatusTip(tr("About this project"));
    connect(about, SIGNAL(triggered()), this, SLOT(test()));

    aboutQt = new QAction(tr("&aboutQt"), this);
    aboutQt->setStatusTip(tr("About QT"));
    connect(aboutQt, SIGNAL(triggered()), this, SLOT(close()));
}

void MainWindow::createMenu()
{
    //创建一个name为file的菜单栏
    fileMenu = menuBar()->addMenu(tr("&管理"));
    //在这个菜单栏添加action即下拉菜单
    fileMenu->addAction(newAthlete);
    fileMenu->addAction(open);
    fileMenu->addAction(Save);
    fileMenu->addAction(saveAs);
    //添加一个分割器
    fileMenu->addSeparator();
    //推出下拉菜单
    fileMenu->addAction(exit);

    //创建一个name为help的菜单栏
    helpMenu = menuBar()->addMenu(tr("&帮助"));
    //在这个菜单栏添加action即下拉菜单
    helpMenu->addAction(about);
    helpMenu->addAction(aboutQt);
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
    while (result.next())
    {
        table->insertRow(i);
        for (int j = 0; j < tableHead.size(); j++)
        {
            table->setItem(i, j, new QTableWidgetItem(result.value(tableName[j]).toString()));
            table->item(i, j)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        }
        i++;
    }

    table->resizeColumnsToContents();
    table->resizeRowsToContents();
}

QString MainWindow::getSql()
{
    // index from 0 to 5
    int index = ui->combo_p->currentIndex();
    QString ComboN = QString::number(index);

    QString sql = "";
    sql = "where no = " + ComboN + " and subject = '";

    //    no->clear();
    //    subject->clear();
    return sql;
}


void MainWindow::on_pauseButton_clicked()
{
    if(this->player==nullptr || this->player1==nullptr){
        return;
    }
    this->player->pause();
    this->player1->pause();
}

void MainWindow::on_playButton_clicked()
{
    if(this->player==nullptr || this->player1==nullptr){
        return;
    }
    this->player->play();
    this->player1->play();
}

