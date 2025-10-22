#ifndef REACHABLE_AREA_EXCEPTION_HPP
#define REACHABLE_AREA_EXCEPTION_HPP
//==============================================================================
#include <ExceptionTools/ExceptionTools.hpp>
//==============================================================================
namespace ReachableArea
{
//==============================================================================
using namespace ExceptionTools;
//==============================================================================
/*
 *\brief Именованные коды генерируемых исключений
 */
enum ReachableAreaEnumFailure
{
//==============================================================================
  //! Внутренние исключения
//==============================================================================
  /*!<
   * Терминальная точка, найденная при обходе границы,
   * не является начальной точкой полидуги.
   */
  ArcsNotBeginTerminalPointOnBorder = SpecialFailure,      //!< 100
  /*!<
   * Вершины внутреннего кольца многоугольника оказались расположенными
   * в порядке "по часовой стрелке".
   */
  ArcsClockWiseInnerRing,                                  //!< 101
  /*!<
   * Начальная или конечная точка полидуги
   * оказалась точкой обрыва не из-за границы.
   */
  ArcsPolyarcBreakNoByEdge,                                //!< 102
  //!< Нулевая дополнительная информация о начале полидуги.
  ArcsPolyarcNullBeginInfo,                                //!< 103
  //!< Нулевая дополнительная информация о конце полидуги.
  ArcsPolyarcNullEndInfo,                                  //!< 104
  /*!<
   * Начальная точка полидуги не найдена
   * в таблице соответствий между границами и граничными точками дуг.
   */
  ArcsPolyarcBeginPointNotFound,                           //!< 105
  //!< Попытка получить ссылку на дугу из пустой полидуги.
  ArcsArcReferenceFromEmptyPolyarc,                        //!< 106

  //!< Нулевой идентификатор у базовой точки полигона.
  EnvironmentPolygonFirstPointNullId                       //!< 110
    = ArcsNotBeginTerminalPointOnBorder + 10,
  //!< Нулевое количество точек в полигоне.
  EnvironmentPolygonNoPoints,                              //!< 111
  /*!<
   * Идентификатор точки выходит за границы
   * множества допустимых идентификаторов точек полигона.
   */
  EnvironmentPolygonPointIdOutOfRange,                     //!< 112
  //!< Попытка создать полигон, имеющий менее двух вершин.
  EnvironmentPolygonTooFewPoints,                          //!< 113

  /*!<
   * Попытка добавить в среду многоугольник с внешним кольцом,
   * содержащим менее 3 вершин.
   */
  GeometryEnvironmentAddPolygonWithLessThanThreeVerticesOuterRing //!< 120
    = EnvironmentPolygonFirstPointNullId + 10,
  /*!<
   * Идентификатор точки выходит за границы
   * множества допустимых идентификаторов точек среды.
   */
  GeometryEnvironmentPointIdOutOfRange,                    //!< 121
  /*!<
   * Идентификатор полигона выходит за границы
   * множества допустимых идентификаторов полигонов среды.
   */
  GeometryEnvironmentPolygonIdOutOfRange,                  //!< 122

  /*!<
   * Создание объекта класса InfoAndQueueManager с нулевым указателем на среду.
   */
  InfoAndQueueManagerEnvironmentNullPointer                //!< 130
    = GeometryEnvironmentAddPolygonWithLessThanThreeVerticesOuterRing + 10,
  //!< Обработка результата расчёта для пустой субсреды.
  InfoAndQueueManagerApplyResultForEmptySubEnvironment,    //!< 131

  //!< Обработка задания с пустой субсредой.
  IterationRunnableRunWithEmptySubEnvironment              //!< 140
    = InfoAndQueueManagerEnvironmentNullPointer + 10,
  //!< Обработка некорректного задания.
  IterationRunnableRunInvalidTask,                         //!< 141
  //!< Обработка задания с неположительной максимальной дальностью.
  IterationRunnableRunTaskWithNonPositiveMaxPath,          //!< 142
  //!< Обработка задания с нулевым указателем на точку расчёта.
  IterationRunnableRunTaskWithCalculationPointNullPointer, //!< 143

  /*!<
   * Создание эллипса с суммой фокальных радиусов меньшей или равной
   * удвоенному фокальному расстоянию.
   */
  EllipseSubEnvironmentCutterEllipseDoesNotExist           //!< 150
    = IterationRunnableRunWithEmptySubEnvironment + 10,

  /*!<
   * Идентификатор границы выходит за пределы
   * множества допустимых идентификаторов границ набора.
   */
  PolarEdgeFunctorEdgeIdOutOfRange                         //!< 160
    = EllipseSubEnvironmentCutterEllipseDoesNotExist + 10,

  //!< Идентификатор точки в среде оказался нулевым.
  VisibilityPointNullId = PolarEdgeFunctorEdgeIdOutOfRange + 10, //!< 170
  /*!<
   * Идентификатор границы,
   * для которой предыдущая точка является концом, оказался нулевым.
   */
  VisibilityPreviousPointNullLastForId,                    //!< 171
  /*!<
   * Локальный идентификатор точки,
   * являющейся начальной для границы,
   * конечной точкой которой является предыдущая точка,
   * оказался равным нулю.
   */
  VisibilityFirstPointForEdgeWhichLastPointIsPreviousNullId, //!< 172
  /*!<
   * Скрытый идентификатор точки, предыдущей относительно точки,
   * являющейся начальной для границы,
   * конечной точкой которой является предыдущая точка,
   * оказался равным нулю.
   */
  VisibilityPreviousFirstPointForEdgeWhichLastPointIsPreviousNullId, //!< 173
  //!< Идентификатор предыдущей точки в среде оказался нулевым.
  VisibilityPreviousPointNullId,                           //!< 174
  //!< Идентификатор точки расчёта оказался нулевым.
  VisibilityCentralPointNullId,                            //!< 175
  //!< Идентификатор полигона, содержащего точку расчёта, оказался нулевым.
  VisibilityPolygonContainingCentralPointNullId,           //!< 176
  /*!<
   * Идентификаторы полигонов,
   * содержащих точки, лежащие на одной границе, не совпадают.
   */
  VisibilityPolygonsContainingPointsFromOneEdgeIdsMismatch, //!< 177
  /*!<
   * Поиск пересечения луча
   * и неправильно направленного относительно него отрезка.
   */
  VisibilityRayIntersectWronglyDirectedEdge,                //!< 178
  /*!<
   * На препятствии заканчивается только одна дуга и не начинается 
   * следующая
   */
  SingleArcEndsAtBorder,                                    //!< 179
//==============================================================================
  //! Внешние (интерфейсные) исключения
//==============================================================================
  /*!<
   * Некорректная геометрия
  */
  IncorrectGeometry = VisibilityPointNullId + 100,          //!< 200 
  /*!<
   * Дальность построения ГЗДЦ не превосходит 0
  */
  NotPositiveRange,                                         //!< 201 
  /*!<
   * Точка отсчёта ГЗДЦ попадает в препятствие
  */
  PointInRestriction                                        //!< 202 
};
//==============================================================================
// !< название библиотеки для исключений
const std::string reachableAreaLibTitle = "Core::ReachableAreaCalculator";
//==============================================================================
/*!
 * \brief Класс формирования исключений библиотеки ReachableAreaCalculator
 */
class ReachableAreaFailure : public ExceptionTools::Failure
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
explicit ReachableAreaFailure(
  const unsigned int failureId,
  const std::string& postfix = "",
  const std::string& prefix = "") noexcept;
};
//==============================================================================
} //! ReachableArea
//==============================================================================
#endif //! REACHABLE_AREA_EXCEPTION_HPP