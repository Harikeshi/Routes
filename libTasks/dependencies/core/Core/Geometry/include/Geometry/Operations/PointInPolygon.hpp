#ifndef GEOMETRY_OPERATIONS_POINT_IN_POLYGON_HPP
#define GEOMETRY_OPERATIONS_POINT_IN_POLYGON_HPP
//==============================================================================
//! PrimaryEntities
#include <PrimaryEntities/GeometryEntities.hpp>
//==============================================================================
namespace Geometry
{
//==============================================================================
/*!
* \brief findCentroid - поиск центра масс полигона.
* \details Используется функция geos, но, если не получилось
*          найти центр масс, выбрасывается исключение
*
* \param[in] polygon - полигон;
*
* \return Центр масс полигона.
*
* \throws GeometryFailure # EmptyPolygon
* \throws GeometryFailure # CentroidIsNotFound
*
*/
  Point2D findCentroid(const Polygon2D& polygon);
//==============================================================================
/*!
* \brief findNearestPoints - поиск точек полигона, ближайших к заданной.
* \details Если заданная точка находится не в одной из дыр полигона,
*          расстояние будет искаться до его внешней границы
*
* \param[in] polygon        - полигон, ближайшие точки которого ищутся;
*
* \param[in] point          - точка, расстояние от которой ищется;
*
* \param[out] nearestPoints - вектор ближайших точек полигона;
*
* \param[out] nearestDist   - расстояние до найденных точек;
*
*/
  void findNearestPoints(
    const Polygon2D& polygon,
    const Point2D& point,
    std::vector<Point2D>& nearestPoints,
    double& nearestDist);
//==============================================================================
/*!
 * \brief Проверка связности точек относительно мультиполигона.
 * \details Cчитаем, что полигоны мультиполигона между собой не пересекаются.
 * \param[in] firstPoint  - первая точка;
 * \param[in] secondPoint - вторая точка;
 * \param[in] mpolygon    - мультиполигон, по которому проверяется связность;
 * \param[in] withBorders - если true - попадание точки на границу
 *                          мультиполигона не будет нарушать связность,
 *                          иначе - будет.
 * \return - true - если точки связны, иначе false.
 * \throws NavigationAreaFailure # EmptyPolygon
 */
  bool checkPointsConnectivity(
    const Point2D& firstPoint, const Point2D& secondPoint,
    const MultiPolygon2D& mpolygon, bool withBorders);
//==============================================================================
} //! Geometry
//==============================================================================
#endif //! GEOMETRY_OPERATIONS_POINT_IN_POLYGON_HPP