#include <Task/Schemes/Search/AtBorder/Utilities.hpp>

namespace Schemes {
namespace Search {
namespace AtBorder {

double calculateAParameter(double mParameter)
{
    if (mParameter < 1)
    {
        throw std::invalid_argument("Отрицательное значение под корнем");
    }

    return mParameter * std::sqrt((mParameter - 1) / (mParameter + 1));
}

PatrollingParameters relativePatrollingParameters(double mParameter, const std::string& flag)
{
    PatrollingParameters relativeParameters;
    if (flag == "linear")
    {
        relativeParameters.patrollingLength = (mParameter * mParameter) / std::sqrt(4 + mParameter * mParameter);
        relativeParameters.lengthDifference = 4 / std::sqrt(4 + mParameter * mParameter);
    }
    else if (flag == "cross")
    {
        double aParameter = calculateAParameter(mParameter);
        relativeParameters.patrollingLength = 2 * aParameter * aParameter / std::sqrt(1 + aParameter * aParameter);
        relativeParameters.lengthDifference = 1 / std::sqrt(1 + aParameter * aParameter);
    }
    return relativeParameters;
}

PatrollingParameters patrollingParameters(const double shipSearchVel, const double targetVel, const double detectionDist, const double circulationRadius, const std::string& flag)
{
    double delta = 2 * M_PI * circulationRadius - 4 * circulationRadius;
    double m = shipSearchVel / targetVel;

    PatrollingParameters relativeParameters = relativePatrollingParameters(m, flag);

    PatrollingParameters out;
    if (flag == "linear")
    {
        out = {(detectionDist * relativeParameters.patrollingLength) - 2 * delta, (detectionDist * relativeParameters.lengthDifference) - 2 * delta};
    }
    else
        out = {detectionDist * relativeParameters.patrollingLength, detectionDist * relativeParameters.lengthDifference};

    return out;
}

BorderedLine positionsAtBorder(const BorderedLine& border, double lengthDifference)
{
    const Point2D A = border.findPoint(border.getStart(), lengthDifference, false, true);
    const Point2D B = border.findPoint(border.getFinish(), lengthDifference, false, false);

    return BorderedLine{A, B};
}
} // namespace AtBorder
} // namespace Search
} // namespace Schemes
