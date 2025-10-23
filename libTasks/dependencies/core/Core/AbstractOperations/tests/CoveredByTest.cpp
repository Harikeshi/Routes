#include <AbstractOperations/Algorithms/CoveredBy.hpp>
//! PrimaryEntities
#include <PrimaryEntities/GeometryEntities.hpp>
#include <PrimaryEntities/GeodesicEntities.hpp>
//! gtest
#include <gtest/gtest.h>
//==============================================================================
namespace AbstractOperations
{
namespace Tests
{
//==============================================================================

TEST(CoveredByTest, Flat)
{
  Polygon2D polygon;
  polygon.outer( ) = {
    Point2D( ), Point2D( 0,1 ), Point2D( 1,1 ), Point2D( 1,0 ), Point2D( )
  };

  // точка, полностью лежащая внутри
  ASSERT_TRUE( coveredBy( Point2D( 0.5, 0.5 ), polygon ) );
  
  // точка на границе 
  ASSERT_TRUE( coveredBy( Point2D( ), polygon ) );

  // точка за пределами
  ASSERT_FALSE( coveredBy( Point2D( -1, -1 ), polygon ) );

  // можно поправить направление
  Ring2D wrong(
    { Point2D(), Point2D(1,0), Point2D(1,1), Point2D(0,1), Point2D() });
  ASSERT_TRUE(coveredBy(Point2D(), wrong, true));
  ASSERT_THROW(coveredBy(Point2D(), wrong), AbstractOperationsFailure);
  try
  {
    coveredBy(Point2D(), wrong);
  }
  catch (AbstractOperationsFailure& ex)
  {
    ASSERT_EQ(ex.getCode(), IncorrectGeometry);
    ASSERT_NE(std::string(ex.what()).find("Wrong orientation"),
              std::string::npos);
  }
}
//==============================================================================
TEST( CoveredByTest, Geo )
{
  GeoPolygon2D polygon;
  polygon.outer( ) = {
    GeoPoint2D( ),
    GeoPoint2D( Degree( 1 ),Degree( 0 ) ),
    GeoPoint2D( Degree( 1 ),Degree( 1 ) ),
    GeoPoint2D( Degree( 0 ),Degree( 1 ) ),
    GeoPoint2D(  )
  };

  // по умолчанию все в порядке
  // точка, полностью лежащая внутри
  ASSERT_TRUE( coveredBy( GeoPoint2D( Degree( 0.5 ), Degree( 0.5 ) ), polygon ) );
  // точка на границе 
  ASSERT_TRUE( coveredBy( GeoPoint2D( Degree( 0 ), Degree( 0 ) ), polygon ) );
  // точка за пределами
  ASSERT_FALSE( coveredBy( GeoPoint2D( Degree( -1 ), Degree( -1 ) ), polygon ) );

  // можно поправить направление
  GeoRing2D wrong( {
    GeoPoint2D( Degree( 0 ),Degree( 0 ) ),
    GeoPoint2D( Degree( 0 ),Degree( 1 ) ),
    GeoPoint2D( Degree( 1 ),Degree( 1 ) ),
    GeoPoint2D( Degree( 1 ),Degree( 0 ) ),
    GeoPoint2D( Degree( 0 ),Degree( 0 ) )
  } );

  ASSERT_TRUE(coveredBy(GeoPoint2D(Degree(0.5), Degree(0.5)), wrong, true));
  ASSERT_THROW(coveredBy(GeoPoint2D(), wrong), AbstractOperationsFailure);
  try
  {
    coveredBy(GeoPoint2D(), wrong);
  }
  catch (AbstractOperationsFailure& ex)
  {
    ASSERT_EQ(ex.getCode(), IncorrectGeometry);
    ASSERT_NE(std::string(ex.what()).find("Wrong orientation"),
              std::string::npos);
  }
}
//==============================================================================
TEST( CoveredByTest, Geo180 )
{
  GeoPolygon2D polygon;
  polygon.outer( ) = {
    GeoPoint2D( Degree( 0 ),Degree( 179 ) ),
    GeoPoint2D( Degree( 1 ),Degree( 179 ) ),
    GeoPoint2D( Degree( 1 ),Degree( -179 ) ),
    GeoPoint2D( Degree( 0 ),Degree( -179 ) ),
    GeoPoint2D( Degree( 0 ),Degree( 179 ) )
  };

  // по умолчанию все в порядке
  // точка, полностью лежащая внутри
  ASSERT_TRUE( coveredBy( GeoPoint2D( Degree( 0.5 ), Degree( 179.5 ) ), polygon ) );
  // точка на границе 
  ASSERT_TRUE( coveredBy( GeoPoint2D( Degree( 0 ), Degree( 179 ) ), polygon ) );
  // точка за пределами
  ASSERT_FALSE( coveredBy( GeoPoint2D( Degree( -1 ), Degree( 178 ) ), polygon ) );

  // если отказаться от нормализации, прилетит исключение
  ASSERT_THROW(
    coveredBy(GeoPoint2D(), polygon, false, false),
    AbstractOperationsFailure);
  try
  {
    coveredBy(GeoPoint2D(), polygon, false, false);
  }
  catch (AbstractOperationsFailure& ex)
  {
    ASSERT_EQ(ex.getCode(), IncorrectGeometry);
    ASSERT_NE(std::string(ex.what()).find("Wrong orientation"),
      std::string::npos);
  }

  // можно поправить направление
  GeoRing2D wrong( {
    GeoPoint2D( Degree( 0 ),Degree( 179 ) ),
    GeoPoint2D( Degree( 0 ),Degree( -179 ) ),
    GeoPoint2D( Degree( 1 ),Degree( -179 ) ),
    GeoPoint2D( Degree( 1 ),Degree( 179 ) ),
    GeoPoint2D( Degree( 0 ),Degree( 179 ) )
  } );
  ASSERT_TRUE(coveredBy(GeoPoint2D(Degree(0.5), Degree(179.5)), wrong, true));
  ASSERT_THROW(coveredBy(GeoPoint2D(), wrong), AbstractOperationsFailure);
  try
  {
    coveredBy(GeoPoint2D(), wrong);
  }
  catch (AbstractOperationsFailure& ex)
  {
    ASSERT_EQ(ex.getCode(), IncorrectGeometry);
    ASSERT_NE(std::string(ex.what()).find("Wrong orientation"),
              std::string::npos);
  }
  // исправление направления без нормализации приводит к решению альтернативной задачи
  ASSERT_TRUE(
    coveredBy(GeoPoint2D(Degree(0.5), Degree(0.5)), wrong, true, false));
}
//==============================================================================
} // ! Tests
} // ! AbstractOperations
