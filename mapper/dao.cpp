#include "dao.h"

// 使用默认连接
Dao::Dao(QObject* parent)
    : QObject(parent) {
    db = QSqlDatabase::addDatabase("QSQLITE");
    sqlHandler = QSqlQuery(db);
    qDebug()<<"数据库驱动"+db.driverName()+"安装成功";
    qDebug()<<"当前数据库连接"<<db.connectionName();
}

// 使用特定连接
Dao::Dao(const QString &tabName, QObject* parent)
    : QObject(parent),tableName(tabName) {
    db = QSqlDatabase::addDatabase("QSQLITE",tabName);
    sqlHandler = QSqlQuery(db);
    qDebug()<<"数据库驱动"+db.driverName()+"安装成功";
    qDebug()<<"当前数据库连接"<<db.connectionName();
}

Dao::~Dao()
{
//    closeDB();
}

// 连接到数据库
bool Dao::getConnection() {
    if(db.isOpen()) return true;
    else db.setDatabaseName("G://Code//QT//OSproject//db//CP_db.db");  // 数据库位置
    if (!db.open()) {
        QMessageBox::critical(0, "无法打开数据库",
                              "无法建立数据库连接");
        qCritical()<<db.lastError().text();
        return false;
    }
    return true;
}

// 关闭数据库连接
void Dao::closeConnection() {
    if(db.isOpen()){
        db.close();
        qDebug()<<"数据库连接关闭";
    }
    else qDebug()<<"数据库连接已经关闭";
}

void Dao::setTableName(const QString &value)
{
    tableName = value;
}

void Dao::resetTableName()
{
    tableName = "Null";
}

//// 查询
//int Dao::sqlQuery(const QString &sql){
//    if(!db.isOpen()) getConnection();
//    if(sqlHandler.exec(sql)){
//        resultSet = sqlHandler.record();
//        return sqlHandler.record().count();
//    }
//    else{
//       QMessageBox::critical(0, "数据库元组查询失败",
//                             sqlHandler.lastError().text());
//       qCritical()<<sqlHandler.lastError();
//       return 0;
//    }
//}

//// 通用查询，数据类型需要能进行序列化与反序列化
//QVariantList Dao::sqlQuery(const QString &sql)
//{
//    qDebug()<<"Curr DB is: "<<tableName;
//    if(!db.isOpen()) getConnection();
//    QVariantList ret;
//    if(sqlHandler.exec(sql)){
//        //以下是查询后的逻辑
//        while(sqlHandler.next()){
//            QVariant t;
//            if(tableName == "Config"){
//                Config cfg(
//                           sqlHandler.value("buffer1_size").toInt(),
//                           sqlHandler.value("buffer2_size").toInt(),
//                           sqlHandler.value("buffer3_size").toInt(),
//                           sqlHandler.value("put_num").toInt(),
//                           sqlHandler.value("put_speed").toInt(),
//                           sqlHandler.value("move_num").toInt(),
//                           sqlHandler.value("move_speed").toInt(),
//                           sqlHandler.value("get_num").toInt(),
//                           sqlHandler.value("get_speed").toInt());//创建一个Config对象返回
//                t.setValue(cfg);
//            }
//            else if(tableName == "Result"){
//                Result res(
////                           sqlHandler.value("r_id").toInt(),
//                           sqlHandler.value("run_time").toString(),
//                           sqlHandler.value("curr_data_num").toInt(),
//                           sqlHandler.value("putin_data_num").toInt(),
//                           sqlHandler.value("getout_data_num").toInt(),
//                           sqlHandler.value("avg_num").toInt());//创建一个Result对象返回
//                t.setValue(res);
//            }
//            else if(tableName == "Message"){//message
//                Message msg(
////                            sqlHandler.value("m_id").toInt(),
//                            sqlHandler.value("t_id").toInt(),
//                            sqlHandler.value("b_id").toInt(),
//                            static_cast<Op_Type>(sqlHandler.value("op_type").toInt()),
//                            sqlHandler.value("data").toString());//创建一个Message对象返回
//                 t.setValue(msg);
//            }
//            else{
//                qCritical()<<"尚未选择数据库!";
//            }
//            ret.push_back(t);
//        }
//    }else{
//        QMessageBox::critical(0, "数据库元组查询失败",
//                              sqlHandler.lastError().text());
//        qCritical()<<sqlHandler.lastError();
//    }
//    return ret;
//}

// 通用增删改
int Dao::sqlExecute(const QString &sql) {
    qDebug()<<"Curr DB is: "<<tableName;
    if(getConnection()){
        if(sqlHandler.exec(sql)){
            return sqlHandler.record().count();
        }else{
            QMessageBox::critical(0, "数据库增删改失败",
                                  sqlHandler.lastError().text());
            qCritical()<<sqlHandler.lastError();
            return -1;
        }
    }else return -1;
}

//// 批量增删
//int Dao::sqlBatchExecute(bool type, QQueue<Message>& tuples)
//{
//    qDebug()<<"Curr DB is: "<<tableName;
//    if(!db.isOpen()) getConnection();
//    if(type){
//        // 批量插入数据
//        QSqlQuery insert_query(db);
//        insert_query.prepare("INSERT INTO message (m_id, t_id, b_id, op_type, data) VALUES (:m_id, :t_id, :b_id, :op_type, :data)");
//        for (auto& msg : tuples){
//            insert_query.bindValue(":m_id", msg.m_id);
//            insert_query.bindValue(":t_id", msg.t_id);
//            insert_query.bindValue(":b_id", msg.b_id);
//            insert_query.bindValue(":op_type", msg.op_type);
//            insert_query.bindValue(":data", msg.data);
//            if (!insert_query.exec()) {
//                qCritical() << "插入数据失败";
//                db.rollback();
//                return -1;
//            }
//        }
//    }else{
//        // 批量删除数据
//        QSqlQuery delete_query(db);
//        delete_query.prepare("DELETE FROM message WHERE m_id = :m_id");
//        for (auto& msg : tuples) {
//            delete_query.bindValue(":m_id", msg.m_id);
//            if (!delete_query.exec()) {
//                qCritical() << "删除数据失败";
//                db.rollback();
//                return -1;
//            }
//        }
//    }
//    // 提交事务处理
//    if(sqlHandler.exec("COMMIT")){
//        return sqlHandler.record().count();
//    }
//    else{
//        QMessageBox::critical(0, "批量事务提交失败",
//                              sqlHandler.lastError().text());
//        qCritical()<<sqlHandler.lastError();
//        return -1;
//    }
//}
