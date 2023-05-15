#ifndef DAO_H
#define DAO_H

#include <QApplication>
#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QMessageBox>
#include <QVector>
#include <QPair>
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
    QVector<T> sqlQuery(const QString& sql,const Args&... args);
    QPair<T,bool> findByID(const QString& sql);
    QVector<T> findAll();
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

// 查询
QVector<T> sqlQuery(const QString& sql,const Args&... args){
        if(sqlHandler.exec(sql)){
            //以下是查询后的逻辑
            while(sqlHandler.next()){
                if(typeid (T) == typeid (Config)){
                    QVector<T> ret;
                    Config cfg(
                               sqlHandler.value("buffer1_size").toInt(),
                               sqlHandler.value("buffer2_size").toInt(),
                               sqlHandler.value("buffer3_size").toInt(),
                               sqlHandler.value("put_num").toInt(),
                               sqlHandler.value("put_speed").toInt(),
                               sqlHandler.value("move_num").toInt(),
                               sqlHandler.value("move_speed").toInt(),
                               sqlHandler.value("get_num").toInt(),
                               sqlHandler.value("get_speed").toInt());//创建一个Config对象返回
                    ret.push_back(cfg);
                    return ret;
                }
                else if(typeid (T) == typeid (Result)){
                    QVector<T> ret;
                    Result res(
                               sqlHandler.value("r_id").toInt(),
                               sqlHandler.value("run_time").toString(),
                               sqlHandler.value("curr_data_num").toInt(),
                               sqlHandler.value("putin_data_num").toInt(),
                               sqlHandler.value("getout_data_num").toInt(),
                               sqlHandler.value("avg_num").toInt());//创建一个Result对象返回
                    ret.push_back(res);
                    return ret;
                }
                else{//message
                    QVector<T> ret;
                    Message msg(
                               sqlHandler.value("m_id").toInt(),
                               sqlHandler.value("t_id").toInt(),
                               sqlHandler.value("b_id").toInt(),
                               sqlHandler.value("data_id").toInt(),
                               sqlHandler.value("op_type").toString(),
                               sqlHandler.value("data").toString());//创建一个Message对象返回
                    ret.push_back(msg);
                    return ret;
                }
            }

        }else{
            QMessageBox::critical(0, "数据库元组查询",
                                  "数据库元组查询失败");
            return QVector<T>();
        }
    }
}

#endif // DAO_H
