#ifndef GEODESIC_OPERATIONS_ATPOLYGONS2D_HPP
#define GEODESIC_OPERATIONS_ATPOLYGONS2D_HPP
//==============================================================================
//! PrimaryEntities
#include <PrimaryEntities/GeodesicEntities.hpp>
//! Geometry
#include <Geometry/Operations/PointInPolygon.hpp>
//==============================================================================
namespace Geodesic
{
//==============================================================================
/*
* \brief Поиск в мультиполигоне полигона с наибольшей площадью
* \details Если площади некоторых полигонов окажутся равными,
*          возвращаем тот из них, который нашли первым
*
* \param[in]  multiPoly - мультиполигон
*
* \return Полигон с наибольшей площадью
*/
  GeoPolygon2D chooseBiggestPolygon(const GeoMultiPolygon2D& multiPoly);
//==============================================================================
} //! Geodesic
//==============================================================================
#endif //! GEODESIC_OPERATIONS_ATPOLYGONS2D_HPP