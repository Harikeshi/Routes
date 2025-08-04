#pragma once

#include <QPointF>
#include <QtMath>

namespace Operations {

inline QPointF getPointAtAngleDistance(const QPointF& start, double degrees, double distance)
{
    auto radians = qDegreesToRadians(degrees);

    auto x = start.x() + distance * qCos(radians);
    auto y = start.y() + distance * qSin(radians);

    return QPointF(x, y);
}
} // namespace Operations