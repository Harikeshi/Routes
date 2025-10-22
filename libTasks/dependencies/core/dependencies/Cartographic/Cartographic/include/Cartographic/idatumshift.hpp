#ifndef CARTOGRAPHIC_I_DATUMSHIFT_HPP
#define CARTOGRAPHIC_I_DATUMSHIFT_HPP
//==============================================================================
#include <Cartographic/authority.hpp>
#include <Cartographic/datum.hpp>
#include <Cartographic/units.hpp>
//==============================================================================
namespace Cartographic
{
//==============================================================================
class IDatumShift
{
public:
  virtual ~IDatumShift() {}

  virtual bool IsIdentity() const = 0;

  virtual AuthorityReference Authority() const = 0;

  virtual DatumReference SourceDatum() const = 0;
  virtual DatumReference TargetDatum() const = 0;

  virtual void Shift(Meters sx, Meters sy, Meters sz,
                     Meters &tx, Meters &ty, Meters &tz,
                     Meters *dx = 0, Meters *dy = 0, Meters *dz = 0) const = 0;
};
//==============================================================================
} // !Cartographic
//==============================================================================
#endif //! CARTOGRAPHIC_I_DATUMSHIFT_HPP
