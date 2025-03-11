#pragma once

#include <QDebug>
#include <QPointF>
#include <QPolygonF>
#include <QVector>

namespace Visual {
namespace Models {
class Perimeter
{
public:
    void setOuter(const QVector<QPointF>& points)
    {
        setLimits(points);

        if (inners.size() != 0)
            inners[0] = points;
        else
        {
            inners.push_back(points);
        }
    }

    void addInner(const QVector<QPointF>& points)
    {
        setLimits(points);

        inners.push_back(points);
    }

    QVector<QPolygonF> getInners() const
    {
        return inners;
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

    void getLimites() const
    {
    }

public:
    void show() const
    {
        for (const auto& inner : inners)
        {
            qDebug() << "[";
            for (const auto& point : inner)
            {
                qDebug() << "[" << point << "]";
            }
            qDebug() << "]";
        }
    }

private:
    double minX, minY, maxX, maxY;
    QVector<QPolygonF> inners;
};
} // namespace Models
} // namespace Visual
