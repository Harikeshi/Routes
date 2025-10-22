#include <Geometry/Operations/CutPolygon.hpp>

#include <Geometry/GeometryException.hpp>
//! AbstractOperations
#include <AbstractOperations/Algorithms/Area.hpp>
#include <AbstractOperations/Algorithms/Envelope.hpp>
#include <AbstractOperations/Algorithms/Intersection.hpp>
//==============================================================================
namespace Geometry
{
//==============================================================================
  MultiPolygon2D cutPolygon(
    const Polygon2D& polygon, size_t stepNumberX, size_t stepNumberY)
  {
    // Пустой полигон резать не нужно
    if (polygon.outer().empty())
      return MultiPolygon2D();

    // Шаг сетки не может быть нулевым
    if (stepNumberX == 0 || stepNumberY == 0)
      throw GeometryFailure(GeometryEnumFailure::ZeroGridStep);

    // Обернём полигон в коробку
    Box2D box;

    try
    {
      AbstractOperations::envelope(polygon, box);
    }
    catch (...)
    {
      ExceptionTools::FailureHandler<GeometryFailure>("cutPolygon");
    }

    // Коэффициент, на который умножается площадь ячейки нарезки для
    // определения минимальных сохраняемых геометрий
    double areaCoef = 10e-8;
    // Минимальная площадь сохраняемых геометрий
    double minArea
      = areaCoef * AbstractOperations::area(box) / (stepNumberX * stepNumberY);


    // Вычислим размер шага по каждой оси
    double xStep = (box.max_corner().getX() - box.min_corner().getX())
      / stepNumberX;
    double yStep = (box.max_corner().getY() - box.min_corner().getY())
      / stepNumberY;

    MultiPolygon2D result;

    double leftSide = box.min_corner().getX();
    double rightSide = leftSide + xStep;
    for (int i = 0; i < stepNumberX; ++i)
    {
      double bottomSide = box.min_corner().getY();
      double topSide = bottomSide + yStep;
      for (int j = 0; j < stepNumberY; ++j)
      {
        // Получим коробку для текущего кусочка
        Box2D curBox(
          Point2D(leftSide, bottomSide),
          Point2D(rightSide, topSide));

        // Получим текущий кусочек (или несколько) полигона,
        // используя его пересечение с коробкой
        MultiPolygon2D curPolygons;

        try
        {
          AbstractOperations::intersection(
            curBox.toPolygon(), polygon, curPolygons);

          for (size_t i = curPolygons.size(); i > 0; --i)
          {
            // Для крупных полигонов и разбиений отсекаем все геометрии,
            // площадь которых меньше точности метра; для маленьких задаётся
            // зависимость от площади ячейки, используемой для нарезки
            if (AbstractOperations::area(curPolygons[i - 1])
                                     < std::min(ACCURACY_METRE, minArea))
              curPolygons.erase(curPolygons.begin() + i - 1);
          }
        }
        catch (...)
        {
          ExceptionTools::FailureHandler<GeometryFailure>("cutPolygon");
        }
        

        result.insert(
          result.end(), curPolygons.begin(), curPolygons.end());

        bottomSide = topSide;
        topSide += yStep;
      }

      leftSide = rightSide;
      rightSide += xStep;
    }

    return result;
  }
//==============================================================================
} //! Geometry
//==============================================================================
