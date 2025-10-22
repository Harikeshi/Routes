#include <Geodesic/GeodesicCartesianTransition/Transition.hpp>
//! MathTools
#include <MathTools/BasicMath/Compare.hpp>
//! gtest
#include <gtest/gtest.h>
//==============================================================================
namespace Geodesic
{
namespace Tests
{
//==============================================================================
constexpr double  ACCURACY_HEIGHT( 1. );          //!< точность расчёта высоты
constexpr double ACCURACY_TRANSITION_METER( 1. ); //!< точность при переводе в м.  
constexpr double ACCURACY_TRANSITION_RADIAN       //!< точность при переводе в     
                          = ACCURACY_RADIAN * 10; //!  радианы
//==============================================================================
typedef std::vector<GeoPoint3D> GeoPoints3D;
typedef std::vector<Point3D> Points3D;
//==============================================================================
/*!
 * \brief Тестирование перехода из геодезических в декартовы координаты
*/
TEST( GeodesicCartesianTransitionTest, GeodesicToCartesian )
{
  using namespace BasicMath;

  Transition transition( std::make_shared<Ellipsoid>( EL_WGS_84 ) );

  GeoPoints3D geoPoints;
  //! 00°00'00.00"С.Ш. 000°00'00.00"В.Д. 0 м.
  geoPoints.emplace_back( Degree( 0. ), Degree( 0. ), 0. );
  //! 42°00'00.00"С.Ш. 142°00'00.00"В.Д. 100 м.
  geoPoints.emplace_back( Degree( 42. ), Degree( 142. ), 100. );
  //! 42°00'00.00"С.Ш. 142°00'00.00"З.Д. 1000 м.
  geoPoints.emplace_back( Degree( 42. ), Degree( -142. ), 1000. );
  //! 42°00'00.00"Ю.Ш. 142°00'00.00"В.Д. 10000 м.
  geoPoints.emplace_back( Degree( -42. ), Degree( 142. ), 10000. );
  //! 42°00'00.00"Ю.Ш. 142°00'00.00"З.Д. -1000 м.
  geoPoints.emplace_back( Degree( -42. ), Degree( -142. ), -1000. );
  //! 90°00'00.00"С.Ш. 090°00'00.00"В.Д. 0 м.
  geoPoints.emplace_back( Degree( 90. ), Degree( 90. ), 0. );

  Points3D testPoints; //!< Данные для WGS-84 Эпоха 2021.1
  testPoints.emplace_back( 6378136.59508, 0.23683999999999886,
    0.13751999999999934 );
  testPoints.emplace_back( -3740745.1999016423, 2922590.3775889887,
    4245670.886682217 );
  testPoints.emplace_back( -3741272.245404209, -2923001.67698466,
    4246273.104227941 );
  testPoints.emplace_back( -3746542.7004298796, 2927119.881421366,
    -4252295.00465169 );
  testPoints.emplace_back( -3740101.033176282, -2922086.625705391,
    -4244934.567975222 );
  testPoints.emplace_back( -0.40491999999999806, -0.23684001033882762,
    6356752.451765632 );


  Points3D points;
  for( int i = 0; i < testPoints.size( ); i++ )
  {
    points.emplace_back( transition.fromGeodesicToCartesian( geoPoints[i] ) );

    ASSERT_TRUE( isEqual( testPoints[i].getX( ), points[i].getX( ),
      ACCURACY_TRANSITION_METER ) );
    ASSERT_TRUE( isEqual( testPoints[i].getY( ), points[i].getY( ),
      ACCURACY_TRANSITION_METER ) );
    ASSERT_TRUE( isEqual( testPoints[i].getZ( ), points[i].getZ( ),
      ACCURACY_TRANSITION_METER ) );
  }
}
//==============================================================================
/*!
* \brief Тестирование перехода из декартовых в геодезические координаты
*/
TEST( GeodesicCartesianTransitionTest, CartesianToGeodesic )
{
  using namespace BasicMath;

  Transition transition( std::make_shared<Ellipsoid>( EL_WGS_84 ), ACCURACY_RADIAN );

  Points3D points; //!< Данные для WGS-84 Эпоха 2021.1
  points.emplace_back( 6378136.59508, 0.23683999999999886,
    0.13751999999999934 );
  points.emplace_back( -3740745.1999016423, 2922590.3775889887,
    4245670.886682217 );
  points.emplace_back( -3741272.245404209, -2923001.67698466,
    4246273.104227941 );
  points.emplace_back( -3746542.7004298796, 2927119.881421366,
    -4252295.00465169 );
  points.emplace_back( -3740101.033176282, -2922086.625705391,
    -4244934.567975222 );
  points.emplace_back( 0., 0.,
    6356752. );


  GeoPoints3D testGeoPoints;
  //! 00°00'00.00"С.Ш. 000°00'00.00"В.Д. 0 м.
  testGeoPoints.emplace_back( Degree( 0. ), Degree( 0. ), 0. );
  //! 42°00'00.00"С.Ш. 142°00'00.00"В.Д. 100 м.
  testGeoPoints.emplace_back( Degree( 42. ), Degree( 142. ), 100. );
  //! 42°00'00.00"С.Ш. 142°00'00.00"З.Д. 1000 м.
  testGeoPoints.emplace_back( Degree( 42. ), Degree( -142. ), 1000. );
  //! 42°00'00.00"Ю.Ш. 142°00'00.00"В.Д. 10000 м.
  testGeoPoints.emplace_back( Degree( -42. ), Degree( 142. ), 10000. );
  //! 42°00'00.00"Ю.Ш. 142°00'00.00"З.Д. -1000 м.
  testGeoPoints.emplace_back( Degree( -42. ), Degree( -142. ), -1000. );
  //! 90°00'00.00"С.Ш. 000°00'00.00"В.Д. 0 м.
  testGeoPoints.emplace_back( Degree( 90. ), Degree( 0. ), 0. );

  GeoPoints3D geoPoints;
  for( int i = 0; i < testGeoPoints.size( ); i++ )
  {
    geoPoints.emplace_back( transition.fromCartesianToGeodesic( points[i] ) );
    geoPoints[i].normalize( REGULAR );

    ASSERT_TRUE( isEqual( testGeoPoints[i].getLatitude( ),
      geoPoints[i].getLatitude( ),
      ACCURACY_TRANSITION_RADIAN) );
    ASSERT_TRUE( isEqual( testGeoPoints[i].getLongitude( ),
      geoPoints[i].getLongitude( ),
      ACCURACY_TRANSITION_RADIAN) );
    ASSERT_TRUE( isEqual( testGeoPoints[i].getHeight( ), geoPoints[i].getHeight( ),
      ACCURACY_HEIGHT ) );
  }
}
//==============================================================================
/*!
* \brief Тестирование получение эллипсоида
*/
TEST( GeodesicCartesianTransitionTest, GetEllipsoid )
{
  using namespace BasicMath;

  //! Создаём эллипсоид 
  std::shared_ptr<Ellipsoid> ellipsoid = std::make_shared<Ellipsoid>(
    EL_KRASOVSKY );

  //! Создаём переводчик на основе эллипсоида
  Transition transition( ellipsoid );

  //! Получаем эллипсоид из переводчика
  std::shared_ptr<Ellipsoid> ellipsoidTransition = transition.getEllipsoid( );

  //! Сравниваем основные параметры
  ASSERT_TRUE( isEqual( ellipsoid->getMajorRadius( ),
    ellipsoidTransition->getMajorRadius( ) ) );
  ASSERT_TRUE( isEqual( ellipsoid->getFlatenning( ),
    ellipsoidTransition->getFlatenning( ) ) );
  ASSERT_TRUE( isEqual( ellipsoid->getMinorRadius( ),
    ellipsoidTransition->getMinorRadius( ) ) );
}
//==============================================================================
} //! Tests
} //! Geodesic
//==============================================================================
