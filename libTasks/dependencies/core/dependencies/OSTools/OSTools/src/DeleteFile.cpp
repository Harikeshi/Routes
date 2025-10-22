#include <OSTools/DeleteFile.hpp>

#include <OSTools/FindFiles.hpp>
//==============================================================================
namespace OS
{
//==============================================================================
void deleteFile(const std::string& fileName) noexcept
{
  //! Если файл существует, то удаляем его
  if (OS::isFileExists(fileName))
    remove(fileName.c_str());
  else
    return;
}
//==============================================================================
void deleteShpFile(const std::string& fileName) noexcept
{
  //! Удаляем файл
  deleteFile(fileName);
  
  //! Определение типа файла
  std::string fileType(fileName.substr(fileName.find_last_of(".")));

  //! Для ".shp" удаляем дополнительно все вспомогательные
  if (fileType == ".shp")
  {
    std::string fileNameClear(fileName.substr(0, fileName.find_last_of(".")));
    deleteFile(fileNameClear + ".dbf");
    deleteFile(fileNameClear + ".prj");
    deleteFile(fileNameClear + ".shx");
  }
}
//==============================================================================
} //! OS
//==============================================================================