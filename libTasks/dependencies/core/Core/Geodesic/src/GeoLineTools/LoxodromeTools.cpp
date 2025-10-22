#include <Geodesic/GeoLineTools/LoxodromeTools.hpp>

#include <Geodesic/GeodesicException.hpp>
//! GeographicLib
#include <GeographicLib/Rhumb.hpp>
//==============================================================================
namespace Geodesic
{
//==============================================================================
LoxodromeTools::LoxodromeTools(const Ellipsoid& ellipsoid 
                                                /*= Ellipsoid(EL_WGS_84)*/)
: GeodesicLineTools()
, mRhumb(std::make_shared<GeographicLib::Rhumb>(ellipsoid.getMajorRadius(),
                                                ellipsoid.getFlatenning()))
{
}
//==============================================================================
Loxodrome LoxodromeTools::createLoxodrome(const GeoPoint2D& startPoint, 
                                          const GeoPoint2D& finishPoint) const
{
  //! Проверяем, совпадают ли начальная и конечная точки.
  if (startPoint == finishPoint)
    throw GeodesicFailure(GeodesicEnumFailure::LoxodromeThroughOnePoint);

  //! Создаем локсодромию 
  Loxodrome loxodrome;

  //! Задаём точки
  loxodrome.mStartPoint = startPoint;
  loxodrome.mFinishPoint = finishPoint;

  //! Широта точек должна быть в диапазоне [-Pi / 2; Pi / 2]
  //! Долгота точек должна быть в диапазоне[-Pi; Pi]
  loxodrome.mStartPoint.normalize(REGULAR);
  loxodrome.mFinishPoint.normalize(REGULAR);

  //! Готовим координаты точки начала в градусах, они нам понадобятся дважды
  double startLat(loxodrome.mStartPoint.getLatitude().toDegrees().get());
  double startLon(loxodrome.mStartPoint.getLongitude().toDegrees().get());

  //! Вычисляем длину и азимут
  double azimuth(0.); //!< азимут в градусах

  mRhumb->Inverse(
    startLat,
    startLon,
    loxodrome.mFinishPoint.getLatitude().toDegrees().get(),
    loxodrome.mFinishPoint.getLongitude().toDegrees().get(),
    loxodrome.mLength, azimuth);
   
  //! Записываем азимут в радианах
  loxodrome.mStartAzimuth.fromDegrees(Degree(azimuth));
  loxodrome.mFinishAzimuth = loxodrome.mStartAzimuth;

  //! Создаем линию
  loxodrome.mLine = std::make_shared<GeographicLib::RhumbLine>(
                                    mRhumb->Line(startLat, startLon, azimuth));

  //! Возвращаем значение
  return loxodrome;
}
//==============================================================================
Loxodrome LoxodromeTools::createLoxodrome(
  const GeoPoint2D& startPoint, const Radian& azimuth, 
  double length /*= 50000.*/) const
{
  //! Создаем локсодромию 
  Loxodrome loxodrome;
  
  //! Задаём точку начала
  loxodrome.mStartPoint = startPoint;
  //! Задаём азимут
  loxodrome.mStartAzimuth = azimuth;
  loxodrome.mFinishAzimuth = azimuth;
  //! Задаём протяжённость
  loxodrome.mLength = length;

  //! Широта точек должна быть в диапазоне [-Pi / 2; Pi / 2]
  //! Долгота точек должна быть в диапазоне[-Pi; Pi]
  loxodrome.mStartPoint.normalize(REGULAR);

  //! Создаем линию
  loxodrome.mLine = std::make_shared<GeographicLib::RhumbLine>(
                      mRhumb->Line(
                        loxodrome.mStartPoint.getLatitude().toDegrees().get(),
                        loxodrome.mStartPoint.getLongitude().toDegrees().get(),
                        azimuth.toDegrees().get()));

  //! Находим точку завершения
  loxodrome.mFinishPoint = loxodrome.pointPosition(length);

  //! Если конечная точка неопределена, мы пересекли полюс.
  if (std::isnan(loxodrome.mFinishPoint.getLongitude().get()))
    throw GeodesicFailure(GeodesicEnumFailure::LoxodromeThroughPole);

  //! Возвращаем значение
  return loxodrome;
}
//==============================================================================
double LoxodromeTools::distance(const GeoPoint2D& startPoint, 
                                const GeoPoint2D& finishPoint) const
{
  //! Подготавливаем точки
  GeoPoint2D start = startPoint;
  GeoPoint2D finish = finishPoint;
  start.normalize(REGULAR);
  finish.normalize(REGULAR);

  //! Находим дальность: решение обратной локсодромической задачи 
  double result;
  double azimuth;
  mRhumb->Inverse(start.getLatitude().toDegrees().get(),
                  start.getLongitude().toDegrees().get(),
                  finish.getLatitude().toDegrees().get(),
                  finish.getLongitude().toDegrees().get(),
                  result, azimuth);

  return result;
}
//==============================================================================
GeoPoint2D LoxodromeTools::pointPosition(const GeoPoint2D& point,
                                         const Radian& azimuth, double distance) 
const
{
  //! Подготавливаем точку отсчёта
  GeoPoint2D startPoint = point;
  startPoint.normalize(REGULAR);

  //! Инициализация координат искомой точки в градусах
  double lat;
  double lon;

  //! Находим нужную точку
  mRhumb->Direct(startPoint.getLatitude().toDegrees().get(),
                 startPoint.getLongitude().toDegrees().get(),
                 azimuth.toDegrees().get(), distance,
                 lat, lon);

  //! Формируем результат
  GeoPoint2D result = GeoPoint2D(Degree(lat), Degree(lon));
  result.normalize(REGULAR);

  return result;
}
//==============================================================================
GeoPoint2D LoxodromeTools::pointPosition(const GeoPoint2D& startPoint, 
                                         const GeoPoint2D& finishPoint, 
                                         double distanceRatio /*= .5*/) const
{
  //! Создание ортодромии
  Loxodrome loxodrome = createLoxodrome(startPoint, finishPoint);
  //! Вычисление искомой точки
  return loxodrome.pointPosition(distanceRatio * loxodrome.getLength());
}
//==============================================================================
double LoxodromeTools::getMajorRadius() const
{
  return mRhumb->MajorRadius();
}
//==============================================================================
double LoxodromeTools::getFlatenning() const
{
  return mRhumb->Flattening();
}
//==============================================================================
std::shared_ptr<Inner::GeodesicLine> 
LoxodromeTools::createLine(const GeoPoint2D& startPoint,
                           const GeoPoint2D& finishPoint) const
{
  return std::make_shared<Loxodrome>(createLoxodrome(startPoint, finishPoint));
}
//==============================================================================
} //! Geodesic
//==============================================================================
