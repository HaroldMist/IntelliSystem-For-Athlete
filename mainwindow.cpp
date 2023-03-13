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

    view = new form(this) ;//将类指针实例化
    connect(view,SIGNAL(mysignal()),this,SLOT(reshow()));
    view2=new xiu;
    connect(view2,SIGNAL(mysignal2()),this,SLOT(reshow()));
    view3=new xiuhang;
    connect(view3,SIGNAL(mysignal3()),this,SLOT(reshow()));

    connect(ui->tableView3_2,SIGNAL(cellClicked(int , int )), this, SLOT(getData(int, int)));
    connect(ui->tableView_2,SIGNAL(cellClicked(int , int )), this, SLOT(getData1(int, int)));
    connect(ui->tableView2_2,SIGNAL(cellClicked(int , int )), this, SLOT(getData2(int, int)));

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
//    db.setDatabaseName("test_qt");
    db.setUserName("root");
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

//显示年龄组别性别xiugai
void MainWindow::showagender()
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

    if (ui->tabWidget_2->currentIndex() != 0)
    {
        QString str=QString("select *from nonstd where name = '%1'").arg( this->ComboN);//表名、列名
        QSqlQuery query;
        query.exec(str);
        int group;
        int age;
        QString gender;
        while(query.next())
        {
            group =query.value("group").toInt();
            age =query.value("age").toInt();
            gender=query.value("gender").toString();
        }
        ui->label_20->setText(QString().setNum(group));
        ui->label_33->setText(QString().setNum(age));
        ui->label_32->setText(gender);

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
    //QString filename = QString("D:/QT/xiangmu/photos/%1.png").arg(this->ComboN);
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
    if (filename.length()==0){
        return;
    }
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
    MainWindow::showtab(ui->tableView2, this->sql_table2, "left");
    MainWindow::showtab(ui->tableView3, this->sql_table3, "front");
}

void MainWindow::on_query2_clicked()
{
    MainWindow::showNameHead();
    MainWindow::showagender();
    MainWindow::showtab(ui->tableView_2, this->sql_table);
    MainWindow::showtab(ui->tableView2_2, this->sql_table2, "left");
    MainWindow::showtab(ui->tableView3_2, this->sql_table3, "front");
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
    fileDialog->setDirectory("E://DBVideo");

    // 设置文件过滤器,多个过滤文件使用空格隔开
    fileDialog->setNameFilter(tr("File(*.mp4)"));

    // 设置视图模式
    fileDialog->setViewMode(QFileDialog::Detail);
    // 获取选择的文件的路径
    if (fileDialog->exec())
    {
        filePath = fileDialog->selectedFiles()[0];
        fileinfo = QFileInfo(filePath);
        this->fileName = fileinfo.fileName();
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
        process.start();
        process.waitForStarted();

        QString cpPath = "docker cp " + filePath + " asd:/PyMAF_use/input\n";
        process.write(cpPath.toStdString().c_str());
        for(int i = 0;i <= 1000;i += 1){
            ui->progressBar->setValue(i);
            Sleep(1);
        }

        QString cpPath2 = "docker exec asd /bin/bash -c 'mv /PyMAF_use/input/" + fileName + " /PyMAF_use/input/test.mp4'\n";
        process.write(cpPath2.toStdString().c_str());
        for(int i = 1000;i <= 2000;i += 1){
            ui->progressBar->setValue(i);
            Sleep(1);
        }        
        process.waitForFinished(5000);
//        process.close();

        ui->label_29->setText("分析准确度中");
        
        QString pyFile;
        switch (ui->comboAngle2->currentIndex())
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
        QProcess process2(this);
        process2.setProgram("powershell");
        process2.start();
        process2.waitForStarted();
        process2.write(pyFile.toStdString().c_str());
        for(int i = 2000;i <= 4000;i += 1){
            ui->progressBar->setValue(i);
            Sleep(3);
        }

        process2.waitForFinished(40000);
        // 从结果中提取标准度
        QString temp=QString::fromUtf8(process2.readAllStandardOutput());
        QRegularExpression re("标准度为([\\d\\.]+)");
        QRegularExpressionMatch match = re.match(temp);
        QString value="1.0";
        if (match.hasMatch()) {
            value = match.captured(1);
        }
        ui->labelResultStd->setText(value);
        // 获取手部轨迹图片
        process2.write("docker cp asd:/PyMAF_use/output/wrist.jpg E:/DBVideo/result/\n");
        process2.waitForFinished(4000);
        process2.close();

        ui->label_29->setText("渲染视频中");

        QProcess process3(this);
        process3.setProgram("powershell");
        process3.start();
        process3.waitForStarted();
        process3.write("docker exec asd /bin/bash -c 'cd /PyMAF_use && python3 /PyMAF_use/demo_orig.py --checkpoint=data/pretrained_model/PyMAF_model_checkpoint.pt --vid_file input/test.mp4 --use_opendr'\n");
        for(int i = 4000;i <= 9500;i += 1){
            ui->progressBar->setValue(i);
            Sleep(4);
        }
        process3.waitForFinished(100000);
        process3.close();

        QProcess process4(this);
        process4.setProgram("powershell");
        process4.start();
        process4.waitForStarted();
        process4.write("docker cp asd:/PyMAF_use/output/test/test_result.mp4 E:/DBVideo/result/\n");
        process4.waitForFinished(100000);
        for(int i = 9500;i <= 10000;i += 1){
            ui->progressBar->setValue(i);
            Sleep(2);
        }
        
        ui->label_29->setText("分析完成");

        // 显示图片
        QImage *img = new QImage;
        img->load("E:/DBVideo/result/wrist.jpg");
        *img = img->scaled(250, 210, Qt::KeepAspectRatio);
        ui->labelResultY->setPixmap(QPixmap::fromImage(*img));

        QString video = "E:/DBVideo/result/test_result.mp4";
        playvideo(video, ui->video_4);
        playvideo(filePath, ui->video_3);

        process4.close();
    }
}

