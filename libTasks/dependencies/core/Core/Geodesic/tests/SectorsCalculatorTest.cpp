#include <Geodesic/ComplicatedOperations/SectorsCalculator.hpp>
//! Geometry
#include <Geometry/Operations/AtPoints2D.hpp>
#include <Geometry/ComplicatedOperations/CakeProblem.hpp>
//! gtest
#include <gtest/gtest.h>
//==============================================================================
//! Тесты для проверки правильности работы функций тортика
//==============================================================================
namespace Geodesic
{
namespace Tests
{
//==============================================================================
using Geometry::SectorPoly;
//==============================================================================
class SectorsCalculatorTest : public ::testing::Test
{
//==============================================================================
protected: 
//==============================================================================
  void SetUp()
  {
    // Зона буквой П
    zone1.outer().emplace_back(Point2D(450, 300));
    zone1.outer().emplace_back(Point2D(450, 350));
    zone1.outer().emplace_back(Point2D(400, 350));
    zone1.outer().emplace_back(Point2D(400, 650));
    zone1.outer().emplace_back(Point2D(450, 650));
    zone1.outer().emplace_back(Point2D(450, 700));
    zone1.outer().emplace_back(Point2D(300, 700));
    zone1.outer().emplace_back(Point2D(300, 300));
    zone1.outer().emplace_back(Point2D(450, 300));

    // Зона с "ямкой"
    zone4.outer().emplace_back(Point2D(600, 500));
    zone4.outer().emplace_back(Point2D(800, 500));
    zone4.outer().emplace_back(Point2D(800, 700));
    zone4.outer().emplace_back(Point2D(650, 700));
    zone4.outer().emplace_back(Point2D(650, 650));
    zone4.outer().emplace_back(Point2D(700, 650));
    zone4.outer().emplace_back(Point2D(700, 550));
    zone4.outer().emplace_back(Point2D(650, 550));
    zone4.outer().emplace_back(Point2D(650, 600));
    zone4.outer().emplace_back(Point2D(600, 600));
    zone4.outer().emplace_back(Point2D(600, 500));

    // Лесенка близко к центру
    zone6.outer().emplace_back(Point2D(550, 350));
    zone6.outer().emplace_back(Point2D(600, 350));
    zone6.outer().emplace_back(Point2D(600, 300));
    zone6.outer().emplace_back(Point2D(650, 300));
    zone6.outer().emplace_back(Point2D(650, 250));
    zone6.outer().emplace_back(Point2D(750, 250));
    zone6.outer().emplace_back(Point2D(550, 445));
    zone6.outer().emplace_back(Point2D(550, 350));

    // Буква  Т, не попадающая полностью в окружность
    zone7.outer().emplace_back(Point2D(850, 550));
    zone7.outer().emplace_back(Point2D(850, 450));
    zone7.outer().emplace_back(Point2D(800, 450));
    zone7.outer().emplace_back(Point2D(800, 400));
    zone7.outer().emplace_back(Point2D(850, 400));
    zone7.outer().emplace_back(Point2D(850, 350));
    zone7.outer().emplace_back(Point2D(950, 350));
    zone7.outer().emplace_back(Point2D(950, 550));
    zone7.outer().emplace_back(Point2D(850, 550));

    // Задаём окружность
    center = Point2D(500, 500);
    radius = 400;

    projection.setProjectionPoint(GeoPoint2D(Radian(0.), Radian(0.)));
  }
//==============================================================================
  void TearDown()
  {

  }
//==============================================================================
  // Области
  Polygon2D zone1; // Зона буквой П
  Polygon2D zone4; // Зона с "ямкой"
  Polygon2D zone6; // Лесенка близко к центру
  Polygon2D zone7; // Буква  Т, не попадающая полностью в окружность

