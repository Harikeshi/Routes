#ifndef SAMPLES_FOR_TESTS_BASE_FLAT_FIGURES_HPP
#define SAMPLES_FOR_TESTS_BASE_FLAT_FIGURES_HPP
//==============================================================================
//! PrimaryEntities
#include <PrimaryEntities/GeometryEntities.hpp>
//==============================================================================
namespace SamplesForTests
{
//==============================================================================
  /*
   *\brief Построить прямоугольник по левому нижнему углу и длинам сторон
   *\param[in] leftBottomPoint - координата левого нижнего угла
   *\param[in] horizontalSize - длина прямоугольника
   *\param[in] verticalSize - высота прямоугольника
   *\return - построенный прямоугольник
   *\throws SamplesForTestsFailure::NegativeDistance
   */
  Polygon2D buildRectangle(const Point2D& leftBottomPoint,
                           const double horizontalSize,
                           const double verticalSize );
//==============================================================================
  /*
   *\brief Построить бантик (некорректная фигура с самопересечением)
   *\details |\/|
   *         |/\|
   *         Boost считает, что у этой фигуры проблемы с направлением обхода.
   *         Хотя на сфере такой же бантик выдает самопересечение.
   *\param[in] leftBottomPoint - координата левого нижнего угла
   *\param[in] horizontalSize - длина бантика
   *\param[in] verticalSize - высота бантика
   *\return - построенный бантик
   *\throws SamplesForTestsFailure::NegativeDistance
   */
  Polygon2D buildBow( const Point2D& leftBottomPoint,
                      const double horizontalSize,
                      const double verticalSize );
//==============================================================================
  /*
   *\brief Построить фигуру с самокасанием
   *\details |\  /|
   *         |_\/_|
   *         Boost считает это фигурой с самопересечением
   *\param[in] leftBottomPoint - координата левого нижнего угла
   *\param[in] horizontalSize - длина конверта
   *\param[in] verticalSize - высота конверта
   *\return - построенный конверт
   *\throws SamplesForTestsFailure::NegativeDistance
   */
  Polygon2D buildPost( const Point2D& leftBottomPoint,
                       const double horizontalSize,
                       const double verticalSize );
//==============================================================================
} //! SamplesForTests
//==============================================================================
#endif //! SAMPLES_FOR_TESTS_BASE_FLAT_FIGURES_HPP