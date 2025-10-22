#ifndef CARTOGRAPHIC_LATITUDE_LONGITUDE_HPP
#define CARTOGRAPHIC_LATITUDE_LONGITUDE_HPP
//==============================================================================
#include <Cartographic/iprojection.hpp>
//==============================================================================
namespace Cartographic
{
//==============================================================================
// Геодезические широта и долгота
class LatitudeLongitude: public IProjection
{
public:
  // Эллипсоид
  typedef IEllipsoid const *BaseParameters;

  explicit LatitudeLongitude(BaseParameters const &);

  void Forward(Degrees b, Degrees l, Degrees &x,
               Degrees &y, Units *d1 = 0, Units *d2 = 0) const;

  void Inverse(Degrees x, Degrees y, Degrees &b,
               Degrees &l, Units *d1 = 0, Units *d2 = 0) const;
};
//==============================================================================
} // !Cartographic
//==============================================================================
#endif //! CARTOGRAPHIC_LATITUDE_LONGITUDE_HPP
