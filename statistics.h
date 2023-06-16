#ifndef STATITICS_H
#define STATITICS_H

#include <QObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QVariant>

// 绘图所需数据 - 父类
class Statistics
{
public:
    Statistics() = default;

    virtual bool readJson(const QString& json) = 0;
    virtual void writeJson() = 0;

public:
    int interval = 100; // 采样间隔ms
    QVector<int> time_staps; // 采样出的时间戳
    QJsonObject json;
};

#endif // STATITICS_H
