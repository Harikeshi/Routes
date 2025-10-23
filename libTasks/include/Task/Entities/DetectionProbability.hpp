#pragma once

// Task
#include <Task/Entities/SearchRegion.hpp>
#include <Task/Outputs/Route.hpp>

namespace Entities {

class DetectionProbability
{
protected:
    SearchRegion region;
    double detectionRange;
    Outputs::Route route;
    double stepLength;
    double productivity;

public:
    DetectionProbability(const SearchRegion& reg, const double& dr, const Outputs::Route ro, const double& sl)
        : region(reg), detectionRange(dr), route(ro), stepLength(sl){};
    double calculateProductivity();
    double calculate();
};

} // namespace Entities
