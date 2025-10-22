#ifndef GEODESIC_OPERATIONS_ALGORITHMS_WITHIN_HPP
#define GEODESIC_OPERATIONS_ALGORITHMS_WITHIN_HPP
//==============================================================================
#include <Geodesic/Projection/Transition.hpp>
#include <Geodesic/Projection/Orthographic/ProjectionBuilder.hpp>
#include <Geodesic/GeodesicException.hpp>
//! PrimaryEntities
#include <PrimaryEntities/AtEntitiesType.hpp>
//! AbstractOperations
#include <AbstractOperations/Algorithms/Within.hpp>
#include <AbstractOperations/Algorithms/CoveredBy.hpp>
#include <AbstractOperations/Algorithms/Buffer.hpp>

#include <IOTools/ShpIO/ShpIO.hpp>
//==============================================================================
namespace Geodesic
{
//==============================================================================
/*!
*\brief Проверка, входит ли одна геометрия в другую
*\details Решение осуществляется в проекции, оптимальной для
*         первой (основной) геометрии.
*         Для проверок "с запасом" и в принципе более стабильной работы со сложными
*         контурами допустимо увеличивать вторую геометрию на некоторый буфер.
*\param[in] mainGeom - основная, входящая геометрия (географические координаты)
*\param[in] minorGeom - вторая, объемлющая геометрия (географические координаты)
*\param[in] withBorders - считать ли точки на границе попавшими в область
*           true - точки на границе относятся к области 
*                  (в этом случае используется coveredBy)
*           false - точки на границе находятся за пределами области
*                   (в этом случае используется within) 
*\param[in] bufSize - размер буфера, на который увеличивается вторая (объемлющая)
*                     геометрия после проецирования [м]
*\return true - первая геометрия входит во вторую
*       false - первая геометрия не входит во вторую
* \throws GeodesicFailure # NegativeDistance
*/
template < class MainGeom, class MinorGeom>
bool withinInProj( const MainGeom& mainGeom, const MinorGeom& minorGeom, 
  const bool withBorders = true, const double bufSize = 0 )
{
  bool retVal;

  if( ( bufSize < 0 ) && ( !BasicMath::isNull( bufSize ) ) )
    throw GeodesicFailure( NegativeDistance );

  try
  {
    // ищем оптимальную для основной геометрии проекцию
    auto proj = findOptimalProjection( mainGeom );

    // проецируем обе геометрии в выбранную проекцию
    auto flatMain = geoGeometryToProjection( mainGeom, 
      std::make_shared<OrthographicProjection>( proj ), true );
    auto flatMinor = geoGeometryToProjection( minorGeom, 
      std::make_shared<OrthographicProjection>( proj ), true );

    if( ( ! BasicMath::isNull( bufSize ) )
      && ( PrimaryEntities::isEntityType1( minorGeom, PrimaryEntities::ArealEntity ) ) )
    {
      AbstractOperations::bufferSquare( flatMinor, bufSize );
    }

    if( withBorders )
      retVal = AbstractOperations::coveredBy( flatMain, flatMinor, true );
    else
      retVal = AbstractOperations::within( flatMain, flatMinor, true );
  }
  catch( ... )
  {
    ExceptionTools::FailureHandler<GeodesicFailure>
      ( "withinInProj" );
  }

  return retVal;
}
//==============================================================================
} // ! Geodesic
//==============================================================================
#endif // ! GEODESIC_OPERATIONS_ALGORITHMS_WITHIN_HPP
