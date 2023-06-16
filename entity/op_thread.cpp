#include "op_thread.h"
#include "ui_mainwindow.h"

QString charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

// 静态变量在类中仅仅是声明，还需要类外定义，类外定义要放在cpp文件中
ThreadState Operation::state = RUNNING;  // 初始所有线程状态为运行
QMutex Operation::mutex;                 // 互斥锁，用于实现线程暂停
QWaitCondition Operation::condition;     // 等待条件，用于实现线程暂停
MainWindow* Operation::ptr = nullptr;
//StatGatherer* Operation::gatherer = StatGatherer::instance();  // Operation::ptr->gatherer.

Operation::Operation(QObject* p)
    : QThread(p) {
    Operation::ptr = static_cast<MainWindow*>(parent());
}

Operation::Operation(Op_Type type, int speed, QObject* p)
    : QThread(p), op_type(type), op_speed(speed) {
    Operation::ptr = static_cast<MainWindow*>(parent());
}

void Operation::run() {
    //    getStatus();
    //    Operation::ptr = static_cast<MainWindow*>(parent());
    while (state != TERMINATED) {
        //        mutex.lock();
        //        put_blocked_num = thread_put_num - Operation::ptr->empty1->available();
        //        move_blocked_num = thread_move_num - (
        //                    Operation::ptr->full1->available()+Operation::ptr->empty2->available()+
        //                    Operation::ptr->full2->available()+Operation::ptr->empty3->available());
        //        get_blocked_num = thread_get_num - (
        //                    Operation::ptr->full1->available()+Operation::ptr->full2->available());
        //        mutex.unlock();
        if (state == BLOCK) {  // 暂停
            mutex.lock();
            Operation::ptr->gatherer->put_blocked_num = Operation::ptr->gatherer->thread_put_num;
            Operation::ptr->gatherer->move_blocked_num = Operation::ptr->gatherer->thread_move_num;
            Operation::ptr->gatherer->get_blocked_num = Operation::ptr->gatherer->thread_get_num;
            condition.wait(&mutex);
            mutex.unlock();
        }
        switch (op_type) {
            case PUT: {
                P(Operation::ptr->empty1);
                //            Operation::ptr->empty1->acquire();
                Operation::ptr->mutex1->lock();
                // 向buffer1放入op_speed个数据
                for (int i = 1; i <= op_speed; ++i) {
                    data = genRandData();
                    putinData(1);
                }
                Operation::ptr->mutex1->unlock();
                //            Operation::ptr->full1->release();
                V(Operation::ptr->full1);
                break;
            }
            case MOVE1_2: {
                P(Operation::ptr->full1);
                P(Operation::ptr->empty2);
                //            Operation::ptr->full1->acquire();
                //            Operation::ptr->empty2->acquire();
                Operation::ptr->mutex1->lock();
                Operation::ptr->mutex2->lock();
                for (int i = 1; i <= op_speed; ++i) {
                    // 从Buffer1中取出1个数据
                    getoutData(1);
                    // 向Buffer2中放入1个数据
                    putinData(2);
                }
                Operation::ptr->mutex1->unlock();
                Operation::ptr->mutex2->unlock();
                //            Operation::ptr->empty1->release();
                //            Operation::ptr->full2->release();
                V(Operation::ptr->empty1);
                V(Operation::ptr->full2);
                break;
            }
            case MOVE1_3: {
                P(Operation::ptr->full1);
                P(Operation::ptr->empty3);
                //            Operation::ptr->full1->acquire();
                //            Operation::ptr->empty3->acquire();
                Operation::ptr->mutex1->lock();
                Operation::ptr->mutex3->lock();
                for (int i = 1; i <= op_speed; ++i) {
                    // 从Buffer1中取出1个数据
                    getoutData(1);
                    // 向Buffer3中放入1个数据
                    putinData(3);
                }
                Operation::ptr->mutex1->unlock();
                Operation::ptr->mutex3->unlock();
                //            Operation::ptr->empty1->release();
                //            Operation::ptr->full3->release();
                V(Operation::ptr->empty1);
                V(Operation::ptr->full3);
                break;
            }
            case GET2: {
                P(Operation::ptr->full2);
                //            Operation::ptr->empty2->acquire();
                Operation::ptr->mutex2->lock();
                for (int i = 1; i <= op_speed; ++i) {
                    // 从buffer2取出1个数据
                    getoutData(2);
                }
                Operation::ptr->mutex2->unlock();
                //            Operation::ptr->full2->release();
                V(Operation::ptr->empty2);
                break;
            }
            case GET3: {
                P(Operation::ptr->full3);
                //            Operation::ptr->empty3->acquire();
                Operation::ptr->mutex3->lock();
                for (int i = 1; i <= op_speed; ++i) {
                    // 从buffer3取出1个数据
                    getoutData(3);
                }
                Operation::ptr->mutex3->unlock();
                //            Operation::ptr->full3->release();
                V(Operation::ptr->empty3);
                break;
            }
            default:
                return;
        }
        //        getStatus();
        msleep(MAX_SPEED - Operation::ptr->gatherer->speed_coefficient);  // 延迟一段时间
    }
}

QString Operation::genRandData() {
    QString data;
    int len = qrand() % DATA_LEN + 1;
    for (int i = 1; i <= len; i++) {
        data += charset[qrand() % charset.size()];
    }
    return data;
}

