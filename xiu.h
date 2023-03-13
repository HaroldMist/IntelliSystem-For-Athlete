#ifndef XIU_H
#define XIU_H

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
class xiu;
}

class xiu : public QMainWindow
{
    Q_OBJECT

public:
    explicit xiu(QWidget *parent = nullptr);
    ~xiu();
    //void GetnameList2();
    void showNameHead2();
    void fromA(QString copy);

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();
    void on_pushButton_3_clicked();

signals:
    void mysignal2();
private:
    Ui::xiu *ui;

    QString name;
    QString nameold;
    int suc;
    QImage *img = new QImage;
};

#endif // XIU_H
