#include <Geodesic/GeoLineTools/Inner/GeodesicLine.hpp>
//! MathTools
#include <MathTools/BasicMath/LengthOperations.hpp> 
//==============================================================================
namespace Geodesic
{
namespace Inner
{
//==============================================================================
double GeodesicLine::divide(
  std::vector<GeoPoint2D>& geoPoints, 
  double maxSegmentLength /*= DEFAULT_MAX_SEGMENT_LENGTH*/,
  bool hasIncludeStartPoint /*= true*/,
  bool hasIncludeFinishPoint /*= true*/) const
{
  //! Добавляем точку начала, если надо
  if (hasIncludeStartPoint)
    geoPoints.emplace_back(mStartPoint);

  //! Проверяем необходимость и вычисляем параметры разбиения
  double segmentLength;
  int numSegments;
  if (BasicMath::avgSegmentLength(mLength, maxSegmentLength, segmentLength, 
                                  numSegments))
  {
    //! Ищем точки разбиения (количество сегментов - 1)
    for (int i = 1; i < numSegments; i++)
      geoPoints.emplace_back(pointPosition(i * segmentLength));
  }

  //! Добавляем точку завершения, если надо
  if (hasIncludeFinishPoint)
    geoPoints.emplace_back(mFinishPoint);

  //! Возвращаем длину сегмента разбиения 
  return segmentLength;
}
//==============================================================================
std::vector<GeoPoint2D> GeodesicLine::divide(
  double maxSegmentLength /*= DEFAULT_MAX_SEGMENT_LENGTH.*/) const
{
  //! Инициализация результирующего набора точек
  std::vector<GeoPoint2D> result;

  //! Разбиение
  divide(result, maxSegmentLength);

  //! Возвращение результата
  return result;
}
//==============================================================================
GeodesicLine::GeodesicLine()
  : mStartPoint(GeoPoint2D())
  , mFinishPoint(GeoPoint2D())
  , mLength(0.)
  , mStartAzimuth(Radian(0.))
  , mFinishAzimuth(Radian(0.))
{
}
//==============================================================================
} //! Inner
} //! Geodesic
//==============================================================================
