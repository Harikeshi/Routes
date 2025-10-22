#include <Geometry/ComplicatedOperations/CakeProblem.hpp>

#include <Geometry/Operations/AtPoints2D.hpp>
#include <Geometry/GeometryException.hpp>
//! AbstractOperations
#include <AbstractOperations/Algorithms/Area.hpp>
#include <AbstractOperations/Algorithms/Correct.hpp>
#include <AbstractOperations/Algorithms/Difference.hpp>
//! gtest
#include <gtest/gtest.h>
//==============================================================================
//! Тесты для проверки правильности работы функций тортика
//==============================================================================
namespace Geometry
{
namespace Tests
{
//==============================================================================
using namespace AbstractOperations;
//==============================================================================
class SectorsCalculatorTest : public ::testing::Test
{
//==============================================================================
protected: 
//==============================================================================
  void SetUp()
  {
    // Контур
    poly.outer().emplace_back(Point2D(0, 0));
    poly.outer().emplace_back(Point2D(-10, 500));
    poly.outer().emplace_back(Point2D(0, 1000));
    poly.outer().emplace_back(Point2D(500, 1010));
    poly.outer().emplace_back(Point2D(1000, 1000));
    poly.outer().emplace_back(Point2D(1010, 500));
    poly.outer().emplace_back(Point2D(1000, 0));
    poly.outer().emplace_back(Point2D(500, -10));
    poly.outer().emplace_back(Point2D(0, 0));

    // Зона буквой П
    zone1.outer().emplace_back(Point2D(450, 300));
    zone1.outer().emplace_back(Point2D(300, 300));
    zone1.outer().emplace_back(Point2D(300, 700));
    zone1.outer().emplace_back(Point2D(450, 700));
    zone1.outer().emplace_back(Point2D(450, 650));
    zone1.outer().emplace_back(Point2D(400, 650));
    zone1.outer().emplace_back(Point2D(400, 350));
    zone1.outer().emplace_back(Point2D(450, 350));
    zone1.outer().emplace_back(Point2D(450, 300));

    // Зона буквой П, задающая сектор с развёрнутым углом
    zone2.outer().emplace_back(Point2D(500, 300));
    zone2.outer().emplace_back(Point2D(300, 300));
    zone2.outer().emplace_back(Point2D(300, 700));
    zone2.outer().emplace_back(Point2D(500, 700));
    zone2.outer().emplace_back(Point2D(500, 650));
    zone2.outer().emplace_back(Point2D(400, 650));
    zone2.outer().emplace_back(Point2D(400, 350));
    zone2.outer().emplace_back(Point2D(500, 350));
    zone2.outer().emplace_back(Point2D(500, 300));

    // Зона буквой П, задающая сектор с углом, большим 180
    zone3.outer().emplace_back(Point2D(550, 300));
    zone3.outer().emplace_back(Point2D(300, 300));
    zone3.outer().emplace_back(Point2D(300, 700));
    zone3.outer().emplace_back(Point2D(550, 700));
    zone3.outer().emplace_back(Point2D(550, 650));
    zone3.outer().emplace_back(Point2D(400, 650));
    zone3.outer().emplace_back(Point2D(400, 350));
    zone3.outer().emplace_back(Point2D(550, 350));
    zone3.outer().emplace_back(Point2D(550, 300));

    // Зона с "ямкой"
    zone4.outer().emplace_back(Point2D(600, 500));
    zone4.outer().emplace_back(Point2D(600, 600));
    zone4.outer().emplace_back(Point2D(650, 600));
    zone4.outer().emplace_back(Point2D(650, 550));
    zone4.outer().emplace_back(Point2D(700, 550));
    zone4.outer().emplace_back(Point2D(700, 650));
    zone4.outer().emplace_back(Point2D(650, 650));
    zone4.outer().emplace_back(Point2D(650, 700));
    zone4.outer().emplace_back(Point2D(800, 700));
    zone4.outer().emplace_back(Point2D(800, 500));
    zone4.outer().emplace_back(Point2D(600, 500));

    // Лесенка почти вплотную к предыдущей зоне
    zone5.outer().emplace_back(Point2D(600, 400));
    zone5.outer().emplace_back(Point2D(600, 495));
    zone5.outer().emplace_back(Point2D(800, 300));
    zone5.outer().emplace_back(Point2D(700, 300));
    zone5.outer().emplace_back(Point2D(700, 350));
    zone5.outer().emplace_back(Point2D(650, 350));
    zone5.outer().emplace_back(Point2D(650, 400));
    zone5.outer().emplace_back(Point2D(600, 400));

    // Лесенка близко к центру
    zone6.outer().emplace_back(Point2D(550, 350));
    zone6.outer().emplace_back(Point2D(550, 445));
    zone6.outer().emplace_back(Point2D(750, 250));
    zone6.outer().emplace_back(Point2D(650, 250));
    zone6.outer().emplace_back(Point2D(650, 300));
    zone6.outer().emplace_back(Point2D(600, 300));
    zone6.outer().emplace_back(Point2D(600, 350));
    zone6.outer().emplace_back(Point2D(550, 350));

    // Буква  Т, не попадающая полностью в окружность
    zone7.outer().emplace_back(Point2D(850, 550));
    zone7.outer().emplace_back(Point2D(950, 550));
    zone7.outer().emplace_back(Point2D(950, 350));
    zone7.outer().emplace_back(Point2D(850, 350));
    zone7.outer().emplace_back(Point2D(850, 400));
    zone7.outer().emplace_back(Point2D(800, 400));
    zone7.outer().emplace_back(Point2D(800, 450));
    zone7.outer().emplace_back(Point2D(850, 450));
    zone7.outer().emplace_back(Point2D(850, 550));

    // Ребристая зона, задающая сектор с углом, большим 180
    zone8.outer().emplace_back(Point2D(150, 450));
    zone8.outer().emplace_back(Point2D(50, 550));
    zone8.outer().emplace_back(Point2D(150, 550));
    zone8.outer().emplace_back(Point2D(50, 650));
    zone8.outer().emplace_back(Point2D(150, 650));
    zone8.outer().emplace_back(Point2D(150, 750));
    zone8.outer().emplace_back(Point2D(250, 750));
    zone8.outer().emplace_back(Point2D(250, 850));

    zone8.outer().emplace_back(Point2D(300, 750));
    zone8.outer().emplace_back(Point2D(350, 850));
    zone8.outer().emplace_back(Point2D(400, 800));
    zone8.outer().emplace_back(Point2D(450, 900));
    zone8.outer().emplace_back(Point2D(450, 800));
    zone8.outer().emplace_back(Point2D(500, 900));
    zone8.outer().emplace_back(Point2D(550, 800));

    zone8.outer().emplace_back(Point2D(600, 850));
    zone8.outer().emplace_back(Point2D(600, 800));
    zone8.outer().emplace_back(Point2D(700, 800));
    zone8.outer().emplace_back(Point2D(650, 750));
    zone8.outer().emplace_back(Point2D(750, 750));
    zone8.outer().emplace_back(Point2D(700, 700));
    zone8.outer().emplace_back(Point2D(800, 700));

    zone8.outer().emplace_back(Point2D(700, 600));
    zone8.outer().emplace_back(Point2D(800, 550));
    zone8.outer().emplace_back(Point2D(700, 500));
    zone8.outer().emplace_back(Point2D(750, 450));
    zone8.outer().emplace_back(Point2D(650, 400));
    zone8.outer().emplace_back(Point2D(550, 450));
    zone8.outer().emplace_back(Point2D(650, 500));

    zone8.outer().emplace_back(Point2D(600, 550));
    zone8.outer().emplace_back(Point2D(650, 600));
    zone8.outer().emplace_back(Point2D(600, 600));
    zone8.outer().emplace_back(Point2D(650, 700));
    zone8.outer().emplace_back(Point2D(600, 650));
    zone8.outer().emplace_back(Point2D(600, 750));
    zone8.outer().emplace_back(Point2D(500, 650));

    zone8.outer().emplace_back(Point2D(500, 750));
    zone8.outer().emplace_back(Point2D(450, 700));
    zone8.outer().emplace_back(Point2D(400, 750));
    zone8.outer().emplace_back(Point2D(350, 600));
    zone8.outer().emplace_back(Point2D(300, 700));
    zone8.outer().emplace_back(Point2D(300, 600));
    zone8.outer().emplace_back(Point2D(200, 650));

    zone8.outer().emplace_back(Point2D(250, 550));
    zone8.outer().emplace_back(Point2D(200, 550));
    zone8.outer().emplace_back(Point2D(250, 500));
    zone8.outer().emplace_back(Point2D(150, 450));

    // Зона, полностью перекрывающая область (> 360 градусов)
    zone9.outer().emplace_back(Point2D(400, 500));
    zone9.outer().emplace_back(Point2D(350, 550));
    zone9.outer().emplace_back(Point2D(350, 650));
    zone9.outer().emplace_back(Point2D(700, 650));
    zone9.outer().emplace_back(Point2D(700, 300));
    zone9.outer().emplace_back(Point2D(200, 300));
    zone9.outer().emplace_back(Point2D(200, 550));
    zone9.outer().emplace_back(Point2D(250, 600));
    zone9.outer().emplace_back(Point2D(300, 550));
    zone9.outer().emplace_back(Point2D(300, 400));
    zone9.outer().emplace_back(Point2D(600, 400));
    zone9.outer().emplace_back(Point2D(600, 550));
    zone9.outer().emplace_back(Point2D(450, 550));
    zone9.outer().emplace_back(Point2D(400, 500));

    // Зона, полностью перекрывающая область (= 360 градусов)
    zone10.outer().emplace_back(Point2D(300, 500));
    zone10.outer().emplace_back(Point2D(400, 500));
    zone10.outer().emplace_back(Point2D(400, 400));
    zone10.outer().emplace_back(Point2D(600, 400));
    zone10.outer().emplace_back(Point2D(600, 600));
    zone10.outer().emplace_back(Point2D(200, 600));
    zone10.outer().emplace_back(Point2D(200, 500));
    zone10.outer().emplace_back(Point2D(100, 500));
    zone10.outer().emplace_back(Point2D(100, 700));
    zone10.outer().emplace_back(Point2D(700, 700));
    zone10.outer().emplace_back(Point2D(700, 300));
    zone10.outer().emplace_back(Point2D(300, 300));
    zone10.outer().emplace_back(Point2D(300, 500));

    // Граничные точки
    leftBorderedPoint1 = Point2D(450., 300.);
    leftBorderedPoint2 = Point2D(500., 350.);
    leftBorderedPoint3 = Point2D(550., 350.);
    leftBorderedPoint4 = Point2D(650., 700.);
    leftBorderedPoint5 = Point2D(600., 495.);
    leftBorderedPoint6 = Point2D(750., 250.);
    leftBorderedPoint7 = Point2D(850., 550.);
    leftBorderedPoint8 = Point2D(150., 450.);

    rightBorderedPoint1 = Point2D(450., 700.);
    rightBorderedPoint2 = Point2D(500., 650.);
    rightBorderedPoint3 = Point2D(550., 650.);
    rightBorderedPoint4 = Point2D(600., 500.);
    rightBorderedPoint5 = Point2D(600., 400.);
    rightBorderedPoint6 = Point2D(550., 350.);
    rightBorderedPoint7 = Point2D(850., 350.);
    rightBorderedPoint8 = Point2D(550., 450.);

    // Задаём полигоны секторов
    sector1.poly.outer().emplace_back(Point2D(402.98575, 111.943));
    sector1.poly.outer().emplace_back(Point2D(100, 187.689437));
    sector1.poly.outer().emplace_back(Point2D(100, 812.310563));
    sector1.poly.outer().emplace_back(Point2D(402.98575, 888.057));
    sector1.poly.outer().emplace_back(Point2D(450, 700));
    sector1.poly.outer().emplace_back(Point2D(450, 650));
    sector1.poly.outer().emplace_back(Point2D(400, 650));
    sector1.poly.outer().emplace_back(Point2D(400, 350));
    sector1.poly.outer().emplace_back(Point2D(450, 350));
    sector1.poly.outer().emplace_back(Point2D(450, 300));
    sector1.poly.outer().emplace_back(Point2D(402.98575, 111.943));

    sector1.externalLeftPoint = Point2D(402.98575, 111.943);
    sector1.externalRightPoint = Point2D(402.98575, 888.057);

    sector2.poly.outer().emplace_back(Point2D(500, 100));
    sector2.poly.outer().emplace_back(Point2D(100, 100));
    sector2.poly.outer().emplace_back(Point2D(100, 900));
    sector2.poly.outer().emplace_back(Point2D(500, 900));
    sector2.poly.outer().emplace_back(Point2D(500, 650));
    sector2.poly.outer().emplace_back(Point2D(400, 650));
    sector2.poly.outer().emplace_back(Point2D(400, 350));
    sector2.poly.outer().emplace_back(Point2D(500, 350));
    sector2.poly.outer().emplace_back(Point2D(500, 100));

    sector2.externalLeftPoint = Point2D(500, 100);
    sector2.externalRightPoint = Point2D(500, 900);

    sector3.poly.outer().emplace_back(Point2D(626.491106, 120.526681));
    sector3.poly.outer().emplace_back(Point2D(100, -54.970355));
    sector3.poly.outer().emplace_back(Point2D(100, 1054.970355));
    sector3.poly.outer().emplace_back(Point2D(626.491099, 879.473319));
    sector3.poly.outer().emplace_back(Point2D(550, 650));
    sector3.poly.outer().emplace_back(Point2D(400, 650));
    sector3.poly.outer().emplace_back(Point2D(400, 350));
    sector3.poly.outer().emplace_back(Point2D(550, 350));
    sector3.poly.outer().emplace_back(Point2D(626.491106, 120.526681));

    sector3.externalLeftPoint = Point2D(626.491106, 120.526681);
    sector3.externalRightPoint = Point2D(626.491099, 879.473319);

    sector4.poly.outer().emplace_back(Point2D(768.328157, 857.770876));
    sector4.poly.outer().emplace_back(Point2D(947.213595, 500));
    sector4.poly.outer().emplace_back(Point2D(600, 500));
    sector4.poly.outer().emplace_back(Point2D(600, 600));
    sector4.poly.outer().emplace_back(Point2D(650, 600));
    sector4.poly.outer().emplace_back(Point2D(650, 550));
    sector4.poly.outer().emplace_back(Point2D(700, 550));
    sector4.poly.outer().emplace_back(Point2D(700, 650));
    sector4.poly.outer().emplace_back(Point2D(650, 650));
    sector4.poly.outer().emplace_back(Point2D(650, 700));
    sector4.poly.outer().emplace_back(Point2D(768.328157, 857.770876));

    sector4.externalLeftPoint = Point2D(768.328157, 857.770876);
    sector4.externalRightPoint = Point2D(947.213595, 500);

    sector5.poly.outer().emplace_back(Point2D(928.121066, 478.593947));
    sector5.poly.outer().emplace_back(Point2D(803.105482, 196.894518));
    sector5.poly.outer().emplace_back(Point2D(600, 400));
    sector5.poly.outer().emplace_back(Point2D(600, 495));
    sector5.poly.outer().emplace_back(Point2D(928.121066, 478.593947));

    sector5.externalLeftPoint = Point2D(928.121066, 478.593947);
    sector5.externalRightPoint = Point2D(803.105482, 196.894518);

    sector6.poly.outer().emplace_back(Point2D(790.617011, 209.382989));
    sector6.poly.outer().emplace_back(Point2D(629.967878, 110.096365));
    sector6.poly.outer().emplace_back(Point2D(550, 350));
    sector6.poly.outer().emplace_back(Point2D(550, 445));
    sector6.poly.outer().emplace_back(Point2D(750, 250));
    sector6.poly.outer().emplace_back(Point2D(790.617011, 209.382989));

    sector6.externalLeftPoint = Point2D(790.617011, 209.382989);
    sector6.externalRightPoint = Point2D(629.967878, 110.096365);

    sector7.poly.outer().emplace_back(Point2D(911.253785, 558.750541));
    sector7.poly.outer().emplace_back(Point2D(881.839553, 336.354477));
    sector7.poly.outer().emplace_back(Point2D(850, 350));
    sector7.poly.outer().emplace_back(Point2D(850, 400));
    sector7.poly.outer().emplace_back(Point2D(800, 400));
    sector7.poly.outer().emplace_back(Point2D(800, 450));
    sector7.poly.outer().emplace_back(Point2D(850, 450));
    sector7.poly.outer().emplace_back(Point2D(850, 550));
    sector7.poly.outer().emplace_back(Point2D(911.253785, 558.750541));

    sector7.externalLeftPoint = Point2D(911.253785, 558.750541);
    sector7.externalRightPoint = Point2D(881.839553, 336.354477);

    sector8.poly.outer().emplace_back(Point2D(104.020203, 443.431458));
    sector8.poly.outer().emplace_back(Point2D(12.490378, 1084.140229));
    sector8.poly.outer().emplace_back(Point2D(1240.491835, 674.80641));
    sector8.poly.outer().emplace_back(Point2D(782.842712, 217.157288));
    sector8.poly.outer().emplace_back(Point2D(550, 450));
    sector8.poly.outer().emplace_back(Point2D(650, 500));
    sector8.poly.outer().emplace_back(Point2D(600, 550));
    sector8.poly.outer().emplace_back(Point2D(650, 600));
    sector8.poly.outer().emplace_back(Point2D(600, 600));
    sector8.poly.outer().emplace_back(Point2D(650, 700));
    sector8.poly.outer().emplace_back(Point2D(600, 650));
    sector8.poly.outer().emplace_back(Point2D(600, 750));
    sector8.poly.outer().emplace_back(Point2D(500, 650));
    sector8.poly.outer().emplace_back(Point2D(500, 750));
    sector8.poly.outer().emplace_back(Point2D(450, 700));
    sector8.poly.outer().emplace_back(Point2D(400, 750));
    sector8.poly.outer().emplace_back(Point2D(350, 600));
    sector8.poly.outer().emplace_back(Point2D(300, 700));
    sector8.poly.outer().emplace_back(Point2D(300, 600));
    sector8.poly.outer().emplace_back(Point2D(200, 650));
    sector8.poly.outer().emplace_back(Point2D(250, 550));
    sector8.poly.outer().emplace_back(Point2D(200, 550));
    sector8.poly.outer().emplace_back(Point2D(250, 500));
    sector8.poly.outer().emplace_back(Point2D(150, 450));
    sector8.poly.outer().emplace_back(Point2D(104.020203, 443.431458));

    sector8.externalLeftPoint = Point2D(104.020203, 443.431458);
    sector8.externalRightPoint = Point2D(782.842712, 217.157288);

    // Задаём окружность
    center = Point2D(500, 500);
    radius = 400;

    // Ширина расширения
    widthExpansion = 25;
  }
//==============================================================================
  void TearDown()
  {

  }
//==============================================================================
  // Области
  Polygon2D poly;  // Контур
  Polygon2D zone1; // Зона буквой П
  Polygon2D zone2; // Зона буквой П, задающая сектор с развёрнутым углом
  Polygon2D zone3; // Зона буквой П, задающая сектор с углом, большим 180
  Polygon2D zone4; // Зона с "ямкой"
  Polygon2D zone5; // Лесенка почти вплотную к предыдущей зоне
  Polygon2D zone6; // Лесенка близко к центру
  Polygon2D zone7; // Буква  Т, не попадающая полностью в окружность
  Polygon2D zone8; // Ребристая зона, задающая сектор с углом, большим 180
  Polygon2D zone9; // Зона, полностью перекрывающая область (> 360 градусов)
  Polygon2D zone10; // Зона, полностью перекрывающая область (= 360 градусов)

