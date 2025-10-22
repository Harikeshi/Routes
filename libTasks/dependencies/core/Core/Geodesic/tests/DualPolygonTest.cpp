#include <Geodesic/Objects/DualPolygon.hpp>

#include <Geodesic/GeoLineTools/OrthodromyTools.hpp>
#include <Geodesic/GeodesicException.hpp>
//! MathTools
#include <MathTools/BasicMath/Compare.hpp>
//! AbstractOperations
#include <AbstractOperations/Normalize.hpp>
#include <AbstractOperations/Algorithms/CoveredBy.hpp>
//! gtest
#include <gtest/gtest.h>
//==============================================================================
namespace Geodesic
{
namespace Tests
{
//==============================================================================
using BasicMath::isEqual;
//==============================================================================
/* Построить квадрат с центром в данной точке и длиной стороны в градусах (!)
*\param[in] center - центр квадрата (географическая точка)
*\param[in] size - длина стороны квадрата [градусы]
*\return - построенный квадрат
*/
GeoPolygon2D buildSquare( const GeoPoint2D center, const Degree size )
{
  GeoPolygon2D retVal;
  Radian lat = center.getLatitude( );
  Radian lon = center.getLongitude( );
  Radian shift = size.toRadians( ) / Radian(2);
  retVal.outer( ).push_back( GeoPoint2D( lat + shift, lon + shift ) );
  retVal.outer( ).push_back( GeoPoint2D( lat - shift, lon + shift ) );
  retVal.outer( ).push_back( GeoPoint2D( lat - shift, lon - shift ) );
  retVal.outer( ).push_back( GeoPoint2D( lat + shift, lon - shift ) );
  retVal.outer( ).push_back( GeoPoint2D( lat + shift, lon + shift ) );
  return retVal;
}
//==============================================================================
double error( const double value, const double etalon )
{
  return fabs( ( fabs(value) - fabs(etalon) ) / etalon );
}

// допустимое отклонение при сравнении величин: 0.013%
const double eps = 0.00013; 
//==============================================================================
//==============================================================================
class DualPolygonTest : public ::testing::Test
{
protected:
  void SetUp( )
  {
    center.set( Degree( 0 ), Degree( 100 ) );
    center180.set( Degree( 0 ), Degree( 179.99 ) );
    polyg = buildSquare( center, Degree( 2 ) );
    radius = orthodromyTools.distance( center, polyg.outer()[0] );
    multPolyg.push_back( polyg );
    circle = GeoCircleLite2D( center, radius );
  }
//==============================================================================
  void TearDown( )
  {
  }
//==============================================================================
  OrthodromyTools orthodromyTools;

