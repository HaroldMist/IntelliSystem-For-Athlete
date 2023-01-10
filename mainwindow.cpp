#include "mainwindow.h"
#include "qcustomplot.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    createConnection();

    GetnameList();

    createActions();
    createMenu();
    showCombo();
    showImages();

    // 分析按钮
    connect(ui->analysis, &QPushButton::clicked, this, &MainWindow::drawGraphic);
    connect(ui->pushButtonDocker, &QPushButton::clicked, this, &MainWindow::startDocker);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::test()
{
}

/// @brief 连接数据库
/// @return bool 是否成功
bool MainWindow::createConnection()
{
    db.setHostName("127.0.0.1");
    db.setPort(3306);
    db.setDatabaseName("dc_db");
    db.setUserName("root");
    // db.setPassword("15897933683");
    db.setPassword("17312767927");
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

/// @brief 显示选项
void MainWindow::showCombo()
{
    ui->combo_p1->addItems(this->nameList);
    ui->combo_p1->setCurrentIndex(0);
    ui->combo_p2->addItems(this->nameList);
    ui->combo_p2->setCurrentIndex(0);
    // 显示管理界面的复选框
    QStringList angles;
    angles << "右侧"
           << "左侧"
           << "前侧";
    ui->comboAngle2->addItems(angles);
}

/// @brief 获取数据库姓名到nameList
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

// 显示姓名和头像
void MainWindow::showNameHead()
{
    int index(0);
    if (ui->tabWidget_2->currentIndex() == 0)
    {
        index = ui->combo_p1->currentIndex();
    }
    else
    {
        index = ui->combo_p2->currentIndex();
    }

    this->ComboN = this->nameList[index];

    QString filename = QString(":/prefix1/resource/p%1.png").arg(index + 1);
    QImage *img = new QImage;
    if (!(img->load(filename)))
    {
        QMessageBox::information(this, tr("加载图像失败"), tr("加载图像失败!"));
        delete img;
        return;
    }
    *img = img->scaled(100, 110, Qt::KeepAspectRatio);
    // 显示
    if (ui->tabWidget_2->currentIndex() == 0)
    {
        ui->label->setText(this->ComboN);
        ui->label_p->setPixmap(QPixmap::fromImage(*img));
    }
    else
    {
        ui->label_16->setText(this->ComboN);
        ui->label_p_2->setPixmap(QPixmap::fromImage(*img));
    }
}

// 双击表格时播放视频，显示内容
void MainWindow::PlayandShow(QSqlQuery sql_result, int row)
{
    // 移动到指定行
    sql_result.first();
    for (int i = 0; i < row; i++)
    {
        sql_result.next();
    }

    QString video = sql_result.value(2).toString();
    playvideo(video, ui->video_ns);

    QString video2 = sql_result.value(3).toString();
    playvideo(video2, ui->video_o, 2);

    QString j_ru = sql_result.value(8).toString();
    ui->label_ru->setText(j_ru);

    QString j_chu = sql_result.value(9).toString();
    ui->label_chu->setText(j_chu);

    QString j_high = sql_result.value(10).toString();
    ui->label_high->setText(j_high);

    QString p_y = sql_result.value(11).toString();
    showImage_y(p_y);
}

/***********************************************************************************************
 * 显示三个方向的数据
 ************************************************************************************************/

void MainWindow::showtab(QTableWidget *table, QSqlQuery &sql_result, QString angle = "right")
{
    QStringList trainResult;
    QString sql;
    trainResult << "记录视频"
                << "动作标准度"
                << "训练日期"
                << "桨最后"
                << "桨最前";
    QString name[5] = {"video", "std", "date", "p_back", "p_front"};

    sql = "select * from nonstd where (angle = '" + angle +
          "') and (name = '" + this->ComboN +
          "') order by video desc";
    sql_result = db.exec(sql);
    showTable(table, sql_result, trainResult, name);
}

/************************************************************************************************
 * 单次分析内容显示
 ************************************************************************************************/

void MainWindow::playvideo(QString path, QVideoWidget *videoWidget, int pos)
{
    // play video
    if (pos == 1)
    {
        this->player = new QMediaPlayer;
    }
    else
    {
        this->player1 = new QMediaPlayer;
    }

    videoWidget->show();

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Could not open video file";
        return;
    }

    if (pos == 1)
    {
        player->setSource(QUrl::fromLocalFile(path));
        player->setVideoOutput(videoWidget);
        player->play();
    }
    else
    {
        player1->setSource(QUrl::fromLocalFile(path));
        player1->setVideoOutput(videoWidget);
        player1->play();
    }
}

/// @brief 显示分析建议文本和龙舟图片
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

