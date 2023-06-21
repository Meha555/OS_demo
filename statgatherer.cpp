#include "statgatherer.h"

// 再次强调静态成员变量需要在源文件中定义！！！
StatGatherer* StatGatherer::_instance = nullptr;
std::once_flag StatGatherer::_flag;

StatGatherer::StatGatherer()
{
}

int StatGatherer::getGet_blocked_num() const
{
    if(!get_blocked_num.isEmpty()) return get_blocked_num.back();
    else return 0;
}

void StatGatherer::setGet_blocked_num(int value)
{
    get_blocked_num.append(value);
}

void StatGatherer::increGet_blocked_num()
{
    get_blocked_num.append(get_blocked_num.back()+1);
}

void StatGatherer::decreGet_blocked_num()
{
    get_blocked_num.append(get_blocked_num.back()-1);
}

int StatGatherer::getMove_blocked_num() const
{
    if(!move_blocked_num.isEmpty()) return move_blocked_num.back();
    else return 0;
}

void StatGatherer::setMove_blocked_num(int value)
{
    move_blocked_num.append(value);
}

void StatGatherer::increMove_blocked_num()
{
    move_blocked_num.append(move_blocked_num.back()+1);
}

void StatGatherer::decreMove_blocked_num()
{
    move_blocked_num.append(move_blocked_num.back()-1);;
}

int StatGatherer::getPut_blocked_num() const
{
    if(!put_blocked_num.isEmpty()) return put_blocked_num.back();
    else return 0;
}

void StatGatherer::setPut_blocked_num(int value)
{
    put_blocked_num.append(value);
}

void StatGatherer::increPut_blocked_num()
{
    put_blocked_num.append(put_blocked_num.back()+1);
}

void StatGatherer::decrePut_blocked_num()
{
    put_blocked_num.append(put_blocked_num.back()-1);
}

QVector<qint64> StatGatherer::getTime_staps() const
{
    return time_staps;
}

void StatGatherer::setTime_staps(qint64 time_passed)
{
    time_staps.push_back(time_passed);
}

QVector<int> StatGatherer::getBuffer3_data() const
{
    return buffer3_data;
}

void StatGatherer::setBuffer3_data(const int value)
{
    buffer3_data.append(value);
//    buffer2_data.append(buffers[2]->cur_num);
}

QVector<int> StatGatherer::getBuffer2_data() const
{
    return buffer2_data;
}

void StatGatherer::setBuffer2_data(const int value)
{
    buffer2_data.append(value);
//    buffer2_data.append(buffers[1]->cur_num);
}

QVector<int> StatGatherer::getBuffer1_data() const
{
    return buffer1_data;
}

void StatGatherer::setBuffer1_data(const int value)
{
    buffer1_data.append(value);
//    buffer2_data.append(buffers[1]->cur_num);
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

Config *StatGatherer::getConfig() const
{
    return config;
}

void StatGatherer::setConfig(Config *value)
{
    config = value;
}

QVector<Buffer *> StatGatherer::getBuffers() const
{
    return buffers;
}
