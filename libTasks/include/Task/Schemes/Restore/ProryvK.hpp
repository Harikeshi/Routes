#pragma once

#include <Task/Abstractions/Scheme.hpp>
#include <Task/Schemes/Restore/Input.hpp>

namespace Schemes {
namespace Restore {
class ProryvK : public Abstractions::Scheme
{
    const Input& input;

public:
    explicit ProryvK(const Input& input);

    Outputs::Route calculate() override;
    /*
    {
        // Вызвать Proryv_k
        auto waypoints = ::Restore::Proryv_k(input.parameters, false);
        // Перевести результат в Output::Route

        Outputs::Route route;

        for (const auto& waypoint : waypoints.wpList)
        {
            route.points.push_back(waypoint.pnt);
        }

        for (size_t i = 0; i != waypoints.wpList.size() - 1; ++i)
        {
            route.velocities.push_back(waypoints.wpList[i].vel);
        }

        return route;
    }
    */
    ~ProryvK() = default;

    Outputs::SchemeEfficiency probability() override
    {
        return Outputs::SchemeEfficiency{};
    }
};
} // namespace Restore
} // namespace Schemes
