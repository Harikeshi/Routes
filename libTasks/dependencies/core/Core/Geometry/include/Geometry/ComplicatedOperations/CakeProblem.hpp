#ifndef GEOMETRY_COMPLICATED_OPERATIONS_CAKE_PROBLEM_HPP
#define GEOMETRY_COMPLICATED_OPERATIONS_CAKE_PROBLEM_HPP
//==============================================================================
#include <Geometry/Objects/Sector.hpp>
//! PrimaryEntities
#include <PrimaryEntities/GeometryEntities.hpp>
#include <PrimaryEntities/GeodesicEntities.hpp>
//==============================================================================
namespace Geometry
{
//==============================================================================
  struct SectorPoly
  {
    Point2D rightPoint;        //!< правая точка сектора
    Point2D leftPoint;         //!< левая точка сектора
    Polygon2D poly;            //!< полигон сектора
    bool isFullSector = false; //!< флаг полного угла сектора

    // Точки, полученные в результате продления отрезков, соединяющих центр
    // окружности и граничные точки сектора, за пределы окружности
    Point2D externalRightPoint;
    Point2D externalLeftPoint;

    SectorPoly(const Point2D& rPoint, const Point2D& lPoint)
      : rightPoint(rPoint), leftPoint(lPoint)
    { }
  };
//==============================================================================
  /*!
  * \brief getPolyCircle Получение окружности на плоскости в
  * /                    виде полигона по центру и радиусу
  * \param[in] center     - центр окружности
  * \param[in] radius     - радиус окружности
  * \return - окружность в виде полигона (в случае равенства радиуса
  * \         нулю возвращается точка центра)
  *\throws GeometryFailure # NegativeRadius
  *\throws GeometryFailure # OperationNotPerformedCorrect
  */
  Polygon2D getPolyCircle(
    const Point2D& center,
    const double radius);
//==============================================================================
  /*!
  * \brief findPolygonLeftAndRightPoints Поиск всех правых и левых точек 
  * \                                    полигона
  * \details Правой будем называть точку, для которой и следующая,
  * \        и предыдущая точки лежат слева. Для левой, соответственно,
  * \        справа. Возвращаются не сами точки, а их индексы в полигоне
  * \param[in] polygon       - полигон
  * \param[in] center        - точка обзора, по которой определяется 
  * \                          положение точек полигона относительно 
  * \                          друг друга
  * \param[out] iLeftPoints  - индексы левых точек
  * \param[out] iRightPoints - индексы правых точек
  *\throws GeometryFailure # EmptyPolygon
  */
  void findPolygonLeftAndRightPoints(
    const Polygon2D& polygon,
    const Point2D& center,
    std::vector<size_t>& iLeftPoints,
    std::vector<size_t>& iRightPoints
    );
//==============================================================================
  /*!
  * \brief findPolygonBorderedPoint Поиск граничной точки полигона
  * \details Точка выбирается по списку индексов всех подходящих точек
  * \        в полигоне. Если нужно найти крайнюю правую точку, 
  * \        в iBorderedPoints подаются все возможные правые точки,
  * \        для левой - все возможные левые.
  * \param[in] polygon         - полигон
  * \param[in] center          - точка обзора, по которой определяется
  * \                            положение точек полигона относительно
  * \                            друг друга
  * \param[in] iBorderedPoints - индексы граничных точек (должны
  * \                            быть найдены заранее)
  * \param[in] isRight         - если true, нужно найти крайнюю правую 
  * \                            точку, иначе - левую
  * \return - индекс граничной точки полигона
  *\throws GeometryFailure # EmptyPolygon
  *\throws GeometryFailure # NoBorderedPointsInPolygon
  *\throws GeometryFailure # PolygonIndexIsOutOfRange
  */
  size_t findPolygonBorderedPoint(
    const Polygon2D& polygon,
    const Point2D& center,
    const std::vector<size_t>& iBorderedPoints,
    bool isRight
    );
//==============================================================================
  /*!
  * \brief getSectorPolygon Построение сектора в виде полигона
  * \details Сектор стоится по граничным точкам полигона. С внутренней
  * \        стороны от этих точек (ближайшей к центру области) сектор
  * \        обрезается по контуру полигона, с внешней замыкается в
  * \        зависимости от угла. Если угол < 180, обрезается по
  * \        треугольнику с высотой, равной радиусу. Если угол = 180,
  * \        достраивается прямоугольник со сторонами, равными 2*R и R
  * \        (границы сектора продлеваются до величины радиуса, а
  * \        затем перпендикулярно этим границам на величину радиуса).
  * \        Если угол > 180, границы сектора продлеваются до величины
  * \        радиуса, а затем перпендикулярно этим границам так, чтобы
  * \        чтобы полностью покрыть окружность со стороны полигона.
  * \param[in] polygon    - полигон
  * \param[in] center     - центр окружности
  * \param[in] radius     - радиус окружности
  * \param[in] iLeft      - индекс крайней левой точки полигона
  * \param[in] iRight     - индекс крайней правой точки полигона
  * \param[in|out] sector - сектор, полигон которого нужно построить
  *\throws GeometryFailure # EmptyPolygon
  *\throws GeometryFailure # NegativeRadius
  *\throws GeometryFailure # OperationNotPerformedCorrect
  */
  void getSectorPolygon(
    const Polygon2D& polygon,
    const Point2D& center,
    const double radius,
    size_t iLeft,
    size_t iRight,
    SectorPoly& sector
    );
//==============================================================================
  /*!
  * \brief findExpansionAngle Поиск угла, на который нужно расширить
  * \                         сектор относительно предыдущего расширения
  * \param[in] center            - центр окружности
  * \param[in] polyPoint         - точка полигона, расширение которой
  * \                              проверяем
  * \param[in] curExpansionPoint - точка текущего расширения
  * \param[in] widthExpansion    - ширина расширения
  * \param[in] isClockwise       - если true - положительный угол по часовой
  * \                              стрелке, иначе - против
  * \return - угол расширения
  */
  Radian findExpansionAngle(
    const Point2D& center,
    const Point2D& polyPoint,
    const Point2D& curExpansionPoint,
    double widthExpansion,
    bool isClockwise);
//==============================================================================
  /*!
  * \brief expandOneSectorsBorder Расширение сектора в одном направлении
  * \details Граничные точки сектора не изменяются.
  * \param[in/out] sector        - расширяемый сектор
  * \param[in] center            - центр окружности
  * \param[in] radius            - радиус окружности
  * \param[in] widthExpansion    - ширина расширения
  * \param[in] problemZone       - зона, в которой расширение может
  * \                              оказаться недостаточным
  * \param[in] pointNearBorder   - точка прямоугольника, диагональ
  * \                              которого соединяет центр и граничную
  * \                              точку: данная точка находится рядом с
  * \                              граничной точкой сектора (через неё 
  * \                              проходит расширение)
  * \param[in] isRightExpansion  - если true - расширить нужно правую
  * \                              границу сектора, иначе левую
  *\throws GeometryFailure # EmptySectorPolygon
  *\throws GeometryFailure # NegativeRadius
  *\throws GeometryFailure # NegativeOrZeroWidthExpansion
  *\throws GeometryFailure # DivisionByZero
  *\throws GeometryFailure # OperationNotPerformedCorrect
  */
  void expandOneSectorsBorder(
    SectorPoly& sector, const Point2D& center, double radius, double widthExpansion,
    const MultiPolygon2D& problemZone, const Point2D& pointNearBorder,
    bool isRightExpansion);
//==============================================================================
  /*!
  * \brief expandSectorsBorders Расширение сектора с двух сторон
  * \details Граничные точки сектора не изменяются.
  * \param[in/out] sector     - расширяемый сектор
  * \param[in] center         - центр окружности
  * \param[in] radius         - радиус окружности
  * \param[in] widthExpansion - ширина расширения
  *\throws GeometryFailure # EmptySectorPolygon
  *\throws GeometryFailure # NegativeRadius
  */
  void expandSectorsBorders(
    SectorPoly& sector, const Point2D& center, double radius, double widthExpansion);
//==============================================================================
  /*!
  * \brief getSectorInPoints Получение секторов в виде граничных точек
  * \                        полигонов
  * \param[in] polygons       - полигоны
  * \param[in] center         - точка обзора, по которой определяется
  * \                           положение точек полигона относительно
  * \                           друг друга
  * \param[in] findPolygons   - если true - для полигонов также собираются
  * \                           сектора, ограничивающие их внутри окружности
  * \param[in] radius         - радиус окружности (нужен, только при
  * \                           построении полигонов)
  * \return - сектора
  */
  std::vector<SectorPoly> getSectorsInPoints(
    const MultiPolygon2D& polygons,
    const Point2D& center,
    bool findPolygons,
    double radius = 0);
//==============================================================================
  /*!
  * \brief calculateFlightCircle Построение запретных областей для
  * \                            участка, ограниченного окружностью
  * \param[in] area           - область с вырезанными запретными зонами
  * \param[in] center         - центр окружности, ограничивающей участок
  * \param[in] radius         - радиус окружности, ограничивающей участок
  * \param[in] widthExpansion - ширина расширения секторов
  * \return - запретные области для участка, ограниченного окружностью
  * \         (если вернулся пустой мультиполигон - запретных областей 
  * \          нет)
  *\throws GeometryFailure # EmptyPolygon
  *\throws GeometryFailure # NegativeRadius
  *\throws GeometryFailure # IncorrectGeometry
  */
  MultiPolygon2D calculateCake(
    const MultiPolygon2D& area,
    const Point2D& center,
    const double radius,
    const double widthExpansion
    );
//==============================================================================
} //! Geometry
//==============================================================================
#endif //! GEOMETRY_COMPLICATED_OPERATIONS_CAKE_PROBLEM_HPP