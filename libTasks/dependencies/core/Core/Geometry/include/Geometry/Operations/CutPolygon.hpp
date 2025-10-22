#ifndef GEOMETRY_OPERATIONS_CUT_POLYGON_HPP
#define GEOMETRY_OPERATIONS_CUT_POLYGON_HPP
//==============================================================================
//! PrimaryEntities
#include <PrimaryEntities/GeometryEntities.hpp>
//==============================================================================
namespace Geometry
{
//==============================================================================
  /*!
   * \brief Нарезка полигона на заданное количество частей по горизонтали
   *        и по вертикали
   * \details Не работает с некорректными геометриями
   * \param[in] polygon - исходный полигон, который необходимо нарезать
   * \param[in] stepNumberX - количество шагов вдоль оси X
   * \param[in] stepNumberY - количество шагов вдоль оси Y
   * \return Мультиполигон, соответсвующий входному, нарезанному по своей
   * коробке сеткой из stepNumberX * stepNumberY частей
   */
  MultiPolygon2D cutPolygon(
    const Polygon2D& polygon, size_t stepNumberX, size_t stepNumberY);
//==============================================================================
} //! Geometry
//==============================================================================
#endif //! GEOMETRY_OPERATIONS_CUT_POLYGON_HPP
