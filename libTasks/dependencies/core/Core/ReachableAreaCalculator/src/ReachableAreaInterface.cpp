#include <ReachableArea/ReachableAreaInterface.hpp>

#include <ReachableArea/ReachableAreaException.hpp>
#include <ReachableAreaCalculator.hpp>
//! MathTools
#include <MathTools/BasicMath/Compare.hpp>
#include <MathTools/Constants.hpp>
//! PrimaryEntities
#include <PrimaryEntities/GeometryEntities.hpp>
//! AbstractOperations
#include <AbstractOperations/Algorithms/Correct.hpp>
#include <AbstractOperations/Algorithms/CoveredBy.hpp>
#include <AbstractOperations/Algorithms/Difference.hpp>
#include <AbstractOperations/Algorithms/Intersection.hpp>
#include <AbstractOperations/Algorithms/Simplify.hpp>
#include <AbstractOperations/Algorithms/Within.hpp>
#include <AbstractOperations/Normalize.hpp>
//! Geometry
#include <Geometry/Operations/PolygonToCircles.hpp>
//! Geodesic
#include <Geodesic/Projection/Orthographic/Orthographic.hpp> 
#include <Geodesic/GeoLineTools/OrthodromyTools.hpp>
//! std
#include <thread>
//==============================================================================
namespace ReachableArea
{
//==============================================================================
Polygon2D calcReachableArea(const Point2D& point, const double range, 
                            const MultiPolygon2D& restrictions, 
                            const Polygon2D& area /*= Polygon2D()*/, 
                            const unsigned int numVerticies /*= 360*/)
{
  //! Проверка дальности
  if ((range < 0) || (BasicMath::isNull(range)))
    throw ReachableAreaFailure(ReachableArea::NotPositiveRange, 
                               "", "calcReachableArea");

  //! Проверка корректности препятствий
  std::string message;
  if ( !AbstractOperations::isCorrect(restrictions, message) )
    throw ReachableAreaFailure(ReachableArea::IncorrectGeometry,
                               message, "calcReachableArea # restrictions");

  //! Проверка корректности района, по которому обрезать
  if ( !AbstractOperations::isCorrect(area, message) )
    throw ReachableAreaFailure(ReachableArea::IncorrectGeometry,
                               message, "calcReachableArea # area");

  //! Проверяем, что точка не попадает в препятствие
  if (AbstractOperations::coveredBy(point, restrictions))
    throw ReachableAreaFailure(ReachableArea::PointInRestriction,
                               "", "calcReachableArea");

  //! Собираем окружение для задачи ГЗДЦ
  ReachableAreaCalculator::Environment environment;
  for (const Polygon2D& polygon : restrictions)
    environment.addPolygon(polygon);

  //! Калькулятор для ГЗДЦ
  ReachableAreaCalculator::ReachableArea calculator;

  //! Инициализация результата расчёта ГЗДЦ
  MultiPolygon2D multiReachableArea;

  //! Определяем количество одновременных потоков
  int threadsCount(std::thread::hardware_concurrency());
  if (threadsCount > 8)
    threadsCount = 8;   //!< достаточно
  else if (threadsCount > 2)
    threadsCount -= 2;  //!< надо делиться
  else
    threadsCount = -1;  //! калькулятор ГЗДЦ сам определиться

  //! Рассчитываем ГЗДЦ
  bool isCorrectReachableArea(true);
  try
  {
    //! Инициализация результата в формате калькулятора ГЗДЦ
    ReachableAreaCalculator::ReachableAreaResult reachableAreaResult;

    //! Вычисление ГЗДЦ
    if (!calculator.calculate(environment, point, range, reachableAreaResult,
      numVerticies, threadsCount))
      return Polygon2D();

    //! Запись расчёта в результат 
    if ( AbstractOperations::isCorrect(reachableAreaResult.area))
      multiReachableArea.emplace_back(reachableAreaResult.area);
    else
      isCorrectReachableArea = false;
  }
  catch (...)
  {
    isCorrectReachableArea = false;
  }

  //! Если ГЗДЦ не построилась дожным образом - строим через окружность
  if (!isCorrectReachableArea)
  {
    try
    {
      //! Не получилось построить идеальную ГЗДЦ - переходим к резервному плану
      //! Строим окружность от начальной точки
      Polygon2D reachableCircle
        = Geometry::toPolygon(Geometry::Circle(point, range * .75),
          360);

      //! Вырезаем препятствия
      AbstractOperations::difference(reachableCircle, restrictions,
        multiReachableArea);
    }
    catch (...)
    {
      ExceptionTools::FailureHandler<ReachableArea::ReachableAreaFailure>(
        "calcReachableArea # calculate");
    }
  }

  //! Обрезаем, если требуется
  if (!area.outer().empty())
  {
    //! Ищем пересечение ГЗДЦ и района
    try
    {
      AbstractOperations::intersection(multiReachableArea, area,
                                     multiReachableArea);
    }
    catch (...)
    {
      ExceptionTools::FailureHandler<ReachableArea::ReachableAreaFailure>(
                                                    "calcReachableArea # cut");
    }
  }

  //! Ищем область, в которую попадает точка отсчёта ГЗДЦ
  Polygon2D result;
  for (auto polygon : multiReachableArea)
  {
    if ( AbstractOperations::coveredBy(point, polygon))
    {
      result = polygon;
      break;
    }
  }

  //! Проверяем на корректность и корректируем при необходимости
  //! TODO: отключить при нахождении альтернативного решения
  if (!AbstractOperations::correct(result))
  { //!< если correct не помог - упрощаем с точностью до метра, т.к. работаем
    //!  на плоскости и такая точность допустима
    Polygon2D preSimpleArea(result);
    result.clear();
    try 
    {
      AbstractOperations::simplify(preSimpleArea, result, 1);
    }
    catch ( std::exception& ex )
    {
      throw ReachableAreaFailure(ReachableArea::IncorrectGeometry,
                                 std::string( ex.what() ), 
                                 "calcReachableArea # simplify");
    }
    catch( ... )
    {
      throw ReachableAreaFailure( ReachableArea::IncorrectGeometry,
                                  "", "calcReachableArea # simplify" );
    }
    //! Проверяем, что упрощение отработало
    std::string message;
    if ( !AbstractOperations::correct(result, message) )
      throw ReachableAreaFailure(ReachableArea::IncorrectGeometry,
                                 message, "calcReachableArea # result");
  }

  //! Возвращаем результат
  return result;
}
//==============================================================================
using AbstractOperations::normalize;
using AbstractOperations::isNeedToBeNormalized;

GeoPolygon2D calcReachableArea(const GeoPoint2D& point, const double range, 
                               const GeoMultiPolygon2D& restrictions,
                               const GeoPolygon2D& area /*= GeoPolygon2D()*/,
                               const bool cutByOrthodromy /*= false*/,
                               const unsigned int numVerticies /*= 360*/)
{
  //! Собираем ортографическую проекцию с центром в заданной точке
  Geodesic::OrthographicProjection projection(point);

  //! Переводим точку в проекцию
  Point2D start(projection.toProjection(point));

  //! Для обрезки по локсодромии нужно проверить район на корректность
  //! По ортодромии он проверится в проекции
  std::string message;
  if (!cutByOrthodromy)
  {
    //! Проверка корректности района, по которому обрезать
    message.clear();
    if (!AbstractOperations::isCorrect(area, message))
      throw ReachableAreaFailure(ReachableArea::IncorrectGeometry,
                                 message, "calcReachableArea # area");
  }

  //! Проверяем корректность препятствий
  message.clear();
  if (!AbstractOperations::isCorrect(restrictions, message))
    throw ReachableAreaFailure(ReachableArea::IncorrectGeometry,
                               message, "calcReachableArea # restriction");

  //! Перепроецируем район, по которому обрезаем
  Polygon2D areaProj;
  if ((!area.outer().empty()) && cutByOrthodromy)
  {
    //! Инструмент для ортодромий
    Geodesic::OrthodromyTools ortodromyTools;

    //! Разбиваем район на ортодромии
    GeoPolygon2D divideArea;
    ortodromyTools.divide(area, divideArea, 10000.);

    //! Перепроецируем район (используем мультиполигон и забираем единственный
    //! из него полигон)
    areaProj = projection.toProjection<GeoPoint2D, Point2D>( { {divideArea} } )
                                                                      .front();
    AbstractOperations::correct(areaProj);
  }

  //! Готовим препятствия на плоскости
  MultiPolygon2D restrictionsProj(
    projection.toProjection<GeoPoint2D, Point2D>(restrictions));

  //! Вычисляем ГЗДЦ на плоскости
  Polygon2D resultProj = calcReachableArea(start, range, restrictionsProj, 
                                           areaProj, numVerticies);
  
  //! Выполняем обратное перепроецирование через мультиполигон и забирем из него
  //! один единственный
  GeoPolygon2D result
    = projection.fromProjection<Point2D, GeoPoint2D>({ resultProj }).front();

  //! Нормализация
  bool isNeedNormalize =    isNeedToBeNormalized(result) 
                         && isNeedToBeNormalized(area);
  if (isNeedNormalize)
    normalize(result, NORMAL);

  //! Обрезка по району
  //! Район должен быть не пустым и обрезка по локсодромии
  if ((!area.outer().empty()) && (!cutByOrthodromy))
  {
    GeoMultiPolygon2D multiResult;
    try
    {
      AbstractOperations::simplify( { result }, multiResult, ACCURACY_RADIAN);
    }
    catch ( std::exception& ex )
    {
      throw ReachableAreaFailure(ReachableArea::IncorrectGeometry,
                                 std::string( ex.what( ) ), 
                                 "calcReachableArea # simplify");
    }
    catch( ... )
    {
      throw ReachableAreaFailure( ReachableArea::IncorrectGeometry,
                                  "", "calcReachableArea # simplify" );
    }
    //! Обрезаем по району
    GeoPolygon2D cutArea(area);
    GeoPoint2D startPoint(point);
    if (isNeedNormalize)
    {
      normalize(cutArea, NORMAL);
      normalize(startPoint, NORMAL);
    }
    try
    {
      AbstractOperations::intersection(multiResult, cutArea, multiResult);
    }
    catch (...)
    {
      ExceptionTools::FailureHandler<ReachableArea::ReachableAreaFailure>(
                                                    "calcReachableArea # cut");
    }
    //! Ищем полигон в который попадает инициализирующая точка
    result.clear();
    try
    {
      for (auto polygon : multiResult)
      {
        if (AbstractOperations::coveredBy(startPoint, polygon))
        { 
          result = polygon;
          break;
        }
      }
    }
    catch (...)
    {
      ExceptionTools::FailureHandler<ReachableArea::ReachableAreaFailure>(
                                         "calcReachableArea # covered by area");
    }
  }

  //! Проверяем на корректность и корректируем при необходимости
  if ( !AbstractOperations::correct(result) )
  { //!< если correct не помог - упрощаем с точностью около метра в радианах
    GeoPolygon2D preSimpleArea(result);
    result.clear();
    try
    {
      AbstractOperations::simplify(preSimpleArea, result, ACCURACY_RADIAN);
    }
    catch ( std::exception& ex )
    {
      throw ReachableAreaFailure(ReachableArea::IncorrectGeometry,
                                 std::string( ex.what( ) ), 
                                 "calcReachableArea # simplify");
    }
    catch( ... )
    {
      throw ReachableAreaFailure( ReachableArea::IncorrectGeometry,
                                  "", "calcReachableArea # simplify" );
    }
    //! Проверяем, что упрощение отработало
    message.clear();
    if ( !AbstractOperations::correct(result, message) )
      throw ReachableAreaFailure(ReachableArea::IncorrectGeometry,
                                 message, "calcReachableArea # result");
  }
  if (isNeedNormalize)
    normalize(result, REGULAR);

  //! Возвращаем результат
  return result;
}
//==============================================================================
} //! ReachableArea
//==============================================================================