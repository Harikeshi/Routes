#pragma once

#include "Perimeter.hpp"
#include "Route.hpp"
#include "Segment.hpp"

namespace Visual {
namespace Models {
struct Limits
{
    double minX;
    double minY;
    double maxX;
    double maxY;

    double getMaxBorderLength() const
    {
        return std::max(std::fabs(maxX - minX), std::fabs(maxY - minY));
    }

    void initFromPerimeter(const Visual::Models::Perimeter& perimeter)
    {
        minX = std::min(minX, perimeter.getMinX());

        minY = std::min(minY, perimeter.getMinY());

        maxX = std::max(maxX, perimeter.getMaxX());

        maxY = std::max(maxY, perimeter.getMaxY());
    }

    void initFromRoutes(const QVector<Visual::Models::Route> routes)
    {
        // максимумы из Routes
        for (const auto& route : routes)
        {
            this->initFromRoute(route);
        }
    }

    void initFromRoute(const Visual::Models::Route route)
    {
        // максимумы из Routes

        for (const auto& segment : route.getSegments())
        {
            this->initFromSegment(segment);
        }
    }

    void initFromSegment(const Visual::Models::Segment segment)
    {
        // максимумы из Routes
        minX = std::min(minX, segment.getStart().x());
        minX = std::min(minX, segment.getEnd().x());

        minY = std::min(minY, segment.getStart().y());
        minY = std::min(minY, segment.getEnd().y());

        maxX = std::max(maxX, segment.getStart().x());
        maxX = std::max(maxX, segment.getEnd().x());

        maxY = std::max(maxY, segment.getStart().y());
        maxY = std::max(maxY, segment.getEnd().y());
    }

    void reset()
    {
        // Максисум на минимальную, минимум на максимальную
        minX = 1e13;
        minY = 1e13;
        maxX = -1e13;
        maxY = -1e13;
    }

    Limits limitsWithMargins(const double percent)
    {
        // Отступы по сторонам
        // TODO: Брать процентное отношение от ширины и длины
        double height = std::fabs(maxY - minY) * percent / 100;
        double width = std::fabs(maxX - minX) * percent / 100;

        auto miX = minX - width;  // minX
        auto miY = minY - height; // minY
        auto maX = maxX + width;  // maxX
        auto maY = maxY + height; // maxY

        return {miX, miY, maX, maY};
    }

    void show() const
    {
        qDebug() << "MinX: " << minX << "MinY: " << minY << "MaxX: " << maxX << "MaxY: " << maxY;
    }
};
} // namespace Models
} // namespace Visual