/// @brief 显示手部轨迹图片
/// @param filename 对应数据的轨迹图片文件
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

/************************************************************************************************
 * 长期分析内容显示
 ************************************************************************************************/

// TODO 把参数改成返回值
/// @brief 从数据库读标准度
/// @param std
void MainWindow::analysisDate(QVector<double> &std)
{
    QDate date_s = ui->date_start->date();
    QDate date_e = ui->date_end->date();

    QString ds = date_s.toString("yyyy-MM-dd");
    QString de = date_e.toString("yyyy-MM-dd");

    int index = ui->combo_p1->currentIndex();
    QMap<QString, QString> map;
    map["右侧"] = "right";
    map["左侧"] = "left";
    map["前侧"] = "front";

    QString sql = "select std from nonstd where date >='" + ds + "'" + "and date<='" + de + "'" + "and name ='" + this->nameList[index] + "' " + "and angle ='" + map[ui->comboAngle->currentText()] + "'" + "order by date asc";
    QSqlQuery result = db.exec(sql);

    std.clear();
    while (result.next())
    {
        std.append(result.value("std").toDouble());
    }
}

// TODO 改成按分析数据画回归曲线
/// @brief 按标准度画曲线
void MainWindow::drawGraphic()
{
    QVector<double> std;
    analysisDate(std);
    QVector<double> t;
    for (int i = 0; i < std.size(); i++)
    {
        t.append(i);
        qDebug() << std[i];
    }

    ui->qcp_analysis->addGraph();
    ui->qcp_analysis->graph(0)->setPen(QPen(Qt::blue));
    ui->qcp_analysis->graph(0)->setData(t, std);

    ui->qcp_analysis->xAxis->setLabel("时间");
    ui->qcp_analysis->yAxis->setLabel("标准度");
    ui->qcp_analysis->xAxis->setRange(0, std.size() + 2);
    ui->qcp_analysis->yAxis->setRange(0, 1.2);
    ui->qcp_analysis->xAxis->setTickLabels(false);
    ui->qcp_analysis->xAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
    ui->qcp_analysis->yAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);

    ui->qcp_analysis->replot();
}

/************************************************************************************************
 * 显示主界面内容
 ************************************************************************************************/

/// @brief 创建菜单栏选项功能
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

/// @brief 创建菜单栏选项
void MainWindow::createMenu()
{
    // 创建一个name为file的菜单栏
    fileMenu = menuBar()->addMenu(tr("&文件"));
    // 在这个菜单栏添加action即下拉菜单
    fileMenu->addAction(newAthlete);
    fileMenu->addAction(open);
    fileMenu->addAction(Save);
    fileMenu->addAction(saveAs);
    // 添加一个分割器
    fileMenu->addSeparator();
    // 推出下拉菜单
    fileMenu->addAction(exit);

    // 创建一个name为help的菜单栏
    helpMenu = menuBar()->addMenu(tr("&帮助"));
    // 在这个菜单栏添加action即下拉菜单
    helpMenu->addAction(about);
    helpMenu->addAction(aboutQt);
}

/// @brief 设置表格样式
/// @param table
void MainWindow::setTable(QTableWidget *table)
{
    table->verticalHeader()->setVisible(false);
    QFont font = table->horizontalHeader()->font();
    font.setBold(true);
    table->horizontalHeader()->setFont(font);
    table->setFrameShape(QFrame::NoFrame);
}

/// @brief 显示表格
/// @param table 表格
/// @param result 数据库查询结果
/// @param tableHead 表格显示列名
/// @param tableName 数据库列名
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

/// @brief 暂停播放
void MainWindow::on_pauseButton_clicked()
{
    if (this->player == nullptr || this->player1 == nullptr)
    {
        return;
    }
    this->player->pause();
    this->player1->pause();
}

/// @brief 继续播放
void MainWindow::on_playButton_clicked()
{
    if (this->player == nullptr || this->player1 == nullptr)
    {
        return;
    }
    this->player->play();
    this->player1->play();
}

void MainWindow::on_query_clicked()
{
    MainWindow::showNameHead();
    MainWindow::showtab(ui->tableView, this->sql_table);
    MainWindow::showtab(ui->tableView2, this->sql_table, "left");
    MainWindow::showtab(ui->tableView3, this->sql_table, "front");
}

void MainWindow::on_query2_clicked()
{
    MainWindow::showNameHead();
    MainWindow::showtab(ui->tableView_2, this->sql_table);
    MainWindow::showtab(ui->tableView2_2, this->sql_table, "left");
    MainWindow::showtab(ui->tableView3_2, this->sql_table, "front");
}

