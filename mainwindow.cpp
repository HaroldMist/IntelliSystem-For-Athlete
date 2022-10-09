#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "show_db.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    createConnection();

    QSqlTableModel *model = new QSqlTableModel;
        model->setTable("nonstd");
        model->setEditStrategy(QSqlTableModel::OnManualSubmit);
        model->select();
        model->setHeaderData(0, Qt::Horizontal, tr("分析角度"));
        model->setHeaderData(1, Qt::Horizontal, tr("记录视频"));


    QTableView *view = ui->tableView;
        view->hideColumn(0);
        view->setColumnHidden(3,true);
        view->setColumnHidden(4,true);
        view->setColumnHidden(5,true);
        view->setColumnHidden(6,true);
        view->setColumnHidden(7,true);
        view->setColumnHidden(8,true);
        view->setColumnHidden(9,true);
        view->setColumnHidden(10,true);
        view->setColumnHidden(11,true);
        view->setColumnHidden(12,true);
        view->setModel(model);
        view->show();

}

MainWindow::~MainWindow()
{
    delete ui;
}



