#include <AbstractOperations/ExtractPointsFromGeoms.hpp>
//! PrimaryEntites
#include <PrimaryEntities/GeometryEntities.hpp>
//! MathTools
#include <MathTools/BasicMath/Compare.hpp>
//! gtest
#include <gtest/gtest.h>
//==============================================================================
namespace AbstractOperations
{
namespace Tests
{
//==============================================================================
using BasicMath::isEqual;
//==============================================================================
/* Построить квадрат с центром в данной точке и длиной стороны в метрах (!)
 *\param[in] center - центр квадрата
 *\param[in] size - длина стороны квадрата [м]
 *\return - построенный квадрат
 */
Polygon2D buildEllSquare( const Point2D center, const double size )
{
  Polygon2D retVal;
  double y = center.getY( );
  double x = center.getX( );
  double shift = size / 2.;
  retVal.outer( ).emplace_back( x + shift, y + shift );
  retVal.outer( ).emplace_back( x + shift, y - shift );
  retVal.outer( ).emplace_back( x - shift, y - shift );
  retVal.outer( ).emplace_back( x - shift, y + shift );
  retVal.outer( ).push_back( retVal.outer( )[0] );
  return retVal;
}
//==============================================================================
//==============================================================================
class ExtractPointsFromGeomsTest : public ::testing::Test
{
protected:
  void SetUp( )
  {
    center.set( 100., 0. );
    otherCenter.set( 110., 0. );

    smallCenteredSquare = buildEllSquare( center, 1. );
    bigCenteredSquare = buildEllSquare( center, 3. );

    smallShiftedSquare = buildEllSquare( otherCenter, 1. );
    bigShiftedSquare = buildEllSquare( otherCenter, 3. );

    outers = {
      bigCenteredSquare.outer( )[0],
      bigCenteredSquare.outer( )[1],
      bigCenteredSquare.outer( )[2],
      bigCenteredSquare.outer( )[3]
    };

    inners = {
      smallCenteredSquare.outer( )[0],
      smallCenteredSquare.outer( )[1],
      smallCenteredSquare.outer( )[2],
      smallCenteredSquare.outer( )[3]
    };

    alls = outers;
    alls.insert( alls.end( ), inners.begin( ), inners.end( ) );

  }
  //==============================================================================
  void TearDown( )
  {
  }
  //==============================================================================

  Point2D center;
  Point2D otherCenter;

  Polygon2D smallCenteredSquare; // обычный квадрат со стороной 1
  Polygon2D bigCenteredSquare; // обычный квадрат со стороной 3

  Polygon2D smallShiftedSquare; // смещенный квадрат со стороной 1
  Polygon2D bigShiftedSquare; // смещенный квадрат со стороной 3

  MultiPoint2D outers;
  MultiPoint2D inners;
  MultiPoint2D alls;

};
//==============================================================================
TEST_F( ExtractPointsFromGeomsTest, getPoints )
{
  Box2D box( bigCenteredSquare.outer( )[2], bigCenteredSquare.outer( )[0] );
  // получаем рамку целиком
  auto border = getBorderPoints( box );
  ASSERT_EQ( border.size( ), 4 );
  ASSERT_EQ( border[0], bigCenteredSquare.outer( )[2] );
  ASSERT_EQ( border[1], bigCenteredSquare.outer( )[0] );
  ASSERT_EQ( border[2], bigCenteredSquare.outer( )[1] );
  ASSERT_EQ( border[3], bigCenteredSquare.outer( )[3] );
  // получаем только противоположные углы
  auto all = getAllPoints( box );
  ASSERT_EQ( all.size( ), 2 );
  ASSERT_EQ( all[0], bigCenteredSquare.outer( )[2] );
  ASSERT_EQ( all[1], bigCenteredSquare.outer( )[0] );

  Linestring2D line;
  for( auto point : outers )
    line.push_back( point );
  all = getAllPoints( line );
  border = getBorderPoints( line );
  ASSERT_EQ( all.size( ), 4 );
  ASSERT_EQ( border.size( ), 4 );
  for( size_t i = 0; i < 4; i++ )
  {
    ASSERT_EQ( all[i], outers[i] );
    ASSERT_EQ( border[i], outers[i] );
  }

  MultiLinestring2D multiLine;
  multiLine.resize( 2 );
  multiLine[0].insert( multiLine[0].end( ), outers.begin( ), outers.end( ) );
  multiLine[1].insert( multiLine[1].end( ), inners.begin( ), inners.end( ) );
  all = getAllPoints( multiLine );
  border = getBorderPoints( multiLine );
  ASSERT_EQ( 8, all.size( ) );
  ASSERT_EQ( 8, border.size( ) );
  for( size_t i = 0; i < 4; i++ )
  {
    ASSERT_EQ( outers[i], all[i] );
    ASSERT_EQ( outers[i], border[i] );
  }
  for( size_t i = 4; i < 8; i++ )
  {
    ASSERT_EQ( inners[i - 4], all[i] );
    ASSERT_EQ( inners[i - 4], border[i] );
  }

  std::vector<Point2D> vect;
  for( auto point : outers )
    vect.push_back( point );
  all = getAllPoints( vect );
  border = getBorderPoints( vect );
  ASSERT_EQ( all.size( ), 4 );
  ASSERT_EQ( border.size( ), 4 );
  for( size_t i = 0; i < 4; i++ )
  {
    ASSERT_EQ( all[i], outers[i] );
    ASSERT_EQ( border[i], outers[i] );
  }

  Ring2D ring( bigCenteredSquare.outer( ) );
  all = getAllPoints( ring );
  border = getBorderPoints( ring );
  ASSERT_EQ( all.size( ), 4 );
  ASSERT_EQ( border.size( ), 4 );
  for( size_t i = 0; i < 4; i++ )
  {
    ASSERT_EQ( all[i], outers[i] );
    ASSERT_EQ( border[i], outers[i] );
  }

  Polygon2D polyg( bigCenteredSquare );
  polyg.inners( ).push_back( smallCenteredSquare.outer( ) );
  all = getAllPoints( polyg );
  border = getBorderPoints( polyg );
  ASSERT_EQ( all.size( ), 8 );
  ASSERT_EQ( border.size( ), 4 );
  for( size_t i = 0; i < 4; i++ )
  {
    ASSERT_EQ( border[i], outers[i] );
  }
  for( size_t i = 0; i < 8; i++ )
  {
    ASSERT_EQ( all[i], alls[i] );
  }

  MultiPolygon2D mult( { polyg } );
  polyg = bigShiftedSquare;
  polyg.inners( ).push_back( smallShiftedSquare.outer( ) );
  mult.push_back( polyg );
  all = getAllPoints( mult );
  border = getBorderPoints( mult );
  ASSERT_EQ( all.size( ), 16 );
  ASSERT_EQ( border.size( ), 8 );
  for( size_t i = 0; i < 4; i++ )
  {
    ASSERT_EQ( border[i], outers[i] );
  }
  for( size_t i = 0; i < 8; i++ )
  {
    ASSERT_EQ( all[i], alls[i] );
  }
  for( size_t i = 4; i < 8; i++ )
  {
    ASSERT_EQ( border[i], polyg.outer( )[i - 4] );
  }
  for( size_t i = 8; i < 12; i++ )
  {
    ASSERT_EQ( all[i], polyg.outer( )[i - 8] );
  }
  for( size_t i = 12; i < 16; i++ )
  {
    ASSERT_EQ( all[i], polyg.inners( )[0][i - 12] );
  }

}
//==============================================================================
} //! Tests
} //! AbstractOperations
//==============================================================================