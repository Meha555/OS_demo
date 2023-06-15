#ifndef STATITICS_H
#define STATITICS_H

#include <QObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

// 存储统计数据，用于绘制图标

class Statitics
{
    Q_OBJECT
public:
    explicit Statitics(QObject* parent = nullptr);

    QJsonObject readJson(const QJsonDocument& json);
    void writeJson(const QString& str);

};

#endif // STATITICS_H
