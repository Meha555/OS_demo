#include "result.h"

int Result::r_id = 1;

Result::Result()
{
    r_id++;
}

Result::Result(int cur):curr_data_num(cur)
{
    r_id++;
}

Result::Result(QString run, int cur, int put, int get, int avg)
    :run_time(run),curr_data_num(cur),putin_data_num(put),
      getout_data_num(get),avg_num(avg)
{
    r_id++;
}

void Result::collectResult(int cur, int putin, int getout, int avg)
{
    curr_data_num = cur;
    putin_data_num = putin;
    getout_data_num = getout;
    avg_num = avg;
    run_time = timeCounter.elapsed();
}

int Result::getR_id() const
{
    return r_id;
}

void Result::setR_id(int value)
{
    r_id = value;
}

QString Result::getRun_time() const
{
    return run_time;
}

void Result::setRun_time(const QString& value)
{
    run_time = value;
}

int Result::getCurr_data_num() const
{
    return curr_data_num;
}

void Result::setCurr_data_num(int value)
{
    curr_data_num = value;
}

int Result::getPutin_data_num() const
{
    return putin_data_num;
}

void Result::setPutin_data_num(int value)
{
    putin_data_num = value;
}

int Result::getGetout_data_num() const
{
    return getout_data_num;
}

void Result::setGetout_data_num(int value)
{
    getout_data_num = value;
}

int Result::getAvg_num() const
{
    return avg_num;
}

void Result::setAvg_num(int value)
{
    avg_num = value;
}

