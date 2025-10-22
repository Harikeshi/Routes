#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace Operations {

inline std::vector<int> split(const std::string& line)
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

inline void save(const std::vector<std::vector<int>>& m, const std::string& path)
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

inline std::vector<std::vector<int>> load(const std::string& path)
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
        std::vector<int> row = split(line);
        result.push_back(row);
    }
    file.close();
    return result;
}

} // namespace Operations
