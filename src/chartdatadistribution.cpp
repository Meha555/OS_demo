#include "chartdatadistribution.h"

ChartDataDistribution::ChartDataDistribution():ChartStat({.buf = "all",.type = DATA_DISTRIBUTION})
{

}

bool ChartDataDistribution::readJson(const QString &str)
{
    QJsonDocument doc = QJsonDocument::fromJson(str.toUtf8());
    if(doc.isEmpty() || !doc.isObject()) return false;
    QJsonObject obj = doc.object();
    _data_source->buf1_cur_num = fromJsonArray<int>(obj["buf1_cur_num"].toArray());
    _data_source->buf2_cur_num = fromJsonArray<int>(obj["buf2_cur_num"].toArray());
    _data_source->buf3_cur_num = fromJsonArray<int>(obj["buf3_cur_num"].toArray());
    _data_source->time_staps = fromJsonArray<qint64>(obj["time_staps"].toArray());
    _data_source->interval = obj["interval"].toInt();
    return true;
}

QByteArray ChartDataDistribution::writeJson()
{
    QJsonObject paramObj;
    paramObj["buf"] = "all";
    paramObj["type"] = DATA_DISTRIBUTION;
    json["param"] = paramObj;
    json["buf1_cur_num"] = toJsonArray(_data_source->buf1_cur_num);
    json["buf2_cur_num"] = toJsonArray(_data_source->buf2_cur_num);
    json["buf3_cur_num"] = toJsonArray(_data_source->buf3_cur_num);
    json["time_staps"] = toJsonArray(_data_source->time_staps);
    json["interval"] = _data_source->interval;
    return QJsonDocument(json).toJson(QJsonDocument::Compact);
}
