#include "PathFinder/PathHandler.hpp"
//! Triangulation
#include "Triangulation/Objects/Structure/Head.hpp"
//! PathFinder
#include "PathFinder/Knot.hpp"
//==============================================================================
namespace TriangularGeometry
{
//==============================================================================
  PathHandler::PathHandler()
    : mKnotsList(nullptr)
    , mCurKnot(nullptr)
  {
    mKnotsList = Head::create();
  }
//==============================================================================
  PathHandler::~PathHandler()
  {
    std::shared_ptr<Knot> knot(nullptr);

    while (!mKnotsList->isEmpty())
    {
      knot = std::dynamic_pointer_cast<Knot>(mKnotsList->first());
      knot->out();
      knot.reset();
    }

    mKnotsList->unlock();
  }
//==============================================================================
  void PathHandler::scanPathGraph(
    std::shared_ptr<Top> startTop,
    std::shared_ptr<Top> finishTop,
    std::shared_ptr<Triangle> startTriangle,
    std::shared_ptr<Triangle> finishTriangle)
  {
    // Порождение начального узла точкой в треугольнике 
    std::shared_ptr<Knot> knot = std::make_shared<Knot>(startTop);
    knot->into(mKnotsList);

    // Длина кратчайшего найденного маршрута 
    double minPathDistance = std::numeric_limits<double>::max();

    // Указатель на обработчик маршрута
    std::shared_ptr<PathHandler> pathHandler
      = std::dynamic_pointer_cast<PathHandler>(shared_from_this());

    while (!mKnotsList->isEmpty())
    {
      mCurKnot = std::dynamic_pointer_cast<Knot>(mKnotsList->first());

      // Если расстояние до текущего узла больше расстояния до
      // конечной точки, текущий узел не даст кратчайший маршрут
      if (mCurKnot->getDistance() > minPathDistance)
        break;

      mCurKnot->zoneObservation(
        finishTop, startTriangle, finishTriangle, pathHandler);
      mCurKnot->out();
      mCurKnot.reset();
      minPathDistance = finishTop->info->distance;
    }

    while (!mKnotsList->isEmpty())
    {
      mCurKnot = std::dynamic_pointer_cast<Knot>(mKnotsList->first());
      mCurKnot->out();
      mCurKnot.reset();
    }
  }
//==============================================================================
  void PathHandler::setKnot(std::shared_ptr<Knot> knot)
  {
    knot->into(mKnotsList);
    std::shared_ptr<Knot> previousKnot
      = std::dynamic_pointer_cast<Knot>(knot->previous());

    // Проходим по всем предыдущим узлам, расстояния до которых
    // от начала маршрута больше, чем до текущего
    while ((previousKnot)
        && (previousKnot->getDistance() >= knot->getDistance()))
    {
      // Если среди них нашёлся совпадающий с текущим, удаляем его.
      // Иначе передвигаем узел, переупорядочивая их.
      if (previousKnot->getTop() == knot->getTop())
      {
        previousKnot->out();
        previousKnot.reset();
      }
      else
        previousKnot->follow(knot);

      previousKnot = std::dynamic_pointer_cast<Knot>(knot->previous());
    }
  }
//==============================================================================
} // !TriangularGeometry
//==============================================================================
