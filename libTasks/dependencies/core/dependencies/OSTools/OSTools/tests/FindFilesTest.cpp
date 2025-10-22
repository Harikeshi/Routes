#include <OSTools/FindFiles.hpp>

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
 * \brief Тестирование нахождения файлов с определённым окончанием
 * в определённой директории
 */
TEST(FindFilesTest, FindFiles)
{
  //! Ищем все файлы с названием, оканчивающимся на ".txt".

  std::forward_list<std::string> result(
    findFiles("./OSToolsTestRes/FindFiles/", ".txt"));
  ASSERT_NE(
    std::find(
      result.begin(),
      result.end(),
      "./OSToolsTestRes/FindFiles/file1.txt"),
    result.end());
  ASSERT_NE(
    std::find(
      result.begin(),
      result.end(),
      "./OSToolsTestRes/FindFiles/file2.txt"),
    result.end());
  ASSERT_NE(
    std::find(
      result.begin(),
      result.end(),
      "./OSToolsTestRes/FindFiles/file3.txt"),
    result.end());
#ifdef __linux__
  ASSERT_NE(
    std::find(
      result.begin(),
      result.end(),
      "./OSToolsTestRes/FindFiles/файл4.txt"),
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
      std::string("./OSToolsTestRes/FindFiles/")
      + std::string{ -12, -32, -23, -21 }
      +"4.txt"),
    result.end());
#endif
  ASSERT_EQ(
    std::find(
      result.begin(),
      result.end(),
      ".//FindFiles/dir.txt"),
    result.end());

  //! Пытаемся найти несуществующие файлы с окончанием ".geojson".

  result = findFiles("./OSToolsTestRes/FindFiles/", ".geojson");
  ASSERT_TRUE(result.empty());

  //! Пытаемся искать файлы в несуществующей директории.

  bool isFailureThrown(false);
  try
  {
    result = findFiles("./OSToolsTestRes/FindFiles/donotexist", ".txt");
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

  //! Ищем все файлы.

  result = findFiles("./OSToolsTestRes/FindFiles/");
  ASSERT_NE(
    std::find(
      result.begin(),
      result.end(),
      "./OSToolsTestRes/FindFiles/file1.txt"),
    result.end());
  ASSERT_NE(
    std::find(
      result.begin(),
      result.end(),
      "./OSToolsTestRes/FindFiles/file2.txt"),
    result.end());
  ASSERT_NE(
    std::find(
      result.begin(),
      result.end(),
      "./OSToolsTestRes/FindFiles/file3.txt"),
    result.end());
#ifdef __linux__
  ASSERT_NE(
    std::find(
      result.begin(),
      result.end(),
      "./OSToolsTestRes/FindFiles/файл4.txt"),
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
      std::string("./OSToolsTestRes/FindFiles/")
      + std::string{ -12, -32, -23, -21 }
      +"4.txt"),
    result.end());
#endif
  ASSERT_NE(
    std::find(
      result.begin(),
      result.end(),
      "./OSToolsTestRes/FindFiles/file5.nontxt"),
    result.end());
  ASSERT_EQ(
    std::find(
      result.begin(),
      result.end(),
      "./OSToolsTestRes/FindFiles/dir.txt"),
    result.end());

  std::size_t count(0);
  for (auto it(result.begin()); it != result.end(); ++it)
    ++count;
  ASSERT_EQ(5, count);
}
//==============================================================================
/*!
* \brief Тестирование нахождения файлов с определённым окончанием
* в определённой директории
*/
TEST(FindFilesTest, IsFileExists)
{
  ASSERT_TRUE(isFileExists("./OSToolsTestRes/FindFiles/file1.txt"));
  ASSERT_FALSE(isFileExists("./OSToolsTestRes/FindFiles/file7.txt"));
}
//==============================================================================
} //! Tests
} //! OS
//==============================================================================
