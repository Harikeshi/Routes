#pragma once

#include <fstream>
#include <iostream>
#include <vector>

namespace Operations {

inline std::string doubleToString(const double& val, const size_t& precision = 1)
{
    std::string str = std::to_string(val);
    str.erase(str.find('.') + 1 + precision, std::string::npos);
    return str;
}

/*!
 * \brief initTable
 * \details Записывает резултат метрических вычислений в таблицу
 * \param metric [in] метрики
 * \param path [in] путь до файла
 */
inline void writeTable(const std::vector<std::string> title, const std::vector<std::vector<std::string>>& values, const std::string& path)
{
    std::ofstream file(path);
    if (!file.is_open())
    {
        throw std::runtime_error("Не удалось открыть файл!");
    }

    for (size_t i = 0; i < title.size(); ++i)
    {
        file << title[i] << ";";
    }
    file << std::endl;

    for (auto valueData : values)
    {
        for (size_t i = 0; i < valueData.size(); ++i)
        {
            file << valueData[i] << ";";
        }
        file << std::endl;
    }
}

} // namespace Operations
