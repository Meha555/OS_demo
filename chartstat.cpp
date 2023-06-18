#include "chartstat.h"

ChartStat::ChartStat()
    : _data_source(StatGatherer::instance()), param(param) {
}

// ChartStat::ChartStat(const ChartParam &param): _data_source(new StatGatherer()),param(param)
// {

// }

void ChartStat::recognize(const QJsonObject& obj) {
    this->param = {
        .buf = obj["param"].toObject()["buf"].toString(),
        .type = static_cast<ProfileType>(obj["param"].toObject()["type"].toInt())};
}
