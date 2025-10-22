#ifndef POINT_GEOPOINT2D_HPP
#define POINT_GEOPOINT2D_HPP
//==============================================================================
#include <Point/Constants.hpp>
//! MathTools
#include <MathTools/AngularUnits/Degree.hpp>
#include <MathTools/AngularUnits/Radian.hpp>
//! Geos
#include <geos/geom/GeometryFactory.h>
//==============================================================================
/*!
 * \brief Геопространственная точка в двумерном пространстве
 */
class GeoPoint2D
{
//==============================================================================
public:
//==============================================================================
  /*!
   * \brief Конструктор класса через два параметра (координаты) в 
   *                     радианах (Radian) 
   * \param[in] latitude  - значение координаты по широте
   * \param[in] longitude - значение координаты по долготе
   */
  GeoPoint2D(const Radian& latitude = Radian(0.), 
             const Radian& longitude = Radian(0.))
    : mPoint(longitude.get(), latitude.get())
  {}
//==============================================================================
  /*!
   * \brief Конструктор класса через два параметра (координаты) в 
   *        градусах (Degree)  
   * \param[in] latitude  - значение координаты по широте
   * \param[in] longitude - значение координаты по долготе
   */
  GeoPoint2D(const Degree& latitude, const Degree& longitude)
    : mPoint(longitude.toRadians().get(), latitude.toRadians().get())
  {}
//==============================================================================
  /*!
   * \brief Копирующий конструктор класса
   * \param[in] geopoint - координаты геопространственной точки
   */
  GeoPoint2D(const GeoPoint2D& geoPoint)
    : mPoint(geoPoint.mPoint)
  {}
//==============================================================================
  /*!
   * \brief Получение координат точки
   * \return Координаты точки: first  - широта;
   *                           second - долгота.                            
   */
  inline std::pair<Radian, Radian> get() const
  {
    return std::make_pair<Radian, Radian>(Radian(mPoint.y),
                                          Radian(mPoint.x));
  }
//==============================================================================
  /*!
   * \brief Получение координаты точки по широте
   * \return Координата по широте
   */
  inline Radian getLatitude() const
  {
    return Radian(mPoint.y);
  }
//==============================================================================
  /*!
   * \brief Получение координаты точки по долготе
   * \return Координата по долготе
   */
  inline Radian getLongitude() const
  {
    return Radian(mPoint.x);
  }
//==============================================================================
  /*!
   * \brief Задание новых координат геопространственной точки
   * \param[in] latitude  - значение координаты по широте в радианах
   * \param[in] longitude - значение координаты по долготе в радианах
   */
  inline void set(const Radian& latitude, const Radian& longitude)
  {
    mPoint.x = longitude.get();
    mPoint.y = latitude.get();
  }
//==============================================================================
  /*!
   * \brief Задание новых координат геопространственной точки
   * \param[in] latitude  - значение координаты по широте в градусах
   * \param[in] longitude - значение координаты по долготе в градусах
   */
  inline void set(const Degree& latitude, const Degree& longitude)
  {
    mPoint.x = longitude.toRadians().get();
    mPoint.y = latitude.toRadians().get();
  }
//==============================================================================
  /*!
   * \brief Проверка, что координаты геопространственной точки находятся в
   *        диапазоне допустимых значений:
   *        - для широты: -90 (-PI / 2) до 90 (PI / 2);
   *        - для долготы: задаётся параметром.  
   * \param[in] range - диапазон значений для координаты по долготе:
   *                    REGULAR - от -180 (-PI) до 180 (PI)
   *                    NORMAL  - от 0 (0) до 360 (2 * PI)
   */
  bool isCorrect(LongitudeRange range = REGULAR) const;
//==============================================================================
  /*!
   * \brief Приведение координат точек в соответствие с задаваемыми диапазонами
   *        значений
   * \param[in] range - диапазон значений:
   *                    REGULAR - от -180 (-PI) до 180 (PI)
   *                    NORMAL  - от 0 (0) до 360 (2 * PI)
   */
  void normalize(LongitudeRange range = NORMAL);
//==============================================================================
  /*!
   * \brief Сравнение двух геопространственных точек
   * \param[in] point - точка, с которой сравниваем текущую
   * \return true  - совпадают
   *         false - не совпадают
   */
  inline bool operator == (const GeoPoint2D& point) const
  {
    return (BasicMath::isEqual(getLatitude().get(),
                               point.getLatitude().get())
         && BasicMath::isEqual(getLongitude().get(),
                               point.getLongitude().get()));
  }
//==============================================================================
  /*
   * \brief Сравнение двух точек на неравенство
   * \param[in] point - точка, с которой сравниваем текущую
   * \return true  - не совпадают
   *         false - совпадают
   */
  inline bool operator != (const GeoPoint2D& point) const
  {
    return !(*this == point);
  }
//==============================================================================
  /*
   * \brief Деструктор
   */
  virtual ~GeoPoint2D() {}
//==============================================================================
private:
//============================================================================== 
  geos::geom::CoordinateXY mPoint; //!< координаты геопространственной
                                   //!  двумерной точки: (lon, lat),
                                   //!  в радианах.
//==============================================================================
public:
//==============================================================================
  /*!
   * \brief Получение geos-координат точки
   * \details [!] Только для внутренних алгоритмов 
   * \return Координаты точки
   */
  inline geos::geom::CoordinateXY getCoordinates() const
  {
    return mPoint;
  }
//============================================================================== 
};
//==============================================================================
#endif //! POINT_GEOPOINT2D_HPP