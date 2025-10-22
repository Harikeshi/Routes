#ifndef GEOMETRY_OPERATIONS_AT_POINTS2D_HPP
#define GEOMETRY_OPERATIONS_AT_POINTS2D_HPP
//==============================================================================
#include <Point/Point2D.hpp>
//! MathTools
#include <MathTools/AngularUnits/Radian.hpp>
//==============================================================================
namespace Geometry
{
//==============================================================================
/*!
 *\brief Нахождение расстояния между точками
 * \param[in] point1 - первая точка
 * \param[in] point2 - вторая точка
 * \return Расстояние между двумя точками
*/
inline double distance(const Point2D& point1, const Point2D& point2)
{
  return sqrt(pow(point1.getX() - point2.getX(), 2.) 
    + pow(point1.getY() - point2.getY(), 2.));
}
//==============================================================================
/*!
 * \brief Нахождение косинуса угла между векторами, построенными по точкам
 * \details Вектора идут от общей точки до первой и второй   
 * \param[in] point1   - первая точка
 * \param[in] ourPoint - общая точка
 * \param[in] point2   - вторая точка
 * \return Косинус угла.
*/
double cosAngle(
  const Point2D& point1, const Point2D& ourPoint, const Point2D& point2);
//==============================================================================
/*!
* \brief Нахождение угла наклона прямой
* \details Прямая, образованная двумя точками, имеет направление
* \        от point1 до point2
* \param[in] point1 - первая точка
* \param[in] point2 - вторая точка
* \return угол наклона прямой (в полуинтервале [0, 2π)).
*/
Radian lineAngularity(const Point2D& point1, const Point2D& point2);
//==============================================================================
/*!
 * \brief middle -- функция, возвращающая для двух точек на плоскости точку,
 * располагающуюся на середине отрезка проведённого между первыми двумя.
 * \param[in] a -- первая точка на плоскости.
 * \param[in] b -- вторая точка на плоскости.
 * \return -- искомая "середина".
 */
Point2D middle(const Point2D& a, const Point2D& b) noexcept;
//==============================================================================
/*!
* \brief Нахождение угла между векторами, построенными по точкам
* \details Вектора идут от общей точки до первой и второй. Диапазон 
* \        угла [0, 2 * Pi) в направлении по/против часовой стрелки
* \        от первого вектора.
* \param[in] point1      - первая точка
* \param[in] ourPoint    - общая точка
* \param[in] point2      - вторая точка
* \param[in] isClockwise - если true - угол ищется в направлении по
* \                        часовой стрелке, иначе - против
* \return Угол.
*/
Radian findAngle(
  const Point2D& point1, const Point2D& ourPoint, const Point2D& point2,
  bool isClockwise = false);
//==============================================================================
/*!
* \brief Поиск точки на прямой
* \details Поиск координат точки, находящейся на прямой на заданном
* \  расстоянии от второй точки отрезка вне(на) отрезка
* \param[in] pointFirst  - начальная точка прямой;
* \param[in] pointSecond - конечная точка прямой;
* \param[in] dist        - расстояние удаления искомой точки от второй
* \                        точки отрезка;
* \param[in] atLine      - если true - поиск точки по направлению 
* \                        внутрь отрезка, иначе - от него;
* \return найденная точка.
*/
Point2D findPointAtLine(
  const Point2D& pointFirst,
  const Point2D& pointSecond,
  double dist, bool atLine);
//==============================================================================
} //! Geometry
//==============================================================================
#endif //! GEOMETRY_OPERATIONS_AT_POINTS2D_HPP