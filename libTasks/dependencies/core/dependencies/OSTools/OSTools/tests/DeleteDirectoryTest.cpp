#include <OSTools/DeleteDirectory.hpp>

#include <OSTools/CreateDirectory.hpp>

#include <OSTools/FindDirectories.hpp>
//! gtest
#include <gtest/gtest.h>
//==============================================================================
namespace OS
{
namespace Tests
{
//==============================================================================
/*!
 * \brief Тестирование удаления директории.
 */
TEST(DeleteDirectoryTest, DeleteDirectory)
{
  createDirectory("./", "my-dir-to-delete");
  ASSERT_TRUE(deleteDirectory("./", "my-dir-to-delete"));
  ASSERT_FALSE(isDirectoryExist("./", "my-dir-to-delete"));
}
//==============================================================================
} //! Tests
} //! OS
//==============================================================================
