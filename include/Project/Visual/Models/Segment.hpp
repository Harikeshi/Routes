#pragma once

#include <QColor>
#include <QDebug>
#include <QPainter>
#include <QPointF>
#include <QWidget>

#include <cmath>

namespace Visual {
namespace Models {
class Segment
{
    QPointF start;
    QPointF end;

    double baseSpeed;

public:
    Segment(const QPointF& s = QPointF{}, const QPointF& e = QPointF{}, const double b = 0)
        : start(s), end(e), baseSpeed(b)
    {
    }

    double getSpeed() const
    {
        return baseSpeed;
    }

    QPointF getEnd() const
    {
        return end;
    }

    QPointF getStart() const
    {
        return start;
    }

    void setSpeed(const double speed)
    {
        baseSpeed = speed;
    }

    void setEnd(const QPointF& point)
    {
        end = point;
    }

    void setStart(const QPointF& point)
    {
        start = point;
    }

    double length() const
    {
        return std::hypot(end.x() - start.x(), end.y() - start.y());
    }

    void show() const
    {
        qDebug() << "start: [" << start.x() << ", " << start.y() << "], end: [" << end.x() << ", " << end.y() << "], speed: " << baseSpeed;
    }
};
} // namespace Models
} // namespace Visual
