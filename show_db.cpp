#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QTableView>

static bool createConnection(){
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setHostName("127.0.0.1");
    db.setPort(3306);
    db.setDatabaseName("dc_db");
    db.setUserName("root");
    db.setPassword("17312767927");
    // bool ok = db.open();
    if (!db.open()) {
        QMessageBox::critical(0, QObject::tr("无法打开数据库"),"无法创建数据库连接！ ", QMessageBox::Cancel);
        return false;
        }
    else{
        return true;
    }
};