  // Индексы левых точек полигонов
  std::vector<size_t> iLeftPoints1{ 0 };
  std::vector<size_t> iLeftPoints2{ 0, 7 };
  std::vector<size_t> iLeftPoints3{ 7 };
  std::vector<size_t> iLeftPoints4{ 1, 7 };
  std::vector<size_t> iLeftPoints5{ 1, 4, 6 };
  std::vector<size_t> iLeftPoints6{ 2, 4, 6 };
  std::vector<size_t> iLeftPoints7{ 0, 4 };
  std::vector<size_t> iLeftPoints8{ 0, 8, 12, 18, 20, 35, 39, 41 };

  // Индексы правых точек полигонов
  std::vector<size_t> iRightPoints1{ 3 };
  std::vector<size_t> iRightPoints2{ 3, 4 };
  std::vector<size_t> iRightPoints3{ 4 };
  std::vector<size_t> iRightPoints4{ 0, 4, 9 };
  std::vector<size_t> iRightPoints5{ 0, 3, 5 };
  std::vector<size_t> iRightPoints6{ 0, 3, 5 };
  std::vector<size_t> iRightPoints7{ 3, 5 };
  std::vector<size_t> iRightPoints8{ 7, 11, 17, 19, 27, 36, 40, 42 };

  // Индексы левых граничных точек
  size_t iLeftBorderedPoint1 = 0;
  size_t iLeftBorderedPoint2 = 7;
  size_t iLeftBorderedPoint3 = 7;
  size_t iLeftBorderedPoint4 = 7;
  size_t iLeftBorderedPoint5 = 1;
  size_t iLeftBorderedPoint6 = 2;
  size_t iLeftBorderedPoint7 = 0;
  size_t iLeftBorderedPoint8 = 0;

