#include <AngularUnits/ConvertFunctions.hpp>
//! BasicMath
#include <MathTools/BasicMath/Compare.hpp>
//! gtest
#include <gtest/gtest.h>
//==============================================================================
namespace Tests
{
//==============================================================================
/*!
 * \brief Тестирование fromDegreesToRadians (перевод из градусов в радианы)
*/
TEST( ConvertFunctionsTest, DegreeToRadian )
{
  using namespace BasicMath;

  ASSERT_TRUE( isEqual( 0., fromDegreesToRadians( 0. ) ) );
  ASSERT_TRUE( isEqual( M_PI / 4, fromDegreesToRadians( 45. ) ) );
  ASSERT_TRUE( isEqual( M_PI / 2, fromDegreesToRadians( 90. ) ) );
  ASSERT_TRUE( isEqual( M_PI, fromDegreesToRadians( 180. ) ) );
  ASSERT_TRUE( isEqual( M_PI * 2, fromDegreesToRadians( 360. ) ) );
  ASSERT_TRUE( isEqual( -M_PI / 4, fromDegreesToRadians( -45. ) ) );
  ASSERT_TRUE( isEqual( -M_PI / 2, fromDegreesToRadians( -90. ) ) );
  ASSERT_TRUE( isEqual( -M_PI, fromDegreesToRadians( -180. ) ) );
  ASSERT_TRUE( isEqual( -M_PI * 2, fromDegreesToRadians( -360. ) ) );
}
//==============================================================================
/*!
 * \brief Тестирование fromRadiansToDegrees (перевод из радиан в градусы)
*/
TEST( ConvertFunctionsTest, RadianToDegree )
{
  using namespace BasicMath;

  ASSERT_TRUE( isEqual( 15., fromRadiansToDegrees( M_PI / 12 ) ) );
  ASSERT_TRUE( isEqual( 30., fromRadiansToDegrees( M_PI / 6 ) ) );
  ASSERT_TRUE( isEqual( 45., fromRadiansToDegrees( M_PI / 4 ) ) );
  ASSERT_TRUE( isEqual( 90., fromRadiansToDegrees( M_PI / 2 ) ) );
  ASSERT_TRUE( isEqual( 180., fromRadiansToDegrees( M_PI ) ) );
  ASSERT_TRUE( isEqual( 360., fromRadiansToDegrees( M_PI * 2 ) ) );
  ASSERT_TRUE( isEqual( -15., fromRadiansToDegrees( -M_PI / 12 ) ) );
  ASSERT_TRUE( isEqual( -30., fromRadiansToDegrees( -M_PI / 6 ) ) );
  ASSERT_TRUE( isEqual( -45., fromRadiansToDegrees( -M_PI / 4 ) ) );
  ASSERT_TRUE( isEqual( -90., fromRadiansToDegrees( -M_PI / 2 ) ) );
  ASSERT_TRUE( isEqual( -180., fromRadiansToDegrees( -M_PI ) ) );
  ASSERT_TRUE( isEqual( -360., fromRadiansToDegrees( -M_PI * 2 ) ) );
}
//==============================================================================
/*!
 * \brief Тестирование intToString (преобразование целого числа в строку)
*/
TEST( ConvertFunctionsTest, IntToString )
{
  ASSERT_EQ( std::string( "000" ), intToString( 0, 3 ) );
  ASSERT_EQ( std::string( "010" ), intToString( 10, 3 ) );
  ASSERT_EQ( std::string( "100" ), intToString( 100, 1 ) );
  ASSERT_EQ( std::string( "-01" ), intToString( -1, 2 ) );
  ASSERT_EQ( std::string( "-1000" ), intToString( -1000, 1 ) );
}
//==============================================================================
/*!
 * \brief Тестирование doubleToString
 *        (преобразование вещественного числа в строку)
*/
TEST( ConvertFunctionsTest, DoubleToString )
{
  ASSERT_EQ( std::string( "00.00" ), doubleToString( 0, 2, 2, "." ) );
  ASSERT_EQ( std::string( "0.02" ), doubleToString( .02, 0, 2, "." ) );
  ASSERT_EQ( std::string( "01.5" ), doubleToString( 1.46, 2, 1, "." ) );
  ASSERT_EQ( std::string( "02" ), doubleToString( 1.56, 2, 0, "." ) );
  ASSERT_EQ( std::string( "00,00" ), doubleToString( 0, 2, 2, "," ) );
  ASSERT_EQ( std::string( "-01.5" ), doubleToString( -1.46, 2, 1, "." ) );
  ASSERT_EQ( std::string( "-01.10" ), doubleToString( -1.1, 2, 2, "." ) );
  ASSERT_EQ( std::string( "-10.1" ), doubleToString( -10.111, 1, 1, "." ) );
}
//==============================================================================
/*!
 * \brief Тестирование fromDegreesToDegreeString
 *        (преобразование вещественного числа градусов в строку)
*/
TEST( ConvertFunctionsTest, DegreesToDegreeString )
{
  //! "01°30'30.00"С.Ш."
  std::string degreeString( std::string( "01" ) + "\xB0"
    + std::string( "30" ) + "\x27"
    + std::string( "30.00" ) + "\x22"
    + "\xD1.\xD8." );
  double degreeValue( 1. + 30. / DEGREE_RATIO
    + 30. / ( DEGREE_RATIO * DEGREE_RATIO ) );
  ASSERT_EQ( degreeString, fromDegreesToDegreeString( degreeValue, LATITUDE ) );

  //! "10°35'40.43"Ю.Ш."
  degreeString = std::string( "10" ) + "\xB0"
    + std::string( "35" ) + "\x27"
    + std::string( "40.43" ) + "\x22"
    + "\xDE.\xD8.";
  degreeValue = -10. - 35. / DEGREE_RATIO
    - 40.43 / ( DEGREE_RATIO * DEGREE_RATIO );
  ASSERT_EQ( degreeString, fromDegreesToDegreeString( degreeValue, LATITUDE ) );

  //! "00°00'00.00"С.Ш."
  degreeString = std::string( "00" ) + "\xB0"
    + std::string( "00" ) + "\x27"
    + std::string( "00.00" ) + "\x22"
    + "\xD1.\xD8.";
  degreeValue = 0.;
  ASSERT_EQ( degreeString, fromDegreesToDegreeString( degreeValue, LATITUDE ) );

  //! "01°02'03"С.Ш."
  degreeString = std::string( "01" ) + "\xB0"
    + std::string( "02" ) + "\x27"
    + std::string( "03" ) + "\x22"
    + "\xD1.\xD8.";
  degreeValue = 1. + 2. / DEGREE_RATIO + 3. / ( DEGREE_RATIO * DEGREE_RATIO );
  ASSERT_EQ( degreeString, fromDegreesToDegreeString( degreeValue, LATITUDE, 0 ) );

  //! "1024°02'03.00"С.Ш."
  degreeString = std::string( "1024" ) + "\xB0"
    + std::string( "02" ) + "\x27"
    + std::string( "03.00" ) + "\x22"
    + "\xD1.\xD8.";
  degreeValue = 1024. + 2. / DEGREE_RATIO + 3. / ( DEGREE_RATIO * DEGREE_RATIO );
  ASSERT_EQ( degreeString, fromDegreesToDegreeString( degreeValue, LATITUDE ) );

  //! "01°02'03,010"С.Ш."
  degreeString = std::string( "01" ) + "\xB0"
    + std::string( "02" ) + "\x27"
    + std::string( "03,010" ) + "\x22"
    + "\xD1.\xD8.";
  degreeValue = 1. + 2. / DEGREE_RATIO + 3.01 / ( DEGREE_RATIO * DEGREE_RATIO );
  ASSERT_EQ( degreeString,
    fromDegreesToDegreeString( degreeValue, LATITUDE, 3, "," ) );

  //! "000°00'00.00"В.Д."
  degreeString = std::string( "000" ) + "\xB0"
    + std::string( "00" ) + "\x27"
    + std::string( "00.00" ) + "\x22"
    + "\xC2.\xC4.";
  degreeValue = 0.;
  ASSERT_EQ( degreeString, fromDegreesToDegreeString( degreeValue, LONGITUDE ) );

  //! "179°59'59.99"З.Д."
  degreeString = std::string( "179" ) + "\xB0"
    + std::string( "59" ) + "\x27"
    + std::string( "59.99" ) + "\x22"
    + "\xC7.\xC4.";
  degreeValue = -179. - 59. / DEGREE_RATIO
    - 59.99 / ( DEGREE_RATIO * DEGREE_RATIO );
  ASSERT_EQ( degreeString, fromDegreesToDegreeString( degreeValue, LONGITUDE ) );
}
//==============================================================================
/*!
 * \brief Тестирование fromRadiansToDegreeString
 *        (преобразование вещественного числа радиан в строку)
*/
TEST( ConvertFunctionsTest, RadiansToDegreeString )
{
  //! "01°30'30.00"С.Ш."
  std::string degreeString( std::string( "01" ) + "\xB0"
    + std::string( "30" ) + "\x27"
    + std::string( "30.00" ) + "\x22"
    + "\xD1.\xD8." );
  double radianValue(
    fromDegreesToRadians( 1. + 30. / DEGREE_RATIO
      + 30. / ( DEGREE_RATIO * DEGREE_RATIO ) ) );
  ASSERT_EQ( degreeString, fromRadiansToDegreeString( radianValue, LATITUDE ) );

  //! "10°35'40.43"Ю.Ш."
  degreeString = std::string( "10" ) + "\xB0"
    + std::string( "35" ) + "\x27"
    + std::string( "40.43" ) + "\x22"
    + "\xDE.\xD8.";
  radianValue = fromDegreesToRadians( -10. - 35. / DEGREE_RATIO
    - 40.43 / ( DEGREE_RATIO * DEGREE_RATIO ) );
  ASSERT_EQ( degreeString, fromRadiansToDegreeString( radianValue, LATITUDE ) );

  //! "00°00'00.00"С.Ш."
  degreeString = std::string( "00" ) + "\xB0"
    + std::string( "00" ) + "\x27"
    + std::string( "00.00" ) + "\x22"
    + "\xD1.\xD8.";
  radianValue = fromDegreesToRadians( 0. );
  ASSERT_EQ( degreeString, fromRadiansToDegreeString( radianValue, LATITUDE ) );

  //! "01°02'03"С.Ш."
  degreeString = std::string( "01" ) + "\xB0"
    + std::string( "02" ) + "\x27"
    + std::string( "03" ) + "\x22"
    + "\xD1.\xD8.";
  radianValue = fromDegreesToRadians( 1. + 2. / DEGREE_RATIO
    + 3. / ( DEGREE_RATIO * DEGREE_RATIO ) );
  ASSERT_EQ( degreeString, fromRadiansToDegreeString( radianValue, LATITUDE, 0 ) );

  //! "1024°02'03.00"С.Ш."
  degreeString = std::string( "1024" ) + "\xB0"
    + std::string( "02" ) + "\x27"
    + std::string( "03.00" ) + "\x22"
    + "\xD1.\xD8.";
  radianValue = fromDegreesToRadians( 1024. + 2. / DEGREE_RATIO
    + 3. / ( DEGREE_RATIO * DEGREE_RATIO ) );
  ASSERT_EQ( degreeString, fromRadiansToDegreeString( radianValue, LATITUDE ) );

  //! "01°02'03,010"С.Ш."
  degreeString = std::string( "01" ) + "\xB0"
    + std::string( "02" ) + "\x27"
    + std::string( "03,010" ) + "\x22"
    + "\xD1.\xD8.";
  radianValue = fromDegreesToRadians( 1. + 2. / DEGREE_RATIO
    + 3.01 / ( DEGREE_RATIO * DEGREE_RATIO ) );
  ASSERT_EQ( degreeString,
    fromRadiansToDegreeString( radianValue, LATITUDE, 3, "," ) );

  //! "000°00'00.00"В.Д."
  degreeString = std::string( "000" ) + "\xB0"
    + std::string( "00" ) + "\x27"
    + std::string( "00.00" ) + "\x22"
    + "\xC2.\xC4.";
  radianValue = fromDegreesToRadians( 0. );
  ASSERT_EQ( degreeString, fromRadiansToDegreeString( radianValue, LONGITUDE ) );

  //! "179°59'59.99"З.Д."
  degreeString = std::string( "179" ) + "\xB0"
    + std::string( "59" ) + "\x27"
    + std::string( "59.99" ) + "\x22"
    + "\xC7.\xC4.";
  radianValue = fromDegreesToRadians( -179. - 59. / DEGREE_RATIO
    - 59.99 / ( DEGREE_RATIO * DEGREE_RATIO ) );
  ASSERT_EQ( degreeString, fromRadiansToDegreeString( radianValue, LONGITUDE ) );
}
//==============================================================================
/*!
 * \brief Тестирование fromDegreesStringToDegrees
 *        (преобразование строки градусов в вещественное число градусов)
*/
TEST( ConvertFunctionsTest, DegreesStringToDegree )
{
  using namespace BasicMath;

  double degreeResult;
  //! "01°30'30.00"С.Ш."
  std::string degreeString( std::string( "01" ) + "\xB0"
    + std::string( "30" ) + "\x27"
    + std::string( "30.00" ) + "\x22"
    + "\xD1.\xD8." );
  double degreeValue( 1. + 30. / DEGREE_RATIO
    + 30. / ( DEGREE_RATIO * DEGREE_RATIO ) );
  ASSERT_TRUE( fromDegreesStringToDegrees( degreeString, degreeResult, true ) );
  ASSERT_TRUE( isEqual( degreeValue, degreeResult ) );
  ASSERT_TRUE( fromDegreesStringToDegrees( degreeString, degreeResult ) );
  ASSERT_TRUE( isEqual( degreeValue, degreeResult ) );

  //! "10°35'40.43"Ю.Ш."
  degreeString = std::string( "10" ) + "\xB0"
    + std::string( "35" ) + "\x27"
    + std::string( "40.43" ) + "\x22"
    + "\xDE.\xD8.";
  degreeValue = -10. - 35. / DEGREE_RATIO
    - 40.43 / ( DEGREE_RATIO * DEGREE_RATIO );
  ASSERT_TRUE( fromDegreesStringToDegrees( degreeString, degreeResult, true ) );
  ASSERT_TRUE( isEqual( degreeValue, degreeResult ) );
  ASSERT_TRUE( fromDegreesStringToDegrees( degreeString, degreeResult ) );
  ASSERT_TRUE( isEqual( degreeValue, degreeResult ) );

  //! "00°00'00.00"С.Ш."
  degreeString = std::string( "00" ) + "\xB0"
    + std::string( "00" ) + "\x27"
    + std::string( "00.00" ) + "\x22"
    + "\xD1.\xD8.";
  degreeValue = 0.;
  ASSERT_TRUE( fromDegreesStringToDegrees( degreeString, degreeResult, true ) );
  ASSERT_TRUE( isEqual( degreeValue, degreeResult ) );
  ASSERT_TRUE( fromDegreesStringToDegrees( degreeString, degreeResult ) );
  ASSERT_TRUE( isEqual( degreeValue, degreeResult ) );

  //! "01°02'03"С.Ш."
  degreeString = std::string( "01" ) + "\xB0"
    + std::string( "02" ) + "\x27"
    + std::string( "03" ) + "\x22"
    + "\xD1.\xD8.";
  degreeValue = 1. + 2. / DEGREE_RATIO + 3. / ( DEGREE_RATIO * DEGREE_RATIO );
  ASSERT_TRUE( fromDegreesStringToDegrees( degreeString, degreeResult, true ) );
  ASSERT_TRUE( isEqual( degreeValue, degreeResult ) );
  ASSERT_TRUE( fromDegreesStringToDegrees( degreeString, degreeResult ) );
  ASSERT_TRUE( isEqual( degreeValue, degreeResult ) );

  //! "1024°02'03.00"С.Ш."
  degreeString = std::string( "1024" ) + "\xB0"
    + std::string( "02" ) + "\x27"
    + std::string( "03.00" ) + "\x22"
    + "\xD1.\xD8.";
  degreeValue = 1024. + 2. / DEGREE_RATIO + 3. / ( DEGREE_RATIO * DEGREE_RATIO );
  ASSERT_TRUE( fromDegreesStringToDegrees( degreeString, degreeResult, true ) );
  ASSERT_TRUE( isEqual( degreeValue, degreeResult ) );
  ASSERT_TRUE( fromDegreesStringToDegrees( degreeString, degreeResult ) );
  ASSERT_TRUE( isEqual( degreeValue, degreeResult ) );

  //! "01°02'03,010"С.Ш."
  degreeString = std::string( "01" ) + "\xB0"
    + std::string( "02" ) + "\x27"
    + std::string( "03,010" ) + "\x22"
    + "\xD1.\xD8.";
  degreeValue = 1. + 2. / DEGREE_RATIO + 3.01 / ( DEGREE_RATIO * DEGREE_RATIO );
  ASSERT_TRUE( fromDegreesStringToDegrees( degreeString, degreeResult, true ) );
  ASSERT_TRUE( isEqual( degreeValue, degreeResult ) );
  ASSERT_TRUE( fromDegreesStringToDegrees( degreeString, degreeResult ) );
  ASSERT_TRUE( isEqual( degreeValue, degreeResult ) );

  //! "000°00'00.00"В.Д."
  degreeString = std::string( "000" ) + "\xB0"
    + std::string( "00" ) + "\x27"
    + std::string( "00.00" ) + "\x22"
    + "\xC2.\xC4.";
  degreeValue = 0.;
  ASSERT_TRUE( fromDegreesStringToDegrees( degreeString, degreeResult, true ) );
  ASSERT_TRUE( isEqual( degreeValue, degreeResult ) );
  ASSERT_TRUE( fromDegreesStringToDegrees( degreeString, degreeResult ) );
  ASSERT_TRUE( isEqual( degreeValue, degreeResult ) );

  //! "179°59'59.99"З.Д."
  degreeString = std::string( "179" ) + "\xB0"
    + std::string( "59" ) + "\x27"
    + std::string( "59.99" ) + "\x22"
    + "\xC7.\xC4.";
  degreeValue = -179. - 59. / DEGREE_RATIO
    - 59.99 / ( DEGREE_RATIO * DEGREE_RATIO );
  ASSERT_TRUE( fromDegreesStringToDegrees( degreeString, degreeResult, true ) );
  ASSERT_TRUE( isEqual( degreeValue, degreeResult ) );
  ASSERT_TRUE( fromDegreesStringToDegrees( degreeString, degreeResult ) );
  ASSERT_TRUE( isEqual( degreeValue, degreeResult ) );

  if( std::string( localeconv( )->decimal_point ) == "," )
  {
    //! "17.9°59'59.99"З.Д."
    degreeString = std::string( "17.9" ) + "\xB0"
      + std::string( "59" ) + "\x27"
      + std::string( "59.99" ) + "\x22"
      + "\xC7.\xC4.";
    ASSERT_FALSE( fromDegreesStringToDegrees( degreeString, degreeResult, true ) );
    ASSERT_TRUE( isEqual( degreeValue, degreeResult ) );
    degreeValue = -17. - 59. / DEGREE_RATIO
      - 59.99 / ( DEGREE_RATIO * DEGREE_RATIO );
    ASSERT_TRUE( fromDegreesStringToDegrees( degreeString, degreeResult ) );
    ASSERT_TRUE( isEqual( degreeValue, degreeResult ) );

    //! "17,9°59'59.99"З.Д."
    degreeString = std::string( "17,9" ) + "\xB0"
      + std::string( "59" ) + "\x27"
      + std::string( "59.99" ) + "\x22"
      + "\xC7.\xC4.";
    ASSERT_FALSE( fromDegreesStringToDegrees( degreeString, degreeResult, true ) );
    ASSERT_TRUE( isEqual( degreeValue, degreeResult ) );
    degreeValue = -17.9 - 59. / DEGREE_RATIO
      - 59.99 / ( DEGREE_RATIO * DEGREE_RATIO );
    ASSERT_TRUE( fromDegreesStringToDegrees( degreeString, degreeResult ) );
    ASSERT_TRUE( isEqual( degreeValue, degreeResult ) );
  }
  else
  {
    //! "17,9°59'59.99"З.Д."
    degreeString = std::string( "17,9" ) + "\xB0"
      + std::string( "59" ) + "\x27"
      + std::string( "59.99" ) + "\x22"
      + "\xC7.\xC4.";
    ASSERT_FALSE( fromDegreesStringToDegrees( degreeString, degreeResult, true ) );
    ASSERT_TRUE( isEqual( degreeValue, degreeResult ) );
    degreeValue = -17. - 59. / DEGREE_RATIO
      - 59.99 / ( DEGREE_RATIO * DEGREE_RATIO );
    ASSERT_TRUE( fromDegreesStringToDegrees( degreeString, degreeResult ) );
    ASSERT_TRUE( isEqual( degreeValue, degreeResult ) );

    //! "17.9°59'59.99"З.Д."
    degreeString = std::string( "17.9" ) + "\xB0"
      + std::string( "59" ) + "\x27"
      + std::string( "59.99" ) + "\x22"
      + "\xC7.\xC4.";
    ASSERT_FALSE( fromDegreesStringToDegrees( degreeString, degreeResult, true ) );
    ASSERT_TRUE( isEqual( degreeValue, degreeResult ) );
    degreeValue = -17.9 - 59. / DEGREE_RATIO
      - 59.99 / ( DEGREE_RATIO * DEGREE_RATIO );
    ASSERT_TRUE( fromDegreesStringToDegrees( degreeString, degreeResult ) );
    ASSERT_TRUE( isEqual( degreeValue, degreeResult ) );
  }

  //! "179'59°59.99"З.Д."
  degreeString = std::string( "179" ) + "\xB0"
    + std::string( "59" ) + "\x22"
    + std::string( "59.99" ) + "\x27"
    + "\xC7.\xC4.";
  ASSERT_FALSE( fromDegreesStringToDegrees( degreeString, degreeResult, true ) );
  ASSERT_TRUE( isEqual( degreeValue, degreeResult ) );
  ASSERT_FALSE( fromDegreesStringToDegrees( degreeString, degreeResult ) );
  ASSERT_TRUE( isEqual( degreeValue, degreeResult ) );

  //! "179°59'59.99"И.Д."
  degreeString = std::string( "179" ) + "\xB0"
    + std::string( "59" ) + "\x27"
    + std::string( "59.99" ) + "\x22"
    + "\xC8.\xC4.";
  ASSERT_FALSE( fromDegreesStringToDegrees( degreeString, degreeResult, true ) );
  ASSERT_TRUE( isEqual( degreeValue, degreeResult ) );
  degreeValue = 179. + 59. / DEGREE_RATIO
    + 59.99 / ( DEGREE_RATIO * DEGREE_RATIO );
  ASSERT_TRUE( fromDegreesStringToDegrees( degreeString, degreeResult ) );
  ASSERT_TRUE( isEqual( degreeValue, degreeResult ) );

  //! "DDD°MM'SS.SS"В.Д."
  degreeString = std::string( "DDD" ) + "\xB0"
    + std::string( "MM" ) + "\x27"
    + std::string( "SS.SS" ) + "\x22"
    + "\xC7.\xC4.";
  ASSERT_FALSE( fromDegreesStringToDegrees( degreeString, degreeResult, true ) );
  ASSERT_TRUE( isEqual( degreeValue, degreeResult ) );
  ASSERT_FALSE( fromDegreesStringToDegrees( degreeString, degreeResult ) );
  ASSERT_TRUE( isEqual( degreeValue, degreeResult ) );
}
//==============================================================================
/*!
* \brief Тестирование fromDegreesStringToRadians
*        (преобразование строки градусов в вещественное число радиан)
*/
TEST( ConvertFunctionsTest, DegreesStringToRadians )
{
  using namespace BasicMath;

  double radianResult;
  //! "01°30'30.00"С.Ш."
  std::string degreeString( std::string( "01" ) + "\xB0"
    + std::string( "30" ) + "\x27"
    + std::string( "30.00" ) + "\x22"
    + "\xD1.\xD8." );
  double radianValue(
    fromDegreesToRadians( 1. + 30. / DEGREE_RATIO
      + 30. / ( DEGREE_RATIO * DEGREE_RATIO ) ) );
  ASSERT_TRUE( fromDegreesStringToRadians( degreeString, radianResult, true ) );
  ASSERT_TRUE( isEqual( radianValue, radianResult ) );
  ASSERT_TRUE( fromDegreesStringToRadians( degreeString, radianResult ) );
  ASSERT_TRUE( isEqual( radianValue, radianResult ) );

  //! "10°35'40.43"Ю.Ш."
  degreeString = std::string( "10" ) + "\xB0"
    + std::string( "35" ) + "\x27"
    + std::string( "40.43" ) + "\x22"
    + "\xDE.\xD8.";
  radianValue = fromDegreesToRadians( -10. - 35. / DEGREE_RATIO
    - 40.43 / ( DEGREE_RATIO * DEGREE_RATIO ) );
  ASSERT_TRUE( fromDegreesStringToRadians( degreeString, radianResult, true ) );
  ASSERT_TRUE( isEqual( radianValue, radianResult ) );
  ASSERT_TRUE( fromDegreesStringToRadians( degreeString, radianResult ) );
  ASSERT_TRUE( isEqual( radianValue, radianResult ) );

  //! "00°00'00.00"С.Ш."
  degreeString = std::string( "00" ) + "\xB0"
    + std::string( "00" ) + "\x27"
    + std::string( "00.00" ) + "\x22"
    + "\xD1.\xD8.";
  radianValue = fromDegreesToRadians( 0. );
  ASSERT_TRUE( fromDegreesStringToRadians( degreeString, radianResult, true ) );
  ASSERT_TRUE( isEqual( radianValue, radianResult ) );
  ASSERT_TRUE( fromDegreesStringToRadians( degreeString, radianResult ) );
  ASSERT_TRUE( isEqual( radianValue, radianResult ) );

  //! "01°02'03"С.Ш."
  degreeString = std::string( "01" ) + "\xB0"
    + std::string( "02" ) + "\x27"
    + std::string( "03" ) + "\x22"
    + "\xD1.\xD8.";
  radianValue = fromDegreesToRadians( 1. + 2. / DEGREE_RATIO
    + 3. / ( DEGREE_RATIO * DEGREE_RATIO ) );
  ASSERT_TRUE( fromDegreesStringToRadians( degreeString, radianResult, true ) );
  ASSERT_TRUE( isEqual( radianValue, radianResult ) );
  ASSERT_TRUE( fromDegreesStringToRadians( degreeString, radianResult ) );
  ASSERT_TRUE( isEqual( radianValue, radianResult ) );

  //! "1024°02'03.00"С.Ш."
  degreeString = std::string( "1024" ) + "\xB0"
    + std::string( "02" ) + "\x27"
    + std::string( "03.00" ) + "\x22"
    + "\xD1.\xD8.";
  radianValue = fromDegreesToRadians( 1024. + 2. / DEGREE_RATIO
    + 3. / ( DEGREE_RATIO * DEGREE_RATIO ) );
  ASSERT_TRUE( fromDegreesStringToRadians( degreeString, radianResult, true ) );
  ASSERT_TRUE( isEqual( radianValue, radianResult ) );
  ASSERT_TRUE( fromDegreesStringToRadians( degreeString, radianResult ) );
  ASSERT_TRUE( isEqual( radianValue, radianResult ) );

  //! "01°02'03,010"С.Ш."
  degreeString = std::string( "01" ) + "\xB0"
    + std::string( "02" ) + "\x27"
    + std::string( "03,010" ) + "\x22"
    + "\xD1.\xD8.";
  radianValue = fromDegreesToRadians( 1. + 2. / DEGREE_RATIO
    + 3.01 / ( DEGREE_RATIO * DEGREE_RATIO ) );
  ASSERT_TRUE( fromDegreesStringToRadians( degreeString, radianResult, true ) );
  ASSERT_TRUE( isEqual( radianValue, radianResult ) );
  ASSERT_TRUE( fromDegreesStringToRadians( degreeString, radianResult ) );
  ASSERT_TRUE( isEqual( radianValue, radianResult ) );

  //! "000°00'00.00"В.Д."
  degreeString = std::string( "000" ) + "\xB0"
    + std::string( "00" ) + "\x27"
    + std::string( "00.00" ) + "\x22"
    + "\xC2.\xC4.";
  radianValue = fromDegreesToRadians( 0. );
  ASSERT_TRUE( fromDegreesStringToRadians( degreeString, radianResult, true ) );
  ASSERT_TRUE( isEqual( radianValue, radianResult ) );
  ASSERT_TRUE( fromDegreesStringToRadians( degreeString, radianResult ) );
  ASSERT_TRUE( isEqual( radianValue, radianResult ) );

  //! "179°59'59.99"З.Д."
  degreeString = std::string( "179" ) + "\xB0"
    + std::string( "59" ) + "\x27"
    + std::string( "59.99" ) + "\x22"
    + "\xC7.\xC4.";
  radianValue = fromDegreesToRadians( -179. - 59. / DEGREE_RATIO
    - 59.99 / ( DEGREE_RATIO * DEGREE_RATIO ) );
  ASSERT_TRUE( fromDegreesStringToRadians( degreeString, radianResult, true ) );
  ASSERT_TRUE( isEqual( radianValue, radianResult ) );
  ASSERT_TRUE( fromDegreesStringToRadians( degreeString, radianResult ) );
  ASSERT_TRUE( isEqual( radianValue, radianResult ) );

  if( std::string( localeconv( )->decimal_point ) == "," )
  {
    //! "17.9°59'59.99"З.Д."
    degreeString = std::string( "17.9" ) + "\xB0"
      + std::string( "59" ) + "\x27"
      + std::string( "59.99" ) + "\x22"
      + "\xC7.\xC4.";
    ASSERT_FALSE( fromDegreesStringToRadians( degreeString, radianResult, true ) );
    ASSERT_TRUE( isEqual( radianValue, radianResult ) );
    radianValue = fromDegreesToRadians( -17. - 59. / DEGREE_RATIO
      - 59.99 / ( DEGREE_RATIO * DEGREE_RATIO ) );
    ASSERT_TRUE( fromDegreesStringToRadians( degreeString, radianResult ) );
    ASSERT_TRUE( isEqual( radianValue, radianResult ) );

    //! "17,9°59'59.99"З.Д."
    degreeString = std::string( "17,9" ) + "\xB0"
      + std::string( "59" ) + "\x27"
      + std::string( "59.99" ) + "\x22"
      + "\xC7.\xC4.";
    ASSERT_FALSE( fromDegreesStringToRadians( degreeString, radianResult, true ) );
    ASSERT_TRUE( isEqual( radianValue, radianResult ) );
    radianValue = fromDegreesToRadians( -17.9 - 59. / DEGREE_RATIO
      - 59.99 / ( DEGREE_RATIO * DEGREE_RATIO ) );
    ASSERT_TRUE( fromDegreesStringToRadians( degreeString, radianResult ) );
    ASSERT_TRUE( isEqual( radianValue, radianResult ) );
  }
  else
  {
    //! "17,9°59'59.99"З.Д."
    degreeString = std::string( "17,9" ) + "\xB0"
      + std::string( "59" ) + "\x27"
      + std::string( "59.99" ) + "\x22"
      + "\xC7.\xC4.";
    ASSERT_FALSE( fromDegreesStringToRadians( degreeString, radianResult, true ) );
    ASSERT_TRUE( isEqual( radianValue, radianResult ) );
    radianValue = fromDegreesToRadians( -17. - 59. / DEGREE_RATIO
      - 59.99 / ( DEGREE_RATIO * DEGREE_RATIO ) );
    ASSERT_TRUE( fromDegreesStringToRadians( degreeString, radianResult ) );
    ASSERT_TRUE( isEqual( radianValue, radianResult ) );

    //! "17.9°59'59.99"З.Д."
    degreeString = std::string( "17.9" ) + "\xB0"
      + std::string( "59" ) + "\x27"
      + std::string( "59.99" ) + "\x22"
      + "\xC7.\xC4.";
    ASSERT_FALSE( fromDegreesStringToRadians( degreeString, radianResult, true ) );
    ASSERT_TRUE( isEqual( radianValue, radianResult ) );
    radianValue = fromDegreesToRadians( -17.9 - 59. / DEGREE_RATIO
      - 59.99 / ( DEGREE_RATIO * DEGREE_RATIO ) );
    ASSERT_TRUE( fromDegreesStringToRadians( degreeString, radianResult ) );
    ASSERT_TRUE( isEqual( radianValue, radianResult ) );
  }

  //! "179'59°59.99"З.Д."
  degreeString = std::string( "179" ) + "\xB0"
    + std::string( "59" ) + "\x22"
    + std::string( "59.99" ) + "\x27"
    + "\xC7.\xC4.";
  ASSERT_FALSE( fromDegreesStringToRadians( degreeString, radianResult, true ) );
  ASSERT_TRUE( isEqual( radianValue, radianResult ) );
  ASSERT_FALSE( fromDegreesStringToRadians( degreeString, radianResult ) );
  ASSERT_TRUE( isEqual( radianValue, radianResult ) );

  //! "179°59'59.99"И.Д."
  degreeString = std::string( "179" ) + "\xB0"
    + std::string( "59" ) + "\x27"
    + std::string( "59.99" ) + "\x22"
    + "\xC8.\xC4.";
  ASSERT_FALSE( fromDegreesStringToRadians( degreeString, radianResult, true ) );
  ASSERT_TRUE( isEqual( radianValue, radianResult ) );
  radianValue = fromDegreesToRadians( 179. + 59. / DEGREE_RATIO
    + 59.99 / ( DEGREE_RATIO * DEGREE_RATIO ) );
  ASSERT_TRUE( fromDegreesStringToRadians( degreeString, radianResult ) );
  ASSERT_TRUE( isEqual( radianValue, radianResult ) );

  //! "DDD°MM'SS.SS"В.Д."
  degreeString = std::string( "DDD" ) + "\xB0"
    + std::string( "MM" ) + "\x27"
    + std::string( "SS.SS" ) + "\x22"
    + "\xC7.\xC4.";
  ASSERT_FALSE( fromDegreesStringToRadians( degreeString, radianResult, true ) );
  ASSERT_TRUE( isEqual( radianValue, radianResult ) );
  ASSERT_FALSE( fromDegreesStringToRadians( degreeString, radianResult ) );
  ASSERT_TRUE( isEqual( radianValue, radianResult ) );
}
//==============================================================================
} //! Tests
//==============================================================================

