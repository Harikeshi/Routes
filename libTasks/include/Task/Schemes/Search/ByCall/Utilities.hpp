#pragma once

#include <Task/Schemes/Search/ByCall/Input.hpp>

/*! std */
#include <string>
#include <vector>

namespace Schemes {
namespace Search {
namespace ByCall {

inline double calculateClosingSpeed(const double searchVel, const double targetVel, const Radian& betaAngle)
{
    // скорость сближения поисковика и цели
    /// @todo throw
    return targetVel * std::cos(betaAngle.get()) + std::sqrt(searchVel * searchVel - targetVel * targetVel * std::sin(betaAngle.get()) * std::sin(betaAngle.get()));
}

inline double calculateOrderAngle(const double searchVel, const double targetVel, const double semiWidth, const double distanceX)
{
    ///@todo tests!
    // расчет угла поворота левого поиска при перестроении
    if (distanceX == 0 || searchVel == 0)
    {
        throw std::invalid_argument("Деление на ноль (calculateOrderAngle)");
    }

    const double alpha1 = std::atan(semiWidth / distanceX);                      // угол между направлением вектора скорости сближения и средним направлением
    const double alpha2 = std::asin((targetVel / searchVel) * std::sin(alpha1)); // угол между направлениями поисковика и сближения, используется теорема синусов

    return alpha1 + alpha2;
}

Point2D positionInSector(const Point2D& lCurr, const Point2D& mCurr, const Radian& alpha, const double& vFinder, const double& vL);

std::vector<Point2D> positionsInSector(const Input& input, const Radian& alphaL, const Radian& alphaR, bool isStandart);

} // namespace ByCall
} // namespace Search
} // namespace Schemes
