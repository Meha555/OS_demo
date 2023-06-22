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

//namespace dao{
//    class Dao;
//}

typedef enum{CONNECTION_ERR = -1,TRANSACTION_ERR = -2,EXECUTEION_ERR = -3,COMMITMENT_ERR = -4} Errno;

class Dao : public QObject
{
    Q_OBJECT
public:
    explicit Dao(const QString& dbName, QObject *parent = nullptr);
    explicit Dao(const QString& dbName, const QString& tabName, QObject *parent = nullptr);
    ~Dao();
//    QVariantList sqlQuery(const QString& sql);
    int sqlExecute(const QString& sql);
//    int sqlBatchExecute(bool type, QQueue<Message>& bf);
    bool getConnection();
    void closeConnection();
    void setTableName(const QString &value);
    void resetTableName();
    void closeDB();
signals:

public:
    QString dbName;
    QString tableName = "Null";//要查询的数据库表名，用于实现分发查询各表，使用时能使得当前db对象为tablename规定的类独占
    QSqlDatabase db;//数据库连接对象
    QSqlQuery sqlHandler;//数据库查询对象
//    QSqlRecord resultSet;
};

#endif // DAO_H
