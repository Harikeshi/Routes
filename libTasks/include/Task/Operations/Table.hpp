#pragma once

#include <Task/Entities/Efficiency.hpp>
#include <fstream>
#include <iostream>

namespace Data {

inline void initTable(const std::string& path)
{
    std::ofstream file(path);
    if (!file.is_open())
    {
        throw std::runtime_error("Не удалось открыть файл!");
    }

    file << "Схема;"
         << "Размер;"
         << "Время;"
         << "Дальность;"
         << "Средняя производительность;"
         << "Среднее время" << std::endl;
}

inline void addTable(const std::string& path, const std::string& scheme, const std::string& size,
                     const double& searchTime, const double& detRange, const double& prod, const double& time)
{
    std::ofstream file(path, std::ios::app);
    if (!file.is_open())
    {
        throw std::runtime_error("Не удалось открыть файл!");
    }
    file << scheme << ";" << size << ";" << searchTime << ";"
         << detRange << ";" << prod << ";" << time << std::endl;
}

} // namespace Data
