#include <Point/GeoPoint3D.hpp>
//! MathTools
#include <MathTools/BasicMath/Compare.hpp>
//! gtest
#include <gtest/gtest.h>
//==============================================================================
namespace Tests
{
//==============================================================================
/*!
 * \brief Тестирование GeoPoint3D
 *        (конструктор на оcнове двух радиан и высоты)
 */
TEST( GeoPoint3DTest, ConstructorByRadian )
{
  using namespace BasicMath;

  GeoPoint2D point;
  double height( 0. );
  ASSERT_EQ( point, GeoPoint3D( ).get( ).first );
  ASSERT_TRUE( isEqual( height, GeoPoint3D( ).get( ).second ) );
  point.set( Radian( M_PI / 2 ), Radian( -M_PI / 2 ) );
  height = -10.;
  ASSERT_EQ( point, GeoPoint3D( Radian( M_PI / 2 ),
    Radian( -M_PI / 2 ), height ).get( ).first );
  ASSERT_TRUE( isEqual( height, GeoPoint3D( Radian( M_PI / 2 ),
    Radian( -M_PI / 2 ),
    height ).get( ).second ) );
  point.set( Radian( -M_PI * 2 ), Radian( M_PI * 2 ) );
  height = 0.25;
  ASSERT_EQ( point, GeoPoint3D( Radian( -M_PI * 2 ),
    Radian( M_PI * 2 ), height ).get( ).first );
  ASSERT_TRUE( isEqual( height, GeoPoint3D( Radian( -M_PI * 2 ),
    Radian( M_PI * 2 ),
    height ).get( ).second ) );
  point.set( Radian( 0.25 * RADIAN_PER_DEGREE ), Radian( -0.1 * RADIAN_PER_DEGREE ) );
  height = -0.1;
  ASSERT_EQ( point, GeoPoint3D( Radian( 0.25 * RADIAN_PER_DEGREE ),
    Radian( -0.1 * RADIAN_PER_DEGREE ),
    height ).get( ).first );
  ASSERT_TRUE( isEqual( height, GeoPoint3D( Radian( 0.25 * RADIAN_PER_DEGREE ),
    Radian( -0.1 * RADIAN_PER_DEGREE ),
    height ).get( ).second ) );
}
//==============================================================================
/*!
 * \brief Тестирование GeoPoint3D
 *        (конструктор на оcнове двух градусов и высоты)
 */
TEST( GeoPoint3DTest, ConstructorByDegree )
{
  using namespace BasicMath;

  GeoPoint2D point;
  double height( 0. );
  ASSERT_EQ( point, GeoPoint3D( ).get( ).first );
  ASSERT_TRUE( isEqual( height, GeoPoint3D( ).getHeight( ) ) );
  point.set( Degree( 90. ), Degree( -90. ) );
  height = -10.;
  ASSERT_EQ( point, GeoPoint3D( Degree( 90. ), Degree( -90. ), height ).get( ).first );
  ASSERT_TRUE( isEqual( height, GeoPoint3D( Degree( 90. ), Degree( -90. ),
    height ).getHeight( ) ) );
  point.set( Degree( -90.0 ), Degree( 90. ) );
  height = 0.25;
  ASSERT_EQ( point, GeoPoint3D( Degree( -90.0 ), Degree( 90. ), height ).get( ).first );
  ASSERT_TRUE( isEqual( height, GeoPoint3D( Degree( -90.0 ), Degree( 90. ),
    height ).getHeight( ) ) );
  point.set( Degree( 0.25 ), Degree( -0.1 ) );
  height = -0.1;
  ASSERT_EQ( point, GeoPoint3D( Degree( 0.25 ), Degree( -0.1 ), height ).get( ).first );
  ASSERT_TRUE( isEqual( height, GeoPoint3D( Degree( 0.25 ), Degree( -0.1 ),
    height ).getHeight( ) ) );
}
//==============================================================================
/*!
 * \brief Тестирование GeoPoint3D
 *        (конструктор на оcнове точки и высоты)
 */
TEST( GeoPoint3DTest, ConstructorByPoint )
{
  using namespace BasicMath;

  GeoPoint2D point;
  double height( 0. );
  ASSERT_EQ( point, GeoPoint3D( ).get( ).first );
  ASSERT_TRUE( isEqual( height, GeoPoint3D( ).getHeight( ) ) );
  point.set( Radian( M_PI / 2 ), Radian( -M_PI / 2 ) );
  height = -10.;
  ASSERT_EQ( point, GeoPoint3D( point, height ).get( ).first );
  ASSERT_TRUE( isEqual( height, GeoPoint3D( point, height ).getHeight( ) ) );
  point.set( Radian( -M_PI * 2 ), Radian( M_PI * 2 ) );
  height = 0.25;
  ASSERT_EQ( point, GeoPoint3D( point, height ).get( ).first );
  ASSERT_TRUE( isEqual( height, GeoPoint3D( point, height ).getHeight( ) ) );
  point.set( Radian( 0.25 * RADIAN_PER_DEGREE ), Radian( -0.1 * RADIAN_PER_DEGREE ) );
  height = -0.1;
  ASSERT_EQ( point, GeoPoint3D( point, height ).get( ).first );
  ASSERT_TRUE( isEqual( height, GeoPoint3D( point, height ).getHeight( ) ) );
}
//==============================================================================
/*!
 * \brief Тестирование GeoPoint3D
 *        (копирующий конструктор)
 */
TEST( GeoPoint3DTest, ConstructorCopy )
{
  GeoPoint3D geoPoint( GeoPoint2D( ), 0. );
  ASSERT_EQ( geoPoint, GeoPoint3D( geoPoint ) );
  geoPoint.set( GeoPoint2D( Radian( -M_PI / 2 ), Radian( M_PI / 2 ) ), 0. );
  ASSERT_EQ( geoPoint, GeoPoint3D( geoPoint ) );
  geoPoint.set( GeoPoint2D( Radian( 0.25 * RADIAN_PER_DEGREE ),
    Radian( -0.1 * RADIAN_PER_DEGREE ) ), 0. );
  ASSERT_EQ( geoPoint, GeoPoint3D( geoPoint ) );
}
//==============================================================================
/*!
 * \brief Тестирование set и get
 *        (присвоение значения на основе радиан)
 */
TEST( GeoPoint3DTest, GeoPoint3DGetSet )
{
  using namespace BasicMath;

  GeoPoint2D point2D;
  GeoPoint3D point3D;
  double height( 0. );
  point2D.set( Radian( M_PI / 2 ), Radian( -M_PI / 2 ) );
  height = -10.;
  point3D.set( point2D, height );
  ASSERT_EQ( point2D, point3D.get( ).first );
  ASSERT_TRUE( isEqual( height, point3D.getHeight( ) ) );
  point2D.set( Degree( -0.001 ), Degree( 1024.25 ) );
  height = 0.25;
  point3D.set( point2D, height );
  ASSERT_EQ( point2D, point3D.get( ).first );
  ASSERT_TRUE( isEqual( height, point3D.getHeight( ) ) );
  point2D.set( Degree( 0. ), Degree( 0. ) );
  height = 0.;
  point3D.set( point2D, height );
  ASSERT_EQ( point2D, point3D.get( ).first );
  ASSERT_TRUE( isEqual( height, point3D.getHeight( ) ) );
}
//==============================================================================
/*!
 * \brief Тестирование get2D (получение Point3D как Point2D)
 */
TEST(GeoPoint3DTest, GeoPoint3DGet2D)
{
  GeoPoint2D point2D;
  GeoPoint3D point3D;
  ASSERT_EQ(point2D, point3D.get2D());
  point2D.set(Radian(M_PI / 2), Radian(-M_PI / 2));
  point3D.set(point2D, -10.);
  ASSERT_EQ(point2D, point3D.get2D());
  point2D.set(Degree(-0.001), Degree(1024.25));
  point3D.set(point2D, 0.25);
  ASSERT_EQ(point2D, point3D.get2D());
}
//==============================================================================
/*!
 * \brief Тестирование getLatitude, getLongitude, getHeight
 *        (получение отдельных значений)
 */
TEST( GeoPoint3DTest, GeoPoint3DGetLatLonH )
{
  using namespace BasicMath;

  GeoPoint3D point3D;
  ASSERT_EQ( Radian( 0. ), point3D.getLatitude( ) );
  ASSERT_EQ( Radian( 0. ), point3D.getLongitude( ) );
  ASSERT_TRUE( isEqual( 0., point3D.getHeight( ) ) );
  point3D.set( GeoPoint2D( Radian( M_PI / 2 ), Radian( -M_PI / 2 ) ), -10. );
  ASSERT_EQ( Radian( M_PI / 2 ), point3D.getLatitude( ) );
  ASSERT_EQ( Radian( -M_PI / 2 ), point3D.getLongitude( ) );
  ASSERT_TRUE( isEqual( -10., point3D.getHeight( ) ) );
  point3D.set( GeoPoint2D( Degree( -0.001 ), Degree( 1024.25 ) ), 0.25 );
  ASSERT_EQ( Degree( -0.001 ).toRadians( ), point3D.getLatitude( ) );
  ASSERT_EQ( Degree( 1024.25 ).toRadians( ), point3D.getLongitude( ) );
  ASSERT_TRUE( isEqual( 0.25, point3D.getHeight( ) ) );
}
//==============================================================================
/*!
 * \brief Тестирование isCorrect
 *        (проверка на попадание координат в диапазон допустимых значений)
 */
TEST( GeoPoint3DTest, IsCorrect )
{
  GeoPoint3D point;
  ASSERT_TRUE( point.isCorrect( REGULAR ) );
  ASSERT_TRUE( point.isCorrect( NORMAL ) );
  point.set( GeoPoint2D( Degree( 90. ), Degree( 90. ) ), -15. );
  ASSERT_TRUE( point.isCorrect( REGULAR ) );
  ASSERT_TRUE( point.isCorrect( NORMAL ) );
  point.set( GeoPoint2D( Degree( -90. ), Degree( -90. ) ), 15. );
  ASSERT_TRUE( point.isCorrect( REGULAR ) );
  ASSERT_FALSE( point.isCorrect( NORMAL ) );
  point.set( GeoPoint2D( Degree( 180. ), Degree( 180. ) ), .15 );
  ASSERT_FALSE( point.isCorrect( REGULAR ) );
  ASSERT_FALSE( point.isCorrect( NORMAL ) );
  point.set( GeoPoint2D( Degree( 0. ), Degree( 180. ) ), -.15 );
  ASSERT_FALSE( point.isCorrect( REGULAR ) );
  ASSERT_TRUE( point.isCorrect( NORMAL ) );
  point.set( GeoPoint2D( Degree( 0. ), Degree( -180. ) ), 9999. );
  ASSERT_TRUE( point.isCorrect( REGULAR ) );
  ASSERT_FALSE( point.isCorrect( NORMAL ) );
  point.set( GeoPoint2D( Degree( 0. ), Degree( 360. ) ), -9999. );
  ASSERT_FALSE( point.isCorrect( REGULAR ) );
  ASSERT_FALSE( point.isCorrect( NORMAL ) );
  point.set( GeoPoint2D( Degree( 0. ), Degree( -360. ) ), 0. );
  ASSERT_FALSE( point.isCorrect( REGULAR ) );
  ASSERT_FALSE( point.isCorrect( NORMAL ) );
  point.set( GeoPoint2D( Degree( 0. ), Degree( 179.999999999 ) ), 0. );
  ASSERT_TRUE( point.isCorrect( REGULAR ) );
  ASSERT_TRUE( point.isCorrect( NORMAL ) );
  point.set( GeoPoint2D( Degree( 0. ), Degree( 359.999999999 ) ), 0. );
  ASSERT_FALSE( point.isCorrect( REGULAR ) );
  ASSERT_TRUE( point.isCorrect( NORMAL ) );
}
//==============================================================================
/*!
 * \brief Тестирование normalize
 *        (приведение долготы к заданному диапазону значений)
 */
TEST( GeoPoint3DTest, Normalize )
{
  using namespace BasicMath;

  GeoPoint2D point2D( Degree( 45. ), Degree( 90. ) );
  double height( 0. );
  GeoPoint3D point3D( point2D, height );
  point3D.normalize( REGULAR );
  ASSERT_EQ( point2D, point3D.get( ).first );
  ASSERT_TRUE( isEqual( height, point3D.getHeight( ) ) );
  point3D.normalize( NORMAL );
  ASSERT_EQ( GeoPoint2D( Degree( 45. ), Degree( 90. ) ), point3D.get( ).first );
  ASSERT_TRUE( isEqual( height, point3D.getHeight( ) ) );

  point2D.set( Degree( 45. ), Degree( 180. ) );
  height = 9999.99;
  point3D.set( point2D, height );
  point3D.normalize( REGULAR );
  ASSERT_EQ( GeoPoint2D( Degree( 45. ), Degree( -180. ) ), point3D.get( ).first );
  ASSERT_TRUE( isEqual( height, point3D.getHeight( ) ) );
  point3D.set( point2D, height );
  point3D.normalize( NORMAL );
  ASSERT_EQ( GeoPoint2D( Degree( 45. ), Degree( 180. ) ), point3D.get( ).first );
  ASSERT_TRUE( isEqual( height, point3D.getHeight( ) ) );

  point2D.set( Degree( 45. ), Degree( -180. ) );
  height = -9999.99;
  point3D.set( point2D, height );
  point3D.normalize( REGULAR );
  ASSERT_EQ( GeoPoint2D( Degree( 45. ), Degree( -180. ) ), point3D.get( ).first );
  ASSERT_TRUE( isEqual( height, point3D.getHeight( ) ) );
  point3D.normalize( NORMAL );
  ASSERT_EQ( GeoPoint2D( Degree( 45. ), Degree( 180. ) ), point3D.get( ).first );
  ASSERT_TRUE( isEqual( height, point3D.getHeight( ) ) );

  point2D.set( Degree( 45. ), Degree( -45. ) );
  height = 0.12345;
  point3D.set( point2D, height );
  point3D.normalize( REGULAR );
  ASSERT_EQ( GeoPoint2D( Degree( 45. ), Degree( -45. ) ), point3D.get( ).first );
  ASSERT_TRUE( isEqual( height, point3D.getHeight( ) ) );
  point3D.normalize( NORMAL );
  ASSERT_EQ( GeoPoint2D( Degree( 45. ), Degree( 315. ) ), point3D.get( ).first );
  ASSERT_TRUE( isEqual( height, point3D.getHeight( ) ) );

  point2D.set( Degree( 45. ), Degree( 135. ) );
  height = -0.12345;
  point3D.set( point2D, height );
  point3D.normalize( REGULAR );
  ASSERT_EQ( GeoPoint2D( Degree( 45. ), Degree( 135. ) ), point3D.get( ).first );
  ASSERT_TRUE( isEqual( height, point3D.getHeight( ) ) );
  point3D.normalize( NORMAL );
  ASSERT_EQ( GeoPoint2D( Degree( 45. ), Degree( 135. ) ), point3D.get( ).first );
  ASSERT_TRUE( isEqual( height, point3D.getHeight( ) ) );

  point2D.set( Degree( 180. ), Degree( 360. ) );
  height = 0.;
  point3D.set( point2D, height );
  point3D.normalize( REGULAR );
  ASSERT_EQ( GeoPoint2D( Degree( 180. ), Degree( 0. ) ), point3D.get( ).first );
  ASSERT_TRUE( isEqual( height, point3D.getHeight( ) ) );
  point3D.set( point2D, height );
  point3D.normalize( NORMAL );
  ASSERT_EQ( GeoPoint2D( Degree( 180. ), Degree( 0. ) ), point3D.get( ).first );
  ASSERT_TRUE( isEqual( height, point3D.getHeight( ) ) );

  point2D.set( Degree( 180. ), Degree( -360. ) );
  point3D.set( point2D, height );
  point3D.normalize( REGULAR );
  ASSERT_EQ( GeoPoint2D( Degree( 180. ), Degree( 0. ) ), point3D.get( ).first );
  ASSERT_TRUE( isEqual( height, point3D.getHeight( ) ) );
  point3D.set( point2D, height );
  point3D.normalize( NORMAL );
  ASSERT_EQ( GeoPoint2D( Degree( 180. ), Degree( 0. ) ), point3D.get( ).first );
  ASSERT_TRUE( isEqual( height, point3D.getHeight( ) ) );
}
//==============================================================================
/*!
 * \brief Тестирование operator ==
 *        Cравнение двух точек на равенство
 */
TEST( GeoPoint3DTest, GeoPoint3DEqual )
{
  GeoPoint3D geoPoint;
  ASSERT_TRUE( geoPoint == GeoPoint3D( ) );
  GeoPoint3D geoPoint1( GeoPoint2D( Radian( -M_PI ), Radian( M_PI ) ), 5. );
  GeoPoint3D geoPoint2( GeoPoint2D( Radian( -M_PI ), Radian( M_PI ) ), 5. );
  ASSERT_TRUE( geoPoint1 == geoPoint2 );
  ASSERT_TRUE( geoPoint2 == geoPoint1 );
  GeoPoint3D geoPoint3( GeoPoint2D( Radian( M_PI ), Radian( -M_PI ) ), 5. );
  ASSERT_FALSE( geoPoint1 == geoPoint3 );
}
//==============================================================================
/*!
 * \brief Тестирование operator !=
 *        Покоординатное сравнение двух точек на неравенство
 */
TEST(GeoPoint3DTest, GeoPoint3DNotEqual)
{
  GeoPoint3D point1;
  GeoPoint3D point2(Radian(0.1), Radian(-1.), 5.);
  ASSERT_TRUE(point1 != point2);

  point2.set(GeoPoint2D(Radian(0.0000000000000001),
                        Radian(0.0000000000000001)),
             0.0000000000000001);
  ASSERT_FALSE(point1 != point2);
}
//==============================================================================
} //! Tests
//==============================================================================
