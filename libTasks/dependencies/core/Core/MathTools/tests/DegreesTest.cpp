#include <MathTools/AngularUnits/Degree.hpp>
//! AngularUnits
#include <MathTools/AngularUnits/Radian.hpp>
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
 * \brief Тестирование Degree
 *        (конструктора на оcнове double)
*/
TEST( DegreesTest, ConstructorDegreeFromDouble )
{
  using namespace BasicMath;

  ASSERT_TRUE( isEqual( 15., Degree( 15. ).get( ) ) );
  ASSERT_TRUE( isEqual( 30., Degree( 30. ).get( ) ) );
  ASSERT_TRUE( isEqual( 45., Degree( 45. ).get( ) ) );
  ASSERT_TRUE( isEqual( 90., Degree( 90. ).get( ) ) );
  ASSERT_TRUE( isEqual( 180., Degree( 180. ).get( ) ) );
  ASSERT_TRUE( isEqual( 360., Degree( 360. ).get( ) ) );
  ASSERT_TRUE( isEqual( -15., Degree( -15. ).get( ) ) );
  ASSERT_TRUE( isEqual( -30., Degree( -30. ).get( ) ) );
  ASSERT_TRUE( isEqual( -45., Degree( -45. ).get( ) ) );
  ASSERT_TRUE( isEqual( -90., Degree( -90. ).get( ) ) );
  ASSERT_TRUE( isEqual( -180., Degree( -180. ).get( ) ) );
  ASSERT_TRUE( isEqual( -360., Degree( -360. ).get( ) ) );
  ASSERT_TRUE( isEqual( 0., Degree( 0. ).get( ) ) );
  ASSERT_TRUE( isEqual( 0.25, Degree( 0.25 ).get( ) ) );
  ASSERT_TRUE( isEqual( -0.1, Degree( -0.1 ).get( ) ) );
}
//==============================================================================
/*!
 * \brief Тестирование Degree
 *        (конструктора на оcнове радиан - Radian)
*/
TEST( DegreesTest, ConstructorDegreeFromRadian )
{
  using namespace BasicMath;

  ASSERT_TRUE( isEqual( 15., Degree( Radian( M_PI / 12 ) ).get( ) ) );
  ASSERT_TRUE( isEqual( 30., Degree( Radian( M_PI / 6 ) ).get( ) ) );
  ASSERT_TRUE( isEqual( 45., Degree( Radian( M_PI / 4 ) ).get( ) ) );
  ASSERT_TRUE( isEqual( 90., Degree( Radian( M_PI / 2 ) ).get( ) ) );
  ASSERT_TRUE( isEqual( 180., Degree( Radian( M_PI ) ).get( ) ) );
  ASSERT_TRUE( isEqual( 360., Degree( Radian( M_PI * 2 ) ).get( ) ) );
  ASSERT_TRUE( isEqual( -15., Degree( Radian( -M_PI / 12 ) ).get( ) ) );
  ASSERT_TRUE( isEqual( -30., Degree( Radian( -M_PI / 6 ) ).get( ) ) );
  ASSERT_TRUE( isEqual( -45., Degree( Radian( -M_PI / 4 ) ).get( ) ) );
  ASSERT_TRUE( isEqual( -90., Degree( Radian( -M_PI / 2 ) ).get( ) ) );
  ASSERT_TRUE( isEqual( -180., Degree( Radian( -M_PI ) ).get( ) ) );
  ASSERT_TRUE( isEqual( -360., Degree( Radian( -M_PI * 2 ) ).get( ) ) );
  ASSERT_TRUE( isEqual( 0., Degree( Radian( 0. ) ).get( ) ) );
  ASSERT_TRUE( isEqual( 0.25, Degree( Radian( 0.25 * RADIAN_PER_DEGREE ) ).get( ) ) );
  ASSERT_TRUE( isEqual( -0.1, Degree( Radian( -0.1 * RADIAN_PER_DEGREE ) ).get( ) ) );
}
//==============================================================================
/*!
 * \brief Тестирование set и get
 *        (присвоение значения на основе double и получение)
*/
TEST( DegreesTest, DegreeGetSet )
{
  using namespace BasicMath;

  Degree degree;
  degree.set( 15. );
  ASSERT_TRUE( isEqual( 15., degree.get( ) ) );
  degree.set( 180. );
  ASSERT_TRUE( isEqual( 180., degree.get( ) ) );
  degree.set( 360. );
  ASSERT_TRUE( isEqual( 360., degree.get( ) ) );
  degree.set( -15. );
  ASSERT_TRUE( isEqual( -15., degree.get( ) ) );
  degree.set( -360. );
  ASSERT_TRUE( isEqual( -360., degree.get( ) ) );
  degree.set( 0. );
  ASSERT_TRUE( isEqual( 0., degree.get( ) ) );
  degree.set( 0.25 );
  ASSERT_TRUE( isEqual( 0.25, degree.get( ) ) );
  degree.set( -0.1 );
  ASSERT_TRUE( isEqual( -0.1, degree.get( ) ) );
}
//==============================================================================
/*!
 * \brief Тестирование toRadians
 *        (перевод из градусов в радианы)
*/
TEST( DegreesTest, DegreeToRadian )
{
  using namespace BasicMath;

  ASSERT_TRUE( isEqual( M_PI / 12, Degree( 15. ).toRadians( ).get( ) ) );
  ASSERT_TRUE( isEqual( M_PI / 2, Degree( 90. ).toRadians( ).get( ) ) );
  ASSERT_TRUE( isEqual( M_PI, Degree( 180. ).toRadians( ).get( ) ) );
  ASSERT_TRUE( isEqual( M_PI * 2, Degree( 360. ).toRadians( ).get( ) ) );
  ASSERT_TRUE( isEqual( -M_PI / 12, Degree( -15. ).toRadians( ).get( ) ) );
  ASSERT_TRUE( isEqual( -M_PI / 2, Degree( -90. ).toRadians( ).get( ) ) );
  ASSERT_TRUE( isEqual( -M_PI, Degree( -180. ).toRadians( ).get( ) ) );
  ASSERT_TRUE( isEqual( -M_PI * 2, Degree( -360. ).toRadians( ).get( ) ) );
  ASSERT_TRUE( isEqual( 0., Degree( 0. ).toRadians( ).get( ) ) );
  ASSERT_TRUE( isEqual( .25 * RADIAN_PER_DEGREE, Degree( .25 ).toRadians( ).get( ) ) );
  ASSERT_TRUE( isEqual( -.1 * RADIAN_PER_DEGREE, Degree( -.1 ).toRadians( ).get( ) ) );
}
//==============================================================================
/*!
 * \brief Тестирование fromRadians
 *        (присвоение значения на основе радиан - Radian)
*/
TEST( DegreesTest, DegreeFromRadian )
{
  using namespace BasicMath;

  Degree degree;
  degree.fromRadians( Radian( M_PI / 12 ) );
  ASSERT_TRUE( isEqual( 15., degree.get( ) ) );
  degree.fromRadians( Radian( M_PI ) );
  ASSERT_TRUE( isEqual( 180., degree.get( ) ) );
  degree.fromRadians( Radian( M_PI * 2 ) );
  ASSERT_TRUE( isEqual( 360., degree.get( ) ) );
  degree.fromRadians( Radian( -M_PI / 12 ) );
  ASSERT_TRUE( isEqual( -15., degree.get( ) ) );
  degree.fromRadians( Radian( -M_PI * 2 ) );
  ASSERT_TRUE( isEqual( -360., degree.get( ) ) );
  degree.fromRadians( Radian( 0. ) );
  ASSERT_TRUE( isEqual( 0., degree.get( ) ) );
  degree.fromRadians( Radian( 0.25 * RADIAN_PER_DEGREE ) );
  ASSERT_TRUE( isEqual( 0.25, degree.get( ) ) );
  degree.fromRadians( Radian( -0.1 * RADIAN_PER_DEGREE ) );
  ASSERT_TRUE( isEqual( -0.1, degree.get( ) ) );
}
//==============================================================================
/*!
 * \brief Тестирование оператора ==
 *        (сравнению двух градусных величин)
*/
TEST( DegreesTest, DegreeOperatorEqual )
{

  ASSERT_TRUE( Degree( .0 ) == Degree( .0 ) );
  ASSERT_TRUE( Degree( WIDE_ANGLE / 33. ) == Degree( Radian( M_PI / 33. ) ) );
  ASSERT_TRUE( Degree( WIDE_ANGLE ) == Degree( Radian( M_PI ) ) );
  ASSERT_TRUE( Degree( 50 + OUTSIDE_INACCURACY )
    == Degree( 50 - OUTSIDE_INACCURACY ) );
  ASSERT_FALSE( Degree( 50 + INSIDE_INACCURACY )
    == Degree( 50 - INSIDE_INACCURACY ) );
  ASSERT_FALSE( Degree( 50. ) == Degree( 49.9 ) );
  ASSERT_FALSE( Degree( -5. ) == Degree( 5. ) );
  // pair
  std::pair<Degree, Degree> degreesPair( std::make_pair( Degree( 0. ), Degree( 0. ) ) );
  ASSERT_EQ( std::make_pair( Degree( 0. ), Degree( 0. ) ), degreesPair );
  degreesPair.first = Degree( WIDE_ANGLE / 33. );
  degreesPair.second = Degree( WIDE_ANGLE / 33. );
  ASSERT_EQ( std::make_pair( Degree( Radian( M_PI / 33. ) ),
    Degree( Radian( M_PI / 33. ) ) ), degreesPair );
  // tuple
  std::tuple<Degree, Degree, Degree> degreesTuple( std::make_tuple( Degree( 0. ),
    Degree( 0. ),
    Degree( 0. ) ) );
  ASSERT_EQ( std::make_tuple( Degree( 0. ), Degree( 0. ), Degree( 0. ) ), degreesTuple );
}
//==============================================================================
/*!
 * \brief Тестирование оператора >
 *        (сравнению двух значений радиан)
*/
TEST( DegreesTest, DegreeOperatorAdvantage )
{
  ASSERT_FALSE( ( Degree( .0 ) > Degree( .0 ) ) );
  ASSERT_TRUE( ( Degree( .0 ) > Degree( -INSIDE_INACCURACY ) ) );
  ASSERT_FALSE( ( Degree( .0 ) > Degree( INSIDE_INACCURACY ) ) );
  ASSERT_TRUE( ( Degree( INSIDE_INACCURACY ) > Degree( .0 ) ) );
  ASSERT_FALSE( ( Degree( OUTSIDE_INACCURACY ) > Degree( .0 ) ) );
  ASSERT_FALSE( ( Degree( 180 ) > Degree( 180 ) ) );
  ASSERT_TRUE( ( Degree( 180 ) > Degree( -180 ) ) );
  ASSERT_FALSE( ( Degree( -180 ) > Degree( 180 ) ) );
  ASSERT_FALSE( Degree( 50 + OUTSIDE_INACCURACY )
               > Degree( 50 - OUTSIDE_INACCURACY ) );
  ASSERT_TRUE( Degree( 50 + INSIDE_INACCURACY ) > Degree( 50 - INSIDE_INACCURACY ) );
  ASSERT_FALSE( Degree( 50 - INSIDE_INACCURACY ) > Degree( 50 + INSIDE_INACCURACY ) );
}
//==============================================================================
/*!
 * \brief Тестирование оператора >=
 *        (сравнению двух значений радиан)
*/
TEST( DegreesTest, DegreeOperatorAE )
{
  ASSERT_TRUE( ( Degree( .0 ) >= Degree( .0 ) ) );
  ASSERT_TRUE( ( Degree( .0 ) >= Degree( -INSIDE_INACCURACY ) ) );
  ASSERT_FALSE( ( Degree( .0 ) >= Degree( INSIDE_INACCURACY ) ) );
  ASSERT_TRUE( ( Degree( INSIDE_INACCURACY ) >= Degree( .0 ) ) );
  ASSERT_TRUE( ( Degree( OUTSIDE_INACCURACY ) >= Degree( .0 ) ) );
  ASSERT_TRUE( ( Degree( 180 ) >= Degree( 180 ) ) );
  ASSERT_TRUE( ( Degree( 180 ) >= Degree( -180 ) ) );
  ASSERT_FALSE( ( Degree( -180 ) >= Degree( 180 ) ) );
  ASSERT_TRUE( Degree( 50 + OUTSIDE_INACCURACY )
    >= Degree( 50 - OUTSIDE_INACCURACY ) );
  ASSERT_TRUE( Degree( 50 + INSIDE_INACCURACY ) >= Degree( 50 - INSIDE_INACCURACY ) );
  ASSERT_FALSE( Degree( 50 - INSIDE_INACCURACY )
    >= Degree( 50 + INSIDE_INACCURACY ) );
}
//==============================================================================
/*!
 *\brief Тестирование оператора <
 *       (сравнению двух значений радиан)
*/
TEST( DegreesTest, DegreeOperatorLess )
{
  ASSERT_FALSE( ( Degree( .0 ) < Degree( .0 ) ) );
  ASSERT_TRUE( ( Degree( .0 ) < Degree( INSIDE_INACCURACY ) ) );
  ASSERT_FALSE( ( Degree( .0 ) < Degree( -INSIDE_INACCURACY ) ) );
  ASSERT_TRUE( ( Degree( -INSIDE_INACCURACY ) < Degree( .0 ) ) );
  ASSERT_FALSE( ( Degree( -OUTSIDE_INACCURACY ) < Degree( .0 ) ) );
  ASSERT_FALSE( ( Degree( 180 ) < Degree( 180 ) ) );
  ASSERT_TRUE( ( Degree( -180 ) < Degree( 180 ) ) );
  ASSERT_FALSE( ( Degree( 180 ) < Degree( -180 ) ) );
  ASSERT_FALSE( Degree( 50 + OUTSIDE_INACCURACY )
    < Degree( 50 - OUTSIDE_INACCURACY ) );
  ASSERT_TRUE( Degree( 50 - INSIDE_INACCURACY ) < Degree( 50 + INSIDE_INACCURACY ) );
  ASSERT_FALSE( Degree( 50 + INSIDE_INACCURACY ) < Degree( 50 - INSIDE_INACCURACY ) );
}
//==============================================================================
/*!
 * \brief Тестирование оператора <=
 *        (сравнению двух значений радиан)
*/
TEST( DegreesTest, DegreeOperatorLE )
{
  ASSERT_TRUE( ( Degree( .0 ) <= Degree( .0 ) ) );
  ASSERT_TRUE( ( Degree( .0 ) <= Degree( INSIDE_INACCURACY ) ) );
  ASSERT_FALSE( ( Degree( .0 ) <= Degree( -INSIDE_INACCURACY ) ) );
  ASSERT_TRUE( ( Degree( -INSIDE_INACCURACY ) <= Degree( .0 ) ) );
  ASSERT_TRUE( ( Degree( -OUTSIDE_INACCURACY ) <= Degree( .0 ) ) );
  ASSERT_TRUE( ( Degree( 180 ) <= Degree( 180 ) ) );
  ASSERT_TRUE( ( Degree( -180 ) <= Degree( 180 ) ) );
  ASSERT_FALSE( ( Degree( 180 ) <= Degree( -180 ) ) );
  ASSERT_TRUE( Degree( 50 + OUTSIDE_INACCURACY )
    <= Degree( 50 - OUTSIDE_INACCURACY ) );
  ASSERT_TRUE( Degree( 50 - INSIDE_INACCURACY ) <= Degree( 50 + INSIDE_INACCURACY ) );
  ASSERT_FALSE( Degree( 50 + INSIDE_INACCURACY )
    <= Degree( 50 - INSIDE_INACCURACY ) );
}
//==============================================================================
/*!
 * \brief Тестирование оператора -
 *        (смена знака)
 */
