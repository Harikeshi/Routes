#pragma once

#include <QDebug>
#include <QPointF>
#include <QPolygonF>
#include <QVector>

class Perimeter
{
public:
    void setOuter(const QVector<QPointF>& points)
    {
        if (inners.size() != 0)
            inners[0] = points;
        else
        {
            inners.push_back(points);
        }
    }

    void addInner(const QVector<QPointF>& points)
    {
        inners.push_back(points);
    }

    QVector<QPolygonF> getInners() const
    {
        return inners;
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
    QVector<QPolygonF> inners;
};
