#include <OSTools/CreateDirectory.hpp>

#include <OSTools/FindDirectories.hpp>
#include <OSTools/DeleteDirectory.hpp>
//! gtest
#include <gtest/gtest.h>
//==============================================================================
namespace OS
{
namespace Tests
{
//==============================================================================
/*!
 * \brief Тестирование создания директории.
 */
TEST(CreateDirectoryTest, CreateDirectory)
{
  ASSERT_TRUE(createDirectory("./", "my-dir"));
  ASSERT_TRUE(isDirectoryExist("./", "my-dir"));
  deleteDirectory("./", "my-dir");
}
//==============================================================================
}//! Tests
}//! OS
//==============================================================================
