#pragma once

#include "Project/Models/Perimeter.hpp"
#include "Project/Models/Route.hpp"
#include "Project/Models/Segment.hpp"

#include <cmath>
namespace Scene::Entities {
struct Limits
{
    using Perimeter = Models::Perimeter;
    using Route = Models::Route;
    using Segment = Models::Segment;

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

    double diagonal() const
    {
        return std::hypot(maxX - minX, maxY - minY);
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
        return qMax(getXDifference(), getYDifference());
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
        minX = qMin(minX, perimeter.getMinX());

        minY = qMin(minY, perimeter.getMinY());

        maxX = qMax(maxX, perimeter.getMaxX());

        maxY = qMax(maxY, perimeter.getMaxY());
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
        minX = qMin(minX, segment.getStart().x());
        minX = qMin(minX, segment.getEnd().x());

        minY = qMin(minY, segment.getStart().y());
        minY = qMin(minY, segment.getEnd().y());

        maxX = qMax(maxX, segment.getStart().x());
        maxX = qMax(maxX, segment.getEnd().x());

        maxY = qMax(maxY, segment.getStart().y());
        maxY = qMax(maxY, segment.getEnd().y());
    }

    void reset()
    {
        // Максимум на минимальную, минимум на максимальную
        minX = 1e30;
        minY = 1e30;
        maxX = -1e30;
        maxY = -1e30;
    }

    Limits limitsWithMargins(const double percent) const
    {
        // Отступы по сторонам
        // TODO: Брать процентное отношение от ширины и длины
        const double height = std::fabs(maxY - minY) * percent / 100;
        const double width = std::fabs(maxX - minX) * percent / 100;

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
} // namespace Scene::Entities
