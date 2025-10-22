#ifndef GEODESIC_PROJECTION_ABSTRACT_PROJECTION_HPP
#define GEODESIC_PROJECTION_ABSTRACT_PROJECTION_HPP
//==============================================================================
//! Helper
#include <Geodesic/Helper/Ellipsoid.hpp>
//! Point
#include <Point/Point2D.hpp>
#include <Point/Point3D.hpp>
#include <Point/GeoPoint2D.hpp>
#include <Point/GeoPoint3D.hpp>
//! std
#include <memory>
//==============================================================================
namespace Geodesic
{
/*!
 * \brief Абстрактный класс с базовыми интерфейсами проекции
*/
class AbstractProjection
{
//==============================================================================
public:
//==============================================================================
  AbstractProjection( 
    const Geodesic::Ellipsoid& ellipsoid = Geodesic::EL_WGS_84 );
//==============================================================================
  /*!
   * \brief Перевод точки в проекцию (из геодезических в декартовы координаты)
   * \details Для 3D
   * \param[in] geoPoint - геодезические координаты точки
   * \return Декартовы координаты точки в проекции
  */
  virtual
  Point3D toProjection(const GeoPoint3D& geoPoint) const = 0;
//==============================================================================
  /*!
   * \brief Перевод точки в проекцию (из геодезических в декартовы координаты)
   * \details Для 2D
   * \param[in] geoPoint - геодезические координаты точки
   * \return Декартовы координаты точки в проекции
  */
  virtual
  Point2D toProjection(const GeoPoint2D& geoPoint) const = 0;
//==============================================================================
  /*!
   * \brief Перевод точки из проекции (из декартовых в геодезические координаты)
   * \param[in] point - декартовы координаты точки
   * \return Геодезические координаты точки, соответствующие видимой стороне 
   *         проекции
   * \throws GeodesicFailure # PointOutOfProjection
   */
  virtual
  GeoPoint3D fromProjection(const Point3D& point) const = 0;
//==============================================================================
  /*!
   * \brief Перевод точки из проекции (из декартовых в геодезические координаты)
   * \param[in] point - декартовы координаты точки
   * \return Геодезические координаты точки, соответствующие видимой стороне 
   *         проекции
   * \throws GeodesicFailure # PointOutOfProjection
   */
  virtual
  GeoPoint2D fromProjection(const Point2D& point) const = 0;
//==============================================================================
protected:
  Geodesic::Ellipsoid mEllipsoid;
//==============================================================================
};
//==============================================================================
} //! Geodesic
//==============================================================================
#endif //! GEODESIC_PROJECTION_ABSTRACT_PROJECTION_HPP