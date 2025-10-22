#ifndef GEODESIC_DATUM_TRANSFORM_DATUM_TRANSFORM_HPP
#define GEODESIC_DATUM_TRANSFORM_DATUM_TRANSFORM_HPP
//==============================================================================
#include <Geodesic/DatumTransform/Constants.hpp> 
//! Point
#include <Point/GeoPoint2D.hpp>
#include <Point/GeoPoint3D.hpp>
//! std
#include <memory>
//==============================================================================
namespace Cartographic
{
  class Transformation;
  typedef std::shared_ptr<Cartographic::Transformation> TransformationPtr;
}
//==============================================================================
namespace Geodesic
{
/*!
 * \brief Перевод геодезических координат точек между системами координат.
 *        На текущий момент поддерживается переход между геодезическими 
 *        координатами в следующих системах координат: WGS 84, СК-42, ПЗ-90.02 и
 *        ПЗ-90.11.
*/
class DatumTransform
{
//==============================================================================
public:
//==============================================================================
  /*!
   * \brief Конструктор класса
   * \param[in] initSystem - исходная СК (из какой переводить)
   * \param[in] goalSystem - целевая СК (в какую переводить)
  */
  DatumTransform(CoordinateSystems initSystem, CoordinateSystems goalSystem);
//==============================================================================
  /*!
   * \brief Получение исходной СК преобразователя
   * \return Исходная СК (из какой переводить)
  */
  inline CoordinateSystems getInitSystems() const
  {
    return mInitSystem;
  }
//==============================================================================
  /*!
   * \brief Получение целевой СК преобразователя
   * \return Целевая СК (в какую переводить)
  */
  inline CoordinateSystems getGoalSystems() const
  {
    return mGoalSystem;
  }
//==============================================================================
  /*!
   * \brief Смена СК для перевода координат
   * \param[in] initSystem - исходная СК (из какой переводить)
   * \param[in] goalSystem - целевая СК (в какую переводить)
  */  
  void setSystems(CoordinateSystems initSystem, CoordinateSystems goalSystem);
//==============================================================================
  /*!
   * \brief Поменять направление перевода 
   *        (поменять исходную и целевую СК местами)
  */
  void swapSystems();
//==============================================================================
  /*!
   * \brief Преобразование координат точки 2D между СК
   * \param[in] point - точка, координаты которой преобразуем
   * \return Точка с преобразованными координатам
  */
  GeoPoint2D transform(const GeoPoint2D& point) const;
//==============================================================================
  /*!
   * \brief Преобразование координат точки 3D между СК
   * \param[in] point - точка, координаты которой преобразуем
   * \return Точка с преобразованными координатам
  */
  GeoPoint3D transform(const GeoPoint3D& point) const;
//==============================================================================
  /*!
   * \brief Преобразование координат точки между СК через оператор ()
   * \param[in|out] point - точка, координаты которой преобразуем
  */
  void operator () (GeoPoint3D& point) const;
//==============================================================================
  /*!
   * \brief Преобразование координат точки между СК через оператор ()
   * \param[in|out] point - точка, координаты которой преобразуем  
  */
  void operator () (GeoPoint2D& point) const;
//==============================================================================
private:
//==============================================================================
  /*!
   * \brief Создание преобразователя СК
   * \throws GeodesicFailure # UnknownCoordinateSystem
   */ 
  void createTransformation();
//==============================================================================
  /*!
   * \brief Преобразование координат точки между СК
   * \param[in|out] point - точка, координаты которой преобразуем  
  */
  void tramsformInner(GeoPoint3D& point) const;
//==============================================================================
  CoordinateSystems mInitSystem;                    //!< Исходная СК
  CoordinateSystems mGoalSystem;                    //!< Целевая СК
  Cartographic::TransformationPtr mTransformation;  //!< Преобразователь СК
//==============================================================================
};
//==============================================================================
} //! Geodesic
//==============================================================================
#endif //! GEODESIC_DATUM_TRANSFORM_DATUM_TRANSFORM_HPP