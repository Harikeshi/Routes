#ifndef TRIANGULAR_GEOMETRY_EXCEPTIONS_HPP
#define TRIANGULAR_GEOMETRY_EXCEPTIONS_HPP
//==============================================================================
#include <ExceptionTools/ExceptionTools.hpp>
//==============================================================================
using namespace ExceptionTools;
//==============================================================================
/*!
 * \brief Именованные коды генерируемых исключений библиотеки TriangularGeometry.
 */
enum TriangularEnumFailure
{
  //!< Нужное ребро не было найденно
  DelauneyNoEdge = SpecialFailure,             //!< 100
  //!< Нужная грань не была найденна
  DelauneyNoTriangle,                          //!< 101
  //!< При очистке удалилось больше чем надо
  DelauneyClearSoMuch,                         //!< 102
  //!< Одна из концевых вершин маршрута за пределами области
  OneOfPointsOutOfArea = DelauneyNoEdge + 10,  //!< 110
  //!< Нет связности начальной и конечной вершин маршрута
  InconnectedPoints,                           //!< 111
  //!< Некорректное направление поиска
  IncorrectDirection = DelauneyNoEdge + 20,    //!< 120
  //!< У вершины отсутствует привязка к ребру
  TopHasNoEdge,                                //!< 121
  //!< Фиксированное ребро не найдено
  NoFixedEdge,                                 //!< 122
};
//==============================================================================
//!< Префикс исключений TriangularGeometry.
const std::string triangularLibTitle = "Core::TriangularGeometry";
//==============================================================================
/*!
  * \brief Класс формирования исключений библиотеки TriangularGeometry
  */
class TriangularFailure : public ExceptionTools::Failure
{
//==============================================================================
public:
  /*!
   * \brief Конструктор.
   *
   * \details Требуется только код,
   * остальные параметры подставятся автоматически.
   *
   * \param[in] failureId -- именованный код исключения.
   * \param[in] postfix - текст уточняющий/дополняющий исключение
   *                      (может использоваться для передачи текста исходного
   *                       исключения)
   * \param[in] prefix - текст, локализующий место возникновения исключения
   *                     (может использоваться для указания имени функции,
   *                      в которой локализовано исключение - трассировка
   *                      исключения)
   */
  explicit TriangularFailure(const unsigned int failureId,
                             const std::string& postfix = "",
                             const std::string& prefix = "" ) noexcept;
};
//==============================================================================
#endif //! TRIANGULAR_GEOMETRY_EXCEPTIONS_HPP