#include <Task/Schemes/Search/InRegion/StraightTack.hpp>

#include <Geometry/Objects/Circle.hpp>
#include <Task/Exceptions/AlgorithmException.hpp>
#include <Task/Operations/Geometry/Vector2D.hpp>
#include <Task/Operations/RouteOperations.hpp>
#include <Task/Schemes/Search/Config.hpp>
#include <Task/Schemes/Search/InRegion/RouteType.hpp>
#include <Task/Schemes/Search/InRegion/Utilities.hpp>

using namespace Entities;
using namespace Geometry;

namespace Schemes {
namespace Search {
namespace InRegion {

using Vector2D = Operations::Vector2D<Point2D>;

StraightTack::StraightTack(const Input& input)
    : input{input}
{
}

Outputs::SchemeEfficiency StraightTack::probability()
{
    // Посчитать вероятность и вернуть объект {0, efficiency, *course}
    return Outputs::SchemeEfficiency{};
}

Outputs::Route StraightTack::calculate()
{
    const SearchRing& outerRing = input.region.outer();
    const Point2D start = input.region.getEntrancePoint();
    const Point2D finish = input.region.getExitPoint();
    const double searchVel = input.ship.searchVelocity;
    const double searchTime = input.searchTime;
    const double path = searchVel * searchTime;

    if (input.region.getPolygon().inners().size() > 0)
    {
        throw Exceptions::AlgorithmFailure(Exceptions::AlgorithmFailureEnum::TryDifferentScheme);
    }

    if (searchTime <= Geometry::distance(start, finish) / searchVel)
    {
        throw Exceptions::AlgorithmFailure(Exceptions::AlgorithmFailureEnum::ShortSearchTime);
    }

    input.region.validate();

    const SearchRing::OBB obb = static_cast<SearchRing>(outerRing).getOBB();
    auto linePoint = input.region.longestMidline(obb);
    const Line midLine(linePoint.first, linePoint.second);
    const auto inter = input.region.intersectionsAllCore(midLine);
    BorderedLine midBorderedLine(inter[0][0].getPoint(), inter[0][1].getPoint());
    Point2D firstPosition = midBorderedLine.findPointAtLine(TRAVERSA_MIN, true);
    Point2D secondPosition = midBorderedLine.findPointAtLine(midBorderedLine.getLength() - TRAVERSA_MIN, true);

    std::vector<Point2D> route{start};
    if (distance(start, firstPosition) >= distance(start, secondPosition))
    {
        std::swap(firstPosition, secondPosition);
    }
    route.push_back(firstPosition);
    route.push_back(secondPosition);

    double exitTackMax = 0, exitTackMin = 0;
    int patrollingCount = 0;

    const auto straightTack = distance(firstPosition, secondPosition);
    const double remainingPatrollingPath = path - distance(start, firstPosition);

    if (distance(firstPosition, finish) > distance(secondPosition, finish))
    {
        exitTackMax = distance(firstPosition, finish);
        exitTackMin = distance(secondPosition, finish);
    }
    else
    {
        exitTackMax = distance(secondPosition, finish);
        exitTackMin = distance(firstPosition, finish);
    }

    int nMin = std::floor((remainingPatrollingPath - exitTackMax) / straightTack);
    int nMax = std::ceil((remainingPatrollingPath - exitTackMin) / straightTack);

    Point2D positionCurr;
    for (int n = nMin; n <= nMax; n++)
    {
        if (input.region.patrollingLength(n, firstPosition, secondPosition) <= remainingPatrollingPath &&
            input.region.patrollingLength(n + 1, firstPosition, secondPosition) > remainingPatrollingPath)
        {
            bool penultimatePosIsCalculated;
            double remainingPath = remainingPatrollingPath - straightTack * n;
            if (n % 2 == 1)
            {
                positionCurr = firstPosition;
                penultimatePosIsCalculated = input.region.penultimatePosition(secondPosition, positionCurr, remainingPath);
            }
            else
            {
                positionCurr = secondPosition;
                penultimatePosIsCalculated = input.region.penultimatePosition(firstPosition, positionCurr, remainingPath);
            }
            if (!penultimatePosIsCalculated)
            {
                std::cout << "Warning! предпоследняя точка некорректна." << std::endl;
            }
            patrollingCount = n;
        }
        else
            continue;
    }

    for (int i = 0; i < patrollingCount - 1; i++)
    {
        route.push_back(route[route.size() - 2]);
    }
    route.push_back(positionCurr);
    route.push_back(finish);

    std::vector<double> velocities(route.size() - 1, searchVel);
    Outputs::Route result(route, velocities);
    return result;
}

} // namespace InRegion
} // namespace Search
} // namespace Schemes
