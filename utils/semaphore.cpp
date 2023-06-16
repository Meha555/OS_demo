#include "semaphore.h"

Semaphore::Semaphore(int val):_count(val) {}

void Semaphore::registerIn() {
    _mutex.lock();
    _count += 1;
    _registration = true;
    _mutex.unlock();
}

int Semaphore::getCount() const {
    return _count;
}

void P(Semaphore* s) {
    s->_mutex.lock();
    s->_count--;
    while(s->_count<0){//如果资源不够
        s->_condition.wait(&s->_mutex);
    }
    s->_mutex.unlock();
}

void V(Semaphore* s) {
    s->_mutex.lock();
    s->_count++;
    if(s->_count<=0){//如果有人在等待
        s->_condition.wakeOne();
    }
    s->_mutex.unlock();
}
