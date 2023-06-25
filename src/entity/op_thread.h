#ifndef GET_H
#define GET_H

#include <QThread>
#include <QTime>
#include <QString>
#include <QMutex>
#include <QWaitCondition>
#include "mainwindow.h"
#include "semaphore.h"
#include "message.h"
#include "statgatherer.h"
#include "utils.hpp"

#ifdef Q_OS_LINUX
#include <pthread.h>
#endif
#ifdef Q_OS_WIN
#include <windows.h>
#endif

class MainWindow;
class StatGatherer;

class Operation : public QThread
{
    Q_OBJECT
public:
    explicit Operation(QObject *parent = nullptr);
    Operation(Op_Type type, int speed, QObject *parent = nullptr);
//    ~Operation();

protected:
    void run() override;

signals:
//    void putIn(qreal val);
//    void getOut(qreal val);
    void putIn1(qreal val);
    void putIn2(qreal val);
    void putIn3(qreal val);
    void getOut1(qreal val);
    void getOut2(qreal val);
    void getOut3(qreal val);

private:
    QString genRandData();

public:
    Op_Type getOp_type() const;
    void setOp_type(const Op_Type &value);

    int getOp_speed() const;
    void setOp_speed(int value);

    static void pauseThread();
    static void resumeThread();
    static void terminateThread();

    void getStatus();
    int getTID() const;

    void putinData(const int bid);
    void getoutData(const int bid);

private:
    Op_Type op_type;//操作类型
    int op_speed;//操作速度【待修改，应该是线程每次拿走、放入的数据个数】
    QString data;//线程当前持有的data，可能由于没有空位放不到buffer里，因此在这里暂存
    static MainWindow* ptr;//主窗口指针，这里非线程安全
    static QMutex mutex;//互斥锁，用于实现线程暂停
    static QWaitCondition condition;//等待条件，用于实现线程暂停
    static ThreadState state; // 线程当前状态，所有线程的状态都是一致的
    static StatGatherer* gatherer;
};

#endif // GET_H
