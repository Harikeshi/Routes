#include <MathTools/Optimization/GAPApproximation.hpp>
//! MathTools
#include <MathTools/Optimization/KnapsackProblem.hpp>
#include <MathTools/MathException.hpp>
//! std
#include <limits>
//=============================================================================
namespace Optimization {
//=============================================================================
typedef std::vector<int>         intVector;   //!< Вектор из int.
typedef std::vector<intVector>   intMatrix;   //!< Матрица из int.
typedef std::vector<float>       floatVector; //!< Вектор из float.
typedef std::vector<floatVector> floatMatrix; //!< Матрица из float.
//=============================================================================
intVector
GAPApproximation(
  const intMatrix& users,
  const intVector& providers,
  const floatMatrix& rates)
{
  //!< Количество потребителей.
  const std::size_t countOfUsers(users.size());
  //!< Количество поставщиков.
  const std::size_t countOfProviders(providers.size());

  //! Проверяем корректность исходных данных.
  std::size_t matrixHeight(users.size());
  if (rates.size() != matrixHeight)
    throw MathFailure(MathEnumFailure::GAPApproximationBadParametersSizes);

  if (matrixHeight == 0)
    if ((countOfProviders == 0) && (countOfUsers == 0))
      return std::vector<int>();
    else
      throw MathFailure(MathEnumFailure::GAPApproximationBadParametersSizes);

  std::size_t matrixWidth(users[0].size());
  for (const auto& matrixRow: users)
    if (matrixRow.size() != matrixWidth)
      throw MathFailure(MathEnumFailure::GAPApproximationBadParametersSizes);
  for (const auto& matrixRow : rates)
    if (matrixRow.size() != matrixWidth)
      throw MathFailure(MathEnumFailure::GAPApproximationBadParametersSizes);
  if (providers.size() != matrixWidth)
    throw MathFailure(MathEnumFailure::GAPApproximationBadParametersSizes);

  for (const auto& matrixRow : users)
    for (int weight : matrixRow)
      if (weight <= 0)
        throw MathFailure(MathEnumFailure::GAPApproximationNonNaturalUserValue);
  for (int capacity : providers)
    if (capacity < 0)
      throw MathFailure(MathEnumFailure::GAPApproximationNegativeProviderValue);
  for (const auto& matrixRow : rates)
    for (float profit : matrixRow)
      if (profit < -std::numeric_limits<float>().epsilon())
        throw MathFailure(MathEnumFailure::GAPApproximationNegativeRateValue);

  //!< Вектор-результат.
  intVector result(countOfUsers, -1);
  /*! 
   * Инициализируем его, считая,
   * что ни на одного потребителя не назначен поставщик.
   */

  //!< "Прибыль" за обслуживание каждого из потребителей.
  floatVector currentRatingForProvider(users.size());
  /*!
   * Она формируется для каждого поставщика
   * в зависимости от выбора предыдущих поставщиков.
   */

  /*!<
   *  Номера потребителей, выбранные для поставщика
   *  в результате решения задачи о рюкзаке 0-1.
   */
  std::vector<std::size_t> approvedUsersNumbers;

  //! Для каждого поставщика по очереди решаем задачу о рюкзаке 0-1.
  for (std::size_t providerNumber(0);
       providerNumber < countOfProviders;
       ++providerNumber)
  {
    //! Формируем персональный набор прибылей для каждого поставщика.
    for (std::size_t userNumber(0); userNumber < countOfUsers; ++userNumber)

      /*!
       * Если потребитель уже "занят" каким-либо поставщиком,
       * записываем разность стоимости для текущего поставщика
       * и стоимости для "занявшего" потребителя поставщика.
       */

      /*!
       * Если выбрать именно этого поставщика именно для этого потребителя
       * будет достаточно выгодно, потребитель "перейдёт к нему".
       */
      if (result[userNumber] != -1)
        currentRatingForProvider[userNumber] =
          rates[userNumber][providerNumber] -
            rates[userNumber][result[userNumber]];
      /*!
       * Если же потребитель "свободен",
       * записываем просто прибыль от него для текущего поставщика.
       */
      else
        currentRatingForProvider[userNumber] =
          rates[userNumber][providerNumber];

    //! Готовим данные на вход к другому алгоритму.

    //!< Вектор весов потребителей для текущего фиксированного поставщика.
    intVector currentWeightsForProvider;
    //! Веса потребителей не меняются.
    currentWeightsForProvider.reserve(countOfUsers);
    for (const intVector& user : users)
      currentWeightsForProvider.emplace_back(user[providerNumber]);

    //! Наконец решаем задачу о рюкзаке 0-1.
    approvedUsersNumbers = knapsackProblemSolution(
                             providers[providerNumber],
                             currentWeightsForProvider,
                             currentRatingForProvider);

    //! Обновляем значение вектора соответствия.
    for (std::size_t number : approvedUsersNumbers)
      result[number] = static_cast<int>(providerNumber);
  }

  //! Далее -- дополнительная часть алгоритма.

  //!< Флаг присутствия потребителей без поставщиков.
  bool isThereEmptyUser(false);

  //! Вычисляем isThereEmptyUser
  for (std::size_t userNumber(0); userNumber < countOfUsers; ++userNumber)
    if (result[userNumber] == -1)
    {
      isThereEmptyUser = true;
      break;
    }

  //! Если все потребители удовлетворены, спокойно покидаем алгоритм.
  if (!isThereEmptyUser)
    return result;

  /*!
   * Иначе пробуем назначить оставшимся потребителям поставщиков
   * (в произвольном порядке).
   */

  /*!
   * Перед этим пересчитываем оставшийся
   * после основного алгоритма запас у поставщиков.
   */
  intVector providersLeft(providers);
  for (std::size_t userNumber(0); userNumber < countOfUsers; ++userNumber)
    if (result[userNumber] != -1)
      providersLeft[result[userNumber]] -= 
        users[userNumber][result[userNumber]];

  /*!
   * Пытаемся назначить всем оставшимся потребителям поставщиков
   * в произвольном порядке.
   */
  for (std::size_t userNumber(0); userNumber < countOfUsers; ++userNumber)
    if (result[userNumber] == -1)
      for (std::size_t providerNumber(0);
           providerNumber < countOfProviders;
           ++providerNumber)
        if ((providers[providerNumber] >= users[userNumber][providerNumber])
            &&
            (rates[userNumber][providerNumber] >
              std::numeric_limits<float>::epsilon()))
        {
          result[userNumber] = static_cast<int>(providerNumber);
          providersLeft[providerNumber] -= users[userNumber][providerNumber];
        };

  return result;
}
//=============================================================================
} //! Optimization