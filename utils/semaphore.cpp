#include "semaphore.h"

Semaphore::Semaphore(int val):_count(val) {}

void Semaphore::editCount(int val) {
    _count += val;
    editFlag = true;
}

int Semaphore::getCount() const {
    return _count;
}

void P(Semaphore* s) {
    s->_mutex.lock();
    s->_count--;
    if(s->_count<0){//如果资源不够
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

bool tryP(Semaphore* s) {
    s->_mutex.lock();
    if(s->_count>0){//如果请求成功
        //如果GET操作在get_cond那里登记过请求（修改过计数器(-1)），则这里请求成功的-1就不必要了
        if(s->editFlag){
            s->editFlag = false;
        }
        else s->_count--;
        return true;
    }
    return false;
    s->_mutex.unlock();
}
