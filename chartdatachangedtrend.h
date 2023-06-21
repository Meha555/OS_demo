#ifndef CHARTDATACHANGEDTREND_H
#define CHARTDATACHANGEDTREND_H

#include "chartstat.h"
//#include "utils.hpp"

class ChartDataChangedTrend : public ChartStat
{
public:
    explicit ChartDataChangedTrend();
    Q_DISABLE_COPY(ChartDataChangedTrend)

    bool readJson(const QString &str) override;
    void writeJson() override;

};

#endif // CHARTDATACHANGEDTREND_H

/*
{
  "param":{
    "buf":"buffer_id",
    "type":"type1"
  },
  "buffer1_data": [], // 第一个缓冲区的数据数量列表
  "buffer2_data": [], // 第二个缓冲区的数据数量列表
  "buffer3_data": [], // 第三个缓冲区的数据数量列表
  "time_staps": [], // 时间轴刻度列表，单位为秒
  "interval": 500 // 采样间隔，单位为毫秒
}
*/
