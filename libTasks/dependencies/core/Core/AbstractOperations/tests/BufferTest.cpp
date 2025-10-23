#include <AbstractOperations/Algorithms/Buffer.hpp>

#include <AbstractOperations/Algorithms/CoveredBy.hpp>
//! PrimaryEntities
#include <PrimaryEntities/Entities/Box.hpp>
#include <PrimaryEntities/Equalities.hpp>
//! gtest
#include <gtest/gtest.h>
//==============================================================================
//! Тесты для проверки правильности работы буфера
//==============================================================================
namespace AbstractOperations
{
namespace Tests
{
//==============================================================================
TEST(BufferTest, TemplateBuffer)
{
  MultiPolygon2D result;

  MultiPolygon2D testResult({ {{Point2D(-1., 2), Point2D(2., 2.),
                                Point2D(2., -1.), Point2D(-1., -1.),
                                Point2D(-1., 2)} } });

  // Квадрат 
  Polygon2D polygon;
  polygon.outer().push_back(Point2D(0., 0.));
  polygon.outer().push_back(Point2D(0., 1.));
  polygon.outer().push_back(Point2D(1., 1.));
  polygon.outer().push_back(Point2D(1., 0.));
  polygon.outer().push_back(Point2D(0., 0.));

  double distance(1.);
  int quadrantSegments
    = geos::operation::buffer::BufferParameters::DEFAULT_QUADRANT_SEGMENTS;
  geos::operation::buffer::BufferParameters::EndCapStyle endCapStyle
    = geos::operation::buffer::BufferParameters::CAP_SQUARE;
  geos::operation::buffer::BufferParameters::JoinStyle joinStyle
    = geos::operation::buffer::BufferParameters::JOIN_MITRE;
  // значение по-умолчанию, взятое из geos (DEFAULT_MITRE_LIMIT)
  double mitreLimit = 5.;

  Helper::buffer(
    polygon,
    result,
    distance,
    quadrantSegments,
    endCapStyle,
    joinStyle,
    mitreLimit);

  ASSERT_EQ(1, result.size());
  ASSERT_EQ(5, result[0].outer().size());
  ASSERT_TRUE(areEqualUpToRotate(result[0].outer(), testResult[0].outer()));

  //! Бантик с самопересечением
  polygon.clear();
  polygon.outer().push_back(Point2D(0., 0.));
  polygon.outer().push_back(Point2D(1., 1.));
  polygon.outer().push_back(Point2D(1., -1.));
  polygon.outer().push_back(Point2D(0., 0.));
  polygon.outer().push_back(Point2D(-1., 1.));
  polygon.outer().push_back(Point2D(-1., -1.));
  polygon.outer().push_back(Point2D(0., 0.));

  ASSERT_THROW(Helper::buffer(polygon, result, distance, quadrantSegments,
                              endCapStyle, joinStyle, mitreLimit),
               AbstractOperationsFailure);
  try
  {
    Helper::buffer(
      polygon,
      result,
      distance,
      quadrantSegments,
      endCapStyle,
      joinStyle,
      mitreLimit);
  }
  catch (const AbstractOperationsFailure& failure)
  {
    ASSERT_EQ(IncorrectGeometry, failure.getCode());
    ASSERT_EQ(abstrLibTitle, failure.getLib());
  }

  //! Самокасание
  polygon.clear();
  polygon.outer().push_back(Point2D(0., 0.));
  polygon.outer().push_back(Point2D(0., 1.));
  polygon.outer().push_back(Point2D(1., 0.));
  polygon.outer().push_back(Point2D(2., 1.));
  polygon.outer().push_back(Point2D(2., 0.));
  polygon.outer().push_back(Point2D(0., 0.));

  ASSERT_THROW(Helper::buffer(polygon, result, distance, quadrantSegments,
                              endCapStyle, joinStyle, mitreLimit),
               AbstractOperationsFailure);
  try
  {
    Helper::buffer(
      polygon,
      result,
      distance,
      quadrantSegments,
      endCapStyle,
      joinStyle,
      mitreLimit);
  }
  catch (const AbstractOperationsFailure& failure)
  {
    ASSERT_EQ(IncorrectGeometry, failure.getCode());
    ASSERT_EQ( abstrLibTitle, failure.getLib());
  }

  /*!
   * На этих данных boost::geometry::buffer создаёт лишние вершины,
   * нарушающие корректность результата,
   * которые должны удаляться нашей функцией.
   */
  distance = (ACCURACY_RADIAN * 100.);
  GeoPolygon2D geoPolygon2D(
    GeoBox2D(
      GeoPoint2D(Degree(37.166666667), Degree(32.75)),
      GeoPoint2D(Degree(37.333333333), Degree(33.)) ).toPolygon() );
  GeoMultiPolygon2D geoResult;
  ASSERT_NO_THROW(
    Helper::buffer(
      geoPolygon2D,
      geoResult,
      distance,
      quadrantSegments,
      endCapStyle,
      joinStyle,
      mitreLimit));
  ASSERT_EQ(1, geoResult.size());
  ASSERT_EQ(5, geoResult[0].outer().size());
  ASSERT_TRUE(geoResult[0].inners().empty());
}
//==============================================================================
TEST(BufferTest, SquareBuffer)
{
  MultiPolygon2D result;

  MultiPolygon2D testResult({ {{Point2D(-1., 2), Point2D(2., 2.),
                                Point2D(2., -1.), Point2D(-1., -1.),
                                Point2D(-1., 2)} } });

  Polygon2D polygon;
  polygon.outer().push_back(Point2D(0., 0.));
  polygon.outer().push_back(Point2D(0., 1.));
  polygon.outer().push_back(Point2D(1., 1.));
  polygon.outer().push_back(Point2D(1., 0.));
  polygon.outer().push_back(Point2D(0., 0.));

  bufferSquare(polygon, result, 1.);

  ASSERT_EQ(1, result.size());
  ASSERT_EQ(5, result[0].outer().size());
  ASSERT_TRUE(areEqualUpToRotate(result[0].outer(), testResult[0].outer()));
}
//==============================================================================
TEST(BufferTest, RoundBuffer)
{
  MultiPolygon2D result;

  //! Полигон
  Polygon2D polygon;
  polygon.outer().push_back(Point2D(0., 0.));
  polygon.outer().push_back(Point2D(0., 1.));
  polygon.outer().push_back(Point2D(1., 1.));
  polygon.outer().push_back(Point2D(1., 0.));
  polygon.outer().push_back(Point2D(0., 0.));

  bufferRound(polygon, result, 1.);

  double halfSqrt2 = sqrt(2.) / 2.;
  MultiPolygon2D testResult({ {{
      Point2D(-1., 0.),
      Point2D(-1., 1.),
      Point2D(0. - halfSqrt2, 1. + halfSqrt2),
      Point2D(0., 2.),
      Point2D(1., 2.),
      Point2D(1. + halfSqrt2, 1. + halfSqrt2),
      Point2D(2., 1.),
      Point2D(2., 0.),
      Point2D(1. + halfSqrt2, 0. - halfSqrt2),
      Point2D(1., -1.),
      Point2D(0., -1.),
      Point2D(0. - halfSqrt2, 0. - halfSqrt2),
      Point2D(-1., 0.)}} });

  ASSERT_EQ(1, result.size());
  ASSERT_EQ(13, result[0].outer().size());
  ASSERT_TRUE(areEqualUpToRotate(result[0].outer(), testResult[0].outer()));
}
//==============================================================================
TEST( BufferTest, BufferAtPlaceTest )
{
  Ring2D origRing( 
    Box2D(Point2D( ), Point2D(20, 20)).toPolygon().outer() );
  Ring2D bufferedRing(
    Box2D( Point2D( -1, -1), Point2D( 21, 21 ) ).toPolygon( ).outer( ) );

  Ring2D ring( origRing );
  bufferSquare( ring, 1 );
  ASSERT_TRUE(areEqualUpToRotate(ring, bufferedRing));

  // если в кольце при надувании появляется дырка, генерируется исключение
  // рассмотрим кольцо-подкову. При надувании его ножки склеятся и получится
  // полигон с дыркой.
  ring = { Point2D( -0.1, 0 ), Point2D( -2, 1 ), Point2D( -2, 3 ), Point2D( 2, 3 ), 
    Point2D( 2, 1 ), Point2D( 0.1, 0 ), Point2D( 1,1 ), Point2D( 1, 2 ), 
    Point2D( -1, 2 ), Point2D( -1, 1 ), Point2D( -0.1, 0 ) };
  ASSERT_THROW( bufferSquare( ring, 0.15 ), AbstractOperationsFailure );
  try
  {
    bufferSquare( ring, 0.15 );
  }
  catch( AbstractOperationsFailure& ex )
  {
    ASSERT_EQ( ex.getCode( ), LossOfData );
    ASSERT_EQ( ex.getLib( ), abstrLibTitle );
  }

  Polygon2D origPoly;
  origPoly.outer( ) = origRing;
  origPoly.inners( ).push_back(
    Box2D( Point2D( 3, 3 ), Point2D( 7, 7 ) ).toPolygon( ).outer( ) );
  correct( origPoly );
  Polygon2D bufferedPoly;
  bufferedPoly.outer( ) = bufferedRing;
  bufferedPoly.inners( ).push_back(
    Box2D( Point2D( 4, 4 ), Point2D( 6, 6 ) ).toPolygon( ).outer( ) );
  correct( bufferedPoly );
  Polygon2D poly( origPoly );
  bufferSquare( poly, 1. );
  ASSERT_TRUE(areEqualUpToRotate(poly.outer(), bufferedPoly.outer()));

  MultiPolygon2D origMult;
  origMult.push_back( origPoly );
  MultiPolygon2D bufferedMult;
  bufferedMult.push_back( poly );
  MultiPolygon2D mult( origMult );
  bufferSquare( mult, 1 );
  ASSERT_EQ( mult, bufferedMult );
}
//==============================================================================
} //! Tests
} //! AbstractOperations
