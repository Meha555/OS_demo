#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <QWaitCondition>
#include <QMutex>

//  自定义记录型信号量类，用于实现统计阻塞线程数和其他详细信息
class Semaphore
{
public:
    Semaphore()=default;
    Q_DISABLE_COPY(Semaphore)
    Semaphore(int val);

    //提供类外访问的全局函数PV操作
    friend void P(Semaphore* s);
    friend void V(Semaphore* s);
    friend bool tryP(Semaphore* s);

    int getCount() const;//获取当前阻塞在信号量上的线程数
    void editCount(int val);//GET操作专用

private:
    int _count{};//当前可用资源数（通过初值-当前值来得出当前阻塞的线程数）
    QMutex _mutex;
    QWaitCondition _condition;
    bool editFlag = false;
};

#endif // SEMAPHORE_H
