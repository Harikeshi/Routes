#include "Triangulation/Objects/Top.hpp"
//! TriangularGeometry
#include <TriangularGeometry/TriangularException.hpp>
//! MathTools
#include <MathTools/BasicMath/BaseCompare.hpp>
//==============================================================================
namespace TriangularGeometry
{
//==============================================================================
  Top::Top(const double x, const double y)
    : Link(), Point2D(x, y)
    , edge(nullptr)
    , id(0)
    , info(nullptr)
  { }
//==============================================================================
  Top::Top(const std::shared_ptr<Top>& top)
    : Link()
    , Point2D(top->getX(), top->getY())
    , edge(top->edge)
    , id(top->id)
    , info(nullptr)
  { }
//==============================================================================
  Top::~Top() {}
//==============================================================================
  TriangularGeometry::Comparison Top::compare(std::shared_ptr<Top> top)
  {
    if (getX() > top->getX())
      return Larger;
    else if (getX() < top->getX())
      return Less;
    else if (getY() > top->getY())
      return Larger;
    else if (getY() < top->getY())
      return Less;
    return Equally;
  }
//==============================================================================
  bool Top::isSame(std::shared_ptr<Top> top)
  {
    if (BasicMath::isEqual(getX(), top->getX()))
    {
      if (BasicMath::isEqual(getY(), top->getY()))
        return true;
    }
    return false;
  }
//==============================================================================
  bool Top::isClockwise(std::shared_ptr<Top> top1,
    std::shared_ptr<Top> top2,
    std::shared_ptr<Top> top3)
  {
    double sum = top1->getX() * top2->getY() - top1->getY() * top2->getX()
      + top2->getX() * top3->getY() - top2->getY() * top3->getX()
      + top3->getX() * top1->getY() - top3->getY() * top1->getX();
    /* При вычислении суммы необходимо результат поделить на 2,
    но в данном случае на знак это не влияет
    */
    return !(sum > 0.0);
  }
//==============================================================================
  bool Top::isLine(std::shared_ptr<Top> top1,
    std::shared_ptr<Top> top2,
    std::shared_ptr<Top> top3)
  {
    double det = (top2->getX() - top1->getX()) * (top3->getY() - top1->getY())
      - (top3->getX() - top1->getX()) * (top2->getY() - top1->getY());
    return (fabs(det) < std::numeric_limits<double>::epsilon());
  }
//==============================================================================
  std::shared_ptr<Edge> Top::getFixedEdge(
    const Geometry::PlanePosition direction)
  {
    if ((direction != Geometry::Left) && (direction != Geometry::Right))
      throw TriangularFailure(TriangularEnumFailure::IncorrectDirection);

    if (!edge)
      throw TriangularFailure(TriangularEnumFailure::TopHasNoEdge);

    // Берём любое ребро вершины
    std::shared_ptr<Edge> curEdge(edge);
    // Следующая грань в заданном направлении
    std::shared_ptr<Triangle> nextTriangle
      = ((direction == Geometry::Left) == (isSame(curEdge->getI())))
      ? curEdge->left : curEdge->right;

    // Если ребро уже фиксировано и располагается с правильной стороны,
    // искать не нужно
    if (!curEdge->fixed || nextTriangle)
    {
      std::shared_ptr<Top> thisTop
        = std::dynamic_pointer_cast<Top>(shared_from_this());

      do
      {
        std::shared_ptr<Top> otherTopInEdge = curEdge->getOtherTop(thisTop);

        // Нет следующего треугольника - не удалось найти фиксированное ребро
        if (nextTriangle == nullptr)
          throw TriangularFailure(TriangularEnumFailure::NoFixedEdge);

        curEdge = nextTriangle->edgeOppositeTop(otherTopInEdge);

        // Нет ребра - не удалось найти фиксированное ребро
        if (curEdge == nullptr)
          throw TriangularFailure(TriangularEnumFailure::NoFixedEdge);

        // Ребро совпало с исходным - фиксированных рёбер нет
        if (curEdge->isSame(edge))
          return nullptr;

        if (nextTriangle == curEdge->left)
          nextTriangle = curEdge->right;
        else
          nextTriangle = curEdge->left;

      } while (!curEdge->fixed);
    }

    return curEdge;
  }
//==============================================================================
  bool Top::updatePathInfo(
    std::shared_ptr<Top> prevTop,
    std::shared_ptr<Top> curTop)
  {
    double distance(
      prevTop->info->distance + Geometry::distance(*prevTop, *curTop));

    if (distance <= curTop->info->distance)
    {
      curTop->info->distance = distance;
      curTop->info->prevTop = prevTop;
      return true;
    }
    return false;
  }
//==============================================================================
  void Top::resetInfo()
  {
    if (info)
    {
      info->distance = std::numeric_limits<double>::max();
      info->prevTop = nullptr;
    }
    else
      info = std::make_shared<PathInfo>();
  }
//==============================================================================
} //! Geometry
//==============================================================================
