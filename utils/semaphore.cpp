#include "semaphore.h"

/* 王道给出的记录型信号量实现对于线程的n不相等的情况有死锁的问题
 * 当n>1时，由于P操作每次在阻塞前都要-n登记在计数器上，
 * 导致V操作唤醒线程时，计数器可能仍小于0，此时无法成功，
 * 唤醒任何被阻塞的线程，而每个线程请求的n不一样，
 * 这样导致n较小的线程本来能被唤醒，却收到n较大的线程的拖累
 */
//#define _WANGDAO

// 这里参考了官方的实现

Semaphore::Semaphore(int val):_count(val) {}

int Semaphore::getCount() const {
    return _count;
}

void P(Semaphore* s, int n) {
    Q_ASSERT_X(n >= 0, "Semaphore::P", "parameter 'n' must be non-negative");

    s->_mutex.lock();
#ifndef _WANGDAO
    while (n > s->_count) {
        if(!s->_is_blocked) emit s->blocked();
        s->_is_blocked = true;
        s->_condition.wait(&s->_mutex);
    }
    if(s->_is_blocked) {
        s->_is_blocked = false;
        emit s->awake();
    }
    s->_count -= n;
#endif
#ifdef _WANGDAO
    s->_count -= n;
    while(s->_count < 0){//如果资源不够
        emit s->blocked();
        s->_condition.wait(&s->_mutex);
    }
#endif
    s->_mutex.unlock();
}

void V(Semaphore* s, int n) {
    Q_ASSERT_X(n >= 0, "Semaphore::V", "parameter 'n' must be non-negative");

    s->_mutex.lock();
    s->_count += n;
#ifndef _WANGDAO
    s->_condition.wakeAll();
//    emit s->wakeuped();
#endif
#ifdef _WANGDAO
    if(s->_count <= 0){//如果有人在等待
        s->_condition.wakeAll();
        emit s->wakeuped();
    }
#endif
    s->_mutex.unlock();
}
