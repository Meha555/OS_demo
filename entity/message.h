#ifndef MESSAGE_H
#define MESSAGE_H

#include <QString>
#include <QVariant>

class Message
{
public:
    Message();
    Message(int tid,int bid,int dataid,QString type,QString data);

public:
    static int m_id;
    int t_id;
    int b_id;
    int data_id;
    QString op_type;
    QString data;
};

// 注册入元对象系统，以便QVariant使用
Q_DECLARE_METATYPE(Message)

#endif // MESSAGE_H
