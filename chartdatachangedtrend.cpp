#include "chartdatachangedtrend.h"

ChartDataChangedTrend::ChartDataChangedTrend()
{

}

bool ChartDataChangedTrend::readJson(const QString &str)
{
    QJsonDocument doc = QJsonDocument::fromJson(str.toUtf8());
    if(doc.isEmpty() || !doc.isObject()) return false;
    QJsonObject obj = doc.object();
    _data_source->buffer1_data = fromJsonArray<int>(obj["buffer1_data"].toArray());
    _data_source->buffer2_data = fromJsonArray<int>(obj["buffer2_data"].toArray());
    _data_source->buffer3_data = fromJsonArray<int>(obj["buffer3_data"].toArray());
    _data_source->time_staps = fromJsonArray<int>(obj["time_staps"].toArray());
    _data_source->interval = obj["interval"].toInt();
    return true;
}

void ChartDataChangedTrend::writeJson()
{
    json["buffer1_data"] = toJsonArray(_data_source->buffer1_data);
    json["buffer2_data"] = toJsonArray(_data_source->buffer2_data);
    json["buffer3_data"] = toJsonArray(_data_source->buffer3_data);
    json["time_staps"] = toJsonArray(_data_source->time_staps);
    json["interval"] = _data_source->interval;
}
