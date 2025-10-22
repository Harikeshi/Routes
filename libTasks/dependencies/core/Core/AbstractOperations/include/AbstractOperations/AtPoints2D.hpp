#ifndef ABSTRACT_OPERATIONS_AT_POINTS2D_HPP
#define ABSTRACT_OPERATIONS_AT_POINTS2D_HPP
//==============================================================================
//! Point
#include <Point/Point2D.hpp>
//! MathTools
#include <MathTools/AngularUnits/Radian.hpp>
//==============================================================================
namespace AbstractOperations
{
//==============================================================================
/*!
 * \brief Проверка, что три точки образуют треугольник с ненулевой площадью.
 * \param[in] point1 - первая точка;
 * \param[in] point2 - вторая точка;
 * \param[in] point3 - третья точка;
 * \return true  - точки образуют треугольник с ненулевой площадью;
 *         false - не образуют.
 */
template <typename PointType>
bool
isNonDegenerateTriangle(
  const PointType& point1,
  const PointType& point2,
  const PointType& point3) noexcept
{
  double firstSide
    = point1.getCoordinates().distance(point2.getCoordinates());
  if (BasicMath::isNull(firstSide))
    return false;

  double secondSide
    = point1.getCoordinates().distance(point3.getCoordinates());
  if (BasicMath::isNull(secondSide))
    return false;

  double thirdSide
    = point2.getCoordinates().distance(point3.getCoordinates());
  if (BasicMath::isNull(thirdSide))
    return false;

  if (BasicMath::isNull(firstSide + secondSide - thirdSide)
    || BasicMath::isNull(firstSide + thirdSide - secondSide)
    || BasicMath::isNull(secondSide + thirdSide - firstSide))
    return false;

  return true;
}
//==============================================================================
} //! AbstractOperations
//==============================================================================
#endif //! ABSTRACT_OPERATIONS_AT_POINTS2D_HPP