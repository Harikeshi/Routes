#include <ReachableAreaCalculator.hpp>
//! Inner
#include <Inner/ReachableArea.hpp>
#include <Inner/Arcs.hpp>
#include <Inner/Visibility.hpp>
//==============================================================================
namespace ReachableAreaCalculator
{
//==============================================================================
  void ReachableAreaResult::clear() noexcept
  {
    area.clear();
    borders.clear();
  }
//==============================================================================
  bool ReachableArea::calculate(
    const Environment& environment,
    const Point2D& center, 
    double maxPath,
    ReachableAreaResult& result, 
    unsigned int mSegmentsNumber, 
    int threadsCount)
  {
    ReachableAreaInner calculator(environment.getInner());

    mInnerCalculator = &calculator;

    QObject::connect(&calculator,
      SIGNAL(processInfo(quint8, quint64, quint64)),
      this,
      SIGNAL(processInfo(quint8, quint64, quint64)));

    bool success = calculator.calculate(
      center, maxPath, result, mSegmentsNumber, threadsCount);

    mInnerCalculator = nullptr;

    return success;
  }
//==============================================================================
  void ReachableArea::abort()
  {
    if (mInnerCalculator)
      mInnerCalculator->abort();
  }
//==============================================================================
} // !ReachableAreaCalculator
//==============================================================================