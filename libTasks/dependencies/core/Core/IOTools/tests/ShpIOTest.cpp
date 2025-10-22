#include <IOTools/ShpIO/ShpIO.hpp>

#include <OSTools/FindFiles.hpp>
#include <OSTools/DeleteFile.hpp>
//! gtest
#include <gtest/gtest.h>
//==============================================================================
namespace Tests
{
//==============================================================================
/*!
 * \brief Тестирование записи и чтения множества точек из .shp-файла.
 */
TEST( ShpIOTest, MultiPointShp )
{
  MultiPoint2D multiPoint;
  multiPoint.emplace_back( Point2D( 1111., 0. ) );
  multiPoint.emplace_back( Point2D( 666., 333. ) );
  ASSERT_EQ(
    exportToShapeFile( "test_multipoint.shp", multiPoint ),
    true );

  //!< Файлы в текущей директории.
  auto files( OS::findFiles( "./" ) );
  ASSERT_NE(
    std::find( files.begin( ), files.end( ), "./test_multipoint.shp" ),
    files.end( ) );
  ASSERT_NE(
    std::find( files.begin( ), files.end( ), "./test_multipoint.shx" ),
    files.end( ) );
  ASSERT_NE(
    std::find( files.begin( ), files.end( ), "./test_multipoint.dbf" ),
    files.end( ) );

  GeoMultiPoint2D geoMultiPoint;
  geoMultiPoint.emplace_back( GeoPoint2D( Degree( 4. ), Degree( 5. ) ) );
  geoMultiPoint.emplace_back( GeoPoint2D( Degree( 3. ), Degree( 4. ) ) );
  ASSERT_EQ(
    exportToShapeFile( "test_geomultipoint.shp", geoMultiPoint ),
    true );

  files = OS::findFiles( "./" );
  ASSERT_NE(
    std::find( files.begin( ), files.end( ), "./test_geomultipoint.shp" ),
    files.end( ) );
  ASSERT_NE(
    std::find( files.begin( ), files.end( ), "./test_geomultipoint.shx" ),
    files.end( ) );
  ASSERT_NE(
    std::find( files.begin( ), files.end( ), "./test_geomultipoint.dbf" ),
    files.end( ) );

  MultiPoint2D multiPointImported(
    importMultiPoint2DFromShapeFile( "test_multipoint.shp" ) );
  GeoMultiPoint2D geoMultiPointImported(
    importGeoMultiPoint2DFromShapeFile( "test_geomultipoint.shp" ) );

  ASSERT_EQ( multiPoint.size( ), multiPointImported.size( ) );
  for( std::size_t i( 0 ); i < multiPoint.size( ); ++i )
    ASSERT_EQ( multiPoint[i], multiPointImported[i] );

  ASSERT_EQ( geoMultiPoint.size( ), geoMultiPointImported.size( ) );
  for( std::size_t i( 0 ); i < geoMultiPoint.size( ); ++i )
    ASSERT_EQ( geoMultiPoint[i], geoMultiPointImported[i] );

  //! Запись и чтение .shp с конвертацией в градусы.

  ASSERT_TRUE(
    exportToShapeFileInDegrees(
      "test_geomultipoint_degrees.shp",
      geoMultiPoint ) );

  files = OS::findFiles( "./" );
  ASSERT_NE(
    std::find( files.begin( ), files.end( ), "./test_geomultipoint_degrees.shp" ),
    files.end( ) );
  ASSERT_NE(
    std::find( files.begin( ), files.end( ), "./test_geomultipoint_degrees.shx" ),
    files.end( ) );
  ASSERT_NE(
    std::find( files.begin( ), files.end( ), "./test_geomultipoint_degrees.dbf" ),
    files.end( ) );

  geoMultiPointImported = importGeoMultiPoint2DFromShapeFileInDegrees(
    "test_geomultipoint_degrees.shp" );

  ASSERT_EQ( geoMultiPoint.size( ), geoMultiPointImported.size( ) );
  for( std::size_t i( 0 ); i < geoMultiPoint.size( ); ++i )
    ASSERT_EQ( geoMultiPoint[i], geoMultiPointImported[i] );

  // На всякий случай убедимся, что файлы не созданы ранее
  OS::deleteShpFile("./empty_multipoint.shp");
  OS::deleteShpFile("./empty_geomultipoint.shp");

  // Запись и чтение пустого множества точек
  ASSERT_TRUE(
    exportToShapeFile("empty_multipoint.shp", MultiPoint2D()));

  files = OS::findFiles("./");
  ASSERT_NE(
    std::find(files.begin(), files.end(), "./empty_multipoint.shp"),
    files.end());
  ASSERT_NE(
    std::find(files.begin(), files.end(), "./empty_multipoint.shx"),
    files.end());
  ASSERT_NE(
    std::find(files.begin(), files.end(), "./empty_multipoint.dbf"),
    files.end());

  multiPointImported = importMultiPoint2DFromShapeFile(
    "empty_multipoint.shp");

  ASSERT_EQ(multiPointImported.size(), 0);

  ASSERT_TRUE(
    exportToShapeFileInDegrees("empty_geomultipoint.shp", GeoMultiPoint2D()));

  files = OS::findFiles("./");
  ASSERT_NE(
    std::find(files.begin(), files.end(), "./empty_geomultipoint.shp"),
    files.end());
  ASSERT_NE(
    std::find(files.begin(), files.end(), "./empty_geomultipoint.shx"),
    files.end());
  ASSERT_NE(
    std::find(files.begin(), files.end(), "./empty_geomultipoint.dbf"),
    files.end());

  geoMultiPointImported = importGeoMultiPoint2DFromShapeFile(
    "empty_geomultipoint.shp");

  ASSERT_EQ(geoMultiPointImported.size(), 0);

  // Удаляем файлы
  OS::deleteShpFile("./empty_multipoint.shp");
  OS::deleteShpFile("./empty_geomultipoint.shp");

  // Если не хотим, чтобы пустая геометрия записывалась
  ASSERT_TRUE(
    exportToShapeFile("empty_multipoint.shp", MultiPoint2D(), false));

  files = OS::findFiles("./");
  ASSERT_EQ(
    std::find(files.begin(), files.end(), "./empty_multipoint.shp"),
    files.end());
  ASSERT_EQ(
    std::find(files.begin(), files.end(), "./empty_multipoint.shx"),
    files.end());
  ASSERT_EQ(
    std::find(files.begin(), files.end(), "./empty_multipoint.dbf"),
    files.end());


  ASSERT_TRUE(
    exportToShapeFileInDegrees(
      "empty_geomultipoint.shp", GeoMultiPoint2D(), false));

  files = OS::findFiles("./");
  ASSERT_EQ(
    std::find(files.begin(), files.end(), "./empty_geomultipoint.shp"),
    files.end());
  ASSERT_EQ(
    std::find(files.begin(), files.end(), "./empty_geomultipoint.shx"),
    files.end());
  ASSERT_EQ(
    std::find(files.begin(), files.end(), "./empty_geomultipoint.dbf"),
    files.end());
}
//==============================================================================
/*!
 * \brief Тестирование записи и чтения ломаной линии из .shp-файла.
 */
TEST( ShpIOTest, LinestringShp )
{
  Linestring2D linestring;
  linestring.emplace_back( Point2D( 0., 0. ) );
  linestring.emplace_back( Point2D( 1., 1. ) );
  linestring.emplace_back( Point2D( 2., 0. ) );
  linestring.emplace_back( Point2D( 3., 1. ) );
  ASSERT_EQ(
    exportToShapeFile( "test_linestring.shp", linestring ),
    true );

  //!< Файлы в текущей директории.
  auto files( OS::findFiles( "./" ) );
  ASSERT_NE(
    std::find( files.begin( ), files.end( ), "./test_linestring.shp" ),
    files.end( ) );
  ASSERT_NE(
    std::find( files.begin( ), files.end( ), "./test_linestring.shx" ),
    files.end( ) );
  ASSERT_NE(
    std::find( files.begin( ), files.end( ), "./test_linestring.dbf" ),
    files.end( ) );

  Linestring2D linestringImported(
    importLinestring2DFromShapeFile( "test_linestring.shp" ) );

  ASSERT_EQ( linestring.size( ), linestringImported.size( ) );
  for( std::size_t i( 0 ); i < linestring.size( ); ++i )
    ASSERT_EQ( linestring[i], linestringImported[i] );

  //! Запись и чтение .shp с конвертацией в градусы.

  GeoLinestring2D geoLinestring{
    { Radian( .512 ), Radian( 6.277 ) },
    { Radian( .683 ), Radian( -6.615 ) },
    { Radian( 1.813 ), Radian( -4.619 ) },
    { Radian( -.593 ), Radian( 8.038 ) }
  };
  ASSERT_TRUE(
    exportToShapeFileInDegrees(
      "test_geolinestring_degrees.shp",
      geoLinestring ) );

  files = OS::findFiles( "./" );
  ASSERT_NE(
    std::find( files.begin( ), files.end( ), "./test_geolinestring_degrees.shp" ),
    files.end( ) );
  ASSERT_NE(
    std::find( files.begin( ), files.end( ), "./test_geolinestring_degrees.shx" ),
    files.end( ) );
  ASSERT_NE(
    std::find( files.begin( ), files.end( ), "./test_geolinestring_degrees.dbf" ),
    files.end( ) );

  GeoLinestring2D geoLinestringImported(
    importGeoLinestring2DFromShapeFileInDegrees(
      "test_geolinestring_degrees.shp" ) );

  ASSERT_EQ( geoLinestring.size( ), geoLinestringImported.size( ) );
  for( std::size_t i( 0 ); i < linestring.size( ); ++i )
    ASSERT_EQ( geoLinestring[i], geoLinestringImported[i] );

  // На всякий случай убедимся, что файлы не созданы ранее
  OS::deleteShpFile("./empty_linestring.shp");
  OS::deleteShpFile("./empty_geolinestring.shp");

  // Запись и чтение пустой ломаной
  ASSERT_TRUE(
    exportToShapeFile("empty_linestring.shp", Linestring2D()));

  files = OS::findFiles("./");
  ASSERT_NE(
    std::find(files.begin(), files.end(), "./empty_linestring.shp"),
    files.end());
  ASSERT_NE(
    std::find(files.begin(), files.end(), "./empty_linestring.shx"),
    files.end());
  ASSERT_NE(
    std::find(files.begin(), files.end(), "./empty_linestring.dbf"),
    files.end());

  linestringImported = importLinestring2DFromShapeFile(
    "empty_linestring.shp");

  ASSERT_EQ(linestringImported.size(), 0);

  ASSERT_TRUE(
    exportToShapeFileInDegrees("empty_geolinestring.shp", GeoLinestring2D()));

  files = OS::findFiles("./");
  ASSERT_NE(
    std::find(files.begin(), files.end(), "./empty_geolinestring.shp"),
    files.end());
  ASSERT_NE(
    std::find(files.begin(), files.end(), "./empty_geolinestring.shx"),
    files.end());
  ASSERT_NE(
    std::find(files.begin(), files.end(), "./empty_geolinestring.dbf"),
    files.end());

  geoLinestringImported = importGeoLinestring2DFromShapeFileInDegrees(
    "empty_geolinestring.shp");

  ASSERT_EQ(geoLinestringImported.size(), 0);

  // Удаляем файлы
  OS::deleteShpFile("./empty_linestring.shp");
  OS::deleteShpFile("./empty_geolinestring.shp");

  // Если не хотим, чтобы пустая геометрия записывалась
  ASSERT_TRUE(
    exportToShapeFile("empty_linestring.shp", Linestring2D(), false));

  files = OS::findFiles("./");
  ASSERT_EQ(
    std::find(files.begin(), files.end(), "./empty_linestring.shp"),
    files.end());
  ASSERT_EQ(
    std::find(files.begin(), files.end(), "./empty_linestring.shx"),
    files.end());
  ASSERT_EQ(
    std::find(files.begin(), files.end(), "./empty_linestring.dbf"),
    files.end());


  ASSERT_TRUE(
    exportToShapeFileInDegrees(
      "empty_geolinestring.shp", GeoLinestring2D(), false));

  files = OS::findFiles("./");
  ASSERT_EQ(
    std::find(files.begin(), files.end(), "./empty_geolinestring.shp"),
    files.end());
  ASSERT_EQ(
    std::find(files.begin(), files.end(), "./empty_geolinestring.shx"),
    files.end());
  ASSERT_EQ(
    std::find(files.begin(), files.end(), "./empty_geolinestring.dbf"),
    files.end());
}
//==============================================================================
/*!
 * \brief Тестирование записи и чтения прямоугольника из .shp-файла.
 */
TEST( ShpIOTest, BoxShp )
{
  GeoBox2D box(
    GeoPoint2D( Degree( 1. ), Degree( 1. ) ),
    GeoPoint2D( Degree( 35. ), Degree( 35. ) ) );
  ASSERT_EQ(
    exportToShapeFile( "test_box.shp", box ),
    true );

  //!< Файлы в текущей директории.
  auto files( OS::findFiles( "./" ) );
  ASSERT_NE(
    std::find( files.begin( ), files.end( ), "./test_box.shp" ),
    files.end( ) );
  ASSERT_NE(
    std::find( files.begin( ), files.end( ), "./test_box.shx" ),
    files.end( ) );
  ASSERT_NE(
    std::find( files.begin( ), files.end( ), "./test_box.dbf" ),
    files.end( ) );

  GeoBox2D boxImported( importGeoBox2DFromShapeFile( "test_box.shp" ) );

  ASSERT_EQ( box.max_corner( ), boxImported.max_corner( ) );
  ASSERT_EQ( box.min_corner( ), boxImported.min_corner( ) );

  //! Запись и чтение .shp с конвертацией в градусы.

  ASSERT_TRUE( exportToShapeFileInDegrees( "test_box_degrees.shp", box ) );

  files = OS::findFiles( "./" );
  ASSERT_NE(
    std::find( files.begin( ), files.end( ), "./test_box_degrees.shp" ),
    files.end( ) );
  ASSERT_NE(
    std::find( files.begin( ), files.end( ), "./test_box_degrees.shx" ),
    files.end( ) );
  ASSERT_NE(
    std::find( files.begin( ), files.end( ), "./test_box_degrees.dbf" ),
    files.end( ) );

  boxImported = importGeoBox2DFromShapeFileInDegrees( "test_box_degrees.shp" );

  ASSERT_EQ( box.max_corner( ), boxImported.max_corner( ) );
  ASSERT_EQ( box.min_corner( ), boxImported.min_corner( ) );
}
//==============================================================================
/*!
 * \brief Тестирование записи и чтения множества многоугольников из .shp-файла.
 */
TEST( ShpIOTest, MultiPolygonShp )
{
  GeoMultiPolygon2D multiPolygon;
  GeoPolygon2D polygon;
  polygon.outer( ).emplace_back( GeoPoint2D( Degree( -10. ), Degree( 175. ) ) );
  polygon.outer( ).emplace_back( GeoPoint2D( Degree( 10. ), Degree( 175. ) ) );
  polygon.outer( ).emplace_back( GeoPoint2D( Degree( 10. ), Degree( -175. ) ) );
  polygon.outer( ).emplace_back( GeoPoint2D( Degree( -10. ), Degree( -175. ) ) );
  polygon.outer( ).emplace_back( GeoPoint2D( Degree( -10. ), Degree( 175. ) ) );
  multiPolygon.emplace_back( polygon );
  GeoPolygon2D polygon2;
  polygon2.outer( ).emplace_back( GeoPoint2D( Degree( -45. ), Degree( -45. ) ) );
  polygon2.outer( ).emplace_back( GeoPoint2D( Degree( -20. ), Degree( -30. ) ) );
  polygon2.outer( ).emplace_back( GeoPoint2D( Degree( 0. ), Degree( 0. ) ) );
  polygon2.outer( ).emplace_back( GeoPoint2D( Degree( -10. ), Degree( 10. ) ) );
  polygon2.outer( ).emplace_back( GeoPoint2D( Degree( -50. ), Degree( 0. ) ) );
  polygon2.outer( ).emplace_back( GeoPoint2D( Degree( -45. ), Degree( -45. ) ) );
  GeoRing2D innerRing;
  innerRing.emplace_back( GeoPoint2D( Degree( -30. ), Degree( -30. ) ) );
  innerRing.emplace_back( GeoPoint2D( Degree( -45. ), Degree( -30. ) ) );
  innerRing.emplace_back( GeoPoint2D( Degree( -45. ), Degree( 0. ) ) );
  innerRing.emplace_back( GeoPoint2D( Degree( -30. ), Degree( 5. ) ) );
  innerRing.emplace_back( GeoPoint2D( Degree( -20. ), Degree( 0. ) ) );
  innerRing.emplace_back( GeoPoint2D( Degree( -30. ), Degree( -30. ) ) );
  polygon2.inners( ).emplace_back( innerRing );
  multiPolygon.emplace_back( polygon2 );
  ASSERT_EQ(
    exportToShapeFile( "test_multipolygon.shp", multiPolygon ),
    true );

  //!< Файлы в текущей директории.
  auto files( OS::findFiles( "./" ) );
  ASSERT_NE(
    std::find( files.begin( ), files.end( ), "./test_multipolygon.shp" ),
    files.end( ) );
  ASSERT_NE(
    std::find( files.begin( ), files.end( ), "./test_multipolygon.shx" ),
    files.end( ) );
  ASSERT_NE(
    std::find( files.begin( ), files.end( ), "./test_multipolygon.dbf" ),
    files.end( ) );

  GeoMultiPolygon2D multiPolygonImported(
    importGeoMultiPolygon2DFromShapeFile( "test_multipolygon.shp" ) );

  ASSERT_EQ( multiPolygon.size( ), multiPolygonImported.size( ) );
  for( std::size_t i( 0 ); i < multiPolygon.size( ); ++i )
  {
    ASSERT_EQ(
      multiPolygon[i].outer( ).size( ),
      multiPolygonImported[i].outer( ).size( ) );

    for( std::size_t j( 0 ); j < multiPolygon[i].outer( ).size( ); ++j )
      ASSERT_EQ(
        multiPolygon[i].outer( )[j],
        multiPolygonImported[i].outer( )[j] );

    ASSERT_EQ(
      multiPolygon[i].inners( ).size( ),
      multiPolygonImported[i].inners( ).size( ) );

    for( std::size_t j( 0 ); j < multiPolygon[i].inners( ).size( ); ++j )
    {
      ASSERT_EQ(
        multiPolygon[i].inners( )[j].size( ),
        multiPolygonImported[i].inners( )[j].size( ) );

      for( std::size_t k( 0 ); k < multiPolygon[i].inners( )[j].size( ); ++k )
        ASSERT_EQ(
          multiPolygon[i].inners( )[j][k],
          multiPolygonImported[i].inners( )[j][k] );
    }
  }

  //! Запись и чтение .shp с конвертацией в градусы.

  ASSERT_TRUE(
    exportToShapeFileInDegrees( "test_multipolygon_degrees.shp", multiPolygon ) );

  files = OS::findFiles( "./" );
  ASSERT_NE(
    std::find( files.begin( ), files.end( ), "./test_multipolygon_degrees.shp" ),
    files.end( ) );
  ASSERT_NE(
    std::find( files.begin( ), files.end( ), "./test_multipolygon_degrees.shx" ),
    files.end( ) );
  ASSERT_NE(
    std::find( files.begin( ), files.end( ), "./test_multipolygon_degrees.dbf" ),
    files.end( ) );

  multiPolygonImported = importGeoMultiPolygon2DFromShapeFileInDegrees(
    "test_multipolygon_degrees.shp" );

  ASSERT_EQ( multiPolygon.size( ), multiPolygonImported.size( ) );
  for( std::size_t i( 0 ); i < multiPolygon.size( ); ++i )
  {
    ASSERT_EQ(
      multiPolygon[i].outer( ).size( ),
      multiPolygonImported[i].outer( ).size( ) );

    for( std::size_t j( 0 ); j < multiPolygon[i].outer( ).size( ); ++j )
      ASSERT_EQ(
        multiPolygon[i].outer( )[j], multiPolygonImported[i].outer( )[j] );

    ASSERT_EQ(
      multiPolygon[i].inners( ).size( ),
      multiPolygonImported[i].inners( ).size( ) );

    for( std::size_t j( 0 ); j < multiPolygon[i].inners( ).size( ); ++j )
    {
      ASSERT_EQ(
        multiPolygon[i].inners( )[j].size( ),
        multiPolygonImported[i].inners( )[j].size( ) );

      for( std::size_t k( 0 ); k < multiPolygon[i].inners( )[j].size( ); ++k )
        ASSERT_EQ(
          multiPolygon[i].inners( )[j][k],
         multiPolygonImported[i].inners( )[j][k] );
    }
  }

  // На всякий случай убедимся, что файл не создан ранее
  OS::deleteShpFile("./empty.shp");

  // Если мультиполигон содержит лишь пустой полигон, записываем его
  // всегда
  ASSERT_TRUE(exportToShapeFileInDegrees("./empty.shp",
    GeoMultiPolygon2D({ GeoPolygon2D() }), false));

  ASSERT_TRUE(OS::isFileExists("./empty.shp"));

  // Проверим, что прочитанный полигон совпадает с записанным
  multiPolygonImported = importGeoMultiPolygon2DFromShapeFileInDegrees(
    "./empty.shp");

  ASSERT_EQ(multiPolygonImported.size(), 1);
  ASSERT_TRUE(multiPolygonImported.at(0).outer().empty());

  // Почистим за собой файл
  OS::deleteShpFile("./empty.shp");

  // Проверим, что если не хотим писать пустой мультиполигон,
  // он не запишется
  ASSERT_TRUE(exportToShapeFileInDegrees(
    "./empty.shp", GeoMultiPolygon2D(), false));

  ASSERT_FALSE(OS::isFileExists("./empty.shp"));

  // Но можно и записывать (поведение по-умолчанию)
  ASSERT_TRUE(exportToShapeFileInDegrees("./empty.shp",
    GeoMultiPolygon2D()));

  ASSERT_TRUE(OS::isFileExists("./empty.shp"));

  multiPolygonImported = importGeoMultiPolygon2DFromShapeFileInDegrees(
    "./empty.shp");

  ASSERT_EQ(multiPolygonImported.size(), 0);

  // И снова чистим файл
  OS::deleteShpFile("./empty.shp");

  // Абсолютно аналогично будет в радианах

  // Мультиполигон содержит лишь пустой полигон - записываем
  ASSERT_TRUE(exportToShapeFile("./empty.shp",
    GeoMultiPolygon2D({ GeoPolygon2D() }), false));

  ASSERT_TRUE(OS::isFileExists("./empty.shp"));

  // Проверяем, что записалось
  multiPolygonImported = importGeoMultiPolygon2DFromShapeFile("./empty.shp");

  ASSERT_EQ(multiPolygonImported.size(), 1);
  ASSERT_TRUE(multiPolygonImported.at(0).outer().empty());

  OS::deleteShpFile("./empty.shp");

  // Экспорт мультиполигона (пустой, не запишется)
  ASSERT_TRUE(exportToShapeFile("./empty.shp", GeoMultiPolygon2D(), false));

  ASSERT_FALSE(OS::isFileExists("./empty.shp"));

  // Экспорт с записью пустого мультиполигона (поведение по-умолчанию)
  ASSERT_TRUE(exportToShapeFile("./empty.shp",
    GeoMultiPolygon2D()));

  ASSERT_TRUE(OS::isFileExists("./empty.shp"));

  multiPolygonImported = importGeoMultiPolygon2DFromShapeFile("./empty.shp");

  ASSERT_EQ(multiPolygonImported.size(), 0);

  // И снова чистим файл
  OS::deleteShpFile("./empty.shp");

  // Абсолютно аналогично будет на плоскости

  // Мультиполигон содержит лишь пустой полигон - записываем
  ASSERT_TRUE(exportToShapeFile("./empty.shp",
    MultiPolygon2D({ Polygon2D() }), false));

  ASSERT_TRUE(OS::isFileExists("./empty.shp"));

  // Проверяем, что записалось
  MultiPolygon2D multiPolygonFlat(
    importMultiPolygon2DFromShapeFile("./empty.shp"));

  ASSERT_EQ(multiPolygonFlat.size(), 1);
  ASSERT_TRUE(multiPolygonFlat.at(0).outer().empty());

  OS::deleteShpFile("./empty.shp");

  // Экспорт, пустые геометрии писать не хотим
  ASSERT_TRUE(exportToShapeFile("./empty.shp", MultiPolygon2D(), false));

  ASSERT_FALSE(OS::isFileExists("./empty.shp"));

  // Экспорт с записью пустого мультиполигона
  ASSERT_TRUE(exportToShapeFile("./empty.shp", MultiPolygon2D()));

  ASSERT_TRUE(OS::isFileExists("./empty.shp"));

  multiPolygonFlat = importMultiPolygon2DFromShapeFile("./empty.shp");

  ASSERT_EQ(multiPolygonFlat.size(), 0);

  // И снова чистим файл
  OS::deleteShpFile("./empty.shp");
}
//==============================================================================
/*!
* \brief Тестирование записи и чтения множества многоугольников из .shp-файла.
*/
TEST( ShpIOTest, PolygonShp )
{
  GeoPolygon2D polygon;
  polygon.outer( ).emplace_back( GeoPoint2D( Degree( -10. ), Degree( 175. ) ) );
  polygon.outer( ).emplace_back( GeoPoint2D( Degree( 10. ), Degree( 175. ) ) );
  polygon.outer( ).emplace_back( GeoPoint2D( Degree( 10. ), Degree( -175. ) ) );
  polygon.outer( ).emplace_back( GeoPoint2D( Degree( -10. ), Degree( -175. ) ) );
  polygon.outer( ).emplace_back( GeoPoint2D( Degree( -10. ), Degree( 175. ) ) );
  GeoPolygon2D polygon2;
  polygon2.outer( ).emplace_back( GeoPoint2D( Degree( -45. ), Degree( -45. ) ) );
  polygon2.outer( ).emplace_back( GeoPoint2D( Degree( -20. ), Degree( -30. ) ) );
  polygon2.outer( ).emplace_back( GeoPoint2D( Degree( 0. ), Degree( 0. ) ) );
  polygon2.outer( ).emplace_back( GeoPoint2D( Degree( -10. ), Degree( 10. ) ) );
  polygon2.outer( ).emplace_back( GeoPoint2D( Degree( -50. ), Degree( 0. ) ) );
  polygon2.outer( ).emplace_back( GeoPoint2D( Degree( -45. ), Degree( -45. ) ) );
  GeoRing2D innerRing;
  innerRing.emplace_back( GeoPoint2D( Degree( -30. ), Degree( -30. ) ) );
  innerRing.emplace_back( GeoPoint2D( Degree( -45. ), Degree( -30. ) ) );
  innerRing.emplace_back( GeoPoint2D( Degree( -45. ), Degree( 0. ) ) );
  innerRing.emplace_back( GeoPoint2D( Degree( -30. ), Degree( 5. ) ) );
  innerRing.emplace_back( GeoPoint2D( Degree( -20. ), Degree( 0. ) ) );
  innerRing.emplace_back( GeoPoint2D( Degree( -30. ), Degree( -30. ) ) );
  polygon2.inners( ).emplace_back( innerRing );

  ASSERT_EQ(
    exportToShapeFile( "test_polygon.shp", polygon ),
    true );

  //!< Файлы в текущей директории.
  auto files( OS::findFiles( "./" ) );
  ASSERT_NE(
    std::find( files.begin( ), files.end( ), "./test_polygon.shp" ),
    files.end( ) );
  ASSERT_NE(
    std::find( files.begin( ), files.end( ), "./test_polygon.shx" ),
    files.end( ) );
  ASSERT_NE(
    std::find( files.begin( ), files.end( ), "./test_polygon.dbf" ),
    files.end( ) );

  GeoPolygon2D polygonImported(
    importGeoPolygon2DFromShapeFile( "test_polygon.shp" ) );

  ASSERT_EQ( polygon.outer( ).size( ), polygonImported.outer( ).size( ) );
  for( std::size_t i( 0 ); i < polygon.outer( ).size( ); ++i )
  {
    ASSERT_EQ(
      polygon.outer( )[i],
      polygonImported.outer( )[i] );

    for( std::size_t j( 0 ); j < polygon.inners( ).size( ); ++j )
    {
      ASSERT_EQ(
        polygon.inners( )[j].size( ),
        polygonImported.inners( )[j].size( ) );

      for( std::size_t k( 0 ); k < polygon.inners( )[j].size( ); ++k )
        ASSERT_EQ(
          polygon.inners( )[j][k],
          polygonImported.inners( )[j][k] );
    }
  }

  //! Запись и чтение .shp с конвертацией в градусы.

  ASSERT_TRUE(
    exportToShapeFileInDegrees( "test_polygon_degrees.shp", polygon2 ) );

  files = OS::findFiles( "./" );
  ASSERT_NE(
    std::find( files.begin( ), files.end( ), "./test_polygon_degrees.shp" ),
    files.end( ) );
  ASSERT_NE(
    std::find( files.begin( ), files.end( ), "./test_polygon_degrees.shx" ),
    files.end( ) );
  ASSERT_NE(
    std::find( files.begin( ), files.end( ), "./test_polygon_degrees.dbf" ),
    files.end( ) );

  polygonImported = importGeoPolygon2DFromShapeFileInDegrees(
    "test_polygon_degrees.shp" );

  ASSERT_EQ( polygon2.outer( ).size( ), polygonImported.outer( ).size( ) );
  for( std::size_t i( 0 ); i < polygon2.outer( ).size( ); ++i )
  {
    ASSERT_EQ(
      polygon2.outer( )[i],
      polygonImported.outer( )[i] );

    for( std::size_t j( 0 ); j < polygon2.inners( ).size( ); ++j )
    {
      ASSERT_EQ(
        polygon2.inners( )[j].size( ),
        polygonImported.inners( )[j].size( ) );

      for( std::size_t k( 0 ); k < polygon2.inners( )[j].size( ); ++k )
        ASSERT_EQ(
          polygon2.inners( )[j][k],
          polygonImported.inners( )[j][k] );
    }
  }

  //! Запись пустого полигона
  ASSERT_TRUE(
    exportToShapeFile( "test_empty_polygon.shp", GeoPolygon2D( ) ) );

  files = OS::findFiles("./");
  ASSERT_NE(
    std::find(files.begin(), files.end(), "./test_empty_polygon.shp"),
    files.end());
  ASSERT_NE(
    std::find(files.begin(), files.end(), "./test_empty_polygon.shx"),
    files.end());
  ASSERT_NE(
    std::find(files.begin(), files.end(), "./test_empty_polygon.dbf"),
    files.end());

  // Удалим файл
  OS::deleteShpFile("./test_empty_polygon.shp");

  // Если не хотим, чтобы пустые полигоны записывались
  ASSERT_TRUE(
    exportToShapeFile("test_empty_polygon.shp", GeoPolygon2D(), false));

  files = OS::findFiles( "./" );
  ASSERT_EQ(
    std::find( files.begin( ), files.end( ), "./test_empty_polygon.shp" ),
    files.end( ) );
  ASSERT_EQ(
    std::find( files.begin( ), files.end( ), "./test_empty_polygon.shx" ),
    files.end( ) );
  ASSERT_EQ(
    std::find( files.begin( ), files.end( ), "./test_empty_polygon.dbf" ),
    files.end( ) );
}
//==============================================================================
} //! Tests
//==============================================================================
