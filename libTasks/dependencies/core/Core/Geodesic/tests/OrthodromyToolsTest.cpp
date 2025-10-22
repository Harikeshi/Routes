#include <Geodesic/GeoLineTools/OrthodromyTools.hpp>

#include <Geodesic/GeodesicException.hpp>
//! MathTools
#include <MathTools/BasicMath/Compare.hpp>
//! gtest
#include <gtest/gtest.h>
//==============================================================================
namespace Geodesic
{
namespace Tests
{
//==============================================================================
constexpr double ACCURACY_DISTANCE(.01); //!< точность расчёта расстояний
//==============================================================================
/*!
* \brief Тестирование конструктора
*/
TEST(OrthodromyToolsTest, Constructor)
{
  using namespace BasicMath;

  //! WGS-84
  Ellipsoid ellipsoid(EL_WGS_84);      //!< создание эллипсоида 
  OrthodromyTools orthodromyTools;  //!< создание инструментов для ортодромии

  ASSERT_TRUE(isEqual(ellipsoid.getMajorRadius(), 
                      orthodromyTools.getMajorRadius()));
  ASSERT_TRUE(isEqual(ellipsoid.getFlatenning(), 
                      orthodromyTools.getFlatenning()));

  //! СК-42
  ellipsoid = Ellipsoid(EL_KRASOVSKY);
  orthodromyTools = OrthodromyTools(ellipsoid);

  ASSERT_TRUE(isEqual(ellipsoid.getMajorRadius(), 
                      orthodromyTools.getMajorRadius()));
  ASSERT_TRUE(isEqual(ellipsoid.getFlatenning(), 
                      orthodromyTools.getFlatenning()));

  //! ПЗ-90
  ellipsoid = Ellipsoid(EL_GENERAL_GLOBE);
  orthodromyTools = OrthodromyTools(Ellipsoid(EL_GENERAL_GLOBE));

  ASSERT_TRUE(isEqual(ellipsoid.getMajorRadius(), 
                      orthodromyTools.getMajorRadius()));
  ASSERT_TRUE(isEqual(ellipsoid.getFlatenning(), 
                      orthodromyTools.getFlatenning()));
}
//==============================================================================
/*!
* \brief Тестирование создания ортодромии по двум точкам
*/
TEST(OrthodromyToolsTest, CreateOrthodromyByPoints)
{
  using namespace BasicMath;

  //! Создаем инструменты для эллипсоида WGS-84
  OrthodromyTools orthodromyTools;

  //! Проверка основных параметров на различных случая.

  //! Ортодромия: линия в 10° в районе экватора
  Orthodromy orthodromy = orthodromyTools.createOrthodromy(
                                            GeoPoint2D(),
                                            GeoPoint2D(Degree(0), Degree(10)));
  ASSERT_EQ(GeoPoint2D(), orthodromy.getStartPoint());
  ASSERT_EQ(GeoPoint2D(Degree(0), Degree(10)), orthodromy.getFinishPoint());
  ASSERT_TRUE(isEqual(1113194.907932736, orthodromy.getLength(), 
                      ACCURACY_DISTANCE));
  ASSERT_EQ(Radian(M_PI_2), orthodromy.getAzimuthAtStart());
  ASSERT_EQ(Radian(M_PI_2), orthodromy.getAzimuthAtFinish());

  //! Ортодромия: линия в 10° в районе экватора, пересекающая 180 меридиан
  orthodromy = orthodromyTools.createOrthodromy(
                                            GeoPoint2D(Degree(0), Degree(175)),
                                            GeoPoint2D(Degree(0), Degree(185)));
  ASSERT_EQ(GeoPoint2D(Degree(0), Degree(175)), orthodromy.getStartPoint());
  ASSERT_EQ(GeoPoint2D(Degree(0), Degree(-175)), orthodromy.getFinishPoint());
  ASSERT_TRUE(isEqual(1113194.907932736, orthodromy.getLength(), 
                      ACCURACY_DISTANCE));
  ASSERT_EQ(Radian(M_PI_2), orthodromy.getAzimuthAtStart());
  ASSERT_EQ(Radian(M_PI_2), orthodromy.getAzimuthAtFinish());

  //! Ортодромия: линия в 10° на широте 45°
  orthodromy = orthodromyTools.createOrthodromy(
                                            GeoPoint2D(Degree(45), Degree(0)),
                                            GeoPoint2D(Degree(45), Degree(10)));
  ASSERT_EQ(GeoPoint2D(Degree(45), Degree(0)), orthodromy.getStartPoint());
  ASSERT_EQ(GeoPoint2D(Degree(45), Degree(10)), orthodromy.getFinishPoint());
  ASSERT_TRUE(isEqual(787967.303606826, orthodromy.getLength(), 
                      ACCURACY_DISTANCE));
  ASSERT_EQ(Degree(86.45996013285624).toRadians(), 
            orthodromy.getAzimuthAtStart());
  ASSERT_EQ(Degree(93.54003986714376).toRadians(), 
            orthodromy.getAzimuthAtFinish());

  //! Ортодромия: линия в 10° на полюсе (на широте 90°)
  orthodromy = orthodromyTools.createOrthodromy(
                                            GeoPoint2D(Degree(90), Degree(0)),
                                            GeoPoint2D(Degree(90), Degree(10)));
  ASSERT_EQ(GeoPoint2D(Degree(90), Degree(0)), orthodromy.getStartPoint());
  ASSERT_EQ(GeoPoint2D(Degree(90), Degree(10)), orthodromy.getFinishPoint());
  ASSERT_TRUE(isEqual(0., orthodromy.getLength(), ACCURACY_DISTANCE));
  ASSERT_EQ(Degree(170.).toRadians(), orthodromy.getAzimuthAtStart());
  ASSERT_EQ(Degree(-180.).toRadians(), orthodromy.getAzimuthAtFinish());

  //! Ортодромия: линия в 10° на меридиане
  orthodromy = orthodromyTools.createOrthodromy(
                                            GeoPoint2D(Degree(0), Degree(0)),
                                            GeoPoint2D(Degree(10), Degree(0)));
  ASSERT_EQ(GeoPoint2D(Degree(0), Degree(0)), orthodromy.getStartPoint());
  ASSERT_EQ(GeoPoint2D(Degree(10), Degree(0)), orthodromy.getFinishPoint());
  ASSERT_TRUE(isEqual(1105854.833234372, orthodromy.getLength(), 
                      ACCURACY_DISTANCE));
  ASSERT_EQ(Degree(0.).toRadians(), orthodromy.getAzimuthAtStart());
  ASSERT_EQ(Degree(0.).toRadians(), orthodromy.getAzimuthAtFinish());

  //! Ортодромия: через полюс в 10° на меридиане
  orthodromy = orthodromyTools.createOrthodromy(
                                          GeoPoint2D(Degree(85), Degree(0)),
                                          GeoPoint2D(Degree(85), Degree(180)));
  ASSERT_EQ(GeoPoint2D(Degree(85), Degree(0)), orthodromy.getStartPoint());
  ASSERT_EQ(GeoPoint2D(Degree(85), Degree(-180)), orthodromy.getFinishPoint());
  ASSERT_TRUE(isEqual(1116911.177292954, orthodromy.getLength(), 
                      ACCURACY_DISTANCE));
  ASSERT_EQ(Degree(0.).toRadians(), orthodromy.getAzimuthAtStart());
  ASSERT_EQ(Degree(-180.).toRadians(), orthodromy.getAzimuthAtFinish());

  //! Ортодромия: случайная
  orthodromy = orthodromyTools.createOrthodromy(
                                          GeoPoint2D(Degree(-75), Degree(-165)),
                                          GeoPoint2D(Degree(85), Degree(125)));
  ASSERT_EQ(GeoPoint2D(Degree(-75), Degree(-165)), orthodromy.getStartPoint());
  ASSERT_EQ(GeoPoint2D(Degree(85), Degree(125)), orthodromy.getFinishPoint());
  ASSERT_TRUE(isEqual(18067319.410595290, orthodromy.getLength(), 
                      ACCURACY_DISTANCE));
  ASSERT_EQ(Degree(-15.95650231241479).toRadians(), 
            orthodromy.getAzimuthAtStart());
  ASSERT_EQ(Degree(-54.70681059354465).toRadians(), 
            orthodromy.getAzimuthAtFinish());

  //! Ортодромия: точка на пересечении экватора и гринвича
  ASSERT_THROW(orthodromyTools.createOrthodromy(GeoPoint2D(), GeoPoint2D()),
               GeodesicFailure);
  try
  {
    orthodromy = orthodromyTools.createOrthodromy(GeoPoint2D(), GeoPoint2D());
  }
  catch (const GeodesicFailure& exception)
  {
    ASSERT_EQ(
      GeodesicEnumFailure::OrthodromyThroughOnePoint,
      exception.getCode());
    ASSERT_EQ(geodesicLibTitle, exception.getLib());
  }
}
//==============================================================================
/*!
* \brief Тестирование создания локсодромии по точке, азимуту и протяженности
*/
TEST(OrthodromyToolsTest, CreateOrthodromyByParams)
{
  using namespace BasicMath;

  //! Создаем инструменты для эллипсоида WGS-84
  OrthodromyTools orthodromyTools;

  //! Проверка основных параметров на различных случаях

  //! Ортодромия: луч от точки на пересечении экватора и гринвича на север
  Orthodromy orthodromy = orthodromyTools.createOrthodromy(GeoPoint2D(),
                                                           Radian(0.), 0.);
  
  ASSERT_EQ(GeoPoint2D(), orthodromy.getStartPoint());
  ASSERT_EQ(GeoPoint2D(), orthodromy.getFinishPoint());
  ASSERT_TRUE(isEqual(0., orthodromy.getLength(), ACCURACY_DISTANCE));
  ASSERT_EQ(Degree(0.).toRadians(), orthodromy.getAzimuthAtStart());
  ASSERT_EQ(Degree(0.).toRadians(), orthodromy.getAzimuthAtFinish());

  //! Ортодромия: линия в 10° в районе экватора
  orthodromy = orthodromyTools.createOrthodromy(GeoPoint2D(), Radian(M_PI_2),
                                                1113194.907932736);
  ASSERT_EQ(GeoPoint2D(), orthodromy.getStartPoint());
  ASSERT_EQ(GeoPoint2D(Degree(0), Degree(10)), orthodromy.getFinishPoint());
  ASSERT_TRUE(isEqual(1113194.907932736, orthodromy.getLength(), 
                      ACCURACY_DISTANCE));
  ASSERT_EQ(Radian(M_PI_2), orthodromy.getAzimuthAtStart());
  ASSERT_EQ(Radian(M_PI_2), orthodromy.getAzimuthAtFinish());

  //! Ортодромия: линия в 10° в районе экватора, пересекающая 180 меридиан
  orthodromy = orthodromyTools.createOrthodromy(
                                            GeoPoint2D(Degree(0), Degree(175)),
                                            Radian(M_PI_2), 1113194.907932736);
  ASSERT_EQ(GeoPoint2D(Degree(0), Degree(175)), orthodromy.getStartPoint());
  ASSERT_EQ(GeoPoint2D(Degree(0), Degree(-175)), orthodromy.getFinishPoint());
  ASSERT_TRUE(isEqual(1113194.907932736, orthodromy.getLength(), 
                      ACCURACY_DISTANCE));
  ASSERT_EQ(Radian(M_PI_2), orthodromy.getAzimuthAtStart());
  ASSERT_EQ(Radian(M_PI_2), orthodromy.getAzimuthAtFinish());

  //! Ортодромия: линия в 10° на широте 45°
  orthodromy = orthodromyTools.createOrthodromy(
                      GeoPoint2D(Degree(45), Degree(0)),
                      Degree(86.45996013285624).toRadians(), 787967.303606826);
  ASSERT_EQ(GeoPoint2D(Degree(45), Degree(0)), orthodromy.getStartPoint());
  ASSERT_EQ(GeoPoint2D(Degree(45), Degree(10)), orthodromy.getFinishPoint());
  ASSERT_TRUE(isEqual(787967.303606826, orthodromy.getLength(), 
                      ACCURACY_DISTANCE));
  ASSERT_EQ(Degree(86.45996013285624).toRadians(), 
            orthodromy.getAzimuthAtStart());
  ASSERT_EQ(Degree(93.54003986714376).toRadians(), 
            orthodromy.getAzimuthAtFinish());

  //! Ортодромия: луч вдоль полюса (на широте 90°)
  orthodromy = orthodromyTools.createOrthodromy(
                                            GeoPoint2D(Degree(90), Degree(0)),
                                            Radian(M_PI_2), 0.);
  ASSERT_EQ(GeoPoint2D(Degree(90), Degree(0)), orthodromy.getStartPoint());
  ASSERT_EQ(GeoPoint2D(Degree(90), Degree(0)), orthodromy.getFinishPoint());
  ASSERT_TRUE(isEqual(0., orthodromy.getLength(), ACCURACY_DISTANCE));
  ASSERT_EQ(Degree(90).toRadians(), orthodromy.getAzimuthAtStart());
  ASSERT_EQ(Degree(90).toRadians(), orthodromy.getAzimuthAtFinish());

    //! Ортодромия: линия в 10° на меридиане
  orthodromy = orthodromyTools.createOrthodromy(
                                            GeoPoint2D(Degree(0), Degree(0)),
                                            Radian(0.), 1105854.833234372);
  ASSERT_EQ(GeoPoint2D(Degree(0), Degree(0)), orthodromy.getStartPoint());
  ASSERT_EQ(GeoPoint2D(Degree(10), Degree(0)), orthodromy.getFinishPoint());
  ASSERT_TRUE(isEqual(1105854.833234372, orthodromy.getLength(), 
                      ACCURACY_DISTANCE));
  ASSERT_EQ(Degree(0.).toRadians(), orthodromy.getAzimuthAtStart());
  ASSERT_EQ(Degree(0.).toRadians(), orthodromy.getAzimuthAtFinish());

  //! Ортодромия: через полюс в 10° на меридиане
  orthodromy = orthodromyTools.createOrthodromy(
                                          GeoPoint2D(Degree(85), Degree(0)),
                                          Radian(0), 1116911.177292954);
  ASSERT_EQ(GeoPoint2D(Degree(85), Degree(0)), orthodromy.getStartPoint());
  ASSERT_EQ(GeoPoint2D(Degree(85), Degree(-180)), orthodromy.getFinishPoint());
  ASSERT_TRUE(isEqual(1116911.177292954, orthodromy.getLength(), 
                      ACCURACY_DISTANCE));
  ASSERT_EQ(Degree(0.).toRadians(), orthodromy.getAzimuthAtStart());
  ASSERT_EQ(Degree(-180.).toRadians(), orthodromy.getAzimuthAtFinish());

  //! Ортодромия: случайная
  orthodromy = orthodromyTools.createOrthodromy(
                    GeoPoint2D(Degree(-75), Degree(-165)),
                    Degree(-15.95650231241479).toRadians(), 18067319.410595290);
  ASSERT_EQ(GeoPoint2D(Degree(-75), Degree(-165)), orthodromy.getStartPoint());
  ASSERT_EQ(GeoPoint2D(Degree(85), Degree(125)), orthodromy.getFinishPoint());
  ASSERT_TRUE(isEqual(18067319.410595290, orthodromy.getLength(), 
                      ACCURACY_DISTANCE));
  ASSERT_EQ(Degree(-15.95650231241479).toRadians(), 
            orthodromy.getAzimuthAtStart());
  ASSERT_EQ(Degree(-54.70681059354465).toRadians(), 
            orthodromy.getAzimuthAtFinish());
}
//==============================================================================
/*!
* \brief Тестирование поиска на ортодромии точки, отстоящей от заданной
*/
TEST(OrthodromyToolsTest, PointPosition)
{
  //! Создаем инструменты для эллипсоида WGS-84
  OrthodromyTools orthodromyTools;

  ASSERT_EQ(GeoPoint2D(Degree(10), Degree(0)),
            orthodromyTools.pointPosition(GeoPoint2D(), Radian(0.),
                                          1105854.833234372));

  ASSERT_EQ(GeoPoint2D(Degree(-10), Degree(0)),
            orthodromyTools.pointPosition(GeoPoint2D(), Radian(0.),
                                          -1105854.833234372));

  ASSERT_EQ(GeoPoint2D(Degree(0), Degree(-175)),
            orthodromyTools.pointPosition(GeoPoint2D(Degree(0), Degree(175)),
                                          Radian(M_PI_2), 1113194.907932736));

  ASSERT_EQ(GeoPoint2D(Degree(0), Degree(175)),
            orthodromyTools.pointPosition(GeoPoint2D(Degree(0), Degree(-175)),
                                          Radian(-M_PI_2), 1113194.907932736));

  ASSERT_EQ(GeoPoint2D(Degree(80), Degree(0)),
            orthodromyTools.pointPosition(GeoPoint2D(Degree(90), Degree(0)),
                                          Radian(M_PI), 1116825.857375850));

  ASSERT_EQ(GeoPoint2D(Degree(85), Degree(0)),
            orthodromyTools.pointPosition(GeoPoint2D(Degree(85), Degree(-180)),
                                          Radian(0.), 1116911.177292954));

  ASSERT_EQ(GeoPoint2D(Degree(0), Degree(10)), 
            orthodromyTools.pointPosition(GeoPoint2D(), 
                                          GeoPoint2D(Degree(0), Degree(20))));

  ASSERT_EQ(GeoPoint2D(Degree(0), Degree(5)), 
            orthodromyTools.pointPosition(GeoPoint2D(), 
                                          GeoPoint2D(Degree(0), Degree(20)), 
                                          .25));

  ASSERT_EQ(
    GeoPoint2D(),
    orthodromyTools.pointPosition(GeoPoint2D(), GeoPoint2D(), .6666666666));
}
//==============================================================================
/*!
 * \brief Тестирование поиска середины ортодромии между двумя точками.
 */
TEST(OrthodromyToolsTest, MiddlePoint)
{
  //! Создаем инструменты для эллипсоида WGS-84
  OrthodromyTools orthodromyTools;

  //! Обычный случай.
  ASSERT_EQ(GeoPoint2D(Degree(0), Degree(4)),
    orthodromyTools.middlePoint(GeoPoint2D(),
      GeoPoint2D(Degree(0), Degree(8))));

  //! Случай, когда мы ищем середину между двумя одинаковыми точками.
  ASSERT_EQ(
    GeoPoint2D(),
    orthodromyTools.middlePoint(GeoPoint2D(), GeoPoint2D()));

}
//==============================================================================
/*!
* \brief Тестирование поиска расстояния между двумя точками по ортодромии
*/
TEST(OrthodromyToolsTest, Distance)
{
  using namespace BasicMath;

  //! Создаем инструменты для эллипсоида WGS-84
  OrthodromyTools orthodromyTools;

  ASSERT_TRUE(isEqual(1105854.833234372,
                      orthodromyTools.distance(
                        GeoPoint2D(), 
                        GeoPoint2D(Degree(10), Degree(0))),
                      ACCURACY_DISTANCE));

  ASSERT_TRUE(isEqual(1105854.833234372,
                      orthodromyTools.distance(
                        GeoPoint2D(), 
                        GeoPoint2D(Degree(-10), Degree(0))),
                      ACCURACY_DISTANCE));

  ASSERT_TRUE(isEqual(1113194.907932736,
                      orthodromyTools.distance(
                        GeoPoint2D(Degree(0), Degree(175)),
                        GeoPoint2D(Degree(0), Degree(-175))),
                      ACCURACY_DISTANCE));

  ASSERT_TRUE(isEqual(1113194.907932736,
                      orthodromyTools.distance(
                        GeoPoint2D(Degree(0), Degree(-175)),
                        GeoPoint2D(Degree(0), Degree(175))),
                      ACCURACY_DISTANCE));

  ASSERT_TRUE(isEqual(1116825.857375850,
                      orthodromyTools.distance(
                        GeoPoint2D(Degree(90), Degree(0)),
                        GeoPoint2D(Degree(80), Degree(0))),
                      ACCURACY_DISTANCE));

  ASSERT_TRUE(isEqual(1116911.177292954,
                      orthodromyTools.distance(
                        GeoPoint2D(Degree(85), Degree(-180)),
                        GeoPoint2D(Degree(85), Degree(0))),
                      ACCURACY_DISTANCE));
}
//==============================================================================
/*!
* \brief Тестирование поиска протяжённости между набором точек по ортодромиям
*/
TEST(OrthodromyToolsTest, LenghtByPoints)
{
  using namespace BasicMath;

  //! Создаем инструменты для эллипсоида WGS-84
  OrthodromyTools orthodromyTools;

  std::vector<GeoPoint2D> points;
  std::vector<double> lengths;

  points.emplace_back(GeoPoint2D(Degree( 10), Degree(0)));

  //! Набор из одной точки
  ASSERT_TRUE(isEqual(0., orthodromyTools.length(points, lengths),
                      ACCURACY_DISTANCE));
  ASSERT_EQ(0, lengths.size());

  //! Набор из трёх точек
  points.emplace_back(GeoPoint2D(Degree( 0 ), Degree(0)));
  points.emplace_back(GeoPoint2D(Degree(-10), Degree(0)));


  ASSERT_TRUE(isEqual(2211709.666468744, 
                      orthodromyTools.length(points, lengths),
                      ACCURACY_DISTANCE));
  ASSERT_TRUE(isEqual(1105854.833234372, lengths[0], ACCURACY_DISTANCE));
  ASSERT_TRUE(isEqual(1105854.833234372, lengths[1], ACCURACY_DISTANCE));

  //! Набор из трёх точек без подсчёта длин сегментов
  ASSERT_TRUE(isEqual(2211709.666468744, orthodromyTools.length(points),
                      ACCURACY_DISTANCE));
}
//==============================================================================
/*!
* \brief Тестирование поиска протяжённости ломаной по ортодромиям
*/
TEST(OrthodromyToolsTest, LenghtByLineString)
{
  using namespace BasicMath;

  //! Создаем инструменты для эллипсоида WGS-84
  OrthodromyTools orthodromyTools;

  GeoLinestring2D line;
  line.emplace_back(GeoPoint2D(Degree( 10), Degree(0)));

  //! Ломаная из одной точки
  std::vector<double> lengths;
  ASSERT_TRUE(isEqual(0., orthodromyTools.length(line, lengths),
                      ACCURACY_DISTANCE));

  line.emplace_back(GeoPoint2D(Degree( 0 ), Degree(0)));
  line.emplace_back(GeoPoint2D(Degree(-10), Degree(0)));

  //! Ломаная из трёх точек
  ASSERT_TRUE(isEqual(2211709.666468744, orthodromyTools.length(line, lengths),
                      ACCURACY_DISTANCE));
  ASSERT_TRUE(isEqual(1105854.833234372, lengths[0], ACCURACY_DISTANCE));
  ASSERT_TRUE(isEqual(1105854.833234372, lengths[1], ACCURACY_DISTANCE));

  //! Ломаная из трёх точек без подсчёта длин сегментов
  ASSERT_TRUE(isEqual(2211709.666468744, orthodromyTools.length(line),
                      ACCURACY_DISTANCE));
}
//==============================================================================
/*!
* \brief Тестирование разбиения ортодромии из двух точек на равные части
*/
TEST(OrthodromyToolsTest, DivideByTwoPoints)
{
  //! Создаем инструменты для эллипсоида WGS-84
  OrthodromyTools orthodromyTools;

  //! Разбиение ортодромии, проходящей вдоль меридиана через полюс.
  //! Изначально задана длина неравномерного разбиения - выравнивание
  auto points = orthodromyTools.divide(GeoPoint2D(Degree(85), Degree(0)),
                                       GeoPoint2D(Degree(85), Degree(180)),
                                       600000.);

  ASSERT_EQ(3, points.size());
  ASSERT_EQ(GeoPoint2D(Degree(85), Degree(0)), points[0]);
  ASSERT_EQ(GeoPoint2D(Degree(90), Degree(-180)), points[1]);
  ASSERT_EQ(GeoPoint2D(Degree(85), Degree(-180)), points[2]);

  //! Разбиение ортодромии, проходящей через две одинаковые точки.

  points = orthodromyTools.divide(GeoPoint2D(Degree(85), Degree(0)),
                                  GeoPoint2D(Degree(85), Degree(0)),
                                  600000.);
  ASSERT_EQ(2, points.size());
  ASSERT_EQ(GeoPoint2D(Degree(85), Degree(0)), points[0]);
  ASSERT_EQ(GeoPoint2D(Degree(85), Degree(0)), points[1]);
}
//==============================================================================
/*!
* \brief Тестирование разбиения ортодромий заданных набором точек на равные 
*        части с заменой исходного набора на результат
*/
TEST(OrthodromyToolsTest, DivideByVectorOfPoints)
{
  //! Создаем инструменты для эллипсоида WGS-84
  OrthodromyTools orthodromyTools;

  //! Ортодромии заданы набором из трёх точек: на пересечение гринвича и 
  //! экватора.
  std::vector<GeoPoint2D> points;
  points.emplace_back(GeoPoint2D(Degree(10), Degree( 0)));
  points.emplace_back(GeoPoint2D(Degree( 0), Degree( 0)));
  points.emplace_back(GeoPoint2D(Degree( 0), Degree(10)));

  orthodromyTools.divide(points, 600000.);
                                
  ASSERT_EQ(5, points.size());
  ASSERT_EQ(GeoPoint2D(Degree(10               ), Degree( 0)), points[0]);
  ASSERT_EQ(GeoPoint2D(Degree( 5.00037949475414), Degree( 0)), points[1]);
  ASSERT_EQ(GeoPoint2D(Degree( 0               ), Degree( 0)), points[2]);
  ASSERT_EQ(GeoPoint2D(Degree( 0               ), Degree( 5)), points[3]);
  ASSERT_EQ(GeoPoint2D(Degree( 0               ), Degree(10)), points[4]);

  //! Ортодромии заданы набором из пяти точек: на пересечение гринвича и 
  //! экватора, где граничные задублированы.
  points.clear();
  points.emplace_back(GeoPoint2D(Degree(10), Degree(0)));
  points.emplace_back(GeoPoint2D(Degree(10), Degree(0)));
  points.emplace_back(GeoPoint2D(Degree(0), Degree(0)));
  points.emplace_back(GeoPoint2D(Degree(0), Degree(10)));
  points.emplace_back(GeoPoint2D(Degree(0), Degree(10)));

  orthodromyTools.divide(points, 600000.);

  ASSERT_EQ(7, points.size());
  ASSERT_EQ(GeoPoint2D(Degree(10), Degree(0)), points[0]);
  ASSERT_EQ(GeoPoint2D(Degree(10), Degree(0)), points[1]);
  ASSERT_EQ(GeoPoint2D(Degree(5.00037949475414), Degree(0)), points[2]);
  ASSERT_EQ(GeoPoint2D(Degree(0), Degree(0)), points[3]);
  ASSERT_EQ(GeoPoint2D(Degree(0), Degree(5)), points[4]);
  ASSERT_EQ(GeoPoint2D(Degree(0), Degree(10)), points[5]);
  ASSERT_EQ(GeoPoint2D(Degree(0), Degree(10)), points[6]);
}
//==============================================================================
/*!
* \brief Тестирование разбиения ортодромий заданных набором точек на равные
*        части без замены исходного набора
*/
TEST(OrthodromyToolsTest, DivideByVectorOfPointsWithoutChange)
{
  //! Создаем инструменты для эллипсоида WGS-84
  OrthodromyTools orthodromyTools;

  //! Ортодромии заданы набором из трёх точек: на пересечение гринвича и 
  //! экватора.
  std::vector<GeoPoint2D> points;
  points.emplace_back(GeoPoint2D(Degree(10), Degree( 0)));
  points.emplace_back(GeoPoint2D(Degree( 0), Degree( 0)));
  points.emplace_back(GeoPoint2D(Degree( 0), Degree(10)));

  std::vector<GeoPoint2D> result;

  orthodromyTools.divide(points, result, 600000.);

  ASSERT_EQ(3, points.size());
  ASSERT_EQ(GeoPoint2D(Degree(10), Degree( 0)), points[0]);
  ASSERT_EQ(GeoPoint2D(Degree( 0), Degree( 0)), points[1]);
  ASSERT_EQ(GeoPoint2D(Degree( 0), Degree(10)), points[2]);

  ASSERT_EQ(5, result.size());
  ASSERT_EQ(GeoPoint2D(Degree(10               ), Degree( 0)), result[0]);
  ASSERT_EQ(GeoPoint2D(Degree( 5.00037949475414), Degree( 0)), result[1]);
  ASSERT_EQ(GeoPoint2D(Degree( 0               ), Degree( 0)), result[2]);
  ASSERT_EQ(GeoPoint2D(Degree( 0               ), Degree( 5)), result[3]);
  ASSERT_EQ(GeoPoint2D(Degree( 0               ), Degree(10)), result[4]);
}
//==============================================================================
/*!
* \brief Тестирование разбиения ортодромий заданных ломаной на равные части с
*        заменой исходной ломаной на результат
*/
TEST(OrthodromyToolsTest, DivideByLinestring)
{
  //! Создаем инструменты для эллипсоида WGS-84
  OrthodromyTools orthodromyTools;

  //! Ортодромии заданы ломаной из трёх точек: на пересечение гринвича и 
  //! экватора.
  GeoLinestring2D line;
  line.emplace_back(GeoPoint2D(Degree(10), Degree( 0)));
  line.emplace_back(GeoPoint2D(Degree( 0), Degree( 0)));
  line.emplace_back(GeoPoint2D(Degree( 0), Degree(10)));

  orthodromyTools.divide(line, 600000.);

  ASSERT_EQ(5, line.size());
  ASSERT_EQ(GeoPoint2D(Degree(10               ), Degree( 0)), line[0]);
  ASSERT_EQ(GeoPoint2D(Degree( 5.00037949475414), Degree( 0)), line[1]);
  ASSERT_EQ(GeoPoint2D(Degree( 0               ), Degree( 0)), line[2]);
  ASSERT_EQ(GeoPoint2D(Degree( 0               ), Degree( 5)), line[3]);
  ASSERT_EQ(GeoPoint2D(Degree( 0               ), Degree(10)), line[4]);
}
//==============================================================================
/*!
* \brief Тестирование разбиения ортодромий заданных ломаной на равные части без
*        замены исходной ломаной
*/
TEST(OrthodromyToolsTest, DivideByLinestringWithoutChange)
{
  //! Создаем инструменты для эллипсоида WGS-84
  OrthodromyTools orthodromyTools;

  //! Ортодромии заданы ломаной из трёх точек: на пересечение гринвича и 
  //! экватора.
  GeoLinestring2D line;
  line.emplace_back(GeoPoint2D(Degree(10), Degree( 0)));
  line.emplace_back(GeoPoint2D(Degree( 0), Degree( 0)));
  line.emplace_back(GeoPoint2D(Degree( 0), Degree(10)));

  GeoLinestring2D result;

  orthodromyTools.divide(line, result, 600000.);

  ASSERT_EQ(3, line.size());
  ASSERT_EQ(GeoPoint2D(Degree(10), Degree( 0)), line[0]);
  ASSERT_EQ(GeoPoint2D(Degree( 0), Degree( 0)), line[1]);
  ASSERT_EQ(GeoPoint2D(Degree( 0), Degree(10)), line[2]);

  ASSERT_EQ(5, result.size());
  ASSERT_EQ(GeoPoint2D(Degree(10               ), Degree( 0)), result[0]);
  ASSERT_EQ(GeoPoint2D(Degree( 5.00037949475414), Degree( 0)), result[1]);
  ASSERT_EQ(GeoPoint2D(Degree( 0               ), Degree( 0)), result[2]);
  ASSERT_EQ(GeoPoint2D(Degree( 0               ), Degree( 5)), result[3]);
  ASSERT_EQ(GeoPoint2D(Degree( 0               ), Degree(10)), result[4]);
}
//==============================================================================
/*!
* \brief Тестирование разбиения ортодромий заданных кольцом на равные части с
*        заменой исходного кольца на результат
*/
TEST(OrthodromyToolsTest, DivideByRing)
{
  //! Создаем инструменты для эллипсоида WGS-84
  OrthodromyTools orthodromyTools;

  //! Ортодромии заданы кольцом из 5 точек: на пересечение гринвича и 
  //! экватора.
  GeoRing2D ring;
  ring.emplace_back(GeoPoint2D(Degree(10), Degree( 0)));
  ring.emplace_back(GeoPoint2D(Degree(10), Degree(10)));
  ring.emplace_back(GeoPoint2D(Degree( 0), Degree(10)));
  ring.emplace_back(GeoPoint2D(Degree( 0), Degree( 0)));
  ring.emplace_back(GeoPoint2D(ring.at(0)));

  orthodromyTools.divide(ring, 600000.);

  ASSERT_EQ(9, ring.size());
  ASSERT_EQ(GeoPoint2D(Degree(10               ), Degree( 0)), ring[0]);
  ASSERT_EQ(GeoPoint2D(Degree(10.0376683703956 ), Degree( 5)), ring[1]);
  ASSERT_EQ(GeoPoint2D(Degree(10               ), Degree(10)), ring[2]);
  ASSERT_EQ(GeoPoint2D(Degree( 5.00037949475414), Degree(10)), ring[3]);
  ASSERT_EQ(GeoPoint2D(Degree( 0               ), Degree(10)), ring[4]);
  ASSERT_EQ(GeoPoint2D(Degree( 0               ), Degree( 5)), ring[5]);
  ASSERT_EQ(GeoPoint2D(Degree( 0               ), Degree( 0)), ring[6]);
  ASSERT_EQ(GeoPoint2D(Degree( 5.00037949475414), Degree( 0)), ring[7]);
  ASSERT_EQ(GeoPoint2D(Degree(10               ), Degree( 0)), ring[8]);
}
//==============================================================================
/*!
* \brief Тестирование разбиения ортодромий заданных кольцом на равные части без
*        замены исходного кольца
*/
TEST(OrthodromyToolsTest, DivideByRingWithoutChange)
{
  //! Создаем инструменты для эллипсоида WGS-84
  OrthodromyTools orthodromyTools;

  //! Ортодромии заданы кольцом из 5 точек: на пересечение гринвича и 
  //! экватора.
  GeoRing2D ring;
  ring.emplace_back(GeoPoint2D(Degree(10), Degree( 0)));
  ring.emplace_back(GeoPoint2D(Degree(10), Degree(10)));
  ring.emplace_back(GeoPoint2D(Degree(0), Degree(10)));
  ring.emplace_back(GeoPoint2D(Degree( 0), Degree( 0)));
  ring.emplace_back(GeoPoint2D(ring.at(0)));

  GeoRing2D result;

  orthodromyTools.divide(ring, result, 600000.);

  ASSERT_EQ(5, ring.size());
  ASSERT_EQ(GeoPoint2D(Degree(10), Degree( 0)), ring[0]);
  ASSERT_EQ(GeoPoint2D(Degree(10), Degree(10)), ring[1]);
  ASSERT_EQ(GeoPoint2D(Degree( 0), Degree(10)), ring[2]);
  ASSERT_EQ(GeoPoint2D(Degree( 0), Degree( 0)), ring[3]);
  ASSERT_EQ(GeoPoint2D(Degree(10), Degree( 0)), ring[4]);

  ASSERT_EQ(9, result.size());
  ASSERT_EQ(GeoPoint2D(Degree(10               ), Degree( 0)), result[0]);
  ASSERT_EQ(GeoPoint2D(Degree(10.0376683703956 ), Degree( 5)), result[1]);
  ASSERT_EQ(GeoPoint2D(Degree(10               ), Degree(10)), result[2]);
  ASSERT_EQ(GeoPoint2D(Degree( 5.00037949475414), Degree(10)), result[3]);
  ASSERT_EQ(GeoPoint2D(Degree( 0               ), Degree(10)), result[4]);
  ASSERT_EQ(GeoPoint2D(Degree( 0               ), Degree( 5)), result[5]);
  ASSERT_EQ(GeoPoint2D(Degree( 0               ), Degree( 0)), result[6]);
  ASSERT_EQ(GeoPoint2D(Degree( 5.00037949475414), Degree( 0)), result[7]);
  ASSERT_EQ(GeoPoint2D(Degree(10               ), Degree( 0)), result[8]);
}
//==============================================================================
/*!
* \brief Тестирование разбиения ортодромий заданных полигоном на равные части с
*        заменой исходного полигона на результат
*/
TEST(OrthodromyToolsTest, DivideByPolygon)
{
  //! Создаем инструменты для эллипсоида WGS-84
  OrthodromyTools orthodromyTools;

  //! Ортодромии заданы полигоном: внешнее и внутреннее кольцо из 5 точек: на 
  //! пересечение гринвича и экватора.
  GeoPolygon2D polygon;
  polygon.outer().emplace_back(GeoPoint2D(Degree(10), Degree( 0)));
  polygon.outer().emplace_back(GeoPoint2D(Degree(10), Degree(10)));
  polygon.outer().emplace_back(GeoPoint2D(Degree( 0), Degree(10)));
  polygon.outer().emplace_back(GeoPoint2D(Degree( 0), Degree( 0)));
  polygon.outer().emplace_back(GeoPoint2D(polygon.outer().at(0)));

  GeoRing2D ring;
  ring.emplace_back(GeoPoint2D(Degree(8), Degree(2)));
  ring.emplace_back(GeoPoint2D(Degree(2), Degree(2)));
  ring.emplace_back(GeoPoint2D(Degree(2), Degree(8)));
  ring.emplace_back(GeoPoint2D(Degree(8), Degree(8)));
  ring.emplace_back(GeoPoint2D(ring.at(0)));
  polygon.inners().emplace_back(ring);

  orthodromyTools.divide(polygon, 600000.);

  ASSERT_EQ(9, polygon.outer().size());
  ASSERT_EQ(9, polygon.inners().at(0).size());

  ASSERT_EQ(GeoPoint2D(Degree(10), Degree(0)), 
            polygon.outer()[0]);
  ASSERT_EQ(GeoPoint2D(Degree(10.0376683703956), Degree( 5)), 
            polygon.outer()[1]);
  ASSERT_EQ(GeoPoint2D(Degree(10), Degree(10)), 
            polygon.outer()[2]);
  ASSERT_EQ(GeoPoint2D(Degree(5.00037949475414), Degree(10)), 
            polygon.outer()[3]);
  ASSERT_EQ(GeoPoint2D(Degree(0), Degree(10)), 
            polygon.outer()[4]);
  ASSERT_EQ(GeoPoint2D(Degree(0), Degree(5)), 
            polygon.outer()[5]);
  ASSERT_EQ(GeoPoint2D(Degree(0), Degree(0)), 
            polygon.outer()[6]);
  ASSERT_EQ(GeoPoint2D(Degree(5.00037949475414), Degree(0)), 
            polygon.outer()[7]);
  ASSERT_EQ(GeoPoint2D(Degree(10), Degree( 0)), 
            polygon.outer()[8]);

  ASSERT_EQ(GeoPoint2D(Degree(8), Degree(2)), 
            polygon.inners().at(0)[0]);
  ASSERT_EQ(GeoPoint2D(Degree(5.00013683484431), Degree(2)), 
            polygon.inners().at(0)[1]);
  ASSERT_EQ(GeoPoint2D(Degree(2), Degree(2)), 
            polygon.inners().at(0)[2]);
  ASSERT_EQ(GeoPoint2D(Degree(2.00276093986168), Degree(5)),
            polygon.inners().at(0)[3]);
  ASSERT_EQ(GeoPoint2D(Degree(2), Degree(8)), 
            polygon.inners().at(0)[4]);
  ASSERT_EQ(GeoPoint2D(Degree(5.00013683484431), Degree(8)),
            polygon.inners().at(0)[5]);
  ASSERT_EQ(GeoPoint2D(Degree(8), Degree(8)), 
            polygon.inners().at(0)[6]);
  ASSERT_EQ(GeoPoint2D(Degree(8.01090803889823), Degree(5)),
            polygon.inners().at(0)[7]);
  ASSERT_EQ(GeoPoint2D(Degree(8), Degree(2)), 
            polygon.inners().at(0)[8]);
}
//==============================================================================
/*!
* \brief Тестирование разбиения ортодромий заданных полигоном на равные части 
*        без замены исходного полигона
*/
TEST(OrthodromyToolsTest, DivideByPolygonWithoutChange)
{
  //! Создаем инструменты для эллипсоида WGS-84
  OrthodromyTools orthodromyTools;

  //! Ортодромии заданы полигоном: внешнее и внутреннее кольцо из 5 точек: на 
  //! пересечение гринвича и экватора.
  GeoPolygon2D polygon;
  polygon.outer().emplace_back(GeoPoint2D(Degree(10), Degree( 0)));
  polygon.outer().emplace_back(GeoPoint2D(Degree(10), Degree(10)));
  polygon.outer().emplace_back(GeoPoint2D(Degree( 0), Degree(10)));
  polygon.outer().emplace_back(GeoPoint2D(Degree( 0), Degree( 0)));
  polygon.outer().emplace_back(GeoPoint2D(polygon.outer().at(0)));

  GeoRing2D ring;
  ring.emplace_back(GeoPoint2D(Degree(8), Degree(2)));
  ring.emplace_back(GeoPoint2D(Degree(2), Degree(2)));
  ring.emplace_back(GeoPoint2D(Degree(2), Degree(8)));
  ring.emplace_back(GeoPoint2D(Degree(8), Degree(8)));
  ring.emplace_back(GeoPoint2D(ring.at(0)));
  polygon.inners().emplace_back(ring);

  GeoPolygon2D result;

  orthodromyTools.divide(polygon, result, 600000.);

  ASSERT_EQ(5, polygon.outer().size());
  ASSERT_EQ(5, polygon.inners().at(0).size());

  ASSERT_EQ(9, result.outer().size());
  ASSERT_EQ(9, result.inners().at(0).size());

  ASSERT_EQ(GeoPoint2D(Degree(10), Degree(0)), 
            result.outer()[0]);
  ASSERT_EQ(GeoPoint2D(Degree(10.0376683703956), Degree( 5)), 
            result.outer()[1]);
  ASSERT_EQ(GeoPoint2D(Degree(10), Degree(10)), 
            result.outer()[2]);
  ASSERT_EQ(GeoPoint2D(Degree(5.00037949475414), Degree(10)), 
            result.outer()[3]);
  ASSERT_EQ(GeoPoint2D(Degree(0), Degree(10)), 
            result.outer()[4]);
  ASSERT_EQ(GeoPoint2D(Degree(0), Degree(5)), 
            result.outer()[5]);
  ASSERT_EQ(GeoPoint2D(Degree(0), Degree(0)), 
            result.outer()[6]);
  ASSERT_EQ(GeoPoint2D(Degree(5.00037949475414), Degree(0)), 
            result.outer()[7]);
  ASSERT_EQ(GeoPoint2D(Degree(10), Degree( 0)), 
            result.outer()[8]);

  ASSERT_EQ(GeoPoint2D(Degree(8), Degree(2)), 
            result.inners().at(0)[0]);
  ASSERT_EQ(GeoPoint2D(Degree(5.00013683484431), Degree(2)), 
            result.inners().at(0)[1]);
  ASSERT_EQ(GeoPoint2D(Degree(2), Degree(2)), 
            result.inners().at(0)[2]);
  ASSERT_EQ(GeoPoint2D(Degree(2.00276093986168), Degree(5)),
            result.inners().at(0)[3]);
  ASSERT_EQ(GeoPoint2D(Degree(2), Degree(8)), 
            result.inners().at(0)[4]);
  ASSERT_EQ(GeoPoint2D(Degree(5.00013683484431), Degree(8)),
            result.inners().at(0)[5]);
  ASSERT_EQ(GeoPoint2D(Degree(8), Degree(8)), 
            result.inners().at(0)[6]);
  ASSERT_EQ(GeoPoint2D(Degree(8.01090803889823), Degree(5)),
            result.inners().at(0)[7]);
  ASSERT_EQ(GeoPoint2D(Degree(8), Degree(2)), 
            result.inners().at(0)[8]);
}
//==============================================================================
/*!
* \brief Тестирование разбиения ортодромий заданных мультиполигоном на равные 
*        части с заменой исходного мультиполигона на результат
*/
TEST(OrthodromyToolsTest, DivideByMultiPolygon)
{
  //! Создаем инструменты для эллипсоида WGS-84
  OrthodromyTools orthodromyTools;

  //! Ортодромии заданы мультиполигоном: 
  GeoMultiPolygon2D multiPolygon;

  //! первый: внешнее и внутреннее кольцо из 5 точек на пересечение гринвича и 
  //!         экватора.
  GeoPolygon2D polygon;
  polygon.outer().emplace_back(GeoPoint2D(Degree(10), Degree( 0)));
  polygon.outer().emplace_back(GeoPoint2D(Degree(10), Degree(10)));
  polygon.outer().emplace_back(GeoPoint2D(Degree( 0), Degree(10)));
  polygon.outer().emplace_back(GeoPoint2D(Degree( 0), Degree( 0)));
  polygon.outer().emplace_back(GeoPoint2D(polygon.outer().at(0)));

  GeoRing2D ring;
  ring.emplace_back(GeoPoint2D(Degree(8), Degree(2)));
  ring.emplace_back(GeoPoint2D(Degree(2), Degree(2)));
  ring.emplace_back(GeoPoint2D(Degree(2), Degree(8)));
  ring.emplace_back(GeoPoint2D(Degree(8), Degree(8)));
  ring.emplace_back(GeoPoint2D(ring.at(0)));
  
  polygon.inners().emplace_back(ring);
  multiPolygon.emplace_back(polygon);

  //! второй: внешнее и внутреннее кольцо из 4 точек в районе полюса.
  polygon.clear();
  polygon.outer().emplace_back(GeoPoint2D(Degree(85), Degree( 0)));
  polygon.outer().emplace_back(GeoPoint2D(Degree(85), Degree(-180)));
  polygon.outer().emplace_back(GeoPoint2D(Degree(85), Degree( 10)));
  polygon.outer().emplace_back(GeoPoint2D(polygon.outer().at(0)));

  ring.clear();
  ring.emplace_back(GeoPoint2D(Degree(87), Degree(2)));
  ring.emplace_back(GeoPoint2D(Degree(90), Degree(5)));
  ring.emplace_back(GeoPoint2D(Degree(87), Degree(8)));
  ring.emplace_back(GeoPoint2D(ring.at(0)));

  polygon.inners().emplace_back(ring);
  multiPolygon.emplace_back(polygon);

  //! Разбиение
  orthodromyTools.divide(multiPolygon, 330000.);

  //! Проверка
  ASSERT_EQ(17, multiPolygon.at(0).outer().size());
  ASSERT_EQ(13, multiPolygon.at(0).inners().at(0).size());
  ASSERT_EQ(10, multiPolygon.at(1).outer().size());
  ASSERT_EQ(6 , multiPolygon.at(1).inners().at(0).size());
}
//==============================================================================
/*!
* \brief Тестирование разбиения ортодромий заданных мультиполигоном на равные 
*        части с заменой исходного мультиполигона на результат
*/
TEST(OrthodromyToolsTest, DivideByMultiPolygonWithoutChange)
{
  //! Создаем инструменты для эллипсоида WGS-84
  OrthodromyTools orthodromyTools;

  //! Ортодромии заданы мультиполигоном: 
  GeoMultiPolygon2D multiPolygon;

  //! первый: внешнее и внутреннее кольцо из 5 точек на пересечение гринвича и 
  //!         экватора.
  GeoPolygon2D polygon;
  polygon.outer().emplace_back(GeoPoint2D(Degree(10), Degree( 0)));
  polygon.outer().emplace_back(GeoPoint2D(Degree(10), Degree(10)));
  polygon.outer().emplace_back(GeoPoint2D(Degree( 0), Degree(10)));
  polygon.outer().emplace_back(GeoPoint2D(Degree( 0), Degree( 0)));
  polygon.outer().emplace_back(GeoPoint2D(polygon.outer().at(0)));

  GeoRing2D ring;
  ring.emplace_back(GeoPoint2D(Degree(8), Degree(2)));
  ring.emplace_back(GeoPoint2D(Degree(2), Degree(2)));
  ring.emplace_back(GeoPoint2D(Degree(2), Degree(8)));
  ring.emplace_back(GeoPoint2D(Degree(8), Degree(8)));
  ring.emplace_back(GeoPoint2D(ring.at(0)));
  
  polygon.inners().emplace_back(ring);
  multiPolygon.emplace_back(polygon);

  //! второй: внешнее и внутреннее кольцо из 4 точек в районе полюса.
  polygon.clear();
  polygon.outer().emplace_back(GeoPoint2D(Degree(85), Degree( 0)));
  polygon.outer().emplace_back(GeoPoint2D(Degree(85), Degree(-180)));
  polygon.outer().emplace_back(GeoPoint2D(Degree(85), Degree( 10)));
  polygon.outer().emplace_back(GeoPoint2D(polygon.outer().at(0)));

  ring.clear();
  ring.emplace_back(GeoPoint2D(Degree(87), Degree(2)));
  ring.emplace_back(GeoPoint2D(Degree(90), Degree(5)));
  ring.emplace_back(GeoPoint2D(Degree(87), Degree(8)));
  ring.emplace_back(GeoPoint2D(ring.at(0)));

  polygon.inners().emplace_back(ring);
  multiPolygon.emplace_back(polygon);

  //! Разбиение
  GeoMultiPolygon2D result;
  orthodromyTools.divide(multiPolygon, result, 330000.);

  //! Проверка
  ASSERT_EQ(17, result.at(0).outer().size());
  ASSERT_EQ(13, result.at(0).inners().at(0).size());
  ASSERT_EQ(10, result.at(1).outer().size());
  ASSERT_EQ(6 , result.at(1).inners().at(0).size());

  ASSERT_EQ(5, multiPolygon.at(0).outer().size());
  ASSERT_EQ(5, multiPolygon.at(0).inners().at(0).size());
  ASSERT_EQ(4, multiPolygon.at(1).outer().size());
  ASSERT_EQ(4 , multiPolygon.at(1).inners().at(0).size());
}
//==============================================================================
/*!
* \brief Тестирование построения окружности с заданным количеством вершин
*/
TEST(OrthodromyToolsTest, CircleByNumVerticies)
{
  //! Создаем инструменты для эллипсоида WGS-84
  OrthodromyTools orthodromyTools;

  //! Радиус меньше 0
  GeoPolygon2D circle;
  ASSERT_THROW(orthodromyTools.circle(GeoPoint2D(), -10.), GeodesicFailure);
  try
  {
    circle = orthodromyTools.circle(GeoPoint2D(), -10.);
  }
  catch (const GeodesicFailure& exception)
  {
    ASSERT_EQ(GeodesicEnumFailure::CircleDoNotExist, exception.getCode());
    ASSERT_EQ(geodesicLibTitle, exception.getLib());
  }

  //! Корректная окружность из 8 точек
  circle = orthodromyTools.circle(GeoPoint2D(), 1113194.907932736, 8);
  ASSERT_EQ(9, circle.outer().size()); //!< 9 - потому что замыкание

  ASSERT_EQ(GeoPoint2D(Degree(10.066361164237714), Degree(0)), 
            circle.outer()[0]);
  ASSERT_EQ(GeoPoint2D(Degree(7.100069383788794), Degree(7.1073187734247796)),
            circle.outer()[1]);
  ASSERT_EQ(GeoPoint2D(Degree(0), Degree(10)), 
            circle.outer()[2]);
  ASSERT_EQ(GeoPoint2D(Degree(-7.100069383788794), Degree(7.1073187734247796)),
            circle.outer()[3]);
  ASSERT_EQ(GeoPoint2D(Degree(-10.066361164237714), Degree(0)), 
            circle.outer()[4]);
  ASSERT_EQ(GeoPoint2D(Degree(-7.100069383788794), Degree(-7.1073187734247796)), 
            circle.outer()[5]);
  ASSERT_EQ(GeoPoint2D(Degree(0), Degree(-10)), 
            circle.outer()[6]);
  ASSERT_EQ(GeoPoint2D(Degree(7.1000693837887940), Degree(-7.1073187734247796)), 
            circle.outer()[7]);
  ASSERT_EQ(GeoPoint2D(Degree(10.066361164237714), Degree(0)), 
            circle.outer()[8]);

 //! Окружность с центром на полюсе
  circle = orthodromyTools.circle(GeoPoint2D(Degree(90), Degree(0)), 
                                  558455.588646477, 8);
  ASSERT_EQ(9, circle.outer().size()); //!< 9 - потому что замыкание

  ASSERT_EQ(GeoPoint2D(Degree(85), Degree(-180)), 
            circle.outer()[0]);
  ASSERT_EQ(GeoPoint2D(Degree(85), Degree(135)),
            circle.outer()[1]);
  ASSERT_EQ(GeoPoint2D(Degree(85), Degree(90)),
            circle.outer()[2]);
  ASSERT_EQ(GeoPoint2D(Degree(85), Degree(45)),
            circle.outer()[3]);
  ASSERT_EQ(GeoPoint2D(Degree(85), Degree(0)), 
            circle.outer()[4]);
  ASSERT_EQ(GeoPoint2D(Degree(85), Degree(-45)), 
            circle.outer()[5]);
  ASSERT_EQ(GeoPoint2D(Degree(85), Degree(-90)),
            circle.outer()[6]);
  ASSERT_EQ(GeoPoint2D(Degree(85), Degree(-135)),
            circle.outer()[7]);
  ASSERT_EQ(GeoPoint2D(Degree(85), Degree(-180)), 
            circle.outer()[8]);
}
//==============================================================================
/*!
* \brief Тестирование построения минимального прямоугольника для окружности
*/
TEST(OrthodromyToolsTest, CircleRectangle)
{
  //! Создаем инструменты для эллипсоида WGS-84
  OrthodromyTools orthodromyTools;

  GeoPolygon2D polygonBox;
  
  //! Радиус не превосходит 0
  ASSERT_THROW(orthodromyTools.circleRectangle(GeoPoint2D(), -10.),
               GeodesicFailure);
  try
  {
    polygonBox = orthodromyTools.circleRectangle(GeoPoint2D(), -10.);
  }
  catch (const GeodesicFailure& exception)
  {
    ASSERT_EQ(GeodesicEnumFailure::CircleDoNotExist, exception.getCode());
    ASSERT_EQ(geodesicLibTitle, exception.getLib());
  }

  //! Буфер меньше 0
  ASSERT_THROW(orthodromyTools.circleRectangle(GeoPoint2D(), 10., -1.),
               GeodesicFailure);
  try
  {
    polygonBox = orthodromyTools.circleRectangle(GeoPoint2D(), 10., -1.);
  }
  catch (const GeodesicFailure& exception)
  {
    ASSERT_EQ(GeodesicEnumFailure::NegativeDistanceLikeValue, 
              exception.getCode());
    ASSERT_EQ(geodesicLibTitle, exception.getLib());
  }

  //! Корректный полигон
  polygonBox = orthodromyTools.circleRectangle(GeoPoint2D(), 1113194.907932736);
  
  ASSERT_EQ(5, polygonBox.outer().size()); 
  ASSERT_EQ(GeoPoint2D(Degree(10.066361164237714),  Degree(-10)),
            polygonBox.outer()[0]);
  ASSERT_EQ(GeoPoint2D(Degree(10.066361164237714),  Degree(10)),
            polygonBox.outer()[1]);
  ASSERT_EQ(GeoPoint2D(Degree(-10.066361164237714), Degree(10)),
            polygonBox.outer()[2]);
  ASSERT_EQ(GeoPoint2D(Degree(-10.066361164237714), Degree(-10)),
            polygonBox.outer()[3]);
  ASSERT_EQ(GeoPoint2D(Degree(10.066361164237714),  Degree(-10)),
            polygonBox.outer()[4]);

  //! Корректный полигон с буфером
  polygonBox = orthodromyTools.circleRectangle(GeoPoint2D(), 1100194.907932736,
                                                               13000.);

  ASSERT_EQ(5, polygonBox.outer().size());
  ASSERT_EQ(GeoPoint2D(Degree(10.066361164237714), Degree(-10)),
    polygonBox.outer()[0]);
  ASSERT_EQ(GeoPoint2D(Degree(10.066361164237714), Degree(10)),
    polygonBox.outer()[1]);
  ASSERT_EQ(GeoPoint2D(Degree(-10.066361164237714), Degree(10)),
    polygonBox.outer()[2]);
  ASSERT_EQ(GeoPoint2D(Degree(-10.066361164237714), Degree(-10)),
    polygonBox.outer()[3]);
  ASSERT_EQ(GeoPoint2D(Degree(10.066361164237714), Degree(-10)),
    polygonBox.outer()[4]);
}
//==============================================================================
/*!
* \brief Тестирование построения окружности с заданным расстоянием между вершин
*/
TEST(OrthodromyToolsTest, CircleBySegmentLength)
{
  //! Создаем инструменты для эллипсоида WGS-84
  OrthodromyTools orthodromyTools;

  //! Радиус меньше 0
  GeoPolygon2D circle;
  ASSERT_THROW(orthodromyTools.circle(GeoPoint2D(), -10., 900000.),
               GeodesicFailure);
  try
  {
    circle = orthodromyTools.circle(GeoPoint2D(), -10., 900000.);
  }
  catch (const GeodesicFailure& exception)
  {
    ASSERT_EQ(GeodesicEnumFailure::CircleDoNotExist, exception.getCode());
    ASSERT_EQ(geodesicLibTitle, exception.getLib());
  }

  //! Расстояние между соседними вершинами меньше 0
  ASSERT_THROW(orthodromyTools.circle(GeoPoint2D(), 10000., -10.),
               GeodesicFailure);
  try
  {
    circle = orthodromyTools.circle(GeoPoint2D(), 10000., -10.);
  }
  catch (const GeodesicFailure& exception)
  {
    ASSERT_EQ(
      GeodesicEnumFailure::IncorrectSegmentLengthForEllipseApproximation,
      exception.getCode());
    ASSERT_EQ(geodesicLibTitle, exception.getLib());
  }

  //! Радиус меньше половины расстояния между соседними вершинами
  ASSERT_THROW(orthodromyTools.circle(GeoPoint2D(), 10., 40.),
               GeodesicFailure);
  try
  {
    circle = orthodromyTools.circle(GeoPoint2D(), 10., 40.);
  }
  catch (const GeodesicFailure& exception)
  {
    ASSERT_EQ(
      GeodesicEnumFailure::IncorrectSegmentLengthForEllipseApproximation,
      exception.getCode());
    ASSERT_EQ(geodesicLibTitle, exception.getLib());
  }

  //! Корректная окружность из 8 точек
  circle = orthodromyTools.circle(GeoPoint2D(), 1113194.907932736, 900000.);
  ASSERT_EQ(9, circle.outer().size()); //!< 9 - потому что замыкание

  ASSERT_EQ(GeoPoint2D(Degree(10.066361164237714), Degree(0)),
            circle.outer()[0]);
  ASSERT_EQ(GeoPoint2D(Degree(7.100069383788794), Degree(7.1073187734247796)),
            circle.outer()[1]);
  ASSERT_EQ(GeoPoint2D(Degree(0), Degree(10)),
            circle.outer()[2]);
  ASSERT_EQ(GeoPoint2D(Degree(-7.100069383788794), Degree(7.1073187734247796)),
            circle.outer()[3]);
  ASSERT_EQ(GeoPoint2D(Degree(-10.066361164237714), Degree(0)),
            circle.outer()[4]);
  ASSERT_EQ(GeoPoint2D(Degree(-7.100069383788794), Degree(-7.1073187734247796)),
            circle.outer()[5]);
  ASSERT_EQ(GeoPoint2D(Degree(0), Degree(-10)),
            circle.outer()[6]);
  ASSERT_EQ(GeoPoint2D(Degree(7.1000693837887940), Degree(-7.1073187734247796)),
            circle.outer()[7]);
  ASSERT_EQ(GeoPoint2D(Degree(10.066361164237714), Degree(0)),
            circle.outer()[8]);
}
//==============================================================================
/*!
* \brief Тестирование построения эллипса
*/
TEST(OrthodromyToolsTest, Ellipse)
{
  //! Создаем инструменты для эллипсоида WGS-84
  OrthodromyTools orthodromyTools;

  //! Точность итерационного процесса неположительна.
  GeoPolygon2D ellipse;
  ASSERT_THROW(orthodromyTools.ellipse(GeoPoint2D(Degree(0), Degree(2)),
                                       GeoPoint2D(Degree(0), Degree(8)),
                                       1113194.907932736,
                                       900000.,
                                       0.),
               GeodesicFailure);
  try
  {
    ellipse = orthodromyTools.ellipse(
      GeoPoint2D(Degree(0), Degree(2)),
      GeoPoint2D(Degree(0), Degree(8)),
      1113194.907932736,
      900000.,
      0.);
  }
  catch (const GeodesicFailure& exception)
  {
    ASSERT_EQ(GeodesicEnumFailure::NonPositiveAccuracy, exception.getCode());
    ASSERT_EQ(geodesicLibTitle, exception.getLib());
  }

  //! Длина большей полуоси меньше 0
  ASSERT_THROW(orthodromyTools.ellipse(GeoPoint2D(Degree(0), Degree(2)),
                                       GeoPoint2D(Degree(0), Degree(8)),
                                       -10.,
                                       900000.,
                                       0.),
               GeodesicFailure);
  try
  {
    ellipse = orthodromyTools.ellipse(
      GeoPoint2D(Degree(0), Degree(2)),
      GeoPoint2D(Degree(0), Degree(8)),
      -10.,
      900000.,
      0.);
  }
  catch (const GeodesicFailure& exception)
  {
    ASSERT_EQ(GeodesicEnumFailure::EllipseDoNotExist, exception.getCode());
    ASSERT_EQ(geodesicLibTitle, exception.getLib());
  }

  //! Расстояние между соседними вершинами меньше 0
  ASSERT_THROW(orthodromyTools.ellipse(GeoPoint2D(),
                                       GeoPoint2D(Degree(0), Degree(10)),
                                       10000.,
                                       -10.),
               GeodesicFailure);
  try
  {
    ellipse = orthodromyTools.ellipse(GeoPoint2D(),
      GeoPoint2D(Degree(0), Degree(10)), 10000.,
      -10.);
  }
  catch (const GeodesicFailure& exception)
  {
    ASSERT_EQ(
      GeodesicEnumFailure::IncorrectSegmentLengthForEllipseApproximation,
      exception.getCode());
    ASSERT_EQ(geodesicLibTitle, exception.getLib());
  }

  //! Фокусы совпадают - окружность из 8 точек
  ellipse = orthodromyTools.ellipse(GeoPoint2D(), GeoPoint2D(), 
                                    1113194.907932736, 900000.);
  ASSERT_EQ(9, ellipse.outer().size()); //!< 9 - потому что замыкание

  ASSERT_EQ(GeoPoint2D(Degree(10.066361164237714), Degree(0)),
            ellipse.outer()[0]);
  ASSERT_EQ(GeoPoint2D(Degree(7.100069383788794), Degree(7.1073187734247796)),
            ellipse.outer()[1]);
  ASSERT_EQ(GeoPoint2D(Degree(0), Degree(10)),
            ellipse.outer()[2]);
  ASSERT_EQ(GeoPoint2D(Degree(-7.100069383788794), Degree(7.1073187734247796)),
            ellipse.outer()[3]);
  ASSERT_EQ(GeoPoint2D(Degree(-10.066361164237714), Degree(0)),
            ellipse.outer()[4]);
  ASSERT_EQ(GeoPoint2D(Degree(-7.100069383788794), Degree(-7.1073187734247796)),
            ellipse.outer()[5]);
  ASSERT_EQ(GeoPoint2D(Degree(0), Degree(-10)),
            ellipse.outer()[6]);
  ASSERT_EQ(GeoPoint2D(Degree(7.1000693837887940), Degree(-7.1073187734247796)),
            ellipse.outer()[7]);
  ASSERT_EQ(GeoPoint2D(Degree(10.066361164237714), Degree(0)),
            ellipse.outer()[8]);

  //! Корректный эллипс
  ellipse = orthodromyTools.ellipse(GeoPoint2D(Degree(0), Degree(2)), 
                                    GeoPoint2D(Degree(0), Degree(8)),
                                    1113194.907932736, 900000.);

  ASSERT_EQ(9, ellipse.outer().size()); //!< 9 - потому что замыкание
}
//==============================================================================
/*!
 * \brief Тестирование построения минимального прямоугольника,
 * содержащего эллипс на поверхности эллипсоида
 */
TEST(OrthodromyToolsTest, EllipseRectangle)
{
  //! Создаем инструменты для эллипсоида WGS-84
  OrthodromyTools orthodromyTools;

  //! Эллипс с главной осью на экваторе.
  GeoPolygon2D ellipseRectangle(
    orthodromyTools.ellipseRectangle(
      GeoPoint2D(Degree(0.), Degree(-1.)),
      GeoPoint2D(Degree(0.), Degree(1.)),
      222638.98158654712));
  ASSERT_EQ(5, ellipseRectangle.outer().size());
  ASSERT_EQ(0, ellipseRectangle.inners().size());
  ASSERT_EQ(
    GeoPoint2D(Degree(1.7427398745557230), Degree(-2.)),
    ellipseRectangle.outer()[0]);
  ASSERT_EQ(
    GeoPoint2D(Degree(1.7427398745557230), Degree(2.)),
    ellipseRectangle.outer()[1]);
  ASSERT_EQ(
    GeoPoint2D(Degree(-1.7427398745557230), Degree(2.)),
    ellipseRectangle.outer()[2]);
  ASSERT_EQ(
    GeoPoint2D(Degree(1.7427398745557230), Degree(-2.)),
    ellipseRectangle.outer()[0]);
  ASSERT_EQ(ellipseRectangle.outer()[0], ellipseRectangle.outer()[4]);

  //! Эллипс с совпадающими фокусами - окружность
  ellipseRectangle = orthodromyTools.ellipseRectangle(GeoPoint2D(), 
                                                      GeoPoint2D(), 
                                                      1113194.907932736);

  ASSERT_EQ(5, ellipseRectangle.outer().size());
  ASSERT_EQ(GeoPoint2D(Degree(10.066361164237714), Degree(-10)),
            ellipseRectangle.outer()[0]);
  ASSERT_EQ(GeoPoint2D(Degree(10.066361164237714), Degree(10)),
            ellipseRectangle.outer()[1]);
  ASSERT_EQ(GeoPoint2D(Degree(-10.066361164237714), Degree(10)),
            ellipseRectangle.outer()[2]);
  ASSERT_EQ(GeoPoint2D(Degree(-10.066361164237714), Degree(-10)),
            ellipseRectangle.outer()[3]);
  ASSERT_EQ(GeoPoint2D(Degree(10.066361164237714), Degree(-10)),
            ellipseRectangle.outer()[4]);

  //! Вызов метода с неположительной точностью.
  ASSERT_THROW(orthodromyTools.ellipseRectangle(
                 GeoPoint2D(Degree(0.), Degree(-1.)),
                 GeoPoint2D(Degree(0.), Degree(1.)),
                 222638.98158654712,
                 .0,
                 .0),
               GeodesicFailure);
  try
  {
    orthodromyTools.ellipseRectangle(
      GeoPoint2D(Degree(0.), Degree(-1.)),
      GeoPoint2D(Degree(0.), Degree(1.)),
      222638.98158654712,
      .0,
      .0);
  }
  catch (const GeodesicFailure& exception)
  {
    ASSERT_EQ(geodesicLibTitle, exception.getLib());
    ASSERT_EQ(GeodesicEnumFailure::NonPositiveAccuracy, exception.getCode());
  }

  //! Вызов метода с отрицательной величиной отступа запаса.
  ASSERT_THROW(orthodromyTools.ellipseRectangle(
                 GeoPoint2D(Degree(0.), Degree(-1.)),
                 GeoPoint2D(Degree(0.), Degree(1.)),
                 222638.98158654712,
                 -1.),
               GeodesicFailure);
  try
  {
    orthodromyTools.ellipseRectangle(
      GeoPoint2D(Degree(0.), Degree(-1.)),
      GeoPoint2D(Degree(0.), Degree(1.)),
      222638.98158654712,
      -1.);
  }
  catch (const GeodesicFailure& exception)
  {
    ASSERT_EQ(geodesicLibTitle, exception.getLib());
    ASSERT_EQ(
      GeodesicEnumFailure::NegativeDistanceLikeValue,
      exception.getCode());
  }

  //! Вызов метода с параметрами несуществующего эллипса.
  ASSERT_THROW(orthodromyTools.ellipseRectangle(
                 GeoPoint2D(Degree(0.), Degree(-1.)),
                 GeoPoint2D(Degree(0.), Degree(1.)),
                 10.),
               GeodesicFailure);
  try
  {
    orthodromyTools.ellipseRectangle(
      GeoPoint2D(Degree(0.), Degree(-1.)),
      GeoPoint2D(Degree(0.), Degree(1.)),
      10.);
  }
  catch (const GeodesicFailure& exception)
  {
    ASSERT_EQ(geodesicLibTitle, exception.getLib());
    ASSERT_EQ(
      GeodesicEnumFailure::EllipseDoNotExist,
      exception.getCode());
  }
}
//==============================================================================
/*!
 * \brief Тестирование "прорежения" множества точек на эллипсоиде.
 */
TEST(OrthodromyToolsTest, CollapsePoints)
{
  //! Создаем инструменты для эллипсоида WGS-84
  OrthodromyTools orthodromyTools;

  GeoPoint2D initPoint(Degree(0.), Degree(0.));
  GeoPoint2D repeatingPoint(initPoint);
  GeoPoint2D nearPoint(Degree(0.), Degree(1.));
  GeoPoint2D farPoint(Degree(0.), Degree(40.));
  
  // Должны отсеяться точка-копия и точка, находящаяся слишком близко к первой.
  GeoMultiPoint2D result(orthodromyTools.collapsePoints(
    { initPoint, repeatingPoint, nearPoint, farPoint },
    222638.981581586));

  ASSERT_EQ(2, result.size());
  ASSERT_EQ(initPoint, result[0]);
  ASSERT_EQ(farPoint, result[1]);
}
//==============================================================================
} //! Tests
} //! Geodesic