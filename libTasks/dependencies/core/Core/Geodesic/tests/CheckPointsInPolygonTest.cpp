#include <Geodesic/Operations/CheckPointsInPolygon.hpp>

#include <Geodesic/GeodesicException.hpp>
//! PrimaryEntities
#include <PrimaryEntities/GeodesicEntities.hpp>
//! gtest
#include <gtest/gtest.h>
//==============================================================================
namespace Geodesic
{
namespace Tests
{
//==============================================================================
TEST(AlgInProj, CheckPointsInPolygonTest)
{
  std::vector<std::pair<GeoPoint2D, double>> points;
  // точка в области
  points.emplace_back(std::make_pair(GeoPoint2D(Degree(2), Degree(2)), 0));
  // площадная точка в области
  points.emplace_back(std::make_pair(GeoPoint2D(Degree(2.5), Degree(2.5)), 100));
  // точка вне области
  points.emplace_back(std::make_pair(GeoPoint2D(Degree(4), Degree(4)), 0));
  // площадная точка, частично попавшая в область
  points.emplace_back(std::make_pair(GeoPoint2D(Degree(3), Degree(3)), 100));

  GeoMultiPolygon2D poly;
  GeoPolygon2D polyInner({ { GeoPoint2D(Degree(1), Degree(1)) ,
                             GeoPoint2D(Degree(1), Degree(3)) ,
                             GeoPoint2D(Degree(3), Degree(3)) ,
                             GeoPoint2D(Degree(3), Degree(1)) ,
                             GeoPoint2D(Degree(1), Degree(1)) } });
  poly.emplace_back(polyInner);

  // тестирование на мультиполигоне
  // ортодромия
  auto result = checkPointsInPolygon(points, poly, true);

  ASSERT_EQ(result.at(0), true);
  ASSERT_EQ(result.at(1), true);
  ASSERT_EQ(result.at(2), false);
  ASSERT_EQ(result.at(3), false);

  // локсодромия
  result = checkPointsInPolygon(points, poly, false);

  ASSERT_EQ(result.at(0), true);
  ASSERT_EQ(result.at(1), true);
  ASSERT_EQ(result.at(2), false);
  ASSERT_EQ(result.at(3), false);

  // тестирование на полигоне
  // ортодромия
  result = checkPointsInPolygon(points, polyInner, true);

  ASSERT_EQ(result.at(0), true);
  ASSERT_EQ(result.at(1), true);
  ASSERT_EQ(result.at(2), false);
  ASSERT_EQ(result.at(3), false);

  // локсодромия
  result = checkPointsInPolygon(points, polyInner, false);

  ASSERT_EQ(result.at(0), true);
  ASSERT_EQ(result.at(1), true);
  ASSERT_EQ(result.at(2), false);
  ASSERT_EQ(result.at(3), false);

  // проверка для пустой геометрии
  result = checkPointsInPolygon(points, GeoPolygon2D(), false);

  ASSERT_EQ(result.at(0), false);
  ASSERT_EQ(result.at(1), false);
  ASSERT_EQ(result.at(2), false);
  ASSERT_EQ(result.at(3), false);

  // попробуем на бантике
  GeoPolygon2D bow({ { GeoPoint2D(Degree(1), Degree(1)) ,
    GeoPoint2D(Degree(3), Degree(3)) ,
    GeoPoint2D(Degree(1), Degree(3)) ,
    GeoPoint2D(Degree(3), Degree(1)) ,
    GeoPoint2D(Degree(1), Degree(1)) } });

  ASSERT_THROW(checkPointsInPolygon(points, bow, false), GeodesicFailure);
  try
  {
    result = checkPointsInPolygon(points, bow, false);
  }
  catch (const GeodesicFailure& exception)
  {
    ASSERT_EQ(ExceptionTools::TransitFailure, exception.getCode());
    ASSERT_EQ(geodesicLibTitle, exception.getLib());
    ASSERT_FALSE(std::string(exception.what()).find("checkPointsInPolygon")
      == std::string::npos);
  }

  // отрицательный радиус
  points.emplace_back(std::make_pair(GeoPoint2D(Degree(3), Degree(3)), -100));

  ASSERT_THROW(checkPointsInPolygon(points, polyInner, false),
               GeodesicFailure);
  try
  {
    result = checkPointsInPolygon(points, polyInner, false);
  }
  catch (const GeodesicFailure& exception)
  {
    ASSERT_EQ(NegativeRadius, exception.getCode());
    ASSERT_EQ( geodesicLibTitle, exception.getLib());
  }
}
//==============================================================================
} //! Tests
} //! Geodesic
//==============================================================================