#include "statgatherer.h"

// 再次强调静态成员变量需要在源文件中定义！！！
StatGatherer* StatGatherer::_instance = nullptr;
std::once_flag StatGatherer::_flag;

int StatGatherer::getGet_blocked_num() const
{
    return get_blocked_num;
}

void StatGatherer::setGet_blocked_num(int value)
{
    get_blocked_num = value;
}

void StatGatherer::increGet_blocked_num()
{
    get_blocked_num++;
}

void StatGatherer::decreGet_blocked_num()
{
    get_blocked_num--;
}

int StatGatherer::getMove_blocked_num() const
{
    return move_blocked_num;
}

void StatGatherer::setMove_blocked_num(int value)
{
    move_blocked_num = value;
}

void StatGatherer::increMove_blocked_num()
{
    move_blocked_num++;
}

void StatGatherer::decreMove_blocked_num()
{
    move_blocked_num--;
}

int StatGatherer::getPut_blocked_num() const
{
    return put_blocked_num;
}

void StatGatherer::setPut_blocked_num(int value)
{
    put_blocked_num = value;
}

void StatGatherer::increPut_blocked_num()
{
    put_blocked_num++;
}

void StatGatherer::decrePut_blocked_num()
{
    put_blocked_num--;
}

int StatGatherer::getGetout_num() const
{
    return getout_num;
}

void StatGatherer::setGetout_num(int value)
{
    getout_num = value;
}

int StatGatherer::getPutin_num() const
{
    return putin_num;
}

void StatGatherer::setPutin_num(int value)
{
    putin_num = value;
}

QVector<int> StatGatherer::getTime_staps() const
{
    return time_staps;
}

void StatGatherer::setTime_staps(int time_passed)
{
    time_staps.push_back(time_passed);
}

QVector<int> StatGatherer::getBuffer3_data() const
{
    return buffer3_data;
}

void StatGatherer::setBuffer3_data(const int value)
{
    buffer3_data.push_back(value);
}

QVector<int> StatGatherer::getBuffer2_data() const
{
    return buffer2_data;
}

void StatGatherer::setBuffer2_data(const int value)
{
    buffer2_data.push_back(value);
}

QVector<int> StatGatherer::getBuffer1_data() const
{
    return buffer1_data;
}

void StatGatherer::setBuffer1_data(const int value)
{
    buffer1_data.push_back(value);
}

int StatGatherer::getInterval() const
{
    return interval;
}

void StatGatherer::setInterval(const int value)
{
    interval = value;
}

int StatGatherer::getThread_get_num() const
{
    return thread_get_num;
}

void StatGatherer::setThread_get_num(int value)
{
    thread_get_num = value;
}

int StatGatherer::getThread_move_num() const
{
    return thread_move_num;
}

void StatGatherer::setThread_move_num(const int value)
{
    thread_move_num = value;
}

int StatGatherer::getThread_put_num() const
{
    return thread_put_num;
}

void StatGatherer::setThread_put_num(int value)
{
    thread_put_num = value;
}

int StatGatherer::getData_num()
{
    return time_staps.size();
}
