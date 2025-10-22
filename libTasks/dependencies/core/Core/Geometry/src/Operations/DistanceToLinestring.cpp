#include <Geometry/Operations/DistanceToLinestring.hpp>

#include <Geometry/GeometryException.hpp>
//! PrimaryEntities
#include <PrimaryEntities/GeometryConverter.hpp>
//! AbstractOperations
#include <AbstractOperations/Algorithms/Distance.hpp>
//==============================================================================
namespace Geometry
{
//==============================================================================
  double distanceBetweenPointAndLinestring(
    const Point2D& point, const Linestring2D& linestring)
  {
    if (linestring.empty())
      throw GeometryFailure(GeometryEnumFailure::EmptyLinestring);

    double resDistance;

    try
    {
      resDistance = AbstractOperations::distanceBetweenGeometries(point,
                                                                  linestring);
    }
    catch (...)
    {
      ExceptionTools::FailureHandler<GeometryFailure>(
        "distanceBetweenPointAndLinestring");
    }

    return resDistance;
  }
//==============================================================================
} //! Geometry
//==============================================================================