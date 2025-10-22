#include <Geodesic/GeodesicException.hpp>
//==============================================================================
namespace Geodesic
{
//==============================================================================
/*
 *\brief Соотношение именованных кодов и текстов ошибок генерируемых исключений
 */
const std::map<unsigned int, std::string> geodesicFailuresText =
{ 
  INCLUDE_STANDART_FAILURE_IN_MAP
  {
    OrthodromyThroughOnePoint,
    "<OrthodromyThroughOnePoint>: Построение ортодромии через две совпадающие "
    "точки"
  },
  {
    LoxodromeThroughOnePoint,
    "<LoxodromeThroughOnePoint>: Построение локсодромии через две совпадающие "
    "точки"
  },
  {
    LoxodromeThroughPole,
    "<LoxodromeThroughPole>: Построение локсодромии через полюс, который не "
    "является начальной точкой"
  },
  {
    NonPositiveAccuracy,
    "<NonPositiveAccuracy>: Задание неположительной точности для итерационного "
    "метода"
  },
  {
    NegativeDistanceLikeValue,
    "<NegativeDistanceLikeValue>: Задание отрицательного значения для параметра типа нормы"
  },
  {
    NullGeometryIncorrectOperation,
    "<NullGeometryIncorrectOperation>: Попытка выполнить для нулевой геометрии операцию, не определённую для неё"
  },
  {
    CircleDoNotExist,
    "<CircleDoNotExist>: Попытка построить не существующую при заданных "
    "параметрах окружность"
  },
  {
    EllipseDoNotExist,
    "<EllipseDoNotExist>: Попытка построить не существующий при заданных "
    "параметрах эллипс"
  },
  {
    IncorrectSegmentLengthForEllipseApproximation,
    "<IncorrectSegmentLengthForEllipseApproximation>: Задание некорректного "
    "значения длины стороны полигонального приближения эллипса"
  },
  {
    UnknownCoordinateSystem,
    "<UnknownCoordinateSystem>: Неизвестная система координат"
  },
  {
    PointOutOfProjection,
    "<PointOutOfProjection>: Точка, которую нужно спроецировать на эллипсоид, "
    "находится за пределами его проекции"
  },
  {
    NegativeDistance,
    "<NegativeDistance>: Отрицательная длина или расстояние"
  }, 
  {
    DisConnectedPolygon,
    "<DisConnectedPolygon>: При проецировании полигона была нарушена его связность"
  },
  {
    NegativeRadius,
    "<NegativeRadius>: Отрицательный радиус окружности"
  },
  {
    EmptyPolygon,
    "<EmptyPolygon>: Пустой полигон"
  },
  {
    EmptyLinestring,
    "<EmptyLinestring>: Пустая ломаная"
  }
};
//==============================================================================
GeodesicFailure::GeodesicFailure(const unsigned int failureId,
                                 const std::string& postfix /*= ""*/,
                                 const std::string& prefix /*= ""*/) noexcept
: Failure(
  failureId,
  geodesicLibTitle,
  geodesicFailuresText.at(failureId),
  postfix,
  prefix)
{}
//==============================================================================
} //! Geodesic
//==============================================================================
