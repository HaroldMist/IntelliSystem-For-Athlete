#include "xiuhang.h"
#include "ui_xiuhang.h"
#include "mainwindow.h"
xiuhang::xiuhang(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::xiuhang)
{
    ui->setupUi(this);
}

xiuhang::~xiuhang()
{
    delete ui;
}
//获取信息
void xiuhang::fromB(QString copy,QString vedio2,QString biaozhun2,QString data2,QString last2,QString first2,QString angle2)
{
    std::string s = copy.toStdString();
    nameold = QString::fromStdString(s);

    std::string a = vedio2.toStdString();
    vedio3 = QString::fromStdString(a);

    std::string b = biaozhun2.toStdString();
    biaozhun3 = QString::fromStdString(b);

    std::string c = data2.toStdString();
    data3 = QString::fromStdString(c);

    std::string d = last2.toStdString();
    last3 = QString::fromStdString(d);

    std::string e = first2.toStdString();
    first3 = QString::fromStdString(e);

    std::string f = angle2.toStdString();
    angle3 = QString::fromStdString(f);

}

//显示当前选择的行信息
void xiuhang::showold()
{

    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    ui->lineEdit_3->clear();
    ui->lineEdit_4->clear();
    ui->lineEdit_5->clear();
    //获取其他信息
    QString str=QString("select *from nonstd where name = '%1'").arg( this->nameold);//表名、列名
    QSqlQuery query;
    query.exec(str);
    while(query.next())
    {
        group =query.value("group").toInt();
        age =query.value("age").toInt();
        gender=query.value("gender").toString();
    }
    ui->lineEdit->setText(vedio3);
    ui->lineEdit_2->setText(biaozhun3);
    ui->lineEdit_3->setText(data3);
    ui->lineEdit_4->setText(last3);
    ui->lineEdit_5->setText(first3);
}

//退出
void xiuhang::on_pushButton_2_clicked()
{
    this->close();
}

//确认修改
void xiuhang::on_pushButton_clicked()
{
    QSqlQuery dequery;
    QString command = QString("DELETE FROM nonstd WHERE `name` = '%1' AND age=%2 AND `group`=%3 AND gender='%4' AND angle='%5' AND video='%6' AND `std`='%7' AND `date`='%8' AND p_back='%9' AND p_front='%10' LIMIT 1;  ")
            .arg( nameold).arg(age).arg(group).arg(gender).arg(angle3).arg(vedio3).arg(biaozhun3).arg(data3).arg(last3).arg(first3);  ;  //只获取当前表格内容
    dequery.exec(command);

    vedio3=ui->lineEdit->text();
    biaozhun3=ui->lineEdit_2->text();
    data3=ui->lineEdit_3->text();
    last3=ui->lineEdit_4->text();
    first3=ui->lineEdit_5->text();


    if(QString(vedio3).isEmpty()||QString(biaozhun3).isEmpty()||QString(data3).isEmpty()||QString(last3).isEmpty()||QString(first3).isEmpty())
    {
        ui->label_7->setText(tr("请重新输入"));
    }
    else
    {

        QString str = QString("INSERT INTO nonstd(`name`,age,`group`,gender,angle,video,`std`, `date`,p_back,p_front) VALUES ('%1', '%2', '%3', '%4','%5','%6','%7','%8','%9','%10')")
                            .arg(nameold).arg(age,0,10).arg(group,0,10).arg(gender).arg(angle3).arg(vedio3).arg(biaozhun3).arg(data3).arg(last3).arg(first3);  //添加数据项
        QSqlQuery query;
        query.exec(str);
        ui->label_7->setText(tr("添加成功!"));
        emit mysignal3();
    }

}

