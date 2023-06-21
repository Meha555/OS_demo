#ifndef UTILS_H
#define UTILS_H

#define _MY

#include <QDebug>
#include <QJsonArray>
#include <QVariant>

#define PRINT(str) do{qDebug()<<str;}while(0);

typedef enum{RUNNING,BLOCK,TERMINATED} ThreadState;

using ProfileType = enum {DATA_CHANGE_TREND,DATA_DISTRIBUTION,THREAD_STATE_TREND};

typedef struct{
    QString buf;
    ProfileType type;
} ChartParam;

template<typename T>
QJsonArray toJsonArray(const QVector<T>& vec)
{
    QJsonArray arr;
    for (const auto& item : vec) {
        arr.append(item);
    }
    return arr;
}

template<typename T>
QVector<T> fromJsonArray(const QJsonArray& arr)
{
    QVector<T> vec;
    for (const auto& item : arr) {
        vec.append(item.toVariant().value<T>());
    }
    return vec;
}

#endif // UTILS_H
