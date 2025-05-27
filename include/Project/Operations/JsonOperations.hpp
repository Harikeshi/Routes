#pragma once

#include <QFile>
#include <QPointF>
#include <QVector>
#include <QJsonDocument>

#include <stdexcept>

namespace Operations {
inline QJsonObject jsonFromFile(const QString& path)
{
    QByteArray barray;

    if (!path.isEmpty())
    {
        QFile file(path);

        // 
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            throw std::runtime_error("Не возможно открыть файл.");
        }

        barray = file.readAll();

        file.close();
    }
    QJsonDocument doc = QJsonDocument::fromJson(barray);

    return doc.object();
}

inline void setQPointF(QPointF& point, const QJsonValue& json)
{
    point.setX(json[0].toDouble());
    point.setY(json[1].toDouble());
}

inline QPointF toQPointF(const QJsonValue& json)
{
    return QPointF(json[0].toDouble(), json[1].toDouble());
}

inline void setDoublePair(std::pair<double, double>& pair, const QJsonValue& json)
{
    pair.first = json[0].toDouble();
    pair.second = json[1].toDouble();
}

inline void setQVectorQPointF(QVector<QPointF>& result, const QJsonValue& json)
{
    result.clear();

    for (const auto& point : json.toArray())
    {
        result.push_back(toQPointF(point));
    }
}

inline QVector<QPointF> toQVectorQPointF(const QJsonValue& json)
{
    QVector<QPointF> result;

    for (const auto& point : json.toArray())
    {
        result.push_back(toQPointF(point));
    }

    return result;
}
} // namespace Entities