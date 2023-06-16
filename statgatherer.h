#ifndef STATGATHERER_H
#define STATGATHERER_H

#include <QObject>
#include "chartdatachangedtrend.h"

// 不知道为什么只能用typedef，用不了using
//using ThreadState = enum{RUNNING,BLOCK,TERMINATED};
typedef enum{RUNNING,BLOCK,TERMINATED} ThreadState;

// 数据收集者
class StatGatherer
{
public:
    StatGatherer();

    ChartDataChangedTrend genChartDataChangedTrend();

public:
    int speed_coefficient = 1000; // 速度系数(ms)

    int thread_put_num; //put操作个数
    int thread_move_num; //move操作个数
    int thread_get_num; //get操作个数

    int putin_num; //已放入的数据个数
    int getout_num; //已取出的数据个数

    int put_blocked_num; //当前阻塞的put线程数
    int move_blocked_num; //当前阻塞的move线程数
    int get_blocked_num; //当前阻塞的get线程数

};

#endif // STATGATHERER_H
