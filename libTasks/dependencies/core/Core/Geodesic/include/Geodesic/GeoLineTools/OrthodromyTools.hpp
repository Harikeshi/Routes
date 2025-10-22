#ifndef GEODESIC_GEOLINETOOLS_ORTHODROMY_TOOLS_HPP
#define GEODESIC_GEOLINETOOLS_ORTHODROMY_TOOLS_HPP
//==============================================================================
//! Helpers
#include <Geodesic/Helper/Ellipsoid.hpp>
#include <Geodesic/GeoLineTools/Helper/Orthodromy.hpp>
//! Inner
#include <Geodesic/GeoLineTools/Inner/GeodesicLineTools.hpp>
//! std
#include <memory>
//==============================================================================
namespace GeographicLib
{
  class Geodesic;
}
//==============================================================================
namespace Geodesic
{
/*!
* \brief Набор действий/операций над ортодромией
*/
class OrthodromyTools : public Inner::GeodesicLineTools
{
//==============================================================================
public:
//==============================================================================
  /*!
   * Функционал унаследованный от GeodesicLineTools:
   * - divide             - разбиение на равные сегменты
  */
//==============================================================================
  /*!
   * \brief Конструктор класса
   * \param[in] ellipsoid - эллипсоид, на котором производятся вычисления
  */
  OrthodromyTools(const Ellipsoid& ellipsoid = Ellipsoid(EL_WGS_84));
//==============================================================================
  /*!
   * \brief Создание ортодромии по двум точкам
   * \details Точки, ортодромии будут соответствовать допустимому диапазону:
   *          широта в диапазоне [-Pi / 2; Pi / 2];
   *          долгота в диапазоне [-Pi; Pi].
   * \param[in] startPoint - точка начала ортодромии
   * \param[in] finishPoint - точка завершения ортодромии
   * \return Ортодромия
   * \throws GeodesicFailure # OrthodromyThroughOnePoint
   */
  Orthodromy createOrthodromy(const GeoPoint2D& startPoint,
                              const GeoPoint2D& finishPoint) const;
//==============================================================================
    /*!
   * \brief Создание ортодромии заданной протяженности от точки в заданном
   *        направлении
   * \details Точки, ортодромии будут соответствовать допустимому диапазону:
   *          широта в диапазоне [-Pi / 2; Pi / 2];
   *          долгота в диапазоне [-Pi; Pi].
   * \param[in] startPoint - точка начала ортодромии
   * \param[in] azimuth - азимут [радианы]
   * \param[in] length - длина [м] (может равняться 0, если нужно задать
   *                                ортодромию как луч)
   * \return Ортодромия
  */
  Orthodromy createOrthodromy(const GeoPoint2D& startPoint, 
                              const Radian& azimuth, 
                              double length = 50000.) const;
//==============================================================================
  /*!
   * \brief Поиск расстояния между двумя точками по ортодромии. 
   * \param[in] startPoint - точка начала отсчёта
   * \param[in] finishPoint - точка завершения отсчёта
   * \return Расстояние между точками по ортодромии [м] 
  */
  double distance(const GeoPoint2D& startPoint, 
                  const GeoPoint2D& finishPoint) const override;
//==============================================================================
  /*!
   * \brief Поиск координат точки, отстоящей от заданной точки на заданное 
   *        расстояние в заданном направлении по ортодромии. 
   * \param[in] point - точка отсчёта
   * \param[in] azimuth - азимут [радианы]
   * \param[in] distance - расстояние от точки отсчёта [м] 
   * \return Координаты точки
  */
  GeoPoint2D pointPosition(const GeoPoint2D& point, const Radian& azimuth,
                           double distance) const override;
//==============================================================================
  /*!
   * \brief Поиск координат точки, находящейся на ортодромии между 
   *        двумя точками на расстоянии, заданном в соотношении, как расстояние 
   *        между точкой начала и искомой (ИТ) и общим расстоянием между 
   *        начальной (НТ) и конечной точками (КТ).
   * \param[in] startPoint - начальная точка
   * \param[in] finishPoint - конечная точка
   * \param[in] distanceRatio - соотношение, вычисляемое по формуле:
   *                            расстояние(НТ-ИТ) / расстояние(НТ-КТ) 
   * \details если две точки совпадают, возвращается их значение.
   * \return Координаты точки
   */
  virtual GeoPoint2D
  pointPosition(
    const GeoPoint2D& startPoint,
    const GeoPoint2D& finishPoint,
    double distanceRatio = .5) const noexcept override;
//==============================================================================
  /*!
   * \brief Поиск координат точки, делящей ортодромию между
   *        двумя точками на две равные части (середины ортодромии).
   * \param[in] startPoint - начальная точка
   * \param[in] finishPoint - конечная точка
   * \return Координаты точки
   */
  inline GeoPoint2D
  middlePoint(
    const GeoPoint2D& startPoint,
    const GeoPoint2D& finishPoint) const noexcept
  {
    return pointPosition(startPoint, finishPoint, .5);
  }
//==============================================================================
  /*!
   * \brief Формирование контура окружности на поверхности эллипсоида
   * \details Все вершины контура находятся на равном удалении (радиусе) по 
   *          ортодромии от заданной точки центра окружности. 
   * \param[in] center - центр окружности [точка]
   * \param[in] radius - радиус окружности [м] (должен быть больше 0)
   * \param[in] numVerticies - требуемое количество вершин в результирующем 
   *                           контуре
   * \return Результирующий контур [полигон]
   * \throws GeodesicFailure # EllipseDoNotExist
  */
  GeoPolygon2D circle(const GeoPoint2D& center, const double radius,
                      const int numVerticies = 360) const;
//==============================================================================
  /*!
   * \brief Формирование контура окружности на поверхности эллипсоида
   * \details Все вершины контура находятся на равном удалении (радиусе) по 
   *          ортодромии от заданной точки центра окружности. 
   *          Если радиус не превосходит половины расстояния между соседними 
   *          вершинами контура, то окружность не строится.
   * \param[in] center - центр окружности [точка]
   * \param[in] radius - радиус окружности [м] (должен быть больше 0)
   * \param[in] segmentLength - максимальное расстояние между соседними  
   *                            вершинами контура [м] (должно быть больше 0)
   * \return Результирующий контур [полигон]
   * \throws GeodesicFailure # EllipseDoNotExist
   *         GeodesicFailure # IncorrectSegmentLengthForEllipseApproximation
   */
  GeoPolygon2D circle(const GeoPoint2D& center, const double radius,
                      const double segmentLength) const;
//==============================================================================
  /*!
   * \brief Формирование контура эллллипса на поверхности эллипсоида
   * \details Решения итерационное и приближённое (не ниже требуемой точности)
   *          Расстояние между вершинами не превышает заданное.
   *          Если длина большей оси не превосходит расстояния между фокусными 
   *          точками, длина сегмента или длина большей полуоси не превышает 0,
   *          то эллипс не строится.
   * \param[in] firstFocus - первая фокусная точка
   * \param[in] secondFocus - вторая фокусная точка
   * \param[in] majorRadius - длина большей полуоси эллипса [м] 
   *                          (должна быть больше 0)
   * \param[in] segmentLength - максимальное расстояние между соседними  
   *                            вершинами контура [м] (должно быть больше 0)
   * \param[in] accuracy - точность вычисления положения вершин контура [м]
   * \return Результирующий контур [полигон]
   * \throws GeodesicFailure # NonPositiveAccuracy
   *         GeodesicFailure # EllipseDoNotExist
   *         GeodesicFailure # IncorrectSegmentLengthForEllipseApproximation
   */
  GeoPolygon2D ellipse (const GeoPoint2D& firstFocus, 
                        const GeoPoint2D& secondFocus,
                        const double majorRadius,
                        const double segmentLength = 10000.,
                        const double accuracy = 0.1);
//==============================================================================
  /*!
   * \brief Формирование минимального прямоугольника, ориентированного по 
   *        сторонам света, содержащего окружность на поверхности эллипсоида.
   * \param[in] center - центр окружности
   * \param[in] radius - радиус окружности [м] (должен быть больше 0)
   * \param[in] buffer - длина дополнительного отступа от границ построенного
   *                     прямоугольника наружу [м] (должна быть неотрицательным)
   * \return искомый прямоугольник
   * \throws GeodesicFailure # CircleDoNotExist
   *         GeodesicFailure # NegativeDistanceLikeValue
   */
  GeoPolygon2D circleRectangle(const GeoPoint2D& center,
                               const double radius,
                               const double buffer = 0.);
//==============================================================================
  /*!
   * \brief Формирование минимального прямоугольника, содержащего эллипс
   * на поверхности эллипсоида.
   * \param[in] firstFocus -- первая фокусная точка эллипса
   * \param[in] secondFocus -- вторая фокусная точка эллипса
   * \param[in] majorRadius -- длина большой полуоси эллипса [м] 
   *                           (должна быть больше 0)
   * \param[in] buffer -- длина дополнительного отступа
   * от границ построенного прямоугольника наружу [м] (должна быть больше нуля)
   * \param[in] accuracy -- точность вычисления положения вершин контура [м]
   * (должна быть больше нуля)
   * \return искомый прямоугольник
   * \throws GeodesicFailure # NonPositiveAccuracy
   *         GeodesicFailure # NegativeDistanceLikeValue
   *         GeodesicFailure # EllipseDoNotExist
   */
  GeoPolygon2D ellipseRectangle(const GeoPoint2D& firstFocus,
                                const GeoPoint2D& secondFocus,
                                const double majorRadius,
                                const double buffer = 0.,
                                const double accuracy = 0.1);
//==============================================================================
  /*!
   * \brief collapsePoints -- "прорежение" группы точек.
   * \details Строится множество точек, которое содержится в исходном,
   * но при этом в нём нет пары точек,
   * длина ортодромии между которыми меньше указанного параметра.
   * \param[in] points -- множество точек, которое нужно "проредить".
   * \param[in] minimalDistance -- желаемое минимальное расстояние
   * между двумя точками в новом множестве.
   * \return искомое "прореженное" множество точек.
   */
  GeoMultiPoint2D
  collapsePoints(
    const GeoMultiPoint2D& points,
    const double minimalDistance) const noexcept;
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
  std::shared_ptr<GeographicLib::Geodesic> mGeodesic; //!< Инструмент ортодромии
//==============================================================================
};
//==============================================================================
} //! Geodesic
//==============================================================================
#endif //! GEODESIC_GEOLINETOOLS_ORTHODROMY_TOOLS_HPP