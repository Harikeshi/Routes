#pragma once

/*! std */
#include <random>

namespace Operations {
/**
 * @brief Статический класс генерации случайных чисел.
 * 
 */
class Random
{
public:
    /**
 * @brief Функция получения случайного числа типа double. 
 * 
 * @param from Минимальное значение случайной величины;
 * @param to Максимальное значение случайной величины;
 * @return double Случайная величина.
 */
    static double getRandom(const double& from, const double& to)
    {
        std::random_device rd;
        std::mt19937 gen(rd());

        std::uniform_real_distribution<double> unifX(from, to);

        double r = unifX(gen);

        return r;
    }
};
} // namespace Operations
