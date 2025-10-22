#ifndef OS_FIND_DIRECTORIES_HPP
#define OS_FIND_DIRECTORIES_HPP
//==============================================================================
//! std
#include <string>
#include <forward_list>
//==============================================================================
namespace OS
{
//==============================================================================
  /*!
   * \brief findDirectories -- функция, возвращающая для директории
   * список путей всех директорий, лежащих непосредственно в ней
   * и имеющих указанное в параметре окончание имени.
   *
   * \param[in] sourceDirectory -- путь директории.
   *
   * \param[in] ending -- строка,
   * которой должны заканчиваться имена искомых директорий.
   *
   * \return -- список путей до искомых директорий,
   * где имена найденных директорий соединены с путём исходной директории.
   *
   * \throws OSFailure # CouldNotOpenDirectory
   *         OSFailure # UnrecognizedFileSystemError
   */
  std::forward_list<std::string>
  findDirectories(
    const std::string& sourceDirectory,
    const std::string& ending = "");
//==============================================================================
/*!
 * \brief isDirectoryExist -- функция, определяющая,
 * существует внутри определённой директории
 * другая директория с заданным именем.
 *
 * \param[in] sourceDirectory -- путь директории,
 * в которой осуществляется поиск другой директории.
 *
 * \param[in] directoryToFindName -- имя искомой директории
 * (без символов типа '/').
 *
 * \return -- true, если искомая директория существует;
 *            false -- иначе.
 *
 * \throws OSFailure # CouldNotOpenDirectory
 *         OSFailure # UnrecognizedFileSystemError
 */
bool
isDirectoryExist(
  const std::string& sourceDirectory,
  const std::string& directoryToFindName);
//==============================================================================
} //! OS
//==============================================================================
#endif //! OS_FIND_DIRECTORIES_HPP