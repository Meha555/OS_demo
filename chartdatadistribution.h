#ifndef CHARTDATADISTRIBUTION_H
#define CHARTDATADISTRIBUTION_H

#include "chartstat.h"
#include "utils.hpp"

class ChartDataDistribution : public ChartStat
{
public:
    explicit ChartDataDistribution();
    Q_DISABLE_COPY(ChartDataDistribution)

    bool readJson(const QString &str) override;
    void writeJson() override;
};

/*
{
  "param": {
    "buf": "all",
    "type": "type2"
  },
  "capacity": []
  "cur_num": [],
  "free_space_num": [],
  "putin_num": [],
  "getout_num": []
}
*/

#endif // CHARTDATADISTRIBUTION_H
