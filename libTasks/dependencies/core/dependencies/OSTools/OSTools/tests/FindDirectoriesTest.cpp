#include <OSTools/FindDirectories.hpp>

#include <OSTools/OSException.hpp>
//! gtest
#include <gtest/gtest.h>
// std
#include <algorithm>
//==============================================================================
namespace OS
{
namespace Tests
{
//==============================================================================
/*!
 * \brief Тестирование нахождения директорий с определённым окончанием
 * в определённой директории
 */
TEST(FindDirectoriesTest, FindDirectories)
{
  //! Ищем все директории с названием, оканчивающимся на "txt".

  std::forward_list<std::string> result(
    findDirectories("./OSToolsTestRes/FindDirectories/", "txt"));
  ASSERT_NE(
    std::find(
      result.begin(),
      result.end(),
      "./OSToolsTestRes/FindDirectories/dir1txt"),
    result.end());
  ASSERT_NE(
    std::find(
      result.begin(),
      result.end(),
      "./OSToolsTestRes/FindDirectories/dir2txt"),
    result.end());
  ASSERT_NE(
    std::find(
      result.begin(),
      result.end(),
      "./OSToolsTestRes/FindDirectories/dir3txt"),
    result.end());
#ifdef __linux__
  ASSERT_NE(
    std::find(
      result.begin(),
      result.end(),
      "./OSToolsTestRes/FindDirectories/директория4txt"),
    result.end());
#else
  /*!
  * Названия директорий у нас в ANSI,
  * так что не удивляйтесь следующей строчке :)
  */
  ASSERT_NE(
    std::find(
      result.begin(),
      result.end(),
      std::string("./OSToolsTestRes/FindDirectories/")
      + std::string{-28, -24, -16, -27, -22, -14, -18, -16, -24, -1 }
      +"4txt"),
    result.end());
#endif
  ASSERT_EQ(
    std::find(
      result.begin(),
      result.end(),
      "./OSToolsTestRes/FindDirectories/filetxt"),
    result.end());

  //! Пытаемся найти несуществующие директории.

  result = findDirectories("./OSToolsTestRes/FindDirectories/", "ahahaha");
  ASSERT_TRUE(result.empty());

  //! Пытаемся искать директории в несуществующей директории.

  bool isFailureThrown(false);
  try
  {
    result = findDirectories(
      "./OSToolsTestRes/FindDirectories/donotexist/donotexist",
      ".txt");
  }
  catch (const Failure& ex)
  {
    isFailureThrown = true;
    ASSERT_EQ(OS_LIB_TITLE, ex.getLib());
    ASSERT_EQ(
      OSEnumFailure::CouldNotOpenDirectory,
      ex.getCode());
  }
  ASSERT_TRUE(isFailureThrown);
  ASSERT_TRUE(result.empty());

  //! Ищем все директории.

  result = findDirectories("./OSToolsTestRes/FindDirectories/");
  ASSERT_NE(
    std::find(
      result.begin(),
      result.end(),
      "./OSToolsTestRes/FindDirectories/dir1txt"),
    result.end());
  ASSERT_NE(
    std::find(
      result.begin(),
      result.end(),
      "./OSToolsTestRes/FindDirectories/dir2txt"),
    result.end());
  ASSERT_NE(
    std::find(
      result.begin(),
      result.end(),
      "./OSToolsTestRes/FindDirectories/dir3txt"),
    result.end());
#ifdef __linux__
  ASSERT_NE(
    std::find(
      result.begin(),
      result.end(),
      "./OSToolsTestRes/FindDirectories/директория4txt"),
    result.end());
#else
  /*!
  * Названия директорий у нас в ANSI,
  * так что не удивляйтесь следующей строчке :)
  */
  ASSERT_NE(
    std::find(
      result.begin(),
      result.end(),
      std::string("./OSToolsTestRes/FindDirectories/")
      + std::string{ -28, -24, -16, -27, -22, -14, -18, -16, -24, -1 }
      +"4txt"),
    result.end());
#endif
  ASSERT_NE(
    std::find(
      result.begin(),
      result.end(),
      "./OSToolsTestRes/FindDirectories/dir5nontxt"),
    result.end());
  ASSERT_EQ(
    std::find(
      result.begin(),
      result.end(),
      "./OSToolsTestRes/FindDirectories/filetxt"),
    result.end());
  ASSERT_NE(
    std::find(
      result.begin(),
      result.end(),
      "./OSToolsTestRes/FindDirectories/."),
    result.end());
  ASSERT_NE(
    std::find(
      result.begin(),
      result.end(),
      "./OSToolsTestRes/FindDirectories/.."),
    result.end());

  std::size_t count(0);
  for (auto it(result.begin()); it != result.end(); ++it)
    ++count;
  ASSERT_EQ(7, count);
}
//==============================================================================
/*!
 * \brief Тестирование определения существования директории.
 */
TEST(FindDirectoriesTest, IsDirectoryExist)
{
  ASSERT_TRUE(
    isDirectoryExist("./OSToolsTestRes/FindDirectories", "dir1txt"));
  ASSERT_TRUE(
    isDirectoryExist("./OSToolsTestRes/FindDirectories/", "dir1txt"));
  ASSERT_FALSE(
    isDirectoryExist("./OSToolsTestRes/FindDirectories", "dir1111txt"));
}
//==============================================================================
} //! Tests
} //! OS
//==============================================================================
