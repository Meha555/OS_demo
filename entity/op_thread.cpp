#include "op_thread.h"
#include "ui_mainwindow.h"

QString charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

int speed_coefficient = 1000;//速度系数(ms)

int thread_put_num; //put操作个数
int thread_move_num; //move操作个数
int thread_get_num; //get操作个数

int put_blocked_num; //当前阻塞的put线程数
int move_blocked_num; //当前阻塞的move线程数
int get_blocked_num; //当前阻塞的get线程数

//for GET
QWaitCondition cond_get;
QMutex mutex_get;

//静态变量在类中仅仅是声明，还需要类外定义，类外定义要放在cpp文件中
ThreadState Operation::state = RUNNING;
QMutex Operation::mutex;//互斥锁，用于实现线程暂停
QWaitCondition Operation::condition;//等待条件，用于实现线程暂停
MainWindow* Operation::ptr = nullptr;

Operation::Operation(QObject *parent):QThread(parent)
{
//    Operation::ptr = static_cast<MainWindow*>(parent());
}

Operation::Operation(Op_Type type, int speed, QObject *parent):QThread(parent),op_type(type),op_speed(speed)
{
//    Operation::ptr = static_cast<MainWindow*>(parent());
}

//Operation::~Operation(){
//    terminateThread();
//}

void Operation::run()
{
    Operation::ptr = static_cast<MainWindow*>(parent());
    while(state != TERMINATED){
        if(state == BLOCK){//暂停
            mutex.lock();
            put_blocked_num = thread_get_num;
            move_blocked_num = thread_move_num;
            get_blocked_num = thread_get_num;
            condition.wait(&mutex);
            mutex.unlock();
        }
        switch (op_type) {
        case PUT:{
            P(Operation::ptr->empty1);
//            Operation::ptr->empty1->acquire();
            Operation::ptr->mutex1->lock();
            QString data = genRandData();
            //向buffer1放入1个数据
            qDebug()<<"PUT向buffer1放入1个数据"<<data<<QTime::currentTime();
            Operation::ptr->displayLogText(ptr->ui->plainText_log1,"PUT向buffer1放入1个数据"+data);
            Operation::ptr->mutex1->unlock();
//            Operation::ptr->full1->release();
            V(Operation::ptr->full1);
            break;
        }
        case MOVE1:{
            P(Operation::ptr->full1);
            P(Operation::ptr->empty2);
//            Operation::ptr->full1->acquire();
//            Operation::ptr->empty2->acquire();
            Operation::ptr->mutex1->lock();
            Operation::ptr->mutex2->lock();
            //从Buffer1中取出1个数据
            qDebug()<<"MOVE1从Buffer1中取出1个数据"<<QTime::currentTime();
            Operation::ptr->displayLogText(ptr->ui->plainText_log1,"MOVE1从Buffer1中取出1个数据");
            //向Buffer2中放入1个数据
            qDebug()<<"MOVE1向Buffer2中放入1个数据"<<QTime::currentTime();
            Operation::ptr->displayLogText(ptr->ui->plainText_log2,"MOVE1向Buffer2中放入1个数据");
            Operation::ptr->mutex1->unlock();
            Operation::ptr->mutex2->unlock();
//            Operation::ptr->empty1->release();
//            Operation::ptr->full2->release();
            V(Operation::ptr->empty1);
            V(Operation::ptr->full2);
            break;
        }
        case MOVE2:{
            P(Operation::ptr->full1);
            P(Operation::ptr->empty3);
//            Operation::ptr->full1->acquire();
//            Operation::ptr->empty3->acquire();
            Operation::ptr->mutex1->lock();
            Operation::ptr->mutex3->lock();
            //从Buffer1中取出1个数据
            qDebug()<<"MOVE2从Buffer1中取出1个数据"<<QTime::currentTime();
            Operation::ptr->displayLogText(ptr->ui->plainText_log1,"MOVE2从Buffer1中取出1个数据");
            //向Buffer3中放入1个数据
            qDebug()<<"MOVE2向Buffer3中放入1个数据"<<QTime::currentTime();
            Operation::ptr->displayLogText(ptr->ui->plainText_log3,"MOVE2向Buffer3中放入1个数据");
            Operation::ptr->mutex1->unlock();
            Operation::ptr->mutex3->unlock();
//            Operation::ptr->empty1->release();
//            Operation::ptr->full3->release();
            V(Operation::ptr->empty1);
            V(Operation::ptr->full3);
            break;
        }
        case GET:{
//            if(!full2_available && !full3_available){

//            }

//            if(Operation::ptr->full2->tryAcquire()){
            if(tryP(Operation::ptr->full2)){
                Operation::ptr->mutex2->lock();
                //从buffer2中取出1个数据
                qDebug()<<"GET从buffer2中取出1个数据"<<QTime::currentTime();
                Operation::ptr->displayLogText(ptr->ui->plainText_log2,"GET从buffer2中取出1个数据");
                Operation::ptr->mutex2->unlock();
//                Operation::ptr->empty2->release();
                V(Operation::ptr->empty2);
            }
//            else if(Operation::ptr->full3->tryAcquire()){
            else if(tryP(Operation::ptr->full3)){
                Operation::ptr->mutex3->lock();
                //从buffer3中取出1个数据
                qDebug()<<"GET从buffer3中取出1个数据"<<QTime::currentTime();
                Operation::ptr->displayLogText(ptr->ui->plainText_log3,"GET从buffer3中取出1个数据");
                Operation::ptr->mutex3->unlock();
//                Operation::ptr->empty3->release();
                V(Operation::ptr->empty3);
            }
            else{
                mutex_get.lock();
                cond_get.wait(&mutex);
                get_blocked_num++;
                Operation::ptr->full2->editCount(1);
                Operation::ptr->full3->editCount(1);
                mutex_get.unlock();
            }
            break;
        }
        default:return;
        }
        msleep(MAX_SPEED - speed_coefficient); // 延迟一段时间
    }
}

QString Operation::genRandData() {
    QString data;
    int len = qrand() % DATA_LEN + 1;
    for(int i = 1; i <= len; i++){
        data += charset[qrand() % charset.size()];
    }
    return data;
}

void Operation::pauseThread()
{
//    if(QThread::isRunning()){
//        state = BLOCK;
//        mutex.lock();
//        condition.wait(&mutex);
//        mutex.unlock();
//    }
    state = BLOCK;
    qDebug()<<"===所有线程暂停===";
    Operation::ptr->displayLogTextSys("===所有线程暂停===",true,QColor("red"));
}

void Operation::resumeThread()
{
//    if(QThread::isRunning()){
        state = RUNNING;
        condition.wakeAll();
        qDebug()<<"===所有线程继续===";
        Operation::ptr->displayLogTextSys("===所有线程继续===",true,QColor("red"));
//    }
}

void Operation::terminateThread()
{
//    if(QThread::isRunning()){
        state = TERMINATED;
        condition.wakeAll();
        qDebug()<<"===所有线程终止===";
        Operation::ptr->displayLogTextSys("===所有线程终止===",true,QColor("red"));
//        quit();
//        wait();
//    }
}

Op_Type Operation::getOp_type() const
{
    return op_type;
}

void Operation::setOp_type(const Op_Type &value)
{
    op_type = value;
}

int Operation::getOp_speed() const
{
    return op_speed;
}

void Operation::setOp_speed(int value)
{
    op_speed = value;
}


