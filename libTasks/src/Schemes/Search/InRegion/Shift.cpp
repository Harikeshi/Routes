#include <Task/Schemes/Search/InRegion/Shift.hpp>

#include <Task/Exceptions/AlgorithmException.hpp>
#include <Task/Operations/Geometry/Vector2D.hpp>
#include <Task/Operations/RouteOperations.hpp>
#include <Task/Schemes/Search/InRegion/RouteType.hpp>
#include <Task/Schemes/Search/InRegion/Utilities.hpp>

#include <random>

namespace Schemes {
namespace Search {
namespace InRegion {

using Vector2D = Operations::Vector2D<Point2D>;

Shift::Shift(const Input& input)
    : input{input}
{
}

Shift::Shift(const Input& input, const ShiftParameters& parameters)
    : input{input}, parameters{parameters}
{
}

/**
 * @brief Метод расчета траектории схемы Сдвиг.
 * 
 * @return Outputs::Route Путь рассчитанный в схеме.
 */
Outputs::Route Shift::calculate()
{
    const Entities::SearchRegion& region = input.region;
    const Entities::SearchRing& outerRing = region.outer();
    size_t entranceIdx = region.getEntrance();
    const Point2D& mFinal(outerRing[region.getExit()]);
    //    const double visionDist(input.ship.detectionRange);
    const double vSearch(input.ship.searchVelocity);
    const double searchTime = input.searchTime;
    const double lengthError = TIME_ACCURRACY * vSearch;

    // линейное распределение: y = 2x,  0 <= x <= 1
    std::piecewise_linear_distribution<> l2x(1, 0, 1, [=](double x) {
        return parameters.distributionCoefficient * x;
    });

    std::random_device randomDevice;
    std::mt19937 randomGenerator(randomDevice());

    const Radian turnAngle = Radian{M_PI / 2};

    // инициализация для триангуляции
    std::vector<Point2D> pathToExit;
    std::vector<Polygon2D> poly{region.getPolygon()};
    auto triangle = std::make_shared<TriangularGeometry::TriangularArea>(poly);
    TriangularGeometry::PathFinder path(triangle);

    region.validate();
    if (searchTime <= Geometry::distance(outerRing[entranceIdx], mFinal) / vSearch)
    {
        throw Exceptions::AlgorithmFailure(Exceptions::AlgorithmFailureEnum::ShortSearchTime);
    }

    std::optional<std::pair<Radian, double>> maxTack;
    double tackMaxLen;

    RouteType routeTypeCurr = RouteType::SecondType;
    Radian courseCurr = region.entranceCourse();

    Point2D positionCurr(outerRing[entranceIdx]);
    std::vector<Point2D> route{positionCurr};

    if (Geometry::BorderedLine{positionCurr, outerRing[entranceIdx + 1]}.getAngleTo(Geometry::Line(positionCurr, courseCurr)) >= M_PI * 5 / 12) // близко к перпендикуляру, >= 75 градусов
    {
        maxTack = region.longerTack(positionCurr, courseCurr, Radian{M_PI / 4});
        if (maxTack.has_value())
        {
            courseCurr = maxTack->first;
            tackMaxLen = maxTack->second;
        }
        else
        {
            throw Exceptions::AlgorithmFailure(Exceptions::AlgorithmFailureEnum::IntersectionNotFound);
        }
    }
    else
    {
        Entities::RingIntersection nearestIntersection = region.nearestIntersectionAhead(positionCurr, courseCurr);
        if (!nearestIntersection.empty())
        {
            tackMaxLen = region.maxTackLength(positionCurr, nearestIntersection);
        }
        else
        {
            throw Exceptions::AlgorithmFailure(Exceptions::AlgorithmFailureEnum::IntersectionNotFound);
        }
    }

    double tackLen = tackLength(tackMaxLen, l2x(randomGenerator), parameters.tackDistLeft);
    positionCurr = Vector2D(positionCurr, tackLen, courseCurr).e;
    double timeCurr = tackLen / vSearch;

    while (routeTypeCurr == RouteType::SecondType)
    {
        // Прокладка пути и определение длины пути
        pathToExit = path.findPath(positionCurr, mFinal);
        const double distToExit = Operations::pathLength(pathToExit);
        const double timeBalance = searchTime - timeCurr - distToExit / vSearch;
        if (timeBalance > 0)
        {
            maxTack = region.nextCourse(positionCurr, courseCurr, turnAngle, false);
            if (maxTack.has_value())
            {
                courseCurr = maxTack->first;
            }
            else
            {
                throw Exceptions::AlgorithmFailure(Exceptions::AlgorithmFailureEnum::IntersectionNotFound);
            }
            route.push_back(positionCurr);
            tackLen = tackLength(maxTack->second, l2x(randomGenerator), parameters.tackDistLeft);
            positionCurr = Vector2D(positionCurr, tackLen, courseCurr).e;
            timeCurr += tackLen / vSearch;
        }
        else
        {
            double remainingPath = (tackLen + distToExit) - vSearch * std::abs(timeBalance);
            region.pathToExit(path, route, positionCurr, lengthError, remainingPath);
            routeTypeCurr = RouteType::ThirdType;
        }
    }
    std::vector<double> velocities(route.size() - 1, vSearch);
    Outputs::Route result(route, velocities);
    return result;
}

/**
 * @brief Метод расчета вероятности схемы Сдвиг.
 * 
 * @return Outputs::SchemeEfficiency Результат расчета вероятности схемы. 
 */
Outputs::SchemeEfficiency Shift::probability()
{
    return Outputs::SchemeEfficiency{};
}
} // namespace InRegion
} // namespace Search
} // namespace Schemes
