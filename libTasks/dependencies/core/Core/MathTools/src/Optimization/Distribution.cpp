#include <MathTools/Optimization/Distribution.hpp>
//! MathExceptions
#include <MathTools/MathException.hpp>
//! Optimization
#include <MathTools/Optimization/KnapsackProblem.hpp>
//! std
#include <algorithm>
#include <functional>
//==============================================================================
namespace Optimization 
{
//==============================================================================
int estimateEquivalentDistribution(
  const std::vector<unsigned int>& multilinkedUser, 
  const std::vector<unsigned int>& simplyConnectedUsers, 
  const std::vector<unsigned int>& providers)
{
  /*
   * \brief Преобразование неупорядоченного вектора неотрицательных целых чисел
   *        в упорядоченный вектор целых чисел
   * \param[in] uintVector - неупорядоченый вектор неотрицательных целых чисел
   * \return Упорядоченный вектор целых чисел
  */
  auto toSortIntVector = [] (const std::vector<unsigned int>& uintVector)
       -> std::vector<int>
  {
    //! перегонка в вектор целых чисел
    std::vector<int> result;
    for (auto value : uintVector)
      result.emplace_back(value);
    //! сортировка
    std::sort(result.begin(), result.end());
    //! возвращаем результат
    return result;
  };

  //! Необработанные массивы исходных данных:
  //! Односвязные потребители, упорядоченные по возрастанию 
  std::vector<int> rawSimplyUsers = toSortIntVector(simplyConnectedUsers);
  //! Поставщики, упорядоченные по возрастанию 
  std::vector<int> rawProviders = toSortIntVector(providers);

  //! Инициализация результата
  int result(0);

  //! Подбираем поставщиков для односвязных сущностей

  //! Проходимся по односвязным потребителям от больших запросов к меньшим 
  for (int i = static_cast<int>(rawSimplyUsers.size()) - 1; i >= 0; --i)
  {
    int selectProvider(-1); //!< индекс подходящего поставщика
    //! Проходимся по списку поставщиков от меньших запасов
    for (size_t j = 0; j < rawProviders.size(); j++)
    {
      if (rawSimplyUsers[i] <= rawProviders[j])
      { //!< поставщик найден
        selectProvider = static_cast<int>(j);
        break;
      }
    }
    if (selectProvider >= 0)
    {
      //!< поставщик найден - убираем его из решения (излишки блокируем)
      //!  односвязного потребителя тоже исключаем из дальнейшего решения
      rawProviders.erase(rawProviders.begin() + selectProvider);
      rawSimplyUsers.erase(rawSimplyUsers.begin() + i);
    }
  }

  //! Делаем второй проход по односвязным потребителям и пытаемся подобрать 
  //! наиболее подходящих по запасам поставщиков
  for (int i = static_cast<int>(rawSimplyUsers.size() - 1); i >= 0; --i)
  {
    int selectProvider = static_cast<int>(rawProviders.size()) - 1;
    if (selectProvider < 0)
      //! поставщики закончились - добhасываем запрос в нехватку
      result -= rawSimplyUsers[i];
    else 
    {
      //! Есть незадействованный поставщик:
      //! - добавляем разницу в нехватку
      //! - исключаем поставщика из дальнейшего решения
      result -= (rawSimplyUsers[i] - rawProviders[selectProvider]);
      rawProviders.erase(rawProviders.begin() + selectProvider);
    }
  }
 
  //! Вычисляем общие запросы многосвязных потребителей
  int multiResult(0);
  for (auto user : multilinkedUser)
    multiResult -= user;

  //! Запасы оставшихся потставщиков исключаем из запросов многосвязных 
  //! потребителей
  for (auto provider : rawProviders)
    multiResult += provider;

  //! Подводим итоги
  if (multiResult <= 0)
    //! Если не хватает запасов на многосвязных, то добавляем к нехваткам по 
    //! односвязным
    return result + multiResult; 
  else if (result < 0)
    //! Если на многосвязных есть запас, то нам он не интересен, если есть 
    //! нехватка на односвязных
    return result;

  //! Если везде всё хорошо - то возвращаем запас относительно многосвязных
  return multiResult;
}
//==============================================================================
} //! Optimization
//==============================================================================
