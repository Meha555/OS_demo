#ifndef STATGATHERER_H
#define STATGATHERER_H

#include <mutex>
#include <QObject>
#include <QVector>
#include "utils.hpp"

// 不知道为什么只能用typedef，用不了using
//using ThreadState = enum{RUNNING,BLOCK,TERMINATED};
//typedef enum{RUNNING,BLOCK,TERMINATED} ThreadState;

class Buffer;
class Config;

// 数据收集者单例
class StatGatherer : public QObject
{
    Q_OBJECT
public:
    StatGatherer();
    ~StatGatherer() = default;
    Q_DISABLE_COPY(StatGatherer)

    // 使用C++11特性，这样快些
    static StatGatherer* instance(){
        std::call_once(_flag, [&]() {
            _instance = new StatGatherer();
        });
        return _instance;
    }

    // Getter and Setter
    Config *getConfig() const;
    void setConfig(Config *value);
    int getGet_blocked_num() const;
    void setGet_blocked_num(int value);
    void increGet_blocked_num();
    void decreGet_blocked_num();
    int getMove_blocked_num() const;
    void setMove_blocked_num(int value);
    void increMove_blocked_num();
    void decreMove_blocked_num();
    int getPut_blocked_num() const;
    void setPut_blocked_num(int value);
    void increPut_blocked_num();
    void decrePut_blocked_num();
    QVector<qint64> getTime_staps() const;
    void setTime_staps(qint64 time_passed);
    QVector<int> getBuffer3_data() const;
    void setBuffer3_data(const int value);
    QVector<int> getBuffer2_data() const;
    void setBuffer2_data(const int value);
    QVector<int> getBuffer1_data() const;
    void setBuffer1_data(const int value);
    int getInterval() const;
    void setInterval(const int value);
    int getThread_get_num() const;
    void setThread_get_num(int value);
    int getThread_move_num() const;
    void setThread_move_num(int value);
    int getThread_put_num() const;
    void setThread_put_num(int value);
    QVector<Buffer *> getBuffers() const;

    int getData_num();

private:
    static StatGatherer* _instance;
    static std::once_flag _flag;

public:
    int speed_coefficient = 1000; // 速度系数(ms)
    int thread_put_num; //put操作个数
    int thread_move_num; //move操作个数
    int thread_get_num; //get操作个数

//    QVector<int> capacity;//buffer容量
//    QVector<int> free_space_num;//当前空闲空间个数
//    QVector<int> putin_num;//已放入当前Buffer中的数据个数
//    QVector<int> getout_num;//已从当前Buffer中取出的数据个数

    //  Buffer数据量变化趋势【曲线图】
    int interval = 100; // 采样间隔ms
    QVector<int> buffer1_data;
    QVector<int> buffer2_data;
    QVector<int> buffer3_data;
    QVector<qint64> time_staps; // 采样出的时间戳

    // Buffer数据量分布【饼图】
    QVector<Buffer *> buffers;
    QVector<int> buf1_cur_num;//当前的数据个数
    QVector<int> buf2_cur_num;//当前的数据个数
    QVector<int> buf3_cur_num;//当前的数据个数

    // 线程状态变化趋势【曲线图】
    Config* config;
    QVector<int> put_blocked_num; //当前阻塞的put线程数
    QVector<int> move_blocked_num; //当前阻塞的move线程数
    QVector<int> get_blocked_num; //当前阻塞的get线程数

};

#endif // STATGATHERER_H
