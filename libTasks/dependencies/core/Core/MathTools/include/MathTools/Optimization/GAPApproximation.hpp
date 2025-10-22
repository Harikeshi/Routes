#ifndef MATH_TOOLS_OPTIMIZATION_GAP_APPROXIMATION_HPP
#define MATH_TOOLS_OPTIMIZATION_GAP_APPROXIMATION_HPP
//=============================================================================
//! std
#include <vector>
//=============================================================================
namespace Optimization {
//=============================================================================
/*!
 * \brief GAPApproximation (GAP - Generalized Assignment Problem) -- функция,
 * вычисляющая приближённое решение обобщённой проблемы о назначениях
 * за полиномиальное время.
 *
 * \details users и rates должны иметь равные размеры.
 * Количество чисел в providers при этом должно быть равно
 * количеству столбцов в users.
 *
 * \param[in] users - матрица из натуральных чисел,
 * каждая i-ая строка которой содержит информацию
 * о весе i-го потребителя для каждого поставщика.
 *
 * \param[in] providers - вектор из целых неотрицательных чисел,
 * содержащий информацию о запасе ресурсов у каждого из поставщиков.
 *
 * \param[in] rates - матрица из неотрицательных чисел,
 * каждая i-ая строка которой содержит информацию
 * о прибыли от обслуживания i-ого потребителся каждым поставщиком.
 *
 * \return Вектор, каждый i-ый элемент которого содержит номер поставщика
 * удовлетворяющего нужды i-ого потребителя. Значение -1 означает,
 * что на i-ого потребителя поставщик не найден.
 *
 * \throws MathFailure # GAPApproximationBadParametersSizes
 *         MathFailure # GAPApproximationNonNaturalUserValue
 *         MathFailure # GAPApproximationNegativeProviderValue
 *         MathFailure # GAPApproximationNegativeRateValue
 */
std::vector<int>
GAPApproximation(
  const std::vector<std::vector<int>>& users,
  const std::vector<int>& providers,
  const std::vector<std::vector<float>>& rates);
//=============================================================================
} //! Optimization
//=============================================================================
#endif //! MATH_TOOLS_OPTIMIZATION_GAP_APPROXIMATION_HPP