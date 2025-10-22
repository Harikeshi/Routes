#include <Inner/Arcs.hpp>

#include <ReachableArea/ReachableAreaException.hpp>
//! gtest
#include <gtest/gtest.h>
//==============================================================================
namespace ReachableAreaCalculator
{
namespace Tests
{
//==============================================================================
class ArcPrivate : public Arc
{
  friend class ArcsTest;
  FRIEND_TEST(ArcsTest, ArcDefaultConstructor);
  FRIEND_TEST(ArcsTest, ArcConstructor);
  FRIEND_TEST(ArcsTest, ArcSetters);
public:
  ArcPrivate() : Arc()
  {}
  ArcPrivate(
    const Point2D& center,
    uinttype centerId,
    double radius,
    const Angle& beginAngle,
    const Angle& endAngle,
    bool isEmpty) : Arc(center, centerId, radius, beginAngle, endAngle, isEmpty)
  {}
  ArcPrivate(
    const Point2D& center,
    uinttype centerId,
    double radius,
    const Angle& beginAngle,
    const Angle& endAngle) : Arc(center, centerId, radius, beginAngle, endAngle)
  {}
};
//==============================================================================
class PolyarcPrivate : public Polyarc
{
  friend class ArcsTest;
  FRIEND_TEST(ArcsTest, PolyarcConstructors);
  FRIEND_TEST(ArcsTest, PolyarcBeginArc);
  FRIEND_TEST(ArcsTest, PolyarcEndArc);
  FRIEND_TEST(ArcsTest, PolyarcAddToBegin);
  FRIEND_TEST(ArcsTest, PolyarcAddToEnd);
  FRIEND_TEST(ArcsTest, PolyarcAddToEndWithoutCheck);
  FRIEND_TEST(ArcsTest, PolyarcGetters);
public:
  PolyarcPrivate() : Polyarc()
  {}
  PolyarcPrivate(const Arc& arc) : Polyarc(arc)
  {}
  PolyarcPrivate(const std::deque<Arc>& arcs) : Polyarc(arcs)
  {}
};
//==============================================================================
class SemiarcContourPrivate : public SemiarcContour
{
  friend class ArcsTest;
  FRIEND_TEST(ArcsTest, SemiarcContourDefaultConstructor);
  FRIEND_TEST(ArcsTest, SemiarcContourPush);
public:
  SemiarcContourPrivate() : SemiarcContour()
  {}
};
//==============================================================================
class ArcManagerPrivate : public ArcManager
{
  friend class ArcsTest;
  FRIEND_TEST(ArcsTest, ArcManagerConstructor);
  FRIEND_TEST(ArcsTest, ArcManagerPush);
  FRIEND_TEST(ArcsTest, ArcManagerMerge);
  FRIEND_TEST(ArcsTest, ArcManagerAbortMerge);
  FRIEND_TEST(ArcsTest, ArcManagerArcIntersect);
  FRIEND_TEST(ArcsTest, ArcManagerCalcAllIntersects);
  FRIEND_TEST(ArcsTest, ArcManagerReverseIntersect);
  FRIEND_TEST(ArcsTest, ArcManagerGetIntersectsForArc);
  FRIEND_TEST(ArcsTest, ArcManagerSplitPolyarc);
  FRIEND_TEST(ArcsTest, ArcManagerAbsorbPolyarc);
  FRIEND_TEST(ArcsTest, ArcManagerAbsorbPolyarcs);
  FRIEND_TEST(ArcsTest, ArcManagerDeleteAbsorbedArcs);
  FRIEND_TEST(ArcsTest, ArcManagerIntersectAllPolyarcs);
  FRIEND_TEST(ArcsTest, ArcManagerCalcEdgesArcInfo);
  FRIEND_TEST(ArcsTest, ArcManagerCalcPolyarcActualities);
  FRIEND_TEST(ArcsTest, ArcManagerIsPolygonCW);
  FRIEND_TEST(ArcsTest, ArcManagerClearArcs);
public:
  ArcManagerPrivate(const GeometryEnvironment& env) : ArcManager(env)
  {}
};
//==============================================================================
class ArcsTest : public ::testing::Test
{
//==============================================================================
public:
//==============================================================================
  void SetUp() { }
//==============================================================================
  void TearDown() { }
//==============================================================================
};
//==============================================================================
/*!
 * \brief Тестирование конструктора дуги по умолчанию.
 */
TEST_F(ArcsTest, ArcDefaultConstructor)
{
  ArcPrivate arc;

  ASSERT_EQ(0U, arc.mId);
  ASSERT_EQ(Point2D(), arc.mCenter);
  ASSERT_EQ(0U, arc.mCenterId);
  ASSERT_EQ(0U, arc.mTaskId);
  ASSERT_EQ(0., arc.mRadius);
  ASSERT_EQ(Angle().value(), arc.mBeginAngle.value());
  ASSERT_EQ(0., arc.mSize);
  ASSERT_EQ(0U, arc.mArcBeginInfo);
  ASSERT_EQ(0U, arc.mArcEndInfo);
  ASSERT_EQ(Point2D(), arc.mBox.min_corner());
  ASSERT_EQ(Point2D(), arc.mBox.max_corner());
}
//==============================================================================
/*!
 * \brief Тестирование конструктора дуги.
 */
TEST_F(ArcsTest, ArcConstructor)
{
  Point2D    center(-22.126, 64.084);
  uinttype   centerId(196U);
  double     radius(-10.78);
  Angle      beginAngle(-50.487);
  Angle      endAngle(-3.393);
  ArcPrivate arc(center, centerId, radius, beginAngle, endAngle);

  ASSERT_EQ(0U, arc.mId);
  ASSERT_EQ(center, arc.mCenter);
  ASSERT_EQ(centerId, arc.mCenterId);
  ASSERT_EQ(0U, arc.mTaskId);
  ASSERT_EQ(beginAngle.value(), arc.mBeginAngle.value());
  ASSERT_EQ(beginAngle.deltaAngleCounterclockwise(endAngle), arc.mSize);
  ASSERT_EQ(0U, arc.mArcBeginInfo);
  ASSERT_EQ(0U, arc.mArcEndInfo);
  ASSERT_EQ(Point2D(-32.906, 53.304), arc.mBox.min_corner());
  ASSERT_EQ(Point2D(-11.684887849940992, 66.45247743442215),
            arc.mBox.max_corner());

  //! Полная дуга.
  arc = ArcPrivate({ 0., 0. }, centerId, 10., Angle(0.), Angle(0.), false);
  ASSERT_EQ(0U, arc.mId);
  ASSERT_EQ(Point2D(0., 0.), arc.mCenter);
  ASSERT_EQ(centerId, arc.mCenterId);
  ASSERT_EQ(0U, arc.mTaskId);
  ASSERT_EQ(Angle(0.).value(), arc.mBeginAngle.value());
  ASSERT_EQ(2. * M_PI, arc.mSize);
  ASSERT_EQ(0U, arc.mArcBeginInfo);
  ASSERT_EQ(0U, arc.mArcEndInfo);
  ASSERT_EQ(Point2D(-10., -10), arc.mBox.min_corner());
  ASSERT_EQ(Point2D(10., 10.), arc.mBox.max_corner());
}
//==============================================================================
/*!
 * \brief Тестирование сеттеров дуги.
 */
TEST_F(ArcsTest, ArcSetters)
{
  ArcPrivate arc;

  arc.setId(38323U);
  ASSERT_EQ(38323U, arc.mId);

  arc.setBeginInfo(ByViewZone, 57633U);
  ASSERT_EQ(ByViewZone, arc.mBeginInfo);
  ASSERT_EQ(57633U, arc.mArcBeginInfo);

  arc.setEndInfo(ByCut, 69392U);
  ASSERT_EQ(ByCut, arc.mEndInfo);
  ASSERT_EQ(69392U, arc.mArcEndInfo);

  arc.setBeginAngle(Angle(6330.29261));
  ASSERT_TRUE(
    BasicMath::isEqual(3.158179637022, arc.mSize, 100. * ACCURACY_DOUBLE));
  ASSERT_TRUE(
    BasicMath::isEqual(
      3.125005670156,
      arc.mBeginAngle.value(),
      100. * ACCURACY_DOUBLE));
  double  centerX(arc.mCenter.getX());
  double  centerY(arc.mCenter.getY());
  Point2D minCorner(centerX - arc.mRadius, centerY - arc.mRadius);
  Point2D maxCorner(centerX + arc.mRadius, centerY);
  Box2D   box(minCorner, maxCorner);
  ASSERT_EQ(box.min_corner(), arc.mBox.min_corner());
  ASSERT_EQ(box.max_corner(), arc.mBox.max_corner());

  arc.setBeginAngle(arc.endAngle(), false);
  ASSERT_TRUE(BasicMath::isEqual(2. * M_PI, arc.mSize));
  ASSERT_TRUE(
    BasicMath::isEqual(arc.endAngle().value(), arc.mBeginAngle.value()));
  maxCorner = { centerX + arc.mRadius, centerY + arc.mRadius };
  box = { minCorner, maxCorner };
  ASSERT_EQ(box.min_corner(), arc.mBox.min_corner());
  ASSERT_EQ(box.max_corner(), arc.mBox.max_corner());

  arc.setEndAngle(arc.beginAngle());
  ASSERT_TRUE(
    BasicMath::isEqual(arc.endAngle().value(), arc.mBeginAngle.value()));
  ASSERT_EQ(box.min_corner(), arc.mBox.min_corner());
  ASSERT_EQ(box.max_corner(), arc.mBox.max_corner());

  arc.setBeginEndAngles(Angle(146949.476445), Angle(-283022.781542));
  ASSERT_TRUE(
    BasicMath::isEqual(4.9621384, arc.mSize, 1e-6));
  maxCorner.set(maxCorner.getX(), centerY + arc.mRadius);
  box = Box2D(minCorner, maxCorner);
  ASSERT_EQ(box.min_corner(), arc.mBox.min_corner());
  ASSERT_EQ(box.max_corner(), arc.mBox.max_corner());

  arc.setBeginEndAngles(Angle(146949.476445), Angle(146949.476445), false);
  ASSERT_TRUE(BasicMath::isEqual(2. * M_PI, arc.mSize));
  maxCorner.set(maxCorner.getX(), centerY + arc.mRadius);
  box = Box2D(
    { centerX - arc.mRadius, centerY - arc.mRadius },
    { centerX + arc.mRadius, centerY + arc.mRadius });
  ASSERT_EQ(box.min_corner(), arc.mBox.min_corner());
  ASSERT_EQ(box.max_corner(), arc.mBox.max_corner());

  arc.setTaskInfo(212614U, 848475U);
  ASSERT_EQ(212614U, arc.mTaskId);
  ASSERT_EQ(848475U, arc.mParentTaskId);
}
//==============================================================================
/*!
 * \brief Тестирование геттеров дуги.
 */
TEST_F(ArcsTest, ArcGetters)
{
  Arc arc(Point2D(667.604, 654.051), 556U, 270.248, Angle(1.51419), Angle(3.51366));

  arc.setId(38323U);
  ASSERT_EQ(38323U, arc.id());

  ASSERT_EQ(Point2D(667.604, 654.051), arc.center());

  ASSERT_EQ(556U, arc.centerId());

  arc.setTaskInfo(212614U, 848475U);

  ASSERT_EQ(212614U, arc.taskId());

  ASSERT_EQ(848475U, arc.parentTaskId());

  ASSERT_EQ(270.248, arc.radius());

  ASSERT_TRUE(BasicMath::isEqual(1.51419, arc.beginAngle().value()));

  ASSERT_TRUE(BasicMath::isEqual(3.51366, arc.endAngle().value()));

  ASSERT_TRUE(BasicMath::isEqual(3.51366 - 1.51419, arc.size()));

  arc.setBeginInfo(ByViewZone, 57633U);
  ASSERT_EQ(ByViewZone, arc.beginInfo());

  arc.setEndInfo(ByCut, 69392U);
  ASSERT_EQ(ByCut, arc.endInfo());

  ASSERT_EQ(57633U, arc.arcBeginInfo());

  ASSERT_EQ(69392U, arc.arcEndInfo());

  Point2D expectedBeginPoint(
    682.89357821148899921161368946955,
    923.86614098010652755714113342625);
  ASSERT_TRUE(
    Geometry::distance(expectedBeginPoint, arc.beginPoint())
    < 100. * ACCURACY_DOUBLE);

  Point2D expectedEndPoint(
    415.8469709678054164347818382427,
    555.80447417398041355604156320924);
  ASSERT_TRUE(
    Geometry::distance(expectedEndPoint, arc.endPoint())
    < 100. * ACCURACY_DOUBLE);

  Box2D box(
    Point2D(arc.center().getX() - arc.radius(), arc.endPoint().getY()),
    Point2D(arc.beginPoint().getX(), arc.center().getY() + arc.radius()));

  ASSERT_TRUE(
    Geometry::distance(box.min_corner(), arc.box().min_corner())
    < 100. * ACCURACY_DOUBLE);
  ASSERT_TRUE(
    Geometry::distance(box.max_corner(), arc.box().max_corner())
    < 100. * ACCURACY_DOUBLE);
}
//==============================================================================
/*!
 * \brief Тестирование определения принадлежности точки сектору.
 */
TEST_F(ArcsTest, ArcIsPointInSector)
{
  Arc arc(Point2D(0., 0.), 556U, 10., Angle(M_PI_2), Angle(M_PI));

  //! Точка принадлежит сектору.

  Point2D point(-5., 5.);
  ASSERT_TRUE(arc.isPointInSector(point));

  /*!
   * Точка не принадлежит сектору,
   * так как находится слишком далеко от центра дуги.
   */

  point = { -25., 25. };
  ASSERT_FALSE(arc.isPointInSector(point));

  /*!
   * Точка не принадлежит сектору,
   * но находится от центра на расстоянии, не превышающем радиус дуги.
   */

  point = { 5., 5. };
  ASSERT_FALSE(arc.isPointInSector(point));
}
//==============================================================================
/*!
 * \brief Тестирование представления дуги как ломаной линии.
 */
TEST_F(ArcsTest, ArcLine)
{
  Arc                  arc(
    Point2D(0., 0.),
    556U,
    10.,
    Angle(M_PI_2),
    Angle(M_PI));
  std::vector<Point2D> line;

  arc.line(line, 2U);
  ASSERT_EQ(2, line.size());
  ASSERT_EQ(Point2D(0., 10.), line[0]);
  ASSERT_EQ(Point2D(-10., 0.), line[1]);
}
//==============================================================================
/*!
 * \brief Тестирование функции пересчёта коробки.
 */
TEST_F(ArcsTest, ArcRecalcBox)
{
  //! Пересчёт коробки по-умолчанию.

  Arc defaultArc;
  defaultArc.setEndAngle(M_PI_4);
  Box2D box(Point2D(0., 0.), Point2D(0., 0.));

  ASSERT_EQ(box.min_corner(), defaultArc.box().min_corner());
  ASSERT_EQ(box.max_corner(), defaultArc.box().max_corner());

  //! Пересчёт коробки для дуги, содержащей 0.

  Arc     arc(Point2D(0., 0.), 556U, 10., Angle(-M_PI_4), Angle(M_PI_4));
  Point2D minCorner(5. * std::sqrt(2), -5. * std::sqrt(2));
  Point2D maxCorner(10., 5. * std::sqrt(2));

  ASSERT_EQ(minCorner, arc.box().min_corner());
  ASSERT_EQ(maxCorner, arc.box().max_corner());

  //! Пересчёт коробки для дуги [pi/4, 7pi/4].

  arc.setBeginEndAngles(M_PI_4, -M_PI_4);
  minCorner = { -10., -10. };
  maxCorner = { 10., 10. };
}
//==============================================================================
/*!
 * \brief Тестирование конструкторов полидуги.
 */
TEST_F(ArcsTest, PolyarcConstructors)
{
  //! Конструктор по-умолчанию.

  PolyarcPrivate polyarc;

  ASSERT_EQ(0, polyarc.mArcs.size());
  ASSERT_EQ(Point2D(), polyarc.mBox.min_corner());
  ASSERT_EQ(Point2D(), polyarc.mBox.max_corner());

  //! Конструктор с дугой-параметром.

  Arc     arc(Point2D(0., 0.), 556U, 10., Angle(-M_PI_4), Angle(M_PI_4));
  Point2D minCorner(5. * std::sqrt(2), -5. * std::sqrt(2));
  Point2D maxCorner(10., 5. * std::sqrt(2));

  polyarc = PolyarcPrivate(arc);
  ASSERT_EQ(1, polyarc.mArcs.size());
  ASSERT_EQ(minCorner, polyarc.mBox.min_corner());
  ASSERT_EQ(maxCorner, polyarc.mBox.max_corner());

  //! Конструктор с двусторонней очередью дуг.

  std::deque<Arc> arcs;
  arcs.emplace_back(arc);
  arcs.emplace_back(Point2D(0., 0.), 556U, 10., Angle(M_PI_4), Angle(-M_PI_4));
  minCorner = { -10., -10. };
  maxCorner = { 10., 10. };

  polyarc = PolyarcPrivate(arcs);
  ASSERT_EQ(2, polyarc.mArcs.size());
  ASSERT_EQ(minCorner, polyarc.mBox.min_corner());
  ASSERT_EQ(maxCorner, polyarc.mBox.max_corner());
}
//==============================================================================
/*!
 * \brief Тестирование получения начальной дуги.
 */
TEST_F(ArcsTest, PolyarcBeginArc)
{
  using namespace ReachableArea;

  //! Получение начальной дуги из пустой полидуги.

  PolyarcPrivate polyarc;
  bool           isFailureThrown(false);
  try
  {
    polyarc.beginArc();
  }
  catch (const ReachableAreaFailure& ex)
  {
    isFailureThrown = true;
    ASSERT_EQ(reachableAreaLibTitle, ex.getLib());
    ASSERT_EQ(ArcsArcReferenceFromEmptyPolyarc, ex.getCode());
  }
  ASSERT_TRUE(isFailureThrown);

  //! Получение начальной дуги из непустой полидуги.

  Arc expectedArc(Point2D(0., 0.), 556U, 10., Angle(-M_PI_4), Angle(M_PI_4));
  polyarc.mArcs.emplace_back(expectedArc);
  polyarc.mArcs.emplace_back(
    Point2D(0., 0.),
    556U,
    10.,
    Angle(M_PI_4),
    Angle(-M_PI_4));

  Arc arc(polyarc.beginArc());
  ASSERT_EQ(expectedArc.center(), arc.center());
  ASSERT_EQ(expectedArc.centerId(), arc.centerId());
  ASSERT_EQ(expectedArc.radius(), arc.radius());
  ASSERT_EQ(expectedArc.beginAngle().value(), expectedArc.beginAngle().value());
  ASSERT_EQ(expectedArc.endAngle().value(), expectedArc.endAngle().value());
}
//==============================================================================
/*!
 * \brief Тестирование получения конечной дуги.
 */
TEST_F(ArcsTest, PolyarcEndArc)
{
  using namespace ReachableArea;

  //! Получение конечной дуги из пустой полидуги.

  PolyarcPrivate polyarc;
  bool           isFailureThrown(false);
  try
  {
    polyarc.endArc();
  }
  catch (const ReachableAreaFailure& ex)
  {
    isFailureThrown = true;
    ASSERT_EQ(reachableAreaLibTitle, ex.getLib());
    ASSERT_EQ(ArcsArcReferenceFromEmptyPolyarc, ex.getCode());
  }
  ASSERT_TRUE(isFailureThrown);

  //! Получение конечной дуги из непустой полидуги.

  Arc expectedArc(Point2D(0., 0.), 556U, 10., Angle(M_PI_4), Angle(-M_PI_4));
  polyarc.mArcs.emplace_back(
    Point2D(0., 0.),
    556U, 10.,
    Angle(-M_PI_4),
    Angle(M_PI_4));
  polyarc.mArcs.emplace_back(expectedArc);

  Arc arc(polyarc.endArc());
  ASSERT_EQ(expectedArc.center(), arc.center());
  ASSERT_EQ(expectedArc.centerId(), arc.centerId());
  ASSERT_EQ(expectedArc.radius(), arc.radius());
  ASSERT_EQ(expectedArc.beginAngle().value(), expectedArc.beginAngle().value());
  ASSERT_EQ(expectedArc.endAngle().value(), expectedArc.endAngle().value());
}
//==============================================================================
/*!
 * \brief Тестирование добавления дуги в начало.
 */
TEST_F(ArcsTest, PolyarcAddToBegin)
{
  //! Добавление дуги возможно.

  constexpr uinttype        taskId(4U);
  constexpr uinttype        parentTaskId(16U);
  constexpr ArcBeginEndInfo beginInfo(ByRefPoint);
  constexpr uinttype        beginId(32U);
  Arc                       arc(
    Point2D(0., 0.),
    556U,
    10.,
    Angle(-M_PI_4),
    Angle(M_PI_4));
  arc.setTaskInfo(taskId, parentTaskId);
  arc.setBeginInfo(beginInfo, beginId);
  Arc                       arcToAdd(
    Point2D(0., 0.),
    beginId,
    10.,
    Angle(3. * M_PI_4),
    Angle(5. * M_PI_4));
  arcToAdd.setTaskInfo(64U, taskId);
  arcToAdd.setEndInfo(ByViewZone);
  PolyarcPrivate            arcs(arc);

  ASSERT_TRUE(arcs.addToBegin(arcToAdd));
  ASSERT_EQ(2, arcs.mArcs.size());
  ASSERT_EQ(
    arcs.mArcs[0].beginAngle().value(),
    arcs.mArcs[1].beginAngle().value());
  ASSERT_EQ(
    arcs.mArcs[0].endAngle().value(),
    arcs.mArcs[1].beginAngle().value());

  Point2D minCorner(-10., -5. * std::sqrt(2));
  Point2D maxCorner(10., 5. * std::sqrt(2));

  ASSERT_EQ(minCorner, arcs.mBox.min_corner());
  ASSERT_EQ(maxCorner, arcs.mBox.max_corner());

  //! Добавление дуги невозможно (полидуга пуста).

  arcs = PolyarcPrivate();

  ASSERT_FALSE(arcs.addToBegin(arcToAdd));
  ASSERT_TRUE(arcs.mArcs.empty());
  ASSERT_EQ(Point2D(), arcs.mBox.min_corner());
  ASSERT_EQ(Point2D(), arcs.mBox.max_corner());

  //! Добавление дуги невозможно (полидуга непуста).

  arcs = PolyarcPrivate(arc);
  arcToAdd.setEndInfo(ByEdge);

  ASSERT_FALSE(arcs.addToBegin(arcToAdd));
  ASSERT_EQ(1, arcs.mArcs.size());
  minCorner = { 5. * std::sqrt(2), -5. * std::sqrt(2) };
  maxCorner = { 10., 5. * std::sqrt(2) };
  ASSERT_EQ(minCorner, arcs.mBox.min_corner());
  ASSERT_EQ(maxCorner, arcs.mBox.max_corner());

  //! Добавление дуги возможно, но требуется исправление перекрытия зон обзора.

  arcToAdd.setEndInfo(ByViewZone);
  arcToAdd.setBeginEndAngles(Angle(M_PI_2), Angle(M_PI));

  ASSERT_TRUE(arcs.addToBegin(arcToAdd));
  ASSERT_EQ(2, arcs.mArcs.size());
  ASSERT_EQ(
    arcs.mArcs[0].endAngle().value(),
    arcs.mArcs[1].beginAngle().value());

  minCorner = { -10., -5. * std::sqrt(2) };
  maxCorner = { 10., 10. };

  ASSERT_EQ(minCorner, arcs.mBox.min_corner());
  ASSERT_EQ(maxCorner, arcs.mBox.max_corner());
}
//==============================================================================
/*!
 * \brief Тестирование добавления дуги в конец.
 */
TEST_F(ArcsTest, PolyarcAddToEnd)
{
  //! Добавление дуги возможно.

  constexpr uinttype        taskId(4U);
  constexpr uinttype        parentTaskId(16U);
  constexpr ArcBeginEndInfo endInfo(ByRefPoint);
  constexpr uinttype        endId(32U);
  Arc                       arc(
    Point2D(0., 0.),
    556U,
    10.,
    Angle(-M_PI_4),
    Angle(M_PI_4));
  arc.setTaskInfo(taskId, parentTaskId);
  arc.setEndInfo(endInfo, endId);
  Arc                       arcToAdd(
    Point2D(0., 0.),
    endId,
    10.,
    Angle(3. * M_PI_4),
    Angle(5. * M_PI_4));
  arcToAdd.setTaskInfo(64U, taskId);
  arcToAdd.setBeginInfo(ByViewZone);
  PolyarcPrivate            arcs(arc);

  ASSERT_TRUE(arcs.addToEnd(arcToAdd));
  ASSERT_EQ(2, arcs.mArcs.size());
  ASSERT_EQ(
    arcs.mArcs[1].beginAngle().value(),
    arcs.mArcs[0].endAngle().value());
  ASSERT_EQ(arcs.mArcs[1].endAngle().value(), arcs.mArcs[0].endAngle().value());

  Point2D minCorner(-10., -5. * std::sqrt(2));
  Point2D maxCorner(10., 5. * std::sqrt(2));

  ASSERT_EQ(minCorner, arcs.mBox.min_corner());
  ASSERT_EQ(maxCorner, arcs.mBox.max_corner());

  //! Добавление дуги невозможно (полидуга пуста).

  arcs = PolyarcPrivate();

  ASSERT_FALSE(arcs.addToEnd(arcToAdd));
  ASSERT_TRUE(arcs.mArcs.empty());
  ASSERT_EQ(Point2D(), arcs.mBox.min_corner());
  ASSERT_EQ(Point2D(), arcs.mBox.max_corner());

  //! Добавление дуги невозможно (полидуга непуста).

  arcs = PolyarcPrivate(arc);
  arcToAdd.setBeginInfo(ByEdge);

  ASSERT_FALSE(arcs.addToEnd(arcToAdd));
  ASSERT_EQ(1, arcs.mArcs.size());
  minCorner = { 5. * std::sqrt(2), -5. * std::sqrt(2) };
  maxCorner = { 10., 5. * std::sqrt(2) };
  ASSERT_EQ(minCorner, arcs.mBox.min_corner());
  ASSERT_EQ(maxCorner, arcs.mBox.max_corner());

  //! Добавление дуги возможно, но требуется исправление перекрытия зон обзора.

  arcToAdd.setBeginInfo(ByViewZone);
  arcToAdd.setBeginEndAngles(Angle(M_PI), Angle(3. * M_PI_2));

  ASSERT_TRUE(arcs.addToEnd(arcToAdd));
  ASSERT_EQ(2, arcs.mArcs.size());
  ASSERT_EQ(
    arcs.mArcs[0].endAngle().value(),
    arcs.mArcs[1].beginAngle().value());

  minCorner = { -10., -10. };
  maxCorner = { 10., 5. * std::sqrt(2) };

  ASSERT_EQ(minCorner, arcs.mBox.min_corner());
  ASSERT_EQ(maxCorner, arcs.mBox.max_corner());
}
//==============================================================================
/*!
 * \brief Тестирование добавления полидуги в конец.
 */
TEST_F(ArcsTest, PolyarcAddToEndWithoutCheck)
{
  Arc            arc(Point2D(0., 0.), 556U, 10., Angle(-M_PI_4), Angle(M_PI_4));
  Arc            arc2(
    Point2D(0., 0.),
    556U,
    10.,
    Angle(3. * M_PI_4),
    Angle(5. * M_PI_4));
  PolyarcPrivate arcs(arc);

  arcs.addToEndWithoutCheck(Polyarc(arc2));
  ASSERT_EQ(2, arcs.mArcs.size());
  ASSERT_EQ(Angle(-M_PI_4).value(), arcs.mArcs[0].beginAngle().value());
  ASSERT_EQ(Angle(M_PI_4).value(), arcs.mArcs[0].endAngle().value());
  ASSERT_EQ(Angle(3. * M_PI_4).value(), arcs.mArcs[1].beginAngle().value());
  ASSERT_EQ(Angle(5. * M_PI_4).value(), arcs.mArcs[1].endAngle().value());
  ASSERT_EQ(Point2D(-10., -5. * std::sqrt(2)), arcs.mBox.min_corner());
  ASSERT_EQ(Point2D(10., 5. * std::sqrt(2)), arcs.mBox.max_corner());
}
//==============================================================================
/*!
 * \brief Тестирование геттеров полидуги.
 */
TEST_F(ArcsTest, PolyarcGetters)
{
  std::deque<Arc> arcs;
  arcs.emplace_back(Point2D(0., 0.), 556U, 10., Angle(-M_PI_4), Angle(M_PI_4));
  arcs.emplace_back(
    Point2D(0., 0.),
    556U,
    10.,
    Angle(3. * M_PI_4),
    Angle(5. * M_PI_4));
  PolyarcPrivate polyarc(arcs);

  //! Получение дуг, составляющих полидугу.

  std::deque<Arc> arcsFromPolyarc(polyarc.arcs());
  ASSERT_EQ(arcs.size(), arcsFromPolyarc.size());
  ASSERT_EQ(
    arcs[0].beginAngle().value(),
    arcsFromPolyarc[0].beginAngle().value());
  ASSERT_EQ(arcs[0].endAngle().value(), arcsFromPolyarc[0].endAngle().value());
  ASSERT_EQ(
    arcs[1].beginAngle().value(),
    arcsFromPolyarc[1].beginAngle().value());
  ASSERT_EQ(arcs[1].endAngle().value(), arcsFromPolyarc[1].endAngle().value());

  //! Получение дуги по id и углу.

  polyarc.mArcs[0].setId(0U);
  polyarc.mArcs[1].setId(1U);

  //! Нужная дуга принадлежит полидуге.

  uinttype pos(0U);
  Arc      arc;
  ASSERT_TRUE(polyarc.getArc(0U, Angle(0.), pos, arc));
  ASSERT_EQ(0U, pos);
  ASSERT_EQ(Angle(-M_PI_4).value(), arc.beginAngle().value());
  ASSERT_EQ(Angle(M_PI_4).value(), arc.endAngle().value());

  //! Нужная дуга не принадлежит полидуге.

  ASSERT_FALSE(polyarc.getArc(0U, Angle(M_PI_2), pos, arc));

  //! Получение коробки.
  
  Box2D actualBox(polyarc.mBox);
  Box2D boxFromGetter(polyarc.box());
  ASSERT_EQ(actualBox.min_corner(), boxFromGetter.min_corner());
  ASSERT_EQ(actualBox.max_corner(), boxFromGetter.max_corner());
}
//==============================================================================
/*!
 * \brief Тестирование представления полидуги как набора точек.
 */
TEST_F(ArcsTest, PolyarcLine)
{
  Polyarc              polyarc(
    Arc(Point2D(0., 0.), 556U, 10., Angle(M_PI_2), Angle(M_PI)));
  std::vector<Point2D> line;

  polyarc.line(line, 2U);
  ASSERT_EQ(2, line.size());
  ASSERT_EQ(Point2D(0., 10.), line[0]);
  ASSERT_EQ(Point2D(-10., 0.), line[1]);
}
//==============================================================================
/*!
 * \brief Тестирование проверки замкнутости полидуги.
 */
TEST_F(ArcsTest, PolyarcIsClosed)
{
  //! Пустая полидуга.

  Polyarc polyarc;
  ASSERT_FALSE(polyarc.isClosed());

  //! Полидуга, состоящая из пустых дуг.
  polyarc = Polyarc({ Arc(), Arc(), Arc(), Arc() });
  ASSERT_FALSE(polyarc.isClosed());

  //! Незамкнутая полидуга.

  polyarc = Polyarc({
    Arc(Point2D(0., 0.), 556U, 10., Angle(-M_PI_4), Angle(M_PI_4)),
    Arc(Point2D(0., 0.), 556U, 10., Angle(3. * M_PI_4), Angle(5. * M_PI_4))
  });
  ASSERT_FALSE(polyarc.isClosed());

  //! Замкнутая полидуга.

  polyarc = Polyarc({
    Arc(Point2D(0., 0.), 556U, 10., Angle(-M_PI_4), Angle(M_PI_4)),
    Arc(Point2D(0., 0.), 556U, 10., Angle(M_PI_4), Angle(7. * M_PI_4))
  });
  ASSERT_TRUE(polyarc.isClosed());
}
//==============================================================================
/*!
 * \brief Тестирование конструктора геометрии,
 * состоящей из набора полидуг и полилиний, по-умолчанию.
 */
TEST_F(ArcsTest, SemiarcContourDefaultConstructor)
{
  SemiarcContourPrivate semiarcContour;

  ASSERT_TRUE(semiarcContour.mPolyarcs.empty());
  ASSERT_TRUE(semiarcContour.mPoints.empty());
  ASSERT_TRUE(semiarcContour.mElemTypes.empty());
}
//==============================================================================
/*!
 * \brief Тестирование добавления в конец для геометрии,
 * состоящей из набора полидуг и полилиний.
 */
TEST_F(ArcsTest, SemiarcContourPush)
{
  SemiarcContourPrivate semiarcContour;

  semiarcContour.push(
    Polyarc({
    Arc(Point2D(0., 0.), 556U, 10., Angle(-M_PI_4), Angle(M_PI_4)),
    Arc(Point2D(0., 0.), 556U, 10., Angle(M_PI_4), Angle(7. * M_PI_4)) }));
  ASSERT_TRUE(semiarcContour.mPoints.empty());
  ASSERT_EQ(1, semiarcContour.mPolyarcs.size());
  ASSERT_EQ(1, semiarcContour.mElemTypes.size());
  ASSERT_EQ(
    Angle(-M_PI_4).value(),
    semiarcContour.mPolyarcs.back().arcs()[0].beginAngle().value());
  ASSERT_EQ(
    Angle(M_PI_4).value(),
    semiarcContour.mPolyarcs.back().arcs()[0].endAngle().value());
  ASSERT_EQ(
    Angle(M_PI_4).value(),
    semiarcContour.mPolyarcs.back().arcs()[1].beginAngle().value());
  ASSERT_EQ(
    Angle(7. * M_PI_4).value(),
    semiarcContour.mPolyarcs.back().arcs()[1].endAngle().value());
  
  semiarcContour.push(Point2D(4., 4.));
  ASSERT_EQ(1, semiarcContour.mPoints.size());
  ASSERT_EQ(1, semiarcContour.mPolyarcs.size());
  ASSERT_EQ(2, semiarcContour.mElemTypes.size());
  ASSERT_EQ(Point2D(4., 4.), semiarcContour.mPoints.back());

  semiarcContour.push({ Point2D(5., 5.), Point2D(6., 6.) });
  ASSERT_EQ(3, semiarcContour.mPoints.size());
  ASSERT_EQ(1, semiarcContour.mPolyarcs.size());
  ASSERT_EQ(4, semiarcContour.mElemTypes.size());
  ASSERT_EQ(Point2D(5., 5.), semiarcContour.mPoints[1]);
  ASSERT_EQ(Point2D(6., 6.), semiarcContour.mPoints[2]);
}
//==============================================================================
/*!
 * \brief Тестирование представления геометрии как набора точек.
 */
TEST_F(ArcsTest, SemiarcLine)
{
  SemiarcContourPrivate semiarcContour;
  semiarcContour.push(
    Polyarc({
    Arc(Point2D(0., 0.), 556U, 10., Angle(-M_PI_4), Angle(M_PI_4)),
    Arc(Point2D(0., 0.), 556U, 10., Angle(M_PI_4), Angle(7. * M_PI_4)) }));
  semiarcContour.push(Point2D(4., 4.));
  semiarcContour.push({ Point2D(5., 5.), Point2D(6., 6.) });
  std::vector<Point2D> line;

  semiarcContour.line(line, 1U);
  ASSERT_EQ(7, line.size());
  ASSERT_EQ(Point2D(5. * std::sqrt(2), -5. * std::sqrt(2)), line[0]);
  ASSERT_EQ(Point2D(5. * std::sqrt(2), 5. * std::sqrt(2)), line[1]);
  ASSERT_EQ(Point2D(5. * std::sqrt(2), 5. * std::sqrt(2)), line[2]);
  ASSERT_EQ(Point2D(5. * std::sqrt(2), -5. * std::sqrt(2)), line[3]);
  ASSERT_EQ(Point2D(4., 4.), line[4]);
  ASSERT_EQ(Point2D(5., 5.), line[5]);
  ASSERT_EQ(Point2D(6., 6.), line[6]);
}
//==============================================================================
/*!
 * \brief Тестирование конструктора менеджера дуг.
 */
TEST_F(ArcsTest, ArcManagerConstructor)
{
  GeometryEnvironment env;
  ArcManagerPrivate   manager(env);

  ASSERT_EQ(env, manager.mEnvironment);
  ASSERT_EQ(1U, manager.mArcIdCounter);
  ASSERT_TRUE(manager.mPolyarcs.empty());
  ASSERT_FALSE(manager.mNeedAbortMerge);
}
//==============================================================================
/*!
 * \brief Тестирование добавления дуг в общий накопитель.
 */
TEST_F(ArcsTest, ArcManagerPush)
{
  GeometryEnvironment env;
  ArcManagerPrivate   manager(env);

  //! Дуги нельзя подклеить.

  constexpr uinttype        taskId(4U);
  constexpr uinttype        parentTaskId(16U);
  constexpr ArcBeginEndInfo endInfo(ByRefPoint);
  constexpr uinttype        endId(32U);
  Arc                       arc(
    Point2D(0., 0.),
    556U,
    10.,
    Angle(-M_PI_4),
    Angle(M_PI_4));
  arc.setTaskInfo(taskId, parentTaskId);
  arc.setEndInfo(endInfo, endId);
  std::vector<Arc> arcs{ arc };

  manager.push(arcs);
  ASSERT_EQ(1, manager.mPolyarcs.size());
  ASSERT_EQ(1, manager.mPolyarcs[0].arcs().size());
  ASSERT_TRUE(
    BasicMath::isEqual(
      Angle(-M_PI_4).value(),
      manager.mPolyarcs[0].arcs()[0].beginAngle().value()));
  ASSERT_TRUE(
    BasicMath::isEqual(
      Angle(M_PI_4).value(),
      manager.mPolyarcs[0].arcs()[0].endAngle().value()));

  //! Дуги можно подклеить.

  Arc                       arcToAdd(
    Point2D(0., 0.),
    endId,
    10.,
    Angle(3. * M_PI_4),
    Angle(5. * M_PI_4));
  arcToAdd.setTaskInfo(64U, taskId);
  arcToAdd.setBeginInfo(ByViewZone);
  arcs = { arcToAdd };

  manager.push(arcs);
  ASSERT_EQ(1, manager.mPolyarcs.size());
  ASSERT_EQ(2, manager.mPolyarcs[0].arcs().size());
  ASSERT_TRUE(
    BasicMath::isEqual(
      Angle(-M_PI_4).value(),
      manager.mPolyarcs[0].arcs()[0].beginAngle().value()));
  ASSERT_TRUE(
    BasicMath::isEqual(
      Angle(M_PI_4).value(),
      manager.mPolyarcs[0].arcs()[0].endAngle().value()));
  ASSERT_TRUE(
    BasicMath::isEqual(
      manager.mPolyarcs[0].arcs()[0].endAngle().value(),
      manager.mPolyarcs[0].arcs()[1].beginAngle().value()));
  ASSERT_TRUE(
    BasicMath::isEqual(
      manager.mPolyarcs[0].arcs()[0].endAngle().value(),
      manager.mPolyarcs[0].arcs()[1].endAngle().value()));
}
//==============================================================================
/*!
 * \brief Тестирование сборки контура по накопленным дугам.
 */
TEST_F(ArcsTest, ArcManagerMerge)
{
  using namespace ReachableArea;

  //! Квадрат -- единственное препятствие.

  Polygon2D square;
  square.outer().resize(5);
  square.outer()[0] = { 1., 1. };
  square.outer()[1] = { 1., 4. };
  square.outer()[2] = { 4., 4. };
  square.outer()[3] = { 4., 1. };
  square.outer()[4] = square.outer()[0];
  GeometryEnvironment env({ square });
  Arc arc(
    Point2D(0., 0.),
    1U,
    3.,
    Angle(1.230959417340774682134929178248),
    Angle(.33983690945412193709639251339175));
  arc.setBeginInfo(ByEdge, 1U);
  arc.setEndInfo(ByEdge, 4U);

  ArcManagerPrivate manager(env);
  manager.push({ arc });
  ReachableAreaResult result;
  manager.merge({ 1U }, result, 4U);

  ASSERT_EQ(7, result.area.outer().size());
  ASSERT_EQ(Point2D(1., 1.), result.area.outer()[0]);
  ASSERT_EQ(
    Point2D(3. * std::cos(std::asin(1. / 3.)), 1.), result.area.outer()[1]);
  ASSERT_EQ(
    Point2D(3. * std::cos(std::asin(1. / 3.)), -1.), result.area.outer()[2]);
  ASSERT_EQ(
    Point2D(-1., -3. * std::sin(std::acos(1. / 3.))), result.area.outer()[3]);
  ASSERT_EQ(
    Point2D(-3. * std::cos(std::asin(1. / 3.)), 1.), result.area.outer()[4]);
  ASSERT_EQ(
    Point2D(1., 3. * std::sin(std::acos(1. / 3.))), result.area.outer()[5]);
  ASSERT_EQ(result.area.outer()[0], result.area.outer()[6]);
  ASSERT_EQ(1, result.borders.size());
  ASSERT_EQ(5, result.borders[0].size());
  ASSERT_EQ(
    Point2D(1., 3. * std::sin(std::acos(1. / 3.))), result.borders[0][0]);
  ASSERT_EQ(
    Point2D(-3. * std::cos(std::asin(1. / 3.)), 1.), result.borders[0][1]);
  ASSERT_EQ(
    Point2D(-1., -3. * std::sin(std::acos(1. / 3.))), result.borders[0][2]);
  ASSERT_EQ(
    Point2D(3. * std::cos(std::asin(1. / 3.)), -1.), result.borders[0][3]);
  ASSERT_EQ(
    Point2D(3. * std::cos(std::asin(1. / 3.)), 1.), result.borders[0][4]);
  ASSERT_TRUE(manager.mPolyarcs.empty());
  ASSERT_EQ(1U, manager.mArcIdCounter);

  //! Препятствия отсутствуют.

  GeometryEnvironment emptyEnv;
  ArcManagerPrivate managerWithEmptyEnv(emptyEnv);
  Arc arc1(
    Point2D(0., 0.),
    1U,
    3.,
    Angle(0.),
    Angle(2. * M_PI),
    false);
  arc1.setBeginInfo(ByRefPoint, 0U);
  arc1.setEndInfo(ByRefPoint, 0U);
  managerWithEmptyEnv.push({ arc1 });

  managerWithEmptyEnv.merge({}, result, 4U);
  ASSERT_EQ(5, result.area.outer().size());
  ASSERT_TRUE(result.area.inners().empty());
  ASSERT_EQ(1, result.borders.size());
  ASSERT_EQ(5, result.borders[0].size());
  ASSERT_EQ(Point2D(0., -3.), result.area.outer()[0]);
  ASSERT_EQ(Point2D(-3., 0.), result.area.outer()[1]);
  ASSERT_EQ(Point2D(0., 3.), result.area.outer()[2]);
  ASSERT_EQ(Point2D(3., 0.), result.area.outer()[3]);
  ASSERT_EQ(result.area.outer()[0], result.area.outer()[4]);
  ASSERT_EQ(Point2D(3., 0.), result.borders[0][0]);
  ASSERT_EQ(Point2D(0., 3.), result.borders[0][1]);
  ASSERT_EQ(Point2D(-3., 0.), result.borders[0][2]);
  ASSERT_EQ(Point2D(0., -3.), result.borders[0][3]);
  ASSERT_EQ(Point2D(3., 0.), result.borders[0][4]);
  ASSERT_TRUE(managerWithEmptyEnv.mPolyarcs.empty());
  ASSERT_EQ(1U, managerWithEmptyEnv.mArcIdCounter);

  // Сразу две дуги пересекаются с препятствием (квадратом).

  Arc miniArc(
    Point2D(3.5, 1.),
    2U,
    .25,
    Angle(M_PI),
    Angle(0.));
  miniArc.setBeginInfo(ByEdge, 4U);
  miniArc.setEndInfo(ByEdge, 4U);
  manager.push({ arc, miniArc });

  manager.merge({ 1U }, result, 4U);

  ASSERT_EQ(11, result.area.outer().size());
  ASSERT_TRUE(result.area.inners().empty());
  ASSERT_EQ(2, result.borders.size());
  ASSERT_EQ(3, result.borders[0].size());
  ASSERT_EQ(5, result.borders[1].size());
  ASSERT_EQ(Point2D(3.75, 1.), result.area.outer()[0]);
  ASSERT_EQ(Point2D(3.5, .75), result.area.outer()[1]);
  ASSERT_EQ(Point2D(3.25, 1.), result.area.outer()[2]);
  ASSERT_EQ(result.area.outer()[0], result.area.outer()[3]);
  ASSERT_EQ(Point2D(1., 1.), result.area.outer()[4]);
  ASSERT_EQ(
    Point2D(3. * std::cos(std::asin(1. / 3.)), 1.), result.area.outer()[5]);
  ASSERT_EQ(
    Point2D(3. * std::cos(std::asin(1. / 3.)), -1.), result.area.outer()[6]);
  ASSERT_EQ(
    Point2D(-1., -3. * std::sin(std::acos(1. / 3.))), result.area.outer()[7]);
  ASSERT_EQ(
    Point2D(-3. * std::cos(std::asin(1. / 3.)), 1.), result.area.outer()[8]);
  ASSERT_EQ(
    Point2D(1., 3. * std::sin(std::acos(1. / 3.))), result.area.outer()[9]);
  ASSERT_EQ(result.area.outer()[4], result.area.outer()[10]);
  ASSERT_EQ(Point2D(3.25, 1.), result.borders[0][0]);
  ASSERT_EQ(Point2D(3.5, .75), result.borders[0][1]);
  ASSERT_EQ(Point2D(3.75, 1.), result.borders[0][2]);
  ASSERT_EQ(
    Point2D(1., 3. * std::sin(std::acos(1. / 3.))), result.borders[1][0]);
  ASSERT_EQ(
    Point2D(-3. * std::cos(std::asin(1. / 3.)), 1.), result.borders[1][1]);
  ASSERT_EQ(
    Point2D(-1., -3. * std::sin(std::acos(1. / 3.))), result.borders[1][2]);
  ASSERT_EQ(
    Point2D(3. * std::cos(std::asin(1. / 3.)), -1.), result.borders[1][3]);
  ASSERT_EQ(
    Point2D(3. * std::cos(std::asin(1. / 3.)), 1.), result.borders[1][4]);
  ASSERT_TRUE(managerWithEmptyEnv.mPolyarcs.empty());
  ASSERT_EQ(1U, managerWithEmptyEnv.mArcIdCounter);

  //! Некорректно заданные окружение и полидуги.

  Polygon2D badSquare1;
  badSquare1.outer().resize(4);
  badSquare1.outer()[0] = { -1., 1. };
  badSquare1.outer()[1] = { -2., 1. };
  badSquare1.outer()[2] = { -2., 2. };
  badSquare1.outer()[3] = { -1., 2. };
  Polygon2D badSquare2;
  badSquare2.outer().resize(4);
  badSquare2.outer()[0] = { 1., -1. };
  badSquare2.outer()[1] = { 2., -1. };
  badSquare2.outer()[2] = { 2., -2. };
  badSquare2.outer()[3] = { 1., -2. };
  GeometryEnvironment badEnv({ badSquare1, badSquare2 });
  ArcManager          badManager(badEnv);
  Arc badArc1(
    Point2D(-2., 1.),
    1U,
    .5,
    Angle(M_PI_2),
    Angle(0.));
  badArc1.setBeginInfo(ByEdge, 1U);
  badArc1.setEndInfo(ByEdge, 4U);
  Arc badArc2(
    Point2D(0., 0.),
    2U,
    std::sqrt(5),
    Angle(M_PI - std::atan(.5)),
    Angle(3. * M_PI_2 + std::atan(.5)));
  badArc2.setBeginInfo(ByEdge, 1U);
  badArc2.setEndInfo(ByEdge, 8U);
  Arc badArc3(
    Point2D(1., -2.),
    3U,
    .5,
    Angle(M_PI_2),
    Angle(0.));
  badArc3.setBeginInfo(ByEdge, 8U);
  badArc3.setEndInfo(ByEdge, 7U);
  Arc badArc4(
    Point2D(2., -1.),
    4U,
    .5,
    Angle(-M_PI_2),
    Angle(M_PI_2));
  badArc4.setBeginInfo(ByEdge, 6U);
  badArc4.setEndInfo(ByEdge, 5U);
  badManager.push({ badArc4, badArc1, badArc2, badArc3 });

  bool isFailureThrown(false);
  try
  {
    badManager.merge({ 1U, 2U }, result, 4U);
  }
  catch (const ReachableAreaFailure& ex)
  {
    isFailureThrown = true;
    ASSERT_EQ(reachableAreaLibTitle, ex.getLib());
    ASSERT_EQ(ArcsNotBeginTerminalPointOnBorder, ex.getCode());
  }
  ASSERT_TRUE(isFailureThrown);

  /*!
   * Два прямоугольника лежат целиком внутри предполагаемой зоны видимости,
   * которая, в свою очередь, ограничена внешним квадратом.
   */

  Polygon2D internalSquare1;
  internalSquare1.outer().resize(4);
  internalSquare1.outer()[0] = { -1., -1. };
  internalSquare1.outer()[1] = { -1., 1. };
  internalSquare1.outer()[2] = { 1., 1. };
  internalSquare1.outer()[3] = { 1., -1. };
  Polygon2D internalSquare2;
  internalSquare2.outer().resize(4);
  internalSquare2.outer()[0] = { 3., -1. };
  internalSquare2.outer()[1] = { 3., 1. };
  internalSquare2.outer()[2] = { 4., 1. };
  internalSquare2.outer()[3] = { 4., -1. };
  Polygon2D externalSquare;
  externalSquare.outer().resize(4);
  externalSquare.outer()[0] = { -10., -10. };
  externalSquare.outer()[1] = { 10., -10. };
  externalSquare.outer()[2] = { 10., 10. };
  externalSquare.outer()[3] = { -10., 10. };
  GeometryEnvironment strangeEnv(
    { internalSquare1, externalSquare, internalSquare2 });
  ArcManagerPrivate   strangeManager(strangeEnv);

  strangeManager.merge({ 1U, 2U, 3U }, result, 4U);

  ASSERT_TRUE(result.borders.empty());
  ASSERT_EQ(5, result.area.outer().size());
  ASSERT_EQ(2, result.area.inners().size());
  ASSERT_EQ(5, result.area.inners()[0].size());
  ASSERT_EQ(5, result.area.inners()[1].size());
  ASSERT_EQ(Point2D(-10., 10.), result.area.outer()[0]);
  ASSERT_EQ(Point2D(10., 10.), result.area.outer()[1]);
  ASSERT_EQ(Point2D(10., -10.), result.area.outer()[2]);
  ASSERT_EQ(Point2D(-10., -10.), result.area.outer()[3]);
  ASSERT_EQ(result.area.outer()[0], result.area.outer()[4]);
  ASSERT_EQ(Point2D(1., -1.), result.area.inners()[0][0]);
  ASSERT_EQ(Point2D(1., 1.), result.area.inners()[0][1]);
  ASSERT_EQ(Point2D(-1., 1.), result.area.inners()[0][2]);
  ASSERT_EQ(Point2D(-1., -1.), result.area.inners()[0][3]);
  ASSERT_EQ(result.area.inners()[0][0], result.area.inners()[0][4]);
  ASSERT_EQ(Point2D(4., -1.), result.area.inners()[1][0]);
  ASSERT_EQ(Point2D(4., 1.), result.area.inners()[1][1]);
  ASSERT_EQ(Point2D(3., 1.), result.area.inners()[1][2]);
  ASSERT_EQ(Point2D(3., -1.), result.area.inners()[1][3]);
  ASSERT_EQ(result.area.inners()[1][0], result.area.inners()[1][4]);
  ASSERT_TRUE(strangeManager.mPolyarcs.empty());
  ASSERT_EQ(1U, strangeManager.mArcIdCounter);

  /*!
   * Квадрат лежит целиком внутри предполагаемой зоны видимости,
   * которая, в свою очередь, ограничена двумя вложенными внешними квадратами.
   */

  Polygon2D bigExternalSquare;
  bigExternalSquare.outer().resize(4);
  bigExternalSquare.outer()[0] = { -12., -12. };
  bigExternalSquare.outer()[1] = { 12., -12. };
  bigExternalSquare.outer()[2] = { 12., 12. };
  bigExternalSquare.outer()[3] = { -12., 12. };
  GeometryEnvironment veryStrangeEnv(
    { internalSquare1, externalSquare, bigExternalSquare });
  ArcManager          veryStrangeManager(veryStrangeEnv);

  isFailureThrown = false;
  try
  {
    veryStrangeManager.merge({ 1U, 2U, 3U }, result, 4U);
  }
  catch (const ReachableAreaFailure& ex)
  {
    isFailureThrown = true;
    ASSERT_EQ(reachableAreaLibTitle, ex.getLib());
    ASSERT_EQ(ArcsClockWiseInnerRing, ex.getCode());
  }
  ASSERT_TRUE(isFailureThrown);
}
//==============================================================================
/*!
 * \brief Тестирование прерывания процесса сборки контура (склейки).
 */
TEST_F(ArcsTest, ArcManagerAbortMerge)
{
  GeometryEnvironment env;
  ArcManagerPrivate   manager(env);

  manager.abortMerge();
  ASSERT_TRUE(manager.mNeedAbortMerge);
}
//==============================================================================
/*!
 * \brief Тестирование определения пересечения между двумя дугами.
 */
TEST_F(ArcsTest, ArcManagerArcIntersect)
{
  GeometryEnvironment env;
  ArcManagerPrivate   manager(env);

  //! Коробки дуг не пересекаются.

  Arc arc1(
    Point2D(-100., 0.),
    1U,
    1.,
    Angle(0.),
    Angle(0.),
    false);
  Arc arc2(
    Point2D(100., 0.),
    2U,
    1.,
    Angle(0.),
    Angle(0.),
    false);

  ArcManagerPrivate::ArcIntersectInfo result(manager.arcIntersect(arc1, arc2));
  ASSERT_EQ(
    ArcManagerPrivate::ArcIntersectType::NoIntersect,
    result.intersectType);

  //! Коробки дуг пересекаются, но сами дуги не пересекаются.

  arc1 = Arc(Point2D(0., 0.), 1U, 1., Angle(M_PI / 6.), Angle(M_PI / 3.));
  arc2 = Arc(Point2D(0., 0.), 2U, 1., Angle(M_PI_2), Angle(0.));
  
  result = manager.arcIntersect(arc1, arc2);
  ASSERT_EQ(
    ArcManagerPrivate::ArcIntersectType::NoIntersect,
    result.intersectType);

  //! Дуги касаются друг друга.
  
  arc1 = Arc(Point2D(-1., 0.), 1U, 1., Angle(-M_PI_2), Angle(M_PI_2));
  arc2 = Arc(Point2D(1., 0), 2U, 1., Angle(M_PI_2), Angle(-M_PI_2));

  result = manager.arcIntersect(arc1, arc2);
  ASSERT_EQ(
    ArcManagerPrivate::ArcIntersectType::DoubleIntersect,
    result.intersectType);
  ASSERT_TRUE(
    BasicMath::isEqual(Angle(0.).value(), result.point1.angle1.value()));
  ASSERT_TRUE(
    BasicMath::isEqual(Angle(M_PI).value(), result.point1.angle2.value()));
  ASSERT_EQ(Point2D(0., 0.), result.point1.point);
  ASSERT_EQ(result.point1.angle1.value(), result.point2.angle1.value());
  ASSERT_EQ(result.point1.angle2.value(), result.point2.angle2.value());
  ASSERT_EQ(result.point1.point, result.point2.point);

  //! Дуги не касаются друг друга, но соответствующие им окружности касаются.

  arc2 = Arc(Point2D(1., 0), 2U, 1., Angle(-M_PI_2), Angle(M_PI_2));

  result = manager.arcIntersect(arc1, arc2);
  ASSERT_EQ(
    ArcManagerPrivate::ArcIntersectType::NoIntersect,
    result.intersectType);

  //! Дуги пересекаются в двух точках.

  arc1 = Arc(Point2D(-1., 0.), 1U, 2., Angle(-M_PI_2), Angle(M_PI_2));
  arc2 = Arc(Point2D(1., 0), 2U, 2., Angle(M_PI_2), Angle(-M_PI_2));

  result = manager.arcIntersect(arc1, arc2);
  ASSERT_EQ(
    ArcManagerPrivate::ArcIntersectType::DoubleIntersect,
    result.intersectType);
  ASSERT_TRUE(BasicMath::isEqual(std::acos(.5), result.point1.angle1.value()));
  ASSERT_TRUE(
    BasicMath::isEqual(M_PI - std::acos(.5), result.point1.angle2.value()));
  ASSERT_EQ(Point2D(0., std::sqrt(3.)), result.point1.point);
  ASSERT_TRUE(
    BasicMath::isEqual(
      2. * M_PI - std::acos(.5),
      result.point2.angle1.value()));
  ASSERT_TRUE(
    BasicMath::isEqual(M_PI + std::acos(.5), result.point2.angle2.value()));
  ASSERT_EQ(Point2D(0., -std::sqrt(3.)), result.point2.point);

  //! Дуги пересекаются в одной (первой) точке.

  arc2 = Arc(Point2D(1., 0), 2U, 2., Angle(M_PI_2), Angle(M_PI));
  result = manager.arcIntersect(arc1, arc2);

  ASSERT_EQ(
    ArcManagerPrivate::ArcIntersectType::SingleIntersect,
    result.intersectType);
  ASSERT_TRUE(BasicMath::isEqual(std::acos(.5), result.point1.angle1.value()));
  ASSERT_TRUE(
    BasicMath::isEqual(M_PI - std::acos(.5), result.point1.angle2.value()));
  ASSERT_EQ(Point2D(0., std::sqrt(3.)), result.point1.point);

  //! Дуги пересекаются в одной (второй) точке.

  arc2 = Arc(Point2D(1., 0), 2U, 2., Angle(M_PI), Angle(-M_PI_2));
  result = manager.arcIntersect(arc1, arc2);

  ASSERT_EQ(
    ArcManagerPrivate::ArcIntersectType::SingleIntersect,
    result.intersectType);
  ASSERT_TRUE(
    BasicMath::isEqual(
      2. * M_PI - std::acos(.5),
      result.point1.angle1.value()));
  ASSERT_TRUE(
    BasicMath::isEqual(M_PI + std::acos(.5), result.point1.angle2.value()));
  ASSERT_EQ(Point2D(0., -std::sqrt(3.)), result.point1.point);

  //! Дуги не пересекаются, но соответствующие им окружности пересекаются.

  arc2 = Arc(Point2D(1., 0), 2U, 2., Angle(-M_PI_2), Angle(M_PI_2));

  result = manager.arcIntersect(arc1, arc2);
  ASSERT_EQ(
    ArcManagerPrivate::ArcIntersectType::NoIntersect,
    result.intersectType);
}
//==============================================================================
/*!
 * \brief Тестирование определения пересечения между накопленными дугами.
 */
TEST_F(ArcsTest, ArcManagerCalcAllIntersects)
{
  Arc                 emptyArc(
    Point2D(-100., 100),
    1U,
    0.,
    Angle(0.),
    Angle(0.));
  Arc                 leftArc(
    Point2D(-1., 0.),
    2U,
    2.,
    Angle(-M_PI_2),
    Angle(M_PI_2));
  Arc                 rightArc(
    Point2D(1., 0),
    3U,
    2.,
    Angle(M_PI_2),
    Angle(-M_PI_2));
  Arc                 farArc(
    Point2D(100., 100.),
    4U,
    10.,
    Angle(0.),
    Angle(M_PI_2));
  GeometryEnvironment env;
  ArcManagerPrivate   manager(env);
  Polyarc polyarc;
  manager.mPolyarcs.emplace_back(
    Polyarc({ emptyArc, emptyArc, emptyArc, emptyArc }));
  manager.push({ leftArc });
  manager.mPolyarcs.emplace_back(Polyarc({ rightArc, farArc, emptyArc }));
  std::vector<ArcManagerPrivate::ArcIntersect> result;

  manager.calcAllIntersects(result);
  ASSERT_EQ(2, result.size());
  ASSERT_EQ(1U, result[0].id);
  ASSERT_EQ(1U, result[0].arc1id);
  ASSERT_EQ(0U, result[0].arc2id);
  ASSERT_EQ(Point2D(0., std::sqrt(3.)), result[0].intersectPoint.point);
  ASSERT_TRUE(
    BasicMath::isEqual(
      std::acos(.5),
      result[0].intersectPoint.angle1.value()));
  ASSERT_TRUE(
    BasicMath::isEqual(
      M_PI - std::acos(.5),
      result[0].intersectPoint.angle2.value()));
  ASSERT_TRUE(result[0].active);
  ASSERT_EQ(2U, result[1].id);
  ASSERT_EQ(1U, result[1].arc1id);
  ASSERT_EQ(0U, result[1].arc2id);
  ASSERT_EQ(Point2D(0., -std::sqrt(3.)), result[1].intersectPoint.point);
  ASSERT_TRUE(
    BasicMath::isEqual(
      2. * M_PI - std::acos(.5),
      result[1].intersectPoint.angle1.value()));
  ASSERT_TRUE(
    BasicMath::isEqual(
      M_PI + std::acos(.5),
      result[1].intersectPoint.angle2.value()));
  ASSERT_TRUE(result[1].active);
}
//==============================================================================
/*!
 * \brief Тестирование "Выворачивания" пересечения
 */
TEST_F(ArcsTest, ArcManagerReverseIntersect)
{
  ArcManagerPrivate::ArcIntersect arcIntersect{
    4992U,
    927U,
    4384U,
    { Point2D(4094.2166, -8005.6387), Angle(2871.2238), Angle(5552.9811) },
    false };
  GeometryEnvironment             env;
  ArcManagerPrivate               manager(env);

  arcIntersect = manager.reverseIntersect(arcIntersect);
  ASSERT_EQ(4992U, arcIntersect.id);
  ASSERT_EQ(4384U, arcIntersect.arc1id);
  ASSERT_EQ(927U, arcIntersect.arc2id);
  ASSERT_EQ(Point2D(4094.2166, -8005.6387), arcIntersect.intersectPoint.point);
  ASSERT_EQ(
    Angle(5552.9811).value(),
    arcIntersect.intersectPoint.angle1.value());
  ASSERT_EQ(
    Angle(2871.2238).value(),
    arcIntersect.intersectPoint.angle2.value());
  ASSERT_FALSE(arcIntersect.active);
}
//==============================================================================
/*!
 * \brief Тестирование получения пересечений для заданной дуги
 */
TEST_F(ArcsTest, ArcManagerGetIntersectsForArc)
{
  std::vector<ArcManagerPrivate::ArcIntersect> allIntersects{
    {
      7414U,
      1555U,
      5928U,
      { Point2D(-8105.1842, -1043.5292), Angle(-4713.054), Angle(-7782.954) },
      false
    },
    {
      1U,
      1U,
      2U,
      {
        Point2D(0., std::sqrt(3)),
        Angle(std::acos(.5)),
        Angle(M_PI - std::acos(.5))
      },
      true
    },
    {
      2U,
      2U,
      1U,
      {
        Point2D(0., -std::sqrt(3)),
        Angle(M_PI + std::acos(.5)),
        Angle(-std::acos(.5))
      },
      true
    }
  };
  Arc                                          arc(
    Point2D(-1., 0.),
    1U,
    2.,
    Angle(-M_PI_2),
    Angle(M_PI_2));
  arc.setId(1U);
  std::vector<ArcManagerPrivate::ArcIntersect> result;
  GeometryEnvironment                          env;
  ArcManagerPrivate                            manager(env);

  ASSERT_TRUE(manager.getIntersectsForArc(allIntersects, arc, result));
  ASSERT_EQ(2, result.size());
  ASSERT_EQ(2U, result[0].id);
  ASSERT_EQ(1U, result[0].arc1id);
  ASSERT_EQ(2U, result[0].arc2id);
  ASSERT_EQ(Point2D(0., -std::sqrt(3.)), result[0].intersectPoint.point);
  ASSERT_TRUE(
    BasicMath::isEqual(
      2. * M_PI - std::acos(.5),
      result[0].intersectPoint.angle1.value()));
  ASSERT_TRUE(
    BasicMath::isEqual(
      M_PI + std::acos(.5),
      result[0].intersectPoint.angle2.value()));
  ASSERT_TRUE(result[0].active);
  ASSERT_EQ(1U, result[1].id);
  ASSERT_EQ(1U, result[1].arc1id);
  ASSERT_EQ(2U, result[1].arc2id);
  ASSERT_EQ(Point2D(0., std::sqrt(3.)), result[1].intersectPoint.point);
  ASSERT_TRUE(
    BasicMath::isEqual(
      std::acos(.5),
      result[1].intersectPoint.angle1.value()));
  ASSERT_TRUE(
    BasicMath::isEqual(
      M_PI - std::acos(.5),
      result[1].intersectPoint.angle2.value()));
  ASSERT_TRUE(result[1].active);
}
//==============================================================================
/*!
 * \brief Тестирование разделения полидуги на две части по точке их пересечения.
 */
TEST_F(ArcsTest, ArcManagerSplitPolyarc)
{
  Arc arc(Point2D(0., 0.), 1U, 1., Angle(0.), Angle(M_PI));
  arc.setId(1U);
  ArcManagerPrivate::ArcIntersect intersect{
    1U,
    1U,
    1U,
    { Point2D(0., 1.), Angle(M_PI_2), Angle(M_PI_2) },
    true
  };
  GeometryEnvironment             env;
  ArcManagerPrivate               manager(env);
  Polyarc                         part1;
  Polyarc                         part2;

  manager.splitPolyarc(Polyarc(arc), intersect, part1, part2);
  ASSERT_EQ(1, part1.arcs().size());
  ASSERT_EQ(1U, part1.arcs()[0].centerId());
  ASSERT_EQ(1., part1.arcs()[0].radius());
  ASSERT_TRUE(BasicMath::isEqual(0., part1.arcs()[0].beginAngle().value()));
  ASSERT_TRUE(BasicMath::isEqual(M_PI_2, part1.arcs()[0].size()));
  ASSERT_EQ(ArcBeginEndInfo::ByCut, part1.arcs()[0].endInfo());
  ASSERT_EQ(1, part2.arcs().size());
  ASSERT_EQ(1U, part2.arcs()[0].centerId());
  ASSERT_EQ(1., part2.arcs()[0].radius());
  ASSERT_TRUE(BasicMath::isEqual(M_PI_2, part2.arcs()[0].beginAngle().value()));
  ASSERT_TRUE(BasicMath::isEqual(M_PI_2, part2.arcs()[0].size()));
  ASSERT_EQ(ArcBeginEndInfo::ByCut, part2.arcs()[0].beginInfo());
}
//==============================================================================
/*!
 * \brief Тестирование поглощения одной полидуги другой.
 */
TEST_F(ArcsTest, ArcManagerAbsorbPolyarc)
{
  Arc                  absorberArc(
    Point2D(0., 0.),
    1U,
    1.,
    Angle(0.),
    Angle(M_PI_2));
  Arc                  notAbsorbedArc1(
    Point2D(0., 0.),
    1U,
    1.,
    Angle(M_PI + M_PI_2 / 2.),
    Angle(-M_PI_2 / 2.));
  Arc                  notAbsorbedArc2(
    Point2D(0., 0.),
    1U,
    1.,
    Angle(M_PI - M_PI_2 / 2.),
    Angle(M_PI + M_PI_2 / 2.));
  Polyarc              absorbing({ notAbsorbedArc1, absorberArc, notAbsorbedArc2 });
  Polyarc              absorber(absorberArc);
  GeometryEnvironment  env;
  ArcManagerPrivate    manager(env);
  std::vector<Polyarc> result;

  manager.absorbPolyarc(absorbing, absorber, result);
  ASSERT_EQ(2, result.size());
  ASSERT_EQ(1, result[0].arcs().size());
  ASSERT_EQ(1, result[1].arcs().size());
  ASSERT_EQ(Point2D(0., 0.), result[0].arcs()[0].center());
  ASSERT_EQ(1U, result[0].arcs()[0].centerId());
  ASSERT_EQ(1., result[0].arcs()[0].radius());
  ASSERT_TRUE(
    BasicMath::isEqual(
      M_PI + M_PI_2 / 2.,
      result[0].arcs()[0].beginAngle().value()));
  ASSERT_TRUE(BasicMath::isEqual(M_PI_2, result[0].arcs()[0].size()));
  ASSERT_EQ(Point2D(0., 0.), result[1].arcs()[0].center());
  ASSERT_EQ(1U, result[1].arcs()[0].centerId());
  ASSERT_EQ(1., result[1].arcs()[0].radius());
  ASSERT_TRUE(
    BasicMath::isEqual(
      M_PI - M_PI_2 / 2.,
      result[1].arcs()[0].beginAngle().value()));
  ASSERT_TRUE(BasicMath::isEqual(M_PI_2, result[1].arcs()[0].size()));
}
//==============================================================================
/*!
 * \brief Тестирование поглощения полидуг другой полидугой.
 */
TEST_F(ArcsTest, ArcManagerAbsorbPolyarcs)
{
  //! Обычный случай.

  Arc                  absorberArc(
    Point2D(0., 0.),
    1U,
    1.,
    Angle(0.),
    Angle(M_PI_2));
  Arc                  notAbsorbedArc1(
    Point2D(0., 0.),
    1U,
    1.,
    Angle(M_PI + M_PI_2 / 2.),
    Angle(-M_PI_2 / 2.));
  Arc                  notAbsorbedArc2(
    Point2D(0., 0.),
    1U,
    1.,
    Angle(M_PI - M_PI_2 / 2.),
    Angle(M_PI + M_PI_2 / 2.));
  std::vector<Polyarc> absorbing{
    Polyarc(absorberArc),
    Polyarc(notAbsorbedArc1),
    Polyarc(notAbsorbedArc2)
  };
  GeometryEnvironment  env;
  ArcManagerPrivate    manager(env);

  manager.absorbPolyarcs(absorbing, absorberArc);
  ASSERT_EQ(2, absorbing.size());
  ASSERT_EQ(1, absorbing[0].arcs().size());
  ASSERT_EQ(1, absorbing[1].arcs().size());
  ASSERT_EQ(Point2D(0., 0.), absorbing[0].arcs()[0].center());
  ASSERT_EQ(1U, absorbing[0].arcs()[0].centerId());
  ASSERT_EQ(1., absorbing[0].arcs()[0].radius());
  ASSERT_TRUE(
    BasicMath::isEqual(
      M_PI + M_PI_2 / 2.,
      absorbing[0].arcs()[0].beginAngle().value()));
  ASSERT_TRUE(BasicMath::isEqual(M_PI_2, absorbing[0].arcs()[0].size()));
  ASSERT_EQ(Point2D(0., 0.), absorbing[1].arcs()[0].center());
  ASSERT_EQ(1U, absorbing[1].arcs()[0].centerId());
  ASSERT_EQ(1., absorbing[1].arcs()[0].radius());
  ASSERT_TRUE(
    BasicMath::isEqual(
      M_PI - M_PI_2 / 2.,
      absorbing[1].arcs()[0].beginAngle().value()));
  ASSERT_TRUE(BasicMath::isEqual(M_PI_2, absorbing[1].arcs()[0].size()));

  //! Поглощение пустого массива полидуг.

  absorbing.clear();

  manager.absorbPolyarcs(absorbing, absorberArc);
  ASSERT_TRUE(absorbing.empty());
}
//==============================================================================
/*!
 * \brief Тестирование поглощения полидуг другой полидугой.
 */
TEST_F(ArcsTest, ArcManagerDeleteAbsorbedArcs)
{
  Arc                 absorberArc(
    Point2D(0., 0.),
    1U,
    1.,
    Angle(0.),
    Angle(M_PI_2));
  Arc                 notAbsorbedArc1(
    Point2D(0., 0.),
    1U,
    1.,
    Angle(M_PI + M_PI_2 / 2.),
    Angle(-M_PI_2 / 2.));
  Arc                 notAbsorbedArc2(
    Point2D(0., 0.),
    1U,
    1.,
    Angle(M_PI - M_PI_2 / 2.),
    Angle(M_PI + M_PI_2 / 2.));
  GeometryEnvironment env;
  ArcManagerPrivate   manager(env);
  manager.mPolyarcs = {
    Polyarc(absorberArc),
    Polyarc(absorberArc),
    Polyarc(notAbsorbedArc1),
    Polyarc(notAbsorbedArc2)
  };

  manager.deleteAbsorbedArcs();
  ASSERT_EQ(2, manager.mPolyarcs.size());
  ASSERT_EQ(1, manager.mPolyarcs[0].arcs().size());
  ASSERT_EQ(1, manager.mPolyarcs[1].arcs().size());
  ASSERT_EQ(Point2D(0., 0.), manager.mPolyarcs[0].arcs()[0].center());
  ASSERT_EQ(1U, manager.mPolyarcs[0].arcs()[0].centerId());
  ASSERT_EQ(1., manager.mPolyarcs[0].arcs()[0].radius());
  ASSERT_TRUE(
    BasicMath::isEqual(
      M_PI + M_PI_2 / 2.,
      manager.mPolyarcs[0].arcs()[0].beginAngle().value()));
  ASSERT_TRUE(
    BasicMath::isEqual(M_PI_2, manager.mPolyarcs[0].arcs()[0].size()));
  ASSERT_EQ(Point2D(0., 0.), manager.mPolyarcs[1].arcs()[0].center());
  ASSERT_EQ(1U, manager.mPolyarcs[1].arcs()[0].centerId());
  ASSERT_EQ(1., manager.mPolyarcs[1].arcs()[0].radius());
  ASSERT_TRUE(
    BasicMath::isEqual(
      M_PI - M_PI_2 / 2.,
      manager.mPolyarcs[1].arcs()[0].beginAngle().value()));
  ASSERT_TRUE(
    BasicMath::isEqual(M_PI_2, manager.mPolyarcs[1].arcs()[0].size()));
}
//==============================================================================
/*!
 * \brief Тестирование выделения полидуг,
 * которые могут являться участками результирующего контура.
 */
TEST_F(ArcsTest, ArcManagerIntersectAllPolyarcs)
{
  //! Ни одна из дуг не подходит на роль участка результирующего контура.

  Arc                 arc1(
    Point2D(0., 0.),
    1U,
    1.,
    Angle(0.),
    Angle(M_PI_2));
  Arc                 arc2(
    Point2D(0., 0.),
    1U,
    1.,
    Angle(M_PI + M_PI_2 / 2.),
    Angle(-M_PI_2 / 2.));
  Arc                 arc3(
    Point2D(0., 0.),
    1U,
    1.,
    Angle(M_PI - M_PI_2 / 2.),
    Angle(M_PI + M_PI_2 / 2.));
  GeometryEnvironment env;
  ArcManagerPrivate   manager(env);
  manager.mPolyarcs = {
    Polyarc(arc1),
    Polyarc(arc1),
    Polyarc(arc2),
    Polyarc(arc3)
  };

  manager.intersectAllPolyarcs();

  ASSERT_TRUE(manager.mPolyarcs.empty());

  //! Есть пересекающиеся дуги с различыми центрами.

  Arc arc4(Point2D(1., 0.), 2U, 1., Angle(M_PI_2), Angle(-M_PI_2));
  arc1.setBeginInfo(ByEdge);
  arc1.setEndInfo(ByEdge);
  arc2.setBeginInfo(ByEdge);
  arc2.setEndInfo(ByEdge);
  arc3.setBeginInfo(ByEdge);
  arc3.setEndInfo(ByEdge);
  arc4.setBeginInfo(ByEdge);
  arc4.setEndInfo(ByEdge);
  manager.push({ arc1 });
  manager.push({ arc4 });

  manager.intersectAllPolyarcs();

  ASSERT_EQ(1, manager.mPolyarcs.size());
  ASSERT_EQ(2, manager.mPolyarcs[0].arcs().size());
  ASSERT_EQ(Point2D(1., 0.), manager.mPolyarcs[0].arcs()[0].center());
  ASSERT_EQ(2U, manager.mPolyarcs[0].arcs()[0].centerId());
  ASSERT_EQ(1., manager.mPolyarcs[0].arcs()[0].radius());
  ASSERT_TRUE(
    BasicMath::isEqual(
      M_PI_2,
      manager.mPolyarcs[0].arcs()[0].beginAngle().value()));
  ASSERT_TRUE(
    BasicMath::isEqual(
      M_PI_2 - std::acos(.5),
      manager.mPolyarcs[0].arcs()[0].size()));
  ASSERT_EQ(Point2D(0., 0.), manager.mPolyarcs[0].arcs()[1].center());
  ASSERT_EQ(1U, manager.mPolyarcs[0].arcs()[1].centerId());
  ASSERT_EQ(1., manager.mPolyarcs[0].arcs()[1].radius());
  ASSERT_TRUE(
    BasicMath::isEqual(
      std::acos(.5),
      manager.mPolyarcs[0].arcs()[1].beginAngle().value()));
  ASSERT_TRUE(
    BasicMath::isEqual(
      M_PI_2 - std::acos(.5),
      manager.mPolyarcs[0].arcs()[1].size()));

  //! Все дуги в полидуге попарно пересекаются.
  
  Arc arc5(
    Point2D(.5, 1.),
    3U,
    1.,
    Angle(M_PI),
    Angle(0.));
  manager.mPolyarcs.clear();
  manager.mArcIdCounter = 4U;
  arc1.setId(1U);
  arc4.setId(2U);
  arc5.setBeginInfo(ByEdge);
  arc5.setEndInfo(ByEdge);
  arc5.setId(3U);
  manager.mPolyarcs.emplace_back(Polyarc({ arc1 }));
  manager.mPolyarcs.emplace_back(Polyarc({ arc4 }));
  manager.mPolyarcs.emplace_back(Polyarc({ arc5 }));

  manager.intersectAllPolyarcs();

  ASSERT_EQ(3, manager.mPolyarcs.size());
  ASSERT_EQ(2, manager.mPolyarcs[0].arcs().size());
  ASSERT_EQ(2, manager.mPolyarcs[1].arcs().size());
  ASSERT_EQ(2, manager.mPolyarcs[2].arcs().size());

  ASSERT_EQ(Point2D(0.5, 1.), manager.mPolyarcs[0].arcs()[0].center());
  ASSERT_EQ(3U, manager.mPolyarcs[0].arcs()[0].centerId());
  ASSERT_EQ(1., manager.mPolyarcs[0].arcs()[0].radius());
  ASSERT_TRUE(
    BasicMath::isEqual(
      3.1415926535897931,
      manager.mPolyarcs[0].arcs()[0].beginAngle().value()));
  ASSERT_TRUE(
    BasicMath::isEqual(
      1.0568473851504345,
      manager.mPolyarcs[0].arcs()[0].size()));

  ASSERT_EQ(Point2D(1., 0.), manager.mPolyarcs[0].arcs()[1].center());
  ASSERT_EQ(2U, manager.mPolyarcs[0].arcs()[1].centerId());
  ASSERT_EQ(1., manager.mPolyarcs[0].arcs()[1].radius());
  ASSERT_TRUE(
    BasicMath::isEqual(
      3.0120404864409704,
      manager.mPolyarcs[0].arcs()[1].beginAngle().value()));
  ASSERT_TRUE(
    BasicMath::isEqual(
      1.7003484939437192,
      manager.mPolyarcs[0].arcs()[1].size()));

  ASSERT_EQ(Point2D(1., 0.), manager.mPolyarcs[1].arcs()[0].center());
  ASSERT_EQ(2U, manager.mPolyarcs[1].arcs()[0].centerId());
  ASSERT_EQ(1., manager.mPolyarcs[1].arcs()[0].radius());
  ASSERT_TRUE(
    BasicMath::isEqual(
      1.5707963267948966,
      manager.mPolyarcs[1].arcs()[0].beginAngle().value()));
  ASSERT_TRUE(
    BasicMath::isEqual(
      0.5235987755982987,
      manager.mPolyarcs[1].arcs()[0].size()));

  ASSERT_EQ(Point2D(0., 0.), manager.mPolyarcs[1].arcs()[1].center());
  ASSERT_EQ(1U, manager.mPolyarcs[1].arcs()[1].centerId());
  ASSERT_EQ(1., manager.mPolyarcs[1].arcs()[1].radius());
  ASSERT_TRUE(
    BasicMath::isEqual(
      1.0471975511965979,
      manager.mPolyarcs[1].arcs()[1].beginAngle().value()));
  ASSERT_TRUE(
    BasicMath::isEqual(
      0.52359877559829870,
      manager.mPolyarcs[1].arcs()[1].size()));

  ASSERT_EQ(Point2D(0., 0.), manager.mPolyarcs[2].arcs()[0].center());
  ASSERT_EQ(1U, manager.mPolyarcs[2].arcs()[0].centerId());
  ASSERT_EQ(1., manager.mPolyarcs[2].arcs()[0].radius());
  ASSERT_TRUE(
    BasicMath::isEqual(
      0.,
      manager.mPolyarcs[2].arcs()[0].beginAngle().value()));
  ASSERT_TRUE(
    BasicMath::isEqual(
      0.12955216714882256,
      manager.mPolyarcs[2].arcs()[0].size()));

  ASSERT_EQ(Point2D(0.5, 1.), manager.mPolyarcs[2].arcs()[1].center());
  ASSERT_EQ(3U, manager.mPolyarcs[2].arcs()[1].centerId());
  ASSERT_EQ(1., manager.mPolyarcs[2].arcs()[1].radius());
  ASSERT_TRUE(
    BasicMath::isEqual(
      5.2263379220291517,
      manager.mPolyarcs[2].arcs()[1].beginAngle().value()));
  ASSERT_TRUE(
    BasicMath::isEqual(
      1.0568473851504345,
      manager.mPolyarcs[2].arcs()[1].size()));

  /*!
   * Все дуги из полидуги попарно пересекаются, 
   * но третья дуга теперь снизу, а не сверху.
   */

  arc1.setBeginEndAngles(-M_PI_2, M_PI_2);
  Arc arc6(
    Point2D(.5, -1.),
    3U,
    1.,
    Angle(0.),
    Angle(M_PI));
  manager.mPolyarcs.clear();
  manager.mArcIdCounter = 4U;
  arc6.setBeginInfo(ByEdge);
  arc6.setEndInfo(ByEdge);
  arc6.setId(3U);
  manager.mPolyarcs.emplace_back(Polyarc({ arc1 }));
  manager.mPolyarcs.emplace_back(Polyarc({ arc4 }));
  manager.mPolyarcs.emplace_back(Polyarc({ arc6 }));

  manager.intersectAllPolyarcs();

  ASSERT_EQ(2, manager.mPolyarcs.size());
  ASSERT_EQ(2, manager.mPolyarcs[0].arcs().size());
  ASSERT_EQ(2, manager.mPolyarcs[1].arcs().size());

  ASSERT_EQ(Point2D(1., 0.), manager.mPolyarcs[0].arcs()[0].center());
  ASSERT_EQ(2U, manager.mPolyarcs[0].arcs()[0].centerId());
  ASSERT_EQ(1., manager.mPolyarcs[0].arcs()[0].radius());
  ASSERT_TRUE(
    BasicMath::isEqual(
      1.5707963267948966,
      manager.mPolyarcs[0].arcs()[0].beginAngle().value()));
  ASSERT_TRUE(
    BasicMath::isEqual(
      0.52359877559829870,
      manager.mPolyarcs[0].arcs()[0].size()));

  ASSERT_EQ(Point2D(0., 0.), manager.mPolyarcs[0].arcs()[1].center());
  ASSERT_EQ(1U, manager.mPolyarcs[0].arcs()[1].centerId());
  ASSERT_EQ(1., manager.mPolyarcs[0].arcs()[1].radius());
  ASSERT_TRUE(
    BasicMath::isEqual(
      1.0471975511965979,
      manager.mPolyarcs[0].arcs()[1].beginAngle().value()));
  ASSERT_TRUE(
    BasicMath::isEqual(
      0.52359877559829870,
      manager.mPolyarcs[0].arcs()[1].size()));

  ASSERT_EQ(Point2D(0., 0.), manager.mPolyarcs[1].arcs()[0].center());
  ASSERT_EQ(1U, manager.mPolyarcs[1].arcs()[0].centerId());
  ASSERT_EQ(1., manager.mPolyarcs[1].arcs()[0].radius());
  ASSERT_TRUE(
    BasicMath::isEqual(
      4.7123889803846897,
      manager.mPolyarcs[1].arcs()[0].beginAngle().value()));
  ASSERT_TRUE(
    BasicMath::isEqual(
      0.52359877559829826,
      manager.mPolyarcs[1].arcs()[0].size()));

  ASSERT_EQ(Point2D(1., 0.), manager.mPolyarcs[1].arcs()[1].center());
  ASSERT_EQ(2U, manager.mPolyarcs[1].arcs()[1].centerId());
  ASSERT_EQ(1., manager.mPolyarcs[1].arcs()[1].radius());
  ASSERT_TRUE(
    BasicMath::isEqual(
      4.1887902047863914,
      manager.mPolyarcs[1].arcs()[1].beginAngle().value()));
  ASSERT_TRUE(
    BasicMath::isEqual(
      0.52359877559829826,
      manager.mPolyarcs[1].arcs()[1].size()));

  //! Две замкнутые полидуги пересекаются.

  Arc closedArc1(
    Point2D(-1., 0.),
    1U,
    2.,
    Angle(0.),
    Angle(0.),
    false);
  closedArc1.setId(1U);
  Arc closedArc2(
    Point2D(1., 0.),
    2U,
    2.,
    Angle(0.),
    Angle(0.),
    false);
  closedArc2.setId(2U);
  manager.mPolyarcs = { Polyarc(closedArc1), Polyarc(closedArc2) };
  manager.mArcIdCounter = 3U;

  manager.intersectAllPolyarcs();

  ASSERT_EQ(1, manager.mPolyarcs.size());
  ASSERT_EQ(3, manager.mPolyarcs[0].arcs().size());

  ASSERT_EQ(Point2D(1., 0.), manager.mPolyarcs[0].arcs()[0].center());
  ASSERT_EQ(2U, manager.mPolyarcs[0].arcs()[0].centerId());
  ASSERT_EQ(2., manager.mPolyarcs[0].arcs()[0].radius());
  ASSERT_TRUE(
    BasicMath::isEqual(
      4.1887902047863914,
      manager.mPolyarcs[0].arcs()[0].beginAngle().value()));
  ASSERT_TRUE(
    BasicMath::isEqual(
      2.0943951023931948,
      manager.mPolyarcs[0].arcs()[0].size()));

  ASSERT_EQ(Point2D(1., 0.), manager.mPolyarcs[0].arcs()[1].center());
  ASSERT_EQ(2U, manager.mPolyarcs[0].arcs()[1].centerId());
  ASSERT_EQ(2., manager.mPolyarcs[0].arcs()[1].radius());
  ASSERT_TRUE(
    BasicMath::isEqual(
      0.,
      manager.mPolyarcs[0].arcs()[1].beginAngle().value()));
  ASSERT_TRUE(
    BasicMath::isEqual(
      2.0943951023931953,
      manager.mPolyarcs[0].arcs()[1].size()));

  ASSERT_EQ(Point2D(-1., 0.), manager.mPolyarcs[0].arcs()[2].center());
  ASSERT_EQ(1U, manager.mPolyarcs[0].arcs()[2].centerId());
  ASSERT_EQ(2., manager.mPolyarcs[0].arcs()[2].radius());
  ASSERT_TRUE(
    BasicMath::isEqual(
      1.0471975511965979,
      manager.mPolyarcs[0].arcs()[2].beginAngle().value()));
  ASSERT_TRUE(
    BasicMath::isEqual(
      4.1887902047863896,
      manager.mPolyarcs[0].arcs()[2].size()));

  //! Незамкнутая полидуга пересекается с замкнутой.

  Arc almostClosedArc2(
    Point2D(1., 0.),
    2U,
    2.,
    Angle(M_PI + .001),
    Angle(M_PI - .001));
  almostClosedArc2.setBeginInfo(ByEdge);
  almostClosedArc2.setEndInfo(ByEdge);
  almostClosedArc2.setId(3U);
  manager.mPolyarcs = { Polyarc(closedArc1), Polyarc(almostClosedArc2) };

  manager.intersectAllPolyarcs();

  ASSERT_EQ(1, manager.mPolyarcs.size());
  ASSERT_EQ(2, manager.mPolyarcs[0].arcs().size());

  ASSERT_EQ(Point2D(-1., 0.), manager.mPolyarcs[0].arcs()[0].center());
  ASSERT_EQ(1U, manager.mPolyarcs[0].arcs()[0].centerId());
  ASSERT_EQ(2., manager.mPolyarcs[0].arcs()[0].radius());
  ASSERT_TRUE(
    BasicMath::isEqual(
      1.0471975511965979,
      manager.mPolyarcs[0].arcs()[0].beginAngle().value()));
  ASSERT_TRUE(
    BasicMath::isEqual(
      4.1887902047863896,
      manager.mPolyarcs[0].arcs()[0].size()));

  ASSERT_EQ(Point2D(1., 0.), manager.mPolyarcs[0].arcs()[1].center());
  ASSERT_EQ(2U, manager.mPolyarcs[0].arcs()[1].centerId());
  ASSERT_EQ(2., manager.mPolyarcs[0].arcs()[1].radius());
  ASSERT_TRUE(
    BasicMath::isEqual(
      4.1887902047863914,
      manager.mPolyarcs[0].arcs()[1].beginAngle().value()));
  ASSERT_TRUE(
    BasicMath::isEqual(
      4.1887902047863896,
      manager.mPolyarcs[0].arcs()[1].size()));

  //! Замкнутая полидуга пересекается с незамкнутой.

  manager.mPolyarcs = { Polyarc(almostClosedArc2), Polyarc(closedArc1) };

  manager.intersectAllPolyarcs();

  ASSERT_EQ(1, manager.mPolyarcs.size());
  ASSERT_EQ(2, manager.mPolyarcs[0].arcs().size());

  ASSERT_EQ(Point2D(1., 0.), manager.mPolyarcs[0].arcs()[0].center());
  ASSERT_EQ(2U, manager.mPolyarcs[0].arcs()[0].centerId());
  ASSERT_EQ(2., manager.mPolyarcs[0].arcs()[0].radius());
  ASSERT_TRUE(
    BasicMath::isEqual(
      4.1887902047863914,
      manager.mPolyarcs[0].arcs()[0].beginAngle().value()));
  ASSERT_TRUE(
    BasicMath::isEqual(
      4.1887902047863896,
      manager.mPolyarcs[0].arcs()[0].size()));

  ASSERT_EQ(Point2D(-1., 0.), manager.mPolyarcs[0].arcs()[1].center());
  ASSERT_EQ(1U, manager.mPolyarcs[0].arcs()[1].centerId());
  ASSERT_EQ(2., manager.mPolyarcs[0].arcs()[1].radius());
  ASSERT_TRUE(
    BasicMath::isEqual(
      1.0471975511965983,
      manager.mPolyarcs[0].arcs()[1].beginAngle().value()));
  ASSERT_TRUE(
    BasicMath::isEqual(
      4.1887902047863896,
      manager.mPolyarcs[0].arcs()[1].size()));


  //! Две незамкнутые полидуги пересекаются.

  Arc almostClosedArc1(
    Point2D(-1., 0.),
    1U,
    2.,
    Angle(.001),
    Angle(-.001));
  almostClosedArc1.setBeginInfo(ByEdge);
  almostClosedArc1.setEndInfo(ByEdge);
  almostClosedArc1.setId(4U);
  manager.mPolyarcs = { Polyarc(almostClosedArc2), Polyarc(almostClosedArc1) };

  manager.intersectAllPolyarcs();

  ASSERT_EQ(1, manager.mPolyarcs.size());
  ASSERT_EQ(2, manager.mPolyarcs[0].arcs().size());

  ASSERT_EQ(Point2D(1., 0.), manager.mPolyarcs[0].arcs()[0].center());
  ASSERT_EQ(2U, manager.mPolyarcs[0].arcs()[0].centerId());
  ASSERT_EQ(2., manager.mPolyarcs[0].arcs()[0].radius());
  ASSERT_TRUE(
    BasicMath::isEqual(
      4.1887902047863914,
      manager.mPolyarcs[0].arcs()[0].beginAngle().value()));
  ASSERT_TRUE(
    BasicMath::isEqual(
      4.1887902047863896,
      manager.mPolyarcs[0].arcs()[0].size()));

  ASSERT_EQ(Point2D(-1., 0.), manager.mPolyarcs[0].arcs()[1].center());
  ASSERT_EQ(1U, manager.mPolyarcs[0].arcs()[1].centerId());
  ASSERT_EQ(2., manager.mPolyarcs[0].arcs()[1].radius());
  ASSERT_TRUE(
    BasicMath::isEqual(
      1.0471975511965983,
      manager.mPolyarcs[0].arcs()[1].beginAngle().value()));
  ASSERT_TRUE(
    BasicMath::isEqual(
      4.1887902047863896,
      manager.mPolyarcs[0].arcs()[1].size()));

  //! Две замкнутые полидуги пересекаются (в другом порядке).

  manager.mPolyarcs = { Polyarc(closedArc2), Polyarc(closedArc1) };

  manager.intersectAllPolyarcs();

  ASSERT_EQ(1, manager.mPolyarcs.size());
  ASSERT_EQ(3, manager.mPolyarcs[0].arcs().size());

  ASSERT_EQ(Point2D(1., 0.), manager.mPolyarcs[0].arcs()[0].center());
  ASSERT_EQ(2U, manager.mPolyarcs[0].arcs()[0].centerId());
  ASSERT_EQ(2., manager.mPolyarcs[0].arcs()[0].radius());
  ASSERT_TRUE(
    BasicMath::isEqual(
      4.1887902047863914,
      manager.mPolyarcs[0].arcs()[0].beginAngle().value()));
  ASSERT_TRUE(
    BasicMath::isEqual(
      2.0943951023931948,
      manager.mPolyarcs[0].arcs()[0].size()));

  ASSERT_EQ(Point2D(1., 0.), manager.mPolyarcs[0].arcs()[1].center());
  ASSERT_EQ(2U, manager.mPolyarcs[0].arcs()[1].centerId());
  ASSERT_EQ(2., manager.mPolyarcs[0].arcs()[1].radius());
  ASSERT_TRUE(
    BasicMath::isEqual(
      0.,
      manager.mPolyarcs[0].arcs()[1].beginAngle().value()));
  ASSERT_TRUE(
    BasicMath::isEqual(
      2.0943951023931953,
      manager.mPolyarcs[0].arcs()[1].size()));

  ASSERT_EQ(Point2D(-1., 0.), manager.mPolyarcs[0].arcs()[2].center());
  ASSERT_EQ(1U, manager.mPolyarcs[0].arcs()[2].centerId());
  ASSERT_EQ(2., manager.mPolyarcs[0].arcs()[2].radius());
  ASSERT_TRUE(
    BasicMath::isEqual(
      1.0471975511965983,
      manager.mPolyarcs[0].arcs()[2].beginAngle().value()));
  ASSERT_TRUE(
    BasicMath::isEqual(
      4.1887902047863896,
      manager.mPolyarcs[0].arcs()[2].size()));

  //! Замкнутая полидуга пересекается с незамкнутой (в другом порядке).

  manager.mPolyarcs = { Polyarc(almostClosedArc1), Polyarc(closedArc2) };

  manager.intersectAllPolyarcs();

  ASSERT_EQ(1, manager.mPolyarcs.size());
  ASSERT_EQ(3, manager.mPolyarcs[0].arcs().size());

  ASSERT_EQ(Point2D(1., 0.), manager.mPolyarcs[0].arcs()[0].center());
  ASSERT_EQ(2U, manager.mPolyarcs[0].arcs()[0].centerId());
  ASSERT_EQ(2., manager.mPolyarcs[0].arcs()[0].radius());
  ASSERT_TRUE(
    BasicMath::isEqual(
      4.1887902047863914,
      manager.mPolyarcs[0].arcs()[0].beginAngle().value()));
  ASSERT_TRUE(
    BasicMath::isEqual(
      2.0943951023931948,
      manager.mPolyarcs[0].arcs()[0].size()));

  ASSERT_EQ(Point2D(1., 0.), manager.mPolyarcs[0].arcs()[1].center());
  ASSERT_EQ(2U, manager.mPolyarcs[0].arcs()[1].centerId());
  ASSERT_EQ(2., manager.mPolyarcs[0].arcs()[1].radius());
  ASSERT_TRUE(
    BasicMath::isEqual(
      0.,
      manager.mPolyarcs[0].arcs()[1].beginAngle().value()));
  ASSERT_TRUE(
    BasicMath::isEqual(
      2.0943951023931953,
      manager.mPolyarcs[0].arcs()[1].size()));

  ASSERT_EQ(Point2D(-1., 0.), manager.mPolyarcs[0].arcs()[2].center());
  ASSERT_EQ(1U, manager.mPolyarcs[0].arcs()[2].centerId());
  ASSERT_EQ(2., manager.mPolyarcs[0].arcs()[2].radius());
  ASSERT_TRUE(
    BasicMath::isEqual(
      1.0471975511965979,
      manager.mPolyarcs[0].arcs()[2].beginAngle().value()));
  ASSERT_TRUE(
    BasicMath::isEqual(
      4.1887902047863896,
      manager.mPolyarcs[0].arcs()[2].size()));

  //! Замкнутая полидуга пересекается с незамкнутой (в другом порядке).

  almostClosedArc2.setBeginEndAngles(Angle(-M_PI_2), Angle(M_PI));
  manager.mPolyarcs = { Polyarc(closedArc1), Polyarc(almostClosedArc2) };

  manager.intersectAllPolyarcs();

  ASSERT_TRUE(manager.mPolyarcs.empty());
}
//==============================================================================
/*!
 * \brief Тестирование формирования таблицы соответствий между границами
 * (определяются по id начальной точки) и набором граничных точек дуг,
 * лежащих на них.
 */
TEST_F(ArcsTest, ArcManagerCalcEdgesArcInfo)
{
  using namespace ReachableArea;

  //! Несколько полидуг обрываются квадратом.

  Polygon2D square;
  square.outer().resize(5);
  square.outer()[0] = { 1., 1. };
  square.outer()[1] = { 1., 5. };
  square.outer()[2] = { 5., 5. };
  square.outer()[3] = { 5., 1. };
  square.outer()[4] = square.outer()[0];
  GeometryEnvironment env({ square });
  ArcManagerPrivate   manager(env);
  Arc                 arc(
    Point2D(0., 0.),
    1U,
    2.,
    Angle(std::acos(.5)),
    Angle(std::asin(.5)));
  arc.setBeginInfo(ByEdge, 1U);
  arc.setEndInfo(ByEdge, 4U);
  Arc                 arc2(
    Point2D(6., 0.),
    2U,
    2.,
    Angle(M_PI - std::asin(.5)),
    Angle(M_PI - std::acos(.5)));
  arc2.setBeginInfo(ByEdge, 4U);
  arc2.setEndInfo(ByEdge, 3U);
  manager.push({ arc, arc2 });
  ArcManagerPrivate::EdgesArcInfo result;

  manager.calcEdgesArcInfo(result);
  ASSERT_EQ(3, result.size());
  ASSERT_EQ(1, result[1U].size());
  ASSERT_EQ(1, result[3U].size());
  ASSERT_EQ(2, result[4U].size());
  ASSERT_EQ(0U, result[1U][0].arcPos);
  ASSERT_TRUE(result[1U][0].beginPoint);
  ASSERT_TRUE(
    BasicMath::isEqual(std::sqrt(3) - 1., result[1U][0].distToEdgeBegin));
  ASSERT_EQ(1U, result[3U][0].arcPos);
  ASSERT_FALSE(result[3U][0].beginPoint);
  ASSERT_TRUE(
    BasicMath::isEqual(5. - std::sqrt(3), result[3U][0].distToEdgeBegin));
  ASSERT_EQ(1U, result[4U][0].arcPos);
  ASSERT_TRUE(result[4U][0].beginPoint);
  ASSERT_TRUE(
    BasicMath::isEqual(std::sqrt(3) - 1., result[4U][0].distToEdgeBegin));
  ASSERT_EQ(0U, result[4U][1].arcPos);
  ASSERT_FALSE(result[4U][1].beginPoint);
  ASSERT_TRUE(
    BasicMath::isEqual(5. - std::sqrt(3), result[4U][1].distToEdgeBegin));

  //! Замкнутая полидуга.
  
  Arc closedArc(
    Point2D(0., 0.),
    2U,
    1.,
    Angle(0.),
    Angle(0.),
    false);
  manager.mPolyarcs.clear();
  manager.push({ closedArc });

  manager.calcEdgesArcInfo(result);
  ASSERT_TRUE(result.empty());

  //! Полидуга, обрывающаяся не из-за окружения.

  arc.setBeginInfo(ByCut);
  manager.mPolyarcs.clear();
  manager.push({ arc });

  bool isFailureThrown(false);
  try
  {
    manager.calcEdgesArcInfo(result);
  }
  catch (const ReachableAreaFailure& ex)
  {
    isFailureThrown = true;
    ASSERT_EQ(reachableAreaLibTitle, ex.getLib());
    ASSERT_EQ(ArcsPolyarcBreakNoByEdge, ex.getCode());
  }
  ASSERT_TRUE(isFailureThrown);

  //! Полидуга с нулевым идентификатором границы, обрывающей её начало.

  arc.setBeginInfo(ByEdge, 0U);
  manager.mPolyarcs.clear();
  manager.push({ arc });

  isFailureThrown = false;
  try
  {
    manager.calcEdgesArcInfo(result);
  }
  catch (const ReachableAreaFailure& ex)
  {
    isFailureThrown = true;
    ASSERT_EQ(reachableAreaLibTitle, ex.getLib());
    ASSERT_EQ(ArcsPolyarcNullBeginInfo, ex.getCode());
  }
  ASSERT_TRUE(isFailureThrown);

  //! Полидуга с нулевым идентификатором границы, обрывающей её конец.

  arc.setBeginInfo(ByEdge, 1U);
  arc.setEndInfo(ByEdge, 0U);
  manager.mPolyarcs.clear();
  manager.push({ arc });

  isFailureThrown = false;
  try
  {
    manager.calcEdgesArcInfo(result);
  }
  catch (const ReachableAreaFailure& ex)
  {
    isFailureThrown = true;
    ASSERT_EQ(reachableAreaLibTitle, ex.getLib());
    ASSERT_EQ(ArcsPolyarcNullEndInfo, ex.getCode());
  }
  ASSERT_TRUE(isFailureThrown);
}
//==============================================================================
/*!
 * \brief Тестирование формирование набора актуальностей полидуг.
 */
TEST_F(ArcsTest, ArcManagerCalcPolyarcActualities)
{
  using namespace ReachableArea;

  //! Полидуга обрывается квадратом; также присутствует замкнутая полидуга.

  Polygon2D square;
  square.outer().resize(5);
  square.outer()[0] = { 1., 1. };
  square.outer()[1] = { 1., 5. };
  square.outer()[2] = { 5., 5. };
  square.outer()[3] = { 5., 1. };
  square.outer()[4] = square.outer()[0];
  GeometryEnvironment env({ square });
  ArcManagerPrivate   manager(env);
  Arc                 arc(
    Point2D(0., 0.),
    1U,
    2.,
    Angle(std::acos(.5)),
    Angle(std::asin(.5)));
  arc.setBeginInfo(ByEdge, 1U);
  arc.setEndInfo(ByEdge, 4U);
  Arc closedArc(
    Point2D(10., 10.),
    2U,
    1.,
    Angle(0.),
    Angle(0.),
    false);
  manager.push({ arc, closedArc });
  ArcManagerPrivate::EdgesArcInfo edgesArcInfo;
  manager.calcEdgesArcInfo(edgesArcInfo);
  std::vector<bool> result;

  manager.calcPolyarcsActualities(edgesArcInfo, result);
  ASSERT_EQ(2, result.size());
  ASSERT_TRUE(result[0]);
  ASSERT_TRUE(result[1]);

  //! Неопознанный номер границы окружения у дуги.

  Arc fakeArc(arc);
  fakeArc.setBeginInfo(ByEdge, 100U);
  manager.mPolyarcs = { fakeArc, closedArc };

  bool isFailureThrown(false);
  try
  {
    manager.calcPolyarcsActualities(edgesArcInfo, result);
  }
  catch (const ReachableAreaFailure& ex)
  {
    isFailureThrown = true;
    ASSERT_EQ(reachableAreaLibTitle, ex.getLib());
    ASSERT_EQ(ArcsPolyarcBeginPointNotFound, ex.getCode());
  }
  ASSERT_TRUE(isFailureThrown);

  //! Одна полидуга окружает другую полидугу.

  Arc bigArc(
    Point2D(0., 0.),
    3U,
    3.,
    Angle(std::acos(1. / 3.)),
    Angle(std::asin(1. / 3.)));
  bigArc.setBeginInfo(ByEdge, 1U);
  bigArc.setEndInfo(ByEdge, 4U);
  manager.mPolyarcs.clear();
  manager.push({ arc, bigArc });
  manager.calcEdgesArcInfo(edgesArcInfo);

  manager.calcPolyarcsActualities(edgesArcInfo, result);
  ASSERT_EQ(2, result.size());
  ASSERT_FALSE(result[0]);
  ASSERT_TRUE(result[1]);
}
//==============================================================================
/*!
 * \brief Тестирование определения направления полигона.
 */
TEST_F(ArcsTest, ArcManagerIsPolygonCW)
{
  std::vector<Point2D> polygon{
    {1., 1.},
    {1., 5.},
    {5., 5.},
    {5., 1.},
    {1., 1.}
  };
  GeometryEnvironment  env;
  ArcManagerPrivate    manager(env);

  ASSERT_TRUE(manager.isPolygonCW(polygon));
}
//==============================================================================
/*!
 * \brief Тестирование метода, сбрасывающего всю информацию о дугах в объекте.
 */
TEST_F(ArcsTest, ArcManagerClearArcs)
{
  GeometryEnvironment env;
  ArcManagerPrivate   manager(env);
  Arc                 arc(
    Point2D(0., 0.),
    1U,
    2.,
    Angle(std::acos(.5)),
    Angle(std::asin(.5)));
  arc.setBeginInfo(ByEdge, 1U);
  arc.setEndInfo(ByEdge, 4U);
  Arc closedArc(
    Point2D(10., 10.),
    2U,
    1.,
    Angle(0.),
    Angle(0.),
    false);
  manager.push({ arc, closedArc });

  manager.clearArcs();
  ASSERT_TRUE(manager.mPolyarcs.empty());
  ASSERT_EQ(1U, manager.mArcIdCounter);
}
//==============================================================================
} //! Tests
} //! ReachableAreaCalculator
//==============================================================================