void MainWindow::on_tableView_cellDoubleClicked(int row, int column)
{
    MainWindow::PlayandShow(this->sql_table, row);
}

void MainWindow::on_tableView2_cellDoubleClicked(int row, int column)
{
    MainWindow::PlayandShow(this->sql_table2, row);
}

void MainWindow::on_tableView3_cellDoubleClicked(int row, int column)
{
    MainWindow::PlayandShow(this->sql_table3, row);
}


void MainWindow::on_pushButton_3_clicked()
{
    QFileDialog *fileDialog = new QFileDialog(this);

    fileDialog->setWindowTitle(QStringLiteral("选择文件"));
    fileDialog->setDirectory("E://");

    // 设置文件过滤器,多个过滤文件使用空格隔开
    fileDialog->setNameFilter(tr("File(*.mp4)"));

    // 设置视图模式
    fileDialog->setViewMode(QFileDialog::Detail);
    // 获取选择的文件的路径
    if (fileDialog->exec())
    {
        filePath = fileDialog->selectedFiles()[0];
        fileinfo = QFileInfo(filePath);
        fileName = fileinfo.fileName();
        ui->label_23->setText(filePath);
    }
}

void MainWindow::startDocker()
{
    if (fileName.isEmpty())
    {
        QMessageBox::warning(this, "错误", "请选择文件");
    }
    else
    {
        ui->label_29->setText("分析中");
        ui->progressBar->setRange(0,10000);
        QProcess process(this);
        process.setProgram("powershell");

        // copy file to container
        QString cpPath = "docker cp " + filePath + "asd: /PyMAF_use/input\n";
        process.write(cpPath.toStdString().c_str());

        // rename file as test.mp4
        cpPath = "docker exec asd /bin/bash -c 'cp /PyMAF_use/input/" + fileName + " /PyMAF_use/input/test.mp4\n";
        process.write(cpPath.toStdString().c_str());
        for(int i = 0;i <= 500;i += 1){
            ui->progressBar->setValue(i);
            Sleep(1);
        }

        // QStringList argument;
        // argument<< "/c" << "docker exec asd /bin/bash -c 'cd PyMAF_use && ls'";
        // process.setArguments(argument);
        
        process.start();
        process.waitForStarted();
        for(int i = 500;i <= 1500;i += 1){
            ui->progressBar->setValue(i);
            Sleep(1);
        }

        ui->label_29->setText("分析准确度中");
        QString pyFile;
        switch (ui->combo_p1->currentIndex())
        {
        case 0:
            pyFile = "docker exec asd /bin/bash -c 'cd /PyMAF_use && python3 /PyMAF_use/extract_feature_3.py'\n";
            break;
        case 1:
            pyFile = "docker exec asd /bin/bash -c 'cd /PyMAF_use && python3 /PyMAF_use/extract_feature_2.py'\n";
            break;
        case 2:
            pyFile = "docker exec asd /bin/bash -c 'cd /PyMAF_use && python3 /PyMAF_use/extract_feature.py'\n";
            break;
        default:
            pyFile = "docker exec asd /bin/bash -c 'cd /PyMAF_use && python3 /PyMAF_use/extract_feature.py'\n";
            break;
        }
        process.write(pyFile.toStdString().c_str());
        for(int i = 1500;i <= 4000;i += 1){
            ui->progressBar->setValue(i);
            Sleep(2);
        }

        ui->label_29->setText("渲染视频中");
        process.write("docker exec asd /bin/bash -c 'cd /PyMAF_use && python3 /PyMAF_use/demo_orig.py --checkpoint=data/pretrained_model/PyMAF_model_checkpoint.pt --vid_file input/test.mp4 --use_opendr'\n");
        for(int i = 4000;i <= 9000;i += 1){
            ui->progressBar->setValue(i);
            Sleep(4);
        }

        cpPath = "docker cp asd: /PyMAF_use/output/test/test_result.mp4 E:/DBVideo/result/";
        process.write(cpPath.toStdString().c_str());

        QString video = "E:/DBVideo/result/test_result.mp4";
        playvideo(video, ui->video_4);
        
        playvideo(filePath, ui->video_3);


        // process.write("docker exec asd /bin/bash -c 'cd /PyMAF_use/output && ls'\n");
        
        process.waitForFinished();
        for(int i = 9000;i <= 10000;i += 1){
            ui->progressBar->setValue(i);
            Sleep(2);
        }
        QString temp=QString::fromLocal8Bit(process.readAllStandardOutput());
        QMessageBox textMessage;
        textMessage.setText(temp);
        textMessage.exec();

        ui->label_29->setText("分析完成");
        
    }
    
    
    
    

  
}


