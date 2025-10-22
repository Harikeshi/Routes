#ifndef CARTOGRAPHIC_I_PROJECTION_HPP
#define CARTOGRAPHIC_I_PROJECTION_HPP
//==============================================================================
#include <Cartographic/units.hpp>
//==============================================================================
namespace Cartographic
{
//==============================================================================
class IForwardProjection
{
public:
  virtual ~IForwardProjection() {}

  virtual void Forward(Degrees b, Degrees l, Units &x, Units &y,
                       Units *d1 = 0, Units *d2 = 0) const = 0;
};
//==============================================================================
class IInverseProjection
{
public:
  virtual ~IInverseProjection() {}

  virtual void Inverse(Units x, Units y, Degrees &b, Degrees &l,
                       Units *d1 = 0, Units *d2 = 0) const = 0;
};
//==============================================================================
class IProjection: public IForwardProjection,
    public IInverseProjection {};
//==============================================================================
class IEllipsoid;
//==============================================================================
template <typename Projection>
IEllipsoid const *GetProjectionEllipsoid(
    typename Projection::BaseParameters const &base_parameters);
//==============================================================================
} // !Cartographic
//==============================================================================
#endif //! CARTOGRAPHIC_I_PROJECTION_HPP