TEST( DegreesTest, DegreeOperatorUnaryMinus )
{
  ASSERT_EQ( Degree( -1. ), -Degree( 1. ) );
  ASSERT_EQ( Degree( 1. ), -Degree( -1. ) );
  ASSERT_EQ( Degree( .125 ), -Degree( -.125 ) );
}
//==============================================================================
/*!
 * \brief Тестирование оператора +=
 */
TEST( DegreesTest, DegreeOperatorIncrease )
{
  Degree degree( 1. );
  degree += Degree( 2. );
  ASSERT_EQ( Degree( 3. ), degree );
  degree += Degree( -4. );
  ASSERT_EQ( Degree( -1. ), degree );
  degree += Degree( .875 );
  ASSERT_EQ( Degree( -.125 ), degree );
  degree += Degree( .125 );
  ASSERT_EQ( Degree( 0. ), degree );
}
//==============================================================================
/*!
 * \brief Тестирование оператора -=
 */
TEST( DegreesTest, DegreeOperatorDecrease )
{
  Degree degree( 1. );
  degree -= Degree( 2.5 );
  ASSERT_EQ( Degree( -1.5 ), degree );
  degree -= Degree( -4. );
  ASSERT_EQ( Degree( 2.5 ), degree );
}
//==============================================================================
/*!
 * \brief Тестирование оператора *=
 */
