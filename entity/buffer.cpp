#include "buffer.h"

//int Buffer::data_id = 1;

Buffer::Buffer(QObject *parent) : QObject(parent)
{
    //    data_id++;
}

void Buffer::putinBuffer(Message &msg)
{
    buffer.enqueue(msg);
    free_space_num--;
    cur_num++;
    putin_num++;
}

Message Buffer::getoutBuffer()
{
    free_space_num++;
    cur_num--;
    getout_num++;
    return buffer.dequeue();
}

void Buffer::setCapacity(const int c)
{
    capacity = c;
}

int Buffer::getCapacity() const
{
    return capacity;
}

int Buffer::getFree_space_num() const
{
    return free_space_num;
}

void Buffer::setFree_space_num(int value)
{
    free_space_num = value;
}

int Buffer::getCur_num() const
{
    return cur_num;
}

void Buffer::setCur_num(int value)
{
    cur_num = value;
}
