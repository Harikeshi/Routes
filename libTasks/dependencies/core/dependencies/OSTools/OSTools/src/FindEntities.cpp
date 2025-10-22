#include <OSTools/FindFiles.hpp>
#include <OSTools/FindDirectories.hpp>
#include <OSTools/OSException.hpp>
//! std
#include <algorithm>
#include <fstream>
#ifdef __linux__
#include <dirent.h>
#else
#include <windows.h>
#include <tchar.h>
#endif
//==============================================================================
enum EntityType
{
  RegularFile,
  Directory
};
//==============================================================================
std::forward_list<std::string>
findEntities(
  EntityType entityType,
  const std::string& directory,
  const std::string& ending /*= ""*/)
{
  //!< Итоговый список.
  std::forward_list<std::string> result;
  //!< Итератор итогового списка.
  auto                           it(result.before_begin());
  //!< Скорректированный путь директории.
  std::string     correctedDirectory(directory);
  /*!
   * Для корректной склейки
   * нужно наличие слэша между именем директории и именем файла.
   */
  if ((correctedDirectory.back() != '/') && (correctedDirectory.back() != '\\'))
    correctedDirectory.push_back('/');
#ifdef __linux__
  //! На Linux используем POSIX-функции.

  //!< Размер окончания.
  std::size_t    sizeOfEnding(ending.size());
  //!< Указатель на директорию.
  DIR*           dir;
  //!< Указатель на вхождение директории.
  struct dirent* ent;

  //! Пробуем открыть директорию.
  if ((dir = opendir(directory.c_str())) != NULL)
  {
    //! Если получилось, проходим по всем вхождениям.
    while ((ent = readdir(dir)) != NULL)
    {
      //!< Имя вхождения.
      std::string fileName(ent->d_name);
      /*!
       * Если имя вхождения меньше длины заданного окончания,
       * вхождение нам точно не подходит.
       */
      if (fileName.size() >= sizeOfEnding)
        /*!
         * Проверяем, соответствует ли его имя нашему требованию.
         */
        if (fileName.substr(fileName.size() - sizeOfEnding, sizeOfEnding)
              == ending)
          //! Нужная ли это нам сущность?
          if (((entityType == RegularFile) && (ent->d_type == DT_REG))
              || ((entityType == Directory) && (ent->d_type == DT_DIR)))
            //! Если да, добавляем путь сущности в список.
            it = result.emplace_after(it, correctedDirectory + fileName);
    }
  }
  else
  {
    /*!
     * Если не получилось открыть директорию,
     * генерируем соответствующее исключение.
     */
    throw OSFailure(OSEnumFailure::CouldNotOpenDirectory);
  }
#else
  //! На Windows пользуемся местным API.

  //!< Информация о найденном файле.
  WIN32_FIND_DATA findFileData;
  //!< Обработчик поиска файлов.
  HANDLE          hFind;

  //! Ищем первый файл, соответствующий нашему требованию, в директории.
  hFind = FindFirstFile(
    (correctedDirectory + "*" + ending).c_str(),
    &findFileData);
  if (hFind != INVALID_HANDLE_VALUE)
    //! Если нашли, продолжаем поиски всех файлов до конца.
    do
      /*!
       * Проверяем,
       * относится ли рассматриваемая сущность к нужной нам категории.
       */
      if (((entityType == RegularFile)
           && ((findFileData.dwFileAttributes & FILE_ATTRIBUTE_NORMAL)
            || (findFileData.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE)))
        || ((entityType == Directory)
          && (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)))
        //! Если всё в порядке, добавляем путь файла в список.
        it = result.emplace_after(
          it,
          correctedDirectory + findFileData.cFileName);
  while (FindNextFile(hFind, &findFileData) != 0);

  if ((GetLastError() != ERROR_NO_MORE_FILES)
    && (GetLastError() != ERROR_FILE_NOT_FOUND))
    //! Если попалась какая-то нештатная ошибка...
    if (GetLastError() == ERROR_PATH_NOT_FOUND)
      //! О ненахождении директории сообщаем отдельно.
      throw OSFailure(OSEnumFailure::CouldNotOpenDirectory);
    else
      //! Иначе выдаём полученный код ошибки.
      throw OSFailure(OSEnumFailure::UnrecognizedFileSystemError,
        "winerror code: " + GetLastError());

  //! Закрываем обработчик.
  FindClose(hFind);
#endif
  //! Возвращаем результат.
  return result;
}
//==============================================================================
std::forward_list<std::string> OS::
findFiles(const std::string& directory, const std::string& ending /*= ""*/)
{
  return findEntities(RegularFile, directory, ending);
}
//==============================================================================
std::forward_list<std::string> OS::
findDirectories(
  const std::string& sourceDirectory,
  const std::string& ending /*= ""*/)
{
  return findEntities(Directory, sourceDirectory, ending);
}
//==============================================================================
bool OS::
isDirectoryExist(
  const std::string& sourceDirectory,
  const std::string& directoryToFindName)
{
  //!< Результат поиска директорий.
  auto        result(OS::findDirectories(sourceDirectory, directoryToFindName));
  //!< Скорректированный путь директории.
  std::string directoryToFind(sourceDirectory);
  /*!
  * Для корректной склейки
  * нужно наличие слэша между именем директории и именем файла.
  */
  if ((directoryToFind.back() != '/')
    && (directoryToFind.back() != '\\'))
    directoryToFind.push_back('/');
  directoryToFind += directoryToFindName;

  return std::find(result.begin(), result.end(), directoryToFind)
    != result.end();
}
//==============================================================================
bool OS::isFileExists(const std::string& fileName)
{
  std::fstream fileStream(fileName.c_str(), std::ios_base::in);
  return (fileStream.is_open());
}
//==============================================================================

