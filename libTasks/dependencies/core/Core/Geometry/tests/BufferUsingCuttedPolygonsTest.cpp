#include <Geometry/Operations/BufferUsingCuttedPolygons.hpp>
//! IOTools
#include <IOTools/GeoJsonIO/GeoJsonIO.hpp>
//! gtest
#include <gtest/gtest.h>
//==============================================================================
namespace Geometry
{
namespace Tests
{
//==============================================================================
  //TEST(BufferUsingCuttedPolygonsTest, getBufferUsingCuttedPolygons)
  //{
  //  Polygon2D square({ {
  //    Point2D(0., 0.), Point2D(0., 1.),
  //    Point2D(1., 1.), Point2D(1., 0.),
  //    Point2D(0., 0.) } });

  //  Polygon2D star({ {
  //    Point2D(2., 1.), Point2D(3., 3.),
  //    Point2D(1., 4.), Point2D(3., 5.),
  //    Point2D(4., 7.), Point2D(5., 5.),
  //    Point2D(7., 4.), Point2D(5., 3.),
  //    Point2D(6., 1.), Point2D(4., 2.),
  //    Point2D(2., 1.) } });

  //  Polygon2D bow({ {
  //    Point2D(-7., -1.),Point2D(-4., -2.),
  //    Point2D(-1., -1.), Point2D(-1., -5.),
  //    Point2D(-4., -4.), Point2D(-7., -5.),
  //    Point2D(-7., -1.) } });

  //  // Зададим параметры буфера
  //  double distance(1);
  //  int quadrantSegments(0); // для квадратного буфера значения не имеет
  //  geos::operation::buffer::BufferParameters::EndCapStyle endCapStyle(
  //                      geos::operation::buffer::BufferParameters::CAP_SQUARE);
  //  geos::operation::buffer::BufferParameters::JoinStyle joinStyle(
  //                      geos::operation::buffer::BufferParameters::JOIN_MITRE);
  //  // значение по-умолчанию, взятое из geos (DEFAULT_MITRE_LIMIT)
  //  double mitreLimit(5.);

  //  // Проверим, что в результате отработки буфера площадь геометрии становится
  //  // больше
  //  // Квадрат
  //  MultiPolygon2D cuttedBuffResult
  //    = getBufferUsingCuttedPolygons({ square }, distance, quadrantSegments,
  //                                   endCapStyle, joinStyle, mitreLimit);

  //  ASSERT_LT(AbstractOperations::area(square),
  //            AbstractOperations::area(cuttedBuffResult));

  //  // Звезда
  //  cuttedBuffResult
  //    = getBufferUsingCuttedPolygons({ star }, distance, quadrantSegments,
  //                                   endCapStyle, joinStyle, mitreLimit);

  //  ASSERT_LT(AbstractOperations::area(star),
  //            AbstractOperations::area(cuttedBuffResult));

  //  // Бантик (корректный)
  //  cuttedBuffResult
  //    = getBufferUsingCuttedPolygons({ bow }, distance, quadrantSegments,
  //                                   endCapStyle, joinStyle, mitreLimit);

  //  ASSERT_LT(AbstractOperations::area(bow),
  //            AbstractOperations::area(cuttedBuffResult));

  //  // Буфер из boost 1.63 не справляется с данным примером, поэтому применение
  //  // буфера через нарезку полигона необходимо, boost 1.74 справляется сразу
  //  // корректно

  //  // Прочитаем геометрию, на которой обычный буфер работает некорретно
  //  Polygon2D inputGeom = importPolygonFromGeoJson(
  //    "BufferUsingCuttedPolygonTestRes/polyIncorrForBoostBuf.geojson");

  //  double distance2(0.00001);

  //  // Построим буфером с нарезкой
  //  cuttedBuffResult
  //    = getBufferUsingCuttedPolygons({ inputGeom }, distance2,
  //                                   quadrantSegments, endCapStyle,
  //                                   joinStyle, mitreLimit);

  //  // Убедимся, что его результат корректен (площадь увеличилась)
  //  ASSERT_LT(AbstractOperations::area(inputGeom),
  //            AbstractOperations::area(cuttedBuffResult));

  //  // Пример, который требует увеличения буфера для получения корректного
  //  // контура
  //  inputGeom = importPolygonFromGeoJson(
  //    "BufferUsingCuttedPolygonTestRes/polyToIncreaseBufDist.geojson");

  //  // Построим буфером с нарезкой
  //  cuttedBuffResult
  //    = getBufferUsingCuttedPolygons({ inputGeom }, distance2,
  //                                   quadrantSegments, endCapStyle,
  //                                   joinStyle, mitreLimit);

  //  // Убедимся, что его результат корректен (площадь увеличилась)
  //  ASSERT_LT(AbstractOperations::area(inputGeom),
  //            AbstractOperations::area(cuttedBuffResult));
  //}
//==============================================================================
} // ! Tests
} // ! Geometry