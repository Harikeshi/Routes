#ifndef GEODESIC_OPERATIONS_DISTANCE_TO_LINESTRING_HPP
#define GEODESIC_OPERATIONS_DISTANCE_TO_LINESTRING_HPP
//==============================================================================
//! PrimaryEntities
#include <PrimaryEntities/GeodesicEntities.hpp>
//==============================================================================
namespace Geodesic
{
//==============================================================================
/*!
 * \brief distanceBetweenPointAndLinestring - поиск расстояния от точки до
 *                                            ломаной линии.
 *
 * \param[in] point      - точка, расстояние от которой ищется;
 *
 * \param[in] linestring - ломаная линия, до которой ищется расстояние,
 *
 * \param[in] useOrthodromy - использовать разбиение по ортодромии (если false
 *                            - по локсодромии),
 *
 * \param[in] useAllPointsForProj - использовать для поиска оптимальной
 *                                  проекции все точки (точки ломаной и точка,
 *                                  расстояние от которой ищется), если false
 *                                  - проекция ищется только по точке, от
 *                                  которой ищется расстояние.
 *
 * \return Расстояние от точки до ломаной [м].
 *
 * \throws GeodesicFailure # EmptyLinestring
 */
double distanceBetweenPointAndLinestring(
  const GeoPoint2D& point,
  const GeoLinestring2D& linestring,
  bool useOrthodromy, bool useAllPointsForProj = false);
//==============================================================================
} //! Geodesic
//==============================================================================
#endif //! GEODESIC_OPERATIONS_DISTANCE_TO_LINESTRING_HPP