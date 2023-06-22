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
    friend void P(Semaphore* s, int n);
    friend void V(Semaphore* s, int n);

    int getCount() const;//获取当前阻塞在信号量上的线程数

signals:
    void blocked();
    void awake();

private:
    int _count = 0; //当前可用资源数（通过初值-当前值来得出当前阻塞的线程数）
    QMutex _mutex; // 保证原子性
    QWaitCondition _condition;
    bool _is_blocked = false; // 用于触发正确的blocked信号以用于统计阻塞线程
};

#endif // SEMAPHORE_H
