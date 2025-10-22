#include <Geodesic/ComplicatedOperations/PositionInArea.hpp>
//! gtest
#include <gtest/gtest.h>
//==============================================================================
namespace Geodesic
{
namespace Tests
{
//==============================================================================
class PositionInAreaTest : public ::testing::Test
{
  //==============================================================================
protected:
  //==============================================================================
  void SetUp( )
  {
    pointInSquare = GeoPoint2D(Degree(1.5), Degree(1.5));
    pointInRectangle = GeoPoint2D(Degree(2), Degree(1.5));
    pointInP = GeoPoint2D(Degree(2.5), Degree(2.1));
    pointInP2 = GeoPoint2D(Degree(2.5), Degree(2));
    pointInDoubleSquare = GeoPoint2D(Degree(3), Degree(2.5));
  }
  //==============================================================================
  void TearDown( )
  {

  }
  //==============================================================================
  // Квадрат
  GeoPolygon2D square{ {
    GeoPoint2D(Degree(1), Degree(1)), GeoPoint2D(Degree(2), Degree(1)),
    GeoPoint2D(Degree(2), Degree(2)), GeoPoint2D(Degree(1), Degree(2)),
    GeoPoint2D(Degree(1), Degree(1)) } };
  GeoPoint2D pointInSquare;

  // Прямоугольник
  GeoPolygon2D rectangle{ {
    GeoPoint2D(Degree(1), Degree(1)), GeoPoint2D(Degree(3), Degree(1)),
    GeoPoint2D(Degree(3), Degree(2)), GeoPoint2D(Degree(1), Degree(2)),
    GeoPoint2D(Degree(1), Degree(1))} };
  GeoPoint2D pointInRectangle;

  // Буква П - центр масс внутри
  GeoPolygon2D letterP{ {
    GeoPoint2D(Degree(1), Degree(1)), GeoPoint2D(Degree(2), Degree(1)),
    GeoPoint2D(Degree(2), Degree(2)), GeoPoint2D(Degree(3), Degree(2)),
    GeoPoint2D(Degree(3), Degree(1)), GeoPoint2D(Degree(4), Degree(1)),
    GeoPoint2D(Degree(4), Degree(3)), GeoPoint2D(Degree(1), Degree(3)),
    GeoPoint2D(Degree(1), Degree(1))} };
  GeoPoint2D pointInP;

  // Буква П - центр масс снаружи
  GeoPolygon2D letterP2{ {
    GeoPoint2D(Degree(1), Degree(0)), GeoPoint2D(Degree(2), Degree(0)),
    GeoPoint2D(Degree(2), Degree(2)), GeoPoint2D(Degree(3), Degree(2)),
    GeoPoint2D(Degree(3), Degree(0)), GeoPoint2D(Degree(4), Degree(0)),
    GeoPoint2D(Degree(4), Degree(3)), GeoPoint2D(Degree(1), Degree(3)),
    GeoPoint2D(Degree(1), Degree(0))} };
  GeoPoint2D pointInP2;

  // Полигон с вырезанным центром (квадрат в квадрате)
  GeoPolygon2D doubleSquare{
    { GeoPoint2D(Degree(1), Degree(1)), GeoPoint2D(Degree(4), Degree(1)),
      GeoPoint2D(Degree(4), Degree(4)), GeoPoint2D(Degree(1), Degree(4)),
      GeoPoint2D(Degree(1), Degree(1)) },
    { GeoPoint2D(Degree(2), Degree(2)), GeoPoint2D(Degree(2), Degree(3)),
      GeoPoint2D(Degree(3), Degree(3)), GeoPoint2D(Degree(3), Degree(2)),
      GeoPoint2D(Degree(2), Degree(2)) } };
  GeoPoint2D pointInDoubleSquare;
  //==============================================================================
};
//==============================================================================
TEST_F( PositionInAreaTest, getPositionInArea )
{
  double eps = 0.00005;
  // Квадрат
  GeoPoint2D pointInPoly = getPositionInArea(square);
  ASSERT_NEAR(
    pointInPoly.getLatitude().get(), pointInSquare.getLatitude().get(), eps);
  ASSERT_NEAR(
    pointInPoly.getLongitude().get(), pointInSquare.getLongitude().get(), eps);

  // Прямоугольник
  pointInPoly = getPositionInArea(rectangle);
  ASSERT_NEAR(pointInPoly.getLatitude().get(),
              pointInRectangle.getLatitude().get(), eps);
  ASSERT_NEAR(pointInPoly.getLongitude().get(),
              pointInRectangle.getLongitude().get(), eps);

  // Буква П - центр масс внутри
  pointInPoly = getPositionInArea(letterP);
  ASSERT_NEAR(
    pointInPoly.getLatitude().get(), pointInP.getLatitude().get(), eps);
  ASSERT_NEAR(
    pointInPoly.getLongitude().get(), pointInP.getLongitude().get(), eps);

  // Буква П - центр масс снаружи
  pointInPoly = getPositionInArea(letterP2);
  ASSERT_NEAR(
    pointInPoly.getLatitude().get(), pointInP2.getLatitude().get(), eps);
  ASSERT_NEAR(
    pointInPoly.getLongitude().get(), pointInP2.getLongitude().get(), eps);

  // Полигон с вырезанным центром (квадрат в квадрате)
  pointInPoly = getPositionInArea(doubleSquare);
  ASSERT_NEAR(pointInPoly.getLatitude().get(),
              pointInDoubleSquare.getLatitude().get(), eps);
  ASSERT_NEAR(pointInPoly.getLongitude().get(),
              pointInDoubleSquare.getLongitude().get(), eps);
}
//==============================================================================
} //! Tests
} //! Geodesic
//==============================================================================