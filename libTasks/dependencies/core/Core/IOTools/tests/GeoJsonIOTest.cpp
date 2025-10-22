#include <IOTools/GeoJsonIO/GeoJsonIO.hpp>

#include <OSTools/FindFiles.hpp>
#include <OSTools/DeleteFile.hpp>
// gtest
#include <gtest/gtest.h>
//==============================================================================
/*!
* \brief Тестирование чтения полигона из .geojson-файла.
*/
TEST(GeoJsonIOTest, ImportPolygonFromGeoJson)
{
  std::string polygonGeoJsonPath = "IOToolsTestRes/polygon.geojson";
  GeoPolygon2D polygon(importGeoPolygonFromGeoJson(polygonGeoJsonPath));

  //! Проверяем количество внутренних колец
  ASSERT_EQ(2, polygon.inners().size());
  //! Проверяем количество точек
  ASSERT_EQ(5, polygon.outer().size());
  ASSERT_EQ(5, polygon.inners()[0].size());
  ASSERT_EQ(7, polygon.inners()[1].size());

  //! Проверяем значения координат вершин
  ASSERT_EQ(GeoPoint2D(Degree{ 60 }, Degree{ 50 }), polygon.outer()[0]);
  ASSERT_EQ(GeoPoint2D(Degree{ 110 }, Degree{ 100 }), polygon.outer()[2]);
  ASSERT_EQ(GeoPoint2D(Degree{ 110 }, Degree{ 50 }), polygon.outer()[3]);
  ASSERT_EQ(GeoPoint2D(Degree{ 100 }, Degree{ 60 }), polygon.inners()[0][1]);
  ASSERT_EQ(GeoPoint2D(Degree{ 90 }, Degree{ 75 }), polygon.inners()[0][2]);
  ASSERT_EQ(GeoPoint2D(Degree{ 70 }, Degree{ 75 }), polygon.inners()[0][4]);
  ASSERT_EQ(GeoPoint2D(Degree{ 65 }, Degree{ 61 }), polygon.inners()[1][1]);
  ASSERT_EQ(GeoPoint2D(Degree{ 71 }, Degree{ 61 }), polygon.inners()[1][2]);
  ASSERT_EQ(GeoPoint2D(Degree{ 68 }, Degree{ 55 }), polygon.inners()[1][5]);

  //! Несуществующий файл.
  polygon = importGeoPolygonFromGeoJson("thisfilehavenottoexist.geojson");
  ASSERT_EQ(0, polygon.outer().size());

  //! Файл, не имеющий формат GeoJson.
  polygon = importGeoPolygonFromGeoJson("file.nogeojson");
  ASSERT_EQ(0, polygon.outer().size());

  //! Проделаем всё то же самое для плоскостной геометрии

  std::string polygonFlatGeoJsonPath = "IOToolsTestRes/polygonFlat.geojson";
  Polygon2D polygonFlat(importPolygonFromGeoJson(polygonFlatGeoJsonPath));

  //! Проверяем количество внутренних колец
  ASSERT_EQ(2, polygonFlat.inners().size());
  //! Проверяем количество точек
  ASSERT_EQ(5, polygonFlat.outer().size());
  ASSERT_EQ(5, polygonFlat.inners()[0].size());
  ASSERT_EQ(7, polygonFlat.inners()[1].size());

  //! Проверяем значения координат вершин
  ASSERT_EQ(Point2D(50, 60), polygonFlat.outer()[0]);
  ASSERT_EQ(Point2D(100, 110), polygonFlat.outer()[2]);
  ASSERT_EQ(Point2D(50, 110), polygonFlat.outer()[3]);
  ASSERT_EQ(Point2D(60, 100), polygonFlat.inners()[0][1]);
  ASSERT_EQ(Point2D(75, 90), polygonFlat.inners()[0][2]);
  ASSERT_EQ(Point2D(75, 70), polygonFlat.inners()[0][4]);
  ASSERT_EQ(Point2D(61, 65), polygonFlat.inners()[1][1]);
  ASSERT_EQ(Point2D(61, 71), polygonFlat.inners()[1][2]);
  ASSERT_EQ(Point2D(55, 68), polygonFlat.inners()[1][5]);

  //! Несуществующий файл.
  polygonFlat = importPolygonFromGeoJson("thisfilehavenottoexist.geojson");
  ASSERT_EQ(0, polygonFlat.outer().size());

  //! Файл, не имеющий формат GeoJson.
  polygonFlat = importPolygonFromGeoJson("file.nogeojson");
  ASSERT_EQ(0, polygonFlat.outer().size());
}
//==============================================================================
/*!
* \brief Тестирование чтения рамки из .geojson-файла.
*/
TEST(GeoJsonIOTest, ImportBoxFromGeoJson)
{
  std::string boxGeoJsonPath = "IOToolsTestRes/box.geojson";
  GeoBox2D box(importGeoBoxFromGeoJson(boxGeoJsonPath));

  ASSERT_EQ(GeoPoint2D(Degree{ 40 }, Degree{ 30 }), box.min_corner());
  ASSERT_EQ(GeoPoint2D(Degree{ 45 }, Degree{ 50 }), box.max_corner());

  //! Несуществующий файл.
  box = importGeoBoxFromGeoJson("thisfilehavenottoexist.geojson");
  ASSERT_EQ(box.min_corner(), box.max_corner());

  //! Файл, не имеющий формат GeoJson.
  box = importGeoBoxFromGeoJson("file.nogeojson");
  ASSERT_EQ(box.min_corner(), box.max_corner());

  //! Проделаем всё то же самое для плоскостной геометрии
  std::string boxFlatGeoJsonPath = "IOToolsTestRes/boxFlat.geojson";
  Box2D boxFlat(importBoxFromGeoJson(boxFlatGeoJsonPath));

  ASSERT_EQ(Point2D(30, 40), boxFlat.min_corner());
  ASSERT_EQ(Point2D(50, 45), boxFlat.max_corner());

  //! Несуществующий файл.
  boxFlat = importBoxFromGeoJson("thisfilehavenottoexist.geojson");
  ASSERT_EQ(boxFlat.min_corner(), boxFlat.max_corner());

  //! Файл, не имеющий формат GeoJson.
  boxFlat = importBoxFromGeoJson("file.nogeojson");
  ASSERT_EQ(boxFlat.min_corner(), boxFlat.max_corner());
}
//==============================================================================
/*!
 * \brief Тестирование чтения множества многоугольников из
*         .geojson-файла.
 */
