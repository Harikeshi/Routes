#include <Task/Schemes/Search/InRegion/_Zigzag.hpp>

#include <Task/Exceptions/AlgorithmException.hpp>
#include <Task/Operations/RouteOperations.hpp>
#include <Task/Operations/Vector2D.hpp>
#include <Task/Schemes/Search/Config.hpp>
#include <Task/Schemes/Search/InRegion/RouteType.hpp>
#include <Task/Schemes/Search/InRegion/Utilities.hpp>

/*! std */
#include <random>

namespace Schemes {
namespace Search {
namespace InRegion {

using Vector2D = Operations::Vector2D<Point2D>;
using Polygon = PrimaryEntities::Polygon<Point2D>;

_Zigzag::_Zigzag(const Input& input)
    : input{input}
{
}

Outputs::SchemeEfficiency _Zigzag::probability()
{
    // Посчитать вероятность и вернуть объект {0, efficiency, *course}
    return Outputs::SchemeEfficiency{};
}

Outputs::Route _Zigzag::calculate()
{
    //region
    const Entities::SearchRegion& region = input.region;

    const Entities::SearchRing& outerRing = region.outer();
    const Point2D mFinal(input.region.getExitPoint());

    //    const double visionDist(input.ship.detectionRange);
    const double vSearch(input.ship.searchVelocity);
    const double tSearch = input.searchTime;
    const double lastOptimalPath = TIME_ACCURRACY * vSearch;

    std::uniform_real_distribution<> u01(0, 1);
    std::uniform_real_distribution<> turnAngles(TURN_ANGLE_MIN_DEG, TURN_ANGLE_MAX_DEG);

    std::random_device randomDevice;
    std::mt19937 randomGenerator(randomDevice());

    // Регион не бывает не валидейт
    region.validate();

    if (tSearch <= region.tackToExit(region.getEntrancePoint()).getLength() / vSearch)
    {
        throw Exceptions::AlgorithmFailure(Exceptions::AlgorithmFailureEnum::ShortSearchTime);
    }

    std::optional<std::pair<Radian, double>> courseLength;
    double tackMaxLen;

    RouteType routeTypeCurr = RouteType::SecondType;
    Radian courseCurr = region.entranceCourse();

    Point2D positionCurr(region.getEntrancePoint());
    std::vector<Point2D> route{positionCurr};

    auto nearestIntersection =
        region.nearestIntersectionAhead(positionCurr, courseCurr, Entities::IntersectionMethod::Geos);

    if (nearestIntersection.empty())
    {
        courseLength =
            region.nextCourse(positionCurr, courseCurr, Radian{M_PI / 4}, true, Entities::IntersectionMethod::Geos);

        if (courseLength.has_value())
        {
            tackMaxLen = courseLength->second;
        }
        else
        {
            throw Exceptions::AlgorithmFailure(Exceptions::AlgorithmFailureEnum::IntersectionNotFound);
        }
    }
    else
    {
        tackMaxLen = region.maxTackLength(positionCurr, nearestIntersection);
    }

    ///////////////////////////////////////////////////////////////////////////

    // инициализация для триангуляции
    /// @todo refactoring,
    /// @todo триангулировать полигон без стартовой точки
    //    std::deque<std::shared_ptr<TriangularGeometry::Top>> lastPath;
    std::vector<Point2D> pathToExit;
    std::vector<Polygon> poly{region.getPolygon()};
    auto triangulation = std::make_shared<TriangularGeometry::TriangularArea>(poly);
    TriangularGeometry::PathFinder path(triangulation);

    double tackLen = tackLength(tackMaxLen, u01(randomGenerator));
    double timeCurr = tackLen / vSearch;
    positionCurr = Vector2D(positionCurr, tackLen, courseCurr).e;

    while (routeTypeCurr == RouteType::SecondType)
    {
        Radian turnAngle = Radian{Degree{turnAngles(randomGenerator)}};

        // Прокладка пути и определение длины пути
        const double distToExit = Operations::pathLength(path.findPath(positionCurr, mFinal));
        const double timeBalance = tSearch - timeCurr - distToExit / vSearch;

        if (timeBalance > 0)
        {
            courseLength = region.nextCourse(positionCurr, courseCurr, turnAngle, true, Entities::IntersectionMethod::Geos);
            if (courseLength.has_value())
            {
                courseCurr = courseLength->first;
            }
            else
            {
                throw Exceptions::AlgorithmFailure(Exceptions::AlgorithmFailureEnum::IntersectionNotFound);
            }
            route.push_back(positionCurr);
            tackLen = tackLength(courseLength->second, u01(randomGenerator));
            positionCurr = Vector2D(positionCurr, tackLen, courseCurr).e;
            timeCurr += tackLen / vSearch;
        }
        else
        {
            double remainingPath =
                (tackLen + distToExit) - vSearch * std::abs(timeBalance);
            if (region.penultimatePosition(route.back(), positionCurr, remainingPath) &&
                region.isSeen(positionCurr))
            {
                route.push_back(positionCurr);
                route.push_back(mFinal);
            }
            else
            {
                pathToExit = region.pathToExitBisection(route.back(), positionCurr, lastOptimalPath, remainingPath, path);
                route.insert(route.end(), pathToExit.begin(), pathToExit.end());
            }
            routeTypeCurr = RouteType::ThirdType;
        }
    }

    std::vector<double> velocities(route.size() - 1, vSearch);
    Outputs::Route result(route, velocities);
    // std::cout << "Время пути: " << Operations::routeDuration(result.points,
    // result.velocities) << " Время отведенное на поиск " << tSearch <<
    // std::endl;
    return result;
}
} // namespace InRegion
} // namespace Search
} // namespace Schemes