  Point2D                          center;         // точка центра
                                                   // окружности построения
  double                           radius;         // радиус окружности 
                                                   // построения
  Geodesic::OrthographicProjection projection;     // проекция
};
//==============================================================================
TEST_F( SectorsCalculatorTest, getSectorsTest)
{
  // Сформируем область и переведём её в геодезические координаты
  MultiPolygon2D zone;
  zone.emplace_back(zone1);
  zone.emplace_back(zone4);
  zone.emplace_back(zone6);
  zone.emplace_back(zone7);

  // Переведём полигон в геополигон
  GeoMultiPolygon2D geoZone;

  for (const auto& polygon : zone)
  {
    GeoPolygon2D geoPoly;
    for (const auto& point : polygon.outer())
      geoPoly.outer().emplace_back(projection.fromProjection(point));

    geoZone.emplace_back(geoPoly);
  }

  // Зададим разнообразные варианты азимутов
  Radian azimuth1(M_PI / 4);
  Radian azimuth2(5 * M_PI / 4);
  Radian azimuth3(3 * M_PI / 4);
  Radian azimuth4(7 * M_PI / 4);
  Radian azimuth5(0.);
  Radian azimuth6(M_PI);
  Radian azimuth7(M_PI / 2);
  Radian azimuth8(3 * M_PI / 2);

  double eps = 0.00005;

  std::vector<Geometry::Sector> sectors1
    = getSectors(geoZone, center, radius, projection, azimuth1);
  ASSERT_NEAR(sectors1[3].getBegAngle().get(), 2.601173, eps);
  ASSERT_NEAR(sectors1[3].getEndAngle().get(), 5.252808, eps);
  ASSERT_NEAR(sectors1[2].getBegAngle().get(), 6.141288, eps);
  ASSERT_NEAR(sectors1[2].getEndAngle().get(), 0.785398, eps);
  ASSERT_NEAR(sectors1[1].getBegAngle().get(), 1.570796, eps);
  ASSERT_NEAR(sectors1[1].getEndAngle().get(), 2.034443, eps);
  ASSERT_NEAR(sectors1[0].getBegAngle().get(), 0.643501, eps);
  ASSERT_NEAR(sectors1[0].getEndAngle().get(), 1.190289, eps);

  std::vector<Geometry::Sector> sectors2
    = getSectors(geoZone, center, radius, projection, azimuth2);
  ASSERT_NEAR(sectors2[3].getBegAngle().get(), 5.742765, eps);
  ASSERT_NEAR(sectors2[3].getEndAngle().get(), 2.111216, eps);
  ASSERT_NEAR(sectors2[2].getBegAngle().get(), 2.999695, eps);
  ASSERT_NEAR(sectors2[2].getEndAngle().get(), 3.926991, eps);
  ASSERT_NEAR(sectors2[1].getBegAngle().get(), 4.712389, eps);
  ASSERT_NEAR(sectors2[1].getEndAngle().get(), 5.176037, eps);
  ASSERT_NEAR(sectors2[0].getBegAngle().get(), 3.785094, eps);
  ASSERT_NEAR(sectors2[0].getEndAngle().get(), 4.331882, eps);

  std::vector<Geometry::Sector> sectors3
    = getSectors(geoZone, center, radius, projection, azimuth3);
  ASSERT_NEAR(sectors3[3].getBegAngle().get(), 1.030377, eps);
  ASSERT_NEAR(sectors3[3].getEndAngle().get(), 3.682012, eps);
  ASSERT_NEAR(sectors3[2].getBegAngle().get(), 4.570492, eps);
  ASSERT_NEAR(sectors3[2].getEndAngle().get(), 5.497787, eps);
  ASSERT_NEAR(sectors3[1].getBegAngle().get(), 6.283185, eps);
  ASSERT_NEAR(sectors3[1].getEndAngle().get(), 0.463648, eps);
  ASSERT_NEAR(sectors3[0].getBegAngle().get(), 5.35589, eps);
  ASSERT_NEAR(sectors3[0].getEndAngle().get(), 5.902679, eps);

  std::vector<Geometry::Sector> sectors4
    = getSectors(geoZone, center, radius, projection, azimuth4);
  ASSERT_NEAR(sectors4[3].getBegAngle().get(), 4.171969, eps);
  ASSERT_NEAR(sectors4[3].getEndAngle().get(), 0.540419, eps);
  ASSERT_NEAR(sectors4[2].getBegAngle().get(), 1.428899, eps);
  ASSERT_NEAR(sectors4[2].getEndAngle().get(), 2.356194, eps);
  ASSERT_NEAR(sectors4[1].getBegAngle().get(), 3.141593, eps);
  ASSERT_NEAR(sectors4[1].getEndAngle().get(), 3.60524, eps);
  ASSERT_NEAR(sectors4[0].getBegAngle().get(), 2.214297, eps);
  ASSERT_NEAR(sectors4[0].getEndAngle().get(), 2.761086, eps);

  std::vector<Geometry::Sector> sectors5
    = getSectors(geoZone, center, radius, projection, azimuth5);
  ASSERT_NEAR(sectors5[3].getBegAngle().get(), 3.386571, eps);
  ASSERT_NEAR(sectors5[3].getEndAngle().get(), 6.038207, eps);
  ASSERT_NEAR(sectors5[2].getBegAngle().get(), 0.643501, eps);
  ASSERT_NEAR(sectors5[2].getEndAngle().get(), 1.570796, eps);
  ASSERT_NEAR(sectors5[1].getBegAngle().get(), 2.356194, eps);
  ASSERT_NEAR(sectors5[1].getEndAngle().get(), 2.819842, eps);
  ASSERT_NEAR(sectors5[0].getBegAngle().get(), 1.428899, eps);
  ASSERT_NEAR(sectors5[0].getEndAngle().get(), 1.975688, eps);

  std::vector<Geometry::Sector> sectors6
    = getSectors(geoZone, center, radius, projection, azimuth6);
  ASSERT_NEAR(sectors6[3].getBegAngle().get(), 0.244978, eps);
  ASSERT_NEAR(sectors6[3].getEndAngle().get(), 2.896614, eps);
  ASSERT_NEAR(sectors6[2].getBegAngle().get(), 3.785094, eps);
  ASSERT_NEAR(sectors6[2].getEndAngle().get(), 4.712389, eps);
  ASSERT_NEAR(sectors6[1].getBegAngle().get(), 5.497787, eps);
  ASSERT_NEAR(sectors6[1].getEndAngle().get(), 5.961435, eps);
  ASSERT_NEAR(sectors6[0].getBegAngle().get(), 4.570492, eps);
  ASSERT_NEAR(sectors6[0].getEndAngle().get(), 5.117281, eps);

  std::vector<Geometry::Sector> sectors7
    = getSectors(geoZone, center, radius, projection, azimuth7);
  ASSERT_NEAR(sectors7[3].getBegAngle().get(), 1.815775, eps);
  ASSERT_NEAR(sectors7[3].getEndAngle().get(), 4.46741, eps);
  ASSERT_NEAR(sectors7[2].getBegAngle().get(), 5.35589, eps);
  ASSERT_NEAR(sectors7[2].getEndAngle().get(), 0., eps);
  ASSERT_NEAR(sectors7[1].getBegAngle().get(), 0.785398, eps);
  ASSERT_NEAR(sectors7[1].getEndAngle().get(), 1.249046, eps);
  ASSERT_NEAR(sectors7[0].getBegAngle().get(), 6.141288, eps);
  ASSERT_NEAR(sectors7[0].getEndAngle().get(), 0.404892, eps);

  std::vector<Geometry::Sector> sectors8
    = getSectors(geoZone, center, radius, projection, azimuth8);
  ASSERT_NEAR(sectors8[3].getBegAngle().get(), 4.957368, eps);
  ASSERT_NEAR(sectors8[3].getEndAngle().get(), 1.325818, eps);
  ASSERT_NEAR(sectors8[2].getBegAngle().get(), 2.214297, eps);
  ASSERT_NEAR(sectors8[2].getEndAngle().get(), 3.141593, eps);
  ASSERT_NEAR(sectors8[1].getBegAngle().get(), 3.926991, eps);
  ASSERT_NEAR(sectors8[1].getEndAngle().get(), 4.390638, eps);
  ASSERT_NEAR(sectors8[0].getBegAngle().get(), 2.999696, eps);
  ASSERT_NEAR(sectors8[0].getEndAngle().get(), 3.546484, eps);

  // Объединение секторов
  sectors5 = getSectors(geoZone, center, radius, projection, azimuth5, true);
  ASSERT_NEAR(sectors5[0].getBegAngle().get(), 0.643501, eps);
  ASSERT_NEAR(sectors5[0].getEndAngle().get(), 1.975688, eps);
  ASSERT_NEAR(sectors5[1].getBegAngle().get(), 2.356194, eps);
  ASSERT_NEAR(sectors5[1].getEndAngle().get(), 2.819842, eps);
  ASSERT_NEAR(sectors5[2].getBegAngle().get(), 3.386571, eps);
  ASSERT_NEAR(sectors5[2].getEndAngle().get(), 6.038207, eps);

  // Объединение секторов с пересечением нулевого угла
  sectors7 = getSectors(geoZone, center, radius, projection, azimuth7, true);
  ASSERT_NEAR(sectors7[0].getBegAngle().get(), 0.785398, eps);
  ASSERT_NEAR(sectors7[0].getEndAngle().get(), 1.249046, eps);
  ASSERT_NEAR(sectors7[1].getBegAngle().get(), 1.815775, eps);
  ASSERT_NEAR(sectors7[1].getEndAngle().get(), 4.46741, eps);
  ASSERT_NEAR(sectors7[2].getBegAngle().get(), 5.35589, eps);
  ASSERT_NEAR(sectors7[2].getEndAngle().get(), 0.404892, eps);

  // Проверка построения секторов при отсутствии полигонов
  std::vector<Geometry::Sector> sectors 
    = getSectors(GeoMultiPolygon2D(), center, radius, projection, azimuth7);
  ASSERT_TRUE(sectors.empty());
}
//==============================================================================
} //! Tests
} //! Geodesic
//==============================================================================