  // Индексы правых граничных точек
  size_t iRightBorderedPoint1 = 3;
  size_t iRightBorderedPoint2 = 4;
  size_t iRightBorderedPoint3 = 4;
  size_t iRightBorderedPoint4 = 0;
  size_t iRightBorderedPoint5 = 0;
  size_t iRightBorderedPoint6 = 0;
  size_t iRightBorderedPoint7 = 3;
  size_t iRightBorderedPoint8 = 27;

  // Левые граничные точки полигонов
  Point2D leftBorderedPoint1;
  Point2D leftBorderedPoint2;
  Point2D leftBorderedPoint3;
  Point2D leftBorderedPoint4;
  Point2D leftBorderedPoint5;
  Point2D leftBorderedPoint6;
  Point2D leftBorderedPoint7;
  Point2D leftBorderedPoint8;

  // Правые граничные точки полигонов
  Point2D rightBorderedPoint1;
  Point2D rightBorderedPoint2;
  Point2D rightBorderedPoint3;
  Point2D rightBorderedPoint4;
  Point2D rightBorderedPoint5;
  Point2D rightBorderedPoint6;
  Point2D rightBorderedPoint7;
  Point2D rightBorderedPoint8;

  // Сектора для полигонов
  SectorPoly sector1 = SectorPoly(Point2D(450., 700.), Point2D(450., 300.));
  SectorPoly sector2 = SectorPoly(Point2D(500., 650.), Point2D(500., 350.));
  SectorPoly sector3 = SectorPoly(Point2D(550., 650.), Point2D(550., 350.));
  SectorPoly sector4 = SectorPoly(Point2D(600., 500.), Point2D(650., 700.));
  SectorPoly sector5 = SectorPoly(Point2D(600., 400.), Point2D(600., 495.));
  SectorPoly sector6 = SectorPoly(Point2D(550., 350.), Point2D(750., 250.));
  SectorPoly sector7 = SectorPoly(Point2D(850., 350.), Point2D(850., 550.));
  SectorPoly sector8 = SectorPoly(Point2D(550., 450.), Point2D(150., 450.));

