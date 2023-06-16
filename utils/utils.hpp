#ifndef UTILS_H
#define UTILS_H

#include <QJsonArray>
#include <QVariant>

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