TEST( DegreesTest, DegreeOperatorMultiplication )
{
  Degree degree( 3. );
  degree *= Degree( -2.5 );
  ASSERT_EQ( Degree( -7.5 ), degree );
}
//==============================================================================
/*!
 * \brief Тестирование оператора /=
 */
TEST( DegreesTest, DegreeOperatorDivision )
{
  using namespace BasicMath;

  Degree degree( 21. );
  degree /= Degree( 7. );
  ASSERT_TRUE( isEqual( Degree( 3. ), degree ) );
  degree /= Degree( -1.5 );
  ASSERT_TRUE( isEqual( Degree( -2. ), degree ) );
  //! На 0 не делим.
  ASSERT_THROW(degree /= Degree(0), MathFailure);
  try
  {
    degree /= Degree( 0 );
  }
  catch( const MathFailure& exception )
  {
    ASSERT_EQ( MathEnumFailure::DegreeDivideByZero, exception.getCode( ) );
    ASSERT_EQ( mathLibTitle, exception.getLib( ) );
    ASSERT_TRUE( isEqual( Degree( -2. ), degree ) );
  }
  //! На почти 0 тоже не делим.
  ASSERT_THROW(degree /= Degree(ACCURACY_DEGREE * 0.1), MathFailure);
  try
  {
    degree /= Degree( ACCURACY_DEGREE * 0.1 );
  }
  catch( const MathFailure& exception )
  {
    ASSERT_EQ( MathEnumFailure::DegreeDivideByZero, exception.getCode( ) );
    ASSERT_EQ( mathLibTitle, exception.getLib( ) );
    ASSERT_TRUE( isEqual( Degree( -2. ), degree ) );
  }
  ASSERT_TRUE( isEqual( Degree( -2. ), degree ) );
}
//==============================================================================
/*!
 * \brief Тестирование арифметических операций (+, -, *, /)
 */
