#include <Geodesic/Operations/MiddlePoints.hpp>

#include <Geodesic/GeodesicException.hpp>
//! MathTools
#include <MathTools/BasicMath/Compare.hpp>
//! gtest
#include <gtest/gtest.h>
//==============================================================================
namespace Geodesic
{
namespace Tests
{
//==============================================================================
/*!
 * \brief Тестирование построения минимального прямоугольника,
 * содержащего эллипс на поверхности эллипсоида
 */
TEST(MiddlePointsTest, CartesianMiddlePoint)
{
  //! Две точки на экваторе.
  GeoPoint2D mid(
    cartesianMiddlePoint(
      GeoMultiPoint2D{
        GeoPoint2D(Degree(0.), Degree(-1.)),
        GeoPoint2D(Degree(0.), Degree(1.))
      }));

  ASSERT_EQ(GeoPoint2D(), mid);

  //! Поиск центра для пустого множества точек.
  ASSERT_THROW(cartesianMiddlePoint(GeoMultiPoint2D()), GeodesicFailure);
  try
  {
    cartesianMiddlePoint(GeoMultiPoint2D());
  }
  catch (const GeodesicFailure& exception)
  {
    ASSERT_EQ(geodesicLibTitle, exception.getLib());
    ASSERT_EQ(
      GeodesicEnumFailure::NullGeometryIncorrectOperation,
      exception.getCode());
  }

  //! Поиск центра для множества из одной точки.
  GeoPoint2D initPoint(Degree(44.), Degree(44.));
  mid = cartesianMiddlePoint(
    GeoMultiPoint2D{ initPoint } );
  ASSERT_EQ(mid, initPoint);
}
//==============================================================================
} //! Tests
} //! Geodesic