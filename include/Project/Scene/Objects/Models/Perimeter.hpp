#pragma once

#include <QDebug>
#include <QPointF>
#include <QPolygonF>
#include <QVector>

#include "./Input.hpp"
#include "./ValidateOperations.hpp"

#include "../../../Operations/JsonOperations.hpp"

namespace Scene {
namespace Objects {
namespace Models {
class Perimeter : public Input
{
public:
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

        if (rings.size() != 0)
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
    void setLimits(const QVector<QPointF> points)
    {
        for (const auto& point : points)
        {
            minX = std::min(minX, point.x());
            minY = std::min(minY, point.y());
            maxX = std::max(maxX, point.x());
            maxY = std::max(maxY, point.y());
        }
    }

    void setLimits(const QPolygonF polygon)
    {
        for (const auto& point : polygon)
        {
            minX = std::min(minX, point.x());
            minY = std::min(minY, point.y());
            maxX = std::max(maxX, point.x());
            maxY = std::max(maxY, point.y());
        }
    }

    void getLimits() const
    {
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

    Perimeter()
    {
        addValidator("entry_point", [](const QJsonObject& json) { validatePointOrPair(json, "entry_point"); });
        addValidator("exit_point", [](const QJsonObject& json) { validatePointOrPair(json, "exit_point"); });

        addValidator("search_region", [](const QJsonObject& json) { validateRegion(json, "borders"); });
    }

    // "search_region"
    void initializeProperties(const QJsonObject& json)
    {
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

private:
    double minX{1e13}, minY{1e13}, maxX{-1e13}, maxY{-1e13};

    QVector<QPolygonF> rings;

    QPointF entrance;
    QPointF exit;
};
} // namespace Models
} // namespace Objects
} // namespace Scene