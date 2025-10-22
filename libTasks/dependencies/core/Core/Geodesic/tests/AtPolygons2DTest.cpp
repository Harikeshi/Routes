#include <Geodesic/Operations/AtPolygons2D.hpp>
//! gtest
#include <gtest/gtest.h>
//==============================================================================
namespace Geodesic
{
namespace Tests
{
//==============================================================================
TEST( AtPolygons2DTest, chooseBiggestPolygon )
{
  GeoPolygon2D firstSquare( { {
    GeoPoint2D( Degree( 0 ), Degree( 1 ) ),
    GeoPoint2D( Degree( 0 ), Degree( 1 ) ),
    GeoPoint2D( Degree( 1 ), Degree( 1 ) ),
    GeoPoint2D( Degree( 1 ), Degree( 2 ) ),
    GeoPoint2D( Degree( 0 ), Degree( 2 ) ),
    GeoPoint2D( Degree( 0 ), Degree( 1 ) ) } } );

  // квадрат, равный по площади первому
  GeoPolygon2D secondSquare( { {
    GeoPoint2D( Degree( 0 ), Degree( -1 ) ),
    GeoPoint2D( Degree( 0 ), Degree( -2 ) ),
    GeoPoint2D( Degree( 1 ), Degree( -2 ) ),
    GeoPoint2D( Degree( 1 ), Degree( -1 ) ),
    GeoPoint2D( Degree( 0 ), Degree( -1 ) ),
    GeoPoint2D( Degree( 0 ), Degree( -1 ) ) } } );

  // квадрат, по площади больше первого
  GeoPolygon2D thirdSquare( { {
    GeoPoint2D( Degree( 0 ), Degree( 3 ) ),
    GeoPoint2D( Degree( 0 ), Degree( 3 ) ),
    GeoPoint2D( Degree( 1 ), Degree( 3 ) ),
    GeoPoint2D( Degree( 1 ), Degree( 5 ) ),
    GeoPoint2D( Degree( 0 ), Degree( 5 ) ),
    GeoPoint2D( Degree( 0 ), Degree( 3 ) ) } } );

  // Квадрат, равный по разности градусов первому, но меньший по площади
  GeoPolygon2D forthSquare( { {
    GeoPoint2D( Degree( 10 ), Degree( 3 ) ),
    GeoPoint2D( Degree( 10 ), Degree( 3 ) ),
    GeoPoint2D( Degree( 11 ), Degree( 3 ) ),
    GeoPoint2D( Degree( 11 ), Degree( 4 ) ),
    GeoPoint2D( Degree( 10 ), Degree( 4 ) ),
    GeoPoint2D( Degree( 10 ), Degree( 3 ) ) } } );

  GeoMultiPolygon2D twoDiffSquares;
  twoDiffSquares.emplace_back( firstSquare );
  twoDiffSquares.emplace_back( thirdSquare );

  GeoMultiPolygon2D twoSimSquares;
  twoSimSquares.emplace_back( firstSquare );
  twoSimSquares.emplace_back( secondSquare );

  GeoMultiPolygon2D twoSimDiffSquares;
  twoSimDiffSquares.emplace_back( firstSquare );
  twoSimDiffSquares.emplace_back( forthSquare );

  GeoPolygon2D biggestPoly1
    = chooseBiggestPolygon( twoDiffSquares );
  GeoPolygon2D biggestPoly2
    = chooseBiggestPolygon( twoSimSquares );
  GeoPolygon2D biggestPoly3
    = chooseBiggestPolygon( twoSimDiffSquares );

  for( size_t i = 0; i < biggestPoly1.outer( ).size( ); ++i )
    ASSERT_EQ( biggestPoly1.outer( )[i], thirdSquare.outer( )[i] );

  for( size_t i = 0; i < biggestPoly2.outer( ).size( ); ++i )
    ASSERT_EQ( biggestPoly2.outer( )[i], firstSquare.outer( )[i] );

  for( size_t i = 0; i < biggestPoly2.outer( ).size( ); ++i )
    ASSERT_EQ( biggestPoly3.outer( )[i], firstSquare.outer( )[i] );
}
//==============================================================================
} //! Tests
} //! Geodesic
//==============================================================================