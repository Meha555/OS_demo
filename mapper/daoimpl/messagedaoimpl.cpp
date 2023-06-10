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

}

int MessageDaoImpl::deleteByID(int m_id)
{

}
