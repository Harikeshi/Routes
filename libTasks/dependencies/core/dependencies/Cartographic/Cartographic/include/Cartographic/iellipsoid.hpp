#ifndef CARTOGRAPHIC_I_ELLIPSOID_HPP
#define CARTOGRAPHIC_I_ELLIPSOID_HPP
//==============================================================================
#include <Cartographic/units.hpp>
//==============================================================================
namespace Cartographic
{
//==============================================================================
class IEllipsoid
{
public:
  virtual ~IEllipsoid() {}

  virtual Meters A() const = 0;
  virtual Units E2() const = 0;
};
//==============================================================================
} // !Cartographic
//==============================================================================
#endif //! CARTOGRAPHIC_I_ELLIPSOID_HPP