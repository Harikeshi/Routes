#include <Task/Outputs/Route.hpp>

#include <Task/Operations/JsonOperations.hpp>
//#include <Task/Operations/UnitConverter.hpp>

namespace Outputs {

nlohmann::json Route::toJson() const
{
    nlohmann::json result;

    auto p = points;
    auto v = velocities;

    //    Operations::nauticalToMetricOutput(p, v);

    result["points"] = Operations::getJson(p);
    result["velocities"] = Operations::getJson(v);

    return result;
}

void Route::validate()
{
    if (points.size() < 2)
    {
        throw std::runtime_error("Меньше 2 точек маршрута");
    }
    if (velocities.size() != points.size())
    {
        throw std::runtime_error("Несоответствие количества отрезков и скоростей");
    }
    for (auto const& velocity : velocities)
    {
        if (std::abs(velocity) < ACCURACY_DOUBLE)
        {
            throw std::runtime_error("Нулевая скорость");
        }
    }
}

double Route::duration() const
{
    if (points.size() < 2 || velocities.size() != points.size() - 1)
        throw std::runtime_error("Некорректный маршрут");

    double duration = 0;
    for (size_t i = 0; i < velocities.size(); ++i)
    {
        if (velocities.at(i) != 0)
        {
            duration += Geometry::distance(points.at(i), points.at(i + 1)) / velocities.at(i);
        }
        else
        {
            throw std::runtime_error("Скорость равна нулю");
        }
    }
    return duration;
}

bool Route::checkTime(double setTime, double timeAccuracy) const
{
    return std::fabs(duration() - setTime) < timeAccuracy;
}

double Route::length() const
{
    double dist = 0;
    for (size_t i = 0; i < points.size() - 1; i++)
    {
        dist += Geometry::distance(points[i], points[i + 1]);
    }
    return dist;
}
} // namespace Outputs
