#include <SamplesForTests/BaseFlatFigures.hpp>
#include <SamplesForTests/SamplesForTestsExceptions.hpp>
//==============================================================================
namespace SamplesForTests
{
//==============================================================================
  Polygon2D buildRectangle( 
    const Point2D & leftBottomPoint, 
    const double horizontalSize, 
    const double verticalSize )
  {
    // для удобства именований вводим оси: х - направо, у - наверх
    double dx( horizontalSize );
    double dy( verticalSize );

    if( BasicMath::isNull( dx ) || BasicMath::isNull( dy ) )
      return Polygon2D( );
    if( ( dx < 0 ) || ( dy < 0 ) )
      throw SamplesForTestsFailure( NegativeDistance );

    double x0( leftBottomPoint.getX( ) );
    double y0( leftBottomPoint.getY( ) );

    Polygon2D retVal;
    retVal.outer( ).push_back( Point2D( x0, y0 ) );
    retVal.outer( ).push_back( Point2D( x0, y0 + dy ) );
    retVal.outer( ).push_back( Point2D( x0 + dx, y0 + dy ) );
    retVal.outer( ).push_back( Point2D( x0 + dx, y0 ) );
    retVal.outer( ).push_back( Point2D( x0, y0 ) );
    return retVal;
  }
//==============================================================================
  Polygon2D buildBow( 
    const Point2D & leftBottomPoint, 
    const double horizontalSize, 
    const double verticalSize )
  {
    // для удобства именований вводим оси: х - направо, у - наверх
    double dx( horizontalSize );
    double dy( verticalSize );

    if( BasicMath::isNull( dx ) || BasicMath::isNull( dy ) )
      return Polygon2D( );
    if( ( dx < 0 ) || ( dy < 0 ) )
      throw SamplesForTestsFailure( NegativeDistance );

    double x0( leftBottomPoint.getX( ) );
    double y0( leftBottomPoint.getY( ) );

    Polygon2D retVal;
    retVal.outer( ).push_back( Point2D( x0, y0 ) );
    retVal.outer( ).push_back( Point2D( x0, y0 + dy ) );
    retVal.outer( ).push_back( Point2D( x0 + dx, y0 ) );
    retVal.outer( ).push_back( Point2D( x0 + dx, y0 + dy ) );
    retVal.outer( ).push_back( Point2D( x0, y0 ) );
    return retVal;
  }
//==============================================================================
  Polygon2D buildPost( 
    const Point2D & leftBottomPoint, 
    const double horizontalSize, 
    const double verticalSize )
  {
    // для удобства именований вводим оси: х - направо, у - наверх
    double dx( horizontalSize );
    double dy( verticalSize );

    if (BasicMath::isNull( dx) || BasicMath::isNull( dy ))
      return Polygon2D();
    if ( ( dx < 0 ) || ( dy < 0 ) )
      throw SamplesForTestsFailure(NegativeDistance);

    double x0( leftBottomPoint.getX( ) );
    double y0( leftBottomPoint.getY( ) );

    Polygon2D retVal;
    retVal.outer().push_back( Point2D ( x0, y0 ) );
    retVal.outer().push_back( Point2D ( x0, y0 + dy ) );
    retVal.outer().push_back( Point2D ( x0 + dx / 2., y0 ) );
    retVal.outer().push_back( Point2D ( x0 + dx, y0 + dy ) );
    retVal.outer().push_back( Point2D ( x0 + dx, y0 ) );
    retVal.outer().push_back( Point2D ( x0, y0 ) );
    return retVal;
  }
//==============================================================================
} //! SamplesForTests
//==============================================================================