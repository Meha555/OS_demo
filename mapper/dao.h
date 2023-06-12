#ifndef DAO_H
#define DAO_H

#include <QApplication>
#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QMessageBox>
//#include <QVariantList>
#include "config.h"
#include "result.h"
#include "message.h"
#include "buffer.h"

namespace dao{
    class Dao;
}

class Dao : public QObject
{
    Q_OBJECT
public:
    explicit Dao(QObject *parent = nullptr);
    explicit Dao(const QString& tabName, QObject *parent = nullptr);
    ~Dao();
//    QVariantList sqlQuery(const QString& sql);
//    int sqlExecute(const QString& sql);
//    int sqlBatchExecute(bool type, QQueue<Message>& bf);
    bool getConnection();
    void closeDB();
    void setTableName(const QString &value);
    void resetTableName();
signals:

public:
    QString tableName = "Null";//要查询的数据库表名，用于实现分发查询各表，使用时能使得当前db对象为tablename规定的类独占
    QSqlDatabase db;//数据库连接对象
    QSqlQuery sqlHandler;//数据库查询对象
//    QSqlRecord resultSet;
};

#endif // DAO_H
