#pragma once

#include <QDebug>
#include <QPointF>
#include <QPolygonF>
#include <QVector>

#include "Input.hpp"
#include "ValidateOperations.hpp"

#include "Project/Operations/JsonOperations.hpp"

#include <algorithm>

namespace Models {
struct Perimeter : public Input
{
public:
    Perimeter()
    {
        addValidator("entry_point", [](const QJsonObject& json) { validatePointOrPair(json, "entry_point"); });
        addValidator("exit_point", [](const QJsonObject& json) { validatePointOrPair(json, "exit_point"); });

        addValidator("search_region", [](const QJsonObject& json) { validateRegion(json, "borders"); });
    }

    size_t getId() const override
    {
        return id;
    }

    // "search_region"
    void initializeProperties(const QJsonObject& json) override
    {
        if (json.contains("id"))
            id = json["id"].toInt();

        Operations::setQPointF(entrance, json["entry_point"]);
        Operations::setQPointF(exit, json["exit_point"]);

        rings.clear();

        for (const auto& border : json["borders"].toArray())
        {
            QVector<QPointF> inner;
            // border [[],[]]
            for (const auto& point : border.toArray())
            {
                inner.push_back(Operations::toQPointF(point));
            }

            this->addInner(inner);
        }
    }

    QJsonObject toJson() const override
    {
        QJsonObject obj;
        obj["id"] = static_cast<qint64>(id);

        // TODO: Используются x и y для работы с базой данных.
        // entrance
        QJsonObject entranceObj;
        entranceObj["x"] = entrance.x();
        entranceObj["y"] = entrance.y();
        obj["entrance"] = entranceObj;

        // exit
        QJsonObject exitObj;
        exitObj["x"] = exit.x();
        exitObj["y"] = exit.y();
        obj["exit"] = exitObj;

        // rings
        QJsonArray ringsArray;
        for (const QPolygonF& ring : rings)
        {
            QJsonArray ringArray;
            for (const QPointF& point : ring)
            {
                QJsonObject pointObj;
                pointObj["x"] = point.x();
                pointObj["y"] = point.y();
                ringArray.append(pointObj);
            }
            ringsArray.append(ringArray);
        }
        obj["rings"] = ringsArray;

        return obj;
    }

    void clear()
    {
        for (auto& inner : rings)
        {
            inner.clear();
        }
    }

    void setOuter(const QVector<QPointF>& points)
    {
        setLimits(points);

        if (rings.empty())
            rings[0] = points;
        else
        {
            rings.push_back(points);
        }
    }

    void setRings(const QVector<QPolygonF>& polygons)
    {
        // TODO: clear?
        for (const auto& polygon : polygons)
        {
            setLimits(polygon);
            rings.push_back(polygon);
        }
    }

    void addInner(const QVector<QPointF>& points)
    {
        setLimits(points);

        rings.push_back(points);
    }

    QVector<QPolygonF> getRings() const
    {
        return rings;
    }

    double getMinX() const
    {
        return minX;
    }

    double getMinY() const
    {
        return minY;
    }

    double getMaxX() const
    {
        return maxX;
    }

    double getMaxY() const
    {
        return maxY;
    }

private:
    void setLimits(const QVector<QPointF>& points)
    {
        for (const auto& point : points)
        {
            minX = qMin(minX, point.x());
            minY = qMin(minY, point.y());
            maxX = qMax(maxX, point.x());
            maxY = qMax(maxY, point.y());
        }
    }

    void setLimitsFromPoint(const QPointF& point)
    {
        minX = qMin(minX, point.x());
        minY = qMin(minY, point.y());
        maxX = qMax(maxX, point.x());
        maxY = qMax(maxY, point.y());
    }

    void setLimits(const QPolygonF& polygon)
    {
        for (const auto& point : polygon)
        {
            this->setLimitsFromPoint(point);
        }
    }

public:
    void swapCoordinates()
    {
        for (auto& ring : rings)
        {
            for (auto& point : ring)
            {
                auto x = point.x();
                point.setX(point.y());
                point.setY(x);
            }
        }

        std::swap(minX, minY);
        std::swap(maxX, maxY);
    }

    void show() const
    {
        for (const auto& ring : rings)
        {
            qDebug() << "[";
            for (const auto& point : ring)
            {
                qDebug() << "[" << point << "]";
            }
            qDebug() << "]";
        }
    }

    size_t id{0};
    // TODO: надо ли

    QVector<QPolygonF> rings;

    QPointF entrance;
    QPointF exit;
    double minX{1e13}, minY{1e13}, maxX{-1e13}, maxY{-1e13};
};
} // namespace Models
