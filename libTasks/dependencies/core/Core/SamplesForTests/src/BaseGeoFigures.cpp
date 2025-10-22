#include <SamplesForTests/BaseGeoFigures.hpp>
#include <SamplesForTests/SamplesForTestsExceptions.hpp>
//! PrimaryEntities
#include <PrimaryEntities/GeometryConverter.hpp>
//! AbstractOperations
#include <AbstractOperations/Algorithms/Correct.hpp>
#include <AbstractOperations/Algorithms/Difference.hpp>
#include <AbstractOperations/Algorithms/Within.hpp>
//==============================================================================
namespace SamplesForTests
{
  GeoPolygon2D buildGSquare(const GeoPoint2D& center, const Degree size)
  { 
    if (BasicMath::isNull(size))
      return GeoPolygon2D();
    if ( size < Degree(0.))
      throw SamplesForTestsFailure(NegativeDistance);

    GeoPolygon2D retVal;
    Radian lat = center.getLatitude();
    Radian lon = center.getLongitude();
    Radian shift = size.toRadians() / Radian(2);
    retVal.outer().push_back(GeoPoint2D(lat + shift, lon + shift));
    retVal.outer().push_back(GeoPoint2D(lat - shift, lon + shift));
    retVal.outer().push_back(GeoPoint2D(lat - shift, lon - shift));
    retVal.outer().push_back(GeoPoint2D(lat + shift, lon - shift));
    retVal.outer().push_back(GeoPoint2D(lat + shift, lon + shift));
    return retVal;
  }
//==============================================================================
  GeoPolygon2D buildGeoRectangle(const GeoPoint2D& southWestPoint,
                                 const Degree latSize,
                                 const Degree lonSize)
  {
    if (BasicMath::isNull(latSize) || BasicMath::isNull(lonSize))
      return GeoPolygon2D();
    if (latSize < Degree(0.) || lonSize < Degree(0.))
      throw SamplesForTestsFailure(NegativeDistance);

    GeoPolygon2D retVal;
    Radian lat = southWestPoint.getLatitude();
    Radian lon = southWestPoint.getLongitude();
    Radian latDist = latSize.toRadians();
    Radian lonDist = lonSize.toRadians();
    retVal.outer().push_back(GeoPoint2D(lat, lon));
    retVal.outer().push_back(GeoPoint2D(lat + latDist, lon));
    retVal.outer().push_back(GeoPoint2D(lat + latDist, lon + lonDist));
    retVal.outer().push_back(GeoPoint2D(lat, lon + lonDist));
    retVal.outer().push_back(GeoPoint2D(lat, lon));
    return retVal;
  }
//==============================================================================
  GeoPolygon2D buildGeoRectangle(const GeoPoint2D& southWestPoint,
                                 const double latSize,
                                 const double lonSize)
  {
    if (BasicMath::isNull(latSize) || BasicMath::isNull(lonSize))
      return GeoPolygon2D();
    if (latSize < 0. || lonSize < 0.)
      throw SamplesForTestsFailure(NegativeDistance);

    return buildGeoRectangle(southWestPoint, 
                             Degree(latSize), 
                             Degree(lonSize)); 
  }       
//==============================================================================
  GeoPolygon2D buildGeoHoleRectangle(const GeoPoint2D& southWestPoint,
                                     const GeoPoint2D& holeSouthWestPoint,
                                     const Degree latSize,
                                     const Degree lonSize,
                                     const Degree holeLatSize, 
                                     const Degree holeLonSize)
  {
    if (BasicMath::isNull(latSize) || BasicMath::isNull(lonSize))
      return GeoPolygon2D();
    if (latSize < Degree(0.) || lonSize < Degree(0.))
      throw SamplesForTestsFailure(NegativeDistance);

    if (BasicMath::isNull(holeLatSize) || BasicMath::isNull(holeLonSize))
      return buildGeoRectangle(southWestPoint, latSize, lonSize);
    if (holeLatSize < Degree(0.) || holeLonSize < Degree(0.))
      throw SamplesForTestsFailure(NegativeDistance);

    GeoPolygon2D retVal; 
    Radian lat = southWestPoint.getLatitude();
    Radian lon = southWestPoint.getLongitude();
    Radian holeLat = holeSouthWestPoint.getLatitude();
    Radian holeLon = holeSouthWestPoint.getLongitude();
    Radian latDist = latSize.toRadians();
    Radian lonDist = lonSize.toRadians();
    Radian holeLatDist = holeLatSize.toRadians();
    Radian holeLonDist = holeLonSize.toRadians();
    retVal.outer().push_back(GeoPoint2D(lat, lon));
    retVal.outer().push_back(GeoPoint2D(lat + latDist, lon));
    retVal.outer().push_back(GeoPoint2D(lat + latDist, lon + lonDist));
    retVal.outer().push_back(GeoPoint2D(lat, lon + lonDist));
    retVal.outer().push_back(GeoPoint2D(lat, lon));
    retVal.inners().resize(1);
    retVal.inners()[0].push_back({GeoPoint2D(holeLat,
                                             holeLon)});
    retVal.inners()[0].push_back({GeoPoint2D(holeLat,
                                             holeLon + holeLonDist)});
    retVal.inners()[0].push_back({GeoPoint2D(holeLat + holeLatDist,
                                             holeLon + holeLonDist)});
    retVal.inners()[0].push_back({GeoPoint2D(holeLat + holeLatDist,
                                             holeLon)});
    retVal.inners()[0].push_back({GeoPoint2D(holeLat,
                                             holeLon)});
    return retVal;
  }
//==============================================================================
  GeoPolygon2D buildBow(const GeoPoint2D& southWestPoint,
                        const Degree latSize,
                        const Degree lonSize)
  {
    if (BasicMath::isNull(latSize) || BasicMath::isNull(lonSize))
      return GeoPolygon2D();
    if (latSize < Degree(0.) || lonSize < Degree(0.))
      throw SamplesForTestsFailure(NegativeDistance);
      
    GeoPolygon2D retVal;
    Radian lat = southWestPoint.getLatitude();
    Radian lon = southWestPoint.getLongitude();
    Radian latDist = latSize.toRadians();
    Radian lonDist = lonSize.toRadians();
    retVal.outer().push_back(GeoPoint2D(lat, lon));
    retVal.outer().push_back(GeoPoint2D(lat + latDist, lon));
    retVal.outer().push_back(GeoPoint2D(lat, lon + lonDist));
    retVal.outer().push_back(GeoPoint2D(lat + latDist, lon + lonDist));
    retVal.outer().push_back(GeoPoint2D(lat, lon));
    return retVal;
  }
//==============================================================================
  GeoPolygon2D buildPost(const GeoPoint2D& southWestPoint,
                         const Degree latSize,
                         const Degree lonSize)
  {
    if (BasicMath::isNull(latSize) || BasicMath::isNull(lonSize))
      return GeoPolygon2D();
    if (latSize < Degree(0.) || lonSize < Degree(0.))
      throw SamplesForTestsFailure(NegativeDistance);

    GeoPolygon2D retVal;
    Radian lat = southWestPoint.getLatitude();
    Radian lon = southWestPoint.getLongitude();
    Radian latDist = latSize.toRadians();
    Radian lonDist = lonSize.toRadians();
    retVal.outer().push_back(GeoPoint2D(lat, lon));
    retVal.outer().push_back(GeoPoint2D(lat + latDist, lon));
    retVal.outer().push_back(GeoPoint2D(lat, lon + lonDist / 2));
    retVal.outer().push_back(GeoPoint2D(lat + latDist, lon + lonDist));
    retVal.outer().push_back(GeoPoint2D(lat, lon + lonDist));
    retVal.outer().push_back(GeoPoint2D(lat, lon));
    return retVal;
  }
//==============================================================================
  void addHole(GeoPolygon2D& polygon,
    const GeoPoint2D& holePoint,
    const double holeLatSize,
    const double holeLonSize)
  {
    if (!AbstractOperations::isCorrect(polygon))
      throw SamplesForTestsFailure(IncorrectGeometry);

    if (BasicMath::isNull(holeLatSize) || BasicMath::isNull(holeLonSize))
      return;
    if (holeLatSize < 0. || holeLonSize < 0.)
      throw SamplesForTestsFailure(NegativeDistance);

    GeoRing2D hole;
    Radian lat = holePoint.getLatitude();
    Radian lon = holePoint.getLongitude();
    Radian holeLatDist = Degree(holeLatSize).toRadians();
    Radian holeLonDist = Degree(holeLonSize).toRadians();
    hole.push_back(GeoPoint2D(lat, lon));
    hole.push_back(GeoPoint2D(lat + holeLatDist, lon));
    hole.push_back(GeoPoint2D(lat + holeLatDist, lon + holeLonDist));
    hole.push_back(GeoPoint2D(lat, lon + holeLonDist));
    hole.push_back(GeoPoint2D(lat, lon));

    if (!AbstractOperations::within(hole, GeoPolygon2D({ {polygon.outer()} })))
      throw (SamplesForTestsFailure(HoleNotInsidePolygon));
    
    GeoPolygon2D tmpPolygon;
    AbstractOperations::difference(polygon, hole, polygon);
  }
//==============================================================================
} //! SamplesForTests
//==============================================================================