#include <ExceptionTools/ExceptionTools.hpp>
//! gtest
#include <gtest/gtest.h>
//==============================================================================
namespace ExceptionTools
{
namespace Tests
{
//==============================================================================
enum Failures
{
  LittleFail = ExceptionTools::SpecialFailure, //!< 100
  MediumFail = LittleFail + 11,                //!< 111
  BigFail
};

const std::map<unsigned int, std::string> failuresText =
{ INCLUDE_STANDART_FAILURE_IN_MAP
  { LittleFail, " Little fail "  },
  { MediumFail, " Medium fail "  },
  { BigFail   , " Big fail "     } };
//==============================================================================
class TestFailure : public Failure
{
public:
  TestFailure( unsigned int code,
    const std::string& postfix = "",
    const std::string& prefix = "" )
    : Failure( code, std::string( "Test" ),
      failuresText.at( code ),
      postfix, prefix )
  {}
};
//==============================================================================
class TestInTestFailure : public Failure
{
public:
  TestInTestFailure( unsigned int code,
    const std::string& postfix = "",
    const std::string& prefix = "" )
    : Failure( code, std::string( "TestInTest" ),
      failuresText.at( static_cast< Failures >( code ) ),
      postfix, prefix )
  {}
};
//==============================================================================
class TestTwinTestFailure : public Failure
{
public:
  TestTwinTestFailure( unsigned int code,
    const std::string& postfix = "",
    const std::string& prefix = "" )
    : Failure( code, std::string( "TestTwinTest" ),
      failuresText.at( static_cast< Failures >( code ) ),
      postfix, prefix )
  {}
};
//==============================================================================
/*!
* \brief Тестирование создания исключения
*/
TEST( ExceptionToolsTest, CreateFailure )
{
  for( const auto& value : failuresText )
  {
    ASSERT_THROW( throw TestFailure( value.first ), Failure );

    try
    {
      throw TestFailure( value.first );
    }
    catch( const Failure& failure )
    {
      ASSERT_EQ( value.first, failure.getCode( ) );
      ASSERT_EQ( std::string( "Test" ), failure.getLib( ) );
      ASSERT_EQ( "[Test] " + value.second, std::string( failure.what( ) ) );
    }
  }
}
//==============================================================================
/*!
* \brief Тестирование проброски исключения
*/
TEST( ExceptionToolsTest, CreateFailureInFailure )
{
  //! Двойной транзит наших исключений
  try
  {
    try
    {
      try
      {
        throw TestTwinTestFailure( BigFail, "Postfix\n", "Prefix" );
      }
      catch( ... )
      {
        FailureHandler<TestInTestFailure>( "[Middle Test]" );
      }
    }
    catch( ... )
    {
      FailureHandler<TestFailure>( "[Up Test]" );
    }
  }
  catch( const Failure& failure )
  {
    ASSERT_EQ( std::string( "[Test] [Up Test] <Transit failure>\n"
      "[TestInTest] [Middle Test] <Transit failure>\n"
      "[TestTwinTest] Prefix Big fail Postfix\n" ),
      std::string( failure.what( ) ) );
  }

  //! Транзит стандартного исключения: выход за пределы массива
  try
  {
    try
    {
      std::vector<int> mas( 3 );
      for( int i = 0; i <= 3; i++ )
      {
        mas.at( i ) = i;
      }
    }
    catch( ... )
    {
      FailureHandler<TestFailure>( "[Up Test]" );
    }
  }
  catch( const Failure& failure )
  {
    ASSERT_TRUE( std::string( failure.what( ) )
      .find( std::string( "[Test] [Up Test] <Third-party "
        "exception> Original message:" ) )
      < std::string( failure.what( ) ).size( ) );
  }
}
//==============================================================================
} //! Tests
} //! ExceptionTools
//==============================================================================