TEST( DegreesTest, DegreeArithmeticOperations )
{
  ASSERT_EQ( Degree( 1.5 ), Degree( -1. ) + Degree( 2.5 ) );
  ASSERT_EQ( Degree( -3.5 ), Degree( -1. ) - Degree( 2.5 ) );
  ASSERT_EQ( Degree( -2.5 ), Degree( -1. ) * Degree( 2.5 ) );
  //! Деление на нормальное значение производится.
  ASSERT_EQ( Degree( -0.4 ), Degree( -1. ) / Degree( 2.5 ) );
  //! Деление на 0 не производится.
  ASSERT_THROW(Degree(-1.) / Degree(0), MathFailure);
  try
  {
    Degree( -1. ) / Degree( 0 );
  }
  catch( const MathFailure& exception )
  {
    ASSERT_EQ( MathEnumFailure::DegreeDivideByZero, exception.getCode( ) );
    ASSERT_EQ( mathLibTitle, exception.getLib( ) );
  }
  //! Деление на почти 0 тоже не производится.
  ASSERT_THROW(Degree(-1.) / Degree(ACCURACY_DEGREE * 0.1), MathFailure);
  try
  {
    Degree( -1. ) / Degree( ACCURACY_DEGREE * 0.1 );
  }
  catch( const MathFailure& exception )
  {
    ASSERT_EQ( MathEnumFailure::DegreeDivideByZero, exception.getCode( ) );
    ASSERT_EQ( mathLibTitle, exception.getLib( ) );
  }
}
//==============================================================================
/*!
 * \brief Тестирование toDegreesString
 *        (конвертирование градусов из вещественного числа в формализованную
 *        строку)
*/
TEST( DegreesTest, DegreeToDegreeString )
{
  //! "01°30'30.00"С.Ш."
  std::string degreeString( std::string( "01" ) + "\xB0"
    + std::string( "30" ) + "\x27"
    + std::string( "30.00" ) + "\x22"
    + "\xD1.\xD8." );
  Degree degree( 1. + 30. / DEGREE_RATIO + 30. / ( DEGREE_RATIO * DEGREE_RATIO ) );
  ASSERT_EQ( degreeString, degree.toDegreesString( LATITUDE ) );

  //! "10°35'40.43"Ю.Ш."
  degreeString = std::string( "10" ) + "\xB0"
    + std::string( "35" ) + "\x27"
    + std::string( "40.43" ) + "\x22"
    + "\xDE.\xD8.";
  degree.set( -10. - 35. / DEGREE_RATIO - 40.43 / ( DEGREE_RATIO * DEGREE_RATIO ) );
  ASSERT_EQ( degreeString, degree.toDegreesString( LATITUDE ) );

  //! "00°00'00.00"С.Ш."
  degreeString = std::string( "00" ) + "\xB0"
    + std::string( "00" ) + "\x27"
    + std::string( "00.00" ) + "\x22"
    + "\xD1.\xD8.";
  degree.set( 0. );
  ASSERT_EQ( degreeString, degree.toDegreesString( LATITUDE ) );

  //! "01°02'03"С.Ш."
  degreeString = std::string( "01" ) + "\xB0"
    + std::string( "02" ) + "\x27"
    + std::string( "03" ) + "\x22"
    + "\xD1.\xD8.";
  degree.set( 1. + 2. / DEGREE_RATIO + 3. / ( DEGREE_RATIO * DEGREE_RATIO ) );
  ASSERT_EQ( degreeString, degree.toDegreesString( LATITUDE, 0 ) );

  //! "1024°02'03.00"С.Ш."
  degreeString = std::string( "1024" ) + "\xB0"
    + std::string( "02" ) + "\x27"
    + std::string( "03.00" ) + "\x22"
    + "\xD1.\xD8.";
  degree.set( 1024. + 2. / DEGREE_RATIO + 3. / ( DEGREE_RATIO * DEGREE_RATIO ) );
  ASSERT_EQ( degreeString, degree.toDegreesString( LATITUDE ) );

  //! "01°02'03,010"С.Ш."
  degreeString = std::string( "01" ) + "\xB0"
    + std::string( "02" ) + "\x27"
    + std::string( "03,010" ) + "\x22"
    + "\xD1.\xD8.";
  degree.set( 1. + 2. / DEGREE_RATIO + 3.01 / ( DEGREE_RATIO * DEGREE_RATIO ) );
  ASSERT_EQ( degreeString, degree.toDegreesString( LATITUDE, 3, "," ) );

  //! "000°00'00.00"В.Д."
  degreeString = std::string( "000" ) + "\xB0"
    + std::string( "00" ) + "\x27"
    + std::string( "00.00" ) + "\x22"
    + "\xC2.\xC4.";
  degree.set( 0. );
  ASSERT_EQ( degreeString, degree.toDegreesString( LONGITUDE ) );

  //! "179°59'59.99"З.Д."
  degreeString = std::string( "179" ) + "\xB0"
    + std::string( "59" ) + "\x27"
    + std::string( "59.99" ) + "\x22"
    + "\xC7.\xC4.";
  degree.set( -179. - 59. / DEGREE_RATIO
    - 59.99 / ( DEGREE_RATIO * DEGREE_RATIO ) );
  ASSERT_EQ( degreeString, degree.toDegreesString( LONGITUDE ) );
}
//==============================================================================
/*!
 * \brief Тестирование fromDegreesString
 *        (конвертирование градусов из формализованной строки в вещественное
 *        число)
*/
TEST( DegreesTest, DegreeStringToDegree )
{
  using namespace BasicMath;

  Degree degree;

  //! "01°30'30.00"С.Ш."
  std::string degreeString( std::string( "01" ) + "\xB0"
    + std::string( "30" ) + "\x27"
    + std::string( "30.00" ) + "\x22"
    + "\xD1.\xD8." );
  ASSERT_TRUE( degree.fromDegreesString( degreeString ) );
  ASSERT_TRUE( isEqual( 1. + 30. / DEGREE_RATIO
    + 30. / ( DEGREE_RATIO * DEGREE_RATIO ),
    degree.get( ) ) );

  //! "10°35'40.43"Ю.Ш."
  degreeString = std::string( "10" ) + "\xB0"
    + std::string( "35" ) + "\x27"
    + std::string( "40.43" ) + "\x22"
    + "\xDE.\xD8.";
  ASSERT_TRUE( degree.fromDegreesString( degreeString ) );
  ASSERT_TRUE( isEqual( -10. - 35. / DEGREE_RATIO
    - 40.43 / ( DEGREE_RATIO * DEGREE_RATIO ),
    degree.get( ) ) );

  //! "00°00'00.00"С.Ш."
  degreeString = std::string( "00" ) + "\xB0"
    + std::string( "00" ) + "\x27"
    + std::string( "00.00" ) + "\x22"
    + "\xD1.\xD8.";
  ASSERT_TRUE( degree.fromDegreesString( degreeString ) );
  ASSERT_TRUE( isEqual( 0., degree.get( ) ) );

  //! "01°02'03"С.Ш."
  degreeString = std::string( "01" ) + "\xB0"
    + std::string( "02" ) + "\x27"
    + std::string( "03" ) + "\x22"
    + "\xD1.\xD8.";
  ASSERT_TRUE( degree.fromDegreesString( degreeString ) );
  ASSERT_TRUE( isEqual( 1. + 2. / DEGREE_RATIO
    + 3. / ( DEGREE_RATIO * DEGREE_RATIO ),
    degree.get( ) ) );

  //! "1024°02'03.00"С.Ш."
  degreeString = std::string( "1024" ) + "\xB0"
    + std::string( "02" ) + "\x27"
    + std::string( "03.00" ) + "\x22"
    + "\xD1.\xD8.";
  ASSERT_TRUE( degree.fromDegreesString( degreeString ) );
  ASSERT_TRUE( isEqual( 1024. + 2. / DEGREE_RATIO
    + 3. / ( DEGREE_RATIO * DEGREE_RATIO ),
    degree.get( ) ) );

  //! "01°02'03,010"С.Ш."
  degreeString = std::string( "01" ) + "\xB0"
    + std::string( "02" ) + "\x27"
    + std::string( "03,010" ) + "\x22"
    + "\xD1.\xD8.";
  ASSERT_TRUE( degree.fromDegreesString( degreeString ) );
  ASSERT_TRUE( isEqual( 1. + 2. / DEGREE_RATIO
    + 3.01 / ( DEGREE_RATIO * DEGREE_RATIO ),
    degree.get( ) ) );

  //! "000°00'00.00"В.Д."
  degreeString = std::string( "000" ) + "\xB0"
    + std::string( "00" ) + "\x27"
    + std::string( "00.00" ) + "\x22"
    + "\xC2.\xC4.";
  ASSERT_TRUE( degree.fromDegreesString( degreeString ) );
  ASSERT_TRUE( isEqual( 0., degree.get( ) ) );

  //! "179°59'59.99"З.Д."
  degreeString = std::string( "179" ) + "\xB0"
    + std::string( "59" ) + "\x27"
    + std::string( "59.99" ) + "\x22"
    + "\xC7.\xC4.";
  ASSERT_TRUE( degree.fromDegreesString( degreeString ) );
  ASSERT_TRUE( isEqual( -179. - 59. / DEGREE_RATIO
    - 59.99 / ( DEGREE_RATIO * DEGREE_RATIO ),
    degree.get( ) ) );

  //! "17.9°59'59.99"З.Д."
  degreeString = std::string( "17.9" ) + "\xB0"
    + std::string( "59" ) + "\x27"
    + std::string( "59.99" ) + "\x22"
    + "\xC7.\xC4.";
  degree.set( 0. );
  ASSERT_FALSE( degree.fromDegreesString( degreeString, true ) );
  ASSERT_TRUE( isEqual( 0., degree.get( ) ) );
  if( std::string( localeconv( )->decimal_point ) == "," )
  {
    ASSERT_TRUE( degree.fromDegreesString( degreeString ) );
    ASSERT_TRUE( isEqual( -17. - 59. / DEGREE_RATIO
      - 59.99 / ( DEGREE_RATIO * DEGREE_RATIO ),
      degree.get( ) ) );
  }
  else
  {
    ASSERT_TRUE( degree.fromDegreesString( degreeString ) );
    ASSERT_TRUE( isEqual( -17.9 - 59. / DEGREE_RATIO
      - 59.99 / ( DEGREE_RATIO * DEGREE_RATIO ),
      degree.get( ) ) );
  }

  //! "17,9°59'59.99"З.Д."
  degreeString = std::string( "17,9" ) + "\xB0"
    + std::string( "59" ) + "\x27"
    + std::string( "59.99" ) + "\x22"
    + "\xC7.\xC4.";
  degree.set( 0. );
  ASSERT_FALSE( degree.fromDegreesString( degreeString, true ) );
  ASSERT_TRUE( isEqual( 0., degree.get( ) ) );
  if( std::string( localeconv( )->decimal_point ) == "." )
  {
    ASSERT_TRUE( degree.fromDegreesString( degreeString ) );
    ASSERT_TRUE( isEqual( -17. - 59. / DEGREE_RATIO
      - 59.99 / ( DEGREE_RATIO * DEGREE_RATIO ),
      degree.get( ) ) );
  }
  else
  {
    ASSERT_TRUE( degree.fromDegreesString( degreeString ) );
    ASSERT_TRUE( isEqual( -17.9 - 59. / DEGREE_RATIO
      - 59.99 / ( DEGREE_RATIO * DEGREE_RATIO ),
      degree.get( ) ) );
  }

  //! "179°59'59.99"И.Д."
  degreeString = std::string( "179" ) + "\xB0"
    + std::string( "59" ) + "\x27"
    + std::string( "59.99" ) + "\x22"
    + "\xC8.\xC4.";
  degree.set( 0. );
  ASSERT_FALSE( degree.fromDegreesString( degreeString, true ) );
  ASSERT_TRUE( isEqual( 0., degree.get( ) ) );
  ASSERT_TRUE( degree.fromDegreesString( degreeString ) );
  ASSERT_TRUE( isEqual( 179. + 59. / DEGREE_RATIO
    + 59.99 / ( DEGREE_RATIO * DEGREE_RATIO ),
    degree.get( ) ) );

  //! "179'59°59.99"З.Д."
  degreeString = std::string( "179" ) + "\xB0"
    + std::string( "59" ) + "\x22"
    + std::string( "59.99" ) + "\x27"
    + "\xC7.\xC4.";
  degree.set( 0. );
  ASSERT_FALSE( degree.fromDegreesString( degreeString ) );
  ASSERT_FALSE( degree.fromDegreesString( degreeString, true ) );
  ASSERT_TRUE( isEqual( 0., degree.get( ) ) );

  //! "DDD°MM'SS.SS"В.Д."
  degreeString = std::string( "DDD" ) + "\xB0"
    + std::string( "MM" ) + "\x27"
    + std::string( "SS.SS" ) + "\x22"
    + "\xC7.\xC4.";
  ASSERT_FALSE( degree.fromDegreesString( degreeString ) );
  ASSERT_FALSE( degree.fromDegreesString( degreeString, true ) );
  ASSERT_TRUE( isEqual( 0., degree.get( ) ) );
}
//==============================================================================
/*!
* \brief Тестирование normalize()
*        Приведение значения радиан в диапазон от -180 до 180
*/
TEST( DegreeTest, DegreeNormalize )
{
  using namespace BasicMath;

  ASSERT_TRUE( isEqual( Degree( -45 ),
    Degree( -6 * WIDE_ANGLE - 45 ).normalize( ) ) );
}
//==============================================================================
/*!
* \brief Тестирование normalizeWithShift()
*        Приведение значения радиан в диапазон от 0 до 360
*/
TEST( DegreeTest, DegreeNormalizeWithShift )
{
  using namespace BasicMath;

  ASSERT_TRUE( isEqual( Degree( 315 ),
    Degree( -6 * WIDE_ANGLE - 45 ).normalizeWithShift( ) ) );
}
//==============================================================================
/*!
* \brief Тестирование математических операций градусов с вещественными числами
*        с двойной точностью
*/
TEST( DegreeTest, DegreeMathOperations )
{
  using namespace BasicMath;

  Degree control( 4. );
  Degree result;

  //! Сложение
  result = Degree( 1. ) + 3.;
  ASSERT_TRUE( isEqual( control, result ) );
  result = Degree( 1. ) + 3.f;
  ASSERT_TRUE( isEqual( control, result ) );
  result = Degree( 1. ) + int( 3 );
  ASSERT_TRUE( isEqual( control, result ) );
  result = Degree( 1. ) + size_t( 3 );
  ASSERT_TRUE( isEqual( control, result ) );

  //! Вычитание
  result = Degree( 7. ) - 3.;
  ASSERT_TRUE( isEqual( control, result ) );
  result = Degree( 7. ) - 3.f;
  ASSERT_TRUE( isEqual( control, result ) );
  result = Degree( 7. ) - int( 3 );
  ASSERT_TRUE( isEqual( control, result ) );
  result = Degree( 7. ) - size_t( 3 );
  ASSERT_TRUE( isEqual( control, result ) );

  //! Умножение
  result = Degree( 2. ) * 2.;
  ASSERT_TRUE( isEqual( control, result ) );
  result = Degree( 2. ) * 2.f;
  ASSERT_TRUE( isEqual( control, result ) );
  result = Degree( 2. ) * int( 2 );
  ASSERT_TRUE( isEqual( control, result ) );
  result = Degree( 2. ) * size_t( 2 );
  ASSERT_TRUE( isEqual( control, result ) );

  //! Деление
  result = Degree( 8. ) / 2.;
  ASSERT_TRUE( isEqual( control, result ) );
  result = Degree( 8. ) / 2.f;
  ASSERT_TRUE( isEqual( control, result ) );
  result = Degree( 8. ) / int( 2 );
  ASSERT_TRUE( isEqual( control, result ) );
  result = Degree( 8. ) / size_t( 2 );
  ASSERT_TRUE( isEqual( control, result ) );

  //! Деление на 0
  ASSERT_THROW(control / 0., MathFailure);
  try
  {
    result = control / 0.;
  }
  catch( const MathFailure& failure )
  {
    ASSERT_EQ( DegreeDivideByZero, failure.getCode( ) );
    ASSERT_EQ( mathLibTitle, failure.getLib( ) );
  }

  ASSERT_THROW(control / 0.f, MathFailure);
  try
  {
    result = control / 0.f;
  }
  catch( const MathFailure& failure )
  {
    ASSERT_EQ( DegreeDivideByZero, failure.getCode( ) );
    ASSERT_EQ( mathLibTitle, failure.getLib( ) );
  }

  ASSERT_THROW(control / int(0), MathFailure);
  try
  {
    result = control / int( 0 );
  }
  catch( const MathFailure& failure )
  {
    ASSERT_EQ( DegreeDivideByZero, failure.getCode( ) );
    ASSERT_EQ( mathLibTitle, failure.getLib( ) );
  }

  ASSERT_THROW(control / size_t(0), MathFailure);
  try
  {
    result = control / size_t( 0 );
  }
  catch( const MathFailure& failure )
  {
    ASSERT_EQ( DegreeDivideByZero, failure.getCode( ) );
    ASSERT_EQ( mathLibTitle, failure.getLib( ) );
  }
}
//==============================================================================
} //! Tests
//==============================================================================
