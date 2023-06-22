#include "configdaoimpl.h"

ConfigDaoImpl::ConfigDaoImpl(const QString &dbName, QObject* parent)
    : Dao(dbName,"Config",parent)
{
}

ConfigDaoImpl::ConfigDaoImpl(const QString& dbName, QString tabName, QObject *parent)
    : Dao(dbName,"Config",parent)
{
}

QVector<Config> ConfigDaoImpl::findAll()
{
    qDebug()<<"Curr DB is: "<<tableName;
    getConnection();
    QVector<Config> ret;
    if(sqlHandler.exec("select * from config")){
        while(sqlHandler.next()){
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
        }
    }else{
        QMessageBox::critical(0, "数据库元组查询失败",
                              sqlHandler.lastError().text());
        qCritical()<<sqlHandler.lastError();
    }
    return ret;
}

Config ConfigDaoImpl::findByID(int c_id)
{
    qDebug()<<"Curr DB is: "<<tableName;
    getConnection();
    if(sqlHandler.exec("select * from config where c_id="+QString::number(c_id))){
        sqlHandler.next(); // 必须加这句，因为QSqlQuery返回的数据集，record是停在第一条记录之前的。所以获得数据集后，必须执行next()或first()到第一条记录，这时候record才是有效的。
        // 否则会报错：QSqlQuery::value: not positioned on a valid record
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
        return cfg;
    }
    else{
        QMessageBox::critical(0, "数据库元组查询失败",
                              sqlHandler.lastError().text());
        qCritical()<<sqlHandler.lastError().text();
        return Config();
    }
}

int ConfigDaoImpl::deleteByID(int c_id)
{
    qDebug()<<"Curr DB is: "<<tableName;
    if(getConnection()){
        if(sqlHandler.exec("delete from config where c_id"+QString::number(c_id))){
            return 1;
        }
        else{
            QMessageBox::critical(0, "数据库元组删除失败",
                                  sqlHandler.lastError().text());
            qCritical()<<sqlHandler.lastError().text();
            return EXECUTEION_ERR;
        }
    }return CONNECTION_ERR;
}

int ConfigDaoImpl::insert(Config &cfg)
{
    qDebug()<<"Curr DB is: "<<tableName;
    if(getConnection()){
        if(sqlHandler.exec(QString("INSERT INTO config (buffer1_size, buffer2_size, buffer3_size, put_num, put_speed, move_num, move_speed, get_num, get_speed) \
                                   VALUES (%1, %2, %3, %4, %5, %6, %7, %8, %9)")
                           .arg(cfg.buffer1_size).arg(cfg.buffer2_size).arg(cfg.buffer3_size)
                           .arg(cfg.put_num).arg(cfg.put_speed)
                           .arg(cfg.move_num).arg(cfg.move_speed)
                           .arg(cfg.get_num).arg(cfg.get_speed))){
            return 1;
        }
        else{
            QMessageBox::critical(0, "数据库元组插入失败",
                                  sqlHandler.lastError().text());
            qCritical()<<sqlHandler.lastError().text();
            return EXECUTEION_ERR;
        }
    }return CONNECTION_ERR;
}
