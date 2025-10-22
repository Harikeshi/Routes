#ifndef MATH_TOOLS_OPTIMIZATION_KNAPSACK_PROBLEM_HPP
#define MATH_TOOLS_OPTIMIZATION_KNAPSACK_PROBLEM_HPP
//=============================================================================
//! std
#include <vector>
//=============================================================================
namespace Optimization {
//=============================================================================
/*!
 * \brief knapsackProblemSolurion -- функция, формирующая решение
 * задачи о рюкзаке 0-1 за псевдополиномиальное время.
 *
 * \details Время решения задачи будет возрастать быстрее, чем полиномиально,
 * только с ростом вместимости рюкзака. itemsWeight и itemsRating должны
 * содержать равные количества чисел.
 *
 * \param[in] providerCapacity - вместимость рюкзака
 * (целочисленная и неотрицательная).
 *
 * \param[in] itemsWeights - упорядоченные по номерам предметов массы предметов
 * (целочисленные и неотрицательные).
 *
 * \param[in] itemsRating - упорядоченные по номерам предметов ценности
 * предметов (вещественные).
 *
 * \return Набор номеров вещей, которые поместятся в рюкзак,
 * давая при этом максимальную ценность в сумме.
 *
 * \throws MathFailure # KnapsackProblemDifferentDimCounts
 *         MathFailure # KnapsackProblemNegativeCapacity
 *         MathFailure # KnapsackProblemNegativeWeightValue
 */
std::vector<std::size_t>
knapsackProblemSolution(
  int knapsackCapacity,
  const std::vector<int>& itemsWeights,
  const std::vector<float>& itemsRating);
//=============================================================================
} //! Optimization
//=============================================================================
#endif //! MATH_TOOLS_OPTIMIZATION_KNAPSACK_PROBLEM_HPP