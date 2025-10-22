#include <Geometry/Objects/BorderedLine.hpp>

#include <Geometry/Objects/Vector3D.hpp>
//! MathTools
#include <MathTools/BasicMath/Compare.hpp>
//==============================================================================
namespace Geometry
{
//==============================================================================
  BorderedLine::BorderedLine(
    const Point2D& firstPoint, const Point2D& secondPoint) :
    Line(firstPoint, secondPoint), mStart(firstPoint), mFinish(secondPoint)
  { }
//==============================================================================
  bool BorderedLine::atBorderLine(const Point2D& point) const
  {
    if (hasPoint(point))
    {
      return ((((mStart.getX() <= point.getX()) 
          && (point.getX() <= mFinish.getX()))
        || ((mStart.getX() >= point.getX()) 
          && (point.getX() >= mFinish.getX())))
        && (((mStart.getY() <= point.getY()) 
          && (point.getY() <= mFinish.getY()))
          || ((mStart.getY() >= point.getY()) 
            && (point.getY() >= mFinish.getY()))));
    }
    return false;
  }
//==============================================================================
  bool BorderedLine::nearBorderLine(
    const Point2D& point, const double eps/* = ACCURACY_DOUBLE*/) const
  {
    if (hasPointNear(point, eps))
    {
      double minX(std::min(mStart.getX(), mFinish.getX())),
             maxX(std::max(mStart.getX(), mFinish.getX())),
             minY(std::min(mStart.getY(), mFinish.getY())),
             maxY(std::max(mStart.getY(), mFinish.getY()));

//      if (isVertical())
      {
        // Если отрезок параллелен оси y, допускается отклонение координаты x
        // его точек
        minX -= eps;
        maxX += eps;
      }
//            else if (getBaseCoeffs().first == 0.)
      {
        // Если отрезок параллелен оси x, допускается отклонение координаты y
        // его точек
        minY -= eps;
        maxY += eps;
      }

      return (((minX <= point.getX()) && (point.getX() <= maxX))
        && (((minY <= point.getY()) && (point.getY() <= maxY))));
    }

    return false;
  }
//==============================================================================
  bool BorderedLine::isIntersect(const BorderedLine& borderLine) const
  {
    //! если прямые, на которых лежат отрезки, пересекаются в 1 точке
    Point2D crossP;
    if (crossPoint(borderLine, crossP))
    {
      return (atBorderLine(crossP) && borderLine.atBorderLine(crossP));
    }

    //! Если точки пересечения нет и прямые различны - отрезки не пересекаются
    if (!isSameTo(borderLine))
      return false;

    //! Если прямые совпали, проверяем возможное наложение отрезков
    return (
      // начало второго отрезка находится внутри первого
      (atBorderLine(borderLine.mStart))
      || // конец второго отрезка находится внутри первого
      (atBorderLine(borderLine.mFinish)));
  }
//==============================================================================
  bool BorderedLine::isIntersectWithoutBorder(
    const BorderedLine& borderLine) const
  {
    //! если прямые, на которых лежат отрезки, пересекаются в 1 точке
    Point2D crossP;
    if (crossPoint(borderLine, crossP))
    {
      return (atBorderLine(crossP) && borderLine.atBorderLine(crossP)
        && !(crossP == mStart) && !(crossP == borderLine.mStart)
        && !(crossP == mFinish) && !(crossP == borderLine.mFinish));
    }

    //! Если точки пересечения нет и прямые различны - отрезки не пересекаются
    if( ! isSameTo( borderLine ) )
      return false;

    //! Если прямые совпали, проверяем возможное наложение отрезков
    return ( 
      // начало второго отрезка находится внутри первого
      ( atBorderLine ( borderLine.mStart )
        && ! ( borderLine.mStart == mStart ) 
        && ! ( borderLine.mStart == mFinish ) )
      || // конец второго отрезка находится внутри первого
      ( atBorderLine ( borderLine.mFinish ) 
        && ! ( borderLine.mFinish == mStart ) 
        && ! ( borderLine.mFinish == mFinish ) )
      || // отрезки совпадают с точностью до направления 
      ( ( borderLine.mFinish == mStart ) 
          || ( borderLine.mFinish == mFinish ) )
        && ( (borderLine.mStart == mStart) 
          || (borderLine.mStart == mFinish) ) 
      );
  }
//==============================================================================
  Geometry::PlanePosition BorderedLine::position(const Point2D& point) const
  {
    Point3D pSt(mStart.getX(), mStart.getY());
    Point3D pFin(mFinish.getX(), mFinish.getY());
    Point3D p(point.getX(), point.getY());

    Vector3D v1(pSt, pFin);
    Vector3D v2(pSt, p);

    double vectorProduct = (v1 * v2).getOZ();
    if (vectorProduct > 0.)
      return Left;
    else if (vectorProduct < 0.)
      return Right;
    double scalarProduct = v1 & v2;
    if (scalarProduct < 0.0)
      return Behind;
    else if (BasicMath::isEqual(scalarProduct, 0.0))
      return Origin;
    double v1Len = v1.length();	
    double v2Len = v2.length();	 
    if (v1Len * v1Len < v2Len * v2Len)
      return Beyond;
    else if (BasicMath::isEqual(v1Len, v2Len))
      return Destination;
    return Between;
  }
//==============================================================================
  Point2D BorderedLine::findPointAtLine(double dist, bool atLine)
  {
    // Случай параллельности y
    if (isVertical())
    {
      // Определяем, должна находиться точка под второй точкой
      // отрезка или над ней
      bool isUnderFromPointSecond 
        = ((mFinish.getY() < mStart.getY()) && (!atLine))
        || ((mFinish.getY() > mStart.getY()) && (atLine));

      if (isUnderFromPointSecond)
        return Point2D(mStart.getX(), mFinish.getY() - dist);

      return Point2D(mStart.getX(), mFinish.getY() + dist);
    }

    // k и b - коэфициенты уравнения прямой
    double k, b;
    std::tie(k, b) = getBaseCoeffs();

    // Случай параллельности х
    if (fabs(k) < ACCURACY_DOUBLE)
    {
      // Определяем, должна находиться точка слева от второй точки
      // отрезка или справа от неё
      bool isLeftFromPointSecond
        = ((mFinish.getX() < mStart.getX()) && (!atLine))
        || ((mFinish.getX() > mStart.getX()) && (atLine));

      if (isLeftFromPointSecond)
        return Point2D(mFinish.getX() - dist, mStart.getY());

      return Point2D(mFinish.getX() + dist, mStart.getY());
    }

    // Откладываем точку на заданное расстояние от второй точки 
    // отрезка
    double offset = sqrt(dist * dist / (1 + k * k));
    double x = mFinish.getX() + offset;
    Point2D resPoint = Point2D(x, k * x + b);

    // Находим расстояние от первой точки отрезка до новой
    double distToResPoint = distance(mStart, resPoint);

    // Проверяем, что нашли точку в правильном направлении
    bool pointAtLineDmax = (dist > getLength())
      && (((distToResPoint < dist) && (atLine))
        || ((distToResPoint > dist) && (!atLine)));

    bool pointAtLineLmax = (dist <= getLength())
      && (((distToResPoint < getLength()) && (atLine))
        || ((distToResPoint > getLength()) && (!atLine)));

    if (pointAtLineDmax || pointAtLineLmax)
      return resPoint;

    // Если направление неправильное, откладываем точку в другом
    x = mFinish.getX() - offset;
    resPoint.set(x, k * x + b);

    return resPoint;
  }
//==============================================================================
  Point2D BorderedLine::findPointAtPerpendLine(
    const Point2D& pointCross, double dist, bool isLeftPoint) const
  {
    Point2D pLeft, pRight;

    if (isVertical()) //исходная прямая||y
    {
      pLeft.set(pointCross.getX() - dist, pointCross.getY());
      pRight.set(pointCross.getX() + dist, pointCross.getY());
    }
    else
    {
      // Коэффициенты исходной прямой
      double k0, b0;
      std::tie(k0, b0) = getBaseCoeffs();

      if (fabs(k0) < ACCURACY_DOUBLE) //исходная прямая||x
      {
        pLeft.set(pointCross.getX(), pointCross.getY() + dist);
        pRight.set(pointCross.getX(), pointCross.getY() - dist);
      }
      else
      {
        Line perpendLine = perpendicularLine(pointCross);

        double k, b;
        std::tie(k, b) = perpendLine.getBaseCoeffs();

        double xLeft = dist / sqrt(1 + k * k) + pointCross.getX();
        double xRight = -1. * dist / sqrt(1 + k * k) + pointCross.getX();

        pLeft.set(xLeft, k * xLeft + b);
        pRight.set(xRight, k * xRight + b);
      }
    }

    if (position(pLeft) == Right)
      std::swap(pLeft, pRight);

    if (isLeftPoint)
      return pLeft;

    return pRight;
  }
//==============================================================================
} //! Geometry
//==============================================================================
