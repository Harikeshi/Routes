#ifndef GEOMETRY_EXCEPTIONS_HPP
#define GEOMETRY_EXCEPTIONS_HPP
//==============================================================================
#include <ExceptionTools/ExceptionTools.hpp>
//==============================================================================
namespace Geometry
{
//==============================================================================
  using namespace ExceptionTools;
//==============================================================================
/*
 *\brief Именованные коды генерируемых исключений
 */
enum GeometryEnumFailure
{
  //!< прямая через две совпадающие точки
  LineThroughOnePoint = SpecialFailure,                     //!< 100
  //!< прямая с вырожденным уравнением c=0
  LineWithZeroParams,                                       //!< 101
  //!< пересечение параллельных прямых
  ParallelLinesIntersect,                                   //!< 102
  //!< пересечение совпадающих прямых
  SameLinesIntersect,                                       //!< 103
  //!< некорректная точка
  IncorrectPoint,                                           //!< 104
  //!< уравнение с угловым коэффициентом для вертикальной прямой
  BaseCoefsForVerticalLine,                                 //!< 105

  //!< попытка задания системы л/з векторов в качестве базиса
  NotABasis = LineThroughOnePoint + 10,                     //!< 110

  //!< попытка найти сектор по индексу, выходящему за границы списка
  SectorIndexOutOfRange = NotABasis + 10,                   //!< 120

  // !< деление вектора на нуль
  DividingByZero = SectorIndexOutOfRange + 10,              //!< 130
  //!< попытка провести с нулевым вектором не определённую для него операцию
  NullVectorIncorrectOperation,                             //!< 131

  //!< построение минимального круга, содержащего пустой многоугольник
  MinimalCircleCoveringEmptyPolygon = DividingByZero + 10,  //!< 140
  //!< построение описанного круга для плохого треугольника
  CircumscribedCircleForBadTriangle,                        //!< 141
  //!< Окружность с отрицательным радиусом
  NegativeRadius,                                           //!< 142 
  //!< Касательная через внутреннюю точку
  TangentThroughInsidePoint,                                //!< 143
  //!< Отрицательная длина стороны многоугольника
  NegativePolygonSideLength,                                //!< 144
  //!< Корректное количество вершин для многоугольника не найдено
  PointsCountForPolygonNotValid,                            //!< 145
  //!< Нет максимального количества вершин для полигона
  NoMaximumPointsCountForPolygon,                           //!< 146

  //!< разбиение на две части некорректного многоугольника
  SeparateInvalidPolygon = MinimalCircleCoveringEmptyPolygon + 10, //!< 150
  //!< неизвестная ошибка при разбиении многоугольника на две части
  SeparatePolygonUnknownFailure,                            //!< 151
  //!< создание покрытия кругами для некорректного многоугольника
  PolygonToCirclesInvalidPolygon,                           //!< 152

  //!< Не удалось найти центр масс полигона
  CentroidIsNotFound,                                       //!< 160
  //!< Пустой полигон
  EmptyPolygon,                                             //!< 161
  //!< Пустая ломаная
  EmptyLinestring,                                          //!< 162

  //!< Не удалось найти граничные точки полигона
  NoBorderedPointsInPolygon = CentroidIsNotFound + 10,      //!< 170
  //!< Индекс вышел за пределы полигона
  PolygonIndexIsOutOfRange,                             
  //!< Пустой полигон сектора
  EmptySectorPolygon,
  //!< Задано нулевое или отрицательное расширение сектора
  NegativeOrZeroWidthExpansion,
  //!< Деление на ноль
  DivisionByZero,

  //!< Нулевой шаг сетки
  ZeroGridStep = NoBorderedPointsInPolygon + 10      //!< 180
};
//==============================================================================
// !< название библиотеки для исключений
const std::string geomLibTitle = "Core::Geometry"; 
//==============================================================================
/*!
  * \brief Класс формирования исключений библиотеки GeometryTools
  */
class GeometryFailure : public ExceptionTools::Failure
{
//==============================================================================
public:
  /*!
  * \brief Создание исключения.
  * \details Требуется только код, остальные параметры подставятся автоматически 
  * \param[in] failureId - именованный код исключения
  * \param[in] postfix - текст уточняющий/дополняющий исключение
  *                      (может использоваться для передачи текста исходного
  *                       исключения)
  * \param[in] prefix - текст, локализующий место возникновения исключения
  *                     (может использоваться для указания имени функции,
  *                      в которой локализовано исключение - трассировка
  *                      исключения)
  */
  explicit GeometryFailure(const unsigned int failureId,
                           const std::string& postfix = "",
                           const std::string& prefix = "" ) noexcept;
};
//==============================================================================
} //! Geometry
//==============================================================================
#endif //! GEOMETRY_EXCEPTIONS_HPP