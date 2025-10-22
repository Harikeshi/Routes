#include <Geodesic/Objects/DualPolygon.hpp>

#include <Geodesic/GeoLineTools/OrthodromyTools.hpp>
#include <Geodesic/GeoLineTools/LoxodromeTools.hpp>
#include <Geodesic/Projection/Transition.hpp>
#include <Geodesic/Projection/Orthographic/ProjectionBuilder.hpp>
#include <Geodesic/GeodesicException.hpp>
//! PrimaryEntities
#include <PrimaryEntities/GeodesicEntities.hpp>
//! Geometry
#include <Geometry/Operations/PolygonToCircles.hpp>
//==============================================================================
namespace Geodesic
{
//==============================================================================
using AbstractOperations::getBorderPoints;
//==============================================================================
void DualPolygon::buildCircle( )
{
  // вариант с пустым полигоном не рассматриваем, т.к. он отсекается на входе

  // достаем точки, формирующие границу
  auto points = getBorderPoints( mPolygon );

  // проецируем в оптимальную проекцию
  auto proj = findOptimalProjection( points );
  auto flatBorder = geoGeometryToProjection ( points, 
    std::make_shared<OrthographicProjection>( proj ) );

  // вокруг спроецированного множества точек можно описать круг
  auto circle = Geometry::minimalCoveringCircle( flatBorder ).getCircle();

  mCircle.center = proj.fromProjection( circle.center );

  // находим произвольную точку на окружности
  Point2D pointOnCircle( 
    circle.center.getX( ) + circle.radius, circle.center.getY( ) );
  GeoPoint2D geoPointOnCircle = proj.fromProjection( pointOnCircle );

  // истинный радиус - длина ортодромии, соединяющей центр окружности с точкой на ней
  OrthodromyTools orthoTools;
  mCircle.radius = orthoTools.distance( mCircle.center, geoPointOnCircle );
}
//==============================================================================
GeoPolygon2D DualPolygon::box( const double buffer ) const
{
  GeoPolygon2D retVal;
  try
  {
    if( mIsOriginallyCircle )
    {
      OrthodromyTools orth;
      retVal = orth.circleRectangle(
          mCircle.center, mCircle.radius, buffer );
    }
    else
    {
      auto retBox = AbstractOperations::envelopeByOrder( getBorderPoints( mPolygon ) );
      if( buffer >= ACCURACY_DOUBLE )
      {
        // TODO поправить те же косяки, что и в circleRectangle
        LoxodromeTools lox;
        OrthodromyTools orth;
        auto point =
          lox.pointPosition( retBox.min_corner( ), Radian( M_PI ), buffer );
        point =
          orth.pointPosition( point, Radian( 3 * M_PI / 2 ), buffer );
        // TODO lonBuf в идеале должен быть разным для верха и низа вигуры
        Radian lonBuf =
          retBox.min_corner( ).getLongitude( ) - point.getLongitude( );
        retBox.min_corner( ) = point;
        point = lox.pointPosition( retBox.max_corner( ), Radian( 0 ), buffer );
        point.set( point.getLatitude( ), point.getLongitude( ) + lonBuf );
        retBox.max_corner( ) = point;
        AbstractOperations::normalize( retBox, REGULAR );
      }
      else if ( buffer <= - ACCURACY_DOUBLE )
        throw GeodesicFailure( NegativeDistance );

      retVal = retBox.toPolygon();
    }
  }
  catch( ... )
  {
    ExceptionTools::FailureHandler<GeodesicFailure>(
      "DualPolygon::box" );
  }

  return retVal;
}
//==============================================================================
void DualPolygon::buildPolygon( )
{
  //! Создаем инструменты для эллипсоида WGS-84 (т.к. храним и считаем всё там)
  OrthodromyTools orthodromyTools;

  mPolygon.clear( );

  if( mWithVertexesNum ) // с заданным числом точек
  {
    //TODO: в идеале, окружность должна быть описанной около, а сейчас все наоборот
    mPolygon.push_back( orthodromyTools.circle( 
      mCircle.center, mCircle.radius, int( mNumOfVertexes ) ) );
  }
  else // с заданной длиной хорды
  {
    if ( mChordMaxLength < 2 * mCircle.radius )
    {
      mPolygon.push_back( orthodromyTools.circle(
        mCircle.center, mCircle.radius, mChordMaxLength ) );
    }
    else
    {
      // если у хорд что-то не сложилось - строим по вершинам
      mPolygon.push_back( orthodromyTools.circle(
        mCircle.center, mCircle.radius, 8 ) );
    }
  }
}
//==============================================================================
//==============================================================================
DualPolygon::DualPolygon( const GeoPoint2D & center, const double radius
  , const size_t numOfVertexes )
  :mCircle(center, radius)
{
  if( radius < 0 )
    throw GeodesicFailure( NegativeRadius );

  mCircle = GeoCircleLite2D( center, radius );
  mIsOriginallyCircle = true;
  mNumOfVertexes = numOfVertexes;
  mWithVertexesNum = true;

  buildPolygon( );
}
//==============================================================================
DualPolygon::DualPolygon( const GeoCircleLite2D & circle, const size_t numOfVertexes)
{
  if( circle.radius < 0 )
    throw GeodesicFailure( NegativeRadius );

  mCircle = circle;
  mIsOriginallyCircle = true;
  mNumOfVertexes = numOfVertexes;
  mWithVertexesNum = true;

  buildPolygon( );
}
//==============================================================================
DualPolygon::DualPolygon( const GeoMultiPolygon2D & polygon
  , const size_t numOfVertexes )
{
  setPolygon( polygon );
  mNumOfVertexes = numOfVertexes;
  mWithVertexesNum = true;

  buildCircle( );
}
//==============================================================================
DualPolygon::DualPolygon( const GeoPoint2D & center, const double radius
  , const double  chord )
  :mCircle( center, radius )
{
  if( radius < 0 )
    throw GeodesicFailure( NegativeRadius );

  mCircle = GeoCircleLite2D( center, radius );
  mIsOriginallyCircle = true;
  mChordMaxLength = chord;
  mWithVertexesNum = false;

  buildPolygon( );
}
//==============================================================================
DualPolygon::DualPolygon( const GeoCircleLite2D & circle, const double chord)
{
  if( circle.radius < 0 )
    throw GeodesicFailure( NegativeRadius );

  mCircle = circle;
  mIsOriginallyCircle = true;
  mChordMaxLength = chord;
  mWithVertexesNum = false;

  buildPolygon( );
}
//==============================================================================
DualPolygon::DualPolygon( const GeoMultiPolygon2D & polygon
  , const double chord )
{
  setPolygon( polygon );
  mChordMaxLength = chord;
  mWithVertexesNum = false;

  buildCircle( );
}
//==============================================================================
//==============================================================================
void DualPolygon::setCenter( const GeoPoint2D & center )
{
  mIsOriginallyCircle = true;
  mCircle.center = center;

  buildPolygon( );
}
//==============================================================================
void DualPolygon::setRadius( const double radius )
{
  if( radius < 0 )
    throw GeodesicFailure( NegativeRadius );

  mIsOriginallyCircle = true;
  mCircle.radius = radius;

  buildPolygon( );
}
//==============================================================================
void DualPolygon::setCircle( const GeoCircleLite2D & circle )
{
  if( circle.radius < 0 )
    throw GeodesicFailure( NegativeRadius );

  mIsOriginallyCircle = true;
  mCircle = circle;

  buildPolygon( );
}
//==============================================================================
void DualPolygon::setPolygon( const GeoMultiPolygon2D & polygon )
{
  // проверяем, пришли ли хоть какие-нибудь точки
  bool isEmpty = true;
  for( const auto& polyg : polygon )
    if( polyg.outer( ).size( ) )
    {
      isEmpty = false;
      break;
    }
  if( isEmpty )
    throw GeodesicFailure( EmptyPolygon );

  mIsOriginallyCircle = false;
  mPolygon = polygon;

  buildCircle( );
}
//==============================================================================
void DualPolygon::setNumOfVertexes( const size_t num )
{
  mNumOfVertexes = num;
  mWithVertexesNum = true;

  if ( mIsOriginallyCircle )
    buildPolygon( );
}
//==============================================================================
void DualPolygon::setLenOfChord( const double chord )
{
  mChordMaxLength = chord;
  mWithVertexesNum = false;

  if( mIsOriginallyCircle )
    buildPolygon( );
}
//==============================================================================
} // ! Geodesic
