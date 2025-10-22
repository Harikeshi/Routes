#include <MathTools/BasicMath/Compare.hpp>
//! Constants
#include <MathTools/Constants.hpp>
//! AngularUnits
#include <MathTools/AngularUnits/Degree.hpp>
#include <MathTools/AngularUnits/Radian.hpp>
//! gtest
#include <gtest/gtest.h>
//==============================================================================
//! Константы, задающие величины гарантировано входящие (inside) или выходящие 
//! (outside) за пределы требуемой точности для типов данных, для которых
//! реализованы (перегружены) функции из Compare
constexpr float OUTSIDE_FLOAT = ACCURACY_FLOAT / 10;
constexpr float INSIDE_FLOAT  = ACCURACY_FLOAT * 10;

constexpr double OUTSIDE_DOUBLE = ACCURACY_DOUBLE / 10;
constexpr double INSIDE_DOUBLE  = ACCURACY_DOUBLE * 10;

constexpr double OUTSIDE_DEGREE = ACCURACY_DEGREE / 10;
constexpr double INSIDE_DEGREE  = ACCURACY_DEGREE * 10;

constexpr double OUTSIDE_RADIAN = ACCURACY_RADIAN / 10;
constexpr double INSIDE_RADIAN  = ACCURACY_RADIAN * 10;
//==============================================================================
namespace BasicMath
{
namespace Tests
{
//==============================================================================
/*!
* \brief Тестирование isEqual
*        Сравнение float с требуемой точностью
*/
TEST( CompareTest, FloatIsEqual )
{
  ASSERT_TRUE( isEqual( float( -OUTSIDE_FLOAT ), float( OUTSIDE_FLOAT ) ) );
  ASSERT_FALSE( isEqual( float( -INSIDE_FLOAT ), float( INSIDE_FLOAT ) ) );
}
//==============================================================================
/*!
* \brief Тестирование isEqual
*        Сравнение double с требуемой точностью
*/
TEST( CompareTest, DoubleIsEqual )
{
  ASSERT_TRUE( isEqual( -OUTSIDE_DOUBLE, OUTSIDE_DOUBLE ) );
  ASSERT_FALSE( isEqual( -INSIDE_DOUBLE, INSIDE_DOUBLE ) );
}
//==============================================================================
/*!
* \brief Тестирование isEqual
*        Сравнение градусов с требуемой точностью
*/
TEST( CompareTest, DegreeIsEqual )
{
  ASSERT_TRUE( isEqual( Degree( -OUTSIDE_DEGREE ), Degree( OUTSIDE_DEGREE ) ) );
  ASSERT_FALSE( isEqual( Degree( -INSIDE_DEGREE ), Degree( INSIDE_DEGREE ) ) );
}
//==============================================================================
/*!
* \brief Тестирование isEqual
*        Сравнение радиан с требуемой точностью
*/
TEST( CompareTest, RadianIsEqual )
{
  ASSERT_TRUE( isEqual( Radian( -OUTSIDE_RADIAN ), Radian( OUTSIDE_RADIAN ) ) );
  ASSERT_FALSE( isEqual( Radian( -INSIDE_RADIAN ), Radian( INSIDE_RADIAN ) ) );
}
//==============================================================================
/*!
* \brief Тестирование isNull для float
*/
TEST( CompareTest, FloatIsNull )
{
  ASSERT_TRUE( isNull( OUTSIDE_FLOAT ) );
  ASSERT_FALSE( isNull( INSIDE_FLOAT ) );
}
//==============================================================================
/*!
* \brief Тестирование isNull для double
*/
TEST( CompareTest, DoubleIsNull )
{
  ASSERT_TRUE( isNull( OUTSIDE_DOUBLE ) );
  ASSERT_FALSE( isNull( INSIDE_DOUBLE ) );
}
//==============================================================================
/*!
* \brief Тестирование isNull для градусов
*/
TEST( CompareTest, DegreeIsNull )
{
  ASSERT_TRUE( isNull( Degree( OUTSIDE_DEGREE ) ) );
  ASSERT_FALSE( isNull( Degree( INSIDE_DEGREE ) ) );
}
//==============================================================================
/*!
* \brief Тестирование isNull для радиан
*/
TEST( CompareTest, RadianIsNull )
{
  ASSERT_TRUE( isNull( Radian( OUTSIDE_RADIAN ) ) );
  ASSERT_FALSE( isNull( Radian( INSIDE_RADIAN ) ) );
}
//==============================================================================
} //! Tests
} //! BasicMath
//==============================================================================