#ifndef PRIMARY_ENTITIES_GEODESIC_ENTITIES_HPP
#define PRIMARY_ENTITIES_GEODESIC_ENTITIES_HPP
//==============================================================================
#include <PrimaryEntities/Entities/MultiPoint.hpp>
#include <PrimaryEntities/Entities/Linestring.hpp>
#include <PrimaryEntities/Entities/MultiLinestring.hpp>
#include <PrimaryEntities/Entities/Ring.hpp>
#include <PrimaryEntities/Entities/Polygon.hpp>
#include <PrimaryEntities/Entities/MultiPolygon.hpp>
#include <PrimaryEntities/Entities/Box.hpp>
//! Point
#include <Point/GeoPoint2D.hpp>
#include <Point/GeoPoint3D.hpp>
//==============================================================================
//! GeoPoint2D
//==============================================================================
typedef PrimaryEntities::MultiPoint<GeoPoint2D>      GeoMultiPoint2D;
typedef PrimaryEntities::Linestring<GeoPoint2D>      GeoLinestring2D;
typedef PrimaryEntities::MultiLinestring<GeoPoint2D> GeoMultiLinestring2D;
typedef PrimaryEntities::Ring<GeoPoint2D>            GeoRing2D;
typedef PrimaryEntities::Polygon<GeoPoint2D>         GeoPolygon2D;
typedef PrimaryEntities::MultiPolygon<GeoPoint2D>    GeoMultiPolygon2D;
typedef PrimaryEntities::Box<GeoPoint2D>             GeoBox2D;
//==============================================================================
//! GeoPoint3D
//==============================================================================
typedef PrimaryEntities::MultiPoint<GeoPoint3D>      GeoMultiPoint3D;
typedef PrimaryEntities::Linestring<GeoPoint3D>      GeoLinestring3D;
typedef PrimaryEntities::MultiLinestring<GeoPoint3D> GeoMultiLinestring3D;
typedef PrimaryEntities::Ring<GeoPoint3D>            GeoRing3D;
typedef PrimaryEntities::Polygon<GeoPoint3D>         GeoPolygon3D;
typedef PrimaryEntities::MultiPolygon<GeoPoint3D>    GeoMultiPolygon3D;
typedef PrimaryEntities::Box<GeoPoint3D>             GeoBox3D;
//==============================================================================
#endif //! PRIMARY_ENTITIES_GEODESIC_ENTITIES_HPP