TEST(GeoJsonIOTest, ImportMultiPolygonFromGeoJson)
{
  //! Чтение мультиполигона, записанного как набор полигонов
  //! Пример выходного файла от НИСО.
  GeoMultiPolygon2D multiPolygon(
    importGeoMultiPolygonFromGeoJson("IOToolsTestRes/coastline.geojson"));

  //! Проверяем количества вершин.
  ASSERT_EQ(6, multiPolygon.size());
  ASSERT_EQ(821, multiPolygon[0].outer().size());
  ASSERT_EQ(120, multiPolygon[0].inners().size());
  ASSERT_EQ(15, multiPolygon[1].outer().size());
  ASSERT_EQ(0, multiPolygon[1].inners().size());
  ASSERT_EQ(17, multiPolygon[2].outer().size());
  ASSERT_EQ(0, multiPolygon[2].inners().size());
  ASSERT_EQ(22, multiPolygon[3].outer().size());
  ASSERT_EQ(0, multiPolygon[3].inners().size());
  ASSERT_EQ(29, multiPolygon[4].outer().size());
  ASSERT_EQ(0, multiPolygon[4].inners().size());
  ASSERT_EQ(64, multiPolygon[5].outer().size());
  ASSERT_EQ(3, multiPolygon[5].inners().size());
  //! Проверяем значения четырёх случайно выбранных вершин.
  ASSERT_EQ(
    GeoPoint2D(Degree(46.732686017292792), Degree(37.629703384899443)),
    multiPolygon[2].outer()[8]);
  ASSERT_EQ(
    GeoPoint2D(Degree(46.69064096473106), Degree(37.720579345161838)),
    multiPolygon[5].outer()[39]);
  ASSERT_EQ(
    GeoPoint2D(Degree(46.911944933653331), Degree(37.328797634138922)),
    multiPolygon[1].outer()[6]);
  ASSERT_EQ(
    GeoPoint2D(Degree(47.079616519489342), Degree(37.674185477266946)),
    multiPolygon[4].outer()[0]);

  //! Несуществующий файл.
  multiPolygon = importGeoMultiPolygonFromGeoJson(
              "thisfilehavenottoexist.geojson");
  ASSERT_EQ(0, multiPolygon.size());

  //! Файл, не имеющий формат GeoJson.
  multiPolygon = importGeoMultiPolygonFromGeoJson("file.nogeojson");
  ASSERT_EQ(0, multiPolygon.size());

  //! Проделаем всё то же самое для плоскостной геометрии

  //! Пример выходного файла от НИСО.
  MultiPolygon2D multiPolygonFlat(
    importMultiPolygonFromGeoJson("IOToolsTestRes/coastlineFlat.geojson"));
  //! Проверяем количества вершин.
  ASSERT_EQ(6, multiPolygonFlat.size());
  ASSERT_EQ(821, multiPolygonFlat[0].outer().size());
  ASSERT_EQ(120, multiPolygonFlat[0].inners().size());
  ASSERT_EQ(15, multiPolygonFlat[1].outer().size());
  ASSERT_EQ(0, multiPolygonFlat[1].inners().size());
  ASSERT_EQ(17, multiPolygonFlat[2].outer().size());
  ASSERT_EQ(0, multiPolygonFlat[2].inners().size());
  ASSERT_EQ(22, multiPolygonFlat[3].outer().size());
  ASSERT_EQ(0, multiPolygonFlat[3].inners().size());
  ASSERT_EQ(29, multiPolygonFlat[4].outer().size());
  ASSERT_EQ(0, multiPolygonFlat[4].inners().size());
  ASSERT_EQ(64, multiPolygonFlat[5].outer().size());
  ASSERT_EQ(3, multiPolygonFlat[5].inners().size());
  //! Проверяем значения четырёх случайно выбранных вершин.
  ASSERT_EQ(
    Point2D(37.629703384899443, 46.732686017292792),
    multiPolygonFlat[2].outer()[8]);
  ASSERT_EQ(
    Point2D(37.720579345161838, 46.69064096473106),
    multiPolygonFlat[5].outer()[39]);
  ASSERT_EQ(
    Point2D(37.328797634138922, 46.911944933653331),
    multiPolygonFlat[1].outer()[6]);
  ASSERT_EQ(
    Point2D(37.674185477266946, 47.079616519489342),
    multiPolygonFlat[4].outer()[0]);

  //! Несуществующий файл.
  multiPolygonFlat = importMultiPolygonFromGeoJson(
    "thisfilehavenottoexist.geojson");
  ASSERT_EQ(0, multiPolygonFlat.size());

  //! Файл, не имеющий формат GeoJson.
  multiPolygonFlat = importMultiPolygonFromGeoJson("file.nogeojson");
  ASSERT_EQ(0, multiPolygonFlat.size());

  //! Чтение мультиполигона, записанного как MultiPolygon
  multiPolygon
    = importGeoMultiPolygonFromGeoJson("IOToolsTestRes/multipolygon.geojson");

  //! Проверяем количества вершин.
  ASSERT_EQ(3, multiPolygon.size());
  ASSERT_EQ(5, multiPolygon[0].outer().size());
  ASSERT_EQ(0, multiPolygon[0].inners().size());
  ASSERT_EQ(4, multiPolygon[1].outer().size());
  ASSERT_EQ(0, multiPolygon[1].inners().size());
  ASSERT_EQ(5, multiPolygon[2].outer().size());
  ASSERT_EQ(0, multiPolygon[2].inners().size());
  //! Проверяем значения трёх случайно выбранных вершин.
  ASSERT_EQ(
    GeoPoint2D(Degree(69.0), Degree(21.9)),
    multiPolygon[0].outer()[2]);
  ASSERT_EQ(
    GeoPoint2D(Degree(69.2), Degree(23.1)),
    multiPolygon[1].outer()[0]);
  ASSERT_EQ(
    GeoPoint2D(Degree(67.6), Degree(21.5)),
    multiPolygon[2].outer()[4]);
}
//==============================================================================
/*!
 * \brief Тестирование чтения множества точек из .geojson-файла.
 */
