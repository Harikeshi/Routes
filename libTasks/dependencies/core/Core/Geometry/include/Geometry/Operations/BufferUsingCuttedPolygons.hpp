#ifndef GEOMETRY_OPERATIONS_BUFFER_USING_CUTTED_POLYGONS_HPP
#define GEOMETRY_OPERATIONS_BUFFER_USING_CUTTED_POLYGONS_HPP
//==============================================================================
//! Geometry
#include <Geometry/Operations/CutPolygon.hpp>
//! AbstractOperations
#include <AbstractOperations/Algorithms/Area.hpp>
#include <AbstractOperations/Algorithms/Buffer.hpp>
#include <AbstractOperations/Algorithms/Correct.hpp>
//==============================================================================
namespace Geometry
{
//==============================================================================
  //TODO: Функция использовалась для исправления случаев, с которыми не
  //      справлялся boost. Необходимо исследовать, есть ли необходимость
  //      в этой функции при использовании GEOS.
  /*!
   * \brief Надувание полигона через его нарезку, надувание каждого его
   *        кусочка отдельно, с последующим их объединением
   * \param[in] polygon          - исходный мультиполигон, к которому
   *                               необходимо применить буфер
   * \param[in] distance         - размер буфера;
   * \param[in] quadrantSegments - количество сегментов для разбиения четверти
   *                               квадрата (для скругления буфера);
   * \param[in] endCapStyle      - тип формирования точек буфера на границах
   *                               линий;
   * \param[in] joinStyle        - тип формирования промежуточных точек линий;
   * \param[in] mitreLimit       - максимальное расстояние от промежуточных
   *                               вершин линии до границы буфера
   *                              (используется, чтобы не допустить вытягивания
   *                               острых углов при надувании геометрии,
   *                               слишком вытянутые углы будут срезаны);
   * \return Мультиполигон, соответсвующий входному, с применённым к нему
   *         буфером
   */
  //MultiPolygon2D getBufferUsingCuttedPolygons(
  //  const MultiPolygon2D& mpolygon,
  //  double distance, int quadrantSegments,
  //  geos::operation::buffer::BufferParameters::EndCapStyle endCapStyle,
  //  geos::operation::buffer::BufferParameters::JoinStyle joinStyle,
  //  double mitreLimit)
  //{
  //  // Результирующий полигон
  //  MultiPolygon2D result;

  //  // Нарезаем каждый полигон на 4 части
  //  MultiPolygon2D cuttedPolygon;
  //  for (const auto& polygon : mpolygon)
  //  {
  //    MultiPolygon2D curPolygons = cutPolygon(polygon, 2, 2);
  //    cuttedPolygon.insert(cuttedPolygon.end(),
  //      curPolygons.begin(), curPolygons.end());
  //  }

  //  for (const auto& poly : cuttedPolygon)
  //  {
  //    // Надуваем каждый из полученных кусочков полигона
  //    MultiPolygon2D polygonBuf;
  //    AbstractOperations::buffer(poly, polygonBuf,
  //      distance, quadrantSegments, endCapStyle,
  //      joinStyle, mitreLimit);

  //    // Если не получилось, и площадь нового полигона почему-то уменьшилась,
  //    // нарезаем снова
  //    if ((AbstractOperations::area(poly)
  //         > AbstractOperations::area(polygonBuf))
  //      || (!AbstractOperations::correct(polygonBuf)))
  //    {
  //      polygonBuf = getBufferUsingCuttedPolygons(
  //      { poly }, distance, quadrantSegments, endCapStyle,
  //        joinStyle, mitreLimit);
  //    }

  //    // Объединяем с уже посчитанными полигонами
  //    MultiPolygon2D tmpRes;
  //    AbstractOperations::unionOf(result, polygonBuf, tmpRes);

  //    // Если что-то пошло не так и результат объединения полигонов оказался
  //    // пустым, пробуем ещё раз, с большим размером буфера
  //    for (int coef = 2; tmpRes.empty(); ++coef)
  //    {
  //      double newDistance(distance * coef);

  //      polygonBuf = getBufferUsingCuttedPolygons(
  //      { poly }, newDistance, quadrantSegments, endCapStyle,
  //        joinStyle, mitreLimit);

  //      AbstractOperations::unionOf(result, polygonBuf, tmpRes);
  //    }

  //    result = tmpRes;
  //  }

  //  return result;
  //}
//==============================================================================
} //! Geometry
//==============================================================================
#endif //! GEOMETRY_OPERATIONS_BUFFER_USING_CUTTED_POLYGONS_HPP
