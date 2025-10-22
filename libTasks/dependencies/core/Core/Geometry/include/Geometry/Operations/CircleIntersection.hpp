#ifndef GEOMETRY_OPERATIONS_CIRCLE_INTERSECTION_HPP
#define GEOMETRY_OPERATIONS_CIRCLE_INTERSECTION_HPP
//==============================================================================
//! Point
#include <Point/Point2D.hpp>
//==============================================================================
namespace Geometry
{
//==============================================================================
  class Line;
  class BorderedLine;
  class Circle;
//==============================================================================
  /*!
   * \brief Поиск точки пересечения окружности и линии
   * \details Если пересечений не существует, вернутся пустые указатели.
   *          Если пересечений несколько, вернутся оба в порядке направления
   *          линии.
   * \param[in] сircle - окружность;
   * \param[in] line   - линия.
   * \return Точки пересечения.
   */
  std::pair<std::shared_ptr<Point2D>, std::shared_ptr<Point2D>>
    findLineAndCircleIntersection(
    const Circle& circle, const Line& line);
//==============================================================================
  /*!
   * \brief Поиск точки пересечения окружности и отрезка
   * \details Если пересечений не существует, вернутся пустые указатели.
   *          Если пересечений несколько, вернутся оба в порядке направления
   *          линии.
   * \param[in] сircle - окружность;
   * \param[in] line   - отрезок.
   * \return Точки пересечения.
   */
  std::pair<std::shared_ptr<Point2D>, std::shared_ptr<Point2D>>
    findBorderedLineAndCircleIntersection(
    const Circle& circle, const BorderedLine& line);
//==============================================================================
} //! Geometry
//==============================================================================
#endif //! GEOMETRY_OPERATIONS_CIRCLE_INTERSECTION_HPP
