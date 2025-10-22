#include <SamplesForTests/BaseGeoFigures.hpp>
#include <SamplesForTests/SamplesForTestsExceptions.hpp>
//! PrimaryEntities
#include <PrimaryEntities/GeometryConverter.hpp>
#include <PrimaryEntities/Equalities.hpp>
//! AbstractOperations
#include <AbstractOperations/Algorithms/Correct.hpp>
//! gtest
#include <gtest/gtest.h>
//==============================================================================
namespace SamplesForTests
{
namespace Tests
{
//==============================================================================
TEST (BaseFiguresTest, SquareTest)
{
  GeoPolygon2D square;

  // обычный квадрат
  square = buildGSquare(GeoPoint2D(Degree(30.), Degree(170.)), Degree(20.));
  ASSERT_EQ(square.outer().size(), 5);
  ASSERT_EQ(square.outer().at(0), GeoPoint2D(Degree(40.), Degree(180.)));
  ASSERT_EQ(square.outer().at(1), GeoPoint2D(Degree(20.), Degree(180.)));
  ASSERT_EQ(square.outer().at(2), GeoPoint2D(Degree(20.), Degree(160.)));
  ASSERT_EQ(square.outer().at(3), GeoPoint2D(Degree(40.), Degree(160.)));
  ASSERT_EQ(square.outer().at(4), GeoPoint2D(Degree(40.), Degree(180.)));

  //  квадрат с отрицательной стороной генерирует исключение
  ASSERT_THROW(buildGSquare(GeoPoint2D(Degree(30.), Degree(170.)),
                            Degree(-20.)), SamplesForTestsFailure);
  try
  {
    square = buildGSquare(GeoPoint2D(Degree(30.), Degree(170.)), Degree(-20.));
  }
  catch (const SamplesForTestsFailure& exception)
  {
    ASSERT_EQ(exception.getCode(), NegativeDistance);
    ASSERT_EQ(sampForTestsLibTitle, exception.getLib());
  }

  // квадрат со стороной 0 - это пустой полигон 
  square = buildGSquare(GeoPoint2D(Degree(30.), Degree(170.)), Degree(0.));
  ASSERT_EQ(square.outer().size(), 0);
}
//==============================================================================
TEST (BaseFiguresTest, RectangleTest)
{
  GeoPolygon2D rectangle;

  // обычный прямоугольник
  rectangle = buildGeoRectangle(GeoPoint2D(Degree(20.), Degree(100.)),
                                           Degree(10.), Degree(20.));
  ASSERT_EQ(rectangle.outer().size(), 5);
  ASSERT_EQ(rectangle.outer().at(0), GeoPoint2D(Degree(20.), Degree(100.)));
  ASSERT_EQ(rectangle.outer().at(1), GeoPoint2D(Degree(30.), Degree(100.)));
  ASSERT_EQ(rectangle.outer().at(2), GeoPoint2D(Degree(30.), Degree(120.)));
  ASSERT_EQ(rectangle.outer().at(3), GeoPoint2D(Degree(20.), Degree(120.)));
  ASSERT_EQ(rectangle.outer().at(4), GeoPoint2D(Degree(20.), Degree(100.)));

  // прямоугольник с отрицательной стороной по долготе генерирует исключение
  ASSERT_THROW(buildGeoRectangle(GeoPoint2D(Degree(20.), Degree(100.)),
                                 Degree(10.), Degree(-20.)),
               SamplesForTestsFailure);
  try
  {
    rectangle = buildGeoRectangle(GeoPoint2D(Degree(20.), Degree(100.)),
                                             Degree(10.), Degree(-20.));
  }
  catch (const SamplesForTestsFailure& exception)
  {
    ASSERT_EQ(exception.getCode(), NegativeDistance);
    ASSERT_EQ(sampForTestsLibTitle, exception.getLib());
  }
  // прямоугольник с отрицательной стороной по широте генерирует исключение
  ASSERT_THROW(buildGeoRectangle(GeoPoint2D(Degree(20.), Degree(100.)),
                                 Degree(-10.), Degree(20.)),
               SamplesForTestsFailure);
  try
  {
    rectangle = buildGeoRectangle(GeoPoint2D(Degree(20.), Degree(100.)),
                                             Degree(-10.), Degree(20.));
  }
  catch (const SamplesForTestsFailure& exception)
  {
    ASSERT_EQ(exception.getCode(), NegativeDistance);
    ASSERT_EQ(sampForTestsLibTitle, exception.getLib());
  }
  // прямоугольник со стороной 0 по долготе - это пустой полигон 
  rectangle = buildGeoRectangle(GeoPoint2D(Degree(20.), Degree(100.)),
                                           Degree(10.), Degree(0.));
  ASSERT_EQ(rectangle.outer().size(), 0);

  // прямоугольник со стороной 0 по широте - это пустой полигон 
  rectangle = buildGeoRectangle(GeoPoint2D(Degree(20.), Degree(100.)),
                                           Degree(0.), Degree(20.));
  ASSERT_EQ(rectangle.outer().size(), 0);
}
//==============================================================================
TEST(BaseFiguresTest, RectangleNewTest)
{

  /* прямоугольник, который принимает размеры сторон в double и 
   * использует для этого функцию построения обычного прямоугольника
  */ 
  GeoPoint2D point(Degree(20.), Degree(100.));
  GeoPolygon2D rectangle;
  rectangle = buildGeoRectangle(point, Degree(10.), Degree(20.));
  GeoPolygon2D rectangleNew;
  rectangleNew = buildGeoRectangle(point, 10., 20.);

  ASSERT_EQ(rectangle.outer(), rectangleNew.outer());

  // прямоугольник с отрицательной стороной по долготе генерирует исключение
  ASSERT_THROW(buildGeoRectangle(GeoPoint2D(Degree(20.), Degree(100.)),
                                 10., -20.), SamplesForTestsFailure);
  try
  {
    rectangleNew = buildGeoRectangle(GeoPoint2D(Degree(20.), Degree(100.)),
                                                             10., -20.);
  }
  catch (const SamplesForTestsFailure& exception)
  {
    ASSERT_EQ(exception.getCode(), NegativeDistance);
    ASSERT_EQ(sampForTestsLibTitle, exception.getLib());
  }
  // прямоугольник с отрицательной стороной по широте генерирует исключение
  ASSERT_THROW(buildGeoRectangle(GeoPoint2D(Degree(20.), Degree(100.)),
                                 -10., 20.), SamplesForTestsFailure);
  try
  {
    rectangleNew = buildGeoRectangle(GeoPoint2D(Degree(20.), Degree(100.)),
                                                             -10., 20.);
  }
  catch (const SamplesForTestsFailure& exception)
  {
    ASSERT_EQ(exception.getCode(), NegativeDistance);
    ASSERT_EQ(sampForTestsLibTitle, exception.getLib());
  }
  // прямоугольник со стороной 0 по долготе - это пустой полигон 
  rectangleNew = buildGeoRectangle(GeoPoint2D(Degree(20.), Degree(100.)),
                                                             10., 0.);
  ASSERT_EQ(rectangleNew.outer().size(), 0);

  // прямоугольник со стороной 0 по широте - это пустой полигон 
  rectangleNew = buildGeoRectangle(GeoPoint2D(Degree(20.), Degree(100.)),
                                                             0., 20.);
  ASSERT_EQ(rectangleNew.outer().size(), 0);
}
//==============================================================================
TEST (BaseFiguresTest, HoleRectangleTest)
{
  GeoPolygon2D holeRectangle;
  holeRectangle = buildGeoHoleRectangle(GeoPoint2D(Degree(70.), Degree(120.)),
                                        GeoPoint2D(Degree(75.), Degree(127.5)),
                                        Degree(20.), Degree(30.),
                                        Degree(10.), Degree(15.));
  // прямоугольник с дыркой
  ASSERT_EQ(holeRectangle.outer().size(), 5);
  ASSERT_EQ(holeRectangle.outer().at(0), GeoPoint2D(Degree(70.), Degree(120.)));
  ASSERT_EQ(holeRectangle.outer().at(1), GeoPoint2D(Degree(90.), Degree(120.)));
  ASSERT_EQ(holeRectangle.outer().at(2), GeoPoint2D(Degree(90.), Degree(150.)));
  ASSERT_EQ(holeRectangle.outer().at(3), GeoPoint2D(Degree(70.), Degree(150.)));
  ASSERT_EQ(holeRectangle.outer().at(4), GeoPoint2D(Degree(70.), Degree(120.)));
  ASSERT_EQ(holeRectangle.inners()[0].size(), 5);
  ASSERT_EQ(holeRectangle.inners().at(0).at(0), GeoPoint2D(Degree(75.), 
                                                Degree(127.5)));
  ASSERT_EQ(holeRectangle.inners().at(0).at(1), GeoPoint2D(Degree(75.),
                                                Degree(142.5)));
  ASSERT_EQ(holeRectangle.inners().at(0).at(2), GeoPoint2D(Degree(85.),
                                                Degree(142.5)));
  ASSERT_EQ(holeRectangle.inners().at(0).at(3), GeoPoint2D(Degree(85.),
                                                Degree(127.5)));
  ASSERT_EQ(holeRectangle.inners().at(0).at(4), GeoPoint2D(Degree(75.),
                                                Degree(127.5)));

  // прямоугольник с отрицательной стороной по долготе генерирует исключение
  ASSERT_THROW(buildGeoHoleRectangle(GeoPoint2D(Degree(70.), Degree(120.)),
                                     GeoPoint2D(Degree(75.), Degree(127.5)),
                                                Degree(20.), Degree(-30.),
                                                Degree(10.), Degree(15.)),
               SamplesForTestsFailure);
  try
  {
    holeRectangle = buildGeoHoleRectangle(GeoPoint2D(Degree(70.), Degree(120.)),
                                          GeoPoint2D(Degree(75.), Degree(127.5)),
                                                     Degree(20.), Degree(-30.),
                                                     Degree(10.), Degree(15.));
  }
  catch (const SamplesForTestsFailure& exception)
  {
    ASSERT_EQ(exception.getCode(), NegativeDistance);
    ASSERT_EQ(sampForTestsLibTitle, exception.getLib());
  }
  // прямоугольник с отрицательной стороной по широте генерирует исключение
  ASSERT_THROW(buildGeoHoleRectangle(GeoPoint2D(Degree(70.), Degree(120.)),
                                     GeoPoint2D(Degree(75.), Degree(127.5)),
                                                Degree(-20.), Degree(30.),
                                                Degree(10.), Degree(15.)),
               SamplesForTestsFailure);
  try
  {
    holeRectangle = buildGeoHoleRectangle(GeoPoint2D(Degree(70.), Degree(120.)),
                                          GeoPoint2D(Degree(75.), Degree(127.5)),
                                                     Degree(-20.), Degree(30.),
                                                     Degree(10.), Degree(15.));
  }
  catch (const SamplesForTestsFailure& exception)
  {
    ASSERT_EQ(exception.getCode(), NegativeDistance);
    ASSERT_EQ(sampForTestsLibTitle, exception.getLib());
  }

  // дырка с отрицательной стороной по долготе генерирует исключение
  ASSERT_THROW(buildGeoHoleRectangle(GeoPoint2D(Degree(70.), Degree(120.)),
                                     GeoPoint2D(Degree(75.), Degree(127.5)),
                                                Degree(20.), Degree(30.),
                                                Degree(10.), Degree(-15.)),
               SamplesForTestsFailure);
  try
  {
    holeRectangle = buildGeoHoleRectangle(GeoPoint2D(Degree(70.), Degree(120.)),
                                          GeoPoint2D(Degree(75.), Degree(127.5)),
                                                     Degree(20.), Degree(30.),
                                                     Degree(10.), Degree(-15.));
  }
  catch (const SamplesForTestsFailure& exception)
  {
    ASSERT_EQ(exception.getCode(), NegativeDistance);
    ASSERT_EQ(sampForTestsLibTitle, exception.getLib());
  }
  // дырка с отрицательной стороной по широте генерирует исключение
  ASSERT_THROW(buildGeoHoleRectangle(GeoPoint2D(Degree(70.), Degree(120.)),
                                     GeoPoint2D(Degree(75.), Degree(127.5)),
                                                Degree(20.), Degree(30.),
                                                Degree(-10.), Degree(15.)),
               SamplesForTestsFailure);
  try
  {
    holeRectangle = buildGeoHoleRectangle(GeoPoint2D(Degree(70.), Degree(120.)),
                                          GeoPoint2D(Degree(75.), Degree(127.5)),
                                                     Degree(20.), Degree(30.),
                                                     Degree(-10.), Degree(15.));
  }
  catch (const SamplesForTestsFailure& exception)
  {
    ASSERT_EQ(exception.getCode(), NegativeDistance);
    ASSERT_EQ(sampForTestsLibTitle, exception.getLib());
  }
  // прямоугольник со стороной 0 по долготе - это пустой полигон 
  holeRectangle = buildGeoHoleRectangle(GeoPoint2D(Degree(70.), Degree(120.)),
                                        GeoPoint2D(Degree(75.), Degree(127.5)),
                                                   Degree(20.), Degree(0.),
                                                   Degree(10.), Degree(15.));
  ASSERT_EQ(holeRectangle.outer().size(), 0);

  // прямоугольник со стороной 0 по широте - это пустой полигон 
  holeRectangle = buildGeoHoleRectangle(GeoPoint2D(Degree(70.), Degree(120.)),
                                        GeoPoint2D(Degree(75.), Degree(127.5)),
                                                   Degree(0.), Degree(30.),
                                                   Degree(10.), Degree(15.));
  ASSERT_EQ(holeRectangle.outer().size(), 0);

  // дырка со стороной 0 по долготе - это полигон с пустой дыркой
  holeRectangle = buildGeoHoleRectangle(GeoPoint2D(Degree(70.), Degree(120.)),
                                        GeoPoint2D(Degree(75.), Degree(127.5)),
                                                   Degree(20.), Degree(30.),
                                                   Degree(10.), Degree(0.));
  ASSERT_EQ(holeRectangle.inners().size(), 0);

  // дырка со стороной 0 по широте - это полигон с пустой дыркой
  holeRectangle = buildGeoHoleRectangle(GeoPoint2D(Degree(70.), Degree(120.)),
                                        GeoPoint2D(Degree(75.), Degree(127.5)),
                                                   Degree(20.), Degree(30.),
                                                   Degree(0.), Degree(15.));
  ASSERT_EQ(holeRectangle.inners().size(), 0);
}
//==============================================================================
TEST(BaseFiguresTest, BowTest)
{
  GeoPolygon2D bow;
  bow = buildBow(GeoPoint2D(Degree(60.), Degree(120.)),
                 Degree(10.), Degree(30.));

  // Фигура с самопересечением(бабочка)
  ASSERT_EQ(bow.outer().size(), 5);
  ASSERT_EQ(bow.outer().at(0), GeoPoint2D(Degree(60.), Degree(120.)));
  ASSERT_EQ(bow.outer().at(1), GeoPoint2D(Degree(70.), Degree(120.)));
  ASSERT_EQ(bow.outer().at(2), GeoPoint2D(Degree(60.), Degree(150.)));
  ASSERT_EQ(bow.outer().at(3), GeoPoint2D(Degree(70.), Degree(150.)));
  ASSERT_EQ(bow.outer().at(4), GeoPoint2D(Degree(60.), Degree(120.)));

  std::string message;
  ASSERT_FALSE(AbstractOperations::isCorrect(bow, message));
  ASSERT_EQ(message, "Self-intersection");

  // бабочка с отрицательной стороной по долготе генерирует исключение
  ASSERT_THROW(buildBow(GeoPoint2D(Degree(60.), Degree(120.)),
                        Degree(10.), Degree(-30.)), SamplesForTestsFailure);
  try
  {
    bow = buildBow(GeoPoint2D(Degree(60.), Degree(120.)),
                              Degree(10.), Degree(-30.));
  }
  catch (const SamplesForTestsFailure& exception)
  {
    ASSERT_EQ(exception.getCode(), NegativeDistance);
    ASSERT_EQ(sampForTestsLibTitle, exception.getLib());
  }
  
  // бабочка с отрицательной стороной по широте генерирует исключение
  ASSERT_THROW(buildBow(GeoPoint2D(Degree(60.), Degree(120.)),
                        Degree(-10.), Degree(30.)), SamplesForTestsFailure);
  try
  {
    bow = buildBow(GeoPoint2D(Degree(60.), Degree(120.)),
                              Degree(-10.), Degree(30.));
  }
  catch (const SamplesForTestsFailure& exception)
  {
    ASSERT_EQ(exception.getCode(), NegativeDistance);
    ASSERT_EQ(sampForTestsLibTitle, exception.getLib());
  }
  // бабочка со стороной 0 по долготе - это пустой полигон 
  bow = buildBow(GeoPoint2D(Degree(60.), Degree(120.)),
                            Degree(10.), Degree(0.));
  ASSERT_EQ(bow.outer().size(), 0);
 
  // бабочка со стороной 0 по широте - это пустой полигон 
  bow = buildBow(GeoPoint2D(Degree(60.), Degree(120.)),
                            Degree(0.), Degree(30.));
  ASSERT_EQ(bow.outer().size(), 0);
  
  
}
//==============================================================================
TEST(BaseFiguresTest, PostTest)
{
  GeoPolygon2D post;
  post = buildPost(GeoPoint2D(Degree(70.), Degree(110.)),
                   Degree(20.), Degree(40.));
  // фигура с самокасанием(конвертик* без верхней стороны)
  ASSERT_EQ(post.outer().size(), 6);
  ASSERT_EQ(post.outer().at(0), GeoPoint2D(Degree(70.), Degree(110.)));
  ASSERT_EQ(post.outer().at(1), GeoPoint2D(Degree(90.), Degree(110.)));
  ASSERT_EQ(post.outer().at(2), GeoPoint2D(Degree(70.), Degree(130.)));
  ASSERT_EQ(post.outer().at(3), GeoPoint2D(Degree(90.), Degree(150.)));
  ASSERT_EQ(post.outer().at(4), GeoPoint2D(Degree(70.), Degree(150.)));
  ASSERT_EQ(post.outer().at(5), GeoPoint2D(Degree(70.), Degree(110.)));

  std::string message;
  ASSERT_FALSE(AbstractOperations::isCorrect(post, message));
  ASSERT_EQ(message, "Ring Self-intersection");

  // конвертик* с отрицательной стороной по долготе генерирует исключение
  ASSERT_THROW(buildPost(GeoPoint2D(Degree(70.), Degree(110.)),
                         Degree(20.), Degree(-40.)), SamplesForTestsFailure);
  try
  {
    post = buildPost(GeoPoint2D(Degree(70.), Degree(110.)),
                               Degree(20.), Degree(-40.));
  }
  catch (const SamplesForTestsFailure& exception)
  {
    ASSERT_EQ(exception.getCode(), NegativeDistance);
    ASSERT_EQ(sampForTestsLibTitle, exception.getLib());
  }

  // конвертик* с отрицательной стороной по широте генерирует исключение
  ASSERT_THROW(buildPost(GeoPoint2D(Degree(70.), Degree(110.)),
                         Degree(-20.), Degree(40.)), SamplesForTestsFailure);
  try
  {
    post = buildPost(GeoPoint2D(Degree(70.), Degree(110.)),
                               Degree(-20.), Degree(40.));
  }
  catch (const SamplesForTestsFailure& exception)
  {
    ASSERT_EQ(exception.getCode(), NegativeDistance);
    ASSERT_EQ(sampForTestsLibTitle, exception.getLib());
  }
  // конвертик* со стороной 0 по долготе - это пустой полигон 
  post = buildPost(GeoPoint2D(Degree(70.), Degree(110.)),
                              Degree(20.), Degree(0.));
  ASSERT_EQ(post.outer().size(), 0);

  // конвертик* со стороной 0 по широте - это пустой полигон 
  post = buildPost(GeoPoint2D(Degree(70.), Degree(110.)),
                              Degree(0.), Degree(40.));
  ASSERT_EQ(post.outer().size(), 0);
}
//==============================================================================
TEST(BaseFiguresTest, addHoleTest)
{
  GeoPolygon2D polygon;
  // добавляю прямоугольник
  polygon = buildGeoRectangle(GeoPoint2D(Degree(20.), Degree(100.)), 10., 20.);


  // полигон добавляет 1 дырку
  addHole(polygon, GeoPoint2D(Degree(22.), Degree(105.)), 6., 5.);

  GeoRing2D resInnerRing({ GeoPoint2D(Degree(22.), Degree(105.)),
                           GeoPoint2D(Degree(22.), Degree(110.)),
                           GeoPoint2D(Degree(28.), Degree(110.)),
                           GeoPoint2D(Degree(28.), Degree(105.)),
                           GeoPoint2D(Degree(22.), Degree(105.)) });

  ASSERT_EQ(polygon.inners().size(), 1);
  ASSERT_TRUE(areEqualUpToRotate(polygon.inners().at(0), resInnerRing));

  // полигон добавляет 2 дырку, не удаляя 1 дырку
  addHole(polygon, GeoPoint2D(Degree(22.), Degree(112.)), 6., 6.);

  resInnerRing = GeoRing2D({ GeoPoint2D(Degree(22.), Degree(112.)),
                             GeoPoint2D(Degree(22.), Degree(118.)),
                             GeoPoint2D(Degree(28.), Degree(118.)),
                             GeoPoint2D(Degree(28.), Degree(112.)),
                             GeoPoint2D(Degree(22.), Degree(112.)) });

  ASSERT_EQ(polygon.inners().size(), 2);
  ASSERT_TRUE(areEqualUpToRotate(polygon.inners().at(1), resInnerRing));


  // некорректный полигон кидает ошибку
  GeoPolygon2D polyBow; 
  polyBow = buildBow(GeoPoint2D(Degree(22.), Degree(108.)),
                                Degree(4.),  Degree(14.));

  ASSERT_THROW(addHole(polyBow, GeoPoint2D(Degree(23.),
                                Degree(109.)), 2., 1.),
               SamplesForTestsFailure);

  try
  {
    addHole(polyBow, GeoPoint2D(Degree(23.), Degree(109.)), 2., 1.);
  }
  catch (const SamplesForTestsFailure& exception)
  {
    ASSERT_EQ(exception.getCode(), IncorrectGeometry);
    ASSERT_EQ(sampForTestsLibTitle, exception.getLib());
  }
 

  // если дырка вне полигона, то полигон кидает ошибку
  ASSERT_THROW(addHole(polygon, GeoPoint2D(Degree(10.),
                                Degree(80.)), 6., 5.),
               SamplesForTestsFailure);
  try
  {
    addHole(polygon, GeoPoint2D(Degree(10.), Degree(80.)), 6., 5.);
  }
  catch (SamplesForTestsFailure& exception)
  {
    ASSERT_EQ(exception.getCode(), HoleNotInsidePolygon);
    ASSERT_EQ(sampForTestsLibTitle, exception.getLib());
  }


  // новая дырка при добавлении объединяется с предыдущими
  addHole(polygon, GeoPoint2D(Degree(24.), Degree(109.)), 2., 4.);

  GeoRing2D resOuterRing({ GeoPoint2D(Degree(20.), Degree(100.)),
                           GeoPoint2D(Degree(30.), Degree(100.)),
                           GeoPoint2D(Degree(30.), Degree(120.)),
                           GeoPoint2D(Degree(20.), Degree(120.)),
                           GeoPoint2D(Degree(20.), Degree(100.)) });

  resInnerRing = GeoRing2D({ GeoPoint2D(Degree(24.), Degree(110.)),
                             GeoPoint2D(Degree(24.), Degree(112.)),
                             GeoPoint2D(Degree(22.), Degree(112.)),
                             GeoPoint2D(Degree(22.), Degree(118.)),
                             GeoPoint2D(Degree(28.), Degree(118.)),
                             GeoPoint2D(Degree(28.), Degree(112.)),
                             GeoPoint2D(Degree(26.), Degree(112.)),
                             GeoPoint2D(Degree(26.), Degree(110.)),
                             GeoPoint2D(Degree(28.), Degree(110.)),
                             GeoPoint2D(Degree(28.), Degree(105.)),
                             GeoPoint2D(Degree(22.), Degree(105.)),
                             GeoPoint2D(Degree(22.), Degree(110.)),
                             GeoPoint2D(Degree(24.), Degree(110.)), });

  ASSERT_EQ(polygon.outer().size(), 5);
  ASSERT_TRUE(areEqualUpToRotate(polygon.outer(), resOuterRing));
  ASSERT_EQ(polygon.inners().size(), 1);
  ASSERT_TRUE(areEqualUpToRotate(polygon.inners().at(0), resInnerRing));


  // дырка с отрицательной стороной по долготе генерирует исключение
  ASSERT_THROW(addHole(polygon, GeoPoint2D(Degree(22.),
               Degree(105.)), 6., -5.), SamplesForTestsFailure);
  try
  {
    addHole(polygon, GeoPoint2D(Degree(22.),
                     Degree(105.)), 6., -5.);
  }
  catch (const SamplesForTestsFailure& exception)
  {
    ASSERT_EQ(exception.getCode(), NegativeDistance);
    ASSERT_EQ(sampForTestsLibTitle, exception.getLib());
  }

  // дырка с отрицательной стороной по широте генерирует исключение
  ASSERT_THROW(addHole(polygon, GeoPoint2D(Degree(22.),
               Degree(105.)), -6., 5.), SamplesForTestsFailure);
  try
  {
    addHole(polygon, GeoPoint2D(Degree(22.),
                     Degree(105.)), -6., 5.);
  }
  catch (const SamplesForTestsFailure& exception)
  {
    ASSERT_EQ(exception.getCode(), NegativeDistance);
    ASSERT_EQ(sampForTestsLibTitle, exception.getLib());
  }

  // дырка со стороной 0 по долготе - это пустой полигон 
  addHole(polygon, GeoPoint2D(Degree(22.), Degree(105.)), 6., 0.);
  ASSERT_EQ(polygon.inners().size(), 1);

  // дырка со стороной 0 по широте - это пустой полигон 
  addHole(polygon, GeoPoint2D(Degree(22.), Degree(105.)), 0., 5.);
  ASSERT_EQ(polygon.inners().size(), 1);
}
//==============================================================================
} //! Tests 
}//! SamplesForTests
//==============================================================================