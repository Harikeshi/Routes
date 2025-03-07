#pragma once

#include <QPainter>
#include <QPointF>

namespace Visual {
namespace Models {
class Ship
{
    double detectionRange;
    double maxVelocity;
    double searchVelocity;
    // double turningRadius;
    // double minLenghtSection;
public:
    void setDetectionRange(const double range)
    {
        detectionRange = range;
    }

    void setSearchVelocity(const double velocity)
    {
        searchVelocity = velocity;
    }

    void setMaxVelocity(const double velocity)
    {
        maxVelocity = velocity;
    }

    double getDetectionRange() const
    {
        return detectionRange;
    }

    double getSearchVelocity() const
    {
        return searchVelocity;
    }

    double getMaxVelocity() const
    {
        return maxVelocity;
    }
};
} // namespace Models
} // namespace Visual
