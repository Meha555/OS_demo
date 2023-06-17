#include "semaphore.h"

//#define _WANGDAO_

Semaphore::Semaphore(int val):_count(val) {}

int Semaphore::getCount() const {
    return _count;
}

void P(Semaphore* s, int n) {
    Q_ASSERT_X(n >= 0, "Semaphore::P", "parameter 'n' must be non-negative");

    s->_mutex.lock();
#ifndef _WANGDAO_
    while (n > s->_count) {
        emit s->blocked();
        s->_condition.wait(&s->_mutex);
    }
    s->_count -= n;
#endif
#ifdef _WANGDAO_
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
#ifndef _WANGDAO_
    s->_condition.wakeAll();
    emit s->wakeuped();
#endif
#ifdef _WANGDAO_
    if(s->_count <= 0){//如果有人在等待
        s->_condition.wakeAll();
        emit s->wakeuped();
    }
#endif
    s->_mutex.unlock();
}
