#ifndef GEODESIC_OPERATIONS_ALGORITHMS_INTERSECTION_HPP
#define GEODESIC_OPERATIONS_ALGORITHMS_INTERSECTION_HPP
//==============================================================================
#include <Geodesic/Projection/Transition.hpp>
#include <Geodesic/Projection/Orthographic/ProjectionBuilder.hpp>
#include <Geodesic/GeodesicException.hpp>
//! AbstractOperations
#include <AbstractOperations/Algorithms/Intersection.hpp>
//==============================================================================
namespace Geodesic
{
//==============================================================================
/*!
*\brief Проверка, пересекаются ли две геометрии
*\details Поиск пересечений осуществляется в проекции, оптимальной для
*         первой (основной) геометрии.
*\param[in] mainGeom - основная геометрия (географические координаты)
*\param[in] minorGeom - вторая геометрия (географические координаты)
*\return true - геометрии пересекаются
*       false - пересечений не найдено
*/
template < class MainGeom, class MinorGeom>
bool hasIntersectionInProj( const MainGeom& mainGeom
  , const MinorGeom& minorGeom )
{
  bool retVal;

  try
  {
    // ищем оптимальную для основной геометрии проекцию
    auto proj = findOptimalProjection( mainGeom );

    // проецируем обе геометрии в выбранную проекцию
    auto flatMain = geoGeometryToProjection( mainGeom, 
      std::make_shared<OrthographicProjection>( proj ) );
    auto flatMinor = geoGeometryToProjection( minorGeom, 
      std::make_shared<OrthographicProjection>( proj ) );

    retVal = AbstractOperations::hasIntersection( flatMain, flatMinor, true );
  }
  catch( ... )
  {
    ExceptionTools::FailureHandler<GeodesicFailure>
      ( "hasInteresectionInProj" );
  }

  return retVal;
}
//==============================================================================
} // ! Geodesic
//==============================================================================
#endif // ! GEODESIC_OPERATIONS_ALGORITHMS_INTERSECTION_HPP
