#ifndef XIUHANG_H
#define XIUHANG_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QImage>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
namespace Ui {
class xiuhang;
}

class xiuhang : public QMainWindow
{
    Q_OBJECT

public:
    explicit xiuhang(QWidget *parent = nullptr);
    void fromB(QString copy,QString vedio2,QString biaozhun2,QString data2,QString last2,QString first2,QString angle2);
    void showold();
    ~xiuhang();
signals:
    void mysignal3();
private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::xiuhang *ui;
    QString nameold;
    QString vedio3;
    QString biaozhun3;
    QString data3;
    QString last3;
    QString first3;
    QString angle3;
    int group;
    int age;
    QString gender;
};

#endif // XIUHANG_H
