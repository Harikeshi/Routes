#ifndef PRIMARY_ENTITIES_GEOMETRY_ENTITIES_HPP
#define PRIMARY_ENTITIES_GEOMETRY_ENTITIES_HPP
//==============================================================================
#include <PrimaryEntities/Entities/MultiPoint.hpp>
#include <PrimaryEntities/Entities/Linestring.hpp>
#include <PrimaryEntities/Entities/MultiLinestring.hpp>
#include <PrimaryEntities/Entities/Ring.hpp>
#include <PrimaryEntities/Entities/Polygon.hpp>
#include <PrimaryEntities/Entities/MultiPolygon.hpp>
#include <PrimaryEntities/Entities/Box.hpp>
//! Point
#include <Point/Point2D.hpp>
#include <Point/Point3D.hpp>
//==============================================================================
//! Point2D
//==============================================================================
typedef PrimaryEntities::MultiPoint<Point2D>      MultiPoint2D;
typedef PrimaryEntities::Linestring<Point2D>      Linestring2D;
typedef PrimaryEntities::MultiLinestring<Point2D> MultiLinestring2D;
typedef PrimaryEntities::Ring<Point2D>            Ring2D;
typedef PrimaryEntities::Polygon<Point2D>         Polygon2D;
typedef PrimaryEntities::MultiPolygon<Point2D>    MultiPolygon2D;
typedef PrimaryEntities::Box<Point2D>             Box2D;
//==============================================================================
//! Point3D
//==============================================================================
typedef PrimaryEntities::MultiPoint<Point3D>      MultiPoint3D;
typedef PrimaryEntities::Linestring<Point3D>      Linestring3D;
typedef PrimaryEntities::MultiLinestring<Point3D> MultiLinestring3D;
typedef PrimaryEntities::Ring<Point3D>            Ring3D;
typedef PrimaryEntities::Polygon<Point3D>         Polygon3D;
typedef PrimaryEntities::MultiPolygon<Point3D>    MultiPolygon3D;
typedef PrimaryEntities::Box<Point3D>             Box3D;
//==============================================================================
#endif //! PRIMARY_ENTITIES_GEOMETRY_ENTITIES_HPP
