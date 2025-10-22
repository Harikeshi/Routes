#include <Geodesic/Operations/DistanceToLinestring.hpp>

#include <Geodesic/Projection/Transition.hpp>
#include <Geodesic/Projection/Orthographic/ProjectionBuilder.hpp>
#include <Geodesic/GeodesicException.hpp>
#include <Geodesic/GeoLineTools/OrthodromyTools.hpp>
#include <Geodesic/GeoLineTools/LoxodromeTools.hpp>
//! Geometry
#include <Geometry/Operations/DistanceToLinestring.hpp>
//==============================================================================
namespace Geodesic
{
//==============================================================================
  double distanceBetweenPointAndLinestring(
    const GeoPoint2D& point,
    const GeoLinestring2D& inputLinestring,
    bool useOrthodromy, bool useAllPointsForProj/* = false*/)
  {
    if (inputLinestring.empty())
      throw GeodesicFailure(GeodesicEnumFailure::EmptyLinestring);

    double resDistance;

    try
    {
      // Разобъём контур на ортодромии/локсодромии
      auto linestring(inputLinestring);
      if (useOrthodromy)
      {
        OrthodromyTools tools;
        tools.divide(linestring);
      }
      else
      {
        LoxodromeTools tools;
        tools.divide(linestring);
      }

      // Найдём оптимальную проекцию
      std::shared_ptr<OrthographicProjection> projection;
      
      if (useAllPointsForProj)
      {
        // Запишем вместе все точки
        GeoMultiPoint2D allPoints(linestring.begin(), linestring.end());
          allPoints.push_back(point);
          projection = std::make_shared<OrthographicProjection>(
            findOptimalProjection<GeoMultiPoint2D>(allPoints));
      }
      else
      {
        projection = std::make_shared<OrthographicProjection>(point);
      }
     

      // Найдём кратчайшее расстояние от точки до линии на плоскости
      resDistance = Geometry::distanceBetweenPointAndLinestring(
        geoGeometryToProjection(point, projection),
        geoGeometryToProjection(linestring, projection));
    }
    catch (...)
    {
      ExceptionTools::FailureHandler<GeodesicFailure>(
        "distanceBetweenPointAndLinestring");
    }

    return resDistance;
  }
//==============================================================================
} //! Geodesic
//==============================================================================