#ifndef REACHABLEAREACALCULATOR_INNER_VISIBILITY_H
#define REACHABLEAREACALCULATOR_INNER_VISIBILITY_H
//==============================================================================
//! Inner
#include <Inner/GeometryEnvironment.hpp>
#include <Inner/SubEnvironment.hpp>
#include <Inner/Arcs.hpp>
//! Point
#ifndef Q_MOC_RUN
#include <Point/Point2D.hpp>
#endif
//==============================================================================
namespace ReachableAreaCalculator
{
//==============================================================================
/*
* \brief Точка с полярными координатами
*/
class PolarPoint : public Point2D
{
//==============================================================================
public:
//==============================================================================
  /*
  * \brief Конструктор
  */
  PolarPoint()
  { }
//==============================================================================
  /*
  * \brief Конструктор
  * \param[in] point  - координаты точки в декартовой системе координат;
  * \param[in] origin - центр полярной системы координат;
  * \param[in] envId  - идентификатор точки в среде.
  */
  PolarPoint(const Point2D& point, const Point2D& origin, uinttype envId);
//==============================================================================
  /*
  * \brief Конструктор
  * \param[in] radius  - радиус;
  * \param[in] azimuth - угол;
  * \param[in] origin  - центр полярной системы координат;
  * \param[in] envId   - идентификатор точки в среде.
  */
  PolarPoint(double radius, double azimuth,
             const Point2D& origin, uinttype envId);
//==============================================================================
  /*
  * \brief Задание значения полярного угла у точки с радиусом == 0
  * \details Если радиус != 0, возвращает false без изменения объекта.
  * \param[in] azimuth - угол;
  * \return true  - угол задан;
  *         false - радиус не нулевой, значение угла не задано.
  */
  bool setAzimuth(const Angle & azimuth);
//==============================================================================
  /*
  * \brief Задание локального идентификатора
  * \param[in] id - локальный идентификатор.
  */
  void setId(uinttype id) {mId = id;}
//==============================================================================
  /*
  * \brief Получение радиуса точки
  * \return радиус точки
  */
  double radius() const {return mRadius;}
//==============================================================================
  /*
  * \brief Получение угла точки
  * \return угол точки
  */
  Angle azimuth() const {return mAzimuth;}
//==============================================================================
  /*
  * \brief Получение идентификатора точки в среде
  * \return идентификатор точки в среде
  */
  uinttype envId() const {return mEnvId;}
//==============================================================================
  /*
  * \brief Получение скрытого идентификатора в среде
  * \details Используется, если граница была подрезана. Соответствует
  * \        идентификатору точки до подрезки при рассмотрении подрезки
  * \        границы как перемещения одной из её граничных точек.
  * \return скрытый идентификатор в среде
  */
  uinttype hiddenEnvId() const {return mHiddenEnvId;}
//==============================================================================
  /*
  * \brief Задание скрытого идентификатора
  * \param[in] id - скрытый идентификатор.
  */
  void setHiddenEnvId(uinttype heid) {mHiddenEnvId = heid;}
//==============================================================================
  /*
  * \brief Получение локального идентификатора
  * \return локальный идентификатор
  */
  uinttype id() const {return mId;}
//==============================================================================
  uinttype firstForId = 0; //! id границы, для которой является началом;
  uinttype lastForId = 0;  //! id границы, для которой является концом;
//==============================================================================
private:
//==============================================================================
  double   mRadius = 0;      //! радиус точки;
  Angle    mAzimuth = 0;     //! азимут точки;
  uinttype mEnvId = 0;       //! идентификатор точки в среде;
  uinttype mHiddenEnvId = 0; //! идентификатор точки в среде в случае
                             //! её перемещения (при подрезке кругом);
  uinttype mId = 0;          //! локальный идентификатор.
};
//==============================================================================
/*
* \brief Функтор сравнения границ
*/
class PolarEdgeFunctor
{
//==============================================================================
public:
//==============================================================================
  /*
  * \brief Конструктор
  * \param[in] polarPoints - точки в полярной системе координат;
  * \param[in] polarEdges  - границы в полярной системе координат.
  */
  PolarEdgeFunctor(
    const std::vector<PolarPoint>& polarPoints,
    const std::vector<GeometrySubEnvironment::Edge>& polarEdges)
    : mPolarPoints(polarPoints)
    , mPolarEdges(polarEdges)
  { }
//==============================================================================
  /*!<
   * \brief Проверка перекрытия границ
   * \details Требует, чтобы одна граница перекрывала другую. 
   *          Подразумевается, что сравниваться могут только границы
   *          с правильным направлением (при заметании сначала следует
   *          начальная точка, потом конечная).
   * \param[in] e1id - идентификатор первой границы;
   * \param[in] e2id - идентификатор второй границы;
   * \return true  - первая граница перекрывается второй;
   *         false - во всех остальных случаях.
   * \throws ReachableAreaFailure # PolarEdgeFunctorEdgeIdOutOfRange
   */
  bool operator () (uinttype e1id, uinttype e2id) const;
//==============================================================================
private:
//==============================================================================
  const std::vector<PolarPoint>& mPolarPoints; //! набор точек в полярной
                                               //! системе координат
  const std::vector<GeometrySubEnvironment::Edge>& 
                                 mPolarEdges;  //! набор границ в полярной
                                               //! системе координат
};
//==============================================================================
//==============================================================================
/*
* \brief Расчёт видимости и заведомой достижимости
*/
class Visibility
{
//==============================================================================
public:
//==============================================================================
  /*
  * \brief Результат расчёта
  */
  struct ProcessResult
  {
    Polygon2D        area;      //! полигон видимости;
    std::vector<uinttype>
      refractionPoints;         //! id точек преломления;
    std::vector<uinttype>
      absorptionPoints;         //! id точек поглощения;
    std::vector<std::pair<uinttype, double>>
      unvisibleReachablePoints; //! id невидимых заведомо
                                //! достижимых точек с 
                                //! верхними оценками путей;
    std::vector<Arc> arcs;      //! набор дуг, ограничивающий 
                                //! область видимости.
  };
//==============================================================================
  /*
  * \brief Флаги режима расчёта
  */
  enum ProcessModeFlag
  {
    AddUnvisible = 1,        //! проводить быструю оценку
                             //! достижимости невидимых точек.
                             //! Возвращать в структуре результата поля;
    ResArea = 1 << 1,        //! area;
    ResRefractions = 1 << 2, //! refractionPoints;
    ResAbsorptions = 1 << 3, //! absorptionPoints;
    ResUnvisibles = 1 << 4,  //! unvisibleReachablePoints;
    ResArcs = 1 << 5         //! arcs.
  };
//==============================================================================
  typedef unsigned int ProcessMode; //! Режим расчёта
//==============================================================================
  /*!<
  * \brief process - проведение расчёта
  * \param[in]  sub            - суб-среда расчёта;
  * \param[in]  center         - точка расчёта;
  * \param[in]  maxPath        - ограничение по дальности;
  * \param[in]  viewZoneBegin  - начало зоны обзора;
  * \param[in]  viewZoneEnd    - конец зоны обзора;
  * \param[in]  mode           - режим расчёта (набор флагов);
  * \param[out] result         - приемник результата;
  * \param[in]  segmentsNumber - количество сегментов, для представления
  *                              окружности.
  * \throws ReachableAreaFailure # VisibilityPointNullId
  *         ReachableAreaFailure # VisibilityPreviousPointNullLastForId
  *         ReachableAreaFailure #
  *           VisibilityFirstPointForEdgeWhichLastPointIsPreviousNullId
  *         ReachableAreaFailure #
  *           VisibilityPreviousFirstPointForEdgeWhichLastPointIsPreviousNullId
  *         ReachableAreaFailure # VisibilityPreviousPointNullId
  */
  static void process(
    const GeometrySubEnvironment* sub,
    const Point2D* center,
    double maxPath,
    Angle viewZoneBegin,
    Angle viewZoneEnd,
    ProcessMode mode,
    ProcessResult& result,
    uinttype segmentsNumber);
//==============================================================================
private:
//==============================================================================
  /*!<
   * \brief createCalculationEnvironment - создание среды для расчёта
   * \details Добавляются две служебные точки, ограничивающие зону
   *          видимости, и возвращаются их локальные идентификаторы
   *          viewZone[Begin,End].PointId.
   * \param[in]  sub                  - суб-среда расчёта;
   * \param[in]  center               - точка расчёта;
   * \param[in]  radius               - ограничение по дальности;
   * \param[in]  viewZoneBegin        - начало зоны обзора;
   * \param[in]  viewZoneSize         - конец зоны обзора;
   * \param[out] polarPoints          - точки среды в полярной СК;
   * \param[out] polarEdges           - границы среды в полярной СК;
   * \param[out] viewZoneBeginPointId - локальный идентификатор точки для
   *                                    viewZoneBegin;
   * \param[out] viewZoneEndPointId   - локальный идентификатор точки для
   *                                    viewZoneEnd.
   * \throws ReachableAreaFailure #
   *           VisibilityPolygonContainingCentralPointNullId
   *         ReachableAreaFailure # VisibilityCentralPointNullId
   *         ReachableAreaFailure # VisibilityPointNullId
   *         ReachableAreaFailure #
   *           VisibilityPolygonsContainingPointsFromOneEdgeIdsMismatch
   */
  static void createCalculationEnvironment(
    const GeometrySubEnvironment* sub,
    const Point2D* center,
    double radius,
    Angle viewZoneBegin,
    double viewZoneSize,
    std::vector<PolarPoint>& polarPoints,
    std::vector<GeometrySubEnvironment::Edge>& polarEdges,
    uinttype& viewZoneBeginPointId,
    uinttype& viewZoneEndPointId);
//==============================================================================
  /*
  * \brief Заполнение диаграммы заметания
  * \param[in]  polarPoints     - точки среды в полярной СК;
  * \param[in]  polarEdges      - границы среды в полярной СК;
  * \param[out] polarPointsSort - отсортированные по возрастанию угла точки;
  * \param[out] sweepDiagram    - диаграмма заметания.
  */
  static void createSweepDiagram(
    const std::vector<PolarPoint>& polarPoints,
    const std::vector<GeometrySubEnvironment::Edge>& polarEdges,
    std::vector<const PolarPoint*>& polarPointsSort,
    std::vector<uinttype>& sweepDiagram);
//==============================================================================
  /*
  * \brief Определение пересечения границей граничной окружности суб-среды
  * \details При возвращении false значения p1 и p2 на выходе не определены.
  * \        При возвращении true p1 и p2 изменяются таким образом, что
  * \        граница полностью лежит в суб-среде. envId измененных точек
  * \        устанавливается в 0 (некорректное), при этом исходное
  * \        сохраняется в hiddenEnvId.
  * \param[in/out]  p1     - первая точка границы;
  * \param[in/out]  p2     - вторая точка границы;
  * \param[in]  center - центр окружности;
  * \param[in]  radius - радиус окружности;
  * \return true  - граница пересекает окружность границы суб-среды;
  * \       false - не пересекает.
  */
  static bool edgeIntersectBorderCircle(
    PolarPoint& p1, PolarPoint& p2, const Point2D& center, double radius);
//==============================================================================
  /*!<
   * \brief rayIntersectEdge - нахождение точки пересечения отрезка с лучом
   * \details Требует, чтобы полярный угол p1 был меньше полярного угла p2
   *          (отрезок направлен правильно) и пересечение гарантированно
   *          имелось.
   * \param[in]  p1     - первая точка отрезка;
   * \param[in]  p2     - вторая точка отрезка;
   * \param[in]  center - центр суб-среды (начало луча);
   * \param[in]  azimuth - направление луча;
   * \return точка пересечения.
   * \throw ReachableAreaFailure # VisibilityRayIntersectWronglyDirectedEdge
   */
  static Point2D rayIntersectEdge(
    const PolarPoint& p1, const PolarPoint& p2,
    const Point2D& center, const Angle& azimuth);
//==============================================================================
  /*
  * \brief Получение заведомо досягаемых точек
  * \details unvisibleReachablePoints заполняется только в случае true.
  * \param[in]  sub                      - суб-среда;
  * \param[in]  center                   - центр;
  * \param[in]  radius                   - радиус;
  * \param[in]  p1                       - точка начала рассматриваемой 
  * \                                      области (при следовании по 
  * \                                      полигону);
  * \param[in]  p2                       - точка окончания рассматриваемой 
  * \                                      области (при следовании по 
  * \                                      полигону);
  * \param[in]  intersect                - точка пересечения луча, 
  * \                                      исходящего из центра в сторону 
  * \                                      точки преломления, с границей, в 
  * \                                      которую луч упёрся;
  * \param[in]  p1IsRef                  - признак, которая из точек 
  * \                                      является точкой преломления 
  * \                                      (p1 если p1IsRef == true, иначе p2);
  * \param[out] unvisibleReachablePoints - набор глобальных идентификаторов
  * \                                      достижимых точек с соответствующими
  * \                                      длинами (путями от центра до точки);
  * \return true  - все точки между p1 и p2 при следовании по полигону
  * \               являются заведомо досягаемыми;
  * \       false - не все точки между p1 и p2 при следовании по полигону
  * \               являются заведомо досягаемыми.
  */
  static bool addUnvisibleArea(
    const GeometrySubEnvironment* sub,
    const Point2D& center, double radius,
    const PolarPoint& p1, const PolarPoint& p2,
    const Point2D& intersect, bool p1IsRef,
    std::vector<std::pair<uinttype, double> >& unvisibleReachablePoints);

};
//==============================================================================
} //! ReachableAreaCalculator
//==============================================================================
#endif // REACHABLEAREACALCULATOR_INNER_VISIBILITY_H
