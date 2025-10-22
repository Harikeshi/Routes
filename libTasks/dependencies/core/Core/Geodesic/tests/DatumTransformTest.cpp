#include <Geodesic/DatumTransform/DatumTransform.hpp>
//! MathTools
#include <MathTools/BasicMath/Compare.hpp>
//! PrimaryEntities
#include <PrimaryEntities/GeodesicEntities.hpp>
//! AbstractOperations
#include <AbstractOperations/ChangeEachPoint.hpp>
//! gtest
#include <gtest/gtest.h>
//==============================================================================
namespace Geodesic
{
namespace Tests
{
//==============================================================================
typedef std::vector<GeoPoint3D> GeoPoints3D;
//==============================================================================
/*!
 * \brief Класс данных для тестирования преобразований СК
*/
class Transform
{
  //==============================================================================
public:
  Transform( )
    : transformator( DatumTransform( CS_SK_42, CS_WGS_84 ) )
  {
    //! Формирование набора точек в СК-42

    //! 00°00'00.00"С.Ш. 000°00'00.00"В.Д.
    m2DPointsSK.emplace_back( Radian( 0. ), Radian( 0. ) );
    //! 42°00'00.00"С.Ш. 142°00'00.00"В.Д.
    m2DPointsSK.emplace_back( Radian( 0.733038285837618 ),
      Radian( 2.478367537831948 ) );
    //! 42°00'00.00"С.Ш. 142°00'00.00"З.Д.
    m2DPointsSK.emplace_back( Radian( 0.733038285837618 ),
      Radian( -2.478367537831948 ) );
    //! 42°00'00.00"Ю.Ш. 142°00'00.00"В.Д.
    m2DPointsSK.emplace_back( Radian( -0.733038285837618 ),
      Radian( 2.478367537831948 ) );
    //! 42°00'00.00"Ю.Ш. 142°00'00.00"З.Д.
    m2DPointsSK.emplace_back( Radian( -0.733038285837618 ),
      Radian( -2.478367537831948 ) );

    m3DPointsSK.emplace_back( m2DPointsSK[0], 0. );
    m3DPointsSK.emplace_back( m2DPointsSK[1], 0. );
    m3DPointsSK.emplace_back( m2DPointsSK[2], 0. );
    m3DPointsSK.emplace_back( m2DPointsSK[3], 0. );
    m3DPointsSK.emplace_back( m2DPointsSK[4], 0. );

    //! Формирование набора точек в WGS84

    //! 00°00'02.95"Ю.Ш. 000°00'03.77"З.Д.
    m2DPointsWGS.emplace_back( Radian( -0.000014303829981 ),
      Radian( -0.000018268465004 ) );
    //! 42°00'00.73"С.Ш. 142°00'04.79"В.Д.
    m2DPointsWGS.emplace_back( Radian( 0.733041805451208 ),
      Radian( 2.478390774783984 ) );
    //! 41°59'56.96"С.Ш. 141°59'53.56"З.Д.
    m2DPointsWGS.emplace_back( Radian( 0.733023557553191 ),
      Radian( -2.478336319436707 ) );
    //! 42°00'04.02"Ю.Ш. 142°00'05.18"В.Д.
    m2DPointsWGS.emplace_back( Radian( -0.733057765858324 ),
      Radian( 2.478392643450590 ) );
    //! 42°00'00.25"Ю.Ш. 141°59'53.95"З.Д.
    m2DPointsWGS.emplace_back( Radian( -0.733039517994239 ),
      Radian( -2.478338188165336 ) );

    m3DPointsWGS.emplace_back( m2DPointsWGS[0], 130.168488 );
    m3DPointsWGS.emplace_back( m2DPointsWGS[1], -23.850769 );
    m3DPointsWGS.emplace_back( m2DPointsWGS[2], 105.127129 );
    m3DPointsWGS.emplace_back( m2DPointsWGS[3], 82.886948 );
    m3DPointsWGS.emplace_back( m2DPointsWGS[4], 211.862325 );

    //! Формирование набора точек в ПЗ-90.02

    //! 00°00'02.96"Ю.Ш. 000°00'03.77"З.Д.
    m2DPointsPZ02.emplace_back( Radian( -0.000014332240188 ),
      Radian( -0.000018281006564 ) );
    //! 42°00'00.73"С.Ш. 142°00'04.79"В.Д.
    m2DPointsPZ02.emplace_back( Radian( 0.733041819438605 ),
      Radian( 2.478390741375373 ) );
    //! 41°59'56.96"С.Ш. 141°59'53.55"З.Д.
    m2DPointsPZ02.emplace_back( Radian( 0.733023561181110 ),
      Radian( -2.478336259466633 ) );
    //! 42°00'04.03"Ю.Ш. 142°00'05.17"В.Д.
    m2DPointsPZ02.emplace_back( Radian( -0.733057821880815 ),
      Radian( 2.478392610042191 ) );
    //! 42°00'00.26"Ю.Ш. 141°59'53.93"З.Д.
    m2DPointsPZ02.emplace_back( Radian( -0.733039563659607 ),
      Radian( -2.478338128195498 ) );

    m3DPointsPZ02.emplace_back( m2DPointsPZ02[0], 130.528503 );
    m3DPointsPZ02.emplace_back( m2DPointsPZ02[1], -24.218634 );
    m3DPointsPZ02.emplace_back( m2DPointsPZ02[2], 104.832474 );
    m3DPointsPZ02.emplace_back( m2DPointsPZ02[3], 82.759972 );
    m3DPointsPZ02.emplace_back( m2DPointsPZ02[4], 211.808868 );

    //! Формирование набора точек в ПЗ-90.11

    //! 00°00'02.95"Ю.Ш. 000°00'03.76"З.Д.
    m2DPointsPZ11.emplace_back( Radian( -0.000014283079689 ),
      Radian( -0.000018231435293 ) );
    //! 42°00'00.72"С.Ш. 142°00'04.80"В.Д.
    m2DPointsPZ11.emplace_back( Radian( 0.733041793444055 ),
      Radian( 2.478390796592947 ) );
    //! 41°59'56.96"С.Ш. 141°59'53.56"З.Д.
    m2DPointsPZ11.emplace_back( Radian( 0.733023545529824 ),
      Radian( -2.478336319899279 ) );
    //! 42°00'04.02"Ю.Ш. 142°00'05.18"В.Д.
    m2DPointsPZ11.emplace_back( Radian( -0.733057762039392 ),
      Radian( 2.478392630641111 ) );
    //! 42°00'00.26"Ю.Ш. 141°59'53.95"З.Д.
    m2DPointsPZ11.emplace_back( Radian( -0.733039541640397 ),
      Radian( -2.478338185444009 ) );

    m3DPointsPZ11.emplace_back( m2DPointsPZ11[0], 130.104462 );
    m3DPointsPZ11.emplace_back( m2DPointsPZ11[1], -23.831028 );
    m3DPointsPZ11.emplace_back( m2DPointsPZ11[2], 105.049576 );
    m3DPointsPZ11.emplace_back( m2DPointsPZ11[3], 82.877525 );
    m3DPointsPZ11.emplace_back( m2DPointsPZ11[4], 211.756500 );
  }
  //==============================================================================
  ~Transform( )
  {
  }
  //==============================================================================
    //! Преобразователи СК
  DatumTransform transformator; //!< преобразователь координат между СК

