#include <Inner/Visibility.hpp>
//! ReachableAreaCalculator
#include <ReachableArea/ReachableAreaException.hpp>

#ifndef Q_MOC_RUN
//! AbstractOperations
#include <AbstractOperations/DeleteExcessVertices.hpp>
#include <AbstractOperations/Algorithms/Within.hpp>
#endif
//==============================================================================
using namespace ReachableArea;
//==============================================================================
namespace ReachableAreaCalculator
{
//==============================================================================
PolarPoint::
PolarPoint(
  const Point2D& point, 
  const Point2D& origin, 
  uinttype envId)
  : Point2D(point)
  , firstForId(0U)
  , lastForId(0U)
  , mRadius(Geometry::distance(origin, point))
  , mAzimuth((mRadius > 0.) ? Geometry::lineAngularity(origin, point).get() : 0)
  , mEnvId(envId)
  , mId(0U)
{ }
//==============================================================================
PolarPoint::
PolarPoint(
  double radius, 
  double azimuth, 
  const Point2D& origin,
  uinttype envId) 
  : Point2D(origin.getX()+radius*std::cos(azimuth)
  , origin.getY()+radius*std::sin(azimuth))
  , firstForId(0U)
  , lastForId(0U)
  , mRadius(radius)
  , mAzimuth(azimuth)
  , mEnvId(envId)
  , mId(0U)
{ }
//==============================================================================
bool PolarPoint::setAzimuth(const Angle& azimuth)
{
  if (mRadius != 0)
    return false;

  mAzimuth = azimuth;

  return true;
}
//==============================================================================
bool PolarEdgeFunctor::operator () (uinttype e1id, uinttype e2id) const
{
  // для случая 0 == 0
  if (e1id == e2id)
  {
    // false не нарушает транзитивность
    return false;
  }

  // любая границы перекрывает отсутствие границы
  if (!e1id)
    return true;
  if (!e2id)
    return false;

  if (e1id > mPolarEdges.size() || e2id > mPolarEdges.size())
    throw ReachableAreaFailure(PolarEdgeFunctorEdgeIdOutOfRange);

  // определение границ по их id
  const GeometrySubEnvironment::Edge& e1 = mPolarEdges[e1id - 1];
  const GeometrySubEnvironment::Edge& e2 = mPolarEdges[e2id - 1];

  // определение начальных и конечных точек границ
  const PolarPoint& e1First = mPolarPoints[e1.first - 1];
  const PolarPoint& e1Last = mPolarPoints[e1.second - 1];
  const PolarPoint& e2First = mPolarPoints[e2.first - 1];
  const PolarPoint& e2Last = mPolarPoints[e2.second - 1];

  // граница, имеющая в своём составе центральную точку, заведомо
  // не может быть перекрыта
  if (e1First.radius() == 0 || e1Last.radius() == 0)
    return false;
  if (e2First.radius() == 0 || e2Last.radius() == 0)
    return true;

  // если начальные точки имеют одинаковое значение полярного угла,
  // но разные радиусы, то перекрываема граница, расположенная дальше
  if (e1First.azimuth().value() == e2First.azimuth().value() &&
    e1First.radius() != e2First.radius())
    return e1First.radius() > e2First.radius();

  // определение, которая из границ заметается первой
  bool e1IsFirst
    = (e1First.azimuth().deltaAngle(e2First.azimuth()) > 0);

  // граница, заметённая первой, - first; другая - second.
  // определение начальных и конечных точек в новых обозначениях
  const PolarPoint& firstFirst = e1IsFirst ? e1First : e2First;
  const PolarPoint& firstLast = e1IsFirst ? e1Last : e2Last;
  const PolarPoint& secondFirst = e1IsFirst ? e2First : e1First;
  const PolarPoint& secondLast = e1IsFirst ? e2Last : e1Last;

  // true, если second перекрывает first
  bool tempResult;

  if (secondFirst.radius() > firstFirst.radius() &&
    secondFirst.radius() > firstLast.radius())
  {
    // если second заведомо дальше first и никак не может его перекрыть
    tempResult = false;
  }
  else
  {
    // вспомогательный угол - от начала first до начала second
    Angle signalAzimuth(
      Geometry::lineAngularity(firstFirst, secondFirst).get());
    
    // угол границы first
    Angle firstAzimuth(Geometry::lineAngularity(firstFirst, firstLast).get());
    
    // начало second лежит на first - изменить вспомогательный угол
    if (signalAzimuth.value() == firstAzimuth.value())
      signalAzimuth = Geometry::lineAngularity(firstFirst, secondLast).get();

    // определение, с какой стороны от first лежит second
    // через вспомогательные углы
    tempResult = firstAzimuth.deltaAngle(signalAzimuth) > 0;
  }

  // возвращение от псевдонимов first / second к e1 / e2
  if (e1IsFirst)
    return tempResult;

  return !tempResult;
}
//==============================================================================
void Visibility::process(
  const GeometrySubEnvironment* sub,
  const Point2D* center,
  double maxPath,
  Angle viewZoneBegin,
  Angle viewZoneEnd,
  ProcessMode mode,
  ProcessResult& result,
  uinttype segmentsNumber)
{
  const EnvironmentPoint* centerEnv =
    dynamic_cast<const EnvironmentPoint*>(center);

  // все точки в полярной СК
  std::vector<PolarPoint> polarPoints; 
  // обрабатываемые границы. Хранят id точек в полярной СК
  std::vector<GeometrySubEnvironment::Edge> polarEdges;

  // учитывать ограниченя зоны обзора
  bool useViewZone = true; 
  double viewZoneSize = viewZoneBegin.deltaAngleCounterclockwise(
    viewZoneEnd);

  if (viewZoneSize == 0)
  {
    // любое число, большее 2пи
    viewZoneSize = 7.0; 
    useViewZone = false;
  }

  // Идентификаторы служебных точек, опредлеляющих ограничения зоны 
  // обзора
  uinttype viewZoneBeginPointId;
  uinttype viewZoneEndPointId;

  // Заполнить polarPoints и polarEdges
  createCalculationEnvironment(
    sub, center, maxPath,
    viewZoneBegin, viewZoneSize,
    polarPoints, polarEdges,
    viewZoneBeginPointId, viewZoneEndPointId);

  // нет границ - область видимости есть круг
  if (polarEdges.empty()) 
  {
    Arc arc(*center, centerEnv->id(),
      maxPath, Angle(0), Angle(2 * M_PI), false);

    if (useViewZone)
      arc.setBeginEndAngles(viewZoneBegin, viewZoneEnd);

    if (mode & ResArea)
    {
      if (useViewZone)
        result.area.outer().push_back(*center);

      arc.line(result.area.outer(), segmentsNumber);
      AbstractOperations::deleteExcessVertices(result.area.outer(), true);
    }

    if (mode & ResArcs)
    {
      arc.setBeginInfo(ByViewZone);
      arc.setEndInfo(ByViewZone);
      result.arcs.push_back(arc);
    }

    return;
  }

  std::vector<const PolarPoint*> polarPointsSort;
  std::vector<uinttype> sweepDiagram;

  // заполнить polarPointsSort и получить диаграмму заметания
  createSweepDiagram(polarPoints, polarEdges, polarPointsSort, sweepDiagram);

  const GeometryEnvironment* environment = sub->parentEnvironment();
  uinttype pointsCount = uinttype(sweepDiagram.size());

  // признак нахождения заметающего луча в зоне обзора
  bool sweepRayInViewZone =
    viewZoneBegin.deltaAngleCounterclockwise(Angle(0)) <= viewZoneSize;

  // сбор результатов из диаграммы
  for (uinttype i = 0; i < pointsCount; i++) 
  {
    // позиция следующей точки
    uinttype next = (i < pointsCount - 1) ? i + 1 : 0;
    // позиция предыдущей точки
    uinttype prev = (i > 0) ? i - 1 : pointsCount - 1;
    
    // текущая
    const PolarPoint* currentPoint = polarPointsSort[i]; 

    // луч вне зоны обзора
    if (useViewZone && !sweepRayInViewZone) 
    {
      // пересечение границы зоны обзора
      if (currentPoint->id() == viewZoneBeginPointId)
      {
        sweepRayInViewZone = true;

        if (mode & ResArea)
        {
          result.area.outer().push_back(*center);

          // после попадания в зону обзора сразу уперлись в границу
          if (sweepDiagram[i])
          {
            const PolarPoint& p1 =
              polarPoints[polarEdges[
                sweepDiagram[i] - 1].first - 1];

            const PolarPoint& p2 =
              polarPoints[polarEdges[
                sweepDiagram[i] - 1].second - 1];

            result.area.outer().push_back(
              rayIntersectEdge(p1, p2, *center, viewZoneBegin));
          }
        }
      }
      continue;
    }

    // обработка последней точки активной границы
    if (currentPoint->lastForId && sweepDiagram[i] &&
      polarEdges[sweepDiagram[i] - 1].second == currentPoint->id())
    {
      // граница прерывается
      if (!currentPoint->firstForId) 
      {
        if (mode & ResArea)
        {
          result.area.outer().push_back(*currentPoint);
        }

        // текущая - заведомо точка преломления
        if ((mode & ResRefractions) && currentPoint->envId())
          result.refractionPoints.push_back(currentPoint->envId());

        // куда-то уперлись
        if (sweepDiagram[next]) 
        {
          // далее попытка добавить "залив"
          uinttype nextEdgeId = sweepDiagram[next];

          const PolarPoint & p1 =
            polarPoints[polarEdges[nextEdgeId - 1].first - 1];
          const PolarPoint & p2 =
            polarPoints[polarEdges[nextEdgeId - 1].second - 1];

          Point2D intersectPoint =
            rayIntersectEdge(p1, p2, *center,
              currentPoint->azimuth());

          std::vector<std::pair<uinttype, double> > unvisPoints;

          if ((mode & AddUnvisible) &&
            addUnvisibleArea(sub, *center, maxPath,
              *currentPoint, p1, intersectPoint, true, unvisPoints))
          {
            if (currentPoint->envId())
            {
              if (mode & ResRefractions)
                result.refractionPoints.pop_back();

              if (mode & ResAbsorptions)
                result.absorptionPoints.push_back(currentPoint->envId());
            }

            // при добавлении "залива" точку можно не просчитывать
            // как точку преломления
            if (mode & ResArea)
            {
              for (const std::pair<uinttype, double> & pt : unvisPoints)
              {
                result.area.outer().push_back(
                  environment->getPoint(pt.first));
              }
            }

            if (mode & ResUnvisibles)
              result.unvisibleReachablePoints.insert(
                result.unvisibleReachablePoints.end(),
                unvisPoints.begin(), unvisPoints.end());
          }

          if (mode & ResArea)
            result.area.outer().push_back(intersectPoint);
        }
      }
      else
      {
        if (mode & ResArea)
          result.area.outer().push_back(*currentPoint);

        if ((mode & ResAbsorptions) && currentPoint->envId())
          result.absorptionPoints.push_back(currentPoint->envId());
      }

      continue;
    }

    // первая точка активной границы - не продолжения предыдущей
    if (currentPoint->firstForId && sweepDiagram[next] &&
      polarEdges[sweepDiagram[next] - 1].first == currentPoint->id())
    {
      if ((mode & ResRefractions) && currentPoint->envId())
        result.refractionPoints.push_back(currentPoint->envId());

      // новая активная граница перекрывает старую
      if (sweepDiagram[i]) 
      {
        uinttype activeEdgeId = sweepDiagram[i];

        const PolarPoint & p1 =
          polarPoints[polarEdges[activeEdgeId - 1].first - 1];
        const PolarPoint & p2 =
          polarPoints[polarEdges[activeEdgeId - 1].second - 1];

        Point2D intersectPoint =
          rayIntersectEdge(p1, p2, *center, currentPoint->azimuth());

        if (mode & ResArea)
          result.area.outer().push_back(intersectPoint);

        std::vector<std::pair<uinttype, double> > unvisPoints;

        if ((mode & AddUnvisible) &&
          addUnvisibleArea(sub, *center, maxPath, p2,
            *currentPoint, intersectPoint, false, unvisPoints))
        {
          if (currentPoint->envId())
          {
            if (mode & ResRefractions)
              result.refractionPoints.pop_back();

            if (mode & ResAbsorptions)
              result.absorptionPoints.push_back(currentPoint->envId());
          }
          if (mode & ResArea)
          {
            for (const std::pair<uinttype, double> & pt :
              unvisPoints)
            {
              result.area.outer().push_back(
                environment->getPoint(pt.first));
            }
          }

          if (mode & ResUnvisibles)
            result.unvisibleReachablePoints.insert(
              result.unvisibleReachablePoints.end(),
              unvisPoints.begin(), unvisPoints.end());
        }
      }
      else 
      {
        // новая активная граница НЕ перекрывает старую

        Arc arc(*center, centerEnv->id(), maxPath,
          Angle(polarPointsSort[prev]->azimuth()),
          Angle(currentPoint->azimuth()));

        if (mode & ResArea)
          arc.line(result.area.outer(), segmentsNumber);

        if (mode & ResArcs)
        {
          // подрезанная точка
          if (currentPoint->hiddenEnvId()) 
          {
            arc.setEndInfo(ByEdge, currentPoint->hiddenEnvId());
          }
          else
          {
            if (!currentPoint->envId())
              throw ReachableAreaFailure(VisibilityPointNullId);

            if (currentPoint->radius() < maxPath)
              arc.setEndInfo(ByRefPoint, currentPoint->envId());
            else
              arc.setEndInfo(ByEdge, currentPoint->envId());
          }

          if (useViewZone &&
            polarPointsSort[prev]->id() == viewZoneBeginPointId)
          {
            arc.setBeginInfo(ByViewZone);
          }
          else if (polarPointsSort[prev]->hiddenEnvId() ||
            polarPointsSort[prev]->radius() >= maxPath)
          {
            if (!polarPointsSort[prev]->lastForId)
              throw ReachableAreaFailure(VisibilityPreviousPointNullLastForId);

            const GeometrySubEnvironment::Edge & edge =
              polarEdges[polarPointsSort[prev]->lastForId - 1];

            if (!edge.first)
              throw ReachableAreaFailure( 
                    VisibilityFirstPointForEdgeWhichLastPointIsPreviousNullId);

            const PolarPoint & edgeFirstPoint =
              polarPoints[edge.first - 1];

            if (edgeFirstPoint.envId())
              arc.setBeginInfo(ByEdge, edgeFirstPoint.envId());
            else
            {
              if (!edgeFirstPoint.hiddenEnvId())
                throw ReachableAreaFailure(
             VisibilityPreviousFirstPointForEdgeWhichLastPointIsPreviousNullId);

              arc.setBeginInfo(ByEdge,
                edgeFirstPoint.hiddenEnvId());
            }
          }
          else
          {
            if (!polarPointsSort[prev]->envId())
              throw ReachableAreaFailure(VisibilityPreviousPointNullId);

            arc.setBeginInfo(ByRefPoint,
              polarPointsSort[prev]->envId());
          }
          result.arcs.push_back(arc);
        }
      }

      if (mode & ResArea)
      {
        result.area.outer().push_back(*currentPoint);
      }
    }

    if (useViewZone && sweepRayInViewZone 
      && currentPoint->id() == viewZoneEndPointId)
    {
      sweepRayInViewZone = false;

      Arc arc(*center, centerEnv->id(), maxPath,
        Angle(polarPointsSort[prev]->azimuth()),
        Angle(currentPoint->azimuth()));

      if (mode & ResArea)
      {
        if (sweepDiagram[i])
        {
          const PolarPoint & p1 =
            polarPoints[polarEdges[sweepDiagram[i] - 1].first - 1];
          const PolarPoint & p2 =
            polarPoints[polarEdges[sweepDiagram[i] - 1].second - 1];
          result.area.outer().push_back(
            rayIntersectEdge(p1, p2, *center, viewZoneEnd));
        }
        else
          arc.line(result.area.outer(), segmentsNumber);

        result.area.outer().push_back(*center);
      }

      if ((mode & ResArcs) && !sweepDiagram[i])
      {
        if (polarPointsSort[prev]->hiddenEnvId() ||
          polarPointsSort[prev]->radius() >= maxPath)
        {
          if (!polarPointsSort[prev]->lastForId)
            throw ReachableAreaFailure(VisibilityPreviousPointNullLastForId);

          const GeometrySubEnvironment::Edge & edge =
            polarEdges[polarPointsSort[prev]->lastForId - 1];

          if (!edge.first)
            throw ReachableAreaFailure(
                    VisibilityFirstPointForEdgeWhichLastPointIsPreviousNullId);

          const PolarPoint & edgeFirstPoint = polarPoints[edge.first - 1];

          if (edgeFirstPoint.envId())
            arc.setBeginInfo(ByEdge, edgeFirstPoint.envId());
          else
          {
            if (!edgeFirstPoint.hiddenEnvId())
              throw ReachableAreaFailure(
            VisibilityPreviousFirstPointForEdgeWhichLastPointIsPreviousNullId);

            arc.setBeginInfo(ByEdge,
              edgeFirstPoint.hiddenEnvId());
          }
        }
        else if (polarPointsSort[prev]->id() == viewZoneBeginPointId)
        {
          arc.setBeginInfo(ByViewZone);
        }
        else
        {
          if (!polarPointsSort[prev]->envId())
            throw ReachableAreaFailure(VisibilityPreviousPointNullId);

          arc.setBeginInfo(ByRefPoint, polarPointsSort[prev]->envId());
        }

        arc.setEndInfo(ByViewZone);
        result.arcs.push_back(arc);
      }
    }
  }

  if (mode & ResArea)
    AbstractOperations::deleteExcessVertices(result.area.outer(), true);
}
//==============================================================================
void Visibility::createCalculationEnvironment(
  const GeometrySubEnvironment* sub,
  const Point2D* center,
  double radius,
  Angle viewZoneBegin,
  double viewZoneSize,
  std::vector<PolarPoint>& polarPoints,
  std::vector<GeometrySubEnvironment::Edge> &polarEdges,
  uinttype& viewZoneBeginPointId,
  uinttype& viewZoneEndPointId)
{
  polarPoints.clear();
  polarEdges.clear();
  polarPoints.reserve(sub->edges().size());
  polarEdges.reserve(sub->edges().size());

  // начало заведомо невидимой зоны
  Angle unvisZoneStart;
  // конец заведомо невидимой зоны
  Angle unvisZoneEnd;

  // Заведомо невидимая зона определяется двумя границами, которым 
  // принадлежит центральная точка
  // Размер (в радианах) заведомо невидимой зоны
  double unvisZoneSize(0.); 
  bool useViewZone = viewZoneSize < 2 * M_PI;

  const EnvironmentPoint* centerEnv =
    dynamic_cast<const EnvironmentPoint*>(center);

  // если центральная точка - одна из точек среды
  if (centerEnv->id()) 
  {
    // определение заведомо невидимой зоны
    if (!centerEnv->polygonId())
      throw ReachableAreaFailure(VisibilityPolygonContainingCentralPointNullId);
    if (!centerEnv->id())
      throw ReachableAreaFailure(VisibilityCentralPointNullId);

    const EnvironmentPolygon & centerPointPolygon =
      sub->parentEnvironment()->getPolygon(centerEnv->polygonId());

    uinttype nextPointId =
      (centerEnv->id() < centerPointPolygon.firstPointId() +
        centerPointPolygon.pointsCount() - 1) ?
      (centerEnv->id() + 1) : centerPointPolygon.firstPointId();

    uinttype prevPointId =
      (centerEnv->id() > centerPointPolygon.firstPointId()) ?
      (centerEnv->id() - 1) : (centerPointPolygon.firstPointId() +
        centerPointPolygon.pointsCount() - 1);

    unvisZoneStart = Geometry::lineAngularity(
      *centerEnv, sub->parentEnvironment()->getPoint(prevPointId)).get();

    unvisZoneEnd = Geometry::lineAngularity(
      *centerEnv, sub->parentEnvironment()->getPoint(nextPointId)).get();

    unvisZoneSize = unvisZoneStart.deltaAngleCounterclockwise(unvisZoneEnd);
  }

  uinttype polarEdgeId = 1;
  uinttype polarPointId = 1;
  uinttype currentPolygonId = 0;
  // локальный id (id в полярной СК) первой точки текущего полигона
  uinttype currentPolygonFirstLocalId = 0; 

  // перебор границ для отбора
  for (const GeometrySubEnvironment::Edge& edge : sub->edges())
  {
    const EnvironmentPoint& p1e =
      sub->parentEnvironment()->getPoint(edge.first);
    const EnvironmentPoint& p2e =
      sub->parentEnvironment()->getPoint(edge.second);

    if (!(p1e.id() && p2e.id()))
      throw ReachableAreaFailure(VisibilityPointNullId);

    if (!(p1e.polygonId() && p1e.polygonId() == p2e.polygonId()))
      throw ReachableAreaFailure(
                      VisibilityPolygonsContainingPointsFromOneEdgeIdsMismatch);

    PolarPoint p1;
    PolarPoint p2;

    // p1 уже отобрана и перенесена в полярную СК
    bool p1exist;
    // p2 уже отобрана и перенесена в полярную СК
    bool p2exist; 

    if ((p1exist = (p1e.polygonId() == currentPolygonId &&
      !polarPoints.empty() && polarPoints.back().envId() == p1e.id())))
    {
      p1 = polarPoints.back();
    }
    else
    {
      p1 = PolarPoint(p1e, *center, p1e.id());

      if (p1e.polygonId() != currentPolygonId)
      {
        currentPolygonId = p1e.polygonId();
        currentPolygonFirstLocalId = 0;
      }
    }

    if ((p2exist = (currentPolygonFirstLocalId &&
      !polarPoints.empty() &&
      p2e.id() == polarPoints[currentPolygonFirstLocalId - 1].envId())))
    { 
      p2 = polarPoints[currentPolygonFirstLocalId - 1];
    }
    else
      p2 = PolarPoint(p2e, *center, p2e.id());

    // условие добавления границы
    // 1 - граница пересекается с кругом отсечения видимости
    // 2 - направлена правильно либо содержит центральную точку (у центральной
    // точки пока что азимут установлен некорректно (не определен)
    // 3 - не лежит полностью в заведомо невидимой зоне
    // 
    if (((p1.radius() <= radius && p2.radius() <= radius) ||
        edgeIntersectBorderCircle(p1, p2, *center, radius)) 
      &&
      ((p1.azimuth().deltaAngle(p2.azimuth()) > 0) ||
        (p1.radius() == 0 && p2.radius() != 0) ||
        (p1.radius() != 0 && p2.radius() == 0)) 
      &&
      ((!centerEnv->id() ||
          p1.radius() == 0 || p2.radius() == 0 ||
          unvisZoneStart.deltaAngleCounterclockwise(p1.azimuth())
          > unvisZoneSize ||
          unvisZoneStart.deltaAngleCounterclockwise(p2.azimuth())
          > unvisZoneSize)) 
      &&
      (!useViewZone ||
        (p1.radius() > 0 &&
          viewZoneBegin.deltaAngleCounterclockwise(p1.azimuth())
          < viewZoneSize) ||
        (p2.radius() > 0 &&
          viewZoneBegin.deltaAngleCounterclockwise(p2.azimuth())
          < viewZoneSize) ||
        (viewZoneBegin.deltaAngleCounterclockwise(p2.azimuth()) <
          viewZoneBegin.deltaAngleCounterclockwise(p1.azimuth()))))
    {
      // добавление точек и границы
      GeometrySubEnvironment::Edge polarEdge;

      if (p1exist)
      {
        polarPoints[p1.id() - 1].firstForId = polarEdgeId;
        polarEdge.first = p1.id();
      }
      else
      {
        p1.firstForId = polarEdgeId;
        p1.setId(polarPointId);
        polarEdge.first = polarPointId;

        if (!currentPolygonFirstLocalId)
          currentPolygonFirstLocalId = polarPointId;

        if (p1.envId() && p1.envId() == centerEnv->id())
          p1.setAzimuth(Angle(unvisZoneStart.value() +
            unvisZoneStart.deltaAngle(unvisZoneEnd) / 2.0));

        // установка корректного азимута для центральной точки
        // азимут устанавливается так, чтобы при заметании
        // (сортировке по азимуту в порядке возрастания)
        // точки шли в порядке "начало предыдущей границы - центр -
        // конец следующей границы"
        polarPoints.push_back(p1);
        ++polarPointId;
      }

      if (p2exist)
      {
        polarPoints[p2.id() - 1].lastForId = polarEdgeId;
        polarEdge.second = p2.id();
      }
      else
      {
        p2.lastForId = polarEdgeId;
        p2.setId(polarPointId);
        polarEdge.second = polarPointId;

        if (
          p2.envId() && p2.envId() == centerEnv->id())
        {
          p2.setAzimuth(Angle(unvisZoneStart.value() +
            unvisZoneStart.deltaAngle(unvisZoneEnd) / 2.0));
        }

        // см. для p1
        polarPoints.push_back(p2);
        ++polarPointId;
      }

      polarEdges.push_back(polarEdge);
      ++polarEdgeId;
    }
  }

  if (useViewZone)
  {
    PolarPoint viewZoneBeginPoint(0.0, viewZoneBegin.value(), *center, 0);
    viewZoneBeginPoint.setId(polarPointId);
    viewZoneBeginPointId = polarPointId++;

    PolarPoint viewZoneEndPoint(0.0, viewZoneBegin.value() + viewZoneSize,
      *center, 0);
    viewZoneEndPoint.setId(polarPointId);
    viewZoneEndPointId = polarPointId++;

    polarPoints.push_back(viewZoneBeginPoint);
    polarPoints.push_back(viewZoneEndPoint);
  }
}
//==============================================================================
void Visibility::createSweepDiagram(
  const std::vector<PolarPoint>& polarPoints,
  const std::vector<GeometrySubEnvironment::Edge>& polarEdges,
  std::vector<const PolarPoint*>& polarPointsSort,
  std::vector<uinttype>& sweepDiagram)
{
  polarPointsSort.clear();
  polarPointsSort.reserve(polarPoints.size());

  for (const PolarPoint & pt : polarPoints)
    polarPointsSort.push_back(&pt);

  std::sort(polarPointsSort.begin(), polarPointsSort.end(),
    [](const PolarPoint* p1, const PolarPoint* p2)
  {
    if (p1->azimuth().value() < p2->azimuth().value())
      return true;

    if (p1->azimuth().value() > p2->azimuth().value())
      return false;

    // далее случай равентва азимутов
    // при заметании следующая граница должна открыться до
    // закрытия предыдущей, иначе возникают прострелы
    if (p2->firstForId == 0 && p1->firstForId != 0)
      return true;
    if (p1->firstForId == 0 && p2->firstForId != 0)
      return false;

    // в остальных случаях сортировка по радиусу
    if (p1->radius() < p2->radius())
      return true;

    return false;
  });

  // sweepDiagram - диаграмма заметания
  // содержит id видимой границы для каждой области. Размер диаграммы
  // соответствует количеству точек. Границы областей определяются 
  // азимутами отсортированных точек, т.е., например, элемент 
  // диаграммы i показывает, что на интервале от polarPointsSort[i-1].azimuth 
  // до polarPointsSort[i].azimuth активна границы с id sweepDiagram[i].
  // для i = 0 начальное значение интервала 0, для последнего эл-та
  // конечное значение 2*pi. Значение 0 в диаграмме обозначает, что на
  // интервале видимых границ в радиусе видимости нет.

  sweepDiagram.clear();
  sweepDiagram.reserve(polarPoints.size());

  // открытые границы. Массив всегда отсортирован
  //  по дальности, последний элемент является видимым.
  std::vector<uinttype> currentEdges; 
  // функтор сравнения границ
  PolarEdgeFunctor edgeCmp(polarPoints, polarEdges); 
  // Первый проход по кругу. Границы, перескающие 0, игнорируются.
  for (const PolarPoint* point : polarPointsSort)  
  {
    uinttype activeEdgeId = (currentEdges.empty()) ? 0 : currentEdges.back();
    sweepDiagram.push_back(activeEdgeId);

    // одна граница закрылась, другая сразу открылась
    if (point->lastForId && point->firstForId) 
    {
      std::vector<uinttype>::iterator iter = std::find(
        currentEdges.begin(), currentEdges.end(), point->lastForId);

      if (iter != currentEdges.end())
      {
        // заменить id границы
        *iter = point->firstForId;
      }
      else
      {
        // если граница не была открыта (начало до нуля)
        currentEdges.insert(
          std::upper_bound(currentEdges.begin(),
          currentEdges.end(), point->firstForId, edgeCmp),
          point->firstForId);
      }
    }
    else
    {
      // граница закрывается
      if (point->lastForId) 
      {
        std::vector<uinttype>::iterator iter = std::find(
          currentEdges.begin(), currentEdges.end(), point->lastForId);

        if (iter != currentEdges.end())
        {
          // изъять из открытых
          currentEdges.erase(iter);
        }
      }
      else if (point->firstForId) 
      {
        // если граница открывается, вставить в список открытых в 
        // нужное место
        currentEdges.insert(
          std::upper_bound(currentEdges.begin(),
          currentEdges.end(), point->firstForId, edgeCmp),
          point->firstForId);
      }
    }
  }

  // Закрытие границ. На данном этапе все границы открыты, при этом
  // границы, пересекающие 0, требуется закрыть, внеся соответсвующие
  // изменения в диаграмму заметания, т.к. данные границы не были
  // учтены при первом проходе по кругу
  for (uinttype i = 0; !currentEdges.empty(); i++) 
  {
    if (edgeCmp(sweepDiagram[i], currentEdges.back()))
      sweepDiagram[i] = currentEdges.back();

    const PolarPoint* point = polarPointsSort[i];

    // граница закрывается
    if (point->lastForId) 
    {
      std::vector<uinttype>::iterator iter = std::find(
        currentEdges.begin(), currentEdges.end(), point->lastForId);

      if (iter != currentEdges.end())
        currentEdges.erase(iter);
    }
  }
}
//==============================================================================
bool Visibility::edgeIntersectBorderCircle(
  PolarPoint& p1, PolarPoint& p2,
  const Point2D& center, double radius)
{
  double x = p1.getX() - center.getX();
  double y = p1.getY() - center.getY();

  // направление от p1 на p2
  Angle alpha(Geometry::lineAngularity(p1, p2).get());
  double sinAlpha = std::sin(alpha.value());
  double cosAlpha = std::cos(alpha.value());

  double b = 2 * (y * sinAlpha + x * cosAlpha);
  double c = x * x + y * y - radius * radius;

  double D = b * b - 4 * c;

  // прямая не пересекает окружность
  if (D <= 0) 
    return false;

  double sqrtD = std::sqrt(D);

  // расстояние от p1 до первого пересечения
  // прямой с окружностью в направлении на p2
  double l1 = (-b - sqrtD) / 2.0; 
  // расстояние от p1 до второго пересечения
  // прямой с окружностью в направлении на p2
  double l2 = (-b + sqrtD) / 2.0; 

  if ((l2 <= 0) || (l1 > Geometry::distance(p1, p2)))
    return false;

  if (p2.radius() > radius)
  {
    Point2D intersect2(p1.getX() + l2 * cosAlpha, p1.getY() + l2 * sinAlpha);
    uinttype envId = p2.envId();

    p2 = PolarPoint(intersect2, center, 0);
    p2.setHiddenEnvId(envId);
  }

  if (p1.radius() > radius)
  {
    Point2D intersect1(p1.getX() + l1 * cosAlpha, p1.getY() + l1 * sinAlpha);
    uinttype envId = p1.envId();

    p1 = PolarPoint(intersect1, center, 0);
    p1.setHiddenEnvId(envId);
  }

  return true;
}
//==============================================================================
Point2D Visibility::rayIntersectEdge(
  const PolarPoint& p1, const PolarPoint& p2,
  const Point2D& center, const Angle& azimuth)
{
  if ((p1.azimuth().deltaAngle(azimuth) < 0.)
      || (azimuth.deltaAngle(p2.azimuth()) < 0.)
      || (p1.azimuth().deltaAngle(azimuth)
            + azimuth.deltaAngle(p2.azimuth())
            - p1.azimuth().deltaAngle(p2.azimuth())
            >= 0.0000001))
    throw ReachableAreaFailure(VisibilityRayIntersectWronglyDirectedEdge);

  // граничные случаи
  if (p1.azimuth().value() == azimuth.value())
    return p1;
  if (p2.azimuth().value() == azimuth.value())
    return p2;

  // Ищем в полярной СК. Азимут известен, нужно найти только радиус
  // Формула выводится через приравнивание площади треугольника,
  // образованного отрезками от центра до p1 и p2 и границей, к сумме
  // площадей двух треугольников, границой которых выступает
  // отрезок луча. Площади считать как половина произведения
  //  сторон на синус угла между ними

  double r = (p1.radius() * p2.radius() * std::sin(p2.azimuth().value() -
    p1.azimuth().value())) / (p1.radius() 
      * std::sin(azimuth.value() - p1.azimuth().value()) +
      p2.radius() * std::sin(p2.azimuth().value() - azimuth.value()));

  // Далее перевод из полярной СК со смещенным центром в 
  // глобальные координаты
  return Point2D(
    center.getX() + r * std::cos(azimuth.value()),
    center.getY() + r * std::sin(azimuth.value()));
}
//==============================================================================
bool Visibility::addUnvisibleArea(
  const GeometrySubEnvironment* sub,
  const Point2D& center,
  double radius,
  const PolarPoint& p1,
  const PolarPoint& p2,
  const Point2D& intersect,
  bool p1IsRef,
  std::vector<std::pair<uinttype, double>>& unvisibleReachablePoints)
{
  unvisibleReachablePoints.clear();

  // отрезки были "обрезаны" - "залива" нет
  if (!p1.envId() || !p2.envId()) 
    return false;

  const GeometryEnvironment* environment = sub->parentEnvironment();
  const EnvironmentPoint& p1env = environment->getPoint(p1.envId());
  const EnvironmentPoint& p2env = environment->getPoint(p2.envId());

  // луч пересекает полигон, которому не принадлежит точка
  // преломления. "Залива" нет как такового.
  if (!p1env.polygonId() || p1env.polygonId() != p2env.polygonId())
    return false; 

  const EnvironmentPolygon& polygon =
    environment->getPolygon(p1env.polygonId());

  double unvisBorderLength = polygon.distOnBorder(p1env, p2env);
  // длина берега "залива"
  double intersectR = Geometry::distance(center, intersect);
  // расстояние от точки пересечения до ближайшей невидимой точки
  double intersectDelta = Geometry::distance(intersect, p1IsRef ? p2 : p1); 
  // полная длина берега "залива"
  unvisBorderLength += intersectDelta; 

  // максимальная длина берега, при которой все невидимые точки 
  // заведомо достижимы. Путь либо через точку преломления, либо
  //  через точку пересечения
  double maxUnvisBorderLength = 2 * radius - intersectR -
    (p1IsRef ? p1 : p2).radius();

  if (unvisBorderLength > maxUnvisBorderLength)
    return false;

  // точки, определяющи полигон залива
  std::vector<Point2D> unvisiblePoints; 

  uinttype startId = p1IsRef ? polygon.nextPointId(p1env.id()) : p1env.id();
  uinttype endId = p1IsRef ? p2env.id() : polygon.prevPointId(p2env.id());

  Box2D unvisAreaBox ( intersect, intersect );

  if (p1IsRef)
    unvisiblePoints.push_back(p1);
  else
    unvisiblePoints.push_back(intersect);

  for (uinttype id = startId;; id = polygon.nextPointId(id))
  {
    EnvironmentPoint pt = environment->getPoint(id);

    unvisiblePoints.push_back(pt);
    unvisAreaBox.expandToPoint( pt );

    // расстояние через точку преломления
    double dist1 = p1IsRef ? p1.radius() + polygon.distOnBorder(p1env, pt) :
      p2.radius() + polygon.distOnBorder(pt, p2env);
    
    // расстояние через точку пересечения
    double dist2 = intersectR + intersectDelta +
      (p1IsRef ? polygon.distOnBorder(pt, p2env) :
        polygon.distOnBorder(p1env, pt));
    
    // минимальное расстояние
    double dist = (dist1 < dist2) ? dist1 : dist2; 

    unvisibleReachablePoints.push_back(std::make_pair(id, dist));

    if (id == endId)
      break;
  }

  if (p1IsRef)
    unvisiblePoints.push_back(intersect);
  else
    unvisiblePoints.push_back(p2);

  Polygon2D unvisiblePointsPoly;

  for (auto point : unvisiblePoints)
  {
    unvisiblePointsPoly.outer().push_back(point);
  }

  AbstractOperations::correct(unvisiblePointsPoly);

  if ( AbstractOperations::within(center, unvisiblePointsPoly))
  {
    unvisibleReachablePoints.clear();
    return false;
  }

  // проверка полигонов суб-среды на отсутствие внутри залива.
  // Достижимость границы залива не гарантирует достижимости всех 
  // точек внутри при наличии островов.
  // Пример: в заливе остров в форме лабиринта
  for (uinttype polygonId : sub->polygonsIds())
  {
    if (polygonId == polygon.id())
      continue;

    const EnvironmentPoint& pt =
      environment->getPolygon(polygonId).firstPoint();

    // проверка коробки
    if (!AbstractOperations::within(static_cast<Point2D>(pt), unvisAreaBox))
      continue;

    // Одна точка определяет весь полигон
    if ( AbstractOperations::within(
      Point2D(pt.getX(), pt.getY()), unvisiblePointsPoly))
    {
      unvisibleReachablePoints.clear();
      return false;
    }
  }

  return true;
}
//==============================================================================
} //! ReachableAreaCalculator
