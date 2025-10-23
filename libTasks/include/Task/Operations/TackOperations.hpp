#pragma once

#include <Geometry/Objects/BorderedLine.hpp>
#include <MathTools/BasicMath/Compare.hpp>
#include <Point/Point2D.hpp>
#include <Task/Operations/Geometry/SegmentOperations.hpp>

namespace Operations {

inline double chordSegment(const Point2D& projection, const Point2D& basePoint, const double& radius, const double& distToLine)
{
    return radius + std::sqrt(pow(radius, 2) - std::pow(distToLine, 2)) - Geometry::distance(basePoint, projection);
}

inline double observationLength(const Geometry::BorderedLine& tack, const Point2D& p, const double& detRange)
{
    const double& distToLine = tack.distanceFromPoint(p);
    if (distToLine - detRange > ACCURACY_DOUBLE * detRange)
    {
        throw std::runtime_error("Точка вне зоны наблюдения!");
    }
    const double& distToStart = Geometry::distance(tack.getStart(), p);
    const double& distToFinish = Geometry::distance(tack.getFinish(), p);
    const Point2D& projection = tack.findNearestLinePoint(p);

    if (distToStart > detRange && distToFinish > detRange)
    {
        return 2 * std::sqrt(std::pow(detRange, 2) - std::pow(distToLine, 2));
    }
    else if (distToStart <= distToFinish)
    {
        const Point2D& baseStart = tack.findPoint(tack.getStart(), detRange, true, true);
        return Operations::chordSegment(baseStart, projection, detRange, distToLine);
    }
    else if (distToStart > distToFinish)
    {
        const Point2D& baseFinish = tack.findPoint(tack.getFinish(), detRange, true, false);
        return Operations::chordSegment(baseFinish, projection, detRange, distToLine);
    }
    else
    {
        return 0;
    }
}

} // namespace Operations
