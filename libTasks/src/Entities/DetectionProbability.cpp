#include <Task/Entities/DetectionProbability.hpp>

// Core
#include <AbstractOperations/Algorithms/Area.hpp>

#include <cmath>

double Entities::DetectionProbability::calculateProductivity()
{
    /// @todo
    return 0;
}

double Entities::DetectionProbability::calculate()
{
    const double& area = AbstractOperations::area(region.getPolygon(), true, true);
    const double detectionProbability = 1 - std::exp(-productivity / area);
    return detectionProbability;
}
