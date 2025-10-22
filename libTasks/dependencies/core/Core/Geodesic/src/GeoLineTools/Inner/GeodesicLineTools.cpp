#include <Geodesic/GeoLineTools/Inner/GeodesicLineTools.hpp>
//! GeodesicException
#include <Geodesic/GeodesicException.hpp>
//==============================================================================
namespace Geodesic
{
namespace Inner
{
//==============================================================================
double GeodesicLineTools::length(
  const std::vector<GeoPoint2D>& points, 
  std::vector<double>& segmentsLength) const
{
  //! Готовим результат по-умолчанию
  double result(0.);
  segmentsLength.clear();

  //! Определяем количество сегментов
  std::size_t numSegments(points.size() - 1);

  //! Проходимся по сегментам и считаем расстояния
  for (std::size_t i = 0; i < numSegments; i++)
  {
    segmentsLength.emplace_back(distance(points[i], points[i+1]));
    result += segmentsLength[i];
  }

  //! Возвращаем результат
  return result;
}
//==============================================================================
double GeodesicLineTools::length(
  const GeoLinestring2D& linestring, 
  std::vector<double>& segmentsLength) const
{
  //! Превращаем ломаную в набор точек и считаем расстояние
  return length(std::vector<GeoPoint2D>(linestring), segmentsLength);
}
//==============================================================================
double GeodesicLineTools::length(const std::vector<GeoPoint2D>& points) const
{
  //! Готовим результат по-умолчанию
  double result(0.);

  //! Определяем количество сегментов
  std::size_t numSegments(points.size() - 1);

  //! Проходимся по сегментам и считаем расстояния
  for (std::size_t i = 0; i < numSegments; i++)
    result += distance(points[i], points[i + 1]);

  //! Возвращаем результат
  return result;
}
//==============================================================================
double GeodesicLineTools::length(const GeoLinestring2D& linestring) const
{
  //! Превращаем ломаную в набор точек и считаем расстояние
  return length(std::vector<GeoPoint2D>(linestring));
}
//==============================================================================
std::vector<GeoPoint2D> 
GeodesicLineTools::divide(
  const GeoPoint2D& startPoint, 
  const GeoPoint2D& finishPoint, 
  double maxSegmentLength /*= DEFAULT_MAX_SEGMENT_LENGTH*/) const
{
  //! Создаём геодезическую и разбиваем её на сегменты
  try
  {
    return createLine(startPoint, finishPoint)->divide(maxSegmentLength);
  }
  catch (const GeodesicFailure& failure)
  {
    if (    (failure.getCode() == OrthodromyThroughOnePoint)  
         || (failure.getCode() == LoxodromeThroughOnePoint))
      //! Если геодезическая линия проходит через две одинаковые точки, то
      //! возвращаем их как результат
      return { startPoint, finishPoint };
    else
      throw GeodesicFailure(TransitFailure, std::string(failure.what()), 
                            "divide");
  }
  catch (...)
  {
    ExceptionTools::FailureHandler<GeodesicFailure>("divide");
  }
  return std::vector<GeoPoint2D>(); //!< нужно для спокойствия компилятора
}
//==============================================================================
void GeodesicLineTools::divide(
  std::vector<GeoPoint2D>& points, 
  double maxSegmentLength /*= DEFAULT_MAX_SEGMENT_LENGTH*/) const
{
  std::size_t numSegments(points.size() - 1); //!< количество сегментов в наборе

  if (numSegments < 1) //! Если количество сегментов меньше 1, то
    return;            //! ничего делать не надо 

  //! Инициализация результата
  std::vector<GeoPoint2D> result;

  //! Последовательный проход по всем парам точек в наборе
  for (int i = 0; i < numSegments; i++)
  {
    try 
    {
      createLine(points[i], points[i + 1])->divide(result, maxSegmentLength,
                                                   true, false);
    }
    catch (const GeodesicFailure& failure)
    {
      if ((failure.getCode() == OrthodromyThroughOnePoint)
        || (failure.getCode() == LoxodromeThroughOnePoint))
        //! Если геодезическая линия проходит через две одинаковые точки, то
        //! к результату добавляем только первую
        result.emplace_back(points[i]);
      else
        throw GeodesicFailure(TransitFailure, std::string(failure.what()),
                              "divide");
    }
    catch (...)
    {
      ExceptionTools::FailureHandler<GeodesicFailure>("divide");
    }
  }

  //! Замыкаем результат последней точкой набора
  result.emplace_back(points[numSegments]);

  //! Нормализуем замыкающую точку в требуемый диапазон 
  //! (остальные уже к нему приведены)
  result[result.size() - 1].normalize(REGULAR);

  //! Обновляем исходный набор до результата
  points = result;
}
//==============================================================================
void GeodesicLineTools::divide(
  const std::vector<GeoPoint2D>& inPoints, 
  std::vector<GeoPoint2D>& outPoints, 
  double maxSegmentLength /*= DEFAULT_MAX_SEGMENT_LENGTH*/) const
{
  //! Собираем результат из исходного набора
  outPoints = inPoints;

  //! Разбиваем  
  divide(outPoints, maxSegmentLength);
}
//==============================================================================
void GeodesicLineTools::divide(
  GeoLinestring2D& linestring, 
  double maxSegmentLength /*= DEFAULT_MAX_SEGMENT_LENGTH*/) const
{
  //! Инициализация результата разбиения
  std::vector<GeoPoint2D> result;

  //! Разбиение 
  divide(std::vector<GeoPoint2D>(linestring), result, maxSegmentLength);

  //! Формирование результирующей ломаной
  linestring.clear();
  linestring.swap(result);
}
//==============================================================================
void GeodesicLineTools::divide(
  const GeoLinestring2D& inLinestring, 
  GeoLinestring2D& outLinestring, 
  double maxSegmentLength /*= DEFAULT_MAX_SEGMENT_LENGTH*/) const
{
  //! Инициализируем результат исходной ломаной
  outLinestring = inLinestring;

  //! Разбиваем  
  divide(outLinestring, maxSegmentLength);
}
//==============================================================================
void GeodesicLineTools::divide(
  GeoRing2D& ring, 
  double maxSegmentLength /*= DEFAULT_MAX_SEGMENT_LENGTH*/) const
{
  //! Инициализация результата разбиения
  std::vector<GeoPoint2D> result;

  //! Разбиение 
  divide(std::vector<GeoPoint2D>(ring), result, maxSegmentLength);

  //! Формирование результирующего кольца
  ring.clear();
  ring.swap(result);
}
//==============================================================================
void GeodesicLineTools::divide(
  const GeoRing2D& inRing, GeoRing2D& outRing, 
  double maxSegmentLength /*= DEFAULT_MAX_SEGMENT_LENGTH*/) const
{
  //! Инициализируем результат исходным кольцом
  outRing = inRing;

  //! Разбиваем  
  divide(outRing, maxSegmentLength);
}
//==============================================================================
void GeodesicLineTools::divide(
  GeoPolygon2D& polygon, 
  double maxSegmentLength /*= DEFAULT_MAX_SEGMENT_LENGTH*/) const
{
  //! Разбиение внешнего кольца полигона
  divide(polygon.outer(), maxSegmentLength);

  //! Разбиение внутренних колец
  for (auto& inner : polygon.inners())
    divide(inner, maxSegmentLength);
}
//==============================================================================
void GeodesicLineTools::divide(
  const GeoPolygon2D& inPolygon, 
  GeoPolygon2D& outPolygon, 
  double maxSegmentLength /*= DEFAULT_MAX_SEGMENT_LENGTH*/) const
{
  //! Инициализируем результат исходным полигоном
  outPolygon = inPolygon;

  //! Разбиваем  
  divide(outPolygon, maxSegmentLength);
}
//==============================================================================
void GeodesicLineTools::divide(
  GeoMultiPolygon2D& multiPolygon, 
  double maxSegmentLength /*= DEFAULT_MAX_SEGMENT_LENGTH*/) const
{
  //! Разбиваем по полигонам
  for (auto& polygon : multiPolygon)
    divide(polygon, maxSegmentLength);
}
//==============================================================================
void GeodesicLineTools::divide(
  const GeoMultiPolygon2D& inMultiPolygon, 
  GeoMultiPolygon2D& outMultiPolygon, 
  double maxSegmentLength /*= DEFAULT_MAX_SEGMENT_LENGTH*/) const
{
  //! Инициализируем результат исходным мультиполигоном
  outMultiPolygon = inMultiPolygon;

  //! Разбиваем  
  divide(outMultiPolygon, maxSegmentLength);
}
//==============================================================================
} //! Inner
} //! Geodesic
//==============================================================================
