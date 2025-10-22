#include <Inner/SubEnvironment.hpp>
//! ReachableAreaCalculator
#include <ReachableArea/ReachableAreaException.hpp>
//==============================================================================
using namespace ReachableArea;
//==============================================================================
namespace ReachableAreaCalculator
{
//==============================================================================
GeometrySubEnvironment* AbstractSubEnvironmentCutter::
generateCuttedSubEnvironment(const GeometryEnvironment* parent) const
{
  GeometrySubEnvironment* sub = new GeometrySubEnvironment(parent);

  uinttype polygonsCount = parent->polygonsCount();

  // перебор всех полигонов
  for (uinttype i = 1; i <= polygonsCount; i++)
  {
    const EnvironmentPolygon& polygon = parent->getPolygon(i);

    // пропустить, если "коробки" не пересекаются
    if ( ! mBox.hasIntersectionWith(polygon.box()) )
      continue;

    uinttype pointsCount = polygon.pointsCount();

    // перебор всех границ полигона через перебор точек
    for (uinttype j = 0; j < pointsCount; j++)
    {
      uinttype pointId = polygon.firstPointId() + j;
      uinttype nextPointId = polygon.nextPointId(pointId);

      if (testEdge(parent->getPoint(pointId),
        parent->getPoint(nextPointId)))
      {
        GeometrySubEnvironment::Edge edge;
        edge.first = pointId;
        edge.second = nextPointId;
        sub->mEdges.push_back(edge);

        if (sub->mPolygonsIds.empty() || sub->mPolygonsIds.back() != i)
          sub->mPolygonsIds.push_back(i);
      }
    }
  }

  return sub;
}
//==============================================================================
GeometrySubEnvironment* AbstractSubEnvironmentCutter::
generateCuttedSubEnvironment(const GeometrySubEnvironment* parentSub) const
{
  const GeometryEnvironment* parent = parentSub->parentEnvironment();
  GeometrySubEnvironment* sub = new GeometrySubEnvironment(parent);

  sub->mEdges.reserve(parentSub->mEdges.size());

  // индекс текущего перебираемого полигона
  uinttype currentPolygonId = 0;
  //!< Следует ли досрочно перейти к следующему полигону?
  bool isNeedToSkipAPolygon(false);

  // перебор всех границ
  for (const GeometrySubEnvironment::Edge& edge : parentSub->mEdges)
  {
    // первая точка границы
    const EnvironmentPoint& firstPoint = parent->getPoint(edge.first);

    /*!
     * Если требуется перейти к следующему полигону,
     * пропускаем итерации, пока не найдём новый полигон.
     */
    if ((isNeedToSkipAPolygon) && (firstPoint.polygonId() == currentPolygonId))
      continue;
    else
      isNeedToSkipAPolygon = false;

    //! Если начался новый полигон...
    if (firstPoint.polygonId() != currentPolygonId)
    {
      currentPolygonId = firstPoint.polygonId();
      const EnvironmentPolygon& polygon =
        parent->getPolygon(currentPolygonId);

      //! Если коробки не пересекаются...
      if ( ! mBox.hasIntersectionWith(polygon.box()) )
      {
        isNeedToSkipAPolygon = true;
        continue;
      }
    }

    // добавление границы в суб-среду после проверки
    if (testEdge(firstPoint, parent->getPoint(edge.second)))
    {
      sub->mEdges.push_back(edge);

      if (sub->mPolygonsIds.empty() ||
        sub->mPolygonsIds.back() != currentPolygonId)
      {
        sub->mPolygonsIds.push_back(currentPolygonId);
      }
    }
  }
  return sub;
}
//==============================================================================
void CircleSubEnvironmentCutter::set(const Point2D& center, double radius)
{
  mCenter = center;
  mRadius = radius;

  mBox.expandToPoint(
    Point2D(mCenter.getX() - mRadius, mCenter.getY() - mRadius));

  mBox.expandToPoint(
    Point2D(mCenter.getX() + mRadius, mCenter.getY() + mRadius));
}
//==============================================================================
bool CircleSubEnvironmentCutter::testEdge(
  const Point2D& p1, const Point2D& p2) const
{
  // грубая проверка "коробкой"
  Box2D edgeBox(p1, p1);
  edgeBox.expandToPoint(p2);

  if ( ! mBox.hasIntersectionWith(edgeBox) )
    return false;

  // простая проверка по радиусу
  if (Geometry::distance(mCenter, p1) < mRadius)
    return true;

  if (Geometry::distance(mCenter, p2) < mRadius)
    return true;

  // если обе границы не лежат в круге отсечения, граница всё равно
  // может его пересекать. Рассмотрение этого случая.
  // Проверка на пересечение есть решение квадратного уравнения,
  // полученного из системы уравнений прямой и окружности.

  double x = p1.getX() - mCenter.getX();
  double y = p1.getY() - mCenter.getY();
  Radian alpha = Geometry::lineAngularity(p1, p2);
  double b = 2 * (y * std::sin(alpha.get()) + x * std::cos(alpha.get()));
  double c = x * x + y * y - mRadius * mRadius;
  double D = b * b - 4 * c;

  // уравнение решений не имеет
  if (D <= 0)
    return false;
  double sqrtD = std::sqrt(D);

  // прямая пересекает окружность, но граница не в области пересечения
  if (-b - sqrtD <= 0)
    return false;
  if (((-b - sqrtD) / 2.0) > Geometry::distance(p1, p2))
    return false;

  return true;
}
//==============================================================================
void EllipseSubEnvironmentCutter::set(
  const Point2D &f1, const Point2D &f2, double l)
{
  mFirstFocus = f1;
  mSecondFocus = f2;
  mEllipseParam = l;

  if (Geometry::distance(mFirstFocus, mSecondFocus) >= mEllipseParam)
    throw ReachableAreaFailure(EllipseSubEnvironmentCutterEllipseDoesNotExist);

  // Определение угла наклона эллипса
  mEllipseAlpha = Geometry::lineAngularity(mFirstFocus, mSecondFocus);
  mEllipseAlphaSin = std::sin(mEllipseAlpha.get());
  mEllipseAlphaCos = std::cos(mEllipseAlpha.get());

  // Определение параметров эллипса
  mEllipseA = mEllipseParam / double(2.0);
  mEllipseC = Geometry::distance(mFirstFocus, mSecondFocus) / double(2.0);
  mEllipseB = sqrt(mEllipseA * mEllipseA - mEllipseC * mEllipseC);
  mEllipseE = mEllipseC / mEllipseA;

  // Вчисление "коробки"
  // Формулы выводятся путём нахождения зависимостей x=x(fi) и y=y(fi)
  // с последующим взятием производных по fi для определения экстремумов
  double e2 = mEllipseE * mEllipseE;
  double sin2 = mEllipseAlphaSin * mEllipseAlphaSin;
  double cos2 = mEllipseAlphaCos * mEllipseAlphaCos;

  // Хитрое выражение
  double Wx = mEllipseAlphaCos / sqrt(1 - e2 * sin2 * (2 - e2));

  // Полярный угол, при котором X достигает экстремума
  double fiXmax = -std::acos(Wx);
  if (mEllipseAlphaSin < 0)
    fiXmax = -fiXmax;

  // Радиус, соответствующий fiXmax
  double rXMax = mEllipseB / sqrt(1 - e2 * Wx * Wx);

  // Хитрое выражение
  double Wy = mEllipseAlphaSin / sqrt(1 - e2 * cos2 * (2 - e2));

  // Полярный угол, при котором Y достигает экстремума
  double fiYmax = std::acos(Wy);
  if (mEllipseAlphaCos < 0)
    fiYmax = -fiYmax;

  // Радиус, соответствующий fiYmax
  double rYMax = mEllipseB / sqrt(1 - e2 * Wy * Wy);

  // Координаты центра
  double cx = (mFirstFocus.getX() + mSecondFocus.getX()) / double(2.0);
  double cy = (mFirstFocus.getY() + mSecondFocus.getY()) / double(2.0);
  mEllipseCenter = Point2D(cx, cy);

  // Растягиваем коробку двумя диагональными точками
  mBox.expandToPoint(
    Point2D( cx + rXMax*std::cos( fiXmax + mEllipseAlpha.get( ) ),
      cy + rYMax*std::sin(fiYmax + mEllipseAlpha.get())));
  mBox.expandToPoint(
    Point2D( cx - rXMax*std::cos( fiXmax + mEllipseAlpha.get( ) ),
      cy - rYMax*std::sin(fiYmax + mEllipseAlpha.get())));
}
//==============================================================================
bool EllipseSubEnvironmentCutter::testEdge(
  const Point2D& p1, const Point2D& p2) const
{
  // грубая проверка "коробкой"
  Box2D edgeBox(p1, p1);
  edgeBox.expandToPoint( p2 );

  if ( ! mBox.hasIntersectionWith(edgeBox) )
    return false;

  // Проверка по определению эллипса
  if (Geometry::distance(p1, mFirstFocus)
    + Geometry::distance(p1, mSecondFocus) < mEllipseParam)
    return true;
  if (Geometry::distance(p2, mFirstFocus)
    + Geometry::distance(p2, mSecondFocus) < mEllipseParam)
    return true;

  // азимуты и радиусы точек начала и конца границы в полярной СК с 
  // центром в центре эллипса и началом отсчёта, совпадающим с его осью
  Radian p1az = Geometry::lineAngularity(mEllipseCenter, p1) - mEllipseAlpha;
  double p1r = Geometry::distance(mEllipseCenter, p1);
  Radian p2az = Geometry::lineAngularity(mEllipseCenter, p2) - mEllipseAlpha;
  double p2r = Geometry::distance(mEllipseCenter, p2);

  // точки начала и конца границы в декартовой СК
  // с центром в центре эллипса и осью x, совпадающей с осью эллипса
  Point2D p1c(p1r * std::cos(p1az.get()), p1r * std::sin(p1az.get()));
  Point2D p2c(p2r * std::cos(p2az.get()), p2r * std::sin(p2az.get()));

  // направление отрезка в новой СК и соответствующие триг.функции
  Radian alpha = Geometry::lineAngularity(p1c, p2c);
  double sinAlpha = std::sin(alpha.get());
  double cosAlpha = std::cos(alpha.get());

  // проверка пересечения границы эллипсом путём решения уравнения,
  // полученного из системы уравнений прямой и эллипса
  double ellipseA2 = mEllipseA * mEllipseA;
  double ellipseB2 = mEllipseB * mEllipseB;

  double a = ellipseA2 * sinAlpha * sinAlpha +
    ellipseB2 * cosAlpha * cosAlpha;
  double b = 2 * (ellipseA2 * p1c.getY() * sinAlpha +
    ellipseB2 * p1c.getX() * cosAlpha);
  double c = ellipseA2 * p1c.getY() * p1c.getY() +
    ellipseB2 * p1c.getX() * p1c.getX() -
    ellipseA2 * ellipseB2;

  double D = b * b - 4 * a * c;

  // прямая не пересекает эллипс
  if (D <= 0)
    return false;

  double sqrtD = std::sqrt(D);
  double l = (-b - sqrtD) / (double(2.0) * a);

  // проверка принадлежности точки пересечения отрезку
  if (l <= 0 || l > Geometry::distance(p1c, p2c)) 
    return false;

  return true;
}
//==============================================================================
} //! ReachableAreaCalculator
