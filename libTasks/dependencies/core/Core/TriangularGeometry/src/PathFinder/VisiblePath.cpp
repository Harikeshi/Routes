#include "PathFinder/VisiblePath.hpp"
//! Triangulation
#include "Triangulation/Objects/Edge.hpp"
//! PathFinder
#include "PathFinder/PathHandler.hpp"
//==============================================================================
namespace TriangularGeometry
{
//==============================================================================
  VisiblePath::VisiblePath(
    std::shared_ptr<Knot> localStartKnot,
    std::shared_ptr<Triangle> localTriangle,
    std::shared_ptr<Edge> exitEdge,
    std::shared_ptr<Triangle> finishTriangle)
    : Link()
    , mLocalStartKnot(localStartKnot)
    , mLocalStartTop(localStartKnot->getTop())
    , mLocalTriangle(localTriangle)
    , mCurExitEdge(exitEdge)
    , mLeftTop(nullptr)
    , mRightTop(nullptr)
    , mFinishTriangle(finishTriangle)
  {
    if (Edge::isCounterclockwise(mCurExitEdge, mLocalTriangle))
    {
      mLeftTop = mCurExitEdge->getJ();
      mRightTop = mCurExitEdge->getI();
    }
    else
    {
      mLeftTop = mCurExitEdge->getI();
      mRightTop = mCurExitEdge->getJ();
    }

    Top::updatePathInfo(mLocalStartTop, mLeftTop);
    Top::updatePathInfo(mLocalStartTop, mRightTop);
  }
//==============================================================================
  VisiblePath::VisiblePath(std::shared_ptr<VisiblePath> visiblePath)
    : Link()
    , mLocalTriangle(visiblePath->mLocalTriangle)
    , mLocalStartKnot(visiblePath->mLocalStartKnot)
    , mCurExitEdge(visiblePath->mCurExitEdge)
    , mLocalStartTop(visiblePath->mLocalStartTop)
    , mLeftTop(visiblePath->mLeftTop)
    , mRightTop(visiblePath->mRightTop)
    , mFinishTriangle(visiblePath->mFinishTriangle)
  {}
//==============================================================================
  void VisiblePath::findLastVisibleKnot(
    std::shared_ptr<Top> finishTop,
    std::shared_ptr<PathHandler> pathHandler)
  {
    bool hasCurEdge(true);
    std::shared_ptr<Triangle> nextTriangle(nullptr);
    std::shared_ptr<Top> nextTop(nullptr);
    std::shared_ptr<Top> leftTop(nullptr);
    std::shared_ptr<Top> rightTop(nullptr);
    Geometry::PlanePosition planePosition;
    std::shared_ptr<Edge> leftEdge(nullptr);
    std::shared_ptr<Edge> rightEdge(nullptr);
    std::shared_ptr<Knot> mileStone(nullptr);

    while (hasCurEdge)
    {
      if ((mLocalTriangle == mFinishTriangle)
        && (locationInVisibleZone(finishTop) == Geometry::Between))
      {
        Top::updatePathInfo(mLocalStartTop, finishTop);
        break;
      }

      if (mCurExitEdge->isBound()
       || mLocalStartKnot->hasEdgeInShadow(mCurExitEdge))
      {
        break;
      }

      if (mCurExitEdge->left == mLocalTriangle)
        nextTriangle = mCurExitEdge->right;
      else
        nextTriangle = mCurExitEdge->left;

      if ((nextTriangle == mFinishTriangle)
        && (locationInVisibleZone(finishTop) == Geometry::Between))
      {
        Top::updatePathInfo(mLocalStartTop, finishTop);
        break;
      }

      // Вершина за текущим ребром
      nextTop = nextTriangle->topOppositeEdge(mCurExitEdge);
      planePosition = locationInVisibleZone(nextTop);

      // Левая и правая вершины ребра
      if (Edge::isCounterclockwise(mCurExitEdge, mLocalTriangle))
      {
        leftTop = mCurExitEdge->getJ();
        rightTop = mCurExitEdge->getI();
      }
      else
      {
        leftTop = mCurExitEdge->getI();
        rightTop = mCurExitEdge->getJ();
      }

      // Левое ребро треугольника
      leftEdge = nextTriangle->edgeOppositeTop(rightTop);
      // Правое ребро треугольника
      rightEdge = nextTriangle->edgeOppositeTop(leftTop);

      mCurExitEdge = nullptr;

      // Поиск вех и определение направлений прослеживания
      if (planePosition == Geometry::Right)
      {
        // Вершина за пределами зоны обзора
        // Образуется правая веха
        if (Top::updatePathInfo(mLocalStartTop, mRightTop))
        {
          // Добавляем последнюю видимую вершину в обработчик
          // маршрута для дальнейшего поиска пути от неё
          mileStone = std::make_shared<Knot>(mRightTop, Geometry::Left);
          pathHandler->setKnot(mileStone);
        }

        // Справа выход из зоны обзора, если можем пойти влево,
        // продолжаем поиск маршрута
        if (!leftEdge->isBound())
          mCurExitEdge = leftEdge;
      }
      else if (planePosition == Geometry::Left)
      {
        // Вершина за пределами зоны обзора
        // Образуется левая веха
        if (Top::updatePathInfo(mLocalStartTop, mLeftTop))
        {
          // Добавляем последнюю видимую вершину в обработчик
          // маршрута для дальнейшего поиска пути от неё
          mileStone = std::make_shared<Knot>(mLeftTop, Geometry::Right);
          pathHandler->setKnot(mileStone);
        }

        // Слева выход из зоны обзора, если можем пойти вправо,
        // продолжаем поиск маршрута
        if (!rightEdge->isBound())
          mCurExitEdge = rightEdge;
      }
      else if (planePosition == Geometry::Between)
      {
        // Вершина внутри зоны обзора
        if (!(leftEdge->isBound() || rightEdge->isBound()))
        {
          // Разветвление пути, можем пойти и влево, и вправо
          std::shared_ptr<VisiblePath> thisVisiblePath =
            std::dynamic_pointer_cast<VisiblePath>(shared_from_this());
          std::shared_ptr<VisiblePath> rightVisiblePath =
            std::make_shared<VisiblePath>(thisVisiblePath);

          // Продолжаем идти по левой ветви
          mCurExitEdge = leftEdge;
          mRightTop = nextTop;

          // Отдельно вызываем построение для правой ветви
          rightVisiblePath->mLocalTriangle = nextTriangle;
          rightVisiblePath->mCurExitEdge = rightEdge;
          rightVisiblePath->mLeftTop = nextTop;
          rightVisiblePath->findLastVisibleKnot(finishTop, pathHandler);
          rightVisiblePath.reset();
        }
        else if (!leftEdge->isBound())
        {
          // Уход по левой ветви
          mCurExitEdge = leftEdge;
          mRightTop = nextTop;
        }
        else if (!rightEdge->isBound())
        {
          // Уход по правой ветви
          mCurExitEdge = rightEdge;
          mLeftTop = nextTop;
        }
        else
          break; // Тупик, конец прослеживания направления
      }

      mLocalTriangle = nextTriangle;
      hasCurEdge = (mCurExitEdge != nullptr);
    }
  }
//==============================================================================
  Geometry::PlanePosition VisiblePath::locationInVisibleZone(
    std::shared_ptr<Top> top)
  {
    Geometry::BorderedLine leftLine(*mLocalStartTop, *mLeftTop);
    Geometry::BorderedLine rightLine(*mLocalStartTop, *mRightTop);

    if ((mLeftTop == nullptr) && (mRightTop == nullptr))
      return Geometry::Between;
    else if (leftLine.position(*top) == Geometry::Left)
      return Geometry::Left;
    else if (rightLine.position(*top) == Geometry::Right)
      return Geometry::Right;
    return Geometry::Between;
  }
//==============================================================================
} // !TriangularGeometry
//==============================================================================
