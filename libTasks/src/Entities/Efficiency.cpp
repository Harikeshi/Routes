// Task
#include <Task/Entities/Efficiency.hpp>
#include <Task/Operations/Geometry/Vector2D.hpp>
#include <Task/Operations/RouteOperations.hpp>
#include <Task/Operations/TackOperations.hpp>

// Core
#include <AbstractOperations/Algorithms/Area.hpp>
#include <AbstractOperations/Algorithms/Envelope.hpp>
#include <AbstractOperations/Algorithms/Intersection.hpp>
#include <AbstractOperations/Algorithms/Union.hpp>
#include <Geometry/Operations/CheckPointsInPolygon.hpp>
#include <PrimaryEntities/Entities/Box.hpp>

namespace Entities {

std::pair<Point2D, Point2D> EfficiencyIndicators::boxTack(const Geometry::BorderedLine& tack, const double& detRange)
{
    auto [minX, maxX] = std::minmax(tack.getStart().getX(), tack.getFinish().getX());
    auto [minY, maxY] = std::minmax(tack.getStart().getY(), tack.getFinish().getY());
    return std::pair(Point2D{minX - detRange, minY - detRange}, Point2D{maxX + detRange, maxY + detRange});
}

void EfficiencyIndicators::initializeMask(const size_t& sizeX, const size_t& sizeY)
{
    mask.resize(sizeY);
    for (size_t i = 0; i < sizeY; ++i)
    {
        mask[i].resize(sizeX);
    }
}

void EfficiencyIndicators::calculateMask(const PrimaryEntities::Polygon<Point2D>& polygon, const double& minX, const double& minY, const int& sizeX, const int& sizeY)
{
    initializeMask(sizeX, sizeY);
    for (int i = 0; i < sizeY; ++i)
    {
        for (int j = 0; j < sizeX; ++j)
        {
            mask[i][j] = AbstractOperations::coveredBy(Point2D{minX + j * cellLength, minY + i * cellLength}, polygon);
        }
    }
}

void EfficiencyIndicators::initializeObservationDensity(const int& sizeX, const int& sizeY)
{
    observationDensity.resize(sizeY);
    for (int i = 0; i < sizeY; ++i)
    {
        observationDensity[i].resize(sizeX);
    }
}

void EfficiencyIndicators::addObservationTack(const Point2D& minCorner, const Point2D& firstPoint, const Point2D& secondPoint, const double& searchVelocity, const double& detRange)
{
    Geometry::BorderedLine tack(firstPoint, secondPoint);
    auto boxTack = this->boxTack(tack, detRange);

    int sizeXTack = std::floor((boxTack.second.getX() - boxTack.first.getX()) / cellLength) + 1;
    int sizeYTack = std::floor((boxTack.second.getY() - boxTack.first.getY()) / cellLength) + 1;

    int initI = std::floor((boxTack.first.getY() - minCorner.getY()) / cellLength);
    int initJ = std::floor((boxTack.first.getX() - minCorner.getX()) / cellLength);

    for (int i = initI, stepTackY = 0; i < initI + sizeYTack; ++i, ++stepTackY)
    {
        for (int j = initJ, stepTackX = 0; j < initJ + sizeXTack; ++j, ++stepTackX)
        {
            /// @todo уменьшить число арифметических действий
            const Point2D& point{boxTack.first.getX() + stepTackX * cellLength, boxTack.first.getY() + stepTackY * cellLength};
            double dist = Operations::pointToSegmentDistance(point, firstPoint, secondPoint);
            if (dist > detRange)
            {
                continue;
            }
            observationDensity[i][j] += Operations::observationLength(tack, point, detRange) / searchVelocity;
        }
    }
}

void EfficiencyIndicators::sumObservationTack(const PrimaryEntities::Box<Point2D>& box, const Outputs::Route& route, const double& detRange)
{
    for (size_t i = 0; i < route.points.size() - 1; ++i)
    {
        addObservationTack(box.min_corner(), route.points[i], route.points[i + 1], route.velocities[i], detRange);
    }
}

double EfficiencyIndicators::observationIntegral()
{
    auto sumDensity = std::accumulate(observationDensity.begin(), observationDensity.end(), 0, [](double acc, const std::vector<double>& row) {
        for (const auto& value : row)
        {
            acc += value;
        }
        return acc;
    });
    return sumDensity * std::pow(cellLength, 2);
}

void EfficiencyIndicators::calculateRouteSquare(const PrimaryEntities::Polygon<Point2D>& polygon, Outputs::Route& route, const double& detRange)
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

void EfficiencyIndicators::calculateObservationDensity(PrimaryEntities::Polygon<Point2D>& polygon, const Outputs::Route& route, const double& detRange)
{
    bool correct = AbstractOperations::correct(polygon);
    if (!correct)
    {
        throw std::runtime_error("Полигон не скорректирован.");
    }

    PrimaryEntities::Box<Point2D> box;
    AbstractOperations::envelope(polygon.outer(), box);

    box = PrimaryEntities::Box<Point2D>(Point2D(box.min_corner().getX() - detRange, box.min_corner().getY() - detRange), Point2D(box.max_corner().getX() + detRange, box.max_corner().getY() + detRange));

    int sizeX = std::floor((box.max_corner().getX() - box.min_corner().getX()) / cellLength) + 1;
    int sizeY = std::floor((box.max_corner().getY() - box.min_corner().getY()) / cellLength) + 1;

    calculateMask(polygon, box.min_corner().getX(), box.min_corner().getY(), sizeX, sizeY);

    initializeObservationDensity(sizeX, sizeY);
    sumObservationTack(box, route, detRange);

    for (size_t i = 0; i < mask.size(); ++i)
    {
        for (size_t j = 0; j < mask[i].size(); ++j)
        {
            observationDensity[i][j] *= mask[i][j];
        }
    }
}

double EfficiencyIndicators::averageProductivity(PrimaryEntities::Polygon<Point2D>& polygon, const Outputs::Route& route, const double& detRange, const double& searchTime)
{
    if (observationDensity.empty())
    {
        calculateObservationDensity(polygon, route, detRange);
    }
    return observationIntegral() / searchTime;
}

double EfficiencyIndicators::averageTime(PrimaryEntities::Polygon<Point2D>& polygon, const Outputs::Route& route, const double& detRange)
{
    if (observationDensity.empty())
    {
        calculateObservationDensity(polygon, route, detRange);
    }
    return observationIntegral() / AbstractOperations::area(polygon, true, true);
}

std::pair<double, double> EfficiencyIndicators::timeStatistics(PrimaryEntities::Polygon<Point2D>& polygon, const Outputs::Route& route, const double& detRange)
{
    if (observationDensity.empty())
    {
        calculateObservationDensity(polygon, route, detRange);
    }

    double sum = 0, sum2 = 0;
    for (size_t i = 0; i < observationDensity.size(); ++i)
    {
        for (size_t j = 0; j < observationDensity[i].size(); ++j)
        {
            sum += observationDensity[i][j];
            sum2 += observationDensity[i][j] * observationDensity[i][j];
        }
    }

    const double polygonArea = AbstractOperations::area(polygon, true, true);

    double mean = sum * std::pow(cellLength, 2) / polygonArea;

    double variance = sum2 * std::pow(cellLength, 2) / polygonArea - std::pow(mean, 2);

    return std::make_pair(mean, sqrt(variance));
}

std::vector<std::vector<double>> EfficiencyIndicators::getObservationDensity() const
{
    return observationDensity;
}

std::vector<std::vector<bool>> EfficiencyIndicators::getMask() const
{
    return mask;
}

double EfficiencyIndicators::getRouteSquare() const
{
    return routeSquare;
}

} // namespace Entities
