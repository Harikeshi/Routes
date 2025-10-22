#include "Triangulation/Objects/Edge.hpp"
#include "Triangulation/Objects/Top.hpp"
#include "Triangulation/Objects/Triangle.hpp"
//! Geometry
#include <Geometry/Objects/BorderedLine.hpp>
//! std
#include <limits>
//==============================================================================
namespace TriangularGeometry
{
//==============================================================================
  Triangle::Triangle()
    : Link()
    , p(nullptr)
    , q(nullptr)
    , r(nullptr)
    , pCenter(Point2D(0.,0.))
    , radius(0.)
    , finit(true)
    , met(false)
    , id(std::numeric_limits<uint32_t>::max())
    , neighTr(nullptr)
  { }
//==============================================================================
 Triangle::Triangle(
   const std::shared_ptr<Edge>& edge1,
   const std::shared_ptr<Edge>& edge2,
   const std::shared_ptr<Edge>& edge3)
 : Link()
 , p(edge1)
 , q(edge2)
 , r(edge3)
 , finit(true)
 , met(false)
 , id(std::numeric_limits<uint32_t>::max())
 , neighTr(nullptr)
{
 //! Задание центра и радиуса описанной окружности
 circle();
}
//==============================================================================
  Triangle::Triangle(const std::shared_ptr<Triangle>& triangle)
    : Link()
    , p(triangle->p)
    , q(triangle->q)
    , r(triangle->r)
    , pCenter(triangle->pCenter)
    , radius(triangle->radius)
    , finit(triangle->finit)
    , met(triangle->met)
    , id(triangle->id)
    , neighTr(triangle->neighTr)
{}
//==============================================================================
Triangle::~Triangle() {}
//==============================================================================
void Triangle::disTry()
{
  if (p)
  {
    if (p->left == shared_from_this())
      p->left = nullptr;
    else
      p->right = nullptr;
  }
  if (q)
  {
    if (q->left == shared_from_this())
      q->left = nullptr;
    else
      q->right = nullptr;
  }
  if (r)
  {
    if (r->left == shared_from_this())
      r->left = nullptr;
    else
      r->right = nullptr;
  }
}
//==============================================================================
void Triangle::circle()
{
  std::shared_ptr<Top> i = p->getI();
  std::shared_ptr<Top> j = p->getJ();
  std::shared_ptr<Top> k = r->getJ();
  if ((j == k) || (i == k))
    k = r->getI();
  double a = j->getX() - i->getX();
  double b = j->getY() - i->getY();
  double c = k->getX() - i->getX();
  double d = k->getY() - i->getY();
  double e = (a * (j->getX() + i->getX()) + b * (j->getY() + i->getY())) / 2.0;
  double f = (c * (k->getX() + i->getX()) + d * (k->getY() + i->getY())) / 2.0;
  double g = a * d - b * c;
  if (fabs(g) < std::numeric_limits<double>::epsilon())
    g = 1.0;
  pCenter = Point2D((d * e - b * f) / g, (a * f - c * e) / g);
  a = (pCenter.getX() - i->getX());
  b = (pCenter.getY() - i->getY());
  radius = sqrt(a * a + b * b);
}
//==============================================================================
bool Triangle::isConFormFixEdges(std::shared_ptr<Top> top)
{
  std::shared_ptr<Top> topEdge(nullptr);
  bool result(true);
  if (p->fixed)
  {
    if ((q->getI() != p->getI()) && (q->getI() != p->getJ()))
      topEdge = q->getI();
    else
      topEdge = q->getJ();
    result = (p->position(*topEdge) == p->position(*top));
  }
  if (result && q->fixed)
  {
    if ((r->getI() != q->getI()) && (r->getI() != q->getJ()))
      topEdge = r->getI();
    else
      topEdge = r->getJ();
    result = (q->position(*topEdge) == q->position(*top));
  }
  if (result && r->fixed)
  {
    if ((p->getI() != r->getI()) && (p->getI() != r->getJ()))
      topEdge = p->getI();
    else
      topEdge = p->getJ();
    result = (r->position(*topEdge) == r->position(*top));
  }
  return result;
}
//==============================================================================
bool Triangle::isTopInCircle(std::shared_ptr<Top> top)
{
  double d = sqrt(pow(pCenter.getX() - top->getX() , 2)
    + pow(pCenter.getY() - top->getY(), 2)) - radius;
  return ((d <= std::numeric_limits<double>::epsilon())
    && isConFormFixEdges(top));
}
//==============================================================================
PositionInTriangle Triangle::positionRelativelyTriangleSide(
  std::shared_ptr<Top> top,
  std::shared_ptr<Edge> edge,
  std::shared_ptr<Top> triangleTop)
{
  // Ищем положение искомой точки относительно ребра
  Geometry::PlanePosition position = edge->position(*top);

  if ((position == Geometry::PlanePosition::Origin)
    || (position == Geometry::PlanePosition::Destination))
    return AtVertex;

  if (position == Geometry::PlanePosition::Between)
    return AtEdge;

  if (edge->position(*triangleTop) == position)
    return Inside;

  return Outside;
}
//==============================================================================
PositionInTriangle Triangle::positionInTriangle(std::shared_ptr<Top> top)
{
  std::shared_ptr<Top> topEdge(nullptr);
  PositionInTriangle result;

  // Выбираем вершину, которая не лежит на ребре p
  if ((q->getI() != p->getI()) && (q->getI() != p->getJ()))
    topEdge = q->getI();
  else
    topEdge = q->getJ();

  // Точка может лежать в треугольнике, если она находится на ребре
  // или по ту же сторону от ребра, что и topEdge
  result = positionRelativelyTriangleSide(top, p, topEdge);

  // Если вершина не внутри грани, положение определено
  if (!(result == Inside))
    return result;

  // Если же вершина внутри грани относительно выбранного ребра,
  // необходимо проверить её положение относительно остальных рёбер
  if ((r->getI() != q->getI()) && (r->getI() != q->getJ()))
    topEdge = r->getI();
  else
    topEdge = r->getJ();

  result = positionRelativelyTriangleSide(top, q, topEdge);

  // Если вершина не внутри грани, положение определено
  if (!(result == Inside))
    return result;

  // Если же вершина внутри грани относительно второго ребра,
  // необходимо проверить её положение относительно последнего ребра.
  // Полученное положение будет соответствовать реальному положению
  // относительно грани
  if ((p->getI() != r->getI()) && (p->getI() != r->getJ()))
    topEdge = p->getI();
  else
    topEdge = p->getJ();

  result = positionRelativelyTriangleSide(top, r, topEdge);

  return result;
}
//==============================================================================
bool Triangle::isTopInTriangle(std::shared_ptr<Top> top)
{
  return (positionInTriangle(top) != Outside);
}
//==============================================================================
void Triangle::nearTriangle(std::shared_ptr<Triangle>& a,
                       std::shared_ptr<Triangle>& b,
                       std::shared_ptr<Triangle>& c)
{
  if (p->left != shared_from_this())
    a = p->left;
  else
    a = p->right;
  if (q->left != shared_from_this())
    b = q->left;
  else
    b = q->right;
  if (r->left != shared_from_this())
    c = r->left;
  else
    c = r->right;
}
//==============================================================================
bool Triangle::hasTopTriangle(std::shared_ptr<Top> top)
{
  return ((p->getI() == top) || (p->getJ() == top)
    || (q->getI() == top) || (q->getJ() == top));
}
//==============================================================================
std::shared_ptr<Top> Triangle::topOppositeEdge(std::shared_ptr<Edge> edge)
{
  if ((p->getI() != edge->getI()) && (p->getI() != edge->getJ()))
    return p->getI();
  if ((p->getJ() != edge->getI()) && (p->getJ() != edge->getJ()))
    return p->getJ();
  if ((q->getI() != edge->getI()) && (q->getI() != edge->getJ()))
    return q->getI();
  return q->getJ();
}
//==============================================================================
void Triangle::allTops(std::shared_ptr<Top>& i,
                        std::shared_ptr<Top>& j,
                        std::shared_ptr<Top>& k)
{
  i = p->getI();
  j = p->getJ();
  k = r->getJ();
  if ((j == k) || (i == k))
    k = r->getI();
}
//==============================================================================
std::shared_ptr<Edge> Triangle::edgeOppositeTop(std::shared_ptr<Top> top)
{
  if ((p->getI() != top) && (p->getJ() != top))
    return p;
  else if ((q->getI() != top) && (q->getJ() != top))
    return q;
  return r;
}
//==============================================================================
} // !TriangularGeometry
//==============================================================================
