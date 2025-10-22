#include <OSTools/DeleteFile.hpp>

#include <OSTools/FindFiles.hpp>
//! gtest
#include <gtest/gtest.h>
//! std
#include <fstream>
//==============================================================================
namespace OS
{
namespace Tests
{
//==============================================================================
/*!
 * \brief Тестирование удаления файлов.
 */
TEST(DeleteFileTest, DeleteFile)
{
    //! Вспомогательная функция для создания пустого файла (при его отсутствии)
    auto createEmptyFile = [](const std::string& name){
        std::fstream stream(name, std::ios_base::out);
    };
    //! Создаём пустые файлы
    createEmptyFile("./OSToolsTestRes/test_file.txt");
    createEmptyFile("./OSToolsTestRes/test_file.shp");
    createEmptyFile("./OSToolsTestRes/test_file.dbf");
    createEmptyFile("./OSToolsTestRes/test_file.shx");

    //! Проверяем, что файлы существуют
    ASSERT_TRUE(isFileExists("./OSToolsTestRes/test_file.txt"));
    ASSERT_TRUE(isFileExists("./OSToolsTestRes/test_file.shp"));
    ASSERT_TRUE(isFileExists("./OSToolsTestRes/test_file.dbf"));
    ASSERT_TRUE(isFileExists("./OSToolsTestRes/test_file.shx"));

    //! Удаляем файл и проверяем, что он удалился
    deleteFile("./OSToolsTestRes/test_file.txt");
    ASSERT_FALSE(isFileExists("./OSToolsTestRes/test_file.txt"));

    //! Удаляем .shp файл и проверяем, что удалился он и .dbf, .shx файлы
    deleteShpFile("./OSToolsTestRes/test_file.shp");
    ASSERT_FALSE(isFileExists("./OSToolsTestRes/test_file.shp"));
    ASSERT_FALSE(isFileExists("./OSToolsTestRes/test_file.dbf"));
    ASSERT_FALSE(isFileExists("./OSToolsTestRes/test_file.shx"));
}
//==============================================================================
} //! Tests
} //! OS
//==============================================================================

