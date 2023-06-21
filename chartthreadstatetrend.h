#ifndef CHARTTHREADSTATETREND_H
#define CHARTTHREADSTATETREND_H

#include "chartstat.h"
//#include "utils.hpp"

class ChartThreadStateTrend : public ChartStat
{
public:
    explicit ChartThreadStateTrend();
    Q_DISABLE_COPY(ChartThreadStateTrend)

    bool readJson(const QString &str) override;
    void writeJson() override;
};

/*
{
  "param":{
    "buf":"all",
    "type":"type3"
  },
  "put_blocked_num": [], // 第一个缓冲区的数据数量列表
  "move_blocked_num": [], // 第二个缓冲区的数据数量列表
  "get_blocked_num": [], // 第三个缓冲区的数据数量列表
  "time_staps": [], // 时间轴刻度列表，单位为秒
  "interval": 500 // 采样间隔，单位为毫秒
}
*/

#endif // CHARTTHREADSTATETREND_H
