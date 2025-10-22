#include <Cartographic/idatumshift.hpp>
#include <Cartographic/iellipsoid.hpp>
#include <Cartographic/igeocentric.hpp>
#include <Cartographic/iprojection.hpp>
#include <Cartographic/transformation.hpp>
// std
#include <memory>
#include <vector>
//==============================================================================
namespace Cartographic
{
//==============================================================================
class Transformation::Internal
{
public:
  Internal();
  ~Internal();

  void clearDatumShifts();

  std::unique_ptr<IForwardGeocentric const> source_geocentric;
  std::unique_ptr<IInverseGeocentric const> target_geocentric;

  std::unique_ptr<IInverseProjection const> source_projection;
  std::unique_ptr<IForwardProjection const> target_projection;

  std::unique_ptr<IEllipsoid const> source_ellipsoid, target_ellipsoid;

  typedef std::vector<IDatumShift *> DatumShifts;
  DatumShifts datum_shifts;

private:
  Internal(Internal const &);
  Internal &operator=(Internal const &);
};
//==============================================================================
Transformation::Internal::Internal()
{}
//==============================================================================
Transformation::Internal::~Internal()
{
  clearDatumShifts();
}
//==============================================================================
void Transformation::Internal::clearDatumShifts()
{
  while (!datum_shifts.empty())
  {
    delete datum_shifts.back();
    datum_shifts.pop_back();
  }
}
//==============================================================================
CARTOGRAPHIC_INTERNAL_DESTRUCTOR(Transformation)
//==============================================================================
Transformation::Transformation()
: internal_(new Internal)
{}
//==============================================================================
void Transformation::SetSourceGeocentric(IForwardGeocentric const *g)
{
  internal_->source_geocentric.reset(g);
}
//==============================================================================
void Transformation::SetTargetGeocentric(IInverseGeocentric const *g)
{
  internal_->target_geocentric.reset(g);
}
//==============================================================================
void Transformation::SetSourceProjection(IInverseProjection const *projection,
                                         IEllipsoid const *ellipsoid)
{
  internal_->source_projection.reset(projection);
  internal_->source_ellipsoid.reset(ellipsoid);
}
//==============================================================================
void Transformation::SetTargetProjection(IForwardProjection const *projection,
                                         IEllipsoid const *ellipsoid)
{
  internal_->target_projection.reset(projection);
  internal_->target_ellipsoid.reset(ellipsoid);
}
//==============================================================================
void Transformation::ClearDatumShifts()
{
  internal_->clearDatumShifts();
}
//==============================================================================
bool Transformation::AddDatumShift(IDatumShift *datum_shift)
{
  std::unique_ptr<IDatumShift> shift(datum_shift);
  if(!internal_->datum_shifts.empty() &&
     internal_->datum_shifts.back()->TargetDatum() != shift->SourceDatum())
  {
    return false;
  }
  if(!shift->IsIdentity())
    internal_->datum_shifts.push_back(shift.release());
  return true;
}
//==============================================================================
void Transformation::Transform(Units &x_, Units &y_, Units &h_,
                               Units *dx, Units *dy, Units *dh) const
{
  Degrees b, l;
  Meters x, y, z;

  internal_->source_projection->Inverse(x_, y_, b, l, dx, dy);
  internal_->source_geocentric->Forward(*internal_->source_ellipsoid, b,
                                        l, h_, x, y, z, dx, dy, dh);

  for(Internal::DatumShifts::const_iterator i(internal_->datum_shifts.begin());
      i != internal_->datum_shifts.end(); ++i)
  {
    (*i)->Shift(x, y, z, x, y, z, dx, dy, dh);
  }

  internal_->target_geocentric->Inverse(*internal_->target_ellipsoid, x, y,
                                        z, b, l, h_, dx, dy, dh);
  internal_->target_projection->Forward(b, l, x_, y_, dx, dy);
}
//==============================================================================
} // !Cartographic
