#ifndef GET_H
#define GET_H

#include <QThread>
#include <QTime>
#include <QString>
#include <QMutex>
#include <QWaitCondition>
#include "mainwindow.h"
#include "semaphore.h"

class MainWindow;

#define DATA_LEN 10
typedef enum {RUNNING,BLOCK,TERMINATED} ThreadState;
typedef enum {PUT,MOVE1,MOVE2,GET} Op_Type;

class Operation : public QThread
{
    Q_OBJECT
public:
    explicit Operation(QObject *parent = nullptr);
    Operation(Op_Type type, int speed, QObject *parent = nullptr);
//    ~Operation();

protected:
    void run() override;

private:
    QString genRandData();// 生成随机数

public:
    Op_Type getOp_type() const;
    void setOp_type(const Op_Type &value);

    int getOp_speed() const;
    void setOp_speed(int value);

    static void pauseThread();
    static void resumeThread();
    static void terminateThread();


private:
    Op_Type op_type;//操作类型
    int op_speed;//操作速度
    static MainWindow* ptr;
    static ThreadState state; // 线程当前状态
    static QMutex mutex;//互斥锁，用于实现线程暂停
    static QWaitCondition condition;//等待条件，用于实现线程暂停
};

#endif // GET_H
