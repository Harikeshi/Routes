#include <ReachableArea/ReachableAreaException.hpp>
//==============================================================================
namespace ReachableArea
{
//==============================================================================
/*
 *\brief Соотношение именованных кодов и текстов ошибок генерируемых исключений
 */
const std::map<unsigned int, std::string> reachableAreaFailuresText =
{
    INCLUDE_STANDART_FAILURE_IN_MAP
  {
    ArcsNotBeginTerminalPointOnBorder,
    "<ArcsNotBeginTerminalPointOnBorder>: "
    "Терминальная точка, найденная при обходе границы, "
    "не является начальной точкой полидуги"
  },
  {
    ArcsClockWiseInnerRing,
    "<ArcsClockWisePolygon>: "
    "Вершины внутреннего кольца многоугольника оказались расположенными "
    "в порядке \"по часовой стрелке\""
  },
  {
    ArcsPolyarcBreakNoByEdge,
    "<ArcsPolyarcBreakNoByEdge>: "
    "Начальная или конечная точка полидуги "
    "оказалась точкой обрыва не из-за границы"
  },
  {
    ArcsPolyarcNullBeginInfo,
    "<ArcsPolyarcNullBeginInfo>: "
    "Нулевая дополнительная информация о начале полидуги"
  },
  {
    ArcsPolyarcNullEndInfo,
    "<ArcsPolyarcNullEndInfo>: "
    "Нулевая дополнительная информация о конце полидуги"
  },
  {
    ArcsPolyarcBeginPointNotFound,
    "<ArcsPolyarcBeginPointNotFound>: "
    "Начальная точка полидуги не найдена "
    "в таблице соответствий между границами и граничными точками дуг"
  },
  {
    ArcsArcReferenceFromEmptyPolyarc,
    "<ArcsArcReferenceFromEmptyPolyarc>: "
    "Попытка получить ссылку на дугу из пустой полидуги"
  },
  {
    EnvironmentPolygonFirstPointNullId,
    "<EnvironmentPolygonFirstPointNullId>: "
    "У базовой точки многоугольника идентификатор оказался нулевым"
  },
  {
    EnvironmentPolygonNoPoints,
    "<EnvironmentPolygonNoPoints>: "
    "Количество точек в многоугольнике оказалось равным нулю"
  },
  {
    EnvironmentPolygonPointIdOutOfRange,
    "<EnvironmentPolygonPointIdOutOfRange>: "
    "Идентификатор точки выходит за границы "
    "множества допустимых значений идентификаторов точек многоугольника"
  },
  {
    EnvironmentPolygonTooFewPoints,
    "<EnvironmentPolygonTooFewPoints>: "
    "Попытка создать многоугольник, содержащий менее двух вершин"
  },
  {
    GeometryEnvironmentAddPolygonWithLessThanThreeVerticesOuterRing,
    "<GeometryEnvironmentAddPolygonWithLessThanThreeVerticesOuterRing>: "
    "Попытка добавить в среду многоугольник с внешним кольцом, "
    "содержащим менее 3 вершин"
  },
  {
    GeometryEnvironmentPointIdOutOfRange,
    "<GeometryEnvironmentPointIdOutOfRange>: "
    "Идентификатор точки выходит за границы "
    "множества допустимых значений идентификаторов точек среды"
  },
  {
    GeometryEnvironmentPolygonIdOutOfRange,
    "<GeometryEnvironmentPolygonIdOutOfRange>: "
    "Идентификатор многоугольника выходит за границы "
    "множества допустимых значений идентификаторов многоугольников среды"
  },
  {
    InfoAndQueueManagerEnvironmentNullPointer,
    "<InfoAndQueueManagerEnvironmentNullPointer>: "
    "Создание объекта класса InfoAndQueueManager с нулевым указателем на среду"
  },
  {
    InfoAndQueueManagerApplyResultForEmptySubEnvironment,
    "<InfoAndQueueManagerApplyResultForEmptySubEnvironment>: "
    "Обработка результата расчёта для пустой субсреды"
  },
  {
    IterationRunnableRunWithEmptySubEnvironment,
    "<IterationRunnableRunWithEmptySubEnvironment>: "
    "Обработка задания с пустой субсредой"
  },
  {
    IterationRunnableRunInvalidTask,
    "<IterationRunnableRunInvalidTask>: Обработка некорректного задания"
  },
  {
    IterationRunnableRunTaskWithNonPositiveMaxPath,
    "<IterationRunnableRunTaskWithNonPositiveMaxPath>: "
    "Обработка задания с неположительной максимальной дальностью"
  },
  {
    IterationRunnableRunTaskWithCalculationPointNullPointer,
    "<IterationRunnableRunTaskWithCalculationPointNullPointer>: "
    "Обработка задания с нулевым указателем на точку расчёта"
  },
  {
    EllipseSubEnvironmentCutterEllipseDoesNotExist,
    "<EllipseSubEnvironmentCutterEllipseDoesNotExist>: "
    "Создание эллипса с суммой фокальных радиусов меньшей или равной "
    "удвоенному фокальному расстоянию"
  },
  {
    PolarEdgeFunctorEdgeIdOutOfRange,
    "<PolarEdgeFunctorEdgeIdOutOfRange>: "
    "Идентификатор границы выходит за пределы "
    "множества допустимых идентификаторов границ набора"
  },
  {
    VisibilityPointNullId,
    "<VisibilityPointNullId>: "
    "Идентификатор точки в среде оказался нулевым"
  },
  {
    VisibilityPreviousPointNullLastForId,
    "<VisibilityPreviousPointNullLastForId>: "
    "Идентификатор границы, "
    "для которой предыдущая точка является концом, оказался нулевым"
  },
  {
    VisibilityFirstPointForEdgeWhichLastPointIsPreviousNullId,
    "<VisibilityFirstPointForEdgeWhichLastPointIsPreviousNullId>: "
    "Локальный идентификатор точки, "
    "являющейся начальной для границы, "
    "конечной точкой которой является предыдущая точка, "
    "оказался равным нулю"
  },
  {
    VisibilityPreviousFirstPointForEdgeWhichLastPointIsPreviousNullId,
    "<VisibilityPreviousFirstPointForEdgeWhichLastPointIsPreviousNullId>: "
    "Скрытый идентификатор точки, предыдущей относительно точки, "
    "являющейся начальной для границы, "
    "конечной точкой которой является предыдущая точка, "
    "оказался равным нулю"
  },
  {
    VisibilityPreviousPointNullId,
    "<VisibilityPreviousPointNullId>: "
    "Идентификатор предыдущей точки в среде оказался нулевым"
  },
  {
    VisibilityCentralPointNullId,
    "<VisibilityCentralPointNullId>: "
    "Идентификатор точки расчёта оказался нулевым"
  },
  {
    VisibilityPolygonContainingCentralPointNullId,
    "<VisibilityPolygonContainingCentralPointNullId>: "
    "Идентификатор многоугольника, содержащего точку расчёта, оказался нулевым"
  },
  {
    VisibilityPolygonsContainingPointsFromOneEdgeIdsMismatch,
    "<VisibilityPolygonsContainingPointsFromOneEdgeIdsMismatch>: "
    "Идентификаторы многоугольников, "
    "содержащих точки, лежащие на одной границе, не совпадают"
  },
  {
    VisibilityRayIntersectWronglyDirectedEdge,
    "<VisibilityRayIntersectWronglyDirectedEdge>: "
    "Поиск пересечения луча "
    "и неправильно направленного относительно него отрезка"
  },
  {
    SingleArcEndsAtBorder, 
    "<SingleArcEndsAtBorder>: "
    "На препятствии заканчивается только одна дуга и не начинается  следующая"
  },
  {
    IncorrectGeometry,
    "<ReachableArea: IncorrectGeometry>: "
      "Некорректная геометрия."
  },
  {
    NotPositiveRange,
    "<ReachableArea: NotPositiveRange>: "
      "Дальность построения ГЗДЦ не превосходит 0."
  },
  {
    PointInRestriction,
    "<ReachableArea: PointInRestriction>: "
    "Точка отсчёта ГЗДЦ попадает в препятствие."
  }
};
//==============================================================================
ReachableAreaFailure::ReachableAreaFailure(
  const unsigned int failureId,
  const std::string& postfix /*= ""*/,
  const std::string& prefix /*= ""*/) noexcept
  : Failure(
    failureId,
    reachableAreaLibTitle,
    reachableAreaFailuresText.at(failureId),
    postfix,
    prefix)
{
}
//==============================================================================
} //! ReachableArea
//==============================================================================
