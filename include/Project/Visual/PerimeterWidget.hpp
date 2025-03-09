#pragma once

#include "SegmentWidget.hpp"

#include <QDebug>
#include <QPainter>
#include <QVector2D>
#include <QWidget>

namespace Visual {
class PerimeterWidget : public QWidget
{
    Q_OBJECT
public:
    PerimeterWidget(QWidget* parent = nullptr)
        : QWidget(parent)
    {
    }

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

    void draw(QPainter& painter) const
    {
        QPen pen(color, 5);
        pen.setCosmetic(true);

        painter.setPen(pen);

        for (const auto& inner : inners)
        {
            painter.drawPolygon(inner);
        }
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

    QColor getColor() const
    {
        return color;
    }

    void setColor(const QColor& c)
    {
        color = c;
    }

private:
    double minX, minY, maxX, maxY;

    QColor color{Qt::blue};

    // TODO: Outer = inners[0]
    QVector<QPolygonF> inners;
};
} // namespace Visual
