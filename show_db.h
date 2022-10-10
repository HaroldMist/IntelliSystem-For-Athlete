#ifndef SHOW_DB_H
#define SHOW_DB_H

#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QTableView>

class Show_db{
public:
    Show_db();
    bool createConnection();
};

#endif // SHOW_DB_H
