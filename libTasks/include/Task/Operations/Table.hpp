#pragma once

#include <Task/Entities/Efficiency.hpp>
#include <Task/SearchScheme.hpp>
#include <fstream>
#include <iostream>

namespace Operations {

/*!
 * \brief initTable
 * \details Записывает резултат метрических вычислений в таблицу
 * \param metric [in] метрики
 * \param path [in] путь до файла
 */
inline void initTable(std::map<SearchScheme, std::vector<std::string>>& metric, const std::string& path)
{
    std::ofstream file(path);
    if (!file.is_open())
    {
        throw std::runtime_error("Не удалось открыть файл!");
    }

    const std::map<SearchScheme, std::string> scheme = {{SearchScheme::Zigzag, "Зигзаг"},
                                                        {SearchScheme::Shift, "Шифт"},
                                                        {SearchScheme::DeterminedShift, "Д. Шифт"}};
    file << "Схема;"
         << "Размер, м ∙ м;"
         << "Время, с;"
         << "Дальность, м;"
         << "Среднее время, с;"
         << "СКО, с"
         << std::endl;
    for (auto& [indexScheme, valueData] : metric)
    {
        file << scheme.at(indexScheme) << ";";
        for (size_t i = 0; i < valueData.size(); ++i)
        {
            if (i == valueData.size() - 1)
            {
                for (size_t j = valueData[i].size() - 1; j >= 0; --j)
                {
                    if (valueData[i][j] == '0')
                    {
                        valueData[i].pop_back();
                    }
                    else
                    {
                        file << valueData[i] << ";";
                        break;
                    }
                }
            }
            else
            {
                file << valueData[i] << ";";
            }
        }
        file << std::endl;
    }
}

} // namespace Operations
