#include <MathTools/AngularUnits/Radian.hpp>
//! AngularUnits
#include <MathTools/AngularUnits/Degree.hpp>
//! BasicMath
#include <MathTools/BasicMath/Compare.hpp>
//! gtest
#include <gtest/gtest.h>
//==============================================================================
namespace Tests
{
//==============================================================================
constexpr double OUTSIDE_INACCURACY       //!< ошибка за границами точности для 
= ACCURACY_DOUBLE / 10;  //!< double 
constexpr double INSIDE_INACCURACY        //!< ошибка в границах точности для 
= ACCURACY_DOUBLE * 10;  //!< double
//==============================================================================
/*!
 * \brief Тестирование Radian
 *        (конструктора на оcнове double)
*/
TEST( RadiansTest, ConstructorRadianFromDouble )
{
  using namespace BasicMath;

  ASSERT_TRUE( isEqual( M_PI / 12, Radian( M_PI / 12 ).get( ) ) );
  ASSERT_TRUE( isEqual( M_PI / 6, Radian( M_PI / 6 ).get( ) ) );
  ASSERT_TRUE( isEqual( M_PI / 4, Radian( M_PI / 4 ).get( ) ) );
  ASSERT_TRUE( isEqual( M_PI / 2, Radian( M_PI / 2 ).get( ) ) );
  ASSERT_TRUE( isEqual( M_PI, Radian( M_PI ).get( ) ) );
  ASSERT_TRUE( isEqual( M_PI * 2, Radian( M_PI * 2 ).get( ) ) );
  ASSERT_TRUE( isEqual( -M_PI / 12, Radian( -M_PI / 12 ).get( ) ) );
  ASSERT_TRUE( isEqual( -M_PI / 6, Radian( -M_PI / 6 ).get( ) ) );
  ASSERT_TRUE( isEqual( -M_PI / 4, Radian( -M_PI / 4 ).get( ) ) );
  ASSERT_TRUE( isEqual( -M_PI / 2, Radian( -M_PI / 2 ).get( ) ) );
  ASSERT_TRUE( isEqual( -M_PI, Radian( -M_PI ).get( ) ) );
  ASSERT_TRUE( isEqual( -M_PI * 2, Radian( -M_PI * 2 ).get( ) ) );
  ASSERT_TRUE( isEqual( 0., Radian( 0. ).get( ) ) );
  ASSERT_TRUE( isEqual( 0.25 * RADIAN_PER_DEGREE,
    Radian( 0.25 * RADIAN_PER_DEGREE ).get( ) ) );
  ASSERT_TRUE( isEqual( -0.1 * RADIAN_PER_DEGREE,
    Radian( -0.1 * RADIAN_PER_DEGREE ).get( ) ) );
}
//==============================================================================
/*!
* \brief Тестирование Radian
*        (конструктора на оcнове градусов - Degree)
*/
TEST( RadiansTest, ConstructorRadianFromDegree )
{
  using namespace BasicMath;

  ASSERT_TRUE( isEqual( M_PI / 12, Radian( Degree( 15. ) ).get( ) ) );
  ASSERT_TRUE( isEqual( M_PI / 6, Radian( Degree( 30. ) ).get( ) ) );
  ASSERT_TRUE( isEqual( M_PI / 4, Radian( Degree( 45. ) ).get( ) ) );
  ASSERT_TRUE( isEqual( M_PI / 2, Radian( Degree( 90. ) ).get( ) ) );
  ASSERT_TRUE( isEqual( M_PI, Radian( Degree( 180. ) ).get( ) ) );
  ASSERT_TRUE( isEqual( M_PI * 2, Radian( Degree( 360. ) ).get( ) ) );
  ASSERT_TRUE( isEqual( -M_PI / 12, Radian( Degree( -15. ) ).get( ) ) );
  ASSERT_TRUE( isEqual( -M_PI / 6, Radian( Degree( -30. ) ).get( ) ) );
  ASSERT_TRUE( isEqual( -M_PI / 4, Radian( Degree( -45. ) ).get( ) ) );
  ASSERT_TRUE( isEqual( -M_PI / 2, Radian( Degree( -90. ) ).get( ) ) );
  ASSERT_TRUE( isEqual( -M_PI, Radian( Degree( -180. ) ).get( ) ) );
  ASSERT_TRUE( isEqual( -M_PI * 2, Radian( Degree( -360. ) ).get( ) ) );
  ASSERT_TRUE( isEqual( 0., Radian( Degree( 0. ) ).get( ) ) );
  ASSERT_TRUE( isEqual( 0.25 * RADIAN_PER_DEGREE, Radian( Degree( 0.25 ) ).get( ) ) );
  ASSERT_TRUE( isEqual( -0.1 * RADIAN_PER_DEGREE, Radian( Degree( -0.1 ) ).get( ) ) );
}
//==============================================================================
/*!
 * \brief Тестирование set и get
 *        (присвоение значения на основе double и получение)
*/
TEST( RadiansTest, RadianGetSet )
{
  using namespace BasicMath;

  Radian radians;
  radians.set( M_PI / 12 );
  ASSERT_TRUE( isEqual( M_PI / 12, radians.get( ) ) );
  radians.set( M_PI );
  ASSERT_TRUE( isEqual( M_PI, radians.get( ) ) );
  radians.set( M_PI * 2 );
  ASSERT_TRUE( isEqual( M_PI * 2, radians.get( ) ) );
  radians.set( -M_PI / 12 );
  ASSERT_TRUE( isEqual( -M_PI / 12, radians.get( ) ) );
  radians.set( -M_PI * 2 );
  ASSERT_TRUE( isEqual( -M_PI * 2, radians.get( ) ) );
  radians.set( 0. );
  ASSERT_TRUE( isEqual( 0., radians.get( ) ) );
  radians.set( 0.25 * RADIAN_PER_DEGREE );
  ASSERT_TRUE( isEqual( 0.25 * RADIAN_PER_DEGREE, radians.get( ) ) );
  radians.set( -0.1 * RADIAN_PER_DEGREE );
  ASSERT_TRUE( isEqual( -0.1 * RADIAN_PER_DEGREE, radians.get( ) ) );
}
//==============================================================================
/*!
 * \brief Тестирование toDegrees
 *        (перевод из радиан в градусы)
*/
TEST( RadiansTest, RadianToDegree )
{
  using namespace BasicMath;

  ASSERT_TRUE( isEqual( 15., Radian( M_PI / 12 ).toDegrees( ).get( ) ) );
  ASSERT_TRUE( isEqual( 90., Radian( M_PI / 2 ).toDegrees( ).get( ) ) );
  ASSERT_TRUE( isEqual( 180., Radian( M_PI ).toDegrees( ).get( ) ) );
  ASSERT_TRUE( isEqual( 360., Radian( M_PI * 2 ).toDegrees( ).get( ) ) );
  ASSERT_TRUE( isEqual( -15., Radian( -M_PI / 12 ).toDegrees( ).get( ) ) );
  ASSERT_TRUE( isEqual( -90., Radian( -M_PI / 2 ).toDegrees( ).get( ) ) );
  ASSERT_TRUE( isEqual( -180., Radian( -M_PI ).toDegrees( ).get( ) ) );
  ASSERT_TRUE( isEqual( -360., Radian( -M_PI * 2 ).toDegrees( ).get( ) ) );
  ASSERT_TRUE( isEqual( 0., Radian( 0. ).toDegrees( ).get( ) ) );
  ASSERT_TRUE( isEqual( .25, Radian( .25 * RADIAN_PER_DEGREE ).toDegrees( ).get( ) ) );
  ASSERT_TRUE( isEqual( -.1, Radian( -.1 * RADIAN_PER_DEGREE ).toDegrees( ).get( ) ) );
}
//==============================================================================
/*!
 * \brief Тестирование fromDegrees
 *        (присвоение значения на основе градусов - Degree)
*/
TEST( RadiansTest, RadianFromDegree )
{
  using namespace BasicMath;

  Radian radian;
  radian.fromDegrees( Degree( 15. ) );
  ASSERT_TRUE( isEqual( M_PI / 12, radian.get( ) ) );
  radian.fromDegrees( Degree( 180. ) );
  ASSERT_TRUE( isEqual( M_PI, radian.get( ) ) );
  radian.fromDegrees( Degree( 360. ) );
  ASSERT_TRUE( isEqual( M_PI * 2, radian.get( ) ) );
  radian.fromDegrees( Degree( -15. ) );
  ASSERT_TRUE( isEqual( -M_PI / 12, radian.get( ) ) );
  radian.fromDegrees( Degree( -360. ) );
  ASSERT_TRUE( isEqual( -M_PI * 2, radian.get( ) ) );
  radian.fromDegrees( Degree( 0. ) );
  ASSERT_TRUE( isEqual( 0., radian.get( ) ) );
  radian.fromDegrees( Degree( 0.25 ) );
  ASSERT_TRUE( isEqual( 0.25 * RADIAN_PER_DEGREE, radian.get( ) ) );
  radian.fromDegrees( Degree( -0.1 ) );
  ASSERT_TRUE( isEqual( -0.1 * RADIAN_PER_DEGREE, radian.get( ) ) );
}
//==============================================================================
/*!
 * \brief Тестирование toDegreesString
 *        (конвертирование радиан из вещественного числа в формализованную
 *        строку градусов)
*/
TEST( RadiansTest, RadianToDegreeString )
{
  //! "01°30'30.00"С.Ш."
  std::string degreeString( std::string( "01" ) + "\xB0"
    + std::string( "30" ) + "\x27"
    + std::string( "30.00" ) + "\x22"
    + "\xD1.\xD8." );
  Radian radian( ( 1. + 30. / DEGREE_RATIO + 30. / ( DEGREE_RATIO * DEGREE_RATIO ) )
    * RADIAN_PER_DEGREE );
  ASSERT_EQ( degreeString, radian.toDegreesString( LATITUDE ) );

  //! "10°35'40.43"Ю.Ш."
  degreeString = std::string( "10" ) + "\xB0"
    + std::string( "35" ) + "\x27"
    + std::string( "40.43" ) + "\x22"
    + "\xDE.\xD8.";
  radian.set( ( -10. - 35. / DEGREE_RATIO - 40.43 / ( DEGREE_RATIO * DEGREE_RATIO ) )
    * RADIAN_PER_DEGREE );
  ASSERT_EQ( degreeString, radian.toDegreesString( LATITUDE ) );

  //! "00°00'00.00"С.Ш."
  degreeString = std::string( "00" ) + "\xB0"
    + std::string( "00" ) + "\x27"
    + std::string( "00.00" ) + "\x22"
    + "\xD1.\xD8.";
  radian.set( 0. );
  ASSERT_EQ( degreeString, radian.toDegreesString( LATITUDE ) );

  //! "01°02'03"С.Ш."
  degreeString = std::string( "01" ) + "\xB0"
    + std::string( "02" ) + "\x27"
    + std::string( "03" ) + "\x22"
    + "\xD1.\xD8.";
  radian.set( ( 1. + 2. / DEGREE_RATIO + 3. / ( DEGREE_RATIO * DEGREE_RATIO ) )
    * RADIAN_PER_DEGREE );
  ASSERT_EQ( degreeString, radian.toDegreesString( LATITUDE, 0 ) );

  //! "1024°02'03.00"С.Ш."
  degreeString = std::string( "1024" ) + "\xB0"
    + std::string( "02" ) + "\x27"
    + std::string( "03.00" ) + "\x22"
    + "\xD1.\xD8.";
  radian.set( ( 1024. + 2. / DEGREE_RATIO + 3. / ( DEGREE_RATIO * DEGREE_RATIO ) )
    * RADIAN_PER_DEGREE );
  ASSERT_EQ( degreeString, radian.toDegreesString( LATITUDE ) );

  //! "01°02'03,010"С.Ш."
  degreeString = std::string( "01" ) + "\xB0"
    + std::string( "02" ) + "\x27"
    + std::string( "03,010" ) + "\x22"
    + "\xD1.\xD8.";
  radian.set( ( 1. + 2. / DEGREE_RATIO + 3.01 / ( DEGREE_RATIO * DEGREE_RATIO ) )
    * RADIAN_PER_DEGREE );
  ASSERT_EQ( degreeString, radian.toDegreesString( LATITUDE, 3, "," ) );

  //! "000°00'00.00"В.Д."
  degreeString = std::string( "000" ) + "\xB0"
    + std::string( "00" ) + "\x27"
    + std::string( "00.00" ) + "\x22"
    + "\xC2.\xC4.";
  radian.set( 0. );
  ASSERT_EQ( degreeString, radian.toDegreesString( LONGITUDE ) );

  //! "179°59'59.99"З.Д."
  degreeString = std::string( "179" ) + "\xB0"
    + std::string( "59" ) + "\x27"
    + std::string( "59.99" ) + "\x22"
    + "\xC7.\xC4.";
  radian.set( ( -179. - 59. / DEGREE_RATIO
    - 59.99 / ( DEGREE_RATIO * DEGREE_RATIO ) ) * RADIAN_PER_DEGREE );
  ASSERT_EQ( degreeString, radian.toDegreesString( LONGITUDE ) );
}
//==============================================================================
/*!
 * \brief Тестирование fromDegreesString
 *        (конвертирование градусов из формализованной строки в вещественное
 *        число радиан)
*/
TEST( RadiansTest, DegreeStringToRadian )
{
  using namespace BasicMath;

  Radian radians;

  //! "01°30'30.00"С.Ш."
  std::string degreeString( std::string( "01" ) + "\xB0"
    + std::string( "30" ) + "\x27"
    + std::string( "30.00" ) + "\x22"
    + "\xD1.\xD8." );
  ASSERT_TRUE( radians.fromDegreesString( degreeString ) );
  ASSERT_TRUE( isEqual( ( 1. + 30. / DEGREE_RATIO
    + 30. / ( DEGREE_RATIO * DEGREE_RATIO ) )
    * RADIAN_PER_DEGREE,
    radians.get( ) ) );

  //! "10°35'40.43"Ю.Ш."
  degreeString = std::string( "10" ) + "\xB0"
    + std::string( "35" ) + "\x27"
    + std::string( "40.43" ) + "\x22"
    + "\xDE.\xD8.";
  ASSERT_TRUE( radians.fromDegreesString( degreeString ) );
  ASSERT_TRUE( isEqual( ( -10. - 35. / DEGREE_RATIO
    - 40.43 / ( DEGREE_RATIO * DEGREE_RATIO ) )
    * RADIAN_PER_DEGREE,
    radians.get( ) ) );

  //! "00°00'00.00"С.Ш."
  degreeString = std::string( "00" ) + "\xB0"
    + std::string( "00" ) + "\x27"
    + std::string( "00.00" ) + "\x22"
    + "\xD1.\xD8.";
  ASSERT_TRUE( radians.fromDegreesString( degreeString ) );
  ASSERT_TRUE( isEqual( 0., radians.get( ) ) );

  //! "01°02'03"С.Ш."
  degreeString = std::string( "01" ) + "\xB0"
    + std::string( "02" ) + "\x27"
    + std::string( "03" ) + "\x22"
    + "\xD1.\xD8.";
  ASSERT_TRUE( radians.fromDegreesString( degreeString ) );
  ASSERT_TRUE( isEqual( ( 1. + 2. / DEGREE_RATIO
    + 3. / ( DEGREE_RATIO * DEGREE_RATIO ) )
    * RADIAN_PER_DEGREE,
    radians.get( ) ) );

  //! "1024°02'03.00"С.Ш."
  degreeString = std::string( "1024" ) + "\xB0"
    + std::string( "02" ) + "\x27"
    + std::string( "03.00" ) + "\x22"
    + "\xD1.\xD8.";
  ASSERT_TRUE( radians.fromDegreesString( degreeString ) );
  ASSERT_TRUE( isEqual( ( 1024. + 2. / DEGREE_RATIO
    + 3. / ( DEGREE_RATIO * DEGREE_RATIO ) )
    * RADIAN_PER_DEGREE,
    radians.get( ) ) );

  //! "01°02'03,010"С.Ш."
  degreeString = std::string( "01" ) + "\xB0"
    + std::string( "02" ) + "\x27"
    + std::string( "03,010" ) + "\x22"
    + "\xD1.\xD8.";
  ASSERT_TRUE( radians.fromDegreesString( degreeString ) );
  ASSERT_TRUE( isEqual( ( 1. + 2. / DEGREE_RATIO
    + 3.01 / ( DEGREE_RATIO * DEGREE_RATIO ) )
    * RADIAN_PER_DEGREE,
    radians.get( ) ) );

  //! "000°00'00.00"В.Д."
  degreeString = std::string( "000" ) + "\xB0"
    + std::string( "00" ) + "\x27"
    + std::string( "00.00" ) + "\x22"
    + "\xC2.\xC4.";
  ASSERT_TRUE( radians.fromDegreesString( degreeString ) );
  ASSERT_TRUE( isEqual( 0., radians.get( ) ) );

  //! "179°59'59.99"З.Д."
  degreeString = std::string( "179" ) + "\xB0"
    + std::string( "59" ) + "\x27"
    + std::string( "59.99" ) + "\x22"
    + "\xC7.\xC4.";
  ASSERT_TRUE( radians.fromDegreesString( degreeString ) );
  ASSERT_TRUE( isEqual( ( -179. - 59. / DEGREE_RATIO
    - 59.99 / ( DEGREE_RATIO * DEGREE_RATIO ) )
    * RADIAN_PER_DEGREE,
    radians.get( ) ) );

  //! "17.9°59'59.99"З.Д."
  degreeString = std::string( "17.9" ) + "\xB0"
    + std::string( "59" ) + "\x27"
    + std::string( "59.99" ) + "\x22"
    + "\xC7.\xC4.";
  radians.set( 0. );
  ASSERT_FALSE( radians.fromDegreesString( degreeString, true ) );
  ASSERT_TRUE( isEqual( 0., radians.get( ) ) );
  if( std::string( localeconv( )->decimal_point ) == "," )
  {
    ASSERT_TRUE( radians.fromDegreesString( degreeString ) );
    ASSERT_TRUE( isEqual( ( -17. - 59. / DEGREE_RATIO
      - 59.99 / ( DEGREE_RATIO * DEGREE_RATIO ) )
      * RADIAN_PER_DEGREE,
      radians.get( ) ) );
  }
  else
  {
    ASSERT_TRUE( radians.fromDegreesString( degreeString ) );
    ASSERT_TRUE( isEqual( ( -17.9 - 59. / DEGREE_RATIO
      - 59.99 / ( DEGREE_RATIO * DEGREE_RATIO ) )
      * RADIAN_PER_DEGREE,
      radians.get( ) ) );
  }

  //! "17,9°59'59.99"З.Д."
  degreeString = std::string( "17,9" ) + "\xB0"
    + std::string( "59" ) + "\x27"
    + std::string( "59.99" ) + "\x22"
    + "\xC7.\xC4.";
  radians.set( 0. );
  ASSERT_FALSE( radians.fromDegreesString( degreeString, true ) );
  ASSERT_TRUE( isEqual( 0., radians.get( ) ) );
  if( std::string( localeconv( )->decimal_point ) == "." )
  {
    ASSERT_TRUE( radians.fromDegreesString( degreeString ) );
    ASSERT_TRUE( isEqual( ( -17. - 59. / DEGREE_RATIO
      - 59.99 / ( DEGREE_RATIO * DEGREE_RATIO ) )
      * RADIAN_PER_DEGREE,
      radians.get( ) ) );
  }
  else
  {
    ASSERT_TRUE( radians.fromDegreesString( degreeString ) );
    ASSERT_TRUE( isEqual( ( -17.9 - 59. / DEGREE_RATIO
      - 59.99 / ( DEGREE_RATIO * DEGREE_RATIO ) )
      * RADIAN_PER_DEGREE,
      radians.get( ) ) );
  }

  //! "179°59'59.99"И.Д."
  degreeString = std::string( "179" ) + "\xB0"
    + std::string( "59" ) + "\x27"
    + std::string( "59.99" ) + "\x22"
    + "\xC8.\xC4.";
  radians.set( 0. );
  ASSERT_FALSE( radians.fromDegreesString( degreeString, true ) );
  ASSERT_TRUE( isEqual( 0., radians.get( ) ) );
  ASSERT_TRUE( radians.fromDegreesString( degreeString ) );
  ASSERT_TRUE( isEqual( ( 179. + 59. / DEGREE_RATIO
    + 59.99 / ( DEGREE_RATIO * DEGREE_RATIO ) )
    * RADIAN_PER_DEGREE,
    radians.get( ) ) );

  //! "179'59°59.99"З.Д."
  degreeString = std::string( "179" ) + "\xB0"
    + std::string( "59" ) + "\x22"
    + std::string( "59.99" ) + "\x27"
    + "\xC7.\xC4.";
  radians.set( 0. );
  ASSERT_FALSE( radians.fromDegreesString( degreeString ) );
  ASSERT_FALSE( radians.fromDegreesString( degreeString, true ) );
  ASSERT_TRUE( isEqual( 0., radians.get( ) ) );

  //! "DDD°MM'SS.SS"В.Д."
  degreeString = std::string( "DDD" ) + "\xB0"
    + std::string( "MM" ) + "\x27"
    + std::string( "SS.SS" ) + "\x22"
    + "\xC7.\xC4.";
  ASSERT_FALSE( radians.fromDegreesString( degreeString ) );
  ASSERT_FALSE( radians.fromDegreesString( degreeString, true ) );
  ASSERT_TRUE( isEqual( 0., radians.get( ) ) );
}
//==============================================================================
/*!
 * \brief Тестирование оператора ==
 *        (сравнению двух значений радиан)
*/
TEST( RadiansTest, RadianOperatorEqual )
{
  ASSERT_TRUE( Radian( .0 ) == Radian( .0 ) );
  ASSERT_TRUE( Radian( M_PI / 33. ) == Radian( Degree( WIDE_ANGLE / 33. ) ) );
  ASSERT_TRUE( Radian( M_PI ) == Radian( Degree( WIDE_ANGLE ) ) );
  ASSERT_TRUE( Radian( Degree( 50 + OUTSIDE_INACCURACY ) )
    == Radian( Degree( 50 - OUTSIDE_INACCURACY ) ) );
  ASSERT_FALSE( Radian( 50 + INSIDE_INACCURACY )
    == Radian( 50 - INSIDE_INACCURACY ) );
  ASSERT_FALSE( Radian( -5. ) == Radian( 5. ) );
  // pair
  std::pair<Radian, Radian> radiansPair( std::make_pair( Radian( 0. ), Radian( 0. ) ) );
  ASSERT_EQ( std::make_pair( Radian( 0. ), Radian( 0. ) ), radiansPair );
  radiansPair.first = Radian( M_PI / 33. );
  radiansPair.second = Radian( M_PI / 33. );
  ASSERT_EQ( std::make_pair( Radian( Degree( WIDE_ANGLE / 33. ) ),
    Radian( Degree( WIDE_ANGLE / 33. ) ) ), radiansPair );
  // tuple
  std::tuple<Radian, Radian, Radian> radiansTuple( std::make_tuple( Radian( 0. ),
    Radian( 0. ),
    Radian( 0. ) ) );
  ASSERT_EQ( std::make_tuple( Radian( 0. ), Radian( 0. ), Radian( 0. ) ), radiansTuple );
}
//==============================================================================
/*!
 * \brief Тестирование оператора >
 *        (сравнению двух значений радиан)
*/
TEST( RadiansTest, RadianOperatorAdvantage )
{
  ASSERT_FALSE( ( Radian( .0 ) > Radian( .0 ) ) );
  ASSERT_TRUE( ( Radian( .0 ) > Radian( -INSIDE_INACCURACY ) ) );
  ASSERT_FALSE( ( Radian( .0 ) > Radian( INSIDE_INACCURACY ) ) );
  ASSERT_TRUE( ( Radian( INSIDE_INACCURACY ) > Radian( .0 ) ) );
  ASSERT_FALSE( ( Radian( OUTSIDE_INACCURACY ) > Radian( .0 ) ) );
  ASSERT_FALSE( ( Radian( M_PI ) > Radian( M_PI ) ) );
  ASSERT_TRUE( ( Radian( M_PI ) > Radian( -M_PI ) ) );
  ASSERT_FALSE( ( Radian( -M_PI ) > Radian( M_PI ) ) );
  ASSERT_FALSE( Radian( 50 + OUTSIDE_INACCURACY )
               > Radian( 50 - OUTSIDE_INACCURACY ) );
  ASSERT_TRUE( Radian( 50 + INSIDE_INACCURACY ) > Radian( 50 - INSIDE_INACCURACY ) );
  ASSERT_FALSE( Radian( 50 - INSIDE_INACCURACY ) > Radian( 50 + INSIDE_INACCURACY ) );
}
//==============================================================================
/*!
 * \brief Тестирование оператора >=
 *        (сравнению двух значений радиан)
*/
TEST( RadiansTest, RadianOperatorAE )
{
  ASSERT_TRUE( ( Radian( .0 ) >= Radian( .0 ) ) );
  ASSERT_TRUE( ( Radian( .0 ) >= Radian( -INSIDE_INACCURACY ) ) );
  ASSERT_FALSE( ( Radian( .0 ) >= Radian( INSIDE_INACCURACY ) ) );
  ASSERT_TRUE( ( Radian( INSIDE_INACCURACY ) >= Radian( .0 ) ) );
  ASSERT_TRUE( ( Radian( OUTSIDE_INACCURACY ) >= Radian( .0 ) ) );
  ASSERT_TRUE( ( Radian( M_PI ) >= Radian( M_PI ) ) );
  ASSERT_TRUE( ( Radian( M_PI ) >= Radian( -M_PI ) ) );
  ASSERT_FALSE( ( Radian( -M_PI ) >= Radian( M_PI ) ) );
  ASSERT_TRUE( Radian( 50 + OUTSIDE_INACCURACY )
    >= Radian( 50 - OUTSIDE_INACCURACY ) );
  ASSERT_TRUE( Radian( 50 + INSIDE_INACCURACY ) >= Radian( 50 - INSIDE_INACCURACY ) );
  ASSERT_FALSE( Radian( 50 - INSIDE_INACCURACY )
    >= Radian( 50 + INSIDE_INACCURACY ) );
}
//==============================================================================
/*!
 *\brief Тестирование оператора <
 *       (сравнению двух значений радиан)
*/
TEST( RadiansTest, RadianOperatorLess )
{
  ASSERT_FALSE( ( Radian( .0 ) < Radian( .0 ) ) );
  ASSERT_TRUE( ( Radian( .0 ) < Radian( INSIDE_INACCURACY ) ) );
  ASSERT_FALSE( ( Radian( .0 ) < Radian( -INSIDE_INACCURACY ) ) );
  ASSERT_TRUE( ( Radian( -INSIDE_INACCURACY ) < Radian( .0 ) ) );
  ASSERT_FALSE( ( Radian( -OUTSIDE_INACCURACY ) < Radian( .0 ) ) );
  ASSERT_FALSE( ( Radian( M_PI ) < Radian( M_PI ) ) );
  ASSERT_TRUE( ( Radian( -M_PI ) < Radian( M_PI ) ) );
  ASSERT_FALSE( ( Radian( M_PI ) < Radian( -M_PI ) ) );
  ASSERT_FALSE( Radian( 50 + OUTSIDE_INACCURACY )
    < Radian( 50 - OUTSIDE_INACCURACY ) );
  ASSERT_TRUE( Radian( 50 - INSIDE_INACCURACY ) < Radian( 50 + INSIDE_INACCURACY ) );
  ASSERT_FALSE( Radian( 50 + INSIDE_INACCURACY ) < Radian( 50 - INSIDE_INACCURACY ) );
}
//==============================================================================
/*!
 * \brief Тестирование оператора <=
 *        (сравнению двух значений радиан)
*/
TEST( RadiansTest, RadianOperatorLE )
{
  ASSERT_TRUE( ( Radian( .0 ) <= Radian( .0 ) ) );
  ASSERT_TRUE( ( Radian( .0 ) <= Radian( INSIDE_INACCURACY ) ) );
  ASSERT_FALSE( ( Radian( .0 ) <= Radian( -INSIDE_INACCURACY ) ) );
  ASSERT_TRUE( ( Radian( -INSIDE_INACCURACY ) <= Radian( .0 ) ) );
  ASSERT_TRUE( ( Radian( -OUTSIDE_INACCURACY ) <= Radian( .0 ) ) );
  ASSERT_TRUE( ( Radian( M_PI ) <= Radian( M_PI ) ) );
  ASSERT_TRUE( ( Radian( -M_PI ) <= Radian( M_PI ) ) );
  ASSERT_FALSE( ( Radian( M_PI ) <= Radian( -M_PI ) ) );
  ASSERT_TRUE( Radian( 50 + OUTSIDE_INACCURACY )
    <= Radian( 50 - OUTSIDE_INACCURACY ) );
  ASSERT_TRUE( Radian( 50 - INSIDE_INACCURACY ) <= Radian( 50 + INSIDE_INACCURACY ) );
  ASSERT_FALSE( Radian( 50 + INSIDE_INACCURACY )
    <= Radian( 50 - INSIDE_INACCURACY ) );
}
//==============================================================================
/*!
* \brief Тестирование оператора -
*        (смена знака)
*/
TEST( RadiansTest, RadianOperatorUnaryMinus )
{
  ASSERT_EQ( Radian( -1. ), -Radian( 1. ) );
  ASSERT_EQ( Radian( 1. ), -Radian( -1. ) );
  ASSERT_EQ( Radian( .125 ), -Radian( -.125 ) );
}
//==============================================================================
/*!
* \brief Тестирование оператора +=
*/
TEST( RadiansTest, RadianOperatorIncrease )
{
  Radian radian( 1. );
  radian += Radian( 2. );
  ASSERT_EQ( Radian( 3. ), radian );
  radian += Radian( -4. );
  ASSERT_EQ( Radian( -1. ), radian );
  radian += Radian( .875 );
  ASSERT_EQ( Radian( -.125 ), radian );
  radian += Radian( .125 );
  ASSERT_EQ( Radian( 0. ), radian );
}
//==============================================================================
/*!
* \brief Тестирование оператора -=
*/
TEST( RadiansTest, RadianOperatorDecrease )
{
  Radian radian( 1. );
  radian -= Radian( 2.5 );
  ASSERT_EQ( Radian( -1.5 ), radian );
  radian -= Radian( -4. );
  ASSERT_EQ( Radian( 2.5 ), radian );
}
//==============================================================================
/*!
* \brief Тестирование оператора *=
*/
TEST( RadiansTest, RadianOperatorMultiplication )
{
  Radian radian( 3. );
  radian *= Radian( -2.5 );
  ASSERT_EQ( Radian( -7.5 ), radian );
}
//==============================================================================
/*!
 * \brief Тестирование оператора /=
*/
TEST( RadiansTest, RadianOperatorDivision )
{
  using namespace BasicMath;

  Radian radian( 21. );
  radian /= Radian( 7. );
  ASSERT_TRUE( isEqual( Radian( 3. ), radian ) );
  radian /= Radian( -1.5 );
  ASSERT_TRUE( isEqual( Radian( -2. ), radian ) );
  // на 0 не делим
  ASSERT_THROW(radian /= Radian(0), MathFailure);
  try
  {
    radian /= Radian( 0 );
  }
  catch( const MathFailure& exception )
  {
    ASSERT_EQ( MathEnumFailure::RadianDivideByZero, exception.getCode( ) );
    ASSERT_EQ( mathLibTitle, exception.getLib( ) );
    ASSERT_TRUE( isEqual( Radian( -2. ), radian ) );
  }
  // на почти 0 тоже не делим
  ASSERT_THROW(radian /= Radian(ACCURACY_RADIAN * 0.1), MathFailure);
  try
  {
    radian /= Radian( ACCURACY_RADIAN * 0.1 );
  }
  catch( const MathFailure& exception )
  {
    ASSERT_EQ( MathEnumFailure::RadianDivideByZero, exception.getCode( ) );
    ASSERT_EQ( mathLibTitle, exception.getLib( ) );
    ASSERT_TRUE( isEqual( Radian( -2. ), radian ) );
  }
  ASSERT_TRUE( isEqual( Radian( -2. ), radian ) );
}
//==============================================================================
/*!
* \brief Тестирование арифметических операций (+, -, *, /)
*/
TEST( RadiansTest, RadianArithmeticOperations )
{
  ASSERT_EQ( Radian( 1.5 ), Radian( -1. ) + Radian( 2.5 ) );
  ASSERT_EQ( Radian( -3.5 ), Radian( -1. ) - Radian( 2.5 ) );
  ASSERT_EQ( Radian( -2.5 ), Radian( -1. ) * Radian( 2.5 ) );
  // деление на нормальное значение производится
  ASSERT_EQ( Radian( -0.4 ), Radian( -1. ) / Radian( 2.5 ) );
  // деление на 0 не производится
  ASSERT_THROW(Radian(-1.) / Radian(0), MathFailure);
  try
  {
    Radian( -1. ) / Radian( 0 );
  }
  catch( const MathFailure& exception )
  {
    ASSERT_EQ( MathEnumFailure::RadianDivideByZero, exception.getCode( ) );
    ASSERT_EQ( mathLibTitle, exception.getLib( ) );
  }
  // деление на почти 0 тоже не производится
  ASSERT_THROW(Radian(-1.) / Radian(ACCURACY_RADIAN * 0.1), MathFailure);
  try
  {
    Radian( -1. ) / Radian( ACCURACY_RADIAN * 0.1 );
  }
  catch( const MathFailure& exception )
  {
    ASSERT_EQ( MathEnumFailure::RadianDivideByZero, exception.getCode( ) );
    ASSERT_EQ( mathLibTitle, exception.getLib( ) );
  }
}
//==============================================================================
/*!
* \brief Тестирование операторов ==, !=, >, >=, <, <=
*        (сравнение радиан и целых чисел)
*/
TEST( RadiansTest, RadianOperatorComparisonInt )
{
  ASSERT_TRUE( Radian( 0. ) == 0 );
  ASSERT_FALSE( Radian( 0.1 ) == 0 );

  ASSERT_FALSE( Radian( 0. ) != 0 );
  ASSERT_TRUE( Radian( 0.1 ) != 0 );

  ASSERT_FALSE( Radian( 0. ) > 0 );
  ASSERT_TRUE( Radian( 0.1 ) > 0 );

  ASSERT_TRUE( Radian( 0. ) >= 0 );
  ASSERT_TRUE( Radian( 0.1 ) >= 0 );

  ASSERT_FALSE( Radian( 0. ) < 0 );
  ASSERT_TRUE( Radian( -0.1 ) < 0 );

  ASSERT_TRUE( Radian( 0. ) <= 0 );
  ASSERT_TRUE( Radian( -0.1 ) <= 0 );
}
//==============================================================================
/*!
 * \brief Тестирование оператора >
 *        (сравнение радиан и вещественных чисел)
*/
TEST( RadiansTest, RadianOperatorComparisonDouble )
{
  ASSERT_FALSE( Radian( 0. ) > 0. );
  ASSERT_FALSE( Radian( -0.1 ) > 0. );
  ASSERT_TRUE( Radian( 0.1 ) > 0. );
}
//==============================================================================
/*!
 * \brief Тестирование оператора >>
 *        (извлечение данных из потока)
*/
TEST( RadianTest, RadianOperatorInputStream )
{
  Degree testValue( 30.0 );
  std::stringstream stream;
  stream << testValue.get( );

  Radian radian;
  stream >> radian;

  ASSERT_TRUE( BasicMath::isEqual( testValue.toRadians( ).get( ), radian.get( ) ) );
}
//==============================================================================
/*!
 * \brief Тестирование normalize()
 *        Приведение значения радиан в диапазон от -PI до PI
*/
TEST( RadianTest, RadianNormalize )
{
  using namespace BasicMath;

  ASSERT_TRUE( isEqual( Radian( -M_PI / 4 ),
    Radian( -6 * M_PI - M_PI / 4 ).normalize( ) ) );
}
//==============================================================================
/*!
* \brief Тестирование normalizeWithShift()
*        Приведение значения радиан в диапазон от 0 до 2*PI
*/
TEST( RadianTest, RadianNormalizeWithShift )
{
  using namespace BasicMath;

  ASSERT_TRUE( isEqual( Radian( 7 * M_PI / 4 ),
    Radian( -6 * M_PI - M_PI / 4 ).normalizeWithShift( ) ) );
}
//==============================================================================
/*!
 * \brief Тестирование математических операций радиан с вещественными числами
 *        с двойной точностью
*/
TEST( RadianTest, RadianMathOperations )
{
  using namespace BasicMath;

  Radian control( 4. );
  Radian result;

  //! Сложение
  result = Radian( 1. ) + 3.;
  ASSERT_TRUE( isEqual( control, result ) );
  result = Radian( 1. ) + 3.f;
  ASSERT_TRUE( isEqual( control, result ) );
  result = Radian( 1. ) + int( 3 );
  ASSERT_TRUE( isEqual( control, result ) );
  result = Radian( 1. ) + size_t( 3 );
  ASSERT_TRUE( isEqual( control, result ) );

  //! Вычитание
  result = Radian( 7. ) - 3.;
  ASSERT_TRUE( isEqual( control, result ) );
  result = Radian( 7. ) - 3.f;
  ASSERT_TRUE( isEqual( control, result ) );
  result = Radian( 7. ) - int( 3 );
  ASSERT_TRUE( isEqual( control, result ) );
  result = Radian( 7. ) - size_t( 3 );
  ASSERT_TRUE( isEqual( control, result ) );

  //! Умножение
  result = Radian( 2. ) * 2.;
  ASSERT_TRUE( isEqual( control, result ) );
  result = Radian( 2. ) * 2.f;
  ASSERT_TRUE( isEqual( control, result ) );
  result = Radian( 2. ) * int( 2 );
  ASSERT_TRUE( isEqual( control, result ) );
  result = Radian( 2. ) * size_t( 2 );
  ASSERT_TRUE( isEqual( control, result ) );

  //! Деление
  result = Radian( 8. ) / 2.;
  ASSERT_TRUE( isEqual( control, result ) );
  result = Radian( 8. ) / 2.f;
  ASSERT_TRUE( isEqual( control, result ) );
  result = Radian( 8. ) / int( 2 );
  ASSERT_TRUE( isEqual( control, result ) );
  result = Radian( 8. ) / size_t( 2 );
  ASSERT_TRUE( isEqual( control, result ) );

  //! Деление на 0
  ASSERT_THROW(control / 0., MathFailure);
  try
  {
    result = control / 0.;
  }
  catch( const MathFailure& failure )
  {
    ASSERT_EQ( RadianDivideByZero, failure.getCode( ) );
    ASSERT_EQ( mathLibTitle, failure.getLib( ) );
  }

  ASSERT_THROW(control / 0.f, MathFailure);
  try
  {
    result = control / 0.f;
  }
  catch( const MathFailure& failure )
  {
    ASSERT_EQ( RadianDivideByZero, failure.getCode( ) );
    ASSERT_EQ( mathLibTitle, failure.getLib( ) );
  }

  ASSERT_THROW(control / int(0), MathFailure);
  try
  {
    result = control / int( 0 );
  }
  catch( const MathFailure& failure )
  {
    ASSERT_EQ( RadianDivideByZero, failure.getCode( ) );
    ASSERT_EQ( mathLibTitle, failure.getLib( ) );
  }

  ASSERT_THROW(control / size_t(0), MathFailure);
  try
  {
    result = control / size_t( 0 );
  }
  catch( const MathFailure& failure )
  {
    ASSERT_EQ( RadianDivideByZero, failure.getCode( ) );
    ASSERT_EQ( mathLibTitle, failure.getLib( ) );
  }
}
//==============================================================================
// тестирование модуля
TEST( RadianTest, FabsTest )
{
  ASSERT_EQ( Radian( 1.5 ), fabs( Radian( 1.5 ) ) );
  ASSERT_EQ( Radian( 1.5 ), fabs( Radian( -1.5 ) ) );
}
//==============================================================================
} //! Tests
//==============================================================================
