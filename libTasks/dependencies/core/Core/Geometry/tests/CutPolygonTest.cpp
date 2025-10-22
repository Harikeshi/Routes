#include <Geometry/Operations/CutPolygon.hpp>

#include <Geometry/GeometryException.hpp>
//! AbstractOperations
#include <AbstractOperations/Algorithms/Area.hpp>
#include <AbstractOperations/Algorithms/Envelope.hpp>
//! PrimaryEntities
#include <PrimaryEntities/Equalities.hpp>
//! SamplesForTests
#include <SamplesForTests/BaseFlatFigures.hpp>
//! gtest
#include <gtest/gtest.h>
//==============================================================================
namespace Geometry
{
namespace Tests
{
//==============================================================================
  TEST(CutPolygonTest, cutPolygon)
  {
    Polygon2D square({ {
        Point2D(0., 0.), Point2D(0., 1.),
        Point2D(1., 1.), Point2D(1., 0.),
      Point2D(0., 0.) } });

    Polygon2D star({ {
        Point2D(2., 1.), Point2D(3., 3.),
        Point2D(1., 4.), Point2D(3., 5.),
        Point2D(4., 7.), Point2D(5., 5.),
        Point2D(7., 4.), Point2D(5., 3.),
        Point2D(6., 1.), Point2D(4., 2.),
        Point2D(2., 1.) } });

    Polygon2D bow({ {
        Point2D(-7., -1.),Point2D(-4., -2.),
        Point2D(-1., -1.), Point2D(-1., -5.),
        Point2D(-4., -4.), Point2D(-7., -5.),
        Point2D(-7., -1.) } });

    // Разбиение квадрата 2x2
    MultiPolygon2D result = cutPolygon(square, 2, 2);
    ASSERT_EQ(result.size(), 4);

    // Коробка полигона
    Box2D polyBox;
    AbstractOperations::envelope(square, polyBox, false);
    // Площадь коробки полигона
    double boxArea = AbstractOperations::area(polyBox);
    // Площадь ячейки сетки
    double cellArea = boxArea / 4;
    // Площадь полигонов в ячейках сетки не может быть больше площади
    // ячейки сетки
    for (const auto& poly : result)
      ASSERT_TRUE(
        cellArea - AbstractOperations::area(poly) > -ACCURACY_DOUBLE);

    // Разбиение квадрата 3x5
    result = cutPolygon(square, 3, 5);
    ASSERT_EQ(result.size(), 15);

    cellArea = boxArea / 15;
    for (const auto& poly : result)
      ASSERT_TRUE(
        cellArea - AbstractOperations::area(poly) > -ACCURACY_DOUBLE);

    // Разбиение звезды 3x3
    result = cutPolygon(star, 3, 3);
    ASSERT_EQ(result.size(), 7);

    AbstractOperations::envelope(star, polyBox, false);
    boxArea = AbstractOperations::area(polyBox);
    cellArea = boxArea / 9;
    for (const auto& poly : result)
      ASSERT_TRUE(
        cellArea - AbstractOperations::area(poly) > -ACCURACY_DOUBLE);

    // Разбиение звезды 8x1
    result = cutPolygon(star, 8, 1);
    ASSERT_EQ(result.size(), 10);

    cellArea = boxArea / 8;
    for (const auto& poly : result)
      ASSERT_TRUE(
        cellArea - AbstractOperations::area(poly) > -ACCURACY_DOUBLE);

    // Разбиение бантика 5x5
    result = cutPolygon(bow, 5, 5);
    ASSERT_EQ(result.size(), 23);

    AbstractOperations::envelope(bow, polyBox, false);
    boxArea = AbstractOperations::area(polyBox);
    cellArea = boxArea / 25;
    for (const auto& poly : result)
      ASSERT_TRUE(
        cellArea - AbstractOperations::area(poly) > -ACCURACY_DOUBLE);

    // Разбиение бантика 25x25
    result = cutPolygon(bow, 25, 25);
    ASSERT_EQ(result.size(), 493);

    cellArea = boxArea / 625;
    for (const auto& poly : result)
      ASSERT_TRUE(
        cellArea - AbstractOperations::area(poly) > -ACCURACY_DOUBLE);

    // Пустой полигон
    result = cutPolygon(Polygon2D(), 5, 5);
    ASSERT_EQ(result, MultiPolygon2D());

    // Полигон, с нулевым шагом сетки
    ASSERT_THROW(cutPolygon(bow, 0, 5), GeometryFailure);
    try
    {
      result = cutPolygon(bow, 0, 5);
    }
    catch (const GeometryFailure& exception)
    {
      ASSERT_EQ(ZeroGridStep, exception.getCode());
      ASSERT_EQ(geomLibTitle, exception.getLib());
      ASSERT_EQ(
        "[Core::Geometry] <ZeroGridStep>: Нулевой шаг сетки"
        , std::string(exception.what()));
    }
  }
//==============================================================================
  TEST( CutPolygonTest, cutIncorrectFigure )
  {
    // некорректная геометрия (бантик с самопересечением)
    auto bow( 
      SamplesForTests::buildBow( Point2D( ), 2, 4 ) );

    ASSERT_THROW( cutPolygon ( bow, 2, 4 ), GeometryFailure );
    try
    {
      cutPolygon( bow, 2, 4 );
    }
    catch( const GeometryFailure& exception )
    {
      ASSERT_EQ( exception.getLib( ), geomLibTitle );
      ASSERT_EQ( exception.getCode( ), TransitFailure );
      ASSERT_NE( std::string( exception.what( ) ).find( "IncorrectGeometry" ),
                 std::string::npos );
    }
  }
//==============================================================================
} // ! Tests
} // ! Geometry