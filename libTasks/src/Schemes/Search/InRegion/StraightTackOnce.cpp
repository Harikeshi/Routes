#include <Task/Schemes/Search/InRegion/StraightTackOnce.hpp>

#include <Geometry/Objects/Circle.hpp>
#include <Task/Exceptions/AlgorithmException.hpp>
#include <Task/Operations/RouteOperations.hpp>
#include <Task/Operations/Vector2D.hpp>
#include <Task/Schemes/Search/Config.hpp>
#include <Task/Schemes/Search/InRegion/RouteType.hpp>
#include <Task/Schemes/Search/InRegion/Utilities.hpp>

using namespace Entities;
using namespace Geometry;

namespace Schemes {
namespace Search {
namespace InRegion {

using Vector2D = Operations::Vector2D<Point2D>;

StraightTackOnce::StraightTackOnce(const Input& input)
    : input{input}
{
}

Outputs::SchemeEfficiency StraightTackOnce::probability()
{
    // Посчитать вероятность и вернуть объект {0, efficiency, *course}
    return Outputs::SchemeEfficiency{};
}

Outputs::Route StraightTackOnce::calculate()
{
    const Ring2D ring = input.region.getPolygon().outer();
    const Point2D start = ring[input.region.getEntrance()];
    const Point2D finish = ring[input.region.getExit()];
    const auto detRange = input.ship.detectionRange;
    const SearchRing::OBB obb = static_cast<SearchRing>(ring).getOBB();
    const double halfLength = std::max(obb.side1.second, obb.side2.second);
    auto linePoint = input.region.longestMidline(obb);
    const Line midLine(linePoint.first, linePoint.second);
    Point2D firstPosition = midLine.findPoint(obb.center, halfLength - detRange, false, false);
    Point2D secondPosition = midLine.findPoint(obb.center, halfLength - detRange, false, true);

    std::vector<Point2D> route{start};
    if (distance(start, firstPosition) >= distance(start, secondPosition))
    {
        std::swap(firstPosition, secondPosition);
    }
    route.push_back(firstPosition);
    route.push_back(secondPosition);

    route.push_back(finish);
    std::vector<double> velocities(route.size() - 1, input.ship.searchVelocity);
    Outputs::Route result(route, velocities);
    return result;
}

} // namespace InRegion
} // namespace Search
} // namespace Schemes
