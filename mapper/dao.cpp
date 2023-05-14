#include "dao.h"

Dao::Dao(QObject* parent)
    : QObject(parent) {
    db = QSqlDatabase::addDatabase("QSQLITE");
    sqlHandler = QSqlQuery(db);
}

Dao::~Dao()
{
    closeDB();
}

// 连接到数据库
bool Dao::getConnection() {
    if(db.isOpen()) return true;
        db.setDatabaseName("G://Code//QT//OSproject//db//CP_db.db");  // 数据库位置
    if (!db.open()) {                 // 如果数据库连接失败，则弹出
        QMessageBox::critical(0, "无法打开数据库",
                              "无法建立数据库连接");
        return false;
    }
    return true;
}

// 关闭数据库
void Dao::closeDB() {
    if(db.isOpen())
        db.close();
}

void Dao::setTableName(const QString &value)
{
    tableName = value;
}

// 查询
QVariantList Dao::sqlQuery(const QString &sql)
{
    if(!db.isOpen()) getConnection();
    QVariantList ret;
    if(sqlHandler.exec(sql)){
        //以下是查询后的逻辑
        while(sqlHandler.next()){\
            QVariant t;
            if(tableName == "Config"){
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
                t.setValue(cfg);
            }
            else if(tableName == "Result"){
                Result res(
//                           sqlHandler.value("r_id").toInt(),
                           sqlHandler.value("run_time").toString(),
                           sqlHandler.value("curr_data_num").toInt(),
                           sqlHandler.value("putin_data_num").toInt(),
                           sqlHandler.value("getout_data_num").toInt(),
                           sqlHandler.value("avg_num").toInt());//创建一个Result对象返回
                t.setValue(res);
            }
            else{//message
                Message msg(
//                            sqlHandler.value("m_id").toInt(),
                            sqlHandler.value("t_id").toInt(),
                            sqlHandler.value("b_id").toInt(),
                            sqlHandler.value("data_id").toInt(),
                            sqlHandler.value("op_type").toString(),
                            sqlHandler.value("data").toString());//创建一个Message对象返回
                 t.setValue(msg);
            }
            ret.push_back(t);
        }
        return ret;
    }else{
        QMessageBox::critical(0, "数据库元组查询失败",
                              sqlHandler.lastError().text());
        qDebug()<<sqlHandler.lastError();
        return ret;
    }
}

// 增删改
int Dao::sqlExecute(const QString &sql) {
    if(!db.isOpen()) getConnection();
    if(sqlHandler.exec(sql)){
        return sqlHandler.record().count();
    }else{
        QMessageBox::critical(0, "数据库增删改失败",
                              sqlHandler.lastError().text());
        qDebug()<<sqlHandler.lastError();
        return -1;
    }
}
