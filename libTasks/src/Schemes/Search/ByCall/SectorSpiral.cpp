#include <Task/Schemes/Search/ByCall/SectorSpiral.hpp>

#include <Task/Operations/Geometry/Vector2D.hpp>
#include <Task/Schemes/Search/ByCall/Utilities.hpp>
#include <Task/Schemes/Search/Config.hpp>

namespace Schemes {
namespace Search {
namespace ByCall {

using Vector2D = Operations::Vector2D<Point2D>;

SectorSpiral::SectorSpiral(const Input& input)
    : input{input}
{
}

Outputs::Route SectorSpiral::calculate()
{
    if (input.target.timeSinceContact > MAX_DELAY_TIME)
    {
        throw std::invalid_argument("Слишком большое время запаздывания");
    }
    if (input.ship.maxVelocity < input.ship.searchVelocity)
    {
        throw std::invalid_argument("Максимальная скорость меньше поисковой");
    }

    std::vector<Point2D> positions;

    Radian alphaL = input.target.courseSector.second;
    Radian alphaR = input.target.courseSector.first;

    bool isOnTheLeft = Operations::layoutOpts<Point2D>::isPointTheLeft(input.target.contactPoint, Vector2D(input.target.contactPoint, input.target.contactRms, ((alphaL + alphaR) / 2).normalize()).e, input.initialPosition);

    if (isOnTheLeft)
    {
        if (input.ship.searchVelocity > input.target.contactVelocity)
        {
            positions = positionsInSector(input, alphaL, alphaR, true);
        }
        else
        {
            positions = positionsInSector(input, alphaL, alphaR, false);
        }
    }
    else
    {
        if (input.ship.searchVelocity > input.target.contactVelocity)
        {
            positions = positionsInSector(input, alphaR, alphaL, true);
        }
        else
        {
            positions = positionsInSector(input, alphaR, alphaL, false);
        }
    }
    std::vector<double> vels{input.ship.maxVelocity, input.ship.searchVelocity, input.ship.searchVelocity};
    return Outputs::Route{positions, vels};
}

Outputs::SchemeEfficiency SectorSpiral::probability()
{
    std::cout << "probability SectorSpiral" << std::endl;
    return Outputs::SchemeEfficiency{};
}

} // namespace ByCall
} // namespace Search
} // namespace Schemes
