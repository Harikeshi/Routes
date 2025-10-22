#ifndef GEODESIC_OPERATIONS_CONVERSION_HPP
#define GEODESIC_OPERATIONS_CONVERSION_HPP
//==============================================================================
#include <Geodesic/GeodesicException.hpp>
//! PrimaryEntities
#include <PrimaryEntities/GeodesicEntities.hpp>
//==============================================================================
namespace Geodesic 
{
//==============================================================================
/*!
 *\brief Преобразовываем GeoBox2D в стороку для postgis
 *       "SRID=4326;POLYGON((min_lon min_lat,max_lon min_lat,max_lon max_lat,
 *                           min_lon max_lat,min_lon min_lat))"
 *       lat и lon записываются в радианах
 *\param[in] box - преобразовываемый GeoBox2D
 *\param[in] precision - количество значащих знаков после разделителя 
 *\return Текстовое представление бокса для postgis
 */
std::string boxToPostgisString(const GeoBox2D& box, const int precision = -1);
//==============================================================================
/*!
 *\brief Преобразовываем текстовое описание бокса в GeoBox2D
 *       "((max_lon, max_lat), (min_lon, min_lat))"
 *       lat и lon -  задаются в градусах
 *\param[in] str - текстовое описание бокса
 *\return GeoBox2D
 */
GeoBox2D boxFromStdString(const std::string& str);
//==============================================================================
} //! Geodesic
//==============================================================================
#endif //! GEODESIC_OPERATIONS_CONVERSION_HPP