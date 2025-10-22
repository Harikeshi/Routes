#include <Geodesic/DatumTransform/DatumTransform.hpp>

#include <Geodesic/GeodesicException.hpp>
//! Cartographic
#include <Cartographic/Alexeev/alexeev.hpp>
#include <Cartographic/database.hpp>
#include <Cartographic/latitudelongitude.hpp>
#include <Cartographic/transformation.hpp>
//==============================================================================
namespace Geodesic
{
//==============================================================================
DatumTransform::DatumTransform(CoordinateSystems initSystem, 
                               CoordinateSystems goalSystem)
: mInitSystem(initSystem)
, mGoalSystem(goalSystem)
, mTransformation(nullptr)
{
  //! подготовка нового преобразователя 
  createTransformation();
}
//==============================================================================
void DatumTransform::setSystems(CoordinateSystems initSystem,
                                CoordinateSystems goalSystem)
{
  mInitSystem = initSystem;
  mGoalSystem = goalSystem;
  createTransformation();
}
//==============================================================================
void DatumTransform::swapSystems()
{
  std::swap(mInitSystem, mGoalSystem);
  createTransformation();
}
//==============================================================================
GeoPoint2D DatumTransform::transform(const GeoPoint2D& point) const
{
  //! Инициализируем результат и вводим третью координату
  GeoPoint3D result(point, 0.);
  //! Преобразуем
  tramsformInner(result);
  //! Возвращаем результат
  return result.get().first;
}
//==============================================================================
GeoPoint3D DatumTransform::transform(const GeoPoint3D& point) const
{
  //! Инициализируем результат
  GeoPoint3D result = point;
  //! Преобразуем
  tramsformInner(result);
  //! Возвращаем результат
  return result;
}
//==============================================================================
void DatumTransform::operator()(GeoPoint3D& point) const
{
  tramsformInner(point);
}
//==============================================================================
void DatumTransform::operator()(GeoPoint2D& point) const
{
  //! Вводим третью координату
  GeoPoint3D point3D(point, 0.);
  //! Преобразуем
  tramsformInner(point3D);
  //! Обновляем координаты точки
  point = point3D.get().first;
}
//==============================================================================
void DatumTransform::createTransformation()
{
  //! Если уже преобразователь есть, то сначала его надо убрать
  if (mTransformation)
    mTransformation.reset();

  //! Сравниваем исходную и целевую СК
  if (mInitSystem == mGoalSystem)
    return; //!< Преобразователь не нужен
  
  //! Инициализация нового преобразователя  
  mTransformation = std::make_shared<Cartographic::Transformation>();

  //! Задание параметров преобразования для исходной СК
  switch (mInitSystem)
  {
  case CS_WGS_84:
    mTransformation->SetSourceProjection<Cartographic::LatitudeLongitude>(
      new Cartographic::Ellipsoid::WGS_84);
    mTransformation->AddDatumShift(
      new Cartographic::HelmertDatumShift<
        Cartographic::Datum::WGS_84,
        Cartographic::Datum::PZ_90_11,
        Cartographic::Authority::GOST_32453_2017>);
    break;
  case CS_SK_42:
    mTransformation->SetSourceProjection<Cartographic::LatitudeLongitude>(
      new Cartographic::Ellipsoid::Krasovsky);
    mTransformation->AddDatumShift(
      new Cartographic::HelmertDatumShift<
        Cartographic::Datum::SK_42,
        Cartographic::Datum::PZ_90_11,
        Cartographic::Authority::GOST_32453_2017>);
    break;
  case CS_PZ_90_11:
    mTransformation->SetSourceProjection<Cartographic::LatitudeLongitude>(
      new Cartographic::Ellipsoid::WGS_84);
    break;
  case CS_PZ_90_02:
    mTransformation->SetSourceProjection<Cartographic::LatitudeLongitude>(
      new Cartographic::Ellipsoid::WGS_84);
    mTransformation->AddDatumShift(
      new Cartographic::HelmertDatumShift<
        Cartographic::Datum::PZ_90_02,
        Cartographic::Datum::PZ_90_11,
        Cartographic::Authority::GOST_32453_2017>);
    break;
  default:
    mTransformation.reset(); //!< непонятно что не создаём
    throw GeodesicFailure(GeodesicEnumFailure::UnknownCoordinateSystem);
  }

  //! Задание параметров преобразования для целевой СК
  switch (mGoalSystem)
  {
  case CS_WGS_84:
    mTransformation->AddDatumShift(
      new Cartographic::HelmertDatumShift<
        Cartographic::Datum::PZ_90_11,
        Cartographic::Datum::WGS_84,
        Cartographic::Authority::GOST_32453_2017>);
    mTransformation->SetGeocentric<Cartographic::Alexeev::Geocentric>();
    mTransformation->SetTargetProjection<Cartographic::LatitudeLongitude>(
      new Cartographic::Ellipsoid::WGS_84);
    break;
  case CS_SK_42:
    mTransformation->AddDatumShift(
      new Cartographic::HelmertDatumShift<
        Cartographic::Datum::PZ_90_11,
        Cartographic::Datum::SK_42,
        Cartographic::Authority::GOST_32453_2017>);
    mTransformation->SetGeocentric<Cartographic::Alexeev::Geocentric>();
    mTransformation->SetTargetProjection<Cartographic::LatitudeLongitude>(
      new Cartographic::Ellipsoid::Krasovsky);
    break;
  case CS_PZ_90_11:
    mTransformation->SetGeocentric<Cartographic::Alexeev::Geocentric>();
    mTransformation->SetTargetProjection<Cartographic::LatitudeLongitude>(
      new Cartographic::Ellipsoid::WGS_84);
    break;
  case CS_PZ_90_02:
    mTransformation->AddDatumShift(
      new Cartographic::HelmertDatumShift<
        Cartographic::Datum::PZ_90_11,
        Cartographic::Datum::PZ_90_02,
        Cartographic::Authority::GOST_32453_2017>);
    mTransformation->SetGeocentric<Cartographic::Alexeev::Geocentric>();
    mTransformation->SetTargetProjection<Cartographic::LatitudeLongitude>(
      new Cartographic::Ellipsoid::WGS_84);
    break;
  default:
    mTransformation.reset(); //!< непонятно что не создаём
    throw GeodesicFailure(GeodesicEnumFailure::UnknownCoordinateSystem);
  }
}
//==============================================================================
void DatumTransform::tramsformInner(GeoPoint3D& point) const
{
  //! Проверяем, что система исправна
  if (!mTransformation)
    return; //! Перевод не осуществляется (совпадают исходная и целевая СК);

  //! Приводим точку в регулярный диапазон долгот: от -180 (-PI) до 180 (PI) 
  if (!point.isCorrect())
    point.normalize(REGULAR);

  //! Преобразователь работает в градусах, готовим исходные данные
  Cartographic::Degrees latitude(point.getLatitude().toDegrees().get());
  Cartographic::Degrees longitude(point.getLongitude().toDegrees().get());
  Cartographic::Meters height(point.getHeight());

  //! Выполняем преобразование
  mTransformation->Transform(latitude, longitude, height);

  //! Обновляем координаты точки
  point.set(GeoPoint2D(Degree(latitude), Degree(longitude)), height);
}
//==============================================================================
}
//==============================================================================

