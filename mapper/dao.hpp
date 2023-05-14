#ifndef DAO_H
#define DAO_H

#include <QApplication>
#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QMessageBox>
#include <QVector>
#include <QVariant>
#include "config.h"
#include "result.h"
#include "message.h"

template<class T>
class Dao : public QObject
{
//    Q_OBJECT
public:
    explicit Dao(QObject *parent = nullptr);
    template<class... Args>

    int sqlExecute(const QString& sql);
    bool getConnection();
    void closeDB();
    void setTableName(const QString &value);

signals:

private:
    QString tableName;//要查询的数据库表名，用于实现分发查询各表，使用时能使得当前db对象为tablename规定的类独占
    QSqlDatabase db;//数据库连接对象
    QSqlQuery sqlHandler;//数据库查询对象
};

template<class T>
Dao<T>::Dao(QObject* parent): QObject(parent) {
    db = QSqlDatabase::addDatabase("QSQLITE");
    sqlHandler = QSqlQuery(db);
}

// 连接到数据库
template<class T>
bool Dao<T>::getConnection() {
    // 如果这个数据库连接对象有一个有效的数据库驱动
    if (db.isValid()) {
        db.setDatabaseName("G://Code//QT//OSproject//db//CP_db.db");  // 数据库名字
        if (!db.open()) {                 // 如果数据库连接失败，则弹出
            QMessageBox::critical(0, "无法打开数据库",
                                  "无法建立数据库连接");
            return false;
        }
        return true;
    } else
        return false;
}

// 关闭数据库
template<class T>
void Dao<T>::closeDB() {
    if(db.isValid()){
        db.close();
    }
}

template<class T>
void Dao<T>::setTableName(const QString &value)
{
    tableName = value;
}

// 插入
template<class T>
int Dao<T>::sqlExecute(const QString &sql)
{
//    sqlHandler.prepare(sql);
    if(sqlHandler.exec(sql)){
        //以下是插入后的逻辑
        if(tableName == "Config"){

        }
        else if(tableName == "Result"){

        }
        else{//message

        }
    }else{
        QMessageBox::critical(0, "数据增删改",
                              "数据库增删改操作失败");
        return 0;
    }
}

#endif // DAO_H
