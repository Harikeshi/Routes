#include <Task/Exceptions/AlgorithmException.hpp>
#include <Task/Operations/RouteOperations.hpp>
#include <Task/Operations/Vector2D.hpp>
#include <Task/Schemes/Search/InRegion/DeterminedShift.hpp>
#include <Task/Schemes/Search/InRegion/RouteType.hpp> //! RouteType(TackType)
#include <Task/Schemes/Search/InRegion/Utilities.hpp>

#include <algorithm>

namespace Schemes {
namespace Search {
namespace InRegion {

using Vector2D = Operations::Vector2D<Point2D>;

DeterminedShift::DeterminedShift(const Input& input)
    : input{input}
{
}

/**
 * @brief Метод расчета траектории схемы Детерменированный Сдвиг.
 *
 * @return Outputs::Route Путь расcчитанный в схеме.
 */
Outputs::Route DeterminedShift::calculate()
{
    const Entities::SearchRegion& region = input.region;
    const Entities::SearchRing& outerRing = region.outer();
    size_t entranceIdx = region.getEntrance();
    const Point2D& mFinal(outerRing[region.getExit()]);
    const double vSearch(input.ship.searchVelocity);
    const double searchTime = input.searchTime;
    const double lengthError = TIME_ACCURRACY * vSearch;
    const std::vector<double> constU01vec = generateConstArray(1000);
    size_t iBackward = 0;

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

    if (Geometry::BorderedLine{positionCurr, outerRing[entranceIdx + 1]}.getAngleTo(Geometry::Line(positionCurr, courseCurr)) >= M_PI * 5 / 12) // близко к перперндикуляру, >= 75 градусов
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

    positionCurr = Vector2D(positionCurr, tackMaxLen, courseCurr).e;
    double timeCurr = tackMaxLen / vSearch;

    while (routeTypeCurr == RouteType::SecondType)
    {
        // Прокладка пути и определение длины пути
        pathToExit = path.findPath(positionCurr, mFinal);
        const double distToExit = Operations::pathLength(pathToExit);
        const double timeBalance = searchTime - timeCurr - distToExit / vSearch;
        if (timeBalance > 0)
        {
            route.push_back(positionCurr);
            Radian reverseDir = (courseCurr - Radian{M_PI}).normalize();
            maxTack = region.longerTack(positionCurr, courseCurr, turnAngle);
            if (maxTack.has_value())
            {
                courseCurr = maxTack->first;
                tackMaxLen = maxTack->second;
            }
            else
            {
                courseCurr = reverseDir;
                auto inter = region.nearestIntersectionAhead(positionCurr, courseCurr);
                tackMaxLen = region.maxTackLength(positionCurr, inter) * constU01vec[iBackward++];
            }
            positionCurr = Vector2D(positionCurr, tackMaxLen, courseCurr).e;
            timeCurr += tackMaxLen / vSearch;
        }
        else
        {
            double remainingPath = (tackMaxLen + distToExit) - vSearch * std::abs(timeBalance);
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
Outputs::SchemeEfficiency DeterminedShift::probability()
{
    return Outputs::SchemeEfficiency{};
}
} // namespace InRegion
} // namespace Search
} // namespace Schemes
