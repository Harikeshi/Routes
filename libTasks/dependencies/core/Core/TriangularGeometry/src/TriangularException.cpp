#include <TriangularGeometry/TriangularException.hpp>
//! std
#include <map>
//==============================================================================
//==============================================================================
/*!<
 * Соотношение именованных кодов и текстов ошибок генерируемых исключений.
 */
const std::map<unsigned int, std::string> triangularFailuresText =
{
  INCLUDE_STANDART_FAILURE_IN_MAP
  {
    DelauneyNoEdge, "<DelauneyNoEdge>: Нужное ребро не было найденно"
  },
  {
    DelauneyNoTriangle, "<DelauneyNoTriangle>: Нужная грань не была найденна"
  },
  {
    DelauneyClearSoMuch, "<DelauneyClearSoMuch>: "
    "При очистке удалилось больше, чем надо"
  },
  {
    OneOfPointsOutOfArea, "<OneOfPointsOutOfArea>: "
    "Одна из концевых вершин за пределами области"
  },
  {
    InconnectedPoints, "<InconnectedPoints>: "
    "Нет связности начальной и конечной вершин маршрута"
  },
  {
    IncorrectDirection, "<IncorrectDirection>: "
      "Некорректное направление поиска"
  },
  {
    TopHasNoEdge, "<TopHasNoEdge>: "
    "У вершины отсутствует привязка к ребру"
  },
  {
    NoFixedEdge, "<NoFixedEdge>: "
    "Фиксированное ребро не найдено"
  }
};
//==============================================================================
TriangularFailure::TriangularFailure(const unsigned int failureId,
                                     const std::string& postfix /*= ""*/,
                                     const std::string& prefix /*= ""*/) noexcept
: Failure(
  failureId,
  triangularLibTitle,
  triangularFailuresText.at(static_cast<TriangularEnumFailure>(failureId)),
  postfix,
  prefix)
{}
//==============================================================================