  Point2D                          center;         // точка центра
                                                   // окружности построения
  double                           radius;         // радиус окружности 
                                                   // построения
  double                           widthExpansion; // ширина расширения
};
//==============================================================================
TEST_F( SectorsCalculatorTest, getPolyCircleTest)
{
  // Проверка получения окружности на плоскости
  Polygon2D circle = getPolyCircle(center, radius);

  ASSERT_FALSE(circle.outer().empty());

  Polygon2D circle2 = getPolyCircle(Point2D(1., 1.), 1.);

  ASSERT_FALSE(circle.outer().empty());
  ASSERT_NEAR(AbstractOperations::area(circle2), M_PI, 0.1);

  // Нулевой радиус (вернётся точка)
  Polygon2D circle3 = getPolyCircle(Point2D(1., 4.), 0.);
  ASSERT_EQ(circle3.outer().size(), 1);
  ASSERT_EQ(circle3.outer()[0], Point2D(1., 4.));

  ASSERT_THROW(getPolyCircle(Point2D(1., 4.), -1.), GeometryFailure);

  try
  {
    Polygon2D circle4 = getPolyCircle(Point2D(1., 4.), -1.);
  }
  catch (const GeometryFailure& exception)
  {
    ASSERT_EQ(NegativeRadius, exception.getCode());
    ASSERT_EQ(geomLibTitle, exception.getLib());
    ASSERT_EQ(
      "[Core::Geometry] <NegativeRadius>: Построение окружности с отрицательным радиусом"
      , std::string(exception.what()));
  }
}
//==============================================================================
TEST_F( SectorsCalculatorTest, findPolygonLeftAndRightPointsTest)
{
  // Проверка поиска правых и левых точек полигона
  std::vector<size_t> iLeftPoints, iRightPoints;
  findPolygonLeftAndRightPoints(zone1, center, iLeftPoints, iRightPoints);
  ASSERT_EQ(iLeftPoints[0], iLeftPoints1[0]);
  ASSERT_EQ(iRightPoints[0], iRightPoints1[0]);
  
  findPolygonLeftAndRightPoints(zone2, center, iLeftPoints, iRightPoints);
  ASSERT_EQ(iLeftPoints[0], iLeftPoints2[0]);
  ASSERT_EQ(iLeftPoints[1], iLeftPoints2[1]);
  ASSERT_EQ(iRightPoints[0], iRightPoints2[0]);
  ASSERT_EQ(iRightPoints[1], iRightPoints2[1]);

  findPolygonLeftAndRightPoints(zone3, center, iLeftPoints, iRightPoints);
  ASSERT_EQ(iLeftPoints[0], iLeftPoints3[0]);
  ASSERT_EQ(iRightPoints[0], iRightPoints3[0]);

  findPolygonLeftAndRightPoints(zone4, center, iLeftPoints, iRightPoints);
  ASSERT_EQ(iLeftPoints[0], iLeftPoints4[0]);
  ASSERT_EQ(iLeftPoints[1], iLeftPoints4[1]);
  ASSERT_EQ(iRightPoints[0], iRightPoints4[0]);
  ASSERT_EQ(iRightPoints[1], iRightPoints4[1]);
  ASSERT_EQ(iRightPoints[2], iRightPoints4[2]);

  findPolygonLeftAndRightPoints(zone5, center, iLeftPoints, iRightPoints);
  ASSERT_EQ(iLeftPoints[0], iLeftPoints5[0]);
  ASSERT_EQ(iLeftPoints[1], iLeftPoints5[1]);
  ASSERT_EQ(iLeftPoints[2], iLeftPoints5[2]);
  ASSERT_EQ(iRightPoints[0], iRightPoints5[0]);
  ASSERT_EQ(iRightPoints[1], iRightPoints5[1]);
  ASSERT_EQ(iRightPoints[2], iRightPoints5[2]);

  findPolygonLeftAndRightPoints(zone6, center, iLeftPoints, iRightPoints);
  ASSERT_EQ(iLeftPoints[0], iLeftPoints6[0]);
  ASSERT_EQ(iLeftPoints[1], iLeftPoints6[1]);
  ASSERT_EQ(iLeftPoints[2], iLeftPoints6[2]);
  ASSERT_EQ(iRightPoints[0], iRightPoints6[0]);
  ASSERT_EQ(iRightPoints[1], iRightPoints6[1]);
  ASSERT_EQ(iRightPoints[2], iRightPoints6[2]);

  findPolygonLeftAndRightPoints(zone7, center, iLeftPoints, iRightPoints);
  ASSERT_EQ(iLeftPoints[0], iLeftPoints7[0]);
  ASSERT_EQ(iLeftPoints[1], iLeftPoints7[1]);
  ASSERT_EQ(iRightPoints[0], iRightPoints7[0]);
  ASSERT_EQ(iRightPoints[1], iRightPoints7[1]);

  findPolygonLeftAndRightPoints(zone8, center, iLeftPoints, iRightPoints);
  ASSERT_EQ(iLeftPoints[0], iLeftPoints8[0]);
  ASSERT_EQ(iLeftPoints[1], iLeftPoints8[1]);
  ASSERT_EQ(iLeftPoints[2], iLeftPoints8[2]);
  ASSERT_EQ(iLeftPoints[3], iLeftPoints8[3]);
  ASSERT_EQ(iLeftPoints[4], iLeftPoints8[4]);
  ASSERT_EQ(iLeftPoints[5], iLeftPoints8[5]);
  ASSERT_EQ(iLeftPoints[6], iLeftPoints8[6]);
  ASSERT_EQ(iLeftPoints[7], iLeftPoints8[7]);
  ASSERT_EQ(iRightPoints[0], iRightPoints8[0]);
  ASSERT_EQ(iRightPoints[1], iRightPoints8[1]);
  ASSERT_EQ(iRightPoints[2], iRightPoints8[2]);
  ASSERT_EQ(iRightPoints[3], iRightPoints8[3]);
  ASSERT_EQ(iRightPoints[4], iRightPoints8[4]);
  ASSERT_EQ(iRightPoints[5], iRightPoints8[5]);
  ASSERT_EQ(iRightPoints[6], iRightPoints8[6]);
  ASSERT_EQ(iRightPoints[7], iRightPoints8[7]);

  Polygon2D emptyPolygon;
  ASSERT_THROW(findPolygonLeftAndRightPoints(emptyPolygon, center,
                                             iLeftPoints, iRightPoints),
               GeometryFailure);
  try
  {
    findPolygonLeftAndRightPoints(emptyPolygon, center, iLeftPoints, iRightPoints);
  }
  catch (const GeometryFailure& exception)
  {
    ASSERT_EQ(EmptyPolygon, exception.getCode());
    ASSERT_EQ(geomLibTitle, exception.getLib());
    ASSERT_EQ(
      "[Core::Geometry] <EmptyPolygon>: Пустой полигон"
      , std::string(exception.what()));
  }
}
//==============================================================================
TEST_F(SectorsCalculatorTest, findPolygonBorderedPointTest)
{
  // Проверка поиска граничной точки полигона
  size_t iLeftBorderedPoint, iRightBorderedPoint;
  iLeftBorderedPoint
    = findPolygonBorderedPoint(zone1, center, iLeftPoints1, false);
  iRightBorderedPoint
    = findPolygonBorderedPoint(zone1, center, iRightPoints1, true);
  ASSERT_EQ(zone1.outer()[iLeftBorderedPoint], leftBorderedPoint1);
  ASSERT_EQ(zone1.outer()[iRightBorderedPoint], rightBorderedPoint1);

  iLeftBorderedPoint
    = findPolygonBorderedPoint(zone2, center, iLeftPoints2, false);
  iRightBorderedPoint
    = findPolygonBorderedPoint(zone2, center, iRightPoints2, true);
  ASSERT_EQ(zone2.outer()[iLeftBorderedPoint], leftBorderedPoint2);
  ASSERT_EQ(zone2.outer()[iRightBorderedPoint], rightBorderedPoint2);

  iLeftBorderedPoint
    = findPolygonBorderedPoint(zone3, center, iLeftPoints3, false);
  iRightBorderedPoint
    = findPolygonBorderedPoint(zone3, center, iRightPoints3, true);
  ASSERT_EQ(zone3.outer()[iLeftBorderedPoint], leftBorderedPoint3);
  ASSERT_EQ(zone3.outer()[iRightBorderedPoint], rightBorderedPoint3);

  iLeftBorderedPoint
    = findPolygonBorderedPoint(zone4, center, iLeftPoints4, false);
  iRightBorderedPoint
    = findPolygonBorderedPoint(zone4, center, iRightPoints4, true);
  ASSERT_EQ(zone4.outer()[iLeftBorderedPoint], leftBorderedPoint4);
  ASSERT_EQ(zone4.outer()[iRightBorderedPoint], rightBorderedPoint4);

  iLeftBorderedPoint
    = findPolygonBorderedPoint(zone5, center, iLeftPoints5, false);
  iRightBorderedPoint
    = findPolygonBorderedPoint(zone5, center, iRightPoints5, true);
  ASSERT_EQ(zone5.outer()[iLeftBorderedPoint], leftBorderedPoint5);
  ASSERT_EQ(zone5.outer()[iRightBorderedPoint], rightBorderedPoint5);

  iLeftBorderedPoint
    = findPolygonBorderedPoint(zone6, center, iLeftPoints6, false);
  iRightBorderedPoint
    = findPolygonBorderedPoint(zone6, center, iRightPoints6, true);
  ASSERT_EQ(zone6.outer()[iLeftBorderedPoint], leftBorderedPoint6);
  ASSERT_EQ(zone6.outer()[iRightBorderedPoint], rightBorderedPoint6);

  iLeftBorderedPoint
    = findPolygonBorderedPoint(zone7, center, iLeftPoints7, false);
  iRightBorderedPoint
    = findPolygonBorderedPoint(zone7, center, iRightPoints7, true);
  ASSERT_EQ(zone7.outer()[iLeftBorderedPoint], leftBorderedPoint7);
  ASSERT_EQ(zone7.outer()[iRightBorderedPoint], rightBorderedPoint7);

  iLeftBorderedPoint
    = findPolygonBorderedPoint(zone8, center, iLeftPoints8, false);
  iRightBorderedPoint
    = findPolygonBorderedPoint(zone8, center, iRightPoints8, true);
  ASSERT_EQ(zone8.outer()[iLeftBorderedPoint], leftBorderedPoint8);
  ASSERT_EQ(zone8.outer()[iRightBorderedPoint], rightBorderedPoint8);

  Polygon2D emptyPolygon;
  ASSERT_THROW(findPolygonBorderedPoint(emptyPolygon, center,
                                        iRightPoints8, true),
               GeometryFailure);
  try
  {
    findPolygonBorderedPoint(emptyPolygon, center, iRightPoints8, true);
  }
  catch (const GeometryFailure& exception)
  {
    ASSERT_EQ(EmptyPolygon, exception.getCode());
    ASSERT_EQ(geomLibTitle, exception.getLib());
    ASSERT_EQ(
      "[Core::Geometry] <EmptyPolygon>: Пустой полигон"
      , std::string(exception.what()));
  }

  std::vector<size_t> iRightPointsEmpty;
  ASSERT_THROW(findPolygonBorderedPoint(zone8, center,
                                        iRightPointsEmpty, true),
               GeometryFailure);
  try
  {
    findPolygonBorderedPoint(zone8, center, iRightPointsEmpty, true);
  }
  catch (const GeometryFailure& exception)
  {
    ASSERT_EQ(NoBorderedPointsInPolygon, exception.getCode());
    ASSERT_EQ(geomLibTitle, exception.getLib());
    ASSERT_EQ(
      "[Core::Geometry] <NoBorderedPointsInPolygon>: Не удалось найти граничные точки полигона"
      , std::string(exception.what()));
  }
}
//==============================================================================
TEST_F( SectorsCalculatorTest, getSectorPolygonTest)
{
  // Проверка построения сектора в виде полигона
  double eps = 0.00005;

  SectorPoly sector(sector1.rightPoint, sector1.leftPoint);
  getSectorPolygon(
    zone1, center, radius,
    iLeftBorderedPoint1, iRightBorderedPoint1, sector);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[0], sector1.poly.outer()[0]) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[1], sector1.poly.outer()[1]) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[2], sector1.poly.outer()[2]) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[3], sector1.poly.outer()[3]) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[4], sector1.poly.outer()[4]) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[5], sector1.poly.outer()[5]) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[6], sector1.poly.outer()[6]) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[7], sector1.poly.outer()[7]) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[8], sector1.poly.outer()[8]) < eps);

  sector.leftPoint = sector2.leftPoint;
  sector.rightPoint = sector2.rightPoint;
  sector.poly.clear();
  getSectorPolygon(
    zone2, center, radius,
    iLeftBorderedPoint2, iRightBorderedPoint2, sector);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[0], sector2.poly.outer()[0]) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[1], sector2.poly.outer()[1]) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[2], sector2.poly.outer()[2]) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[3], sector2.poly.outer()[3]) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[4], sector2.poly.outer()[4]) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[5], sector2.poly.outer()[5]) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[6], sector2.poly.outer()[6]) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[7], sector2.poly.outer()[7]) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[8], sector2.poly.outer()[8]) < eps);

  sector.leftPoint = sector3.leftPoint;
  sector.rightPoint = sector3.rightPoint;
  sector.poly.clear();
  getSectorPolygon(
    zone3, center, radius,
    iLeftBorderedPoint3, iRightBorderedPoint3, sector);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[0], sector3.poly.outer()[0]) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[1], sector3.poly.outer()[1]) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[2], sector3.poly.outer()[2]) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[3], sector3.poly.outer()[3]) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[4], sector3.poly.outer()[4]) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[5], sector3.poly.outer()[5]) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[6], sector3.poly.outer()[6]) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[7], sector3.poly.outer()[7]) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[8], sector3.poly.outer()[8]) < eps);

  sector.leftPoint = sector4.leftPoint;
  sector.rightPoint = sector4.rightPoint;
  sector.poly.clear();
  getSectorPolygon(
    zone4, center, radius,
    iLeftBorderedPoint4, iRightBorderedPoint4, sector);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[0], sector4.poly.outer()[0]) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[1], sector4.poly.outer()[1]) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[2], sector4.poly.outer()[2]) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[3], sector4.poly.outer()[3]) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[4], sector4.poly.outer()[4]) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[5], sector4.poly.outer()[5]) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[6], sector4.poly.outer()[6]) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[7], sector4.poly.outer()[7]) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[8], sector4.poly.outer()[8]) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[9], sector4.poly.outer()[9]) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[10], sector4.poly.outer()[10]) < eps);

  sector.leftPoint = sector5.leftPoint;
  sector.rightPoint = sector5.rightPoint;
  sector.poly.clear();
  getSectorPolygon(
    zone5, center, radius,
    iLeftBorderedPoint5, iRightBorderedPoint5, sector);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[0], sector5.poly.outer()[0]) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[1], sector5.poly.outer()[1]) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[2], sector5.poly.outer()[2]) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[3], sector5.poly.outer()[3]) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[4], sector5.poly.outer()[4]) < eps);

  sector.leftPoint = sector6.leftPoint;
  sector.rightPoint = sector6.rightPoint;
  sector.poly.clear();
  getSectorPolygon(
    zone6, center, radius,
    iLeftBorderedPoint6, iRightBorderedPoint6, sector);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[0], sector6.poly.outer()[0]) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[1], sector6.poly.outer()[1]) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[2], sector6.poly.outer()[2]) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[3], sector6.poly.outer()[3]) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[4], sector6.poly.outer()[4]) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[5], sector6.poly.outer()[5]) < eps);

  sector.leftPoint = sector7.leftPoint;
  sector.rightPoint = sector7.rightPoint;
  sector.poly.clear();
  getSectorPolygon(
    zone7, center, radius,
    iLeftBorderedPoint7, iRightBorderedPoint7, sector);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[0], sector7.poly.outer()[0]) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[1], sector7.poly.outer()[1]) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[2], sector7.poly.outer()[2]) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[3], sector7.poly.outer()[3]) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[4], sector7.poly.outer()[4]) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[5], sector7.poly.outer()[5]) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[6], sector7.poly.outer()[6]) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[7], sector7.poly.outer()[7]) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[8], sector7.poly.outer()[8]) < eps);

  sector.leftPoint = sector8.leftPoint;
  sector.rightPoint = sector8.rightPoint;
  sector.poly.clear();
  getSectorPolygon(
    zone8, center, radius,
    iLeftBorderedPoint8, iRightBorderedPoint8, sector);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[0], sector8.poly.outer()[0]) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[1], sector8.poly.outer()[1]) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[2], sector8.poly.outer()[2]) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[3], sector8.poly.outer()[3]) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[4], sector8.poly.outer()[4]) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[5], sector8.poly.outer()[5]) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[6], sector8.poly.outer()[6]) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[7], sector8.poly.outer()[7]) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[8], sector8.poly.outer()[8]) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[9], sector8.poly.outer()[9]) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[10], sector8.poly.outer()[10]) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[11], sector8.poly.outer()[11]) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[12], sector8.poly.outer()[12]) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[13], sector8.poly.outer()[13]) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[14], sector8.poly.outer()[14]) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[15], sector8.poly.outer()[15]) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[16], sector8.poly.outer()[16]) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[17], sector8.poly.outer()[17]) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[18], sector8.poly.outer()[18]) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[19], sector8.poly.outer()[19]) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[20], sector8.poly.outer()[20]) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[21], sector8.poly.outer()[21]) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[22], sector8.poly.outer()[22]) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[23], sector8.poly.outer()[23]) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[24], sector8.poly.outer()[24]) < eps);

  // угол меньше 90 градусов
  Polygon2D smallZone({ {Point2D(550, 700), Point2D(700, 700), Point2D(700, 550),
    Point2D(650, 550), Point2D(650, 650), Point2D(550, 650), Point2D(550, 700)} });
  sector.leftPoint = Point2D(550, 700);
  sector.rightPoint = Point2D(700, 550);
  sector.poly.clear();
  getSectorPolygon(smallZone, center, radius, 0, 2, sector);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[0], Point2D(613.13708, 952.54834)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[1], Point2D(952.54834, 613.13708)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[2], Point2D(700, 550)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[3], Point2D(650, 550)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[4], Point2D(650, 650)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[5], Point2D(550, 650)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[6], Point2D(550, 700)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[7], Point2D(613.13708, 952.54834)) < eps);

  // угол 90 градусов
  Polygon2D zone90({ { Point2D(500, 700), Point2D(700, 700), Point2D(700, 500),
    Point2D(650, 500), Point2D(650, 650), Point2D(500, 650), Point2D(500, 700) } });
  sector.leftPoint = Point2D(500, 650);
  sector.rightPoint = Point2D(650, 500);
  sector.poly.clear();
  getSectorPolygon(zone90, center, radius, 5, 3, sector);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[0], Point2D(500, 1065.68542)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[1], Point2D(1065.68542, 500)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[2], Point2D(650, 500)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[3], Point2D(650, 650)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[4], Point2D(500, 650)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[5], Point2D(500, 1065.68542)) < eps);

  // угол больше 90 и меньше 180 градусов
  Polygon2D middleZone({ { Point2D(500, 700), Point2D(700, 700), Point2D(700, 400),
    Point2D(650, 400), Point2D(650, 650), Point2D(500, 650), Point2D(500, 700) } });
  sector.leftPoint = Point2D(500, 650);
  sector.rightPoint = Point2D(650, 400);
  sector.poly.clear();
  getSectorPolygon(middleZone, center, radius, 5, 3, sector);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[0], Point2D(500, 900)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[1], Point2D(739.60862, 900)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[2], Point2D(965.73103, 477.48634)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[3], Point2D(832.82012, 278.11992)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[4], Point2D(650, 400)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[5], Point2D(650, 650)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[6], Point2D(500, 650)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[7], Point2D(500, 900)) < eps);

  // угол больше 180 и меньше 270 градусов
  Polygon2D middleZone2({ { Point2D(500, 700), Point2D(700, 700), Point2D(700, 300),
    Point2D(300, 300), Point2D(300, 400), Point2D(350, 400), Point2D(350, 350), 
    Point2D(650, 350), Point2D(650, 650), Point2D(500, 650),  Point2D(500, 700) } });
  sector.leftPoint = Point2D(500, 650);
  sector.rightPoint = Point2D(300, 400);
  sector.poly.clear();
  getSectorPolygon(middleZone2, center, radius, 9, 4, sector);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[0], Point2D(500, 900)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[1], Point2D(1217.4418, 900)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[2], Point2D(463.07885, -320.58489)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[3], Point2D(142.22912, 321.11456)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[4], Point2D(300, 400)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[5], Point2D(350, 400)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[6], Point2D(350, 350)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[7], Point2D(650, 350)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[8], Point2D(650, 650)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[9], Point2D(500, 650)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[10], Point2D(500, 900)) < eps);

  // угол 270 градусов
  Polygon2D zone270({ { Point2D(500, 700), Point2D(700, 700), Point2D(700, 300),
    Point2D(300, 300), Point2D(300, 500), Point2D(350, 500), Point2D(350, 350),
    Point2D(650, 350), Point2D(650, 650), Point2D(500, 650),  Point2D(500, 700) } });
  sector.leftPoint = Point2D(500, 650);
  sector.rightPoint = Point2D(300, 500);
  sector.poly.clear();
  getSectorPolygon(zone270, center, radius, 9, 5, sector);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[0], Point2D(500, 900)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[1], Point2D(1465.68542, 900)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[2], Point2D(100, -465.68542)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[3], Point2D(100, 500)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[4], Point2D(350, 500)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[5], Point2D(350, 350)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[6], Point2D(650, 350)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[7], Point2D(650, 650)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[8], Point2D(500, 650)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[9], Point2D(500, 900)) < eps);

  // угол больше 270 градусов
  Polygon2D bigZone({ { Point2D(500, 700), Point2D(700, 700), Point2D(700, 300),
    Point2D(300, 300), Point2D(300, 600), Point2D(350, 600), Point2D(350, 350),
    Point2D(650, 350), Point2D(650, 650), Point2D(500, 650),  Point2D(500, 700) } });
  sector.leftPoint = Point2D(500, 650);
  sector.rightPoint = Point2D(350, 600);
  sector.poly.clear();  getSectorPolygon(bigZone, center, radius, 9, 5, sector);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[0], Point2D(500, 900)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[1], Point2D(900, 900)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[2], Point2D(900, 260.39138)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[3], Point2D(477.48634, 34.26894)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[4], Point2D(-54.7002, 389.05996)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[5], Point2D(167.17988, 721.88008)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[6], Point2D(350, 600)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[7], Point2D(350, 350)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[8], Point2D(650, 350)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[9], Point2D(650, 650)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[10], Point2D(500, 650)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[11], Point2D(500, 900)) < eps);

  // угол почти 360 градусов
  Polygon2D almostFullZone({ { Point2D(500, 700), Point2D(700, 700), 
    Point2D(700, 300), Point2D(300, 300), Point2D(300, 700), Point2D(450, 700), 
    Point2D(450, 650), Point2D(350, 650), Point2D(350, 350), Point2D(650, 350), 
    Point2D(650, 650), Point2D(500, 650),  Point2D(500, 700) } });
  sector.leftPoint = Point2D(500, 650);
  sector.rightPoint = Point2D(450, 700);
  sector.poly.clear();
  getSectorPolygon(almostFullZone, center, radius, 11, 5, sector);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[0], Point2D(500, 900)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[1], Point2D(900, 900)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[2], Point2D(900, 146.22265)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[3], Point2D(197.74661, 59.77131)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[4], Point2D(14.92875, 791.04275)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[5], Point2D(402.98575, 888.057)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[6], Point2D(450, 700)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[7], Point2D(450, 650)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[8], Point2D(350, 650)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[9], Point2D(350, 350)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[10], Point2D(650, 350)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[11], Point2D(650, 650)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[12], Point2D(500, 650)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector.poly.outer()[13], Point2D(500, 900)) < eps);

  Polygon2D emptyPolygon;
  ASSERT_THROW(getSectorPolygon(emptyPolygon, center, radius,
                                iLeftBorderedPoint8,
                                iRightBorderedPoint8, sector),
               GeometryFailure);
  try
  {
    getSectorPolygon(emptyPolygon, center, radius, 
      iLeftBorderedPoint8, iRightBorderedPoint8, sector);
  }
  catch (const GeometryFailure& exception)
  {
    ASSERT_EQ(EmptyPolygon, exception.getCode());
    ASSERT_EQ(geomLibTitle, exception.getLib());
    ASSERT_EQ(
      "[Core::Geometry] <EmptyPolygon>: Пустой полигон"
      , std::string(exception.what()));
  }

  ASSERT_THROW(getSectorPolygon(zone8, center, -1, iLeftBorderedPoint8,
                                iRightBorderedPoint8, sector),
               GeometryFailure);
  try
  {
    getSectorPolygon(zone8, center, -1,
      iLeftBorderedPoint8, iRightBorderedPoint8, sector);
  }
  catch (const GeometryFailure& exception)
  {
    ASSERT_EQ(NegativeRadius, exception.getCode());
    ASSERT_EQ(geomLibTitle, exception.getLib());
    ASSERT_EQ(
      "[Core::Geometry] <NegativeRadius>: Построение окружности с отрицательным радиусом"
      , std::string(exception.what()));
  }
}
//==============================================================================
TEST_F( SectorsCalculatorTest, expandSectorsBordersTest)
{
  // Проверка расширения сектора с двух сторон
  double eps = 0.00005;

  expandSectorsBorders(sector1, center, radius, widthExpansion);
  ASSERT_TRUE(
    Geometry::distance(sector1.poly.outer()[0], Point2D(100, 812.31056))
    < eps);
  ASSERT_TRUE(
    Geometry::distance(sector1.poly.outer()[1], Point2D(402.98575, 888.057)) 
    < eps);
  ASSERT_TRUE(
    Geometry::distance(sector1.poly.outer()[2], Point2D(450.76056, 896.95778))
    < eps);
  ASSERT_TRUE(
    Geometry::distance(sector1.poly.outer()[3], Point2D(474.80986, 703.07747))
    < eps);
  ASSERT_TRUE(
    Geometry::distance(sector1.poly.outer()[4], Point2D(450, 700)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector1.poly.outer()[5], Point2D(450, 650)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector1.poly.outer()[6], Point2D(400, 650)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector1.poly.outer()[7], Point2D(400, 350)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector1.poly.outer()[8], Point2D(450, 350)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector1.poly.outer()[9], Point2D(450, 300)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector1.poly.outer()[10], Point2D(474.80986, 296.92253))
    < eps);
  ASSERT_TRUE(
    Geometry::distance(sector1.poly.outer()[11], Point2D(450.76056, 103.04222))
    < eps);
  ASSERT_TRUE(
    Geometry::distance(sector1.poly.outer()[12], Point2D(402.98575, 111.943)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector1.poly.outer()[13], Point2D(100, 187.68944)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector1.poly.outer()[14], Point2D(100, 812.31056))
    < eps);

  expandSectorsBorders(sector2, center, radius, widthExpansion);
  ASSERT_TRUE(
    Geometry::distance(sector2.poly.outer()[0], Point2D(100, 900)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector2.poly.outer()[1], Point2D(500, 900)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector2.poly.outer()[2], Point2D(566.66667, 894.40532))
    < eps);
  ASSERT_TRUE(
    Geometry::distance(sector2.poly.outer()[3], Point2D(524.65033, 645.83333))
    < eps);
  ASSERT_TRUE(
    Geometry::distance(sector2.poly.outer()[4], Point2D(500, 650)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector2.poly.outer()[5], Point2D(400, 650)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector2.poly.outer()[6], Point2D(400, 350)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector2.poly.outer()[7], Point2D(500, 350)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector2.poly.outer()[8], Point2D(524.65033, 354.16667))
    < eps);
  ASSERT_TRUE(
    Geometry::distance(sector2.poly.outer()[9], Point2D(566.66667, 105.59468)) 
    < eps);
  ASSERT_TRUE(
    Geometry::distance(sector2.poly.outer()[10], Point2D(500, 100)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector2.poly.outer()[11], Point2D(100, 100)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector2.poly.outer()[12], Point2D(100, 900)) < eps);

  expandSectorsBorders(sector3, center, radius, widthExpansion);
  ASSERT_TRUE(
    Geometry::distance(sector3.poly.outer()[0], Point2D(100, 1054.97035)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector3.poly.outer()[1], Point2D(626.49111, 879.47332))
    < eps);
  ASSERT_TRUE(
    Geometry::distance(sector3.poly.outer()[2], Point2D(684.89996, 854.69988))
    < eps);
  ASSERT_TRUE(
    Geometry::distance(sector3.poly.outer()[3], Point2D(572.16874, 638.44375))
    < eps);
  ASSERT_TRUE(
    Geometry::distance(sector3.poly.outer()[4], Point2D(550, 650)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector3.poly.outer()[5], Point2D(400, 650)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector3.poly.outer()[6], Point2D(400, 350)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector3.poly.outer()[7], Point2D(550, 350)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector3.poly.outer()[8], Point2D(572.16874, 361.55625)) 
    < eps);
  ASSERT_TRUE(
    Geometry::distance(sector3.poly.outer()[9], Point2D(684.89996, 145.30012)) 
    < eps);
  ASSERT_TRUE(
    Geometry::distance(sector3.poly.outer()[10], Point2D(626.49111, 120.52668)) 
    < eps);
  ASSERT_TRUE(
    Geometry::distance(sector3.poly.outer()[11], Point2D(100, -54.97035)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector3.poly.outer()[12], Point2D(100, 1054.97035)) < eps);

  expandSectorsBorders(sector4, center, radius, widthExpansion);
  ASSERT_TRUE(
    Geometry::distance(sector4.poly.outer()[0], Point2D(933.0127, 388.1966))
    < eps);
  ASSERT_TRUE(
    Geometry::distance(sector4.poly.outer()[1], Point2D(593.75, 475.79385))
    < eps);
  ASSERT_TRUE(
    Geometry::distance(sector4.poly.outer()[2], Point2D(600, 500)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector4.poly.outer()[3], Point2D(600, 600)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector4.poly.outer()[4], Point2D(650, 600)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector4.poly.outer()[5], Point2D(650, 550)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector4.poly.outer()[6], Point2D(700, 550)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector4.poly.outer()[7], Point2D(700, 650)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector4.poly.outer()[8], Point2D(650, 650)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector4.poly.outer()[9], Point2D(650, 700)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector4.poly.outer()[10], Point2D(628.60025, 712.92481)) 
    < eps);
  ASSERT_TRUE(
    Geometry::distance(sector4.poly.outer()[11], Point2D(731.20606, 882.81034)) 
    < eps);
  ASSERT_TRUE(
    Geometry::distance(sector4.poly.outer()[12], Point2D(768.32816, 857.77088)) 
    < eps);
  ASSERT_TRUE(
    Geometry::distance(sector4.poly.outer()[13], Point2D(947.2136, 500)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector4.poly.outer()[14], Point2D(933.0127, 388.1966))
    < eps);

  expandSectorsBorders(sector5, center, radius, widthExpansion);
  ASSERT_TRUE(
    Geometry::distance(sector5.poly.outer()[0], Point2D(744.74988, 148.08615))
    < eps);
  ASSERT_TRUE(
    Geometry::distance(sector5.poly.outer()[1], Point2D(579.47574, 385.72574))
    < eps);
  ASSERT_TRUE(
    Geometry::distance(sector5.poly.outer()[2], Point2D(600, 400)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector5.poly.outer()[3], Point2D(600, 495)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector5.poly.outer()[4], Point2D(594.97448, 519.48968)) 
    < eps);
  ASSERT_TRUE(
    Geometry::distance(sector5.poly.outer()[5], Point2D(919.90573, 586.16868)) 
    < eps);
  ASSERT_TRUE(
    Geometry::distance(sector5.poly.outer()[6], Point2D(928.12107, 478.59395)) 
    < eps);
  ASSERT_TRUE(
    Geometry::distance(sector5.poly.outer()[7], Point2D(803.10548, 196.89452)) 
    < eps);
  ASSERT_TRUE(
    Geometry::distance(sector5.poly.outer()[8], Point2D(744.74988, 148.08615))
    < eps);

  expandSectorsBorders(sector6, center, radius, widthExpansion);
  ASSERT_TRUE(
    Geometry::distance(sector6.poly.outer()[0], Point2D(566.68382, 94.45128))
    < eps);
  ASSERT_TRUE(
    Geometry::distance(sector6.poly.outer()[1], Point2D(525.33126, 345.94375))
    < eps);
  ASSERT_TRUE(
    Geometry::distance(sector6.poly.outer()[2], Point2D(550, 350)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector6.poly.outer()[3], Point2D(550, 445)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector6.poly.outer()[4], Point2D(561.76471, 467.05882)) 
    < eps);
  ASSERT_TRUE(
    Geometry::distance(sector6.poly.outer()[5], Point2D(862.64222, 306.59081)) 
    < eps);
  ASSERT_TRUE(
    Geometry::distance(sector6.poly.outer()[6], Point2D(788.56615, 211.43385)) 
    < eps);
  ASSERT_TRUE(
    Geometry::distance(sector6.poly.outer()[7], Point2D(790.61701, 209.38299)) 
    < eps);
  ASSERT_TRUE(
    Geometry::distance(sector6.poly.outer()[8], Point2D(783.58774, 205.03866)) 
    < eps);
  ASSERT_TRUE(
    Geometry::distance(sector6.poly.outer()[9], Point2D(776.46483, 195.88869)) 
    < eps);
  ASSERT_TRUE(
    Geometry::distance(sector6.poly.outer()[10], Point2D(773.70134, 198.92853)) 
    < eps);
  ASSERT_TRUE(
    Geometry::distance(sector6.poly.outer()[11], Point2D(629.96788, 110.09637)) 
    < eps);
  ASSERT_TRUE(
    Geometry::distance(sector6.poly.outer()[12], Point2D(566.68382, 94.45128))
    < eps);

  expandSectorsBorders(sector7, center, radius, widthExpansion);
  ASSERT_TRUE(
    Geometry::distance(sector7.poly.outer()[0], Point2D(870.27188, 311.63855))
    < eps);
  ASSERT_TRUE(
    Geometry::distance(sector7.poly.outer()[1], Point2D(838.66464, 327.7175))
    < eps);
  ASSERT_TRUE(
    Geometry::distance(sector7.poly.outer()[2], Point2D(850, 350)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector7.poly.outer()[3], Point2D(850, 400)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector7.poly.outer()[4], Point2D(800, 400)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector7.poly.outer()[5], Point2D(800, 450)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector7.poly.outer()[6], Point2D(850, 450)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector7.poly.outer()[7], Point2D(850, 550)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector7.poly.outer()[8], Point2D(844.72332, 574.43679)) 
    < eps);
  ASSERT_TRUE(
    Geometry::distance(sector7.poly.outer()[9], Point2D(906.07007, 587.68351)) 
    < eps);
  ASSERT_TRUE(
    Geometry::distance(sector7.poly.outer()[10], Point2D(911.25378, 558.75054)) 
    < eps);
  ASSERT_TRUE(
    Geometry::distance(sector7.poly.outer()[11], Point2D(881.83955, 336.35448)) 
    < eps);
  ASSERT_TRUE(
    Geometry::distance(sector7.poly.outer()[12], Point2D(870.27188, 311.63855))
    < eps);

  expandSectorsBorders(sector8, center, radius, widthExpansion);
  ASSERT_TRUE(
    Geometry::distance(sector8.poly.outer()[0], Point2D(1240.49184, 674.80641))
    < eps);
  ASSERT_TRUE(
    Geometry::distance(sector8.poly.outer()[1], Point2D(782.84271, 217.15729))
    < eps);
  ASSERT_TRUE(
    Geometry::distance(sector8.poly.outer()[2], Point2D(664.57513, 135.42487))
    < eps);
  ASSERT_TRUE(
    Geometry::distance(sector8.poly.outer()[3], Point2D(527.21405, 439.71405))
    < eps);
  ASSERT_TRUE(
    Geometry::distance(sector8.poly.outer()[4], Point2D(550, 450)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector8.poly.outer()[5], Point2D(650, 500)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector8.poly.outer()[6], Point2D(600, 550)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector8.poly.outer()[7], Point2D(650, 600)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector8.poly.outer()[8], Point2D(600, 600)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector8.poly.outer()[9], Point2D(650, 700)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector8.poly.outer()[10], Point2D(600, 650)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector8.poly.outer()[11], Point2D(600, 750)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector8.poly.outer()[12], Point2D(500, 650)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector8.poly.outer()[13], Point2D(500, 750)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector8.poly.outer()[14], Point2D(450, 700)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector8.poly.outer()[15], Point2D(400, 750)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector8.poly.outer()[16], Point2D(350, 600)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector8.poly.outer()[17], Point2D(300, 700)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector8.poly.outer()[18], Point2D(300, 600)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector8.poly.outer()[19], Point2D(200, 650)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector8.poly.outer()[20], Point2D(250, 550)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector8.poly.outer()[21], Point2D(200, 550)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector8.poly.outer()[22], Point2D(250, 500)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector8.poly.outer()[23], Point2D(150, 450)) < eps);
  ASSERT_TRUE(
    Geometry::distance(sector8.poly.outer()[24], Point2D(155.27668, 425.56321)) 
    < eps);
  ASSERT_TRUE(
    Geometry::distance(sector8.poly.outer()[25], Point2D(109.01139, 415.57306)) 
    < eps);
  ASSERT_TRUE(
    Geometry::distance(sector8.poly.outer()[26], Point2D(104.0202, 443.43146)) 
    < eps);
  ASSERT_TRUE(
    Geometry::distance(sector8.poly.outer()[27], Point2D(12.49038, 1084.14023)) 
    < eps);
  ASSERT_TRUE(
    Geometry::distance(sector8.poly.outer()[28], Point2D(1240.49184, 674.80641))
    < eps);

  // Нулевое расширение сектора
  SectorPoly newSector = sector1;
  expandSectorsBorders(newSector, center, radius, 0);
  for (size_t i = 0; i < sector1.poly.outer().size(); ++i)
  {
    ASSERT_TRUE(Geometry::distance(
      sector1.poly.outer()[i], newSector.poly.outer()[i]) < eps);
  }

  SectorPoly emptySector = SectorPoly(Point2D(450., 700.),
                                      Point2D(450., 300.));
  ASSERT_THROW(expandSectorsBorders(emptySector, center, radius,
                                    widthExpansion),
               GeometryFailure);
  try
  {
    expandSectorsBorders(emptySector, center, radius, widthExpansion);
  }
  catch (const GeometryFailure& exception)
  {
    ASSERT_EQ(EmptySectorPolygon, exception.getCode());
    ASSERT_EQ(geomLibTitle, exception.getLib());
    ASSERT_EQ(
      "[Core::Geometry] <EmptySectorPolygon>: Пустой полигон сектора"
      , std::string(exception.what()));
  }

  ASSERT_THROW(expandSectorsBorders(sector1, center, -1, widthExpansion),
               GeometryFailure);
  try
  {
    expandSectorsBorders(sector1, center, -1, widthExpansion);
  }
  catch (const GeometryFailure& exception)
  {
    ASSERT_EQ(NegativeRadius, exception.getCode());
    ASSERT_EQ(geomLibTitle, exception.getLib());
    ASSERT_EQ(
      "[Core::Geometry] <NegativeRadius>: Построение окружности с отрицательным радиусом"
      , std::string(exception.what()));
  }
}
//==============================================================================
TEST_F( SectorsCalculatorTest, cakeForFinishSectionTest)
{
  // Проверка самого тортика

  // Первая проверка
  MultiPolygon2D zone;
  zone.emplace_back(zone1);
  zone.emplace_back(zone4);
  zone.emplace_back(zone6);
  zone.emplace_back(zone7);
  correct(zone);

  MultiPolygon2D area;
  difference(poly, zone, area);
  correct(area);

  MultiPolygon2D cake 
    = calculateCake(area, center, radius, widthExpansion);

  ASSERT_TRUE(isCorrect(cake));
  ASSERT_EQ(cake.size(), 3);

  double zoneSize1 = AbstractOperations::area(zone1);
  double zoneSize2 = AbstractOperations::area(zone2);
  double zoneSize3 = AbstractOperations::area(zone3);
  double zoneSize4 = AbstractOperations::area(zone4);

  double cakeSize = AbstractOperations::area(cake);

  // Площадь секторов должна быть больше площади исходных полигонов
  // и не больше площади всей окружности
  ASSERT_TRUE(cakeSize > zoneSize1 + zoneSize2 + zoneSize3 + zoneSize4);
  ASSERT_TRUE(cakeSize < 1.01 * M_PI * radius * radius);

  // Вторая проверка
  zone.clear();
  zone.emplace_back(zone2);
  zone.emplace_back(zone4);
  zone.emplace_back(zone6);
  zone.emplace_back(zone7);
  correct(zone);

  area.clear();
  difference(poly, zone, area);
  correct(area);

  cake = calculateCake(area, center, radius, widthExpansion);

  ASSERT_TRUE(isCorrect(cake));
  ASSERT_EQ(cake.size(), 2);

  zoneSize1 = AbstractOperations::area(zone1);
  zoneSize2 = AbstractOperations::area(zone2);
  zoneSize3 = AbstractOperations::area(zone3);
  zoneSize4 = AbstractOperations::area(zone4);

  cakeSize = AbstractOperations::area(cake);

  // Площадь секторов должна быть больше площади исходных полигонов
  // и не больше площади всей окружности
  ASSERT_TRUE(cakeSize > zoneSize1 + zoneSize2 + zoneSize3 + zoneSize4);
  ASSERT_TRUE(cakeSize < 1.01 * M_PI * radius * radius);

  // Третья проверка
  zone.clear();
  zone.emplace_back(zone3);
  zone.emplace_back(zone4);
  zone.emplace_back(zone5);
  correct(zone);

  area.clear();
  difference(poly, zone, area);
  correct(area);

  cake = calculateCake(area, center, radius, widthExpansion);

  ASSERT_TRUE(isCorrect(cake));
  ASSERT_EQ(cake.size(), 2);

  zoneSize1 = AbstractOperations::area(zone1);
  zoneSize2 = AbstractOperations::area(zone2);
  zoneSize3 = AbstractOperations::area(zone3);
  zoneSize4 = AbstractOperations::area(zone4);

  cakeSize = AbstractOperations::area(cake);

  // Площадь секторов должна быть больше площади исходных полигонов
  // и не больше площади всей окружности
  ASSERT_TRUE(cakeSize > zoneSize1 + zoneSize2 + zoneSize3 + zoneSize4);
  ASSERT_TRUE(cakeSize < 1.01 * M_PI * radius * radius);

  // Четвёртая проверка - полный угол сектора
  zone.clear();
  zone.emplace_back(zone9);
  correct(zone);

  area.clear();
  difference(poly, zone, area);
  correct(area);

  cake = calculateCake(area, center, radius, widthExpansion);

  ASSERT_TRUE(isCorrect(cake));
  ASSERT_EQ(cake.size(), 1);

  zoneSize1 = AbstractOperations::area(zone9);
  cakeSize = AbstractOperations::area(cake);

  // Площадь секторов должна быть больше площади исходных полигонов
  // и не больше площади всей окружности
  ASSERT_TRUE(cakeSize > zoneSize1);
  ASSERT_TRUE(cakeSize < 1.01 * M_PI * radius * radius);

  // Пятая проверка - нулевой радиус
  zone.clear();
  zone.emplace_back(zone1);
  correct(zone);

  area.clear();
  difference(poly, zone, area);
  correct(area);
  cake = calculateCake(area, center, 0, widthExpansion);

  ASSERT_EQ(cake.size(), 0);

  // Шестая проверка - препятствий нет
  zone.clear();

  area.clear();
  difference(poly, zone, area);
  correct(area);
  cake = calculateCake(area, center, radius, widthExpansion);

  ASSERT_EQ(cake.size(), 0);

  // Седьмая проверка - препятствия не попали в окружность
  zone.clear();
  Polygon2D zoneOutOfCircle;
  zoneOutOfCircle.outer().push_back(Point2D(400, 300));
  zoneOutOfCircle.outer().push_back(Point2D(500, 300));
  zoneOutOfCircle.outer().push_back(Point2D(500, 200));
  zoneOutOfCircle.outer().push_back(Point2D(400, 200));
  zoneOutOfCircle.outer().push_back(Point2D(400, 300));
  zone.push_back(zoneOutOfCircle);

  area.clear();
  difference(poly, zone, area);
  correct(area);
  cake = calculateCake(area, center, 100, widthExpansion);

  ASSERT_EQ(cake.size(), 0);

  // Восьмая проверка - в проблемной зоне крайняя точка и точка
  // расширения не совпадают
  zone.clear();
  Polygon2D zoneWithProblemAngles;
  zoneWithProblemAngles.outer().push_back(Point2D(550, 550));
  zoneWithProblemAngles.outer().push_back(Point2D(550, 600));
  zoneWithProblemAngles.outer().push_back(Point2D(600, 650));
  zoneWithProblemAngles.outer().push_back(Point2D(900, 650));
  zoneWithProblemAngles.outer().push_back(Point2D(950, 600));
  zoneWithProblemAngles.outer().push_back(Point2D(950, 550));
  zoneWithProblemAngles.outer().push_back(Point2D(900, 500));
  zoneWithProblemAngles.outer().push_back(Point2D(850, 550));
  zoneWithProblemAngles.outer().push_back(Point2D(850, 600));
  zoneWithProblemAngles.outer().push_back(Point2D(800, 600));
  zoneWithProblemAngles.outer().push_back(Point2D(800, 550));
  zoneWithProblemAngles.outer().push_back(Point2D(750, 505));
  zoneWithProblemAngles.outer().push_back(Point2D(700, 550));
  zoneWithProblemAngles.outer().push_back(Point2D(700, 600));
  zoneWithProblemAngles.outer().push_back(Point2D(650, 600));
  zoneWithProblemAngles.outer().push_back(Point2D(650, 550));
  zoneWithProblemAngles.outer().push_back(Point2D(600, 510));
  zoneWithProblemAngles.outer().push_back(Point2D(550, 550));
  zoneWithProblemAngles.outer().push_back(Point2D(550, 550));

  zone.push_back(zoneWithProblemAngles);

  area.clear();
  difference(poly, zone, area);
  correct(area);
  cake = calculateCake(area, center, radius, 50);

  ASSERT_TRUE(isCorrect(cake));
  ASSERT_EQ(cake.size(), 1);

  zoneSize1 = AbstractOperations::area(zoneWithProblemAngles);
  cakeSize = AbstractOperations::area(cake);

  // Площадь секторов должна быть больше площади исходных полигонов
  // и не больше площади всей окружности
  ASSERT_TRUE(cakeSize > zoneSize1);
  ASSERT_TRUE(cakeSize < 1.01 * M_PI * radius * radius);

  // Точка, от которой проводилось расширение
  Point2D pointForExpansion(600, 510);
  // Тогда следующая за ней точка не должна быть на границе сектора
  Point2D innerForSector(650, 550);

  bool expansionPointFound(false);
  bool innerPointFound(false);

  for (const auto& point : cake[0].outer())
  {
    if (point == pointForExpansion)
      expansionPointFound = true;

    if (point == innerForSector)
      innerPointFound = true;
  }

  ASSERT_TRUE(expansionPointFound);
  ASSERT_FALSE(innerPointFound);
  
  // Девятая проверка - полный угол сектора (между правой и левой
  // границами полигона изначально ровно 360 градусов)
  zone.clear();
  zone.emplace_back(zone10);
  correct(zone);

  area.clear();
  difference(poly, zone, area);
  correct(area);

  cake = calculateCake(area, center, radius, widthExpansion);

  ASSERT_TRUE(isCorrect(cake));
  ASSERT_EQ(cake.size(), 1);

  zoneSize1 = AbstractOperations::area(zone9);
  cakeSize = AbstractOperations::area(cake);

  // Площадь секторов должна быть больше площади исходных полигонов
  // и не больше площади всей окружности
  ASSERT_TRUE(cakeSize > zoneSize1);
  ASSERT_TRUE(cakeSize < 1.01 * M_PI * radius * radius);
  
  area.clear();
  ASSERT_THROW(calculateCake(area, center, 100, widthExpansion),
               GeometryFailure);
  try
  {
    cake = calculateCake(area, center, 100, widthExpansion);
  }
  catch (const GeometryFailure& exception)
  {
    ASSERT_EQ(EmptyPolygon, exception.getCode());
    ASSERT_EQ(geomLibTitle, exception.getLib());
    ASSERT_EQ(
      "[Core::Geometry] <EmptyPolygon>: Пустой полигон"
      , std::string(exception.what()));
  }

  // Десятая проверка - центр попал в препятствие
  zone.clear();
  Polygon2D zoneWithCenter;
  zoneWithCenter.outer().push_back(Point2D(600, 600));
  zoneWithCenter.outer().push_back(Point2D(600, 400));
  zoneWithCenter.outer().push_back(Point2D(400, 400));
  zoneWithCenter.outer().push_back(Point2D(400, 600));
  zoneWithCenter.outer().push_back(Point2D(600, 600));

  zone.push_back(zoneWithCenter);

  area.clear();
  difference(poly, zone, area);
  correct(area);
  cake = calculateCake(area, center, radius, 50);

  ASSERT_TRUE(isCorrect(cake));
  ASSERT_EQ(cake.size(), 1);

  zoneSize1 = AbstractOperations::area(zoneWithCenter);
  cakeSize = AbstractOperations::area(cake);

  // Площадь секторов должна быть больше площади исходных полигонов
  // и не больше площади всей окружности
  ASSERT_TRUE(cakeSize > zoneSize1);
  ASSERT_TRUE(cakeSize < 1.01 * M_PI * radius * radius);

  // Одиннадцатая проверка - лишние точки на рёбрах полигона
  zone.clear();
  Polygon2D zoneWitheExcessVertices;
  zoneWitheExcessVertices.outer().push_back(Point2D(600, 600));
  zoneWitheExcessVertices.outer().push_back(Point2D(700, 600));
  zoneWitheExcessVertices.outer().push_back(Point2D(800, 600));
  zoneWitheExcessVertices.outer().push_back(Point2D(800, 500));
  zoneWitheExcessVertices.outer().push_back(Point2D(800, 400));
  zoneWitheExcessVertices.outer().push_back(Point2D(700, 400));
  zoneWitheExcessVertices.outer().push_back(Point2D(600, 400));
  zoneWitheExcessVertices.outer().push_back(Point2D(600, 500));
  zoneWitheExcessVertices.outer().push_back(Point2D(600, 600));

  zone.push_back(zoneWitheExcessVertices);

  area.clear();
  difference(poly, zone, area);
  correct(area);
  cake = calculateCake(area, center, radius, 50);

  ASSERT_TRUE(isCorrect(cake));
  ASSERT_EQ(cake.size(), 1);

  zoneSize1 = AbstractOperations::area(zone);
  cakeSize = AbstractOperations::area(cake);

  // Площадь секторов должна быть больше площади исходных полигонов
  // и не больше площади всей окружности
  ASSERT_TRUE(cakeSize > zoneSize1);
  ASSERT_TRUE(cakeSize < 1.01 * M_PI * radius * radius);

  // Двенадцатая проверка - граничные точки ближе ширины расширения
  // к центру (не удалось построить касательную для расширения)
  zone.clear();
  Polygon2D zoneNearThanWidth;
  zoneNearThanWidth.outer().push_back(Point2D(450, 550));
  zoneNearThanWidth.outer().push_back(Point2D(450, 450));
  zoneNearThanWidth.outer().push_back(Point2D(400, 500));
  zoneNearThanWidth.outer().push_back(Point2D(450, 550));

  zone.push_back(zoneNearThanWidth);

  area.clear();
  difference(poly, zone, area);
  correct(area);

  ASSERT_THROW(calculateCake(area, center, radius, 90), GeometryFailure);
  try
  {
    cake = calculateCake(area, center, radius, 90);
  }
  catch (const GeometryFailure& exception)
  {
    ASSERT_EQ(ExceptionTools::TransitFailure, exception.getCode());
    ASSERT_EQ(geomLibTitle, exception.getLib());
    ASSERT_FALSE(std::string(exception.what()).find("expandSectorsBorders")
      == std::string::npos);
  }

  // Тринадцатая проверка - отрицательный радиус
  zone.clear();
  zone.push_back(zone1);
  correct(zone);

  area.clear();
  difference(poly, zone, area);
  correct(area);
  ASSERT_THROW(calculateCake(area, center, -1, widthExpansion),
               GeometryFailure);
  try
  {
    cake = calculateCake(area, center, -1, widthExpansion);
  }
  catch (const GeometryFailure& exception)
  {
    ASSERT_EQ(NegativeRadius, exception.getCode());
    ASSERT_EQ(geomLibTitle, exception.getLib());
    ASSERT_EQ(
      "[Core::Geometry] <NegativeRadius>: Построение окружности с отрицательным радиусом"
      , std::string(exception.what()));
  }
}
//==============================================================================
} //! Tests
} //! Geometry
//==============================================================================