TEST(GeoJsonIOTest, ImportMultiPointFromGeoJson)
{
  std::string multiPointGeoJsonPath = "IOToolsTestRes/multipoint.geojson";
  GeoMultiPoint2D multiPoint(
    importGeoMultiPointFromGeoJson(multiPointGeoJsonPath));
  //! Проверяем количество точек
  ASSERT_EQ(5, multiPoint.size());
  //! Проверяем координаты точек
  ASSERT_EQ(GeoPoint2D(Degree{61.456},Degree{25.156}), multiPoint[0]);
  ASSERT_EQ(GeoPoint2D(Degree{61.256},Degree{25.909}), multiPoint[2]);
  ASSERT_EQ(GeoPoint2D(Degree{60.419},Degree{26.212}), multiPoint[3]);
  ASSERT_EQ(GeoPoint2D(Degree{61.777},Degree{25.555}), multiPoint[4]);

  //! Несуществующий файл.
  multiPoint = importGeoMultiPointFromGeoJson(
    "thisfilehavenottoexist.geojson");
  ASSERT_EQ(0, multiPoint.size());

  //! Файл, не имеющий формат GeoJson.
  multiPoint = importGeoMultiPointFromGeoJson("file.nogeojson");
  ASSERT_EQ(0, multiPoint.size());

  //! Проделаем всё то же самое для плоскостной геометрии

  std::string multiPointFlatGeoJsonPath
    = "IOToolsTestRes/multipointFlat.geojson";
  MultiPoint2D multiPointFlat(
    importMultiPointFromGeoJson(multiPointFlatGeoJsonPath));
  //! Проверяем количество точек
  ASSERT_EQ(5, multiPointFlat.size());
  //! Проверяем координаты точек
  ASSERT_EQ(Point2D(25.156, 61.456), multiPointFlat[0]);
  ASSERT_EQ(Point2D(25.909, 61.256), multiPointFlat[2]);
  ASSERT_EQ(Point2D(26.212, 60.419), multiPointFlat[3]);
  ASSERT_EQ(Point2D(25.555, 61.777), multiPointFlat[4]);
  
  //! Несуществующий файл.
  multiPointFlat = importMultiPointFromGeoJson(
    "thisfilehavenottoexist.geojson");
  ASSERT_EQ(0, multiPointFlat.size());

  //! Файл, не имеющий формат GeoJson.
  multiPointFlat = importMultiPointFromGeoJson("file.nogeojson");
  ASSERT_EQ(0, multiPointFlat.size());
}
//==============================================================================
/*!
* \brief Тестирование чтения ломаной линии из .geojson-файла.
*/
TEST(GeoJsonIOTest, ImportLinestringFromGeoJson)
{
  std::string linestringGeoJsonPath = "IOToolsTestRes/linestring.geojson";
  GeoLinestring2D linestring(
    importGeoLinestringFromGeoJson(linestringGeoJsonPath));
  //! Проверяем количество точек
  ASSERT_EQ(3, linestring.size());
  //! Проверяем координаты точек
  ASSERT_EQ(GeoPoint2D(Degree{ 1. }, Degree{ -2. }), linestring[0]);
  ASSERT_EQ(GeoPoint2D(Degree{ 0. }, Degree{ -1. }), linestring[1]);
  ASSERT_EQ(GeoPoint2D(Degree{ 2. }, Degree{ 0. }), linestring[2]);

  //! Несуществующий файл.
  linestring = importGeoLinestringFromGeoJson(
    "thisfilehavenottoexist.geojson");
  ASSERT_EQ(0, linestring.size());

  //! Файл, не имеющий формат GeoJson.
  linestring = importGeoLinestringFromGeoJson("file.nogeojson");
  ASSERT_EQ(0, linestring.size());

  //! Проделаем всё то же самое для плоскостной геометрии

  std::string linestringFlatGeoJsonPath
    = "IOToolsTestRes/linestringFlat.geojson";
  Linestring2D linestringFlat(
    importLinestringFromGeoJson(linestringFlatGeoJsonPath));
  //! Проверяем количество точек
  ASSERT_EQ(3, linestringFlat.size());
  //! Проверяем координаты точек
  ASSERT_EQ(Point2D(-2., 1.), linestringFlat[0]);
  ASSERT_EQ(Point2D(-1., 0.), linestringFlat[1]);
  ASSERT_EQ(Point2D(0., 2.), linestringFlat[2]);

  //! Несуществующий файл.
  linestringFlat = importLinestringFromGeoJson(
    "thisfilehavenottoexist.geojson");
  ASSERT_EQ(0, linestringFlat.size());

  //! Файл, не имеющий формат GeoJson.
  linestringFlat = importLinestringFromGeoJson("file.nogeojson");
  ASSERT_EQ(0, linestringFlat.size());
}
//==============================================================================
/*!
 * \brief Тестирование чтения набора ломаных линий из .geojson-файла.
 */
TEST(GeoJsonIOTest, ImportMultiLinestringFromGeoJson)
{
  //! Чтение мультилинии, записанной как набор ломаных линий

  std::string linestringGeoJsonPath
    = "IOToolsTestRes/threeLinestrings.geojson";
  GeoMultiLinestring2D linestring(
    importGeoMultiLinestringFromGeoJson(linestringGeoJsonPath));

  //! Проверяем количество ломаных
  ASSERT_EQ(3, linestring.size());

  //! Проверяем количество точек
  ASSERT_EQ(2, linestring.at(0).size());
  ASSERT_EQ(4, linestring.at(1).size());
  ASSERT_EQ(3, linestring.at(2).size());

  //! Проверяем координаты первых точек
  ASSERT_EQ(GeoPoint2D(Degree{ 0.1 }, Degree{ -4.6 }), linestring[0][0]);
  ASSERT_EQ(GeoPoint2D(Degree{ 0.3 }, Degree{ -5.3 }), linestring[1][0]);
  ASSERT_EQ(GeoPoint2D(Degree{ 0. }, Degree{ -5.1 }), linestring[2][0]);

  //! Несуществующий файл.
  linestring = importGeoMultiLinestringFromGeoJson(
    "thisfilehavenottoexist.geojson");
  ASSERT_EQ(0, linestring.size());

  //! Файл, не имеющий формат GeoJson.
  linestring = importGeoMultiLinestringFromGeoJson("file.nogeojson");
  ASSERT_EQ(0, linestring.size());

  //! Проделаем всё то же самое для плоскостной геометрии

  std::string linestringFlatGeoJsonPath
    = "IOToolsTestRes/threeLinestringsFlat.geojson";
  MultiLinestring2D linestringFlat(
    importMultiLinestringFromGeoJson(linestringFlatGeoJsonPath));
  //! Проверяем количество ломаных
  ASSERT_EQ(3, linestringFlat.size());

  //! Проверяем количество точек
  ASSERT_EQ(2, linestringFlat.at(0).size());
  ASSERT_EQ(4, linestringFlat.at(1).size());
  ASSERT_EQ(3, linestringFlat.at(2).size());

  //! Проверяем координаты точек
  ASSERT_EQ(Point2D(-4.6, 0.1), linestringFlat[0][0]);
  ASSERT_EQ(Point2D(-5.3, 0.3), linestringFlat[1][0]);
  ASSERT_EQ(Point2D(-5.1, 0.), linestringFlat[2][0]);

  //! Несуществующий файл.
  linestringFlat = importMultiLinestringFromGeoJson(
    "thisfilehavenottoexist.geojson");
  ASSERT_EQ(0, linestringFlat.size());

  //! Файл, не имеющий формат GeoJson.
  linestringFlat = importMultiLinestringFromGeoJson("file.nogeojson");
  ASSERT_EQ(0, linestringFlat.size());

  //! Чтение мультилинии, записанной как MultiLineString

  linestringGeoJsonPath = "IOToolsTestRes/multilinestring.geojson";
  linestring =
    importGeoMultiLinestringFromGeoJson(linestringGeoJsonPath);

  //! Проверяем количество ломаных
  ASSERT_EQ(3, linestring.size());

  //! Проверяем количество точек
  ASSERT_EQ(2, linestring.at(0).size());
  ASSERT_EQ(4, linestring.at(1).size());
  ASSERT_EQ(3, linestring.at(2).size());

  //! Проверяем координаты первых точек
  ASSERT_EQ(GeoPoint2D(Degree{ 0.1 }, Degree{ -4.6 }), linestring[0][0]);
  ASSERT_EQ(GeoPoint2D(Degree{ 0.3 }, Degree{ -5.3 }), linestring[1][0]);
  ASSERT_EQ(GeoPoint2D(Degree{ 0. }, Degree{ -5.1 }), linestring[2][0]);
}
//==============================================================================
/*!
 * \brief Тестирование чтения множества
 *        многоугольников c высотами из .geojson-файла
 */
