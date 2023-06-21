#ifndef BUFFER_H
#define BUFFER_H

#include <QObject>
#include <QQueue>
#include "message.h"

class Buffer : public QObject
{
    Q_OBJECT
public:
    explicit Buffer(QObject *parent = nullptr);
    void putinBuffer(Message& msg);
    Message getoutBuffer();
    void showBuffer();

    void setCapacity(const int c);
    int getCapacity() const;

    int getFree_space_num() const;
    void setFree_space_num(int value);

    int getCur_num() const;
    void setCur_num(int value);

public:
//    static int data_id; // data的唯一索引
    int capacity;//buffer容量
    int free_space_num;//当前空闲空间个数
    int cur_num = 0;//当前的数据个数
    int putin_num;//已放入当前Buffer中的数据个数
    int getout_num;//已从当前Buffer中取出的数据个数
    QQueue<Message> buffer;//内存中的缓冲区，数据先放在这里，等到积攒到一定量后统一写入数据库
    int opt_count = 0;//对当前Buffer进行了多少次操作的计数器，用于与缓冲区配合完成write back on operate

};

#endif // BUFFER_H
