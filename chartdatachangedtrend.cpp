#include "chartdatachangedtrend.h"

ChartDataChangedTrend::ChartDataChangedTrend()
{

}

bool ChartDataChangedTrend::readJson(const QString &str)
{
    QJsonDocument doc = QJsonDocument::fromJson(str.toUtf8());
    if(doc.isEmpty() || !doc.isObject()) return false;
    QJsonObject obj = doc.object();
    buffer1_data = fromJsonArray<int>(obj["buffer1_data"].toArray());
    buffer2_data = fromJsonArray<int>(obj["buffer2_data"].toArray());
    buffer3_data = fromJsonArray<int>(obj["buffer3_data"].toArray());
    time_staps = fromJsonArray<int>(obj["time_staps"].toArray());
    interval = obj["interval"].toInt();
    return true;
}

void ChartDataChangedTrend::writeJson()
{
    json["buffer1_data"] = toJsonArray(buffer1_data);
    json["buffer2_data"] = toJsonArray(buffer2_data);
    json["buffer3_data"] = toJsonArray(buffer3_data);
    json["time_staps"] = toJsonArray(time_staps);
    json["interval"] = interval;
}