TEST(GeoJsonIOTest, ImportPolygonsWithHeightFromGeoJson)
{
    //! Копия содержимого файла coastline.geojson,
    //! но с добавленными высотами
    auto polygonsWithHeight = importGeoPolygonsWithHeightFromGeoJson(
                              "IOToolsTestRes/withheight.geojson");
    //! Проверка количества полигонов
    ASSERT_EQ(6, polygonsWithHeight.size());
    //! Проверка количества вершин в полигонах
    ASSERT_EQ(821, polygonsWithHeight[0].first.outer().size());
    ASSERT_EQ(120, polygonsWithHeight[0].first.inners().size());
    ASSERT_EQ(15, polygonsWithHeight[1].first.outer().size());
    ASSERT_EQ(0, polygonsWithHeight[1].first.inners().size());
    ASSERT_EQ(17, polygonsWithHeight[2].first.outer().size());
    ASSERT_EQ(0, polygonsWithHeight[2].first.inners().size());
    ASSERT_EQ(22, polygonsWithHeight[3].first.outer().size());
    ASSERT_EQ(0, polygonsWithHeight[3].first.inners().size());
    ASSERT_EQ(29, polygonsWithHeight[4].first.outer().size());
    ASSERT_EQ(0, polygonsWithHeight[4].first.inners().size());
    ASSERT_EQ(64, polygonsWithHeight[5].first.outer().size());
    ASSERT_EQ(3, polygonsWithHeight[5].first.inners().size());
    //! Проверяем значения четырёх случайно выбранных вершин.
    ASSERT_EQ(
      GeoPoint2D(Degree(46.732686017292792), Degree(37.629703384899443)),
      polygonsWithHeight[2].first.outer()[8]);
    ASSERT_EQ(
      GeoPoint2D(Degree(46.69064096473106), Degree(37.720579345161838)),
      polygonsWithHeight[5].first.outer()[39]);
    ASSERT_EQ(
      GeoPoint2D(Degree(46.911944933653331), Degree(37.328797634138922)),
      polygonsWithHeight[1].first.outer()[6]);
    ASSERT_EQ(
      GeoPoint2D(Degree(47.079616519489342), Degree(37.674185477266946)),
      polygonsWithHeight[4].first.outer()[0]);
    //! Проверяем значения высот полигонов
    ASSERT_EQ(1.0, polygonsWithHeight[0].second);
    ASSERT_EQ(256.0, polygonsWithHeight[2].second);
    ASSERT_EQ(0.0, polygonsWithHeight[3].second);
    ASSERT_EQ(9999.0, polygonsWithHeight[4].second);

    //! Несуществующий файл.
    polygonsWithHeight = importGeoPolygonsWithHeightFromGeoJson(
                         "thisfilehavenottoexist.geojson");
    ASSERT_EQ(0, polygonsWithHeight.size());

    //! Файл, не имеющий формат GeoJson
    polygonsWithHeight = importGeoPolygonsWithHeightFromGeoJson(
                         "file.nogeojson");
    ASSERT_EQ(0, polygonsWithHeight.size());

    //! Проделаем всё то же самое для плоскостной геометрии

    //! Копия содержимого файла coastline.geojson,
    //! но с добавленными высотами
    auto polygonsWithHeightFlat = importPolygonsWithHeightFromGeoJson(
      "IOToolsTestRes/withheight.geojson");
    //! Проверка количества полигонов
    ASSERT_EQ(6, polygonsWithHeightFlat.size());
    //! Проверка количества вершин в полигонах
    ASSERT_EQ(821, polygonsWithHeightFlat[0].first.outer().size());
    ASSERT_EQ(120, polygonsWithHeightFlat[0].first.inners().size());
    ASSERT_EQ(15, polygonsWithHeightFlat[1].first.outer().size());
    ASSERT_EQ(0, polygonsWithHeightFlat[1].first.inners().size());
    ASSERT_EQ(17, polygonsWithHeightFlat[2].first.outer().size());
    ASSERT_EQ(0, polygonsWithHeightFlat[2].first.inners().size());
    ASSERT_EQ(22, polygonsWithHeightFlat[3].first.outer().size());
    ASSERT_EQ(0, polygonsWithHeightFlat[3].first.inners().size());
    ASSERT_EQ(29, polygonsWithHeightFlat[4].first.outer().size());
    ASSERT_EQ(0, polygonsWithHeightFlat[4].first.inners().size());
    ASSERT_EQ(64, polygonsWithHeightFlat[5].first.outer().size());
    ASSERT_EQ(3, polygonsWithHeightFlat[5].first.inners().size());
    //! Проверяем значения четырёх случайно выбранных вершин.
    ASSERT_EQ(
      Point2D(37.629703384899443, 46.732686017292792),
      polygonsWithHeightFlat[2].first.outer()[8]);
    ASSERT_EQ(
      Point2D(37.720579345161838, 46.69064096473106),
      polygonsWithHeightFlat[5].first.outer()[39]);
    ASSERT_EQ(
      Point2D(37.328797634138922, 46.911944933653331),
      polygonsWithHeightFlat[1].first.outer()[6]);
    ASSERT_EQ(
      Point2D(37.674185477266946, 47.079616519489342),
      polygonsWithHeightFlat[4].first.outer()[0]);
    //! Проверяем значения высот полигонов
    ASSERT_EQ(1.0, polygonsWithHeightFlat[0].second);
    ASSERT_EQ(256.0, polygonsWithHeightFlat[2].second);
    ASSERT_EQ(0.0, polygonsWithHeightFlat[3].second);
    ASSERT_EQ(9999.0, polygonsWithHeightFlat[4].second);

    //! Несуществующий файл.
    polygonsWithHeightFlat = importPolygonsWithHeightFromGeoJson(
      "thisfilehavenottoexist.geojson");
    ASSERT_EQ(0, polygonsWithHeight.size());

    //! Файл, не имеющий формат GeoJson
    polygonsWithHeightFlat = importPolygonsWithHeightFromGeoJson(
      "file.nogeojson");
    ASSERT_EQ(0, polygonsWithHeightFlat.size());
}
//==============================================================================
/*!
* \brief Тестирование записи множества точек в .geojson-файл.
*/
TEST(GeoJsonIOTest, ExportMultiPointToGeoJson)
{
  //!< Исходное множество точек
  GeoMultiPoint2D multiPoint =
  {
    GeoPoint2D{ Degree{ 66 }, Degree{ 21 } },
    GeoPoint2D{ Degree{ 67 }, Degree{ 20 } },
    GeoPoint2D{ Degree{ 68 }, Degree{ 19 } },
    GeoPoint2D{ Degree{ 69 }, Degree{ 18 } }
  };

  //! Записываем в .geojson-файл
  const std::string path = "IOToolsTestRes/multipoint_test.geojson";
  ASSERT_TRUE(exportToGeoJson(path, multiPoint));

  //!< Набор точек, извлеченный из полученного файла
  GeoMultiPoint2D control = importGeoMultiPointFromGeoJson(path);

  //! Проверяем число точек
  ASSERT_EQ(multiPoint.size(), control.size());

  //! Проверяем соответствие точек исходным
  for (std::size_t i = 0; i < control.size(); ++i)
    ASSERT_EQ(control[i], multiPoint[i]);

  // Проверим запись пустого множества точек
  // На всякий случай убедимся, что файл не создан ранее
  OS::deleteFile("IOToolsTestRes/empty.geojson");

  // Если не хотим записывать пустое множество точек,
  // оно не запишется
  ASSERT_TRUE(exportToGeoJson("IOToolsTestRes/empty.geojson",
    GeoMultiPoint2D(), false));

  ASSERT_FALSE(OS::isFileExists("IOToolsTestRes/empty.geojson"));

  // Но можно и записывать (поведение по-умолчанию)
  ASSERT_TRUE(exportToGeoJson("IOToolsTestRes/empty.geojson",
    GeoMultiPoint2D()));

  ASSERT_TRUE(OS::isFileExists("IOToolsTestRes/empty.geojson"));

  control = importGeoMultiPointFromGeoJson("IOToolsTestRes/empty.geojson");

  ASSERT_EQ(control.size(), 0);

  // Почистим за собой файл
  OS::deleteFile("IOToolsTestRes/empty.geojson");

  // Проделаем всё то же самое для плоскостной геометрии

  //!< Исходное множество точек
  MultiPoint2D multiPointFlat =
  {
    Point2D(66, 21), Point2D(67, 20),
    Point2D(68, 19), Point2D(69, 18)
  };

  //! Записываем в .geojson-файл
  const std::string pathFlat = "IOToolsTestRes/multipointFlat_test.geojson";
  ASSERT_TRUE(exportToGeoJson(pathFlat, multiPointFlat));

  //!< Набор точек, извлеченный из полученного файла
  MultiPoint2D controlFlat = importMultiPointFromGeoJson(pathFlat);

  //! Проверяем число точек
  ASSERT_EQ(multiPointFlat.size(), controlFlat.size());

  //! Проверяем соответствие точек исходным
  for (std::size_t i = 0; i < controlFlat.size(); ++i)
    ASSERT_EQ(controlFlat[i], multiPointFlat[i]);

  // Проверим запись пустого множества точек
  // На всякий случай убедимся, что файл не создан ранее
  OS::deleteFile("IOToolsTestRes/emptyFlat.geojson");

  // Если не хотим записывать пустое множество точек,
  // оно не запишется
  ASSERT_TRUE(exportToGeoJson("IOToolsTestRes/emptyFlat.geojson",
    MultiPoint2D(), false));

  ASSERT_FALSE(OS::isFileExists("IOToolsTestRes/emptyFlat.geojson"));

  // Но можно и записывать (поведение по-умолчанию)
  ASSERT_TRUE(exportToGeoJson("IOToolsTestRes/emptyFlat.geojson",
    MultiPoint2D()));

  ASSERT_TRUE(OS::isFileExists("IOToolsTestRes/emptyFlat.geojson"));

  controlFlat
    = importMultiPointFromGeoJson("IOToolsTestRes/emptyFlat.geojson");

  ASSERT_EQ(controlFlat.size(), 0);

  // Почистим за собой файл
  OS::deleteFile("IOToolsTestRes/emptyFlat.geojson");
}
//==============================================================================
/*!
* \brief Тестирование записи ломаной линии в .geojson-файл.
*/
TEST(GeoJsonIOTest, ExportLinestringToGeoJson)
{
  //!< Исходное множество точек
  GeoLinestring2D linestring =
  {
    GeoPoint2D{ Degree{ 66 }, Degree{ 21 } },
    GeoPoint2D{ Degree{ 67 }, Degree{ 20 } },
    GeoPoint2D{ Degree{ 68 }, Degree{ 19 } },
    GeoPoint2D{ Degree{ 69 }, Degree{ 18 } }
  };

  //! Записываем в .geojson-файл
  const std::string path = "IOToolsTestRes/linestring_test.geojson";
  ASSERT_TRUE(exportToGeoJson(path, linestring));

  //!< Набор точек, извлеченный из полученного файла
  GeoLinestring2D control = importGeoLinestringFromGeoJson(path);

  //! Проверяем число точек
  ASSERT_EQ(linestring.size(), control.size());

  //! Проверяем соответствие точек исходным
  for (std::size_t i = 0; i < control.size(); ++i)
    ASSERT_EQ(control[i], linestring[i]);

  // Проверим запись пустого множества точек
  // На всякий случай убедимся, что файл не создан ранее
  OS::deleteFile("IOToolsTestRes/empty.geojson");

  // Если не хотим записывать пустое множество точек,
  // оно не запишется
  ASSERT_TRUE(exportToGeoJson("IOToolsTestRes/empty.geojson",
    GeoLinestring2D(), false));

  ASSERT_FALSE(OS::isFileExists("IOToolsTestRes/empty.geojson"));

  // Но можно и записывать (поведение по-умолчанию)
  ASSERT_TRUE(exportToGeoJson("IOToolsTestRes/empty.geojson",
    GeoLinestring2D()));

  ASSERT_TRUE(OS::isFileExists("IOToolsTestRes/empty.geojson"));

  control = importGeoLinestringFromGeoJson("IOToolsTestRes/empty.geojson");

  ASSERT_EQ(control.size(), 0);

  // Почистим за собой файл
  OS::deleteFile("IOToolsTestRes/empty.geojson");

  // Проделаем всё то же самое для плоскостной геометрии

  //!< Исходное множество точек
  Linestring2D linestringFlat =
  {
    Point2D(66, 21), Point2D(67, 20),
    Point2D(68, 19), Point2D(69, 18)
  };

  //! Записываем в .geojson-файл
  const std::string pathFlat = "IOToolsTestRes/linestringFlat_test.geojson";
  ASSERT_TRUE(exportToGeoJson(pathFlat, linestringFlat));

  //!< Набор точек, извлеченный из полученного файла
  Linestring2D controlFlat = importLinestringFromGeoJson(pathFlat);

  //! Проверяем число точек
  ASSERT_EQ(linestringFlat.size(), controlFlat.size());

  //! Проверяем соответствие точек исходным
  for (std::size_t i = 0; i < controlFlat.size(); ++i)
    ASSERT_EQ(controlFlat[i], linestringFlat[i]);

  // Проверим запись пустого множества точек
  // На всякий случай убедимся, что файл не создан ранее
  OS::deleteFile("IOToolsTestRes/emptyFlat.geojson");

  // Если не хотим записывать пустое множество точек,
  // оно не запишется
  ASSERT_TRUE(exportToGeoJson("IOToolsTestRes/emptyFlat.geojson",
    Linestring2D(), false));

  ASSERT_FALSE(OS::isFileExists("IOToolsTestRes/emptyFlat.geojson"));

  // Но можно и записывать (поведение по-умолчанию)
  ASSERT_TRUE(exportToGeoJson("IOToolsTestRes/emptyFlat.geojson",
    Linestring2D()));

  ASSERT_TRUE(OS::isFileExists("IOToolsTestRes/emptyFlat.geojson"));

  controlFlat
    = importLinestringFromGeoJson("IOToolsTestRes/emptyFlat.geojson");

  ASSERT_EQ(controlFlat.size(), 0);

  // Почистим за собой файл
  OS::deleteFile("IOToolsTestRes/emptyFlat.geojson");
}
//==============================================================================
/*!
* \brief Тестирование записи набора ломаных линий в .geojson-файл.
*/
TEST(GeoJsonIOTest, ExportMultiLinestringToGeoJson)
{
  //!< Исходный набор ломаных линий
  GeoMultiLinestring2D multilinestring =
  { {
      GeoPoint2D{ Degree{ 66 }, Degree{ 21 } },
      GeoPoint2D{ Degree{ 67 }, Degree{ 20 } },
      GeoPoint2D{ Degree{ 68 }, Degree{ 19 } },
      GeoPoint2D{ Degree{ 69 }, Degree{ 18 } }
    },
    {
      GeoPoint2D{ Degree{ 68 }, Degree{ 21 } },
      GeoPoint2D{ Degree{ 69 }, Degree{ 20 } },
      GeoPoint2D{ Degree{ 70 }, Degree{ 19 } },
      GeoPoint2D{ Degree{ 71 }, Degree{ 18 } }
    } };

  //! Записываем в .geojson-файл
  const std::string path = "IOToolsTestRes/multilinestring_test.geojson";
  ASSERT_TRUE(exportToGeoJson(path, multilinestring));

  //!< Набор ломаных линий, извлеченный из полученного файла
  GeoMultiLinestring2D control = importGeoMultiLinestringFromGeoJson(path);

  //! Проверяем количество ломаных
  ASSERT_EQ(multilinestring.size(), control.size());

  //! Проверяем число точек
  ASSERT_EQ(multilinestring[0].size(), control[0].size());
  ASSERT_EQ(multilinestring[1].size(), control[1].size());

  //! Проверяем соответствие точек исходным
  for (std::size_t i = 0; i < control.size(); ++i)
    for (std::size_t j = 0; j < control[i].size(); ++j)
      ASSERT_EQ(control[i][j], multilinestring[i][j]);

  // Проверим запись пустого множества ломаных линий
  // На всякий случай убедимся, что файл не создан ранее
  OS::deleteFile("IOToolsTestRes/empty.geojson");

  // Если не хотим записывать пустое множество ломаных линий,
  // оно не запишется
  ASSERT_TRUE(exportToGeoJson("IOToolsTestRes/empty.geojson",
                              GeoMultiLinestring2D(), false));

  ASSERT_FALSE(OS::isFileExists("IOToolsTestRes/empty.geojson"));

  // Но можно и записывать (поведение по-умолчанию)
  ASSERT_TRUE(exportToGeoJson("IOToolsTestRes/empty.geojson",
                              GeoMultiLinestring2D()));

  ASSERT_TRUE(OS::isFileExists("IOToolsTestRes/empty.geojson"));

  control = importGeoMultiLinestringFromGeoJson(
    "IOToolsTestRes/empty.geojson");

  ASSERT_EQ(control.size(), 0);

  // Почистим за собой файл
  OS::deleteFile("IOToolsTestRes/empty.geojson");

  // Проделаем всё то же самое для плоскостной геометрии

  //!< Исходный набор ломаных линий
  MultiLinestring2D multilinestringFlat =
  { {
      Point2D(66, 21), Point2D(67, 20),
      Point2D(68, 19), Point2D(69, 18)
    },
    {
      Point2D(68, 21), Point2D(69, 20),
      Point2D(70, 19), Point2D(71, 18)
    } };

  //! Записываем в .geojson-файл
  const std::string pathFlat
    = "IOToolsTestRes/multilinestringFlat_test.geojson";
  ASSERT_TRUE(exportToGeoJson(pathFlat, multilinestringFlat));

  //!< Набор ломаных линий, извлеченный из полученного файла
  MultiLinestring2D controlFlat = importMultiLinestringFromGeoJson(pathFlat);

  //! Проверяем количество ломаных
  ASSERT_EQ(multilinestringFlat.size(), controlFlat.size());

  //! Проверяем число точек
  ASSERT_EQ(multilinestringFlat[0].size(), controlFlat[0].size());
  ASSERT_EQ(multilinestringFlat[1].size(), controlFlat[1].size());

  //! Проверяем соответствие точек исходным
  for (std::size_t i = 0; i < controlFlat.size(); ++i)
    for (std::size_t j = 0; j < controlFlat[i].size(); ++j)
      ASSERT_EQ(controlFlat[i][j], multilinestringFlat[i][j]);

  // Проверим запись пустого множества ломаных линий
  // На всякий случай убедимся, что файл не создан ранее
  OS::deleteFile("IOToolsTestRes/emptyFlat.geojson");

  // Если не хотим записывать пустое множество ломаных линий,
  // оно не запишется
  ASSERT_TRUE(exportToGeoJson("IOToolsTestRes/emptyFlat.geojson",
                              MultiLinestring2D(), false));

  ASSERT_FALSE(OS::isFileExists("IOToolsTestRes/emptyFlat.geojson"));

  // Но можно и записывать (поведение по-умолчанию)
  ASSERT_TRUE(exportToGeoJson("IOToolsTestRes/emptyFlat.geojson",
                              MultiLinestring2D()));

  ASSERT_TRUE(OS::isFileExists("IOToolsTestRes/emptyFlat.geojson"));

  controlFlat
    = importMultiLinestringFromGeoJson("IOToolsTestRes/emptyFlat.geojson");

  ASSERT_EQ(controlFlat.size(), 0);

  // Почистим за собой файл
  OS::deleteFile("IOToolsTestRes/emptyFlat.geojson");
}
//=============================================================================
/*!
* \brief Тестирование записи полигона в .geojson-файл
*/
TEST(GeoJsonIOTest, ExportPolygonToGeoJson)
{
  //!< Исходный полигон
  GeoPolygon2D polygon;

  //! Формируем полигон без внутренних колец на основе рамки
  polygon = GeoBox2D(GeoPoint2D(Degree(0), Degree(5)),
    GeoPoint2D(Degree(100), Degree(90))).toPolygon();

  //! Добавляем внутреннее кольцо
  polygon.inners().resize(1);
  polygon.inners()[0] =
  {
    GeoPoint2D(Degree(10), Degree(20)),
    GeoPoint2D(Degree(30), Degree(20)),
    GeoPoint2D(Degree(30), Degree(45)),
    GeoPoint2D(Degree(10), Degree(45))
  };

  //! Записываем в .geojson-файл
  const std::string path = "IOToolsTestRes/polygon_test.geojson";
  ASSERT_TRUE(exportToGeoJson(path, polygon));

  //!< Полигон, извлеченный из полученного файла
  GeoPolygon2D control = importGeoPolygonFromGeoJson(path);

  //! Проверяем число внутренних колец
  ASSERT_EQ(1, control.inners().size());
  //! Проверяем соответствие внешних колец
  ASSERT_EQ(polygon.outer(), control.outer());
  //! Проверяем соответствие внутренних колец
  ASSERT_EQ(polygon.inners()[0], control.inners()[0]);

  // Проверим запись пустого полигона
  // На всякий случай убедимся, что файл не создан ранее
  OS::deleteFile("IOToolsTestRes/empty.geojson");

  // Если не хотим записывать пустой полигон, он не запишется
  ASSERT_TRUE(exportToGeoJson("IOToolsTestRes/empty.geojson",
    GeoPolygon2D(), false));

  ASSERT_FALSE(OS::isFileExists("IOToolsTestRes/empty.geojson"));

  // Но можно и записывать (поведение по-умолчанию)
  ASSERT_TRUE(exportToGeoJson("IOToolsTestRes/empty.geojson",
    GeoPolygon2D()));

  ASSERT_TRUE(OS::isFileExists("IOToolsTestRes/empty.geojson"));

  control = importGeoPolygonFromGeoJson("IOToolsTestRes/empty.geojson");

  ASSERT_EQ(control.outer().size(), 0);

  // Почистим за собой файл
  OS::deleteFile("IOToolsTestRes/empty.geojson");

  // Проделаем всё то же самое для плоскостной геометрии

  //!< Исходный полигон
  Polygon2D polygonFlat;

  //! Формируем полигон без внутренних колец на основе рамки
  polygonFlat = Box2D(Point2D(0, 5), Point2D(100, 90)).toPolygon();

  //! Добавляем внутреннее кольцо
  polygonFlat.inners().resize(1);
  polygonFlat.inners()[0] =
  {
    Point2D(10, 20), Point2D(30, 20),
    Point2D(30, 45), Point2D(10, 45)
  };

  //! Записываем в .geojson-файл
  const std::string pathFlat = "IOToolsTestRes/polygonFlat_test.geojson";
  ASSERT_TRUE(exportToGeoJson(pathFlat, polygonFlat));

  //!< Полигон, извлеченный из полученного файла
  Polygon2D controlFlat = importPolygonFromGeoJson(pathFlat);

  //! Проверяем число внутренних колец
  ASSERT_EQ(1, controlFlat.inners().size());
  //! Проверяем соответствие внешних колец
  ASSERT_EQ(polygonFlat.outer(), controlFlat.outer());
  //! Проверяем соответствие внутренних колец
  ASSERT_EQ(polygonFlat.inners()[0], controlFlat.inners()[0]);

  // Проверим запись пустого полигона
  // На всякий случай убедимся, что файл не создан ранее
  OS::deleteFile("IOToolsTestRes/emptyFlat.geojson");

  // Если не хотим записывать пустой полигон, он не запишется
  ASSERT_TRUE(exportToGeoJson("IOToolsTestRes/emptyFlat.geojson",
    Polygon2D(), false));

  ASSERT_FALSE(OS::isFileExists("IOToolsTestRes/emptyFlat.geojson"));

  // Но можно и записывать (поведение по-умолчанию)
  ASSERT_TRUE(exportToGeoJson("IOToolsTestRes/emptyFlat.geojson",
    Polygon2D()));

  ASSERT_TRUE(OS::isFileExists("IOToolsTestRes/emptyFlat.geojson"));

  controlFlat = importPolygonFromGeoJson("IOToolsTestRes/emptyFlat.geojson");

  ASSERT_EQ(controlFlat.outer().size(), 0);

  // Почистим за собой файл
  OS::deleteFile("IOToolsTestRes/emptyFlat.geojson");
}
//==============================================================================
/*!
* \brief Тестирование записи рамки в .geojson-файл
*/
TEST(GeoJsonIOTest, ExportBoxToGeoJson)
{
  //!< Исходная рамка
  GeoBox2D box(GeoPoint2D(Degree(16), Degree(7)),
    GeoPoint2D(Degree(99), Degree(64)));

  //! Записываем в .geojson-файл
  const std::string path = "IOToolsTestRes/box_test.geojson";
  ASSERT_TRUE(exportToGeoJson(path, box));

  //!< Рамка, извлеченная из полученного файла
  GeoBox2D control = importGeoBoxFromGeoJson(path);
  //! Проверяем соответствие координат углов
  ASSERT_EQ(box.min_corner(), control.min_corner());
  ASSERT_EQ(box.max_corner(), control.max_corner());

  // Проделаем всё то же самое для плоскостной геометрии

  //!< Исходная рамка
  Box2D boxFlat(Point2D(16, 7), Point2D(99, 64));

  //! Записываем в .geojson-файл
  const std::string pathFlat = "IOToolsTestRes/boxFlat_test.geojson";
  ASSERT_TRUE(exportToGeoJson(pathFlat, boxFlat));

  //!< Рамка, извлеченная из полученного файла
  Box2D controlFlat = importBoxFromGeoJson(pathFlat);
  //! Проверяем соответствие координат углов
  ASSERT_EQ(boxFlat.min_corner(), controlFlat.min_corner());
  ASSERT_EQ(boxFlat.max_corner(), controlFlat.max_corner());
}
//==============================================================================
/*!
 * \brief Тестирование записи множества
*         многоугольников в .geojson-файл.
*/
TEST(GeoJsonIOTest, ExportMultiPolygonToGeoJson)
{
  //! Исходный мультиполигон
  GeoMultiPolygon2D original;
  original.emplace_back(GeoBox2D(GeoPoint2D(Degree(0), Degree(0)),
                        GeoPoint2D(Degree(10), Degree(10))).toPolygon());
  original.emplace_back(GeoBox2D(GeoPoint2D(Degree(11), Degree(11)),
                        GeoPoint2D(Degree(20), Degree(20))).toPolygon());

  ASSERT_TRUE(exportToGeoJson("IOToolsTestRes/test.geojson",
                                               original));

  GeoMultiPolygon2D control = importGeoMultiPolygonFromGeoJson(
    "IOToolsTestRes/test.geojson");

  ASSERT_EQ(control.size(), original.size());
  for (std::size_t i = 0; i < control.size(); ++i)
    ASSERT_EQ(control.at(i).outer(), original.at(i).outer());

  // Проверим запись пустого мультиполигона
  // На всякий случай убедимся, что файл не создан ранее
  OS::deleteFile("IOToolsTestRes/empty.geojson");

  // Если не хотим записывать пустой мультиполигон, он не запишется
  ASSERT_TRUE(exportToGeoJson("IOToolsTestRes/empty.geojson",
    GeoMultiPolygon2D(), false));

  ASSERT_FALSE(OS::isFileExists("IOToolsTestRes/empty.geojson"));

  // Но можно и записывать (поведение по-умолчанию)
  ASSERT_TRUE(exportToGeoJson("IOToolsTestRes/empty.geojson",
    GeoMultiPolygon2D()));

  ASSERT_TRUE(OS::isFileExists("IOToolsTestRes/empty.geojson"));

  control = importGeoMultiPolygonFromGeoJson("IOToolsTestRes/empty.geojson");

  ASSERT_EQ(control.size(), 0);

  // Почистим за собой файл
  OS::deleteFile("IOToolsTestRes/empty.geojson");

  // Если мультиполигон содержит лишь пустой полигон, записываем его
  // всегда
  ASSERT_TRUE(exportToGeoJson("IOToolsTestRes/empty.geojson",
    GeoMultiPolygon2D({ GeoPolygon2D() })));

  ASSERT_TRUE(OS::isFileExists("IOToolsTestRes/empty.geojson"));

  control = importGeoMultiPolygonFromGeoJson("IOToolsTestRes/empty.geojson");

  ASSERT_EQ(control.size(), 1);
  ASSERT_TRUE(control.at(0).outer().empty());

  // Почистим за собой файл
  OS::deleteFile("IOToolsTestRes/empty.geojson");

  // Проделаем всё то же самое для плоскостной геометрии

  //! Исходный мультиполигон
  MultiPolygon2D originalFlat;
  originalFlat.emplace_back(
    Box2D(Point2D(0, 0), Point2D(10, 10)).toPolygon());
  originalFlat.emplace_back(
    Box2D(Point2D(11, 11), Point2D(20, 20)).toPolygon());

  ASSERT_TRUE(exportToGeoJson("IOToolsTestRes/testFlat.geojson",
    originalFlat));

  MultiPolygon2D controlFlat = importMultiPolygonFromGeoJson("IOToolsTestRes/"
    "testFlat.geojson");

  ASSERT_EQ(controlFlat.size(), originalFlat.size());
  for (std::size_t i = 0; i < controlFlat.size(); ++i)
    ASSERT_EQ(controlFlat.at(i).outer(), originalFlat.at(i).outer());

  // Проверим запись пустого мультиполигона
  // На всякий случай убедимся, что файл не создан ранее
  OS::deleteFile("IOToolsTestRes/emptyFlat.geojson");

  // Если не хотим записывать пустой мультиполигон, он не запишется
  ASSERT_TRUE(exportToGeoJson("IOToolsTestRes/emptyFlat.geojson",
    MultiPolygon2D(), false));

  ASSERT_FALSE(OS::isFileExists("IOToolsTestRes/emptyFlat.geojson"));

  // Но можно и записывать (поведение по-умолчанию)
  ASSERT_TRUE(exportToGeoJson("IOToolsTestRes/emptyFlat.geojson",
    MultiPolygon2D()));

  ASSERT_TRUE(OS::isFileExists("IOToolsTestRes/emptyFlat.geojson"));

  controlFlat = importMultiPolygonFromGeoJson(
    "IOToolsTestRes/emptyFlat.geojson");

  ASSERT_EQ(controlFlat.size(), 0);

  // Почистим за собой файл
  OS::deleteFile("IOToolsTestRes/emptyFlat.geojson");

  // Если мультиполигон содержит лишь пустой полигон, записываем его
  // всегда
  ASSERT_TRUE(exportToGeoJson("IOToolsTestRes/emptyFlat.geojson",
    MultiPolygon2D({ Polygon2D() })));

  ASSERT_TRUE(OS::isFileExists("IOToolsTestRes/emptyFlat.geojson"));

  controlFlat = importMultiPolygonFromGeoJson(
    "IOToolsTestRes/emptyFlat.geojson");

  ASSERT_EQ(controlFlat.size(), 1);
  ASSERT_TRUE(controlFlat.at(0).outer().empty());

  // Почистим за собой файл
  OS::deleteFile("IOToolsTestRes/emptyFlat.geojson");
}
//==============================================================================
/*!
 * \brief Тестирование записи множества
 *        многоугольников c высотами в .geojson-файл
 */
