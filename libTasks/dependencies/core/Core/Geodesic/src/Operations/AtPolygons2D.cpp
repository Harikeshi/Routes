#include <Geodesic/Operations/AtPolygons2D.hpp>

#include <Geodesic/GeodesicException.hpp>
#include <Geodesic/Projection/Orthographic/Orthographic.hpp>
#include <Geodesic/Projection/Transition.hpp>
//! AbstractOperations
#include <AbstractOperations/Algorithms/Area.hpp>
#include <AbstractOperations/Algorithms/Envelope.hpp>
//! Geometry 
#include <Geometry/ComplicatedOperations/SmartMiddlePointInPolygon.hpp>
//==============================================================================
namespace Geodesic
{
//==============================================================================
GeoPolygon2D chooseBiggestPolygon(const GeoMultiPolygon2D& multiPoly)
{
  //! Если пусто, то возвращаем пустой полигон
  if (multiPoly.empty())
    return GeoPolygon2D();

  //! Если в коллекции всего один полигон
  if (multiPoly.size() == 1)
    return multiPoly.at(0);

  // Cоздадим коробку для полигона
  GeoBox2D box;
  AbstractOperations::envelope(multiPoly, box);

  // Вычислим центр проекции
  Radian lonCenter( (box.max_corner().getLongitude() 
                     + box.min_corner().getLongitude()) / 2.);
  Radian latCenter( (box.max_corner().getLatitude() 
                     + box.min_corner().getLatitude()) / 2.);

  // Зададим проекцию
  OrthographicProjection projection(std::make_shared<Ellipsoid>(EL_WGS_84),
                                    GeoPoint3D(latCenter, lonCenter));

  // Выберем полигон, площадь которого является наибольшей
  size_t resultInd(0);
  try
  {
    double maxArea(AbstractOperations::area(
      projection.toProjection<GeoPoint2D, Point2D>({ multiPoly[0] })));

    for (size_t i = 1; i < multiPoly.size(); ++i)
    {
      double curArea(AbstractOperations::area(
        projection.toProjection<GeoPoint2D, Point2D>({ multiPoly[i] } )));

      if (curArea - maxArea > 1.) //! Превышает хотя бы на 1 кв.м.
      {
        maxArea = curArea;
        resultInd = i;
      }
    }
  }
  catch (...)
  {
    ExceptionTools::FailureHandler<GeodesicFailure>("chooseBiggestPolygon");
  }

  //! Вернём полученный результат
  return multiPoly.at(resultInd);
}
//==============================================================================
} //! Geodesic
//==============================================================================