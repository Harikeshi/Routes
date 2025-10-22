#include "PathFinder/Knot.hpp"
//! PathFinder
#include "PathFinder/PathHandler.hpp"
#include "PathFinder/VisiblePath.hpp"
//==============================================================================
namespace TriangularGeometry
{
//==============================================================================
  Knot::Knot(std::shared_ptr<Top> top)
    : Link()
    , mTop(top)
    , mDistance(0.)
    , mIsMileStone(false)
    , mShadow(nullptr)
    , mShadowDirect(Geometry::Between)
  { }
//==============================================================================
  Knot::Knot(
    std::shared_ptr<Top> mileStone, Geometry::PlanePosition shadowDirect)
    : Link()
    , mTop(mileStone)
    , mDistance(mileStone->info->distance)
    , mIsMileStone(true)
    , mShadow(mileStone->info->prevTop)
    , mShadowDirect(shadowDirect)
  { }
//==============================================================================
  void Knot::zoneObservation(
    std::shared_ptr<Top> finishTop,
    std::shared_ptr<Triangle> startTriangle,
    std::shared_ptr<Triangle> finishTriangle,
    std::shared_ptr<PathHandler> pathHandler)
  {
    if (!mIsMileStone)
    {
      // Если начинаем со стартовой вершины, проверим, где она
      // находится относительно начальной грани маршрута
      auto knotPosition = startTriangle->positionInTriangle(mTop);

      if (knotPosition == AtVertex)
      {
        // Если попали на вершину, ищем маршрут через все возможные
        // грани
        zoneObservationForTriangleVertex(
          finishTop, finishTriangle, pathHandler);
      }
      else if (knotPosition == AtEdge)
      {
        // Если попали на ребро, найдём, на какое именно
        std::shared_ptr<Edge> curEdge(nullptr);
        if (startTriangle->p->atEdge(mTop))
          curEdge = startTriangle->p;
        else if (startTriangle->q->atEdge(mTop))
          curEdge = startTriangle->q;
        else
          curEdge = startTriangle->r;

        // Нужно проверить две грани, смежных с данным ребром
        zoneObservationForTopAtEdge(
          finishTop, finishTriangle, curEdge, pathHandler);
      }
      else
      {
        // Если попали внутрь грани, можем искать маршрут через три
        // выходных ребра грани
        zoneObservationForInnerTop(
          finishTop, startTriangle, finishTriangle, pathHandler);
      }
    }
    else
    {
      // Если начинаем не со стартовой вершины, ищем маршрут через
      // все возможные грани
      zoneObservationForTriangleVertex(
        finishTop, finishTriangle, pathHandler);
    }
  }
//==============================================================================
  bool Knot::hasEdgeInShadow(std::shared_ptr<Edge> edge) const
  {
    if (!mIsMileStone)
      return false;

    Geometry::BorderedLine shadowLine(*mShadow, *mTop);

    return ((mShadowDirect == shadowLine.position(*edge->getI()))
      && (mShadowDirect == shadowLine.position(*edge->getJ())));
  }
//==============================================================================
  void Knot::zoneObservationForTriangleVertex(
    std::shared_ptr<Top> finishTop,
    std::shared_ptr<Triangle> finishTriangle,
    std::shared_ptr<PathHandler> pathHandler)
  {
    std::shared_ptr<VisiblePath> visiblePath(nullptr);
    std::shared_ptr<Knot> thisKnot(nullptr);

    // Ребро, через которое ищем путь
    std::shared_ptr<Edge> exitEdge(nullptr);

    // Выбираем произвольное ребро, содержащее текущую вершину
    std::shared_ptr<Edge> incidentEdge = mTop->edge;

    // В первую очередь хотим проверять грани, лежащие слева от
    // выбранного ребра
    Geometry::PlanePosition direction = Geometry::Left;

    // Пока хоть какие-то грани, содержащие текущую вершину
    // и ещё не исследованные, есть, продолжаем
    while (true)
    {
      // Ищем ребро грани, расположенное напротив текущей вершины
      // (через это ребро будем дальше искать маршрут)
      exitEdge = getExitEdge(incidentEdge, direction);

      if (exitEdge == nullptr)
        break;

      thisKnot = std::dynamic_pointer_cast<Knot>(shared_from_this());

      // Определяем смежную с выходным ребром грань, в которой
      // находится вершина
      if ((exitEdge->left) &&
        (exitEdge->left->isTopInTriangle(mTop)))
      {
        visiblePath = std::make_shared<VisiblePath>(
          thisKnot, exitEdge->left, exitEdge, finishTriangle);
      }
      else
      {
        visiblePath = std::make_shared<VisiblePath>(
          thisKnot, exitEdge->right, exitEdge, finishTriangle);
      }

      // Для каждой найденной грани выполняем поиск пути до
      // конечной вершины
      visiblePath->findLastVisibleKnot(finishTop, pathHandler);
      visiblePath.reset();
    }
  }
//==============================================================================
  void Knot::zoneObservationForTopAtEdge(
    std::shared_ptr<Top> finishTop,
    std::shared_ptr<Triangle> finishTriangle,
    std::shared_ptr<Edge> exitEdge,
    std::shared_ptr<PathHandler> pathHandler)
  {
    std::shared_ptr<VisiblePath> visiblePath(nullptr);
    std::shared_ptr<Knot> thisKnot(nullptr);
    std::shared_ptr<Triangle> localStartTriangle(nullptr);

    for (int triangleIndex = 1; triangleIndex <= 2; triangleIndex++)
    {
      switch (triangleIndex)
      {
      case 1:
        localStartTriangle = exitEdge->left;
        break;
      case 2:
        localStartTriangle = exitEdge->right;
        break;
      default:
        break;
      }

      if (!(localStartTriangle == nullptr))
      {
        thisKnot = std::dynamic_pointer_cast<Knot>(shared_from_this());
        visiblePath = std::make_shared<VisiblePath>(
          thisKnot, localStartTriangle, exitEdge, finishTriangle);
        visiblePath->findLastVisibleKnot(finishTop, pathHandler);
        visiblePath.reset();
      }
    }
  }
//==============================================================================
  void Knot::zoneObservationForInnerTop(
    std::shared_ptr<Top> finishTop,
    std::shared_ptr<Triangle> startTriangle,
    std::shared_ptr<Triangle> finishTriangle,
    std::shared_ptr<PathHandler> pathHandler)
  {
    std::shared_ptr<VisiblePath> visiblePath(nullptr);
    std::shared_ptr<Knot> thisKnot(nullptr);

    // Ребро, через которое ищем путь
    std::shared_ptr<Edge> exitEdge(nullptr);

    for (int edgeIndex = 1; edgeIndex <= 3; edgeIndex++)
    {
      // Цикл по трём выходным ребрам
      switch (edgeIndex)
      {
      case 1:
        exitEdge = startTriangle->p;
        break;
      case 2:
        exitEdge = startTriangle->q;
        break;
      case 3:
        exitEdge = startTriangle->r;
        break;
      default:
        break;
      }

      // Если текущее ребро не является граничным, ищем через него
      // путь к конечной вершине
      if (!exitEdge->isBound())
      {
        thisKnot = std::dynamic_pointer_cast<Knot>(shared_from_this());
        visiblePath = std::make_shared<VisiblePath>(
          thisKnot, startTriangle, exitEdge, finishTriangle);
        visiblePath->findLastVisibleKnot(finishTop, pathHandler);
        visiblePath.reset();
      }
    }
  }
//==============================================================================
  std::shared_ptr<Edge> Knot::getExitEdge(
    std::shared_ptr<Edge>& curEdge,
    Geometry::PlanePosition& direction)
  {
    std::shared_ptr<Edge> result(nullptr);

    // Указатель на грань, прилегающую с заданной стороны от
    // указанного ребра
    std::shared_ptr<Triangle> adjacentTriangle(nullptr);
    // Указатель на ребро, следующее в указанном направлении поиска
    std::shared_ptr<Edge> nextEdge(nullptr);

    // Ищем прилегающую с выбранной стороны к ребру грань
    Edge::scanBunch(curEdge, mTop, direction, adjacentTriangle, nextEdge);

    // Если искали слева и ничего не нашли, пробуем искать справа
    // (всегда сначала ищем слева)
    if ((adjacentTriangle == nullptr) && (direction == Geometry::Left))
    {
      direction = Geometry::Right;
      curEdge = mTop->edge;
      Edge::scanBunch(curEdge, mTop, direction, adjacentTriangle, nextEdge);
    }

    if (adjacentTriangle)
      result 
      = adjacentTriangle->edgeOppositeTop(mTop); // Текущее выходное ребро

    curEdge = nextEdge;
    return result;
  }
//==============================================================================
} // !TriangularGeometry
//==============================================================================
