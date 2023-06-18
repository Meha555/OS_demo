#ifndef STATGATHERER_H
#define STATGATHERER_H

#include <mutex>
#include <QObject>
#include <QVector>
#include <QtCore/qglobal.h>
#include "mainwindow.h"

// 不知道为什么只能用typedef，用不了using
//using ThreadState = enum{RUNNING,BLOCK,TERMINATED};
typedef enum{RUNNING,BLOCK,TERMINATED} ThreadState;

// 数据收集者单例
class StatGatherer : public QObject
{
    Q_OBJECT
public:
    StatGatherer();
    StatGatherer(MainWindow* ptr);
    ~StatGatherer() = default;
    Q_DISABLE_COPY(StatGatherer)

    // 使用C++11特性，这样快些
    static StatGatherer* instance(){
        std::call_once(_flag, [&]() {
            _instance = new StatGatherer(MainWindow* ptr);
        });
        return _instance;
    }

    // Getter and Setter
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
    QVector<int> getTime_staps() const;
    void setTime_staps(int time_passed);
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

    int getData_num();

private:
    static StatGatherer* _instance;
    static std::once_flag _flag;

public:
    int speed_coefficient = 1000; // 速度系数(ms)
    int thread_put_num; //put操作个数
    int thread_move_num; //move操作个数
    int thread_get_num; //get操作个数

    QVector<Buffer*> buffers;

//    QVector<int> capacity;//buffer容量
//    QVector<int> free_space_num;//当前空闲空间个数
//    QVector<int> cur_num;//当前的数据个数
//    QVector<int> putin_num;//已放入当前Buffer中的数据个数
//    QVector<int> getout_num;//已从当前Buffer中取出的数据个数

    //  Buffer数据量变化趋势【曲线图+柱状/饼图】 Buffer数据量分布【柱状/饼图】
    int interval = 100; // 采样间隔ms
    QVector<int> buffer1_data;
    QVector<int> buffer2_data;
    QVector<int> buffer3_data;
    QVector<int> time_staps; // 采样出的时间戳.

    // 线程状态变化趋势【曲线图】
    QVector<int> put_blocked_num; //当前阻塞的put线程数
    QVector<int> move_blocked_num; //当前阻塞的move线程数
    QVector<int> get_blocked_num; //当前阻塞的get线程数
};

#endif // STATGATHERER_H
