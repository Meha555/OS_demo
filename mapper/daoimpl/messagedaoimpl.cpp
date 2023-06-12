#include "messagedaoimpl.h"

MessageDaoImpl::MessageDaoImpl(QObject* parent): Dao(parent)
{
    tableName = "Message";
}

MessageDaoImpl::MessageDaoImpl(const QString &tabName, QObject *parent):Dao(tabName,parent)
{}

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
        qCritical()<<sqlHandler.lastError();
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
    if(sqlHandler.exec("select * from message where m_id"+QString::number(m_id))){
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
        qCritical()<<sqlHandler.lastError();
        return Message();
    }
}

int MessageDaoImpl::deleteByID(int m_id)
{
    qDebug()<<"Curr DB is: "<<tableName;
    if(!db.isOpen()) getConnection();
    if(sqlHandler.exec("delete from message where m_id"+QString::number(m_id))){
        return 1;
    }
    else{
        QMessageBox::critical(0, "数据库元组删除失败",
                              sqlHandler.lastError().text());
        qCritical()<<sqlHandler.lastError();
        return 0;
    }
}

int MessageDaoImpl::batchInsert(QQueue<Message> &bf)
{
    qDebug()<<"Curr DB is: "<<tableName;
    if(!db.isOpen()) getConnection();
    // 批量插入数据
    QSqlQuery insert_query(db);
    insert_query.prepare("INSERT INTO message (m_id, t_id, b_id, op_type, data) VALUES (:m_id, :t_id, :b_id, :op_type, :data)");
    for (auto& msg : bf){
        insert_query.bindValue(":m_id", msg.m_id);
        insert_query.bindValue(":t_id", msg.t_id);
        insert_query.bindValue(":b_id", msg.b_id);
        insert_query.bindValue(":op_type", msg.op_type);
        insert_query.bindValue(":data", msg.data);
        if (!insert_query.exec()) {
            qCritical() << "插入数据失败";
            db.rollback();
            return -1;
        }
    }
    // 提交事务处理
    if(sqlHandler.exec("COMMIT")){
        return sqlHandler.record().count();
    }
    else{
        QMessageBox::critical(0, "批量事务提交失败",
                              sqlHandler.lastError().text());
        qCritical()<<sqlHandler.lastError();
        return -1;
    }
}

int MessageDaoImpl::batchDelete(QQueue<Message> &bf)
{
    qDebug()<<"Curr DB is: "<<tableName;
    if(!db.isOpen()) getConnection();
    // 批量删除数据
    QSqlQuery delete_query(db);
    delete_query.prepare("DELETE FROM message WHERE m_id = :m_id");
    for (auto& msg : bf) {
        delete_query.bindValue(":m_id", msg.m_id);
        if (!delete_query.exec()) {
            qCritical() << "删除数据失败";
            db.rollback();
            return -1;
        }
    }
    // 提交事务处理
    if(sqlHandler.exec("COMMIT")){
        return sqlHandler.record().count();
    }
    else{
        QMessageBox::critical(0, "批量事务提交失败",
                              sqlHandler.lastError().text());
        qCritical()<<sqlHandler.lastError();
        return -1;
    }
}
