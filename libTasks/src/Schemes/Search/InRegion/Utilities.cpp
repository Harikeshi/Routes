#include <Task/Schemes/Search/InRegion/Utilities.hpp>

#include <Task/Schemes/Search/Config.hpp>

namespace Schemes {
namespace Search {
namespace InRegion {

std::vector<double> generateConstArray(size_t size)
{
    std::vector<double> res(size);
    std::random_device rd;
    std::mt19937 gen(rd());
    gen.seed(0);
    std::uniform_real_distribution<> u01;
    for (size_t i = 0; i < size; ++i)
    {
        res[i] = u01(gen);
    }
    return res;
}

//Radian generateTurnAngle(std::uniform_real_distribution<>& u01)
//{
//    std::random_device rd;
//    std::mt19937 gen(rd());

//    double rand01 = u01(gen);
//    return Radian(Degree((1 - rand01) * TURN_ANGLE_MIN_DEG + rand01 * TURN_ANGLE_MAX_DEG));
//}

/*
double generateTackDistance(const double tackMax, const double detectionRange, std::uniform_real_distribution<>& u01)
{
    if (tackMax <= detectionRange)
    {
        return tackMax;
    }
    else
    {
        std::random_device rd;
        std::mt19937 gen(rd());

        double rand01 = u01(gen);
        return detectionRange + (tackMax - detectionRange) * rand01;
    }
}

double generateTackDistance(const double tackMax, const double detectionRange, std::piecewise_linear_distribution<>& p01)
{
    if (tackMax <= detectionRange)
    {
        return tackMax;
    }
    else
    {
        std::random_device rd;
        std::mt19937 gen(rd());

        double rand01 = p01(gen);
        return detectionRange + (tackMax - detectionRange) * rand01;
    }
}
*/

} // namespace InRegion
} // namespace Search
} // namespace Schemes
