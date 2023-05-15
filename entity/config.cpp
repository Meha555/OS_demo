#include "config.h"

int Config::c_id = 0;

Config::Config()
{
}

Config::Config(int buf1_size, int buf2_size, int buf3_size, int put_n, int put_speed, int move_n, int move_speed, int get_n, int get_speed)
    :buffer1_size(buf1_size),buffer2_size(buf2_size),buffer3_size(buf3_size),put_num(put_n),put_speed(put_speed),move_num(move_n),move_speed(move_speed),get_num(get_n),get_speed(get_speed)
{
    if(!flag){
        c_id++;
        flag = true;
    }
}

int Config::getBuffer1_size() const
{
    return buffer1_size;
}

void Config::setBuffer1_size(int value)
{
    buffer1_size = value;
}

int Config::getBuffer2_size() const
{
    return buffer2_size;
}

void Config::setBuffer2_size(int value)
{
    buffer2_size = value;
}

int Config::getBuffer3_size() const
{
    return buffer3_size;
}

void Config::setBuffer3_size(int value)
{
    buffer3_size = value;
}

int Config::getPut_num() const
{
    return put_num;
}

void Config::setPut_num(int value)
{
    put_num = value;
}

int Config::getPut_speed() const
{
    return put_speed;
}

void Config::setPut_speed(int value)
{
    put_speed = value;
}

int Config::getMove_num() const
{
    return move_num;
}

void Config::setMove_num(int value)
{
    move_num = value;
}

int Config::getMove_speed() const
{
    return move_speed;
}

void Config::setMove_speed(int value)
{
    move_speed = value;
}

int Config::getGet_num() const
{
    return get_num;
}

void Config::setGet_num(int value)
{
    get_num = value;
}

int Config::getGet_speed() const
{
    return get_speed;
}

void Config::setGet_speed(int value)
{
    get_speed = value;
}

