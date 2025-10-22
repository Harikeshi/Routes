#include <MathTools/Optimization/KnapsackProblem.hpp>
// MathTools
#include <MathTools/MathException.hpp>
//=============================================================================
namespace Optimization {
//=============================================================================
typedef std::vector<int>         intVector;   //!< Вектор из int.
typedef std::vector<intVector>   intMatrix;   //!< Матрица из int.
typedef std::vector<float>       floatVector; //!< Вектор из float.
typedef std::vector<floatVector> floatMatrix; //!< Матрица из float.
//=============================================================================
std::vector<std::size_t>
knapsackProblemSolution(
  int knapsackCapacity,
  const intVector& itemsWeights,
  const floatVector& itemsRating)
{
  //!< Количество предметов.
  const std::size_t countOfItems(itemsWeights.size());

  //! Проверяем корректность исходных данных.
  if (itemsRating.size() != countOfItems)
    throw MathFailure(MathEnumFailure::KnapsackProblemDifferentDimCounts);
  if (knapsackCapacity < 0)
    throw MathFailure(MathEnumFailure::KnapsackProblemNegativeCapacity);
  for (int weight : itemsWeights)
    if (weight < 0)
      throw MathFailure(MathEnumFailure::KnapsackProblemNegativeWeightValue);

  //!< Матрица максимальной выгоды.
  std::vector<std::vector<std::pair<float, std::vector<std::size_t>>>>
    maxRatingMatrix;
  maxRatingMatrix.resize(countOfItems + 1);

  /*!
   *  Алгоритм построен на последовательном заполнении элементов этой матрицы.
   * каждый элемент с индексами {i, j} содержит следующие данные:
   *
   * 1) Максимальная выгода, которая может быть получена,
   * при использовании первых i предметов из списка при вместимости равной j.
   *
   * 2) Соответствующий набор предметов, на котором это значение реализуется.
   *
   *  После инициализации
   * вычисления ведутся по определённым рекуррентным формулам.
   */

  //! Количество столбцов равно вместимости рюкзака (она целочисленная).
  for (auto& str : maxRatingMatrix)
    str.resize(knapsackCapacity + 1);

  //! Инициализируем первый столбец нулями.
  for (int i(0); i <= knapsackCapacity; ++i)
    maxRatingMatrix[0][i].first = 0.;

  //! Проход по всем предметам.
  for (std::size_t i(1); i <= countOfItems; ++i)
    /*!
     * Проход по всем возможным значениям вместимости рюкзака
     * (по всем неотрицательным числам, не большим,
     * чем настоящая вместимость рюкзака).
     */
    for (int j(0); j <= knapsackCapacity; ++j)
      //! Если текущего запаса вместимости не хватает на текущий предмет...
      if (itemsWeights[i - 1] > j)
        /*! 
         * Прибыль наследуется (не увеличивается)
         * и набор предметов не меняется.
         */
        maxRatingMatrix[i][j] = maxRatingMatrix[i - 1][j];
      /*!
       * Если запаса хватает,
       * но брать этот предмет в текущих условиях невыгодно...
       */
      else if (maxRatingMatrix[i - 1][j].first >=
               maxRatingMatrix[i - 1][j - itemsWeights[i - 1]].first +
                 itemsRating[i - 1])
        /*! 
         * Прибыль наследуется (не увеличивается)
         * и набор предметов не меняется.
         */
        maxRatingMatrix[i][j] = maxRatingMatrix[i - 1][j];
      /*! 
       * Если нам выгодно взять этот предмет, берём его:
       * рассчитываем новое значение суммарной прибыли
       * и дополняем список предметов выбранным предметом.
       */
      else
      {
        //! Вычисляем необходимые данные для этого элемента матрицы.
        maxRatingMatrix[i][j] = maxRatingMatrix[i - 1][j - itemsWeights[i - 1]];
        maxRatingMatrix[i][j].first += itemsRating[i - 1];
        maxRatingMatrix[i][j].second.push_back(i - 1);
      }

  return maxRatingMatrix[countOfItems][knapsackCapacity].second;
}
//=============================================================================
} //! Optimization
//=============================================================================
