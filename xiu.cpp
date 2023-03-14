#include "xiu.h"
#include "ui_xiu.h"
#include "mainwindow.h"

xiu::xiu(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::xiu)
{
    ui->setupUi(this);
    ui->spinBox->setRange(12, 60);
    ui->spinBox_2->setRange(0, 10);
}

xiu::~xiu()
{
    delete ui;
}


void xiu::fromA(QString copy)
{
    std::string s = copy.toStdString();
    nameold = QString::fromStdString(s);
}


// 显示姓名和头像
void xiu::showNameHead2()
{

    ui->lineEdit->setText(this->nameold);
    ui->label_5->clear();
    ui->spinBox_2->clear();
    ui->spinBox->clear();
    ui->lineEdit_2->clear();
    ui->pushButton_3->setVisible(false);
    //获取其他信息
    QString str=QString("select *from nonstd where name = '%1'").arg(this->nameold);//表名、列名
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
    ui->spinBox_2->setValue(group);
    ui->spinBox->setValue(age);
    ui->lineEdit_2->setText(gender);


    QString filename = QString("E:/CodeRepos/DC/xiangmu/resource/p%1.png").arg(this->indexOld=1);
    // QString filename = QString(":/prefix1/resource/p%1.png").arg(this->indexOld);
    QImage *img = new QImage;
    if (!(img->load(filename)))
    {
        QMessageBox::information(this, tr("加载图像失败"), tr("加载图像失败!"));
        delete img;
        return;
    }
    *img = img->scaled(100, 110, Qt::KeepAspectRatio);
    // 显示
    // ui->lineEdit->setText(this->nameold);
    ui->label_5->setPixmap(QPixmap::fromImage(*img));
}



//退出
void xiu::on_pushButton_2_clicked()
{
    this->close();
}

//修改
void xiu::on_pushButton_clicked()
{

    QSqlQuery dequery;
    QString command = QString("delete from nonstd where name = '%1' ") .arg( this->nameold);  //只获取当前表格内容
    dequery.exec(command);

    name= ui->lineEdit->text();
    int age= ui->spinBox->text().toInt();
    QString gender= ui->lineEdit_2->text();
    int group= ui->spinBox_2->text().toInt();
    QString str=0;


    if(gender=="男"||gender=="女")
    {
        str = QString("INSERT INTO nonstd(name,age, gender, `group`) VALUES ('%1', '%2', '%3', '%4')").arg(name).arg(age,0,10).arg(gender).arg(group,0,10);  //添加数据项
    }
    else
    {
        ui->label_7->setText(tr("请重新输入性别"));
    }
    if(  QString(name).isEmpty())
    {
        str=0;
        ui->label_7->setText(tr("请输入姓名"));
    }

    else
    {
        QSqlQuery query;

        if( query.exec(str))
        {

            ui->label_7->setText(tr("添加成功!"));
            emit mysignal2();
            QString address=QString(":/prefix1/resource/p%1.png").arg(name);
            img->save(address, "PNG");
        }
    }
}


//修改头像
void xiu::on_pushButton_3_clicked()
{

    QString address=QString(":/prefix1/resource/p%1.png").arg(nameold);
    QFile file(address);
    bool ok = file.remove();

    QString fileName;
    fileName = QFileDialog::getOpenFileName(this,tr("Select Image"),"",tr("Images(*.png );;AllFiles(*.*)"));
    if(fileName.isEmpty())
    {
        ui->label_7->setText(tr("重新选择头像!"));
    }
    else
    {
      //  QImage *img = new QImage;
        if(img->load(fileName)) //加载图像
        {
            *img = img->scaled(100, 110, Qt::KeepAspectRatio);
            ui->label_5->setPixmap(QPixmap::fromImage(*img));
            suc=1;
        }
    }

}

