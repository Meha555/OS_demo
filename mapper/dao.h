#ifndef DAO_H
#define DAO_H

#include <QApplication>
#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QMessageBox>
#include <QVariantList>
#include "config.h"
#include "result.h"
#include "message.h"

class Dao : public QObject
{
    Q_OBJECT
public:
    explicit Dao(QObject *parent = nullptr);
    ~Dao();
    QVariantList sqlQuery(const QString& sql);
    int sqlExecute(const QString& sql);
    bool getConnection();
    void closeDB();
    void setTableName(const QString &value);

signals:

public:
    QString tableName;//要查询的数据库表名，用于实现分发查询各表，使用时能使得当前db对象为tablename规定的类独占
    QSqlDatabase db;//数据库连接对象
    QSqlQuery sqlHandler;//数据库查询对象
};

#endif // DAO_H
