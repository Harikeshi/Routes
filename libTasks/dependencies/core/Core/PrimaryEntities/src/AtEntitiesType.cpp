#include <PrimaryEntities/AtEntitiesType.hpp>
//! PrimaryEntities 
#include <PrimaryEntities/GeometryEntities.hpp>
#include <PrimaryEntities/GeodesicEntities.hpp>
//! std
#include <typeindex>
//==============================================================================
namespace PrimaryEntities
{
//==============================================================================
unsigned int analyseEntityType1(const std::type_info& typeEntity)
{
  size_t typeCode = typeEntity.hash_code();

  //! Классификация сущности

  if (typeCode == typeid(Point2D).hash_code())            //!< Point2D
    return GeometryEntity | DottedEntity | Dim2DEntity | SingleEntity;

  if (typeCode == typeid(MultiPoint2D).hash_code())       //!< MultiPoint2D
    return GeometryEntity | DottedEntity | Dim2DEntity | CollectionEntity;

  if (typeCode == typeid(Point3D).hash_code())            //!< Point3D
    return GeometryEntity | DottedEntity | Dim3DEntity | SingleEntity;

  if (typeCode == typeid(MultiPoint3D).hash_code())       //!< MultiPoint3D
    return GeometryEntity | DottedEntity | Dim3DEntity | CollectionEntity;

  if (typeCode == typeid(Linestring2D).hash_code())       //!< Linestring2D
    return GeometryEntity | LinearEntity | Dim2DEntity | SingleEntity;

  if (typeCode == typeid(MultiLinestring2D).hash_code())  //!< MultiLinestring2D
    return GeometryEntity | LinearEntity | Dim2DEntity | CollectionEntity;

  if (typeCode == typeid(Linestring3D).hash_code())       //!< Linestring3D
    return GeometryEntity | LinearEntity | Dim3DEntity | SingleEntity;

  if (typeCode == typeid(MultiLinestring3D).hash_code())  //!< MultiLinestring3D
    return GeometryEntity | LinearEntity | Dim3DEntity | CollectionEntity;

  if (   (typeCode == typeid(Box2D).hash_code())          //!< Box2D
      || (typeCode == typeid(Ring2D).hash_code())         //!< Ring2D
      || (typeCode == typeid(Polygon2D).hash_code()))     //!< Polygon2D
    return GeometryEntity | ArealEntity | Dim2DEntity | SingleEntity;

  if (typeCode == typeid(MultiPolygon2D).hash_code())     //!< MultiPolygon2D
    return GeometryEntity | ArealEntity | Dim2DEntity | CollectionEntity;

  if (   (typeCode == typeid(Box3D).hash_code())          //!< Box3D
      || (typeCode == typeid(Ring3D).hash_code())         //!< Ring3D
      || (typeCode == typeid(Polygon3D).hash_code()))     //!< Polygon3D
    return GeometryEntity | ArealEntity | Dim3DEntity | SingleEntity;

  if (typeCode == typeid(MultiPolygon3D).hash_code())     //!< MultiPolygon3D
    return GeometryEntity | ArealEntity | Dim3DEntity | CollectionEntity;

  if (typeCode == typeid(GeoPoint2D).hash_code())         //!< GeoPoint2D
    return GeodesicEntity | DottedEntity | Dim2DEntity | SingleEntity;

  if (typeCode == typeid(GeoMultiPoint2D).hash_code())    //!< GeoMultiPoint2D
    return GeodesicEntity | DottedEntity | Dim2DEntity | CollectionEntity;

  if (typeCode == typeid(GeoPoint3D).hash_code())         //!< GeoPoint3D
    return GeodesicEntity | DottedEntity | Dim3DEntity | SingleEntity;

  if (typeCode == typeid(GeoLinestring2D).hash_code())    //!< GeoLinestring2D
    return GeodesicEntity | LinearEntity | Dim2DEntity | SingleEntity;

  if (typeCode == typeid(GeoMultiLinestring2D).hash_code()) //!< GeoMultiLinestring2D
    return GeodesicEntity | LinearEntity | Dim2DEntity | CollectionEntity;

  if (   (typeCode == typeid(GeoBox2D).hash_code())       //!< GeoBox2D
      || (typeCode == typeid(GeoRing2D).hash_code())      //!< GeoRing2D
      || (typeCode == typeid(GeoPolygon2D).hash_code()))  //!< GeoPolygon2D
    return GeodesicEntity | ArealEntity | Dim2DEntity | SingleEntity;

  if (typeCode == typeid(GeoMultiPolygon2D).hash_code())  //!< GeoMultiPolygon2D
    return GeodesicEntity | ArealEntity | Dim2DEntity | CollectionEntity;

  return UndefineEntity; //!< тип неопределён 
}
//==============================================================================
} //! PrimaryEntities
//==============================================================================
