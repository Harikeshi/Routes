#include "Triangulation/Objects/Edge.hpp"
//! std
#include <limits>
//==============================================================================
namespace TriangularGeometry
{
//==============================================================================
Edge::Edge(const std::shared_ptr<Top>& startTop,
  const std::shared_ptr<Top>& finishTop)
  : Link()
  , BorderedLine(*startTop, *finishTop)
  , mI(startTop)
  , mJ(finishTop)
  , left(nullptr)
  , right(nullptr)
  , fixed(false)
{ }
//==============================================================================
Edge::Edge(const std::shared_ptr<Edge>& edge)
  : Link()
  , BorderedLine(*edge->getI(), *edge->getJ())
  , mI(edge->getI())
  , mJ(edge->getJ())
  , left(edge->left)
  , right(edge->right)
  , fixed(edge->fixed)
{ }
//==============================================================================
Edge::~Edge() {}
//==============================================================================
bool Edge::isBound()
{
  return fixed;
}
//==============================================================================
bool Edge::isSame(std::shared_ptr<Edge> edge)
{
  if ((mStart == edge->mStart) && (mFinish == edge->mFinish))
  {
    return true;
  }
  else if ((mFinish == edge->mStart) && (mStart == edge->mFinish))
  {
    std::swap(mStart, mFinish);
    std::swap(left, right);

    return true;
  }
  return false;
}
//==============================================================================
bool Edge::isIntersectEdge(std::shared_ptr<Edge> edge)
{
  bool result(false);
  Geometry::PlanePosition positionI = position(*edge->getI());
  Geometry::PlanePosition positionJ = position(*edge->getJ());
  bool topsByDiffSides = ((positionI == Geometry::PlanePosition::Left)
                       && (positionJ == Geometry::PlanePosition::Right))
                      || ((positionJ == Geometry::PlanePosition::Left)
                       && (positionI == Geometry::PlanePosition::Right));

  if (topsByDiffSides)
  {
    positionI = edge->position(*mI);
    positionJ = edge->position(*mJ);
    result = ((positionI == Geometry::PlanePosition::Left)
           && (positionJ == Geometry::PlanePosition::Right))
          || ((positionJ == Geometry::PlanePosition::Left)
           && (positionI == Geometry::PlanePosition::Right));
  }
  return result;
}
//==============================================================================
bool Edge::atEdge(std::shared_ptr<Top> top)
{
  Geometry::PlanePosition topPosition = position(*top);

  return (topPosition == Geometry::PlanePosition::Between);
}
//==============================================================================
bool Edge::isCounterclockwise(
  std::shared_ptr<Edge> edge,
  std::shared_ptr<Triangle> triangle)
{
  return (edge->position(*(triangle->topOppositeEdge(edge))) == Geometry::Left);
}
//==============================================================================
void Edge::scanBunch(
  std::shared_ptr<Edge> edge,
  std::shared_ptr<Top> top,
  Geometry::PlanePosition position,
  std::shared_ptr<Triangle>& adjacentTriangle,
  std::shared_ptr<Edge>& nextEdge)
{
  adjacentTriangle = nullptr;
  nextEdge = nullptr;

  if (((edge->getI() == top) && (position == Geometry::Left))
    || ((edge->getJ() == top) && (position == Geometry::Right)))
    adjacentTriangle = edge->left;
  else
    adjacentTriangle = edge->right;

  if (adjacentTriangle)
  {
    std::shared_ptr<Edge> tempEdge = adjacentTriangle->edgeOppositeTop(top);

    if ((adjacentTriangle->p != edge) && (adjacentTriangle->p != tempEdge))
      nextEdge = adjacentTriangle->p;
    else if ((adjacentTriangle->q != edge) && (adjacentTriangle->q != tempEdge))
      nextEdge = adjacentTriangle->q;
    else
      nextEdge = adjacentTriangle->r;
  }
}
//==============================================================================
std::shared_ptr<Edge> Edge::getNeighbour(
  std::shared_ptr<Top> top, bool findLeftEdge/* = true*/)
{
  std::shared_ptr<Triangle> adjacentTriangle(nullptr);
  std::shared_ptr<Edge> nextEdge(nullptr);

  Geometry::PlanePosition position
    = findLeftEdge ? Geometry::Left : Geometry::Right;

  std::shared_ptr<Edge> thisEdge
    = std::dynamic_pointer_cast<Edge>(shared_from_this());

  Edge::scanBunch(thisEdge, top, position, adjacentTriangle, nextEdge);

  return nextEdge;
}
//==============================================================================
void Edge::setI(std::shared_ptr<Top> top)
{
  mI = top;
  mStart = *top;
}
//==============================================================================
void Edge::setJ(std::shared_ptr<Top> top)
{
  mJ = top;
  mFinish = *top;
}
//==============================================================================
std::shared_ptr<TriangularGeometry::Top> Edge::getI()
{
  return mI;
}
//==============================================================================
std::shared_ptr<TriangularGeometry::Top> Edge::getJ()
{
  return mJ;
}
//==============================================================================
std::shared_ptr<TriangularGeometry::Top> Edge::getOtherTop(
  std::shared_ptr<Top> top)
{
  if (top->isSame(mI))
    return mJ;

  return mI;
}
//==============================================================================
} // !TriangularGeometry
//==============================================================================