  //! Координаты точек в различных СК
  GeoMultiPoint2D m2DPointsSK;    //!< набор геоточек 2D в СК СК-42
  GeoPoints3D m3DPointsSK;    //!< набор геоточек 3D в СК СК-42
  GeoMultiPoint2D m2DPointsWGS;   //!< набор геоточек 2D в СК WGS84
  GeoPoints3D m3DPointsWGS;   //!< набор геоточек 3D в СК WGS84
  GeoMultiPoint2D m2DPointsPZ02;  //!< набор геоточек 2D в СК ПЗ-90.02 
  GeoPoints3D m3DPointsPZ02;  //!< набор геоточек 3D в СК ПЗ-90.02
  GeoMultiPoint2D m2DPointsPZ11;  //!< набор геоточек 2D в СК ПЗ-90.11
  GeoPoints3D m3DPointsPZ11;  //!< набор геоточек 3D в СК ПЗ-90.11
//==============================================================================
};
//==============================================================================
class TransformationTest : public ::testing::Test
{
protected:
  void SetUp( )
  {
    transform = new Transform;
  }

  void TearDown( )
  {
    delete transform;
  }

  Transform* transform;
};
//==============================================================================
/*!
 * \brief Тестирование преобразование СК-42 -> WGS84
*/
TEST_F( TransformationTest, SKtoWGS )
{
  using namespace BasicMath;

  transform->transformator.setSystems( CS_SK_42, CS_WGS_84 );

  //! Берем точки в СК-42
  GeoMultiPoint2D testPoints2D( transform->m2DPointsSK );
  GeoPoints3D testPoints3D( transform->m3DPointsSK );

  //! Преобразуем в WGS84
  AbstractOperations::changeEachPoint(
    testPoints2D, transform->transformator );

  for( auto& point : testPoints3D )
    transform->transformator( point );

  //! Сравниваем результат с контрольными точками в WGS84
  for( int i = 0; i < testPoints2D.size( ); i++ )
  {
    ASSERT_TRUE( isEqual( transform->m2DPointsWGS[i].getLatitude( ),
      testPoints2D[i].getLatitude( ) ) );
    ASSERT_TRUE( isEqual( transform->m2DPointsWGS[i].getLongitude( ),
      testPoints2D[i].getLongitude( ) ) );

    ASSERT_EQ( std::round( transform->m3DPointsWGS[i].getHeight( ) ),
      std::round( testPoints3D[i].getHeight( ) ) );
  }
}
//==============================================================================
/*!
* \brief Тестирование преобразование WGS84 -> СК-42
*/
TEST_F( TransformationTest, WGStoSK )
{
  using namespace BasicMath;

  transform->transformator.setSystems( CS_WGS_84, CS_SK_42 );

  //! Берем точки в WGS84
  GeoMultiPoint2D testPoints2D( transform->m2DPointsWGS );
  GeoPoints3D testPoints3D( transform->m3DPointsWGS );

  //! Преобразуем в СК-42
  AbstractOperations::changeEachPoint(
    testPoints2D, transform->transformator );

  for( auto& point : testPoints3D )
    transform->transformator( point );

  //! Сравниваем результат с контрольными точками в СК-42
  for( int i = 0; i < testPoints2D.size( ); i++ )
  {
    ASSERT_TRUE( isEqual( transform->m2DPointsSK[i].getLatitude( ),
      testPoints2D[i].getLatitude( ) ) );
    ASSERT_TRUE( isEqual( transform->m2DPointsSK[i].getLongitude( ),
      testPoints2D[i].getLongitude( ) ) );

    ASSERT_EQ( std::round( transform->m3DPointsSK[i].getHeight( ) ),
      std::round( testPoints3D[i].getHeight( ) ) );
  }
}
//==============================================================================
/*!
* \brief Тестирование преобразование СК-42 -> ПЗ-90.02
*/
TEST_F( TransformationTest, SKtoPZ02 )
{
  using namespace BasicMath;

  transform->transformator.setSystems( CS_SK_42, CS_PZ_90_02 );

  //! Берем точки в СК-42
  GeoMultiPoint2D testPoints2D( transform->m2DPointsSK );
  GeoPoints3D testPoints3D( transform->m3DPointsSK );

  //! Преобразуем в ПЗ-90.02
  AbstractOperations::changeEachPoint(
    testPoints2D, transform->transformator );

  for( auto& point : testPoints3D )
    transform->transformator( point );

  //! Сравниваем результат с контрольными точками в ПЗ-90.02
  for( int i = 0; i < testPoints2D.size( ); i++ )
  {
    ASSERT_TRUE( isEqual( transform->m2DPointsPZ02[i].getLatitude( ),
      testPoints2D[i].getLatitude( ) ) );
    ASSERT_TRUE( isEqual( transform->m2DPointsPZ02[i].getLongitude( ),
      testPoints2D[i].getLongitude( ) ) );

    ASSERT_EQ( std::round( transform->m3DPointsPZ02[i].getHeight( ) ),
      std::round( testPoints3D[i].getHeight( ) ) );
  }
}
//==============================================================================
/*!
* \brief Тестирование преобразование ПЗ-90.02 -> СК-42
*/
TEST_F( TransformationTest, PZ02toSK )
{
  using namespace BasicMath;

  transform->transformator.setSystems( CS_PZ_90_02, CS_SK_42 );

  //! Берем точки в ПЗ-90.02
  GeoMultiPoint2D testPoints2D( transform->m2DPointsPZ02 );
  GeoPoints3D testPoints3D( transform->m3DPointsPZ02 );

  //! Преобразуем в СК-42
  AbstractOperations::changeEachPoint(
    testPoints2D, transform->transformator );

  for( auto& point : testPoints3D )
    transform->transformator( point );

  //! Сравниваем результат с контрольными точками в СК-42
  for( int i = 0; i < testPoints2D.size( ); i++ )
  {
    ASSERT_TRUE( isEqual( transform->m2DPointsSK[i].getLatitude( ),
      testPoints2D[i].getLatitude( ) ) );
    ASSERT_TRUE( isEqual( transform->m2DPointsSK[i].getLongitude( ),
      testPoints2D[i].getLongitude( ) ) );

    ASSERT_EQ( std::round( transform->m3DPointsSK[i].getHeight( ) ),
      std::round( testPoints3D[i].getHeight( ) ) );
  }
}
//==============================================================================
/*!
* \brief Тестирование преобразование СК-42 -> ПЗ-90.11
*/
TEST_F( TransformationTest, SKtoPZ11 )
{
  using namespace BasicMath;

  transform->transformator.setSystems( CS_SK_42, CS_PZ_90_11 );

  //! Берем точки в СК-42
  GeoMultiPoint2D testPoints2D( transform->m2DPointsSK );
  GeoPoints3D testPoints3D( transform->m3DPointsSK );

  //! Преобразуем в ПЗ-90.11
  AbstractOperations::changeEachPoint(
    testPoints2D, transform->transformator );

  for( auto& point : testPoints3D )
    transform->transformator( point );

  //! Сравниваем результат с контрольными точками в ПЗ-90.11
  for( int i = 0; i < testPoints2D.size( ); i++ )
  {
    ASSERT_TRUE( isEqual( transform->m2DPointsPZ11[i].getLatitude( ),
      testPoints2D[i].getLatitude( ) ) );
    ASSERT_TRUE( isEqual( transform->m2DPointsPZ11[i].getLongitude( ),
      testPoints2D[i].getLongitude( ) ) );

    ASSERT_EQ( std::round( transform->m3DPointsPZ11[i].getHeight( ) ),
      std::round( testPoints3D[i].getHeight( ) ) );
  }
}
//==============================================================================
/*!
* \brief Тестирование преобразование ПЗ-90.11 -> СК-42
*/
TEST_F( TransformationTest, PZ11toSK )
{
  using namespace BasicMath;

  transform->transformator.setSystems( CS_PZ_90_11, CS_SK_42 );

  //! Берем точки в ПЗ-90.11
  GeoMultiPoint2D testPoints2D( transform->m2DPointsPZ11 );
  GeoPoints3D testPoints3D( transform->m3DPointsPZ11 );

  //! Преобразуем в СК-42
  AbstractOperations::changeEachPoint(
    testPoints2D, transform->transformator );

  for( auto& point : testPoints3D )
    transform->transformator( point );

  //! Сравниваем результат с контрольными точками в СК-42
  for( int i = 0; i < testPoints2D.size( ); i++ )
  {
    ASSERT_TRUE( isEqual( transform->m2DPointsSK[i].getLatitude( ),
      testPoints2D[i].getLatitude( ) ) );
    ASSERT_TRUE( isEqual( transform->m2DPointsSK[i].getLongitude( ),
      testPoints2D[i].getLongitude( ) ) );

    ASSERT_EQ( std::round( transform->m3DPointsSK[i].getHeight( ) ),
      std::round( testPoints3D[i].getHeight( ) ) );
  }
}
//==============================================================================
/*!
* \brief Тестирование преобразование WGS84 -> ПЗ-90.02
*/
TEST_F( TransformationTest, WGStoPZ02 )
{
  using namespace BasicMath;

  transform->transformator.setSystems( CS_WGS_84, CS_PZ_90_02 );

  //! Берем точки в WGS84
  GeoMultiPoint2D testPoints2D( transform->m2DPointsWGS );
  GeoPoints3D testPoints3D( transform->m3DPointsWGS );

  //! Преобразуем в ПЗ-90.02
  AbstractOperations::changeEachPoint(
    testPoints2D, transform->transformator );

  for( auto& point : testPoints3D )
    transform->transformator( point );

  //! Сравниваем результат с контрольными точками в ПЗ-90.02
  for( int i = 0; i < testPoints2D.size( ); i++ )
  {
    ASSERT_TRUE( isEqual( transform->m2DPointsPZ02[i].getLatitude( ),
      testPoints2D[i].getLatitude( ) ) );
    ASSERT_TRUE( isEqual( transform->m2DPointsPZ02[i].getLongitude( ),
      testPoints2D[i].getLongitude( ) ) );

    ASSERT_EQ( std::round( transform->m3DPointsPZ02[i].getHeight( ) ),
      std::round( testPoints3D[i].getHeight( ) ) );
  }
}
//==============================================================================
/*!
* \brief Тестирование преобразование ПЗ-90.02 -> WGS84
*/
TEST_F( TransformationTest, PZ02toWGS )
{
  using namespace BasicMath;

  transform->transformator.setSystems( CS_PZ_90_02, CS_WGS_84 );

  //! Берем точки в ПЗ-90.11
  GeoMultiPoint2D testPoints2D( transform->m2DPointsPZ02 );
  GeoPoints3D testPoints3D( transform->m3DPointsPZ02 );

  //! Преобразуем в WGS84
  AbstractOperations::changeEachPoint(
    testPoints2D, transform->transformator );

  for( auto& point : testPoints3D )
    transform->transformator( point );

  //! Сравниваем результат с контрольными точками в WGS84
  for( int i = 0; i < testPoints2D.size( ); i++ )
  {
    ASSERT_TRUE( isEqual( transform->m2DPointsWGS[i].getLatitude( ),
      testPoints2D[i].getLatitude( ) ) );
    ASSERT_TRUE( isEqual( transform->m2DPointsWGS[i].getLongitude( ),
      testPoints2D[i].getLongitude( ) ) );

    ASSERT_EQ( std::round( transform->m3DPointsWGS[i].getHeight( ) ),
      std::round( testPoints3D[i].getHeight( ) ) );
  }
}
//==============================================================================
/*!
* \brief Тестирование преобразование WGS84 -> ПЗ-90.11
*/
TEST_F( TransformationTest, WGStoPZ11 )
{
  using namespace BasicMath;

  transform->transformator.setSystems( CS_WGS_84, CS_PZ_90_11 );

  //! Берем точки в WGS84
  GeoMultiPoint2D testPoints2D( transform->m2DPointsWGS );
  GeoPoints3D testPoints3D( transform->m3DPointsWGS );

  //! Преобразуем в ПЗ-90.11
  AbstractOperations::changeEachPoint(
    testPoints2D, transform->transformator );

  for( auto& point : testPoints3D )
    transform->transformator( point );

  //! Сравниваем результат с контрольными точками в ПЗ-90.11
  for( int i = 0; i < testPoints2D.size( ); i++ )
  {
    ASSERT_TRUE( isEqual( transform->m2DPointsPZ11[i].getLatitude( ),
      testPoints2D[i].getLatitude( ) ) );
    ASSERT_TRUE( isEqual( transform->m2DPointsPZ11[i].getLongitude( ),
      testPoints2D[i].getLongitude( ) ) );

    ASSERT_EQ( std::round( transform->m3DPointsPZ11[i].getHeight( ) ),
      std::round( testPoints3D[i].getHeight( ) ) );
  }
}
//==============================================================================
/*!
* \brief Тестирование преобразование ПЗ-90.11 -> WGS84
*/
TEST_F( TransformationTest, PZ11toWGS )
{
  using namespace BasicMath;

  transform->transformator.setSystems( CS_PZ_90_11, CS_WGS_84 );

  //! Берем точки в ПЗ-90.11
  GeoMultiPoint2D testPoints2D( transform->m2DPointsPZ11 );
  GeoPoints3D testPoints3D( transform->m3DPointsPZ11 );

  //! Преобразуем в WGS84
  AbstractOperations::changeEachPoint(
    testPoints2D, transform->transformator );

  for( auto& point : testPoints3D )
    transform->transformator( point );

  //! Сравниваем результат с контрольными точками в WGS84
  for( int i = 0; i < testPoints2D.size( ); i++ )
  {
    ASSERT_TRUE( isEqual( transform->m2DPointsWGS[i].getLatitude( ),
      testPoints2D[i].getLatitude( ) ) );
    ASSERT_TRUE( isEqual( transform->m2DPointsWGS[i].getLongitude( ),
      testPoints2D[i].getLongitude( ) ) );

    ASSERT_EQ( std::round( transform->m3DPointsWGS[i].getHeight( ) ),
      std::round( testPoints3D[i].getHeight( ) ) );
  }
}
//==============================================================================
/*!
* \brief Тестирование преобразование ПЗ-90.02 -> ПЗ-90.11
*/
TEST_F( TransformationTest, PZ02toPZ11 )
{
  using namespace BasicMath;

  transform->transformator.setSystems( CS_PZ_90_02, CS_PZ_90_11 );

  //! Берем точки в ПЗ-90.02
  GeoMultiPoint2D testPoints2D( transform->m2DPointsPZ02 );
  GeoPoints3D testPoints3D( transform->m3DPointsPZ02 );

  //! Преобразуем в ПЗ-90.11
  AbstractOperations::changeEachPoint(
    testPoints2D, transform->transformator );

  for( auto& point : testPoints3D )
    transform->transformator( point );

  //! Сравниваем результат с контрольными точками в ПЗ-90.11
  for( int i = 0; i < testPoints2D.size( ); i++ )
  {
    ASSERT_TRUE( isEqual( transform->m2DPointsPZ11[i].getLatitude( ),
      testPoints2D[i].getLatitude( ) ) );
    ASSERT_TRUE( isEqual( transform->m2DPointsPZ11[i].getLongitude( ),
      testPoints2D[i].getLongitude( ) ) );

    ASSERT_EQ( std::round( transform->m3DPointsPZ11[i].getHeight( ) ),
      std::round( testPoints3D[i].getHeight( ) ) );
  }
}
//==============================================================================
/*!
* \brief Тестирование преобразование ПЗ-90.11 -> ПЗ-90.02
*/
TEST_F( TransformationTest, PZ11toPZ02 )
{
  using namespace BasicMath;

  transform->transformator.setSystems( CS_PZ_90_11, CS_PZ_90_02 );

  //! Берем точки в ПЗ-90.11
  GeoMultiPoint2D testPoints2D( transform->m2DPointsPZ11 );
  GeoPoints3D testPoints3D( transform->m3DPointsPZ11 );

  //! Преобразуем в ПЗ-90.02
  AbstractOperations::changeEachPoint(
    testPoints2D, transform->transformator );

  for( auto& point : testPoints3D )
    transform->transformator( point );

  //! Сравниваем результат с контрольными точками в ПЗ-90.02
  for( int i = 0; i < testPoints2D.size( ); i++ )
  {
    ASSERT_TRUE( isEqual( transform->m2DPointsPZ02[i].getLatitude( ),
      testPoints2D[i].getLatitude( ) ) );
    ASSERT_TRUE( isEqual( transform->m2DPointsPZ02[i].getLongitude( ),
      testPoints2D[i].getLongitude( ) ) );

    ASSERT_EQ( std::round( transform->m3DPointsPZ02[i].getHeight( ) ),
      std::round( testPoints3D[i].getHeight( ) ) );
  }
}
//==============================================================================
/*!
* \brief Тестирование смены СК
*/
TEST_F( TransformationTest, ChangeCoordinateSystems )
{
  transform->transformator.setSystems( CS_PZ_90_11, CS_PZ_90_02 );
  ASSERT_EQ( CS_PZ_90_11, transform->transformator.getInitSystems( ) );
  ASSERT_EQ( CS_PZ_90_02, transform->transformator.getGoalSystems( ) );

  transform->transformator.swapSystems( );
  ASSERT_EQ( CS_PZ_90_02, transform->transformator.getInitSystems( ) );
  ASSERT_EQ( CS_PZ_90_11, transform->transformator.getGoalSystems( ) );
}
//==============================================================================
/*!
* \brief Тестирование преобразования СК без изменения исходной точки
*/
TEST_F( TransformationTest, TransformCS )
{
  using namespace BasicMath;

  transform->transformator.setSystems( CS_SK_42, CS_WGS_84 );

  GeoPoint2D testPoint2D;
  GeoPoint3D testPoint3D;

  for( int i = 0; i < transform->m2DPointsSK.size( ); i++ )
  {
    testPoint2D
      = transform->transformator.transform( transform->m2DPointsSK[i] );
    ASSERT_TRUE( isEqual( transform->m2DPointsWGS[i].getLatitude( ),
      testPoint2D.getLatitude( ) ) );
    ASSERT_TRUE( isEqual( transform->m2DPointsWGS[i].getLongitude( ),
      testPoint2D.getLongitude( ) ) );

    testPoint3D
      = transform->transformator.transform( transform->m3DPointsSK[i] );
    ASSERT_EQ( std::round( transform->m3DPointsWGS[i].getHeight( ) ),
      std::round( testPoint3D.getHeight( ) ) );
  }
}
//==============================================================================
} //! Tests
} //! Geodesic
//==============================================================================
