#include <Geometry/Operations/PointInPolygon.hpp>

#include <Geometry/Objects/BorderedLine.hpp>
#include <Geometry/Operations/AtPoints2D.hpp>
#include <Geometry/GeometryException.hpp>
//! PrimaryEntities
#include <PrimaryEntities/GeometryConverter.hpp>
//! AbstractOperations
#include <AbstractOperations/Algorithms/Correct.hpp>
#include <AbstractOperations/Algorithms/CoveredBy.hpp>
#include <AbstractOperations/Algorithms/Within.hpp>
//! std
#include <functional>
//==============================================================================
namespace Geometry
{
//==============================================================================
  template <typename Geometry1, typename Geometry2>
    using isPointInsideFunction
      = std::function<bool(Geometry1, Geometry2, bool, bool)>;
//==============================================================================
  Point2D findCentroid(const Polygon2D& polygon)
  {
    if (polygon.outer().empty())
      throw GeometryFailure(GeometryEnumFailure::EmptyPolygon);

    // Ищем центр масс
    std::unique_ptr<geos::geom::Point> center;

    try
    {
      center = PrimaryEntities::toGeosGeometry(polygon)->getCentroid();
    }
    catch (...)
    {
      ExceptionTools::FailureHandler<GeometryFailure>("findCentroid");
    }

    // Если вернулся некорректный центр масс, возвращаем ошибку
    if (center->isEmpty())
      throw GeometryFailure(GeometryEnumFailure::CentroidIsNotFound);

    return PrimaryEntities::fromGeosGeometry<Point2D>(center.get());
  }
//==============================================================================
  void findNearestPoints(
    const Polygon2D& polygon,
    const Point2D& point,
    std::vector<Point2D>& nearestPoints,
    double& nearestDist)
  {
    nearestPoints.clear();

    // Если полигон пуст, ближайших точек нет
    if (polygon.outer().empty())
    {
      nearestDist = 0;
      return;
    }
    // Для начала определим, попала ли точка в какую-то из дыр или
    // находится снаружи полигона. Если индекс кольца -1, то точка
    // снаружи, иначе индекс кольца обозначает номер внутреннего
    // кольца полигона, в который попала точка
    int ringIndex(-1);
    for (size_t i = 0; i < polygon.inners().size(); ++i)
    {
      if (AbstractOperations::within(point, polygon.inners()[i], true))
      {
        ringIndex = i;
        break;
      }
    }

    Ring2D nearestRing
      = (ringIndex < 0) ? polygon.outer() : polygon.inners()[ringIndex];

    // Для начала примем за кратчайшее расстояние - расстояние до
    // первой точки кольца
    nearestDist = distance(point, nearestRing[0]);

    // Пройдём по всем рёбрам кольца, чтобы найти ближайшие
    for (size_t i = 0; i < nearestRing.size() - 1; ++i)
    {
      // Текущее ребро
      BorderedLine curLine(nearestRing[i], nearestRing[i + 1]);
      // Перпендикуляр к нему от заданной точки
      Line perpLine = curLine.perpendicularLine(point);
      // Проекция заданной точки на ребро
      Point2D curNearestPoint = perpLine.crossPoint(curLine);

      // если точка пересечения не попала на отрезок, возьмём в 
      // качестве искомой точки ближайшую из граничных
      if (!curLine.nearBorderLine(curNearestPoint, 1e-06))
      {
        if (distance(point, nearestRing[i]) < distance(point, nearestRing[i + 1]))
          curNearestPoint = nearestRing[i];
        else
          curNearestPoint = nearestRing[i + 1];
      }

      // расстояние от заданной точки до проекции
      double dist = distance(point, curNearestPoint);
      if (BasicMath::isNull(dist - nearestDist))
      {
        // Если расстояние до текущей точки равно кратчайшему,
        // добавляем точку в вектор ближайших точек
        nearestPoints.emplace_back(curNearestPoint);
      }
      else if (dist < nearestDist)
      {
        // Если новое расстояние оказалось лучше предыдущего, всю 
        // информацию о предыдущем можно удалить
        nearestPoints.clear();

        nearestPoints.emplace_back(curNearestPoint);
        nearestDist = dist;
      }
    }
  }
//==============================================================================
  bool checkPointsConnectivity(
    const Point2D& firstPoint, const Point2D& secondPoint,
    const MultiPolygon2D& mpolygon, bool withBorders)
  {
    // Нет полигона, не можем оценивать связность
    if (mpolygon.empty())
      throw GeometryFailure(GeometryEnumFailure::EmptyPolygon);

    isPointInsideFunction<Point2D, MultiPolygon2D> isPointInside;

    if (withBorders)
      isPointInside
        = AbstractOperations::coveredBy<Point2D, MultiPolygon2D>;
    else
      isPointInside
        = AbstractOperations::within<Point2D, MultiPolygon2D>;

    // Если хотя бы одна из точек не попала в полигон, ни о какой связности не
    // может быть и речи
    if ( !isPointInside(firstPoint, mpolygon, false, true)
      || !isPointInside(secondPoint, mpolygon, false, true))
      return false;

    // Ищем полигон, в котором оказалась первая точка
    for (const auto& poly : mpolygon)
    {
      if (isPointInside(firstPoint, { poly }, false, true))
      {
        // Если первая точка оказалась в полигоне, вторая должна оказаться
        // в нём же, иначе связности нет
        if (isPointInside(secondPoint, { poly }, false, true))
          return true;
        else
          return false;
      }
    }

    // Сюда прийти не должны. Если дошли, что-то пошло не так.
    return false;
  }
//==============================================================================
} //! Geometry
//==============================================================================