void MainWindow::reshow()
{
    GetnameList();
    ui->combo_p1->clear();
    ui->combo_p2->clear();
    showCombo();
    MainWindow::showtab(ui->tableView_2, this->sql_table);
    MainWindow::showtab(ui->tableView2_2, this->sql_table2, "left");
    MainWindow::showtab(ui->tableView3_2, this->sql_table3, "front");

}
//新增运动员
void MainWindow::on_pushButtonAddAth_clicked()
{
    view->show();
}



//删除运动员

void MainWindow::on_pushButtonDelAth_clicked()
{
    QMessageBox message(QMessageBox::Warning,"提示","确定要删除该项吗？",QMessageBox::Yes|QMessageBox::No,NULL);
        if (message.exec()==QMessageBox::Yes)
        {
        QSqlQuery dequery;
        QString command = QString("delete from nonstd where name = '%1' ") .arg( this->ComboN);  //只获取当前表格内容
        dequery.exec(command);
        GetnameList();
        ui->combo_p1->clear();
        ui->combo_p2->clear();
        showCombo();
        ui->label_16->setText("未选择");
        ui->label_20->clear();
        ui->label_32->clear();
        ui->label_33->clear();
        ui->label_p_2->clear();
        QString address=QString(":/prefix1/resource/%1.png").arg(this->ComboN);
        QFile file(address);
        bool ok = file.remove();
        }
}


//修改个人信息
void MainWindow::on_pushButton_clicked()
{
   // view2 = new xiu(this);
    view2->fromA(ComboN);
    view2->show();
    view2->showNameHead2();

}

//删除该行数据
void MainWindow::on_pushButton_2_clicked()
{
    //进行查询组名
    QString str=QString("select *from nonstd where name = '%1'").arg( this->ComboN);//表名、列名
    QSqlQuery query;
    query.exec(str);
    int group;
    int age;
    QString gender;
    while(query.next())
    {
        group =query.value("group").toInt();
        age =query.value("age").toInt();
        gender=query.value("gender").toString();
    }

    //进行
    QString tishi=QString("确定要删除第%1项吗？").arg(col);
    QMessageBox message(QMessageBox::Warning,"提示",tishi,QMessageBox::Yes|QMessageBox::No,NULL);
        if (message.exec()==QMessageBox::Yes)
        {

            QSqlQuery query;
            QString command = QString("DELETE FROM nonstd WHERE `name` = '%1' AND age=%2 AND `group`=%3 AND gender='%4' AND angle='%5' AND video='%6' AND `std`='%7' AND `date`='%8' AND p_back='%9' AND p_front='%10' LIMIT 1;  ")
                                        .arg( this->ComboN).arg(age).arg(group).arg(gender).arg(angle1).arg(vedio1).arg(biaozhun).arg(data).arg(last).arg(first);  //只获取当前表格内容
            query.exec(command);
        }

        MainWindow::reshow();


}
//从三个表获取数据
void MainWindow::getData(int row, int column)
{
   angle1="front";
   col = ui->tableView3_2->rowCount();
   vedio1=ui->tableView3_2->item(row,0)->text();
   biaozhun=ui->tableView3_2->item(row,1)->text();
   data=ui->tableView3_2->item(row,2)->text();
   last=ui->tableView3_2->item(row,3)->text();
   first=ui->tableView3_2->item(row,4)->text();
}
void MainWindow::getData2(int row, int column)
{
   angle1="left";
   col = ui->tableView2_2->rowCount() ;
   vedio1=ui->tableView2_2->item(row,0)->text();
   biaozhun=ui->tableView2_2->item(row,1)->text();
   data=ui->tableView2_2->item(row,2)->text();
   last=ui->tableView2_2->item(row,3)->text();
   first=ui->tableView2_2->item(row,4)->text();
}
void MainWindow::getData1(int row, int column)
{
   angle1="right";
   col = ui->tableView_2->rowCount();
   vedio1=ui->tableView_2->item(row,0)->text();
   biaozhun=ui->tableView_2->item(row,1)->text();
   data=ui->tableView_2->item(row,2)->text();
   last=ui->tableView_2->item(row,3)->text();
   first=ui->tableView_2->item(row,4)->text();
}




//修改该行数据
void MainWindow::on_pushButton_4_clicked()
{
    view3->fromB(ComboN,vedio1,biaozhun,data,last,first,angle1);
    view3->show();
    view3->showold();
}


void MainWindow::on_pushButtonImport_clicked()
{
    if (ui->label_29->text()!="分析完成"){
        QMessageBox::critical(this,"Error","请先分析再导入！");
        return;
    }
    QMessageBox messageBox;
    messageBox.setWindowTitle("提示");
    messageBox.setText("您确定要添加这些数据吗？");
    messageBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    messageBox.setDefaultButton(QMessageBox::Cancel);
    messageBox.setIcon(QMessageBox::Question);

    if (messageBox.exec()==QMessageBox::Ok){
        ui->label_29->setText("导入成功");
    }
}

