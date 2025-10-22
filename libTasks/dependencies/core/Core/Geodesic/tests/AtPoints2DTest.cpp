#include <Geodesic/Operations/AtPoints2D.hpp>

#include <Geodesic/GeodesicException.hpp>
#include <Geodesic/GeoLineTools/OrthodromyTools.hpp>
//! gtest
#include <gtest/gtest.h>
//==============================================================================
namespace Geodesic
{
namespace Tests
{
//==============================================================================
TEST(AtPoints2DTest, baseFunctionsTest)
{
  // различные точки
  GeoPoint2D first;
  GeoPoint2D second( Degree( 1 ), Degree( 2 ) );

  OrthodromyTools orth;

  auto line = orth.createOrthodromy( first, second );
  double len = line.getLength( );

  ASSERT_EQ( len, distance( first, second ) );
}
//==============================================================================
} // ! Tests
} // ! Geodesic
