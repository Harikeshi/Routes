#ifndef GEODESIC_COMPLICATED_OPERATIONS_SECTORSCALCULATOR_HPP
#define GEODESIC_COMPLICATED_OPERATIONS_SECTORSCALCULATOR_HPP
//==============================================================================
#include <Geodesic/Projection/Orthographic/Orthographic.hpp>
//! PrimaryEntities
#include <PrimaryEntities/GeodesicEntities.hpp>
//! Geometry
#include <Geometry/Objects/Sector.hpp>
//==============================================================================
namespace Geodesic
{
//==============================================================================
/*!
* \brief getSectors Формирование секторов для полигонов
* \param[in] geoPolygons    - полигоны
* \param[in] center         - центр окружности, ограничивающей участок
* \param[in] radius         - радиус окружности, ограничивающей участок
* \param[in] projection     - проекция
* \param[in] startAzimuth   - положение начала отсчёта угла (задаётся
* \                           углом от направления севера по часовой
* \                           стрелке)
* \param[in] unionSectors   - если true сектора при пересечении
* \                           объединяются, если false - каждому полигону
* \                           соответствует свой сектор
* \return - сектора для полигонов
*/
  std::vector<Geometry::Sector> getSectors(
    const GeoMultiPolygon2D& geoPolygons,
    const Point2D& center,
    const double radius,
    const Geodesic::OrthographicProjection& projection,
    const Radian startAzimuth,
    const bool unionSectors = false
    );
//==============================================================================
} //! Geodesic
//==============================================================================
#endif //! GEODESIC_COMPLICATED_OPERATIONS_SECTORSCALCULATOR_HPP