TEST(GeoJsonIOTest, ExportPolygonsWithHeightToGeoJson)
{
  //! Тестовый набор полигонов с высотами
  //! (1 полигон с 1 внутренним кольцом)
  std::vector<std::pair<GeoPolygon2D, double>> original;
  original.resize(1);
  original[0].first.outer().resize(4);
  original[0].first.outer()[0] = {Degree{61.74897}, Degree{25.77714}};
  original[0].first.outer()[1] = {Degree{61.74897}, Degree{28.19846}};
  original[0].first.outer()[2] = {Degree{66.66147}, Degree{28.19846}};
  original[0].first.outer()[3] = {Degree{66.66147}, Degree{25.77714}};
  original[0].first.inners().resize(1);
  original[0].first.inners()[0].resize(4);
  original[0].first.inners()[0][0] = {Degree{62.98479}, Degree{26.06060}};
  original[0].first.inners()[0][1] = {Degree{62.98479}, Degree{27.65535}};
  original[0].first.inners()[0][2] = {Degree{65.12345}, Degree{27.65535}};
  original[0].first.inners()[0][3] = {Degree{65.12345}, Degree{26.06060}};
  original[0].second = 512.0;

  std::string path = "IOToolsTestRes/temp.geojson";
  //! Запись в .geojson - файл
  ASSERT_TRUE(exportToGeoJson(path, original));
  //! Записанный набор полигонов с высотами
  auto read = importGeoPolygonsWithHeightFromGeoJson(path);

  //! Число полигонов
  ASSERT_EQ(original.size(), read.size());
  //! Внешнее кольцо
  ASSERT_EQ(original[0].first.outer(), read[0].first.outer());
  //! Внутреннее кольцо
  ASSERT_EQ(original[0].first.inners()[0], read[0].first.inners()[0]);
  //! Высота полигона
  ASSERT_EQ(original[0].second, read[0].second);

  // Проверим запись пустой геометрии
  // На всякий случай убедимся, что файл не создан ранее
  OS::deleteFile("IOToolsTestRes/empty.geojson");

  // Если не хотим записывать пустую геометрию, она не запишется
  ASSERT_TRUE(exportToGeoJson("IOToolsTestRes/empty.geojson",
    std::vector<std::pair<GeoPolygon2D, double>>(), false));

  ASSERT_FALSE(OS::isFileExists("IOToolsTestRes/empty.geojson"));

  // Но можно и записывать (поведение по-умолчанию)
  ASSERT_TRUE(exportToGeoJson("IOToolsTestRes/empty.geojson",
    std::vector<std::pair<GeoPolygon2D, double>>()));

  ASSERT_TRUE(OS::isFileExists("IOToolsTestRes/empty.geojson"));

  read = importGeoPolygonsWithHeightFromGeoJson(
    "IOToolsTestRes/empty.geojson");

  ASSERT_EQ(read.size(), 0);

  // Почистим за собой файл
  OS::deleteFile("IOToolsTestRes/empty.geojson");

  // Проделаем всё то же самое для плоскостной геометрии

  //! Тестовый набор полигонов с высотами
  //! (1 полигон с 1 внутренним кольцом)
  std::vector<std::pair<Polygon2D, double>> originalFlat;
  originalFlat.resize(1);
  originalFlat[0].first.outer().resize(4);
  originalFlat[0].first.outer()[0] = ((61.74897), (25.77714));
  originalFlat[0].first.outer()[1] = ((61.74897), (28.19846));
  originalFlat[0].first.outer()[2] = ((66.66147), (28.19846));
  originalFlat[0].first.outer()[3] = ((66.66147), (25.77714));
  originalFlat[0].first.inners().resize(1);
  originalFlat[0].first.inners()[0].resize(4);
  originalFlat[0].first.inners()[0][0] = ((62.98479), (26.06060));
  originalFlat[0].first.inners()[0][1] = ((62.98479), (27.65535));
  originalFlat[0].first.inners()[0][2] = ((65.12345), (27.65535));
  originalFlat[0].first.inners()[0][3] = ((65.12345), (26.06060));
  originalFlat[0].second = 512.0;

  std::string pathFlat = "IOToolsTestRes/tempFlat.geojson";
  //! Запись в .geojson - файл
  ASSERT_TRUE(exportToGeoJson(pathFlat, originalFlat));
  //! Записанный набор полигонов с высотами
  auto readFlat = importPolygonsWithHeightFromGeoJson(pathFlat);

  //! Число полигонов
  ASSERT_EQ(originalFlat.size(), readFlat.size());
  //! Внешнее кольцо
  ASSERT_EQ(originalFlat[0].first.outer(), readFlat[0].first.outer());
  //! Внутреннее кольцо
  ASSERT_EQ(originalFlat[0].first.inners()[0], readFlat[0].first.inners()[0]);
  //! Высота полигона
  ASSERT_EQ(originalFlat[0].second, readFlat[0].second);

  // Проверим запись пустой геометрии
  // На всякий случай убедимся, что файл не создан ранее
  OS::deleteFile("IOToolsTestRes/emptyFlat.geojson");

  // Если не хотим записывать пустую геометрию, она не запишется
  ASSERT_TRUE(exportToGeoJson("IOToolsTestRes/emptyFlat.geojson",
    std::vector<std::pair<Polygon2D, double>>(), false));

  ASSERT_FALSE(OS::isFileExists("IOToolsTestRes/emptyFlat.geojson"));

  // Но можно и записывать (поведение по-умолчанию)
  ASSERT_TRUE(exportToGeoJson("IOToolsTestRes/emptyFlat.geojson",
    std::vector<std::pair<Polygon2D, double>>()));

  ASSERT_TRUE(OS::isFileExists("IOToolsTestRes/emptyFlat.geojson"));

  readFlat = importPolygonsWithHeightFromGeoJson(
    "IOToolsTestRes/emptyFlat.geojson");

  ASSERT_EQ(readFlat.size(), 0);

  // Почистим за собой файл
  OS::deleteFile("IOToolsTestRes/emptyFlat.geojson");
}
//==============================================================================
