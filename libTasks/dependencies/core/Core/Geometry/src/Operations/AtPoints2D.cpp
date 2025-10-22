#include <Geometry/Operations/AtPoints2D.hpp>

#include <Geometry/Objects/Vector3D.hpp>
#include <Geometry/Objects/BorderedLine.hpp>
#include <Geometry/Operations/AtVectors.hpp>
#include <Geometry/GeometryException.hpp>
//==============================================================================
namespace Geometry
{
//==============================================================================
  double cosAngle(
    const Point2D& point1, const Point2D& ourPoint, const Point2D& point2)
  {
    Point3D p1(point1.getX(), point1.getY());
    Point3D p2(point2.getX(), point2.getY());
    Point3D p(ourPoint.getX(), ourPoint.getY());

    Vector3D v1(p, p1);
    Vector3D v2(p, p2);

    return cosAngle(v1, v2);
  }
//==============================================================================
Radian lineAngularity(const Point2D& point1, const Point2D& point2)
{
  Radian result(
    std::fmod(
      std::atan2(
        point2.getY() - point1.getY(),
        point2.getX() - point1.getX()),
      2. * M_PI));
  if (result.get() < 0.)
    result += Radian(2. * M_PI);

  return result;
}
//==============================================================================
Point2D middle(const Point2D& a, const Point2D& b) noexcept
{
  Point2D resultPoint((a.getX() + b.getX()) / 2.,
                      (a.getY() + b.getY()) / 2.);

  return resultPoint;
}
//==============================================================================
Radian findAngle(
  const Point2D& point1, const Point2D& ourPoint, const Point2D& point2,
  bool isClockwise /* = false */)
{
  double ax = point1.getX() - ourPoint.getX();
  double ay = point1.getY() - ourPoint.getY();
  double bx = point2.getX() - ourPoint.getX();
  double by = point2.getY() - ourPoint.getY();
  double scalarProduct = ax * bx + ay * by;
  double aLength = sqrt(ax * ax + ay * ay);
  double bLength = sqrt(bx * bx + by * by);

  if (BasicMath::isNull(aLength) || BasicMath::isNull(bLength))
    throw GeometryFailure(GeometryEnumFailure::NullVectorIncorrectOperation);

  double cosAngle = scalarProduct / (aLength * bLength);
  if (fabs(cosAngle) > 1.)
    cosAngle /= fabs(cosAngle);

  //! Если третья точка лежит справа от вектора ourPoint-point1,
  //! угол больше 180
  Geometry::BorderedLine firstSide(ourPoint, point1);
  if (!isClockwise && (firstSide.position(point2) == Right))
    return Radian(2 * M_PI - acos(cosAngle));
  if (isClockwise && (firstSide.position(point2) == Left))
    return Radian(2 * M_PI - acos(cosAngle));

  return Radian(acos(cosAngle));
}
//==============================================================================
Point2D findPointAtLine(
  const Point2D& pointFirst, 
  const Point2D& pointSecond, 
  double dist, bool atLine)
{
  return Geometry::BorderedLine(
    pointFirst, pointSecond).findPointAtLine(dist, atLine);
}
//==============================================================================
} //! Geometry
//==============================================================================