  GeoPoint2D center; // обычный центр
  GeoPoint2D center180; // центр возле 180-го меридиана
  double radius; // радиус окружности
  GeoCircleLite2D circle; // окружность
  GeoPolygon2D polyg; // вписанный квадрат
  GeoMultiPolygon2D multPolyg; // мультиполигон со вписанным квадратом
};
//==============================================================================
//==============================================================================
TEST_F( DualPolygonTest, FromCircle )
{
  // можно построить полигон по окружности
  DualPolygon dual( circle );
  // он будет считать себя окружностью с заданными параметрами
  ASSERT_TRUE( dual.isOriginallyCircle( ) );
  ASSERT_EQ( center, dual.getCenter() );
  ASSERT_TRUE( isEqual( radius, dual.getRadius( ) ) );
  ASSERT_EQ( circle, dual.getCircle( ) );
  // из него можно добыть многоугольное представление заданной детализации
  GeoMultiPolygon2D polygon = dual.getPolygon( );
  ASSERT_EQ( 1, polygon.size( ) );
  ASSERT_EQ( dual.getNumOfVertexes( ), polygon[0].outer( ).size( ) - 1);
  ASSERT_TRUE( error( radius
    , orthodromyTools.distance(center, polygon[0].outer()[0]) ) < eps );

  // можно задать дургой радиус
  dual.setRadius( radius * 2 );
  // полигон станет считать себя окружностью с новыми параметрами
  ASSERT_TRUE( dual.isOriginallyCircle( ) );
  ASSERT_TRUE( isEqual( 2 * radius, dual.getRadius( ) ) );
  // и новым многоугольным представлением
  polygon = dual.getPolygon( );
  ASSERT_TRUE( error( radius * 2
    , orthodromyTools.distance( center, polygon[0].outer( )[0] ) ) < eps );

  // можно поменять детальность многоугольного представления
  dual.setNumOfVertexes( 4 );
  // на окружность и самоосознание это никак не повлияет
  ASSERT_TRUE( dual.isOriginallyCircle( ) );
  ASSERT_EQ( center, dual.getCenter( ) );
  ASSERT_TRUE( isEqual( 2 * radius, dual.getRadius( ) ) );
  // а многоугольное представление будет перестроено
  polygon = dual.getPolygon( );
  ASSERT_EQ( dual.getNumOfVertexes( ), 4 );
  ASSERT_EQ( dual.getNumOfVertexes( ), polygon[0].outer( ).size( ) - 1 );

  // на 180-м меридиане все тоже неплохо
  dual.setCenter( center180 );
  ASSERT_TRUE( dual.isOriginallyCircle( ) );
  polygon = dual.getPolygon( );
  ASSERT_TRUE( error( radius * 2
    , orthodromyTools.distance( center180, polygon[0].outer( )[0] ) ) < eps );
  ASSERT_TRUE( error( radius * 4
    , orthodromyTools.distance( polygon[0].outer( )[0],
                                polygon[0].outer( )[2] ) ) < eps );
  ASSERT_TRUE( error( radius * 4
    , orthodromyTools.distance( polygon[0].outer( )[1],
                                polygon[0].outer( )[3] ) ) < eps );

  // можно получать окружность из её характеристик, а не как объект
  DualPolygon dual2( center, radius );
  polygon = dual2.getPolygon( );
  ASSERT_EQ( 1, polygon.size( ) );
  ASSERT_EQ( dual2.getNumOfVertexes( ), polygon[0].outer( ).size( ) - 1 );
  ASSERT_TRUE( error( radius
    , orthodromyTools.distance( center, polygon[0].outer( )[0] ) ) < eps );

  // можно задавать новую окружность целиком
  dual2.setCircle( GeoCircleLite2D( center180, 2 * radius ) );
  polygon = dual2.getPolygon( );
  ASSERT_EQ( 1, polygon.size( ) );
  ASSERT_EQ( dual2.getNumOfVertexes( ), polygon[0].outer( ).size( ) - 1 );
  ASSERT_TRUE( error( 2 * radius
    , orthodromyTools.distance( center180, polygon[0].outer( )[0] ) ) < eps );

  // никаким образом нельзя запихнуть в полигон круг с отрицательным радиусом
  ASSERT_THROW( DualPolygon err (center, -1), GeodesicFailure );
  try
  {
    DualPolygon err( center, -1 );
  }
  catch( const GeodesicFailure& exception )
  {
    ASSERT_EQ( NegativeRadius, exception.getCode( ) );
    ASSERT_EQ( geodesicLibTitle, exception.getLib( ) );
    ASSERT_EQ(
      "[Core::Geodesic] <NegativeRadius>: Отрицательный радиус окружности"
      , std::string( exception.what( ) ) );
  }

  ASSERT_THROW( DualPolygon err( GeoCircleLite2D ( center, -1 ) ),
                GeodesicFailure );
  try
  {
    DualPolygon err( GeoCircleLite2D( center, -1 ) );
  }
  catch( const GeodesicFailure& exception )
  {
    ASSERT_EQ( NegativeRadius, exception.getCode( ) );
    ASSERT_EQ( geodesicLibTitle, exception.getLib( ) );
    ASSERT_EQ(
      "[Core::Geodesic] <NegativeRadius>: Отрицательный радиус окружности"
      , std::string( exception.what( ) ) );
  }

  ASSERT_THROW( dual.setRadius ( -1 ), GeodesicFailure );
  try
  {
    dual.setRadius( -1 );
  }
  catch( const GeodesicFailure& exception )
  {
    ASSERT_EQ( NegativeRadius, exception.getCode( ) );
    ASSERT_EQ( geodesicLibTitle, exception.getLib( ) );
    ASSERT_EQ(
      "[Core::Geodesic] <NegativeRadius>: Отрицательный радиус окружности"
      , std::string( exception.what( ) ) );
  }

}
//==============================================================================
TEST_F( DualPolygonTest, FromPolygon )
{
  // можно использовать при создании полигона многоугольный контур
  DualPolygon dual( multPolyg );
  // он будет считать себя многоугольником с заданной границей
  ASSERT_FALSE( dual.isOriginallyCircle( ) );
  ASSERT_EQ( multPolyg[0].outer(), dual.getPolygon( )[0].outer() );
  // и дополнительно будет хранить описанную окружность
  ASSERT_EQ( center, dual.getCenter( ) );
  ASSERT_TRUE( error ( radius, dual.getRadius( ) ) < eps );

  // при задании какого-то из параметров окружности первичная сущность меняется
  dual.setRadius( 2 * radius );
  ASSERT_TRUE( dual.isOriginallyCircle( ) );
  GeoMultiPolygon2D polygon = dual.getPolygon( );
  ASSERT_TRUE( error( radius * 2
    , orthodromyTools.distance( center, polygon[0].outer( )[0] ) ) < eps );

  // можно задать новый граничный многоугольник. Первичная сущность обновится
  dual.setPolygon( multPolyg );
  ASSERT_FALSE( dual.isOriginallyCircle( ) );
  ASSERT_EQ( center, dual.getCenter( ) );
  ASSERT_TRUE( error ( radius, dual.getRadius( ) ) < eps );
  ASSERT_EQ( multPolyg[0].outer(), dual.getPolygon( )[0].outer() );

  // для мультиполигона учитываются все полигоны, а не только первый
  multPolyg.push_back( buildSquare( 
    GeoPoint2D(Degree(0), Degree(104)), Degree( 2 ) ) );
  dual.setPolygon( multPolyg );
  ASSERT_EQ( GeoPoint2D( Degree( 0 ), Degree( 102 ) ), dual.getCenter( ) );

  // никаким образом нельзя запихнуть в полигон совсем пустую границу
  GeoMultiPolygon2D empty;
  ASSERT_THROW( DualPolygon err( empty ), GeodesicFailure );
  try
  {
    DualPolygon err( empty );
  }
  catch( const GeodesicFailure& exception )
  {
    ASSERT_EQ( EmptyPolygon, exception.getCode( ) );
    ASSERT_EQ( geodesicLibTitle, exception.getLib( ) );
    ASSERT_EQ(
      "[Core::Geodesic] <EmptyPolygon>: Пустой полигон"
      , std::string( exception.what( ) ) );
  }

  ASSERT_THROW( dual.setPolygon ( empty ), GeodesicFailure );
  try
  {
    dual.setPolygon( empty );
  }
  catch( const GeodesicFailure& exception )
  {
    ASSERT_EQ( EmptyPolygon, exception.getCode( ) );
    ASSERT_EQ( geodesicLibTitle, exception.getLib( ) );
    ASSERT_EQ(
      "[Core::Geodesic] <EmptyPolygon>: Пустой полигон"
      , std::string( exception.what( ) ) );
  }
}
//==============================================================================
TEST_F( DualPolygonTest, BoxTest )
{
  // вокруг того, что в первую очередь многоугольник, сразу строится корректно
  DualPolygon dual( { polyg } );
  GeoPolygon2D box = dual.box( );
  ASSERT_TRUE(
    AbstractOperations::coveredBy( dual.getPolygon( ), box, false, false ) );

  // небольшая окружность на экваторе тоже строится корректно
  dual = DualPolygon( circle );
  box = dual.box( );
  ASSERT_TRUE(
    AbstractOperations::coveredBy( dual.getPolygon( ), box, false, false) );

  // для других окружностей нужно немножко буфера
  circle = GeoCircleLite2D( GeoPoint2D( Degree( 70 ), Degree( 0 ) ), 100e3 );
  dual = DualPolygon( circle );
  box = dual.box( 89 );
  ASSERT_FALSE(
    AbstractOperations::coveredBy( dual.getPolygon( ), box, false, false) );
  box = dual.box( 90 );
  ASSERT_TRUE(
    AbstractOperations::coveredBy( dual.getPolygon( ), box, false, false) );

  // возле 180-го получим вывернутый полигон
  auto circle180 = GeoCircleLite2D( center180, radius );
  DualPolygon dual180 = DualPolygon( circle180 );
  GeoPolygon2D box180 = dual180.box( );
  using namespace AbstractOperations;
  auto normBox = box180;
  auto normPoly = dual180.getPolygon( );

  normalize( normBox );
  normalize( normPoly );

  ASSERT_TRUE(
    AbstractOperations::coveredBy( normPoly, normBox, false, false) );
  // у вывернутого полигона некорректная ориентация, для выполнения проверки
  // её нужно исправить
  ASSERT_TRUE(
    AbstractOperations::coveredBy( GeoPoint2D(), box180, true, false) );

  dual180 = DualPolygon( { box180 } );
  box180 = dual180.box( );
  normBox = box180;
  normPoly = dual180.getPolygon( );
  normalize( normBox );
  normalize( normPoly );
  // нормализованная коробка содержит нормализованный полигон
  ASSERT_TRUE(
    AbstractOperations::coveredBy( normPoly, normBox, false, false) );
  // первичная коробка получается в классическом диапазоне,
  // у вывернутого полигона некорректная ориентация, для выполнения проверки
  // её нужно исправить
  ASSERT_TRUE(
    AbstractOperations::coveredBy( GeoPoint2D( ), box180, true, false) );

  // при надувании коробка корректно перешагивает 180-й
  polyg = buildSquare( GeoPoint2D( Degree(0), Degree(179) ) , Degree( 1 ) );
  dual = DualPolygon ( { polyg } );
  box = dual.box( 100e3 );
  normBox = box;
  normPoly = dual.getPolygon( );
  normalize( normBox );
  normalize( normPoly );
  // нормализованная коробка уходит за 180-й
  ASSERT_TRUE(AbstractOperations::coveredBy(
    GeoPoint2D(Degree(0), Degree(180.1)), normBox, false, false) );
  // первичная коробка получается в классическом диапазоне,
  // у вывернутого полигона некорректная ориентация, для выполнения проверки
  // её нужно исправить
  ASSERT_TRUE(
    AbstractOperations::coveredBy( GeoPoint2D( ), box, true, false) );

  polyg = buildSquare( GeoPoint2D( Degree( 0 ), Degree( -179 ) ),
                       Degree( 1 ) );
  dual = DualPolygon( { polyg } );
  box = dual.box( 100e3 );
  normBox = box;
  normPoly = dual.getPolygon( );
  normalize( normBox );
  normalize( normPoly );
  // нормализованная коробка уходит за 180-й
  ASSERT_TRUE(AbstractOperations::coveredBy(
    GeoPoint2D( Degree( 0 ), Degree( 180.1 ) ), normBox, false, false) );
  // первичная коробка получается в классическом диапазоне,
  // у вывернутого полигона некорректная ориентация, для выполнения проверки
  // её нужно исправить
  ASSERT_TRUE(
    AbstractOperations::coveredBy( GeoPoint2D( ), box, true, false) );

  // если заданы кривые параметры, вылетает исключение
  ASSERT_THROW( dual.box( -1 ), GeodesicFailure );
  try
  {
    dual.box( -1 );
  }
  catch( GeodesicFailure& ex )
  {
    ASSERT_EQ( ex.getCode( ), TransitFailure );
    ASSERT_EQ( ex.getLib( ), geodesicLibTitle );
    ASSERT_NE( std::string( ex.what( ) ).find( "DualPolygon::box" ), 
      std::string::npos );
  }
}
//==============================================================================
} //! Tests
} //! Geodesic
//==============================================================================