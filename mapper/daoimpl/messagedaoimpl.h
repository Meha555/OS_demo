#ifndef MESSAGEDAOIMPL_H
#define MESSAGEDAOIMPL_H

#include <QVector>
#include <QQueue>
#include "dao.h"
#include "message.h"

//namespace dao {
//    class MessageDaoImpl;
//}

class MessageDaoImpl : public Dao
{
public:
    MessageDaoImpl(const QString& dbName, QObject *parent = nullptr);
    MessageDaoImpl(const QString& dbName, const QString& tabName,QObject *parent = nullptr);
    QVector<Message> findAll();
    Message findByID(int m_id);
    int deleteByID(int m_id);
    int batchInsert(QQueue<Message>& bf);
    int batchDelete(QQueue<Message>& bf);
    int deleteAll();
};

#endif // MESSAGEDAOIMPL_H
