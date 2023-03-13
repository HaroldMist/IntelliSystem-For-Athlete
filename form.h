#ifndef FORM_H
#define FORM_H

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
#include <QTimer>

namespace Ui {
class form;
}

class form : public QMainWindow
{
    Q_OBJECT

public:
    explicit form(QWidget *parent = nullptr);
    ~form();


private slots:
    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();
signals:
    void mysignal();
private:
    Ui::form *ui;
    QString name;
    int suc;
    QImage *img = new QImage;
};

#endif // FORM_H
