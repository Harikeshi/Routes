#pragma once

// Core
#include <AbstractOperations/Algorithms/Area.hpp>
#include <AbstractOperations/Algorithms/Envelope.hpp>
#include <AbstractOperations/Algorithms/Intersection.hpp>
#include <AbstractOperations/Algorithms/Union.hpp>
#include <Geometry/Operations/CheckPointsInPolygon.hpp>
#include <PrimaryEntities/Entities/Box.hpp>

// Task
#include <Task/Entities/SearchRing.hpp>
#include <Task/Operations/RouteOperations.hpp>
#include <Task/Operations/SegmentOperations.hpp>
#include <Task/Operations/Vector2D.hpp>
#include <Task/Outputs/Route.hpp>
#include <Task/Schemes/Search/InRegion/Input.hpp>
#include <Task/SearchScheme.hpp>

namespace EfficiencyMeasure {
using Input = Schemes::Search::InRegion::Input;
using SearchRing = Entities::SearchRing;
using Line = Entities::Line;

class EfficiencyIndicators
{
protected:
    double routeSquare;
    std::vector<std::vector<int>> observationDensity;
    std::vector<std::vector<bool>> mask;

    std::pair<Point2D, Point2D> boxTack(const Geometry::BorderedLine& tack, const double& detRange)
    {
        double maxX = std::max(tack.getStart().getX(), tack.getFinish().getX());
        double maxY = std::max(tack.getStart().getY(), tack.getFinish().getY());
        double minX = std::min(tack.getStart().getX(), tack.getFinish().getX());
        double minY = std::min(tack.getStart().getY(), tack.getFinish().getY());
        return std::pair(Point2D{minX - detRange, minY - detRange}, Point2D{maxX + detRange, maxY + detRange});
    }

    void initializeMask(const PrimaryEntities::Polygon<Point2D>& polygon, const double& minX, const double& minY, const int& sizeX, const int& sizeY, const int& cellLength)
    {
        for (int i = 0; i < sizeY; ++i)
        {
            std::vector<bool> rowIsPolygon;
            for (int j = 0; j < sizeX; ++j)
            {
                rowIsPolygon.push_back(AbstractOperations::coveredBy(Point2D{minX + j * cellLength, minY + i * cellLength}, polygon));
            }
            mask.push_back(rowIsPolygon);
        }
    }

    void addObsercationTack(const Point2D& minCorner, const Point2D& firstPoint, const Point2D& secondPoint, const double& detRange, const int& cellLength, const int& stepLength)
    {
        int maxVal = std::floor(detRange / stepLength) + 1;
        Geometry::BorderedLine tack(firstPoint, secondPoint);
        if (tack.getLength() < detRange)
        {
            maxVal = std::floor(tack.getLength() / stepLength) + 1;
        }
        auto boxTack = this->boxTack(tack, detRange);

        int sizeXTack = std::floor((boxTack.second.getX() - boxTack.first.getX()) / cellLength) + 1;
        int sizeYTack = std::floor((boxTack.second.getY() - boxTack.first.getY()) / cellLength) + 1;

        int initI = std::floor((boxTack.first.getY() - minCorner.getY()) / cellLength);
        int initJ = std::floor((boxTack.first.getX() - minCorner.getX()) / cellLength);

        for (int i = initI, stepTackY = 0; i < initI + sizeYTack; ++i, ++stepTackY)
        {
            for (int j = initJ, stepTackX = 0; j < initJ + sizeXTack; ++j, ++stepTackX)
            {
                double dist = Operations::pointToSegmentDistance(Point2D{boxTack.first.getX() + stepTackX * cellLength, boxTack.first.getY() + stepTackY * cellLength}, firstPoint, secondPoint);
                if (dist > detRange)
                {
                    continue;
                }
                observationDensity[i][j] += maxVal - std::floor(dist / stepLength);
            }
        }
    }

    void sumObservationTack(const PrimaryEntities::Box<Point2D>& box, const Outputs::Route& route, const double& detRange, const int& sizeX, const int& sizeY, const int& cellLength, const int& stepLength)
    {
        observationDensity.resize(sizeY);
        for (int i = 0; i < sizeY; ++i)
        {
            observationDensity[i].resize(sizeX);
        }

        for (size_t i = 0; i < route.points.size() - 1; ++i)
        {
            addObsercationTack(box.min_corner(), route.points[i], route.points[i + 1], detRange, cellLength, stepLength);
        }
    }

