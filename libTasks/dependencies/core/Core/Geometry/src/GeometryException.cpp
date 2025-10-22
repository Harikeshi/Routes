#include <Geometry/GeometryException.hpp>
//==============================================================================
namespace Geometry
{
//==============================================================================
/*
 *\brief Соотношение именованных кодов и текстов ошибок генерируемых исключений
 */
const std::map<unsigned int, std::string> geomFailuresText =
{ 
  INCLUDE_STANDART_FAILURE_IN_MAP
  { 
    LineThroughOnePoint,
    "<LineThroughOnePoint>: Построение прямой через две совпадающие точки"
  },
  { 
    LineWithZeroParams,
    "<LineWithZeroParams>: Построение прямой с некорретным уравнением a=b=0"
  },
  {
    ParallelLinesIntersect,
    "<ParallelLinesIntersect>: Пересечение двух параллельных прямых"
  },
  {
    SameLinesIntersect,
    "<SameLinesIntersect>: Пересечение двух совпадающих прямых"
  },
  {
    IncorrectPoint,
    "<IncorrectPoint>: Некорректная точка"
  },
  {
    BaseCoefsForVerticalLine,
    "<BaseCoefsForVerticalLine>: Уравнение с угловым коэффициентом для "
    "вертикальной прямой"
  },
  {
    NotABasis,
    "<NotABasis>: Попытка задания системы линейно зависимых векторов "
    "в качестве базиса"
  },
  {
    SectorIndexOutOfRange,
    "<SectorIndexOutOfRange>: "
    "Попытка найти сектор по индексу, выходящему за границы списка"
  },
  {
    DividingByZero,
    "<DividingByZero>: Попытка поделить вектор на ноль"
  },
  {
    NullVectorIncorrectOperation,
    "<NullVectorIncorrectOperation>: Попытка провести с нулевым вектором "
    "операцию, которая для него не определена"
  },
  {
    MinimalCircleCoveringEmptyPolygon,
    "<MinimalCircleCoveringEmptyPolygon>: "
    "Попытка построить круг минимального радиуса, "
    "содержащий пустой многоугольник"
  },
  {
    CircumscribedCircleForBadTriangle,
    "<CircumscribedCircleForBadTriangle>: "
    "Попытка построить описанный около треугольника, "
    "вершины которого лежат на одной прямой и не совпадают, круг"
  },
  {
    NegativeRadius,
    "<NegativeRadius>: Построение окружности с отрицательным радиусом"
  },
  {
    TangentThroughInsidePoint,
    "<TangentThroughInsidePoint>: Построение касательной через точку внутри "
    "окружности"
  },
  {
    NegativePolygonSideLength,
    "<NegativePolygonSideLength>: Отрицательная длина стороны многоугольника"
  },
  {
    PointsCountForPolygonNotValid,
    "<PointsCountForPolygonNotValid>: Корректное количество вершин для "
    "многоугольника не найдено"
  },
  {
    NoMaximumPointsCountForPolygon,
    "<NoMaximumPointsCountForPolygon>: Нет максимального количества вершин "
    "для полигона"
  },
  {
    SeparateInvalidPolygon,
    "<SeparateInvalidPolygon>: "
    "Попытка разделить на две части некорректный многоугольник"
  },
  {
    SeparatePolygonUnknownFailure,
    "<SeparatePolygonUnknownFailure>: "
    "Неизвестная ошибка при разделении многоугольника на две части"
  },
  {
    PolygonToCirclesInvalidPolygon,
    "<PolygonToCirclesInvalidPolygon>: "
    "Создание покрытия кругами для некорректного многоугольника"
  },
  {
    CentroidIsNotFound,
    "<CentroidIsNotFound>: "
    "Не удалось найти центр масс полигона"
  },
  {
    EmptyPolygon,
    "<EmptyPolygon>: "
    "Пустой полигон"
  },
  {
    EmptyLinestring,
    "<EmptyLinestring>: "
    "Пустая ломаная"
  },
  {
    NoBorderedPointsInPolygon,
    "<NoBorderedPointsInPolygon>: Не удалось найти граничные точки полигона"
  },
  {
    PolygonIndexIsOutOfRange,
    "<PolygonIndexIsOutOfRange>: Индекс вышел за пределы полигона"
  },
  {
    EmptySectorPolygon,
    "<EmptySectorPolygon>: Пустой полигон сектора"
  },
  {
    NegativeOrZeroWidthExpansion,
    "<NegativeOrZeroWidthExpansion>: Задано нулевое или отрицательное "
    "расширение сектора"
  },
  {
    DivisionByZero,
    "<DivisionByZero>: Деление на ноль"
  },  
  {
    ZeroGridStep,
      "<ZeroGridStep>: Нулевой шаг сетки"
  }
};
//==============================================================================
GeometryFailure::GeometryFailure(const unsigned int failureId,
                                 const std::string& postfix /*= ""*/,
                                 const std::string& prefix /*= ""*/ ) noexcept
: Failure ( 
           failureId, 
           geomLibTitle, 
           geomFailuresText.at(failureId),
           postfix,
           prefix)
{
}
//==============================================================================
} //! Geometry
//==============================================================================
