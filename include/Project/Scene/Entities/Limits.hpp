#pragma once

#include "../Objects/Models/Perimeter.hpp"
#include "../Objects/Models/Route.hpp"
#include "../Objects/Models/Segment.hpp"

namespace Scene {
namespace Entities {
struct Limits
{
    using Perimeter = Objects::Models::Perimeter;
    using Route = Objects::Models::Route;
    using Segment = Objects::Models::Segment;

    double minX;
    double minY;
    double maxX;
    double maxY;

    // Учитывайте, что у нас значения 0-800 по дефолту
    Limits(const double miX = 1e30, const double miY = 1e30, const double maX = -1e30, const double maY = -1e30)
        : minX{miX}, minY{miY}, maxX{maX}, maxY{maY}
    {
    }

    void set(const double miX, const double miY, const double maX, const double maY)
    {
        minX = miX;
        minY = miY;
        maxX = maX;
        maxY = maY;
    }

    double getMinX()
    {
        return minX;
    }

    double getMinY()
    {
        return minY;
    }

    double getMaxX()
    {
        return maxX;
    }

    double getMaxY()
    {
        return maxY;
    }

    Limits getSwapped() const
    {
        return Limits{minY, minX, maxY, maxX};
    }

    void swap()
    {
        std::swap(minX, minY);
        std::swap(maxX, maxY);
    }

    double getMaxDifference() const
    {
        return std::max(getXDifference(), getYDifference());
    }

    double getXDifference() const
    {
        return std::fabs(maxX - minX);
    }

    double getYDifference() const
    {
        return std::fabs(maxY - minY);
    }

    // TODO: Ошибка требуется реализация в соответствующих классах setLimits()
    void initFromPerimeter(const Perimeter& perimeter)
    {
        minX = std::min(minX, perimeter.getMinX());

        minY = std::min(minY, perimeter.getMinY());

        maxX = std::max(maxX, perimeter.getMaxX());

        maxY = std::max(maxY, perimeter.getMaxY());
    }

    // TODO : Ошибка требуется реализация в соответствующих классах setLimits()
    void initFromRoutes(const QVector<Route>& routes)
    {
        // максимумы из Routes
        for (const auto& route : routes)
        {
            this->initFromRoute(route);
        }
    }

    // TODO : Ошибка требуется реализация в соответствующих классах setLimits()
    void initFromRoute(const Route& route)
    {
        // максимумы из Route
        for (const auto& segment : route.getSegments())
        {
            this->initFromSegment(segment);
        }
    }

    // TODO : Ошибка требуется реализация в соответствующих классах setLimits()
    void initFromSegment(const Segment& segment)
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
        minX = 1e30;
        minY = 1e30;
        maxX = -1e30;
        maxY = -1e30;
    }

    Limits limitsWithMargins(const double percent) const
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
} // namespace Entities
} // namespace Scene