#ifndef MATH_TOOLS_BASIC_MATH_ARITHMETIC_HPP
#define MATH_TOOLS_BASIC_MATH_ARITHMETIC_HPP
//==============================================================================
namespace BasicMath
{
//==============================================================================
 /*!
  *\brief Квадрат значения
  *\param value - значение, возводимое в квадрат
  *\return - квадрат
  */
  template <typename T>
  T sqr( const T value )
  {
    return value * value;
  }
//==============================================================================
} //! BasicMath
//==============================================================================
#endif //! MATH_TOOLS_BASIC_MATH_RANGE_HPP