#ifndef MESSAGE_H
#define MESSAGE_H

#include <QString>
#include <QVariant>

#define DATA_LEN 10
typedef enum {PUT,MOVE1,MOVE2,GET} Op_Type;

class Message
{
public:
    Message();
    Message(QString tid,int bid,Op_Type type,const QString& data);

public:
    static int m_id;
    QString t_id;//是哪个线程放进来的
    int b_id;//是在哪个buffer里的
    Op_Type op_type;//放入的线程是什么类型
    QString data;
};

// 注册入元对象系统，以便QVariant使用
Q_DECLARE_METATYPE(Message)

#endif // MESSAGE_H
