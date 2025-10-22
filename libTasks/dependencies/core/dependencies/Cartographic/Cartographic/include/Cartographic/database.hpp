#ifndef CARTOGRAPHIC_DATABASE_HPP
#define CARTOGRAPHIC_DATABASE_HPP
//==============================================================================
#include <Cartographic/ellipsoid.hpp>
#include <Cartographic/helmertdatumshift.hpp>
//==============================================================================
CARTOGRAPHIC_DECLARE_AUTHORITY(GOST_32453_2017)
//==============================================================================
CARTOGRAPHIC_DECLARE_DATUM(SK_42)
CARTOGRAPHIC_DECLARE_DATUM(PZ_90_02)
CARTOGRAPHIC_DECLARE_DATUM(PZ_90_11)
CARTOGRAPHIC_DECLARE_DATUM(WGS_84)
//==============================================================================
CARTOGRAPHIC_DECLARE_ELLIPSOID(GeneralGlobe)
CARTOGRAPHIC_DECLARE_ELLIPSOID(Krasovsky)
CARTOGRAPHIC_DECLARE_ELLIPSOID(WGS_84)
//==============================================================================
CARTOGRAPHIC_DECLARE_HELMERTDATUMSHIFT(
  Datum::SK_42, Datum::PZ_90_11, Authority::GOST_32453_2017
)
CARTOGRAPHIC_DECLARE_HELMERTDATUMSHIFT(
  Datum::PZ_90_11, Datum::WGS_84, Authority::GOST_32453_2017
)
CARTOGRAPHIC_DECLARE_HELMERTDATUMSHIFT(
  Datum::PZ_90_02, Datum::PZ_90_11, Authority::GOST_32453_2017
)
//==============================================================================
#endif //! CARTOGRAPHIC_DATABASE_HPP

