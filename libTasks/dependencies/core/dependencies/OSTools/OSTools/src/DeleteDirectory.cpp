#include <OSTools/DeleteDirectory.hpp>
//! std
#include <cstdlib>
#ifndef __linux__
#include <windows.h>
#include <direct.h>
#endif
//==============================================================================
#ifndef __linux__
/*!
 * \brief deleteDirectoryWindows -- функция,
 * удаляющая внутри определённой директории
 * другую директорию с заданным именем.
 *
 * \param[in] directoryToDelete -- путь удаляемой директории.
 *
 * \return -- 0, если директория была успешно удалена;
 *            любое значение, кроме 0 -- иначе.
 */
int deleteDirectoryWindows(const std::string& directoryToDelete) noexcept
{
  //!< Информация о найденном файле.
  WIN32_FIND_DATA fileInformation;
  //!< Обработчик поиска файлов.
  HANDLE          hFile(
    FindFirstFile((directoryToDelete + "/*").c_str(), &fileInformation));
  //! Если новый файл успешно найден...
  if (hFile != INVALID_HANDLE_VALUE)
  {
    //! Проходим по всем файлам в директории.
    do
    {
      //!< Имя очередного найденного файла.
      std::string fileName(fileInformation.cFileName);
      //! Если найденный файл -- не текущая и не объемлющая директория...
      if ((fileName != ".") && (fileName != ".."))
      {
        //!< Путь очередного найденного файла.
        std::string strFilePath(directoryToDelete + "/" + fileName);
        //! Если файл -- директория...
        if (fileInformation.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
          //!< Код завершения удаления вложенной директории.
          int iRC(deleteDirectoryWindows(strFilePath));
          /*!
           * Если при удалении вложенной директории возникла ошибка,
           * возвращаем её код наверх.
           */
          if (iRC != 0)
            return iRC;
        }
        else
        {
          //! Пробуем выставить файлу нужный атрибут.
          if (SetFileAttributes(
                strFilePath.c_str(), FILE_ATTRIBUTE_NORMAL) == FALSE)
            return GetLastError();
          //! Пробуем удалить файл.
          if (DeleteFile(strFilePath.c_str()) == FALSE)
            return GetLastError();
        }
      }
      //! Продолжаем, пока получается найти очередной файл в директории.
    } while (FindNextFile(hFile, &fileInformation) == TRUE);

    //!< Код последней возникшей ошибки.
    DWORD dwError(GetLastError());
    /*!
     * Если ошибка не связана с тем, что мы просмотрели все файлы в директории,
     * считаем, что удаление окончилось неуспешно.
     */
    if (dwError != ERROR_NO_MORE_FILES)
      return dwError;
  }

  //! Заканчиваем поиск файлов.
  FindClose(hFile);

  //! Удаляем текущую директорию.

  //! Пробуем удалить файл.
  if (_rmdir(directoryToDelete.c_str()) != 0)
    return static_cast<int>(_doserrno);

  return 0;
}
//==============================================================================
#endif
namespace OS
{
//==============================================================================
bool
deleteDirectory(
  const std::string& sourceDirectory,
  const std::string& directoryToDeleteName) noexcept
{
#ifdef __linux__
  return system(
           (std::string("rm -rf ")
             + sourceDirectory
             + "/"
             + directoryToDeleteName).c_str())
           != -1;
#else
  return deleteDirectoryWindows(sourceDirectory + "/" + directoryToDeleteName)
           == 0;
#endif
}
//==============================================================================
} //! OS
//==============================================================================