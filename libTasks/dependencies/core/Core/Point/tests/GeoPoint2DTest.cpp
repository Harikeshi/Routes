#include <Point/GeoPoint2D.hpp>
//! gtest
#include <gtest/gtest.h>
//==============================================================================
namespace Tests
{
//==============================================================================
/*!
 * \brief Тестирование GeoPoint2D
 *        (конструктор на оcнове двух радиан)
 */
TEST( GeoPoint2DTest, ConstructorByRadian )
{
  ASSERT_EQ( std::make_pair( Radian( 0. ), Radian( 0. ) ),
    GeoPoint2D( ).get( ) );
  ASSERT_EQ( std::make_pair( Radian( M_PI / 2 ), Radian( -M_PI / 2 ) ),
    GeoPoint2D( Radian( M_PI / 2 ), Radian( -M_PI / 2 ) ).get( ) );
  ASSERT_EQ( std::make_pair( Radian( -M_PI * 2 ), Radian( M_PI * 2 ) ),
    GeoPoint2D( Radian( -M_PI * 2 ), Radian( M_PI * 2 ) ).get( ) );
  ASSERT_EQ( std::make_pair( Radian( 0.25 * RADIAN_PER_DEGREE ),
    Radian( -0.1 * RADIAN_PER_DEGREE ) ),
    GeoPoint2D( Radian( 0.25 * RADIAN_PER_DEGREE ),
      Radian( -0.1 * RADIAN_PER_DEGREE ) ).get( ) );
}
//==============================================================================
/*!
 * \brief Тестирование GeoPoint2D
 *        (конструктор на оcнове двух градусов)
 */
TEST( GeoPoint2DTest, ConstructoByDegree )
{
  ASSERT_EQ( GeoPoint2D( Radian( 0. ), Radian( 0. ) ),
    GeoPoint2D( Degree( 0. ), Degree( 0. ) ) );
  ASSERT_EQ( GeoPoint2D( Radian( -M_PI / 2 ), Radian( M_PI / 2 ) ),
    GeoPoint2D( Degree( -90. ), Degree( 90. ) ) );
  ASSERT_EQ( GeoPoint2D( Radian( -M_PI * 2 ), Radian( M_PI * 2 ) ),
    GeoPoint2D( Degree( -360. ), Degree( 360. ) ) );
  ASSERT_EQ( GeoPoint2D( Radian( 0.25 * RADIAN_PER_DEGREE ), 
    Radian( -0.1 * RADIAN_PER_DEGREE ) ),
    GeoPoint2D( Degree( 0.25 ), Degree( -0.1 ) ) );
}
//==============================================================================
/*!
 * \brief Тестирование GeoPoint2D
 *        (копирующий конструктор)
 */
TEST( GeoPoint2DTest, ConstructorCopy )
{
  GeoPoint2D point;
  ASSERT_EQ( point, GeoPoint2D( point ) );
  point.set( Radian( -M_PI / 2 ), Radian( M_PI / 2 ) );
  ASSERT_EQ( point, GeoPoint2D( point ) );
  point.set( Radian( -M_PI * 2 ), Radian( M_PI * 2 ) );
  ASSERT_EQ( point, GeoPoint2D( point ) );
  point.set( Radian( 0.25 * RADIAN_PER_DEGREE ), Radian( -0.1 * RADIAN_PER_DEGREE ) );
  ASSERT_EQ( point, GeoPoint2D( point ) );
}
//==============================================================================
/*!
 * \brief Тестирование set и get (для радиан)
 *        (присвоение значения на основе радиан)
 */
TEST( GeoPoint2DTest, RadianGetSet )
{
  GeoPoint2D point;
  point.set( Radian( M_PI / 4 ), Radian( -M_PI / 4 ) );
  ASSERT_EQ( std::make_pair( Radian( M_PI / 4 ), Radian( -M_PI / 4 ) ), point.get( ) );
  point.set( Radian( -0.001 * RADIAN_PER_DEGREE ),
    Radian( 1024.25 * RADIAN_PER_DEGREE ) );
  ASSERT_EQ( std::make_pair( Radian( -0.001 * RADIAN_PER_DEGREE ),
    Radian( 1024.25 * RADIAN_PER_DEGREE ) ), point.get( ) );
  point.set( Radian( 0. ), Radian( 0. ) );
  ASSERT_EQ( std::make_pair( Radian( 0. ), Radian( 0. ) ), point.get( ) );
}
//==============================================================================
/*!
 * \brief Тестирование set (для Degree) и get (для Radian)
 *        (присвоение значения на основе радиан)
 */
TEST( GeoPoint2DTest, DegreeGetSet )
{
  GeoPoint2D point;
  point.set( Degree( 60. ), Degree( -60. ) );
  ASSERT_EQ( Radian( M_PI / 3 ), point.getLatitude( ) );
  ASSERT_EQ( Radian( -M_PI / 3 ), point.getLongitude( ) );
  point.set( Degree( -0.001 ), Degree( 1024.25 ) );
  ASSERT_EQ( Radian( -0.001 * RADIAN_PER_DEGREE ), point.getLatitude( ) );
  ASSERT_EQ( Radian( 1024.25 * RADIAN_PER_DEGREE ), point.getLongitude( ) );
  point.set( Degree( 0. ), Degree( 0. ) );
  ASSERT_EQ( Radian( ), point.getLatitude( ) );
  ASSERT_EQ( Radian( ), point.getLongitude( ) );
}
//==============================================================================
/*!
 * \brief Тестирование isCorrect
 *        (проверка на попадание широты и долготы в диапазон допустимых значений)
 */
TEST( GeoPoint2DTest, IsCorrect )
{
  GeoPoint2D point;
  ASSERT_TRUE( point.isCorrect( REGULAR ) );
  ASSERT_TRUE( point.isCorrect( NORMAL ) );
  point.set( Degree( 90. ), Degree( 90. ) );
  ASSERT_TRUE( point.isCorrect( REGULAR ) );
  ASSERT_TRUE( point.isCorrect( NORMAL ) );
  point.set( Degree( -90. ), Degree( -90. ) );
  ASSERT_TRUE( point.isCorrect( REGULAR ) );
  ASSERT_FALSE( point.isCorrect( NORMAL ) );
  point.set( Degree( 180. ), Degree( 180. ) );
  ASSERT_FALSE( point.isCorrect( REGULAR ) );
  ASSERT_FALSE( point.isCorrect( NORMAL ) );
  point.set( Degree( 0. ), Degree( 180. ) );
  ASSERT_FALSE( point.isCorrect( REGULAR ) );
  ASSERT_TRUE( point.isCorrect( NORMAL ) );
  point.set( Degree( 0. ), Degree( -180. ) );
  ASSERT_TRUE( point.isCorrect( REGULAR ) );
  ASSERT_FALSE( point.isCorrect( NORMAL ) );
  point.set( Degree( 0. ), Degree( 360. ) );
  ASSERT_FALSE( point.isCorrect( REGULAR ) );
  ASSERT_FALSE( point.isCorrect( NORMAL ) );
  point.set( Degree( 0. ), Degree( -360. ) );
  ASSERT_FALSE( point.isCorrect( REGULAR ) );
  ASSERT_FALSE( point.isCorrect( NORMAL ) );
  point.set( Degree( 0. ), Degree( 179.999999999 ) );
  ASSERT_TRUE( point.isCorrect( REGULAR ) );
  ASSERT_TRUE( point.isCorrect( NORMAL ) );
  point.set( Degree( 0. ), Degree( 359.999999999 ) );
  ASSERT_FALSE( point.isCorrect( REGULAR ) );
  ASSERT_TRUE( point.isCorrect( NORMAL ) );
}
//==============================================================================
/*!
 * \brief Тестирование normalize
 *        (приведение долготы к заданному диапазону значений)
 */
TEST( GeoPoint2DTest, Normalize )
{
  GeoPoint2D point( Degree( 45. ), Degree( 90. ) );
  point.normalize( REGULAR );
  ASSERT_EQ( GeoPoint2D( Radian( M_PI / 4 ), Radian( M_PI / 2 ) ), point );
  point.normalize( NORMAL );
  ASSERT_EQ( GeoPoint2D( Radian( M_PI / 4 ), Radian( M_PI / 2 ) ), point );

  point.set( Degree( 45. ), Degree( 180. ) );
  point.normalize( REGULAR );
  ASSERT_EQ( GeoPoint2D( Radian( M_PI / 4 ), Radian( -M_PI ) ), point );
  point.set( Degree( 45. ), Degree( 180. ) );
  point.normalize( NORMAL );
  ASSERT_EQ( GeoPoint2D( Radian( M_PI / 4 ), Radian( M_PI ) ), point );

  point.set( Degree( 45. ), Degree( -180. ) );
  point.normalize( REGULAR );
  ASSERT_EQ( GeoPoint2D( Radian( M_PI / 4 ), Radian( -M_PI ) ), point );
  point.normalize( NORMAL );
  ASSERT_EQ( GeoPoint2D( Radian( M_PI / 4 ), Radian( M_PI ) ), point );

  point.set( Degree( 45. ), Degree( -45. ) );
  point.normalize( REGULAR );
  ASSERT_EQ( GeoPoint2D( Radian( M_PI / 4 ), Radian( -M_PI / 4 ) ), point );
  point.normalize( NORMAL );
  ASSERT_EQ( GeoPoint2D( Radian( M_PI / 4 ), Radian( 7 * M_PI / 4 ) ), point );

  point.set( Degree( 45. ), Degree( 135. ) );
  point.normalize( REGULAR );
  ASSERT_EQ( GeoPoint2D( Radian( M_PI / 4 ), Radian( 3 * M_PI / 4 ) ), point );
  point.normalize( NORMAL );
  ASSERT_EQ( GeoPoint2D( Radian( M_PI / 4 ), Radian( 3 * M_PI / 4 ) ), point );

  point.set( Degree( 180. ), Degree( 360. ) );
  point.normalize( REGULAR );
  ASSERT_EQ( GeoPoint2D( Radian( M_PI ), Radian( 0. ) ), point );
  point.set( Degree( 180. ), Degree( 360. ) );
  point.normalize( NORMAL );
  ASSERT_EQ( GeoPoint2D( Radian( M_PI ), Radian( 0. ) ), point );

  point.set( Degree( 180. ), Degree( -360. ) );
  point.normalize( REGULAR );
  ASSERT_EQ( GeoPoint2D( Radian( M_PI ), Radian( 0. ) ), point );
  point.set( Degree( 180. ), Degree( -360. ) );
  point.normalize( NORMAL );
  ASSERT_EQ( GeoPoint2D( Radian( M_PI ), Radian( 0. ) ), point );
}
//==============================================================================
/*!
 * \brief Тестирование operator ==
 *        (сравнение точек)
 */
TEST( GeoPoint2DTest, GeoPoint2DEqual )
{
  GeoPoint2D point;
  ASSERT_TRUE( point == GeoPoint2D( ) );
  point.set( Radian( M_PI ), Radian( M_PI ) );
  ASSERT_TRUE( point == GeoPoint2D( Radian( M_PI ), Radian( M_PI ) ) );
  point.set( Radian( -M_PI / 2 ), Radian( M_PI / 4 ) );
  ASSERT_TRUE( point == GeoPoint2D( Radian( -M_PI / 2 ), Radian( M_PI / 4 ) ) );
  point.set( Radian( 1024.25 * RADIAN_PER_DEGREE ),
    Radian( -0.001 * RADIAN_PER_DEGREE ) );
  ASSERT_TRUE( point == GeoPoint2D( Radian( 1024.25 * RADIAN_PER_DEGREE ),
    Radian( -0.001 * RADIAN_PER_DEGREE ) ) );
  ASSERT_FALSE( point == GeoPoint2D( ) );
}
//==============================================================================
/*!
 * \brief Тестирование operator !=
 *        Покоординатное сравнение двух точек на неравенство
 */
TEST(GeoPoint2DTest, GeoPoint2DNotEqual)
{
  GeoPoint2D point1;
  GeoPoint2D point2(Radian(0.1), Radian(-1.));
  ASSERT_TRUE(point1 != point2);

  point2.set(Radian(0.0000000000000001), Radian(0.0000000000000001));
  ASSERT_FALSE(point1 != point2);
}
//==============================================================================
} //! Tests
//==============================================================================
