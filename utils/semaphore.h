#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <QObject>
#include <QWaitCondition>
#include <QMutex>

//  自定义记录型信号量类，用于实现统计阻塞线程数和其他详细信息
class Semaphore : public QObject
{
    Q_OBJECT
public:
    Q_DISABLE_COPY(Semaphore)
    Semaphore(int val);

    //提供类外访问的全局函数PV操作
    friend void P(Semaphore* s);
    friend void V(Semaphore* s);

    int getCount() const;//获取当前阻塞在信号量上的线程数
    void registerIn();//GET操作专用

signals:
    void blocked();
    void wakeuped();

private:
    int _count = 0; //当前可用资源数（通过初值-当前值来得出当前阻塞的线程数）
    QMutex _mutex; // 保证原子性
    QWaitCondition _condition;
    bool _registration = false;
    bool _cancellation = true;
};

#endif // SEMAPHORE_H