    double ObservationIntegral(const double& searchVelocity, const int& cellLength, const int& stepLength)
    {
        double integral = 0.0;
        for (const auto& i : observationDensity)
        {
            for (const auto& j : i)
            {
                integral += j * std::pow(cellLength, 2) * stepLength / searchVelocity;
            }
        }
        return integral;
    }

public:
    EfficiencyIndicators() = default;

    void calculateRouteSquare(const PrimaryEntities::Polygon<Point2D>& polygon, Outputs::Route& route, const double& detRange)
    {
        Polygon2D geometry;
        auto detRing = Operations::detectionRing(route.points, detRange, 360);
        if (detRing.empty())
            routeSquare = 0.0;
        for (size_t i = 0; i < detRing.size(); i++)
        {
            AbstractOperations::unionOf(detRing[i], geometry, geometry, true, true);
        }
        AbstractOperations::intersection(geometry, polygon, geometry);
        routeSquare = AbstractOperations::area(geometry, true, true);
    }

    std::vector<std::vector<int>> calculateObservationDensity(const PrimaryEntities::Polygon<Point2D>& polygon, const Outputs::Route& route, const double& detRange)
    {
        const int cellLength = 200, stepLength = 200;

        PrimaryEntities::Box<Point2D> box;
        AbstractOperations::envelope(polygon.outer(), box);

        box = PrimaryEntities::Box<Point2D>(Point2D(box.min_corner().getX() - detRange, box.min_corner().getY() - detRange), Point2D(box.max_corner().getX() + detRange, box.max_corner().getY() + detRange));

        int sizeX = std::floor((box.max_corner().getX() - box.min_corner().getX()) / cellLength) + 1;
        int sizeY = std::floor((box.max_corner().getY() - box.min_corner().getY()) / cellLength) + 1;

        initializeMask(polygon, box.min_corner().getX(), box.min_corner().getY(), sizeX, sizeY, cellLength);

        sumObservationTack(box, route, detRange, sizeX, sizeY, cellLength, stepLength);

        for (size_t i = 0; i < mask.size(); ++i)
        {
            for (size_t j = 0; j < mask[i].size(); ++j)
            {
                observationDensity[i][j] *= mask[i][j];
            }
        }

        return observationDensity;
    }

    std::string size(const PrimaryEntities::Polygon<Point2D>& polygon) const
    {
        auto [minX, maxX] = std::minmax_element(polygon.outer().begin(), polygon.outer().end(), [](const Point2D& first, const Point2D& second) {
            return first.getX() < second.getX();
        });
        auto [minY, maxY] = std::minmax_element(polygon.outer().begin(), polygon.outer().end(), [](const Point2D& first, const Point2D& second) {
            return first.getY() < second.getY();
        });
        int a = maxX->getX() - minX->getX();
        int b = maxY->getY() - minY->getY();
        return std::to_string(a) + "x" + std::to_string(b);
    }

    double averageProductivity(const PrimaryEntities::Polygon<Point2D>& polygon, const Outputs::Route& route, const double& detRange, const double& searchVelocity, const double& searchTime)
    {
        const int cellLength = 200, stepLength = 200;

        if (observationDensity.empty())
        {
            calculateObservationDensity(polygon, route, detRange);
        }

        return ObservationIntegral(searchVelocity, cellLength, stepLength) / searchTime;
    }

    double averageTime(const PrimaryEntities::Polygon<Point2D>& polygon, const Outputs::Route& route, const double& detRange, const double& searchVelocity)
    {
        const int cellLength = 200, stepLength = 200;

        if (observationDensity.empty())
        {
            calculateObservationDensity(polygon, route, detRange);
        }

        return ObservationIntegral(searchVelocity, cellLength, stepLength) / AbstractOperations::area(polygon, true, true);
    }

    std::vector<std::vector<int>> getObservationDensity() const
    {
        return observationDensity;
    }

    std::vector<std::vector<bool>> getMask() const
    {
        return mask;
    }

    double getRouteSquare()
    {
        return routeSquare;
    }
};

} // namespace EfficiencyMeasure
