#ifndef REACHABLEAREACALCULATOR_INNER_GEOMETRYENVIRONMENT_H
#define REACHABLEAREACALCULATOR_INNER_GEOMETRYENVIRONMENT_H
//==============================================================================
#ifndef Q_MOC_RUN
//! Point
#include <Point/Point2D.hpp>
//! PrimaryEntities
#include <PrimaryEntities/GeometryEntities.hpp>
//! Geometry
#include <Geometry/Operations/AtPoints2D.hpp>
#endif
//==============================================================================
namespace ReachableAreaCalculator
{
//==============================================================================
typedef unsigned int uinttype; //! целочисленный беззнаковый тип
//==============================================================================
class GeometryEnvironment;
//==============================================================================
//==============================================================================
/*
* \brief Объект, принадлежащей среде
*/
class EnvironmentObject
{
//==============================================================================
public:
//==============================================================================
  /*
  * \brief  Конструктор
  * \param[in] parent - среда.
  */
  EnvironmentObject(const GeometryEnvironment* parent = nullptr) 
    : mParent(parent)
  { }
//==============================================================================
  /*
  * \brief  Деструктор
  */
  virtual ~EnvironmentObject(){}
//==============================================================================
  /*
  * \brief Получение среды
  * \return среда
  */
  const GeometryEnvironment* parentEnvironment() const
  {
    return mParent;
  }
//==============================================================================
private:
//==============================================================================
  const GeometryEnvironment* mParent; //! среда
};
//==============================================================================
//==============================================================================
/*
* \brief Точка в среде
*/
class EnvironmentPoint : public EnvironmentObject, public Point2D
{
//==============================================================================
public:
//==============================================================================
  /*
  * \brief  Конструктор
  */
  EnvironmentPoint()
  { }
//==============================================================================
  /*
  * \brief Конструктор
  * \param[in] parent    - среда, которой точка принадлежит;
  * \param[in] id        - идентификатор;
  * \param[in] polygonId - идентификатор полигона, которому 
  * \                      принадлежит точка;
  * \param[in] point     - геометрические данные.
  */
  EnvironmentPoint(
    const GeometryEnvironment* parent,
    uinttype id,
    uinttype polygonId,
    const Point2D& point) 
    : EnvironmentObject(parent)
    , Point2D(point)
    , mId(id)
    , mPolygonId(polygonId)
  { }
//==============================================================================
  /*
  * \brief Конструктор
  * \param[in] point - геометрические данные.
  */
  EnvironmentPoint(const Point2D& point) 
    : EnvironmentObject(nullptr)
    , Point2D(point)
    , mId(0)
    , mPolygonId(0)
  { }
//==============================================================================
  /*
  * \brief Получение идентификатора точки в среде
  * \return идентификатор точки в среде
  */
  uinttype id() const
  {
    return mId;
  }
//==============================================================================
  /*
  * \brief Получение идентификатора полигона, которому точка принадлежит
  * \return идентификатор полигона, которому точка принадлежит
  */
  uinttype polygonId() const
  {
    return mPolygonId;
  }
//==============================================================================
  /*!
   * \brief Сравнение двух точек.
   * \param[in] p -- точка, с которой необходимо провести сравнение.
   * \return true, если точки не равны;
   *         false, в противном случае.
   */
  inline bool operator != (const EnvironmentPoint& p) const noexcept
  {
    return !(*this == p);
  }
//==============================================================================
private:
//==============================================================================
  uinttype mId = 0;        //! идентификатор. Корректные значения > 0
  uinttype mPolygonId = 0; //! id полигона. Корректные значения > 0
//==============================================================================
};
//==============================================================================
//==============================================================================
/*
* \brief Полигон в среде
*/
class EnvironmentPolygon : public EnvironmentObject
{
//==============================================================================
  friend class GeometryEnvironment;
//==============================================================================
public:
//==============================================================================
  /*
  * \brief Получение идентификатора
  * \details Корректные значения > 0
  * \return идентификатор полигона
  */
  uinttype id() const
  {
    return mId;
  }
//==============================================================================
  /*!<
   * \brief firstPoint -- получение первой (базовой) точки полигона
   * \return первая (базовая) точка полигона
   * \throws ReachableAreaFailure # EnvironmentPolygonFirstPointNullId
   *         ReachableAreaFailure # EnvironmentPolygonNoPoints
   */
  const EnvironmentPoint& firstPoint() const;
//==============================================================================
  /*
  * \brief Получение идентификатора первой (базовой) точки полигона
  * \return идентификатор первой (базовой) точки полигона
  */
  uinttype firstPointId() const
  {
    return mFirstPointId;
  }
//==============================================================================
  /*!<
   * \brief prevPointId - получение идентификатора точки, предыдущей для точки с
   *        идентификатором, переданным в аргументе, по направлению обхода.
   * \param[in] id - идентификатор точки;
   * \return идентификатор предыдущей точки
   * \throws ReachableAreaFailure # EnvironmentPolygonPointIdOutOfRange
   */
  uinttype prevPointId(uinttype id) const;
//==============================================================================
  /*!<
   * \brief nextPointId - получение идентификатора точки,
   *        следующей после точки с идентификатором,
   *        переданным в аргументе, по направлению обхода.
   * \param[in] id - идентификатор точки;
   * \return идентификатор следующей точки
   * \throws ReachableAreaFailure # EnvironmentPolygonPointIdOutOfRange
   */
  uinttype nextPointId(uinttype id) const;
//==============================================================================
  /*
  * \brief Получение периметра
  * \return периметр
  */
  double perimeter() const {return mLengths[0];}
//==============================================================================
  /*
  * \brief Получение коробки
  * \return коробка
  */
  const Box2D& box() const
  {
    return mBox;
  }
//==============================================================================
  /*
  * \brief Получение количества точек в полигоне
  * \return количество точек в полигоне
  */
  uinttype pointsCount() const {return mSize;}
//==============================================================================
  /*!<
   * \brief distOnBorder - получение расстояния по границе между точками
   *        при движении по направлению обхода.
   * \details Возвращает std::numeric_limits<double>::infinity() при 
   *          несоответствии у точек идентификаторов полигонов.
   * \param[in] p1 - первая точка;
   * \param[in] p2 - вторая точка;
   * \return расстояние между точками
   * \throws ReachableAreaFailure # EnvironmentPolygonPointIdOutOfRange
   */
  double distOnBorder(const EnvironmentPoint& p1,
                      const EnvironmentPoint& p2) const;
//==============================================================================
  /*!
   * \brief Сравнение двух полигонов.
   * \param[in] p -- полигон, с которым необходимо провести сравнение.
   * \return true, если полигоны равны;
   *         false, в противном случае.
   */
  bool operator == (const EnvironmentPolygon& p) const noexcept;
//==============================================================================
  /*!
   * \brief Сравнение двух полигонов.
   * \param[in] p -- полигон, с которым необходимо провести сравнение.
   * \return true, если полигоны не равны;
   *         false, в противном случае.
   */
  inline bool operator != (const EnvironmentPolygon& p) const noexcept
  {
    return !(*this == p);
  }
//==============================================================================
private:
//==============================================================================
  uinttype            mId;           //! идентификатор полигона;
  uinttype            mFirstPointId; //! идентификатор первой точки;
  uinttype            mSize;         //! количество точек в полигоне;
  std::vector<double> mLengths;      //! расстояния от базовой точки до 
                                     //! точек при следовании по 
                                     //! направлению обхода. Элемент [0] 
                                     //! содержит значение периметра,
                                     //! элемент [i] - значение для точки
                                     //! с id = _firstPointid + i;
  Box2D               mBox;          //! описанная "коробка" полигона.
//==============================================================================
  /*!<
   * \brief Конструктор
   * \param[in] parent - среда;
   * \param[in] id     - присваеваемый полигону идентификатор;
   * \param[in] first  - начало вектора, содержащего точки полигона;
   * \param[in] end    - конец вектора, содержащего точки полигона.
   * \throws ReachableAreaFailure # EnvironmentPolygonTooFewPoints
   */
  EnvironmentPolygon(const GeometryEnvironment* parent,uinttype id,
                     std::vector<EnvironmentPoint>::iterator first,
                     std::vector<EnvironmentPoint>::iterator end);
//==============================================================================
};
//==============================================================================
//==============================================================================
/*
* \brief Среда
*/
class GeometryEnvironment
{
//==============================================================================
public:
//==============================================================================
  /*
  * \brief Конструктор
  */
  GeometryEnvironment()
  { }
//==============================================================================
  /*
  * \brief Конструктор
  * \param[in] polygons - набор полигонов.
  */
  GeometryEnvironment(const std::vector<Polygon2D>& polygons);
//==============================================================================
  /*!<
   * \brief addPolygon - добавление полигона в среду
   * \param[in] polygon - полигон.
   * \throws ReachableAreFailure #
   *           GeometryEnvironmentAddPolygonWithLessThanThreeVerticesOuterRing
   */
  void addPolygon(const Polygon2D& polygon);
//==============================================================================
  /*
  * \brief Получение количества точек в среде
  * \return количество точек в среде
  */
  uinttype pointsCount() const
  {
    return uinttype(mPoints.size());
  }
//==============================================================================
  /*
  * \brief Получение количества полигонов в среде
  * \return количество полигонов в среде
  */
  uinttype polygonsCount() const
  {
    return uinttype(mPolygons.size());
  }
//==============================================================================
  /*!<
   * \brief getPoint - получение точки по id
   * \param[in] id - идентификатор точки.
   * \return точка
   * \throws ReachableAreaFailure # GeometryEnvironmentPointIdOutOfRange
   */
  const EnvironmentPoint& getPoint(uinttype id) const;
//==============================================================================
  /*!<
   * \brief getPolygon - получение полигона по id
   * \param[in] id - идентификатор полигона.
   * \return полигон
   * \throws ReachableAreaFailure # GeometryEnvironmentPolygonIdOutOfRange
   */
  const EnvironmentPolygon& getPolygon(uinttype id) const;
  //==============================================================================
  /*!
  * \brief Сравнение двух сред.
  * \param[in] p -- среда, с которой необходимо провести сравнение.
  * \return true, если среды равны;
  *         false, в противном случае.
  */
  bool operator == (const GeometryEnvironment& p) const noexcept;
  //==============================================================================
  /*!
   * \brief Сравнение двух сред.
   * \param[in] p -- среда, с которой необходимо провести сравнение.
   * \return true, если среды не равны;
   *         false, в противном случае.
   */
  inline bool operator != (const GeometryEnvironment& p) const noexcept
  {
    return !(*this == p);
  }
//==============================================================================
private:
//==============================================================================
  std::vector<EnvironmentPoint>   mPoints;   //! все точки среды
  std::vector<EnvironmentPolygon> mPolygons; //! все полигоны среды
//==============================================================================
};
//==============================================================================
} //! ReachableAreaCalculator
//==============================================================================
#endif // REACHABLEAREACALCULATOR_INNER_GEOMETRYENVIRONMENT_H
