#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace Operations {

/*!
 * \brief split
 * \details
 * \param line
 * \return
 */
inline std::vector<int> splitLine(const std::string& line)
{
    std::vector<int> result;
    std::string number;
    for (size_t i = 0; i < line.size(); ++i)
    {
        if (line[i] != ' ')
        {
            number.push_back(line[i]);
        }
        else
        {
            result.push_back(std::stoi(number));
            number.clear();
        }
    }
    return result;
}

/*!
 * \brief save
 * \details Запись результата плотности в файл
 * \param m [in] плотность
 * \param path [in] путь до файла
 */
template<typename T>
inline void saveMatrix(const std::vector<std::vector<T>>& m, const std::string& path)
{
    std::ofstream file(path);
    if (!file.is_open())
    {
        throw std::runtime_error("Не удалось открыть файл!");
    }
    for (const auto& row : m)
    {
        for (const auto& column : row)
        {
            file << column << " ";
        }
        file << std::endl;
    }
    file.close();
}

/*!
 * \brief load
 * \details Выгрузка плотности из файла
 * \param path [in] путь до файла
 * \return [out] плотность
 */
inline std::vector<std::vector<int>> loadMatrix(const std::string& path)
{
    std::ifstream file(path);
    if (!file.is_open())
    {
        throw std::runtime_error("Не удалось открыть файл!");
    }
    std::vector<std::vector<int>> result;
    std::string line;

    while (std::getline(file, line))
    {
        std::vector<int> row = splitLine(line);
        result.push_back(row);
    }
    file.close();
    return result;
}

} // namespace Operations
