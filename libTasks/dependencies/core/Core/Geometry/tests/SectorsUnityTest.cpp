#include <Geometry/Objects/SectorsUnity.hpp>

#include <Geometry/GeometryException.hpp>
//! std
#include <map>
//! gtest
#include <gtest/gtest.h>
//==============================================================================
namespace Geometry
{
namespace Tests
{
//==============================================================================
class SectorsUnityTest : public ::testing::Test
{
protected:

  void SetUp( )
  {
    // пустой набор секторов
    vEmpty = {};

    // полный сектор
    vFull = {
      Sector( Degree( 0 ), Degree( 360 ) )
    };

    // формально полный сектор
    vApproxFull = {
      Sector( Degree( 1 ), Degree( 359 ) )
    };

    // набор секторов без пересечения 0
    vSimpleOrdered = {
      Sector( Degree( 20 ), Degree( 40 ) ),
      Sector( Degree( 120 ), Degree( 140 ) ),
      Sector( Degree( 220 ), Degree( 240 ) )
    };
     vSimpleRand = {
       Sector( Degree( 20 ), Degree( 40 ) ),
       Sector( Degree( 220 ), Degree( 240 ) ),
       Sector( Degree( 120 ), Degree( 140 ) )
    };

    // набор секторов с пересечением 0
     vWithZeroOrdered = {
       Sector( Degree( 20 ), Degree( 40 ) ),
       Sector( Degree( 120 ), Degree( 140 ) ),
       Sector( Degree( 220 ), Degree( 240 ) ),
       Sector( Degree( 350 ), Degree( 10 ) )
    };
     vWithZeroRand = {
       Sector( Degree( 220 ), Degree( 240 ) ),
       Sector( Degree( 120 ), Degree( 140 ) ),
       Sector( Degree( 350 ), Degree( 10 ) ),
       Sector( Degree( 20 ), Degree( 40 ) )
    };
  }
//==============================================================================
  void TearDown( )
  {
  }
//==============================================================================
  std::vector < Sector >  vEmpty;
  std::vector < Sector >  vFull;
  std::vector < Sector >  vApproxFull;
  std::vector < Sector >  vSimpleRand;
  std::vector < Sector >  vSimpleOrdered;
  std::vector < Sector >  vWithZeroOrdered;
  std::vector < Sector >  vWithZeroRand;
};  
//==============================================================================
//==============================================================================
TEST_F( SectorsUnityTest, addOneSectorTest )
{
  Sector summandSec ( Degree( 260 ), Degree( 320 ) );
  Sector summandSecWithZero ( Degree( 350 ), Degree( 10 ) );

  std::vector< Sector > expectedVect;
  std::vector< Sector > resVect;

  // добавление чего-либо в полный сектор не меняет его
  SectorsUnity fullSec( vFull );
  bool res = fullSec.addSector( summandSec );
  ASSERT_TRUE( res );
  fullSec.getVector( resVect );
  ASSERT_EQ( resVect, vFull );

  res = fullSec.addSector( summandSecWithZero );
  ASSERT_TRUE( res );
  fullSec.getVector( resVect );
  ASSERT_EQ( resVect, vFull );

  // после добавления чего-то в сектор, он может стать полным (с точностью eps)
  // при этом его границы в градусах дойдут до (0, 360)
  SectorsUnity emptySec0( vEmpty, Degree(3.) );
  res = emptySec0.addSectors( vApproxFull ); // (1, 359) в градусах
  ASSERT_TRUE( res );
  emptySec0.getVector( resVect );
  ASSERT_EQ( resVect, vFull ); // (0, 360)

  // добавление обычного сектора в пустой сектор
  SectorsUnity emptySec( vEmpty );
  res = emptySec.addSector( summandSec );
  ASSERT_FALSE( res );
  emptySec.getVector( resVect );
  expectedVect.emplace_back( summandSec );
  ASSERT_EQ( resVect, expectedVect );

  // добавление обычного сектора в пустое место
  SectorsUnity simpleSec( vSimpleOrdered );
  res = simpleSec.addSector( summandSec );
  ASSERT_FALSE( res );
  simpleSec.getVector( resVect );
  expectedVect = vSimpleOrdered;
  expectedVect.emplace_back( summandSec );
  ASSERT_EQ( resVect, expectedVect );

  // добавление обычного сектора, частично пересекающего имеющиеся
  res = simpleSec.addSector( Sector( Degree( 30 ), Degree( 230 ) ) );
  ASSERT_FALSE( res );
  simpleSec.getVector( resVect );
  expectedVect = { Sector( Degree( 20 ), Degree( 240 ) )
  , summandSec};
  ASSERT_EQ( resVect, expectedVect );

  // добавление обычного сектора, частично прилипающего к имеющимся
  SectorsUnity approxSimpleSec( vSimpleOrdered, Degree( 2 ) );
  res =
    approxSimpleSec.addSector( Sector( Degree( 241 ), Degree( 260 ) ) );
  ASSERT_FALSE( res );
  approxSimpleSec.getVector( resVect );
  expectedVect = vSimpleOrdered;
  expectedVect[2] = Sector ( Degree(220), Degree( 260 ));
  ASSERT_EQ( resVect, expectedVect );


  // добавление пересекающего 0 сектора в пустой сектор
  SectorsUnity emptySec2 ( vEmpty );
  res = emptySec2.addSector( summandSecWithZero );
  ASSERT_FALSE( res );
  emptySec2.getVector( resVect );
  expectedVect = { summandSecWithZero };
  ASSERT_EQ( resVect, expectedVect );

  // добавление пересекающего 0 сектора в обычный сектор
  SectorsUnity simpleSec2( vSimpleOrdered );
  res = simpleSec2.addSector( summandSecWithZero );
  ASSERT_FALSE( res );
  simpleSec2.getVector( resVect );
  ASSERT_EQ( resVect, vWithZeroOrdered );

  // добавление пересекающего 0 сектора к секторам, пересекающим 0
  res = simpleSec2.addSector( Sector( Degree( 345 ), Degree( 5 ) ) );
  ASSERT_FALSE( res );
  simpleSec2.getVector( resVect );
  expectedVect = vWithZeroOrdered;
  expectedVect[3] = Sector ( Degree(345), Degree(10) );
  ASSERT_EQ( resVect, expectedVect );
}
//==============================================================================
//==============================================================================
TEST_F( SectorsUnityTest, addSeveralSectorTest )
{
  // добавление обычных секторов
  SectorsUnity emptySec( vEmpty );
  bool res = emptySec.addSectors( vSimpleRand );
  ASSERT_FALSE( res );
  std::vector< Sector > resVect;
  emptySec.getVector( resVect );
  ASSERT_EQ( resVect, vSimpleOrdered );

  // добавление слипающихся (с разных сторон) секторов 
  SectorsUnity prSectors(
  { Sector( Degree( 30 ), Degree( 120 ) ),
    Sector( Degree( 180 ), Degree( 20 ) ) }, Degree(10) );
  prSectors.getVector( resVect );
  std::vector<Sector> expectedVect = { Sector( Degree( 180 ), Degree( 120 ) ) };
  ASSERT_EQ( resVect, expectedVect );

  SectorsUnity prSectors2(
  { Sector( Degree( 180 ), Degree( 20 ) ), 
    Sector( Degree( 30 ), Degree( 120 ) ) }, Degree( 10 ) );
  prSectors2.getVector( resVect );
  expectedVect = { Sector( Degree( 180 ), Degree( 120 ) ) };
  ASSERT_EQ( resVect, expectedVect );
}
//==============================================================================
//==============================================================================
TEST_F( SectorsUnityTest, ConstructorTest )
{
  SectorsUnity secDeg( vSimpleRand );
  std::vector< Sector > resVect;
  secDeg.getVector( resVect );
  ASSERT_EQ( resVect, vSimpleOrdered );
}
//==============================================================================
//==============================================================================
TEST_F( SectorsUnityTest, cutSectorTest )
{
  // удаление существующего сектора
  SectorsUnity sec( vSimpleRand );
  sec.cutSector( 2 );
  std::vector< Sector > resVect;
  sec.getVector( resVect );
  std::vector<Sector> expectedVect = {
    Sector( Degree( 20 ), Degree( 40 ) ),
    Sector( Degree( 220 ), Degree( 240 ) )
  };
  ASSERT_EQ( resVect, expectedVect );

  // попытка удалить не существующий сектор
  ASSERT_THROW(sec.cutSector(3), GeometryFailure);
  try
  {
    sec.cutSector(3);
  }
  catch (const GeometryFailure& ex)
  {
    ASSERT_EQ(GeometryEnumFailure::SectorIndexOutOfRange, ex.getCode());
    ASSERT_EQ(geomLibTitle, ex.getLib());
    ASSERT_EQ(
      "[Core::Geometry] <SectorIndexOutOfRange>: "
        "Попытка найти сектор по индексу, выходящему за границы списка"
      , std::string(ex.what()));
  }
}
//==============================================================================
//==============================================================================
TEST_F( SectorsUnityTest, changeSectorTest )
{
  // замена существующего сектора
  SectorsUnity sec( vSimpleRand );
  bool res = sec.changeSector( 2, 
    Sector( Degree( 250 ), Degree( 270 ) ) );
  ASSERT_FALSE( res );
  std::vector< Sector > resVect;
  sec.getVector( resVect );
  std::vector<Sector> expectedVect = {
    Sector( Degree( 20 ), Degree( 40 ) ),
    Sector( Degree( 220 ), Degree( 240 ) ),
    Sector( Degree( 250 ), Degree( 270 ) )
  };
  ASSERT_EQ( resVect, expectedVect );

  // если удалить ничего не получилось, новый сектор все равно добавляем
  res = sec.changeSector( 5, Sector( Degree( 90 ), Degree( 110 ) ) );
  ASSERT_FALSE( res );
  sec.getVector( resVect );
  expectedVect = {
    Sector( Degree( 20 ), Degree( 40 ) ),
    Sector( Degree( 90 ), Degree( 110 ) ),
    Sector( Degree( 220 ), Degree( 240 ) ),
    Sector( Degree( 250 ), Degree( 270 ) )
  };
  ASSERT_EQ( resVect, expectedVect );

  // можно получить полный сектор
  res = sec.changeSector( 5, Sector( Degree( 235 ), Degree( 221 ) ) );
  ASSERT_TRUE( res );
  sec.getVector( resVect );
  expectedVect = { Sector( Degree( 0 ), Degree( 360 ) ) };
  ASSERT_EQ( resVect, expectedVect );
}
//==============================================================================
//==============================================================================
TEST_F( SectorsUnityTest, getVectorTest )
{
  // можно передать желаемый вектор как параметр
  SectorsUnity sec( { Sector( Degree( 30 ), Degree( 100 ) ) } );
  std::vector< Sector > resVect;
  sec.getVector( resVect );
  std::vector < Sector > expectedVect = 
    { Sector( Radian( Degree( 30 ) ), Radian( Degree( 100 ) ) ) };
  ASSERT_EQ( resVect, expectedVect );

  // можно ничего не передавать
  resVect.clear( );
  resVect = sec.getVector( );
  ASSERT_EQ( resVect, expectedVect );
}
//==============================================================================
//==============================================================================
TEST_F(SectorsUnityTest, getIntegerVectorTest)
{
  SectorsUnity sectors({ 
    Sector(Degree(30.5), Degree(100.5)),
    Sector(Degree(40.5), Degree(120.5)),
    Sector(Degree(150.5), Degree(240.5)),
  });
  
  std::vector <Sector> expectedSectors =
  { Sector((Degree(31)), (Degree(120))), 
    Sector((Degree(151)), (Degree(240))) };

  std::vector<Sector> sectorsInt = sectors.getIntegerVector();
  ASSERT_EQ(sectorsInt, expectedSectors);

  // сектора, которые должны объединиться только при целочисленных
  SectorsUnity sectors2({
    Sector(Degree(30.5), Degree(100.1)),
    Sector(Degree(100.9), Degree(120.5)),
    Sector(Degree(150.5), Degree(240.5)),
  });
 
  std::vector <Sector> expectedSectors2 =
  { Sector((Degree(31)), (Degree(120))),
    Sector((Degree(151)), (Degree(240))) };

  std::vector<Sector> sectorsInt2 = sectors2.getIntegerVector();
  ASSERT_EQ(sectorsInt2, expectedSectors2);

  // проверка сектора, который должен расшириться
  SectorsUnity sectors3({
    Sector(Degree(30.000000001), Degree(100.999999999)),
    Sector(Degree(111.5), Degree(120.5)),
    Sector(Degree(150.5), Degree(240.5)),
  });
  
  std::vector <Sector> expectedSectors3 =
  { Sector((Degree(30)), (Degree(101))),
    Sector((Degree(112)), (Degree(120))),
    Sector((Degree(151)), (Degree(240))) };

  std::vector<Sector> sectorsInt3 = sectors3.getIntegerVector();
  ASSERT_EQ(sectorsInt3, expectedSectors3);
}
//==============================================================================
//==============================================================================
} // ! Tests
} // ! Geometry

