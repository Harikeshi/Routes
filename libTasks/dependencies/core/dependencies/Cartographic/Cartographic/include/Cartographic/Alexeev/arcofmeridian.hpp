#ifndef CARTOGRAPHIC_ALEXEEV_ARCOFMERIDIAN_HPP
#define CARTOGRAPHIC_ALEXEEV_ARCOFMERIDIAN_HPP
//==============================================================================
#include <Cartographic/units.hpp>
//==============================================================================
namespace Cartographic
{
//==============================================================================
class IEllipsoid;
//==============================================================================
namespace Alexeev
{
namespace ArcOfMeridian
{
//==============================================================================
// Длина дуги меридиана от экватора до широты b, x = (*pa) * b + (*pc)
Meters Length(IEllipsoid const &ellipsoid, Radians b,
              Units *pa = 0, Units *pc = 0);
Meters Length(Meters a, Units e2, Radians b, Units *pa = 0, Units *pc = 0);
//==============================================================================
// Величина дуги меридиана длиной x
Radians Angle(IEllipsoid const &ellipsoid, Meters x);
//==============================================================================
} // !ArcOfMeridian
} // !Alexeev
//==============================================================================
} // !Cartographic
//==============================================================================
#endif //! CARTOGRAPHIC_ALEXEEV_ARCOFMERIDIAN_HPP

