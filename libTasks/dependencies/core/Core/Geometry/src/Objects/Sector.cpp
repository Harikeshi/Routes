#include <Geometry/Objects/Sector.hpp>
//! MathTools
#include <MathTools/BasicMath/Compare.hpp>
//==============================================================================
namespace Geometry
{
//==============================================================================
Radian getValInCorrectInterval( const Radian& val )
{
  double value = val.get( );

  //оставляем возможность задать полный сектор
  return Radian( BasicMath::isEqual( value, 2 * M_PI ) ? 2 * M_PI
    : BasicMath::normalizeWithShift( value ) );
}
//==============================================================================
Sector::Sector( const Radian & begAngle, const Radian & endAngle )
{
  mSector = std::make_pair( getValInCorrectInterval( begAngle ),
    getValInCorrectInterval( endAngle ) );
}
//==============================================================================
Sector::Sector( const Degree & begAngle, const Degree & endAngle )
{
  mSector = std::make_pair( getValInCorrectInterval( begAngle.toRadians( ) ),
    getValInCorrectInterval( endAngle.toRadians( ) ) );
}
//==============================================================================
} // ! Geometry
//==============================================================================
