#ifndef POINT_GEOPOINT3D_HPP
#define POINT_GEOPOINT3D_HPP
//==============================================================================
#include <Point/GeoPoint2D.hpp>
//! MathTools
#include <MathTools/BasicMath/Compare.hpp>
//==============================================================================
/*!
 * \brief Геопространственная точка в трёхмерном пространстве
 */
class GeoPoint3D
{
//==============================================================================
public:
//==============================================================================
  /*!
   * \brief Конструктор класса через два параметра
   * \param[in] geoPoint - координаты точки по широте и долготе
   * \param[in] height   - координата точки по высоте в метрах
   */
  GeoPoint3D(const GeoPoint2D& geoPoint, const double height)
  {
    geos::geom::CoordinateXY coordinates = geoPoint.getCoordinates();
    mPoint.x = coordinates.x;
    mPoint.y = coordinates.y;
    mPoint.z = height;
  }
//==============================================================================
  /*!
   * \brief Конструктор класса через три координаты: широту, долготу и высоту
   * \param[in] latitude  - координата точки по широте в радианах
   * \param[in] longitude - координата точки по долготе в радианах
   * \param[in] height    - координата точки по высоте в метрах
   */
  GeoPoint3D(const Radian& latitude = Radian(0.), 
             const Radian& longitude = Radian(0.),
             const double height = 0.)
    : mPoint(longitude.get(), latitude.get(), height)
  {
  }
//==============================================================================
  /*!
   * \brief Конструктор класса через три координаты: широту, долготу и высоту
   * \param[in] latitude  - координата точки по широте в градусах
   * \param[in] longitude - координата точки по долготе в градусах
   * \param[in] height    - координата точки по высоте в метрах
   */
  GeoPoint3D(const Degree& latitude, const Degree& longitude, 
             const double height = 0.)
    : mPoint(longitude.toRadians().get(), latitude.toRadians().get(), height)
  {
  }
//==============================================================================
  /*!
   * \brief Копирующий конструктор
   * \param[in] geoPoint - копируемая точка
   */
  GeoPoint3D(const GeoPoint3D& geoPoint)
    : mPoint(geoPoint.mPoint)
  {}
//==============================================================================
  /*!
   * \brief Получение координат точки
   * \return Координаты точки: first  - по широте и долготе в радианах;
   *                           second - по высоте в метрах;
   */
  inline std::pair<GeoPoint2D, double> get() const
  {
    return std::make_pair(GeoPoint2D(getLatitude(), getLongitude()), mPoint.z);
  }
  //==============================================================================
  /*!
   * \brief Получение координат точки
   * \return Координаты точки (широта и долгота)
   */
  inline GeoPoint2D get2D() const
  {
    return GeoPoint2D(getLatitude(), getLongitude());
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
   * \brief Получение координаты точки по высоте
   * \return Координата по высоте
   */
  inline double getHeight() const
  {
    return mPoint.z;
  }
//==============================================================================
  /*!
   * \brief Задание координаты точки по высоте
   * \param[in] height - новая координата точки по высоте [м].
   */
  inline void setHeight(double height) noexcept
  {
    mPoint.z = height;
  }
//==============================================================================
  /*!
   * \brief Задание новых координат точки
   * \param[in] geoPoint - новые координаты точки по широте и долготе
   * \param[in] height   - новая координата точки по высоте в метрах
   */
  inline void set(const GeoPoint2D& geoPoint, const double height)
  {
    geos::geom::CoordinateXY coordinates = geoPoint.getCoordinates();
    mPoint.x = coordinates.x;
    mPoint.y = coordinates.y;
    mPoint.z = height;
  }
//==============================================================================
  /*!
   * \brief Проверка, что координаты геопространственной точки находятся в 
   *        диапазоне допустимых значений:
   *        - для широты: -90 (-PI / 2) до 90 (PI / 2);
   *        - для долготы: задаётся параметром;
   *        - для высоты: не проверяется.  
   * \param[in] range - диапазон значений для координаты по долготе:
   *                    REGULAR - от -180 (-PI) до 180 (PI)
   *                    NORMAL  - от 0 (0) до 360 (2 * PI)
   */
  bool isCorrect(LongitudeRange range = REGULAR) const
  {
    return get2D().isCorrect(range);
  }
//==============================================================================
  /*!
   * \brief Приведение координат точек в соответствие с задаваемыми диапазонами
   *        значений:
   *        - для широты: -90 (-PI / 2) до 90 (PI / 2);
   *        - для долготы: задаётся параметром;
   *        - для высоты: не проверяется.
   * \param[in] range - диапазон значений:
   *                    REGULAR - от -180 (-PI) до 180 (PI)
   *                    NORMAL  - от 0 (0) до 360 (2 * PI)
   */
  void normalize(LongitudeRange range = NORMAL)
  {
    GeoPoint2D normalizedPoint = get2D();
    normalizedPoint.normalize(range);
    mPoint.x = normalizedPoint.getCoordinates().x;
  }
//==============================================================================
  /*
   * \brief Сравнение двух точек на равенство
   * \param[in] geoPoint - точка, с которой сравниваем текущую
   * \return true  - совпадают
   *         false - не совпадают
   */
  inline bool operator == (const GeoPoint3D& geoPoint) const
  {
    return (BasicMath::isEqual(getLatitude().get(),
                               geoPoint.getLatitude().get())
         && BasicMath::isEqual(getLongitude().get(),
                               geoPoint.getLongitude().get())
         && BasicMath::isEqual(getHeight(), geoPoint.getHeight()));
  }
//==============================================================================
  /*
   * \brief Сравнение двух точек на неравенство
   * \param[in] point - точка, с которой сравниваем текущую
   * \return true  - не совпадают
   *         false - совпадают
   */
  inline bool operator != (const GeoPoint3D& point) const
  {
    return !(*this == point);
  }
//==============================================================================
  /*
   * \brief Деструктор
   */
  virtual ~GeoPoint3D() {}
//==============================================================================
private: 
//==============================================================================
  geos::geom::Coordinate mPoint; //!< координаты геопространственной
                                 //!  трёхмерной точки: (lon, lat, height),
                                 //!  широта и долгота в радианах.
//==============================================================================
public:
//==============================================================================
  /*!
   * \brief Получение geos-координат точки
   * \details [!] Только для внутренних алгоритмов
   * \return Координаты точки
   */
  inline geos::geom::Coordinate getCoordinates() const
  {
    return mPoint;
  }
//==============================================================================
};
//==============================================================================
#endif //! POINT_GEOPOINT3D_HPP