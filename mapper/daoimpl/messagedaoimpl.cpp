#include "messagedaoimpl.h"

MessageDaoImpl::MessageDaoImpl(QObject* parent): Dao(parent)
{
    tableName = "Message";
}

MessageDaoImpl::MessageDaoImpl(const QString &tabName, QObject *parent):Dao(tabName,parent){}

QVector<Message> MessageDaoImpl::findAll()
{
    qDebug()<<"Curr DB is: "<<tableName;
    if(!db.isOpen()) getConnection();
    QVector<Message> ret;
    if(sqlHandler.exec("select * from message")){
        while(sqlHandler.next()){
            Message msg(
//                            sqlHandler.value("m_id").toInt(),
                            sqlHandler.value("t_id").toInt(),
                            sqlHandler.value("b_id").toInt(),
                            static_cast<Op_Type>(sqlHandler.value("op_type").toInt()),
                            sqlHandler.value("data").toString());//创建一个Message对象返回
            ret.push_back(msg);
            msg.messageInfo();
        }
    }else{
        QMessageBox::critical(0, "数据库元组查询失败",
                              sqlHandler.lastError().text());
        qCritical()<<sqlHandler.lastError().text();
    }
    for(auto& ele: ret){
        ele.messageInfo();
    }
    return ret;
}

Message MessageDaoImpl::findByID(int m_id)
{
    qDebug()<<"Curr DB is: "<<tableName;
    if(!db.isOpen()) getConnection();
    if(sqlHandler.exec("select * from message where m_id="+QString::number(m_id))){
        Message msg(
//                            sqlHandler.value("m_id").toInt(),
                        sqlHandler.value("t_id").toInt(),
                        sqlHandler.value("b_id").toInt(),
                        static_cast<Op_Type>(sqlHandler.value("op_type").toInt()),
                        sqlHandler.value("data").toString());//创建一个Message对象返回
        return msg;
    }
    else{
        QMessageBox::critical(0, "数据库元组查询失败",
                              sqlHandler.lastError().text());
        qCritical()<<sqlHandler.lastError().text();
        return Message();
    }
}

int MessageDaoImpl::deleteByID(int m_id)
{
    qDebug()<<"Curr DB is: "<<tableName;
    if(!db.isOpen()) getConnection();
    if(sqlHandler.exec("delete from message where m_id="+QString::number(m_id))){
        return 1;
    }
    else{
        QMessageBox::critical(0, "数据库元组删除失败",
                              sqlHandler.lastError().text());
        qCritical()<<sqlHandler.lastError().text();
        return 0;
    }
}

// 批量插入数据
int MessageDaoImpl::batchInsert(QQueue<Message>& bf)
{
    qDebug() << "Curr DB is: " << tableName;
    if (!db.isOpen()) getConnection();
    //开启事务
    if (db.transaction()) {
        sqlHandler.prepare("INSERT INTO message (t_id, b_id, op_type, data) VALUES (:t_id, :b_id, :op_type, :data)");
        for (const auto& msg : bf) {
            sqlHandler.bindValue(":t_id", msg.t_id);
            sqlHandler.bindValue(":b_id", msg.b_id);
            sqlHandler.bindValue(":op_type", msg.op_type);
            sqlHandler.bindValue(":data", msg.data);
            if (!sqlHandler.exec()) {
                qCritical() << "插入数据失败: " << sqlHandler.lastError().text();
                db.rollback();
                return -1;
            }
        }
        // 提交事务处理
        if (db.commit()) {
//            return bf.count();
            return sqlHandler.numRowsAffected();
        } else {
            qCritical() << "提交事务失败: " << db.lastError().text();
            db.rollback();
            return -1;
        }
    } else {
        QMessageBox::critical(0, "批量事务开启失败",
                              db.lastError().text());
        qCritical() << db.lastError().text();
        return -1;
    }
}

// 批量删除数据
int MessageDaoImpl::batchDelete(QQueue<Message> &bf)
{
    qDebug()<<"Curr DB is: "<<tableName;
    if(!db.isOpen()) getConnection();
    //开启事务
    if(db.transaction()){
        sqlHandler.prepare("DELETE FROM message WHERE m_id = :m_id");
        for (auto& msg : bf) {
            sqlHandler.bindValue(":m_id", msg.m_id);
            if (!sqlHandler.exec()) {
                qCritical() << "删除数据失败";
                db.rollback();
                return -1;
            }
        }
        // 提交事务处理
        if (db.commit()) {
//            return bf.count();
            return sqlHandler.numRowsAffected();//这个方法能返回最后一次执行sql语句受影响的行数
        } else {
            qCritical() << "提交事务失败: " << db.lastError().text();
            db.rollback();
            return -1;
        }
    }
    else{
        QMessageBox::critical(0, "批量事务开启失败",
                              sqlHandler.lastError().text());
        qCritical()<<sqlHandler.lastError().text();
        return -1;
    }
}
