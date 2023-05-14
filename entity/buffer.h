#ifndef BUFFER_H
#define BUFFER_H

#include <QObject>

class Buffer : public QObject
{
    Q_OBJECT
public:
    explicit Buffer(QObject *parent = nullptr);

    void setCapacity(const int c);
    int getCapacity() const;

    int getFree_space_num() const;
    void setFree_space_num(int value);

    int getCur_num() const;
    void setCur_num(int value);

public:
    static int data_id; // data的唯一索引

//private:
    int capacity;//buffer容量
    int free_space_num;//当前空闲空间个数
    int cur_num = 0;//当前的数据个数
    int putin_num;//已放入当前Buffer中的数据个数
    int getout_num;//已从当前Buffer中取出的数据个数

signals:

};

#endif // BUFFER_H
