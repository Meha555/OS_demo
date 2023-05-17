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

Operation::Operation(QObject *p):QThread(p)
{
    Operation::ptr = static_cast<MainWindow*>(parent());
}

Operation::Operation(Op_Type type, int speed, QObject *p):QThread(p),op_type(type),op_speed(speed)
{
    Operation::ptr = static_cast<MainWindow*>(parent());
}

void Operation::run()
{
//    getStatus();
//    Operation::ptr = static_cast<MainWindow*>(parent());
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
//            P(Operation::ptr->empty1);
            Operation::ptr->empty1->acquire();
            Operation::ptr->mutex1->lock();
            data = genRandData();
            //向buffer1放入1个数据
            putinData(1);
            Operation::ptr->mutex1->unlock();
            Operation::ptr->full1->release();
//            V(Operation::ptr->full1);
            break;
        }
        case MOVE1:{
//            P(Operation::ptr->full1);
//            P(Operation::ptr->empty2);
            Operation::ptr->full1->acquire();
            Operation::ptr->empty2->acquire();
            Operation::ptr->mutex1->lock();
            Operation::ptr->mutex2->lock();
            //从Buffer1中取出1个数据
            getoutData(1);
            //向Buffer2中放入1个数据
            putinData(2);
            Operation::ptr->mutex1->unlock();
            Operation::ptr->mutex2->unlock();
            Operation::ptr->empty1->release();
            Operation::ptr->full2->release();
//            V(Operation::ptr->empty1);
//            V(Operation::ptr->full2);
            break;
        }
        case MOVE2:{
//            P(Operation::ptr->full1);
//            P(Operation::ptr->empty3);
            Operation::ptr->full1->acquire();
            Operation::ptr->empty3->acquire();
            Operation::ptr->mutex1->lock();
            Operation::ptr->mutex3->lock();
            //从Buffer1中取出1个数据
            getoutData(1);
            //向Buffer3中放入1个数据
            putinData(3);
            Operation::ptr->mutex1->unlock();
            Operation::ptr->mutex3->unlock();
            Operation::ptr->empty1->release();
            Operation::ptr->full3->release();
//            V(Operation::ptr->empty1);
//            V(Operation::ptr->full3);
            break;
        }
        case GET:{
            if(Operation::ptr->full2->tryAcquire()){
//            if(tryP(Operation::ptr->full2)){
                Operation::ptr->mutex2->lock();
                //从buffer2中取出1个数据
                getoutData(2);
                Operation::ptr->mutex2->unlock();
                Operation::ptr->empty2->release();
//                V(Operation::ptr->empty2);
            }
            else if(Operation::ptr->full3->tryAcquire()){
//            else if(tryP(Operation::ptr->full3)){
                Operation::ptr->mutex3->lock();
                //从buffer3中取出1个数据
                getoutData(3);
                Operation::ptr->mutex3->unlock();
                Operation::ptr->empty3->release();
//                V(Operation::ptr->empty3);
            }
//            else{
//                mutex_get.lock();
//                cond_get.wait(&mutex);
//                get_blocked_num++;
//                Operation::ptr->full2->editCount(1);
//                Operation::ptr->full3->editCount(1);
//                mutex_get.unlock();
//            }
            break;
        }
        default:return;
        }
//        getStatus();
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

void Operation::getStatus()
{
    QString s;
    QString name;
    switch (op_type) {
    case 0:name = "PUT";break;
    case 1:name = "MOVE1";break;
    case 2:name = "MOVE2";break;
    case 3:name = "GET";break;
    }
    s = name+"线程=>"+"速度:"+QString::number(op_speed)+" 运行:"+(isRunning()?"是":"否")+" 结束:"+(isFinished()?"是":"否");
    qDebug()<<s;
    Operation::ptr->displayLogTextSys(s,true,QColor("orange"));
}

int Operation::getTID() const
{
    int tid;
#ifdef Q_OS_LINUX
    tid = pthread_self();
#endif

#ifdef Q_OS_WIN
    tid = GetCurrentThreadId();
#endif
    qDebug() << "TID: " << tid;
    //qDebug()<<QStringLiteral("@0x%1").arg(quintptr(QThread::currentThreadId()), 16, 16, QLatin1Char('0'));
    return tid;
}

void Operation::putinData(const int bid)
{
    Message msg(getTID(),bid,op_type,data);
//    ptr->dao.sqlExecute(QString("insert into message values(%1,%2,%3,%4,%5)")
//             .arg(msg.m_id).arg(msg.t_id).arg(msg.b_id).arg(msg.op_type).arg(msg.data));
    QString log;
    switch (op_type) {
    case PUT:{
        log = "PUT向buffer1放入1个数据"+data;
        Operation::ptr->buffer1->putinBuffer(msg);
        Operation::ptr->displayLogText(ptr->ui->plainText_log1,log);
        break;
    }
    case MOVE1:{
        log = "MOVE1向Buffer2中放入1个数据"+data;
        Operation::ptr->buffer2->putinBuffer(msg);
        Operation::ptr->displayLogText(ptr->ui->plainText_log2,log);
        break;
    }
    case MOVE2:{
        log = "MOVE2向Buffer3中放入1个数据"+data;
        Operation::ptr->buffer3->putinBuffer(msg);
        Operation::ptr->displayLogText(ptr->ui->plainText_log3,log);
        break;
    }
    default:break;
    }
    qDebug()<<log<<QTime::currentTime();
    emit putIn(1);
}

void Operation::getoutData(const int bid)
{
    QString log;
    switch (op_type) {
    case MOVE1:{
        log = "MOVE1从Buffer1中取出1个数据"+data;
        Operation::ptr->displayLogText(ptr->ui->plainText_log1,log);
        data = Operation::ptr->buffer1->getoutBuffer().data;
        break;
    }
    case MOVE2:{
        log = "MOVE2从Buffer1中取出1个数据"+data;
        Operation::ptr->displayLogText(ptr->ui->plainText_log1,log);
        data = Operation::ptr->buffer1->getoutBuffer().data;
        break;
    }
    case GET:
        if(bid==2){
            log = "GET从buffer2中取出1个数据"+data;
            Operation::ptr->displayLogText(ptr->ui->plainText_log2,log);
            data = Operation::ptr->buffer2->getoutBuffer().data;
        }
        else{
            log = "GET从buffer3中取出1个数据"+data;
            Operation::ptr->displayLogText(ptr->ui->plainText_log3,log);
            data = Operation::ptr->buffer3->getoutBuffer().data;
        }
        break;
    default:break;
    }
    qDebug()<<log<<QTime::currentTime();
    emit getOut(-1);
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


