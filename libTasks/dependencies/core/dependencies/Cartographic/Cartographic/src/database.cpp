#include <Cartographic/database.hpp>
#include <Cartographic/math.hpp>
//==============================================================================
CARTOGRAPHIC_DEFINE_AUTHORITY(GOST_32453_2017)
//==============================================================================
CARTOGRAPHIC_DEFINE_DATUM(SK_42)
CARTOGRAPHIC_DEFINE_DATUM(PZ_90_02)
CARTOGRAPHIC_DEFINE_DATUM(PZ_90_11)
CARTOGRAPHIC_DEFINE_DATUM(WGS_84)
//==============================================================================
CARTOGRAPHIC_DEFINE_ELLIPSOID(GeneralGlobe, 6378136., 1. / 298.25784)
CARTOGRAPHIC_DEFINE_ELLIPSOID(Krasovsky, 6378245., 1. / 298.3)
CARTOGRAPHIC_DEFINE_ELLIPSOID(WGS_84,    6378137., 1. / 298.257223563)
//==============================================================================
CARTOGRAPHIC_DEFINE_HELMERTDATUMSHIFT(
  Datum::SK_42, Datum::PZ_90_11, Authority::GOST_32453_2017,
  23.557, -140.844, -79.778,
  -.00230 / 3600. * Rho, -.34646 / 3600. * Rho, -.79421 / 3600. * Rho,
  -.228e-6
)

CARTOGRAPHIC_DEFINE_HELMERTDATUMSHIFT(
  Datum::PZ_90_11, Datum::WGS_84, Authority::GOST_32453_2017,
  .013, -.106, -.022,
  .00230 / 3600. * Rho, -.00354 / 3600. * Rho, .00421 / 3600. * Rho,
  .008e-6
)

CARTOGRAPHIC_DEFINE_HELMERTDATUMSHIFT(
  Datum::PZ_90_02, Datum::PZ_90_11, Authority::GOST_32453_2017,
  -.373, .186, .202,
  -.00230 / 3600. * Rho, 0.00354 / 3600. * Rho, -.00421 / 3600. * Rho,
  -.008e-6
)
