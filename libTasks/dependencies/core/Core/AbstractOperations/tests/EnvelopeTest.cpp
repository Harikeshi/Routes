#include <AbstractOperations/Algorithms/Envelope.hpp>
//! PrimaryEntities
#include <PrimaryEntities/GeometryEntities.hpp>
//! gtest
#include <gtest/gtest.h>
//==============================================================================
namespace AbstractOperations
{
namespace Tests
{
//==============================================================================
TEST(EnvelopeTest, Envelope)
{
  /*!
   * Ищем минимальный прямоугольник со сторонами,
   * параллельными координатным осям,
   * для случайного кольца в географических координатах.
   */

  GeoRing2D geoRing{
    GeoPoint2D(Degree(50.), Degree(178.)),
    GeoPoint2D(Degree(50.), Degree(-178.)),
    GeoPoint2D(Degree(60.), Degree(-178.)),
    GeoPoint2D(Degree(60.), Degree(178.)),
    GeoPoint2D(Degree(50.), Degree(178.))
  };

  GeoBox2D geoResult;
  envelope(geoRing, geoResult);
  ASSERT_EQ(GeoPoint2D(Degree(50.), Degree(178.)), 
            geoResult.min_corner());
  ASSERT_EQ(GeoPoint2D(Degree(60.), Degree(-178.)), 
            geoResult.max_corner());

  envelope(geoRing, geoResult, false);
  ASSERT_EQ(GeoPoint2D(Degree(50.), Degree(-178.)),
            geoResult.min_corner());
  ASSERT_EQ(GeoPoint2D(Degree(60.), Degree(178.)),
            geoResult.max_corner());

  Ring2D ring{
    Point2D(50., 178.),
    Point2D(50., -178.),
    Point2D(60., 178),
    Point2D(60., -178.),
    Point2D(50., 178.)
  };

  Box2D result;
  envelope(ring, result);
  ASSERT_EQ(Point2D(50., -178.), result.min_corner());
  ASSERT_EQ(Point2D(60., 178.), result.max_corner());

  envelope(ring, result, false);
  ASSERT_EQ(Point2D(50., -178.), result.min_corner());
  ASSERT_EQ(Point2D(60., 178.), result.max_corner());
}
//==============================================================================
} //! Tests
} //! ExternalGeometry
//==============================================================================