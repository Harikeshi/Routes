#include <Cartographic/Alexeev/arcofmeridian.hpp>
#include <Cartographic/Alexeev/simplepolyconic.hpp>
#include <Cartographic/iellipsoid.hpp>
#include <Cartographic/math.hpp>
//==============================================================================
namespace Cartographic
{
//==============================================================================
Alexeev::SimplePolyconic::SimplePolyconic(BaseParameters const &base_parameters)
: mP1(base_parameters.ellipsoid->A()),
  mP2(base_parameters.ellipsoid->E2()),
  mP3(ArcOfMeridian::Length(*base_parameters.ellipsoid,
                            base_parameters.b0 * Rho)),
  mP4(base_parameters.l0) {}
//==============================================================================
void Alexeev::SimplePolyconic::Forward(Degrees b, Degrees l,
  Meters &x, Meters &y,
  Units *, Units *) const
{
  b *= Rho;
  Units const sb(sin(b)), dlsb((l - mP4) * Rho * sb),
    ncb(mP1 * cos(b) / sb / sqrt(1. - mP2 * sb * sb));
  x = ArcOfMeridian::Length(mP1, mP2, b) + ncb * (1. - cos(dlsb)) - mP3;
  y = ncb * sin(dlsb);
}
//==============================================================================
template <>
IEllipsoid const *GetProjectionEllipsoid<Alexeev::SimplePolyconic>(
  Alexeev::SimplePolyconic::BaseParameters const &base_parameters)
{
  return base_parameters.ellipsoid.get();
}
//==============================================================================
} // !Cartographic
