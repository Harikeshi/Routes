#ifndef GEODESIC_GEOLINETOOLS_LOXODROME_TOOLS_HPP
#define GEODESIC_GEOLINETOOLS_LOXODROME_TOOLS_HPP
//==============================================================================
//! Helpers
#include <Geodesic/Helper/Ellipsoid.hpp>
#include <Geodesic/GeoLineTools/Helper/Loxodrome.hpp>
//! Inner
#include <Geodesic/GeoLineTools/Inner/GeodesicLineTools.hpp>
//! std
#include <memory>
//==============================================================================
namespace GeographicLib
{
  class Rhumb;
}
//==============================================================================
namespace Geodesic
{
//==============================================================================
/*!
* \brief Набор действий/операций над локсодромией
*/
class LoxodromeTools : public Inner::GeodesicLineTools
{
//==============================================================================
public:
//==============================================================================
  /*!
   * \brief Конструктор класса
   * \param[in] ellipsoid - эллипсоид, на котором производятся вычисления
  */
  LoxodromeTools(const Ellipsoid& ellipsoid = Ellipsoid(EL_WGS_84));
//==============================================================================
  /*!
   * \brief Создание локсодромии по двум точкам
   * \details ЛОКСОДРОМИЯ НЕ ПРОЙДЕТ ЧЕРЕЗ ПОЛЮС!  
   *          Долготы точек задания локсодромии будут приведены к диапазону 
   *          [-Pi; Pi].
   *          Широта точек должна быть в диапазоне [-Pi / 2; Pi / 2];
   * \param[in] startPoint - точка начала локсодромии
   * \param[in] finishPoint - точка завершения локсодромии
   * \return Локсодромия
   * \throws GeodesicFailure # LoxodromeThroughOnePoint
   */
  Loxodrome createLoxodrome(const GeoPoint2D& startPoint, 
                            const GeoPoint2D& finishPoint) const;
//==============================================================================
  /*!
   * \brief Создание локсодромии заданной длины от точки в заданном направлении 
   * \details ЛОКСОДРОМИЯ НЕ МОЖЕТ ПРОЙТИ ЧЕРЕЗ ПОЛЮС! 
   *          Долготы точек задания локсодромии будут приведены к диапазону 
   *          [-Pi; Pi].
   *          Широта точек должна быть в диапазоне [-Pi / 2; Pi / 2];
   *          Если пересечёт полюс, будет вызвано соответствующее исключение.
   *          Но исключение не выбросится,
   *          если точка отсчёта - полюс и азимут направлен от него.
   * \param[in] startPoint - точка начала локсодромии
   * \param[in] azimuth - азимут [радианы]
   * \param[in] length - длина [м] (может равняться 0, если нужно задать 
   *                                локсодромию как луч)
   * \return Локсодромия
   * \throws GeodesicFailure # LoxodromeThroughPole
   */
  Loxodrome createLoxodrome(const GeoPoint2D& startPoint, const Radian& azimuth,
                            double length = 50000.) const;
//==============================================================================
  /*!
   * \brief Поиск расстояния между двумя точками по локсодромии. 
   * \details ЛОКСОДРОМИЯ НЕ ПРОЙДЕТ ЧЕРЕЗ ПОЛЮС!      
   * \param[in] startPoint - точка начала отсчёта
   * \param[in] finishPoint - точка завершения отсчёта
   * \return Расстояние между точками по локсодромии [м] 
  */
  double distance(const GeoPoint2D& startPoint, 
                  const GeoPoint2D& finishPoint) const override;
//==============================================================================
  /*!
   * \brief Поиск координат точки, отстоящей от заданной точки на заданное 
   *        расстояние в заданном направлении по локсодромии. 
   * \details ЛОКСОДРОМИЯ НЕ МОЖЕТ ПРОЙТИ ЧЕРЕЗ ПОЛЮС! 
   *          Если зацепит полюс, то долгота искомой точки будет равна NAN.
   *          Исключение: точка отсчёта - полюс и азимут направлен от него
   *                      (абсолютное значение азимута > Pi / 2)
   * \param[in] point - точка отсчёта
   * \param[in] azimuth - азимут [радианы]
   * \param[in] distance - расстояние от точки отсчёта [м] 
   * \return Координаты точки
  */
  GeoPoint2D pointPosition(const GeoPoint2D& point, const Radian& azimuth,
                           double distance) const override;
//==============================================================================
  /*!
   * \brief Поиск координат точки, находящейся на локсодромии между 
   *        двумя точками на расстоянии, заданном в соотношении, как расстояние 
   *        между точкой начала и искомой (ИТ) и общим расстоянием между 
   *        начальной (НТ) и конечной точками (КТ).
   * \param[in] startPoint - начальная точка
   * \param[in] finishPoint - конечная точка
   * \param[in] distanceRatio - соотношение, вычисляемое по формуле:
   *                            расстояние(НТ-ИТ) / расстояние(НТ-КТ) 
   * \return Координаты точки
  */
  virtual GeoPoint2D pointPosition(const GeoPoint2D& startPoint,
                                   const GeoPoint2D& finishPoint,
                                   double distanceRatio = .5) const override;
//==============================================================================
  /*!
   * \brief Получить длину большой полуоси эллипсоида
   * \return Длина большой полуоси эллипсоида [м]
  */
  double getMajorRadius() const override;
//==============================================================================
  /*!
   * \brief Получить коэффициент полярного сжатия эллипсоида
   * \return Коэффициент полярного сжатия
  */
  double getFlatenning() const override;
//==============================================================================
protected:
//==============================================================================
  std::shared_ptr<Inner::GeodesicLine> 
  createLine(const GeoPoint2D& startPoint,
             const GeoPoint2D& finishPoint) const override;
//==============================================================================
  std::shared_ptr<GeographicLib::Rhumb> mRhumb; //!< Инструмент локсодромии
//==============================================================================
};
//==============================================================================
} //! Geodesic
//==============================================================================
#endif //! GEODESIC_GEOLINETOOLS_LOXODROME_TOOLS_HPP