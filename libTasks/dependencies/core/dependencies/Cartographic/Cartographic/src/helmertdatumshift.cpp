#include <Cartographic/helmertdatumshift.hpp>
#include <Cartographic/math.hpp>
//==============================================================================
namespace Cartographic
{
//==============================================================================
void HelmertFunction(Meters d_x, Meters d_y, Meters d_z,
                     Radians w_x, Radians w_y, Radians w_z, Units m,
                     Meters source_x, Meters source_y, Meters source_z,
                     Meters &target_x, Meters &target_y, Meters &target_z,
                     Meters *, Meters *, Meters *)
{
  target_x = (1. + m) * ( source_x + w_z * source_y - w_y * source_z) + d_x;
  target_y = (1. + m) * (-w_z * source_x + source_y + w_x * source_z) + d_y;
  target_z = (1. + m) * ( w_y * source_x - w_x * source_y + source_z) + d_z;
}
//==============================================================================
} // !Cartographic
