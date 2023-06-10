#ifndef MESSAGEDAOIMPL_H
#define MESSAGEDAOIMPL_H

#include <QVector>
#include "dao.h"
#include "message.h"

class MessageDaoImpl : public Dao
{
public:
    MessageDaoImpl(QObject *parent = nullptr);
    MessageDaoImpl(const QString& tabName,QObject *parent = nullptr);
    QVector<Message> findAll();
    Message findByID(int m_id);
    int deleteByID(int m_id);
};

#endif // MESSAGEDAOIMPL_H
