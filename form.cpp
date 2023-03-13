#include "form.h"
#include "ui_form.h"
#include "mainwindow.h"

form::form(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::form)
{
    ui->setupUi(this);
    ui->spinBox->setRange(10, 40);
    ui->spinBox_2->setRange(1, 10);

}

form::~form()
{
    delete ui;
}

void form::on_pushButton_clicked()
{
    name= ui->lineEdit->text();
    int age= ui->spinBox->text().toInt();
    QString gender= ui->lineEdit_4->text();
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
    if(suc!=1)
    {
        ui->label_7->setText(tr("请选择头像"));
    }
    else
    {
        QSqlQuery query;

        if( query.exec(str))
        {

            ui->label_7->setText(tr("添加成功!"));
            emit mysignal();
            QString address=QString("D:/QT/xiangmu/photos/%1.png").arg(name);
            img->save(address, "PNG");
            //this->close();
        }
    }
}

//退出
void form::on_pushButton_3_clicked()
{
    this->close();
}

//选择头像
void form::on_pushButton_2_clicked()
{
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
