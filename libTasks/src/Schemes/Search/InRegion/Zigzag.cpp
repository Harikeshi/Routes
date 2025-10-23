#include <Task/Entities/Intersection.hpp>
#include <Task/Schemes/Search/InRegion/Zigzag.hpp>

#include <AbstractOperations/Algorithms/Helper/BufferHelper.hpp>
#include <Task/Exceptions/AlgorithmException.hpp>
#include <Task/Operations/Geometry/Vector2D.hpp>
#include <Task/Operations/RouteOperations.hpp>
#include <Task/Schemes/Search/Config.hpp>
#include <Task/Schemes/Search/InRegion/RouteType.hpp>
#include <Task/Schemes/Search/InRegion/Utilities.hpp>

#include <random>

namespace Schemes {
namespace Search {
namespace InRegion {

using Vector2D = Operations::Vector2D<Point2D>;
using Polygon = PrimaryEntities::Polygon<Point2D>;

Zigzag::Zigzag(const Input& input)
    : input{input}
{
}

Outputs::SchemeEfficiency Zigzag::probability()
{
    // Посчитать вероятность и вернуть объект {0, efficiency, *course}
    return Outputs::SchemeEfficiency{};
}

Outputs::Route Zigzag::calculate()
{
    const Entities::SearchRegion region = input.region;
    const Entities::SearchRing outerRing = region.outer();
    const size_t entranceIdx = region.getEntrance();
    const size_t exitIdx = region.getExit();
    const Point2D& mFinal = region.getExitPoint();
    const double vSearch(input.ship.searchVelocity);
    const double searchTime = input.searchTime;
    const double lengthError = TIME_ACCURRACY * vSearch;

    std::uniform_real_distribution<> u01(0, 1);
    std::uniform_real_distribution<> turnAngles(TURN_ANGLE_MIN_DEG, TURN_ANGLE_MAX_DEG);

    std::random_device randomDevice;
    std::mt19937 randomGenerator(randomDevice());

    region.validate();
    if (searchTime <= Geometry::distance(outerRing[entranceIdx], mFinal) / vSearch)
    {
        throw Exceptions::AlgorithmFailure(Exceptions::AlgorithmFailureEnum::ShortSearchTime);
    }

    std::optional<std::pair<Radian, double>> maxTack;
    double tackMaxLen;

    RouteType routeTypeCurr = RouteType::SecondType;
    Radian courseCurr = region.entranceCourse();

    // инициализация для триангуляции
    /// @todo refactoring,внутрь класса SearchRegion, path - поле класса. Продумать взаимодействие.
    /// @todo триангулировать полигон без стартовой точки
    ///
    std::vector<Point2D> pathToExit;
    auto infPolygon = region.crop(TRAVERSA_MIN);
    const Entities::SearchRegion regionInf(infPolygon[0], infPolygon[0].outer()[entranceIdx], infPolygon[0].outer()[exitIdx]);

    if (!AbstractOperations::correct(infPolygon[0]))
    {
        throw std::runtime_error("Полигон не скорректирован. ");
    }
    auto triangle = std::make_shared<TriangularGeometry::TriangularArea>(infPolygon);
    TriangularGeometry::PathFinder path(triangle);

    Point2D positionCurr(outerRing[entranceIdx]);
    std::vector<Point2D> route{positionCurr};

    Entities::RingIntersection nearestIntersection = regionInf.nearestIntersectionAhead(positionCurr, courseCurr);
    if (nearestIntersection.empty())
    {
        maxTack = regionInf.longerTack(positionCurr, courseCurr, Radian{M_PI / 4});
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
        tackMaxLen = regionInf.maxTackLength(positionCurr, nearestIntersection);
    }

    double tackLen = tackLength(tackMaxLen, u01(randomGenerator));
    double timeCurr = tackLen / vSearch;
    positionCurr = Vector2D(positionCurr, tackLen, courseCurr).e;

    while (routeTypeCurr == RouteType::SecondType)
    {
        Radian turnAngle = Radian{Degree{turnAngles(randomGenerator)}};

        // Прокладка пути и определение длины пути
        pathToExit = path.findPath(positionCurr, mFinal);
        const double distToExit = Operations::pathLength(pathToExit);
        const double timeBalance = searchTime - timeCurr - distToExit / vSearch;
        if (timeBalance > 0)
        {
            maxTack = regionInf.nextCourse(positionCurr, courseCurr, turnAngle);
            if (maxTack.has_value())
            {
                courseCurr = maxTack->first;
            }
            else
            {
                throw Exceptions::AlgorithmFailure(Exceptions::AlgorithmFailureEnum::IntersectionNotFound);
            }
            route.push_back(positionCurr);
            tackLen = tackLength(maxTack->second, u01(randomGenerator));
            positionCurr = Vector2D(positionCurr, tackLen, courseCurr).e;
            timeCurr += tackLen / vSearch;
        }
        else
        {
            /// @todo в отдельную функцию, можно SearchRegion::routeToExit()
            double remainingPath = (tackLen + distToExit) - vSearch * std::abs(timeBalance);
            regionInf.pathToExit(path, route, positionCurr, lengthError, remainingPath);
            routeTypeCurr = RouteType::ThirdType;
        }
    }
    std::vector<double> velocities(route.size() - 1, vSearch);
    Outputs::Route result(route, velocities);
    return result;
}
} // namespace InRegion
} // namespace Search
} // namespace Schemes
