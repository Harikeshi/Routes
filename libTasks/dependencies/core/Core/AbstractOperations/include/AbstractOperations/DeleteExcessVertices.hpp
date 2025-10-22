#ifndef ABSTRACT_OPERATIONS_DELETE_EXCESS_VERTICES_HPP
#define ABSTRACT_OPERATIONS_DELETE_EXCESS_VERTICES_HPP
//==============================================================================
#include <AbstractOperations/AtPoints2D.hpp>
#include <AbstractOperations/Algorithms/Distance.hpp>
#include <AbstractOperations/LinesIntersectionTools.hpp>

#include <PrimaryEntities/GeometryConverter.hpp>
#include <PrimaryEntities/Entities/Polygon.hpp>
#include <PrimaryEntities/Entities/MultiPolygon.hpp>
#include <PrimaryEntities/Entities/MultiLinestring.hpp>
//==============================================================================
namespace AbstractOperations
{
//==============================================================================
/*!
 * \brief deleteExcessVertices - функция,
 * удаляющая лишние вершины в кольце многоугольника.
 * \details Удаляет дублирующиеся точки, выбросы и точки, лежащие
 *          на границе (т.е. на выходе не более двух точек подряд
 *          на одной прямой).
 *          Также функция по желанию может изменить порядок следования точек
 *          в кольце многоугольника на обратный
 *          вне зависимости от исходной направленности.
 *          На выходе получается кольцо,
 *          последняя вершина которого совпадает с первой.
 *          Кольцо вырождается (очищается),
 *          если исходное или результирующее кольцо
 *          содержат менее трёх вершин.
 * \param[in|out] ring - кольцо (массив вершин);
 * \param[in] isNeedToReverse - нужно ли изменять
 * порядок следования вершин в кольце на обратный.
 * \return true  - кольцо не выродилось после коррекции;
 *         false - кольцо выродилось после коррекции.
 */
template <typename PointType>
bool deleteExcessVertices(
  std::vector<PointType>& ring,
  bool isNeedToReverse = false) noexcept
{
  using namespace PrimaryEntities;

  if (ring.size() < 2)
  {
    ring.clear();
    return false;
  }

  // Если в кольце первая и последняя вершина равны,
  // это будет нам мешать, так что пока удалим последнюю
  if (ring.at(0) == ring.at(ring.size() - 1))
    ring.pop_back();

  if (ring.size() < 3)
  {
    ring.clear();
    return false;
  }

  // Проверим первую точку
  std::vector<PointType> newRing;
  const std::size_t oldSize(ring.size());
  newRing.reserve(oldSize);

  std::size_t i(0);

  // Проверяем, что в кольце есть хотя бы одна невырожденная точка, относительно
  // последней точки
  PointType point1(ring.at(oldSize - 1));
  PointType point2;
  PointType point3;
  while (newRing.empty() && (i + 2) < oldSize)
  {
    point2 = ring.at(i);
    point3 = ring.at(i + 1);
    if (isNonDegenerateTriangle(point1, point2, point3))
      newRing.emplace_back(point2);
    else if (
      fabs(distanceBetweenGeometries(point1, point3)
         - distanceBetweenGeometries(point1, point2)
         - distanceBetweenGeometries(point2, point3)) > ACCURACY_DOUBLE)
      newRing.emplace_back(point2);
    ++i;
  }

  // Если всё ещё не нашли ни одного невырожденного треугольника, уже и не
  // найдём
  if (newRing.empty())
  {
    ring.clear();
    return false;
  }

  // Если в новом кольце есть вершины, некоторые случаи вырожденности уже
  // в нём могут быть решены, так что предыдущие вершины берём из нового кольца
  for (; (i + 1) < oldSize; ++i)
  {
    point1 = newRing.at(newRing.size() - 1);
    point2 = ring.at(i);
    point3 = ring.at(i + 1);
    if (isNonDegenerateTriangle(point1, point2, point3))
      newRing.emplace_back(point2);
    else if (
      fabs(distanceBetweenGeometries(point1, point3)
         - distanceBetweenGeometries(point1, point2)
         - distanceBetweenGeometries(point2, point3)) > ACCURACY_DOUBLE)
      newRing.emplace_back(point2);
  }

  // Проверим последнюю точку
  point1 = newRing.at(newRing.size() - 1);
  point2 = ring.at(oldSize - 1);
  point3 = newRing.at(0);
  if (isNonDegenerateTriangle(point1, point2, point3))
    newRing.emplace_back(point2);
  else if (
    fabs(distanceBetweenGeometries(point1, point3)
       - distanceBetweenGeometries(point1, point2)
       - distanceBetweenGeometries(point2, point3)) > ACCURACY_DOUBLE)
    newRing.emplace_back(point2);


  // Проверим, что кольцо не выродилось в результате коррекции
  if (newRing.size() < 3)
  {
    ring.clear();
    return false;
  }

  if (isNeedToReverse)
  {
    ring.clear();
    ring.reserve(newRing.size());
    for (int j(int(newRing.size()) - 1); j >= 0; --j)
      ring.emplace_back(newRing.at(j));
  }
  else
    ring.swap(newRing);

  //! Добавляем замыкающую вершину.
  ring.push_back(ring.at(0));

  return true;
}
//==============================================================================
/*!
 * \brief deleteExcessVertices - функция,
 * удаляющая лишние вершины в многоугольнике.
 * \details Удаляет дублирующиеся точки, выбросы и точки, лежащие
 *          на границе (т.е. на выходе не более двух точек подряд
 *          на одной прямой).
 *          Кольцо многоугольника вырождается
 *          (очищается, если внешнее, удаляется, если внутреннее),
 *          если исходное или результирующее кольцо
 *          содержат менее трёх вершин.
 * \param[in|out] polygon - многоугольник;
 * \return true  - ни одно кольцо не выродилось после коррекции;
 *         false - некоторое кольцо выродилось после коррекции.
 */
template <typename PointType>
bool deleteExcessVertices(PrimaryEntities::Polygon<PointType>& polygon)
{
  if (!deleteExcessVertices(polygon.outer(), false))
  {
    polygon.clear();
    return false;
  }

  bool result(true);
  for (auto it(polygon.inners().begin()); it != polygon.inners().end();)
  {
    if (!deleteExcessVertices(*it, false))
    {
      it = polygon.inners().erase(it);
      result = false;   
    }
    else
      ++it;
  }
  return result;
}
//==============================================================================
/*!
 * \brief deleteExcessVertices - функция,
 * удаляющая лишние вершины в множестве многоугольников.
 * \details Удаляет дублирующиеся точки, выбросы и точки, лежащие
 *          на границе (т.е. на выходе не более двух точек подряд
 *          на одной прямой).
 *          Кольцо многоугольника вырождается
 *          (очищается, если внешнее, удаляется, если внутреннее),
 *          если исходное или результирующее кольцо
 *          содержат менее трёх вершин.
 * \param[in|out] multiPolygon - множество многоугольников;
 * \return true  - ни одно кольцо ни одного многоугольника
 * не выродилось после коррекции;
 *         false - некоторое кольцо некоторого многоугольника
 * выродилось после коррекции.
 */
template <typename PointType>
bool
deleteExcessVertices(
  PrimaryEntities::MultiPolygon<PointType>& multiPolygon)
{
  bool result(true);
  for (int i = multiPolygon.size() - 1; i >= 0; --i)
  {
    if (!deleteExcessVertices(multiPolygon.at(i)))
    {
      if (multiPolygon.at(i).outer().empty())
        multiPolygon.erase(multiPolygon.begin() + i);
      result = false;
    }
  }
  return result;
}
//==============================================================================
} //! AbstractOperations
//==============================================================================
#endif //! ABSTRACT_OPERATIONS_DELETE_EXCESS_VERTICES_HPP