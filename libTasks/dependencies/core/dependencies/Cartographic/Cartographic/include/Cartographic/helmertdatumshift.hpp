#ifndef CARTOGRAPHIC_HELMER_DATUMSHIFT_HPP
#define CARTOGRAPHIC_HELMER_DATUMSHIFT_HPP
//==============================================================================
#include <Cartographic/idatumshift.hpp>
//==============================================================================
namespace Cartographic
{
//==============================================================================
template <DatumObject, DatumObject, AuthorityObject>
class HelmertDatumShift;
//==============================================================================
template <DatumObject datum, AuthorityObject authority>
class HelmertDatumShift<datum, datum, authority>:
    public IDatumShift
{
public:
  HelmertDatumShift() {}

  bool IsIdentity() const
  {
    return true;
  }

  AuthorityReference Authority() const
  {
    return authority;
  }

  DatumReference SourceDatum() const
  {
    return datum;
  }
  DatumReference TargetDatum() const
  {
    return datum;
  }

  void Shift(Meters sx, Meters sy, Meters sz, Meters &tx,
             Meters &ty, Meters &tz,Meters * = 0,
             Meters * = 0, Meters * = 0) const
  {
    tx = sx;
    ty = sy;
    tz = sz;
  }
};
//==============================================================================
void HelmertFunction(Meters d_x, Meters d_y, Meters d_z,
                     Radians w_x, Radians w_y, Radians w_z, Units m,
                     Meters source_x, Meters source_y, Meters source_z,
                     Meters &target_x, Meters &target_y, Meters &target_z,
                     Meters *dx = 0, Meters *dy = 0, Meters *dz = 0);
//==============================================================================
} // !Cartographic
//==============================================================================
#define CARTOGRAPHIC_DECLARE_HELMERTDATUMSHIFT(SOURCE, TARGET, AUTHORITY)      \
/*===========================================================================*/\
namespace Cartographic {                                                       \
/*===========================================================================*/\
template <>                                                                    \
class HelmertDatumShift< SOURCE, TARGET, AUTHORITY >:                          \
public IDatumShift                                                             \
{                                                                              \
public:                                                                        \
  HelmertDatumShift() {}                                                       \
                                                                               \
  bool IsIdentity() const { return false; }                                    \
                                                                               \
  AuthorityReference Authority() const { return AUTHORITY; }                   \
                                                                               \
  DatumReference SourceDatum() const { return SOURCE; }                        \
  DatumReference TargetDatum() const { return TARGET; }                        \
                                                                               \
private:                                                                       \
  struct Values                                                                \
  {                                                                            \
    Meters dx, dy, dz;                                                         \
    Radians wx, wy, wz;                                                        \
    Units m;                                                                   \
  };                                                                           \
  static Values const values_;                                                 \
                                                                               \
public:                                                                        \
  void Shift(                                                                  \
    Meters source_x, Meters source_y, Meters source_z,                         \
    Meters &target_x, Meters &target_y, Meters &target_z,                      \
    Meters *dx = 0, Meters *dy = 0, Meters *dz = 0                             \
  ) const                                                                      \
  {                                                                            \
    HelmertFunction(                                                           \
      values_.dx, values_.dy, values_.dz,                                      \
      values_.wx, values_.wy, values_.wz, values_.m,                           \
      source_x, source_y, source_z,                                            \
      target_x, target_y, target_z, dx, dy, dz                                 \
    );                                                                         \
  }                                                                            \
                                                                               \
private:                                                                       \
  friend class HelmertDatumShift< TARGET, SOURCE, AUTHORITY >;                 \
};                                                                             \
/*===========================================================================*/\
template <>                                                                    \
class HelmertDatumShift< TARGET, SOURCE, AUTHORITY >:                          \
public IDatumShift                                                             \
{                                                                              \
public:                                                                        \
  HelmertDatumShift()                                                          \
  {}                                                                           \
                                                                               \
  bool IsIdentity() const { return false; }                                    \
                                                                               \
  AuthorityReference Authority() const { return AUTHORITY; }                   \
                                                                               \
  DatumReference SourceDatum() const { return TARGET; }                        \
  DatumReference TargetDatum() const { return SOURCE; }                        \
                                                                               \
private:                                                                       \
  static HelmertDatumShift< SOURCE, TARGET, AUTHORITY >::Values const &values_;\
                                                                               \
public:                                                                        \
  void Shift(Meters source_x, Meters source_y, Meters source_z,                \
             Meters &target_x, Meters &target_y, Meters &target_z,             \
             Meters *dx = 0, Meters *dy = 0, Meters *dz = 0) const             \
  {                                                                            \
    HelmertFunction(-values_.dx, -values_.dy, -values_.dz,                     \
                    -values_.wx, -values_.wy, -values_.wz, -values_.m,         \
                    source_x, source_y, source_z,                              \
                    target_x, target_y, target_z, dx, dy, dz                   \
    );                                                                         \
  }                                                                            \
};                                                                             \
/*===========================================================================*/\
}
//==============================================================================
#define CARTOGRAPHIC_DEFINE_HELMERTDATUMSHIFT(                                 \
  SOURCE, TARGET, AUTHORITY, DX, DY, DZ, WX, WY, WZ, M                         \
)                                                                              \
/*===========================================================================*/\
namespace Cartographic {                                                       \
/*===========================================================================*/\
HelmertDatumShift< SOURCE, TARGET, AUTHORITY >:: Values const                  \
  HelmertDatumShift< SOURCE, TARGET, AUTHORITY >::values_ = {                  \
    (DX), (DY), (DZ), (WX), (WY), (WZ), (M)                                    \
  },                                                                           \
  &HelmertDatumShift< TARGET, SOURCE, AUTHORITY >::values_(                    \
    HelmertDatumShift< SOURCE, TARGET, AUTHORITY >::values_                    \
  );                                                                           \
/*===========================================================================*/\
}
//==============================================================================
#define CARTOGRAPHIC_ADD_HELMERTDATUMSHIFT(                                    \
          SOURCE, TARGET, AUTHORITY, DX, DY, DZ, WX, WY, WZ, M                 \
        )                                                                      \
        CARTOGRAPHIC_DECLARE_HELMERTDATUMSHIFT(SOURCE, TARGET, AUTHORITY)      \
        CARTOGRAPHIC_DEFINE_HELMERTDATUMSHIFT(                                 \
          SOURCE, TARGET, AUTHORITY, DX, DY, DZ, WX, WY, WZ, M                 \
        )
//==============================================================================
#endif //! CARTOGRAPHIC_HELMER_DATUMSHIFT_HPP
