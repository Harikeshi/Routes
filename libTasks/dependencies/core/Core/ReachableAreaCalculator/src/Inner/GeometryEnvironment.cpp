#include <Inner/GeometryEnvironment.hpp>

#include <ReachableArea/ReachableAreaException.hpp>

//! std
#include <limits> // для std::numeric_limits
//==============================================================================
using namespace ReachableArea;
//==============================================================================
namespace ReachableAreaCalculator
{
//==============================================================================
const EnvironmentPoint& EnvironmentPolygon::firstPoint() const
{
  if (!mFirstPointId)
    throw ReachableAreaFailure(EnvironmentPolygonFirstPointNullId);
  if (!mSize)
    throw ReachableAreaFailure(EnvironmentPolygonNoPoints);

  return parentEnvironment()->getPoint(mFirstPointId);
}
//==============================================================================
uinttype EnvironmentPolygon::prevPointId(uinttype id) const
{
  if ((id < mFirstPointId) || (id > mFirstPointId + mSize - 1))
    throw ReachableAreaFailure(EnvironmentPolygonPointIdOutOfRange);

  return (id > mFirstPointId) ? id - 1 : mFirstPointId + mSize - 1;
}
//==============================================================================
uinttype EnvironmentPolygon::nextPointId(uinttype id) const
{
  if ((id < mFirstPointId) || (id > mFirstPointId + mSize - 1))
    throw ReachableAreaFailure(EnvironmentPolygonPointIdOutOfRange);

  return (id < mFirstPointId + mSize - 1) ? id + 1 : mFirstPointId;
}
//==============================================================================
double EnvironmentPolygon::distOnBorder(const EnvironmentPoint& p1,
                                        const EnvironmentPoint& p2) const
{
  // проверка на принадлежность точек полигону
  if (p1.polygonId() != mId || p2.polygonId() != mId)
    return std::numeric_limits<double>::infinity();

  // определение позиций точек внутри полигона по их глобальным id
  uinttype pos1 = p1.id() - mFirstPointId;
  uinttype pos2 = p2.id() - mFirstPointId;

  if (pos1 >= mSize || pos2 >= mSize)
    throw ReachableAreaFailure(EnvironmentPolygonPointIdOutOfRange);

  if (pos1 == pos2)
  {
    // расстояний от точки до самой себя
    return 0.0;
  }

  // в mLengths[0] лежит периметр, в остальных - расстояние до 
  // базовой точки по направлению обхода

  // при следовании по полигону от p1 до p2
  // не происходит проход через базовую точку
  if (pos2 > pos1)
  {
    if (pos1 == 0)
      return mLengths[pos2];

    return mLengths[pos2] - mLengths[pos1];
  }
  else
  {
    // если проход через базовую точку происходит
    if (pos2 == 0)
      return mLengths[0] - mLengths[pos1];
    return mLengths[0] - mLengths[pos1] + mLengths[pos2];
  }
}
//==============================================================================
bool EnvironmentPolygon::
operator == (const EnvironmentPolygon& p) const noexcept
{
  if ((mId != p.mId)
      || (mFirstPointId != p.mFirstPointId)
      || (mSize != p.mSize))
    return false;

  std::size_t countOfLengths(mLengths.size());
  if (countOfLengths != p.mLengths.size())
    return false;

  for (std::size_t i(0); i < countOfLengths; ++i)
    if (!BasicMath::isEqual(mLengths[i], p.mLengths[i]))
      return false;

  return true;
}
//==============================================================================
EnvironmentPolygon::EnvironmentPolygon(
  const GeometryEnvironment* parent,
  uinttype id,
  std::vector<EnvironmentPoint>::iterator first,
  std::vector<EnvironmentPoint>::iterator end)
  : EnvironmentObject(parent)
  , mId(id)
  , mSize(uinttype(end - first))
  , mBox(*first, *first)
{
  if (mSize < 2U)
    throw ReachableAreaFailure(EnvironmentPolygonTooFewPoints);

  mFirstPointId = first->id();
  mLengths.reserve(mSize);
  mLengths.push_back(0);

  double length = 0;
  const EnvironmentPoint * prevPoint = &(*first);

  for (auto iter = first + 1; iter != end; iter++)
  {
    mBox.expandToPoint(*iter);
    length += Geometry::distance(*iter, *prevPoint);
    mLengths.push_back(length);
    prevPoint = &(*iter);
  }

  length += Geometry::distance(*first, *prevPoint);
  mLengths[0] = length;
}
//==============================================================================
GeometryEnvironment::GeometryEnvironment(
  const std::vector<Polygon2D>& polygons)
{
  for (const Polygon2D & polygon : polygons)
    addPolygon(polygon);
}
//==============================================================================
void GeometryEnvironment::addPolygon(const Polygon2D& polygon)
{
  if (polygon.outer().size() < 3)
    throw ReachableAreaFailure(
              GeometryEnvironmentAddPolygonWithLessThanThreeVerticesOuterRing);

  // id начинаются с 1
  uinttype polygonId = uinttype(mPolygons.size() + 1);
  uinttype firstPos = uinttype(mPoints.size());
  uinttype pointId = firstPos + 1;

  std::size_t polygonSize(polygon.outer().size());
  for (size_t i = 0; i < polygonSize; ++i)
    mPoints.emplace_back(this, pointId++, polygonId, polygon.outer()[i]);

  // убрать последнюю точку, если пришёл замкнутый полигон
  if (mPoints.back() == mPoints[firstPos])
    mPoints.pop_back();
    
  EnvironmentPolygon envPolygon(
    this, polygonId, mPoints.begin() + firstPos, mPoints.end());
  mPolygons.emplace_back(envPolygon);
}
//==============================================================================
const EnvironmentPoint& GeometryEnvironment::getPoint(uinttype id) const
{
  if (id > mPoints.size() || id == 0)
    throw ReachableAreaFailure(GeometryEnvironmentPointIdOutOfRange);

  return mPoints[id - 1];
}
//==============================================================================
const EnvironmentPolygon& GeometryEnvironment::getPolygon(uinttype id) const
{
  if (id > mPolygons.size() || id == 0)
    throw ReachableAreaFailure(GeometryEnvironmentPolygonIdOutOfRange);

  return mPolygons[id - 1];
}
//==============================================================================
bool GeometryEnvironment::
operator == (const GeometryEnvironment& p) const noexcept
{
  std::size_t countOfPoints(mPoints.size());
  std::size_t countOfPolygons(mPolygons.size());

  if ((countOfPoints != p.mPoints.size())
      || (countOfPolygons != p.mPolygons.size()))
    return false;

  for (std::size_t i(0); i < countOfPoints; ++i)
    if (mPoints[i] != p.mPoints[i])
      return false;

  for (std::size_t i(0); i < countOfPolygons; ++i)
    if (mPolygons[i] != p.mPolygons[i])
      return false;

  return true;
}
//==============================================================================
} // ReachableAreaCalculator
