#ifndef CHARTDATADISTRIBUTION_H
#define CHARTDATADISTRIBUTION_H

#include "chartstat.h"
//#include "utils.hpp"

class ChartDataDistribution : public ChartStat
{
public:
    explicit ChartDataDistribution();
    Q_DISABLE_COPY(ChartDataDistribution)

    bool readJson(const QString &str) override;
    QByteArray writeJson() override;
};

/*
{
  "param": {
    "buf": "all",
    "type": "type2"
  },
  "buf1_cur_num": [],
  "buf2_cur_num": [],
  "buf3_cur_num": [],
  "time_staps": [],
  "interval": 500
}
*/

#endif // CHARTDATADISTRIBUTION_H
