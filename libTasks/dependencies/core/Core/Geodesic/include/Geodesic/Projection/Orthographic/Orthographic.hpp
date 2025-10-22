#ifndef GEODESIC_PROJECTION_ORTHOGRAPHIC_ORTHOGRAPHIC_HPP
#define GEODESIC_PROJECTION_ORTHOGRAPHIC_ORTHOGRAPHIC_HPP
//==============================================================================
#include <Geodesic/Projection/AbstractProjection.hpp>
//! Helper
#include <Geodesic/Helper/Ellipsoid.hpp>
//! Point
#include <Point/Point2D.hpp>
#include <Point/Point3D.hpp>
#include <Point/GeoPoint2D.hpp>
#include <Point/GeoPoint3D.hpp>
//! AbstractOperations
#include <AbstractOperations/Algorithms/Union.hpp>
//! std
#include <memory>
//==============================================================================
namespace Geodesic
{
/*!
 * \brief Класс Ортографическая проекция
*/
class OrthographicProjection : public AbstractProjection
{
//==============================================================================
public:
//==============================================================================
  /*!
   * \brief Конструктор класса
   * \details Если эллипсоид не задан (пустой указатель), то собирается
   *          эллипсоид WGS-84
   * \param[in] ellipsoid - эллипсоид в котором задаются геодезические 
   *                        координаты
   * \param[in] center - точка приложения (центр области) проекции
   * \param[in] azimuth - азимут направления оси Y системы координат проекции
   *                      [радианы]
  */
  OrthographicProjection(
            const std::shared_ptr<Ellipsoid>& ellipsoid 
                                          = std::make_shared<Ellipsoid>(EL_WGS_84),
            const GeoPoint3D& center = GeoPoint3D(Radian(0.), Radian(0.), 0.f),
            const Radian& azimuth = Radian(0.));
//==============================================================================
  /*!
   * \brief Конструктор класса
   * \details Если эллипсоид не задан (пустой указатель), то собирается
   *          эллипсоид WGS-84
   * \param[in] center - точка приложения (центр области) проекции
   * \param[in] ellipsoid - эллипсоид в котором задаются геодезические
   *                        координаты
   * \param[in] azimuth - азимут направления оси Y системы координат проекции
   *                      [радианы]
  */
  OrthographicProjection(
    const GeoPoint2D& center,
    const std::shared_ptr<Ellipsoid>& ellipsoid
                                          = std::make_shared<Ellipsoid>(EL_WGS_84),
    const Radian& azimuth = Radian(0.));
//==============================================================================
  /*!
   * \brief Перевод точки в проекцию (из геодезических в декартовы координаты)
   * \details Для 3D
   * \param[in] geoPoint - геодезические координаты точки
   * \return Декартовы координаты точки в проекции
  */
  Point3D toProjection(const GeoPoint3D& geoPoint) const;
//==============================================================================
  /*!
   * \brief Перевод точки в проекцию (из геодезических в декартовы координаты)
   * \details Для 2D
   * \param[in] geoPoint - геодезические координаты точки
   * \return Декартовы координаты точки в проекции
  */
  Point2D toProjection(const GeoPoint2D& geoPoint) const;
//==============================================================================
  /*!
   * \brief Перевод множества многоугольников в проекцию
   * (из геодезических в декартовы координаты)
   * \param[in] geoMultiPolygon - множество многоугольников на эллипсоиде
   * \return множество многоугольников в проекции
   */
  template <typename GeoPointType, typename ProjectionPointType>
  PrimaryEntities::MultiPolygon<ProjectionPointType>
  toProjection(
    const PrimaryEntities::MultiPolygon<GeoPointType>& geoMultiPolygon)
  {
    //!< Результат.
    PrimaryEntities::MultiPolygon<ProjectionPointType> result;
    result.reserve(geoMultiPolygon.size());

    for (const auto& geoPolygon : geoMultiPolygon)
    {
      size_t geoPolygonSize = geoPolygon.outer().size();

      // Проецируем внешнее кольцо
      PrimaryEntities::Polygon<ProjectionPointType> polygon;
      polygon.outer().reserve(geoPolygonSize);
      for (size_t i = 0; i < geoPolygonSize - 1; ++i)
        polygon.outer().emplace_back(toProjection(geoPolygon.outer().at(i)));

      // Последняя точка либо проецируется, либо копируется из
      // первой в зависимости от замкнутости контура
      polygon.outer().emplace_back(
        (geoPolygon.outer().at(0)
          == geoPolygon.outer().at(geoPolygonSize - 1))
        ? polygon.outer().at(0)
        : toProjection(geoPolygon.outer().at(geoPolygonSize - 1)));

      polygon.inners().reserve(geoPolygon.inners().size());
      for (const auto& geoRing : geoPolygon.inners())
      {
        size_t geoRingSize = geoRing.size();
        // Проецируем внутреннее кольцо
        PrimaryEntities::Ring<ProjectionPointType> ring;
        ring.reserve(geoRingSize);
        for (size_t i = 0; i < geoRingSize - 1; ++i)
          ring.emplace_back(toProjection(geoRing.at(i)));

        // Последняя точка либо проецируется, либо копируется из
        // первой в зависимости от замкнутости контура
        ring.emplace_back(
          (geoRing.at(0) == geoRing.at(geoRingSize - 1))
          ? ring.at(0) : toProjection(geoRing.at(geoRingSize - 1)));

        polygon.inners().emplace_back(ring);
      }
      result.emplace_back(polygon);
    }

    return result;
  }
//==============================================================================
  /*!
   * \brief Перевод точки из проекции (из декартовых в геодезические координаты)
   * \param[in] point - декартовы координаты точки
   * \return Геодезические координаты точки, соответствующие видимой стороне 
   *         проекции
   * \throws GeodesicFailure # PointOutOfProjection
   */
  GeoPoint3D fromProjection(const Point3D& point) const;
//==============================================================================
  /*!
   * \brief Перевод точки из проекции (из декартовых в геодезические координаты)
   * \param[in] point - декартовы координаты точки
   * \return Геодезические координаты точки, соответствующие видимой стороне 
   *         проекции
   * \throws GeodesicFailure # PointOutOfProjection
   */
  GeoPoint2D fromProjection(const Point2D& point) const;
//==============================================================================
  /*!
   * \brief Перевод множества многоугольников из проекции
   * (из декартовых в геодезические координаты)
   * \param[in] multiPolygon - множество многоугольников в проекции
   * \return множество многоугольников на эллипсоиде
   */
  template <typename ProjectionPointType, typename GeoPointType>
  PrimaryEntities::MultiPolygon<GeoPointType>
    fromProjection(
      const PrimaryEntities::MultiPolygon<ProjectionPointType>& multiPolygon)
  {
    //!< Результат.
    PrimaryEntities::MultiPolygon<GeoPointType> result;
    result.reserve(multiPolygon.size());

    for (const auto& polygon : multiPolygon)
    {
      size_t polygonSize = polygon.outer().size();

      // Переводим из проекции внешнее кольцо
      PrimaryEntities::Polygon<GeoPointType> geoPolygon;
      geoPolygon.outer().reserve(polygonSize);
      for (size_t i = 0; i < polygonSize - 1; ++i)
        geoPolygon.outer().emplace_back(fromProjection(polygon.outer().at(i)));

      // Последняя точка либо переводится отдельно, либо копируется
      // из первой в зависимости от замкнутости контура
      geoPolygon.outer().emplace_back(
        (polygon.outer().at(0)
          == polygon.outer().at(polygonSize - 1))
        ? geoPolygon.outer().at(0)
        : fromProjection(polygon.outer().at(polygonSize - 1)));

      geoPolygon.inners().reserve(polygon.inners().size());
      for (const auto& ring : polygon.inners())
      {
        size_t ringSize = ring.size();

        // Переводим из проекции внутреннее кольцо
        PrimaryEntities::Ring<GeoPointType> geoRing;
        geoRing.reserve(ringSize);
        for (size_t i = 0; i < ringSize - 1; ++i)
          geoRing.emplace_back(fromProjection(ring.at(i)));

        // Последняя точка либо переводится отдельно, либо копируется
        // из первой в зависимости от замкнутости контура
        geoRing.emplace_back(
          (ring.at(0) == ring.at(ringSize - 1))
          ? geoRing.at(0) : fromProjection(ring.at(ringSize - 1)));

        geoPolygon.inners().emplace_back(geoRing);
      }
      result.emplace_back(geoPolygon);
    }
    return result;
  }
//==============================================================================
  /*!
   * \brief Задание центра проекции
   * \param[in] center - точка приложения (центр области) проекции
   * \param[in] azimuth - азимут направления оси Y системы координат проекции
   *                      [радианы]
  */
  void setProjectionPoint(const GeoPoint3D center, 
                          const Radian azimuth = Radian(0.));
//==============================================================================
  /*!
   * \brief Задание центра проекции
   * \param[in] center - точка приложения (центр области) проекции
   * \param[in] azimuth - азимут направления оси Y системы координат проекции
   *                      [радианы]
  */
  void setProjectionPoint(const GeoPoint2D center, 
                          const Radian azimuth = Radian(0.));
//==============================================================================
  /*!
   * \brief Получение центра проекции
   * return Точка приложения (центр области) проекции
  */
  GeoPoint3D getCenterProjection() const;
  //==============================================================================
  /*!
  * \brief Получение центра проекции
  * return Точка приложения (центр области) проекции
  */
  GeoPoint2D getCenterProjection2D() const;
//==============================================================================
  /*!
   * \brief Получение азимута направления оси Y системы координат проекции
   * return Азимут направления оси Y системы координат проекции [радианы]
  */
  Radian getAzimuth() const;
//==============================================================================
  /*!
   * \brief Задание эллипсоида задания геодезических координат
   * \param[in] ellipsoid - эллипсоид в котором задаются геодезические
   *                        координаты
   * return Признак успешности перестроения проекции:
   *        true  - проекция перестроена;
   *        false - проекция не перестроена (пустой указатель на эллипсоид).          
  */
  bool setEllipsoid(const std::shared_ptr<Ellipsoid>& ellipsoid);
//==============================================================================
  /*!
   * \brief Получение эллипсоида задания геодезических координат
   * return Эллипсоид в котором задаются геодезические координаты
  */
  std::shared_ptr<Ellipsoid> getEllipsoid() const;
//==============================================================================
private:
//==============================================================================
  std::shared_ptr<OrthographicInner> mInner;
//==============================================================================
};
//==============================================================================
} //! Geodesic
//==============================================================================
#endif //! GEODESIC_PROJECTION_ORTHOGRAPHIC_ORTHOGRAPHIC_HPP