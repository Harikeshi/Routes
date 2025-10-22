#ifndef CARTOGRAPHIC_TRANSFORMATION_HPP
#define CARTOGRAPHIC_TRANSFORMATION_HPP
//==============================================================================
#include <Cartographic/internal.hpp>
#include <Cartographic/units.hpp>
//==============================================================================
namespace Cartographic
{
//==============================================================================
class IDatumShift;
class IEllipsoid;
class IForwardGeocentric;
class IForwardProjection;
class IInverseGeocentric;
class IInverseProjection;
//==============================================================================
class Transformation
{
public:
  Transformation();

  template <typename Geocentric>
  void SetGeocentric();
  template <typename Geocentric>
  void SetSourceGeocentric();
  template <typename Geocentric>
  void SetTargetGeocentric();

  template <typename Projection>
  void SetSourceProjection(typename Projection::BaseParameters const &params);
  template <typename Projection>
  void SetTargetProjection(typename Projection::BaseParameters const &params);

  void ClearDatumShifts();
  bool AddDatumShift(IDatumShift *datum_shift);

  void Transform(Units &x, Units &y, Units &h, Units *dx = 0,
                 Units *dy = 0, Units *dh = 0) const;

  template
  <
      typename ItX,
      typename ItY,
      typename ItH
  >
  void Transform(ItX x_begin, ItX x_end, ItY y_begin, ItH h_begin,
                 Units *dx = 0, Units *dy = 0, Units *dh = 0) const;
  template
  <
      typename ItX,
      typename ItY
  >
  void Transform(ItX x_begin, ItX x_end, ItY y_begin,
                 Units *dx = 0, Units *dy = 0) const;

private:
  void SetSourceGeocentric(IForwardGeocentric const *geocentric);
  void SetTargetGeocentric(IInverseGeocentric const *geocentric);

  void SetSourceProjection(IInverseProjection const *projection,
                           IEllipsoid const *ellipsoid);
  void SetTargetProjection(IForwardProjection const *projection,
                           IEllipsoid const *ellipsoid);

  CARTOGRAPHIC_INTERNAL(Transformation)
};
//==============================================================================
template <typename Geocentric>
void Transformation::SetGeocentric()
{
  SetSourceGeocentric<Geocentric>();
  SetTargetGeocentric<Geocentric>();
}
//==============================================================================
template <typename Geocentric>
void Transformation::SetSourceGeocentric()
{
  SetSourceGeocentric(new Geocentric);
}
//==============================================================================
template <typename Geocentric>
void Transformation::SetTargetGeocentric()
{
  SetTargetGeocentric(new Geocentric);
}
//==============================================================================
template <typename Projection>
IEllipsoid const *GetProjectionEllipsoid(
  typename Projection::BaseParameters const &params
);
//==============================================================================
template <typename Projection>
void Transformation::SetSourceProjection(
  typename Projection::BaseParameters const &params
)
{
  SetSourceProjection(new Projection(params),
                      GetProjectionEllipsoid<Projection>(params));
}
//==============================================================================
template <typename Projection>
void Transformation::SetTargetProjection(
  typename Projection::BaseParameters const &params
)
{
  SetTargetProjection(new Projection(params),
                      GetProjectionEllipsoid<Projection>(params));
}
//==============================================================================
template
<
    typename ItX,
    typename ItY,
    typename ItH
>
void Transformation::Transform(ItX x_begin, ItX x_end, ItY y_begin,
                               ItH h_begin, Units *dx, Units *dy,
                               Units *dh) const
{
  for(; x_begin != x_end; ++x_begin, ++y_begin, ++h_begin)
    Transform(*x_begin, *y_begin, *h_begin, dx, dy, dh);
}
//==============================================================================
template
<
    typename ItX,
    typename ItY
>
void Transformation::Transform(ItX x_begin, ItX x_end, ItY y_begin,
                               Units *dx, Units *dy) const
{
  for (; x_begin != x_end; ++x_begin, ++y_begin)
  {
    Meters h(0.);
    Transform(*x_begin, *y_begin, h, dx, dy);
  }
}
//==============================================================================
} // !Cartographic
//==============================================================================
#endif //! CARTOGRAPHIC_TRANSFORMATION_HPP
