#include "chartstat.h"

// ChartStat::ChartStat():_data_source(StatGatherer::instance())
// {
// }

ChartStat::ChartStat()
    : _data_source(new StatGatherer()) {
}