void Operation::pauseThread() {
    //    if(QThread::isRunning()){
    //        state = BLOCK;
    //        mutex.lock();
    //        condition.wait(&mutex);
    //        mutex.unlock();
    //    }
    if (state == RUNNING)
        state = BLOCK;
    qDebug() << "===所有线程暂停===";
    Operation::ptr->displayLogTextSys("===所有线程暂停===", true, QColor("red"));
}

void Operation::resumeThread() {
    //    if(QThread::isRunning()){
    if (state == BLOCK) {
        state = RUNNING;
        condition.wakeAll();
    }
    qDebug() << "===所有线程继续===";
    Operation::ptr->displayLogTextSys("===所有线程继续===", true, QColor("red"));
    //    }
}

void Operation::terminateThread() {
    //    if(QThread::isRunning()){
    if (state != TERMINATED) {
        state = TERMINATED;
        condition.wakeAll();
        qDebug() << "===所有线程终止===";
        Operation::ptr->displayLogTextSys("===所有线程终止===", true, QColor("red"));
    }
    //        quit();
    //        wait();
    //    }
}

void Operation::getStatus() {
    QString s;
    QString name;
    switch (op_type) {
        case PUT:
            name = "PUT";
            break;
        case MOVE1_2:
            name = "MOVE1";
            break;
        case MOVE1_3:
            name = "MOVE2";
            break;
        case GET2:
            name = "GET2";
            break;
        case GET3:
            name = "GET3";
            break;
    }
    s = name + "线程=>" + "速度:" + QString::number(op_speed) + " 运行:" + (isRunning() ? "是" : "否") + " 结束:" + (isFinished() ? "是" : "否");
    qDebug() << s;
    //    Operation::ptr->displayLogTextSys(s,true,QColor("orange"));
}

inline int Operation::getTID() const {
    int tid;
#ifdef Q_OS_LINUX
    tid = pthread_self();
#endif
#ifdef Q_OS_WIN
    tid = GetCurrentThreadId();
#endif
    qDebug() << "TID: " << tid;
    // qDebug()<<QStringLiteral("@0x%1").arg(quintptr(QThread::currentThreadId()), 16, 16, QLatin1Char('0'));
    return tid;
}

void Operation::putinData(const int bid) {
    Operation::ptr->gatherer->putin_num++;
    Message msg(getTID(), bid, op_type, data);
    QString log;
    switch (op_type) {
        case PUT: {
            log = "PUT向buffer1放入1个数据" + data;
            Operation::ptr->buffer1->putinBuffer(msg);
            Operation::ptr->displayLogText(ptr->ui->plainText_log1, log);
            Operation::ptr->ui->bufBall1->updateProgress(op_speed);
            break;
        }
        case MOVE1_2: {
            log = "MOVE1_2向Buffer2中放入1个数据" + data;
            Operation::ptr->buffer2->putinBuffer(msg);
            Operation::ptr->displayLogText(ptr->ui->plainText_log2, log);
            Operation::ptr->ui->bufBall2->updateProgress(op_speed);
            break;
        }
        case MOVE1_3: {
            log = "MOVE1_3向Buffer3中放入1个数据" + data;
            Operation::ptr->buffer3->putinBuffer(msg);
            Operation::ptr->displayLogText(ptr->ui->plainText_log3, log);
            Operation::ptr->ui->bufBall3->updateProgress(op_speed);
            break;
        }
        default:
            break;
    }
    qDebug() << log;
    //    emit putIn(1);
}

void Operation::getoutData(const int bid) {
    Q_UNUSED(bid)
    Operation::ptr->gatherer->getout_num++;
    QString log;
    switch (op_type) {
        case MOVE1_2: {
            log = "MOVE1_2从Buffer1中取出1个数据" + data;
            Operation::ptr->displayLogText(ptr->ui->plainText_log1, log);
            data = Operation::ptr->buffer1->getoutBuffer().data;
            Operation::ptr->ui->bufBall1->updateProgress(-op_speed);
            break;
        }
        case MOVE1_3: {
            log = "MOVE1_3从Buffer1中取出1个数据" + data;
            Operation::ptr->displayLogText(ptr->ui->plainText_log1, log);
            data = Operation::ptr->buffer1->getoutBuffer().data;
            Operation::ptr->ui->bufBall1->updateProgress(-op_speed);
            break;
        }
        case GET2: {
            log = "GET2从buffer2中取出1个数据" + data;
            Operation::ptr->displayLogText(ptr->ui->plainText_log2, log);
            data = Operation::ptr->buffer2->getoutBuffer().data;
            Operation::ptr->ui->bufBall2->updateProgress(-op_speed);
            break;
        }
        case GET3: {
            log = "GET3从buffer3中取出1个数据" + data;
            Operation::ptr->displayLogText(ptr->ui->plainText_log3, log);
            data = Operation::ptr->buffer3->getoutBuffer().data;
            Operation::ptr->ui->bufBall3->updateProgress(-op_speed);
            break;
        }
        default:
            break;
    }
    qDebug() << log;
    //    emit getOut(-1);
}

Op_Type Operation::getOp_type() const {
    return op_type;
}

void Operation::setOp_type(const Op_Type& value) {
    op_type = value;
}

int Operation::getOp_speed() const {
    return op_speed;
}

void Operation::setOp_speed(int value) {
    op_speed = value;
}
