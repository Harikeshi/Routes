#include <OSTools/ExpandEvironmentVariables.hpp>
//! gtest
#include <gtest/gtest.h>
//==============================================================================
namespace OS
{
namespace Tests
{
//==============================================================================
#if defined(_WIN64) || defined(_WIN32)
/*!
 * \brief Тестирование декодирования переменных среды Windows.
 */
TEST(ExpandEnvironmentVariablesTest, ExpandEnvironmentVariables)
{
  _putenv("EXPAND_ENVIRONMENT_VARIABLES_TEST=only_for_windows");

  ASSERT_EQ(
    std::string("This test is only_for_windows!"),
    expandEnvironmentVariables(
      "This test is %EXPAND_ENVIRONMENT_VARIABLES_TEST%!"));

  _putenv("EXPAND_ENVIRONMENT_VARIABLES_TEST=");
}
//==============================================================================
#endif
//==============================================================================
} //! Tests
} //! OS
//==============================================================================

