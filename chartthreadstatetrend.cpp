#include "chartthreadstatetrend.h"


ChartThreadStateTrend::ChartThreadStateTrend():ChartStat({.buf = "all",.type = THREAD_STATE_TREND})
{

}

bool ChartThreadStateTrend::readJson(const QString &str)
{
    QJsonDocument doc = QJsonDocument::fromJson(str.toUtf8());
    if(doc.isEmpty() || !doc.isObject()) return false;
    QJsonObject obj = doc.object();
    _data_source->put_blocked_num = fromJsonArray<int>(obj["put_blocked_num"].toArray());
    _data_source->move_blocked_num = fromJsonArray<int>(obj["move_blocked_num"].toArray());
    _data_source->get_blocked_num = fromJsonArray<int>(obj["get_blocked_num"].toArray());
    _data_source->time_staps = fromJsonArray<qint64>(obj["time_staps"].toArray());
    _data_source->interval = obj["interval"].toInt();
    return true;
}

QByteArray ChartThreadStateTrend::writeJson()
{
    QJsonObject paramObj;
    paramObj["buf"] = "all";
    paramObj["type"] = THREAD_STATE_TREND;
    json["param"] = paramObj;
    json["put_blocked_num"] = toJsonArray(_data_source->buf1_cur_num);
    json["move_blocked_num"] = toJsonArray(_data_source->buf2_cur_num);
    json["get_blocked_num"] = toJsonArray(_data_source->buf3_cur_num);
    json["time_staps"] = toJsonArray(_data_source->time_staps);
    json["interval"] = _data_source->interval;
    return QJsonDocument(json).toJson(QJsonDocument::Compact);
}
