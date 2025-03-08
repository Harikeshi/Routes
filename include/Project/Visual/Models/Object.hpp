#pragma once

#include <QPainter>
#include <QPointF>

namespace Visual {
namespace Models {
class Object
{
    double detectionRange;
    double maxVelocity;
    double currentVelocity;
    // double turningRadius;
    // double minLenghtSection;
public:
    void setDetectionRange(const double range)
    {
        detectionRange = range;
    }

    void setCurrentVelocity(const double velocity)
    {
        currentVelocity = velocity;
    }

    void setMaxVelocity(const double velocity)
    {
        maxVelocity = velocity;
    }

    double getDetectionRange() const
    {
        return detectionRange;
    }

    double getCurrentVelocity() const
    {
        return currentVelocity;
    }

    double getMaxVelocity() const
    {
        return maxVelocity;
    }
};
} // namespace Models
} // namespace Visual
