#include <MathTools/Optimization/KnapsackProblem.hpp>
//! gtest
#include <gtest/gtest.h>
//=============================================================================
namespace Optimization
{
namespace Tests
{
//=============================================================================
/*!
 * \brief Тестирование решения задачи о рюкзаке 0-1.
 */
 //=============================================================================
TEST( KnapsackProblemTest, GeneralTest )
{
  int                capacity( 3 );
  std::vector<int>   weights{ 10, 20, 5, 40 };
  std::vector<float> profits{ 1.f, 1.5f, 5.f, 1.f };
  std::vector<std::size_t> result(
    knapsackProblemSolution( capacity, weights, profits ) );
  ASSERT_EQ( result.size( ), 0 );

  capacity = 20;
  result = knapsackProblemSolution( capacity, weights, profits );
  ASSERT_EQ( result.size( ), 2 );
  bool isExpectedSolution(
    ( ( result[0] == 0 ) && ( result[1] == 2 ) )
    || ( ( result[0] == 2 ) && ( result[1] == 0 ) ) );
  ASSERT_EQ( isExpectedSolution, true );

  weights.clear( );
  profits.clear( );
  result = knapsackProblemSolution( capacity, weights, profits );
  ASSERT_EQ( result.size( ), 0 );

  capacity = 4;
  weights = { 1, 2, 5, 4 };
  profits = { 1.f, 1.5f, 5.f, 1.f };
  result = knapsackProblemSolution( capacity, weights, profits );
  isExpectedSolution =
    ( ( ( result[0] == 0 ) && ( result[1] == 1 ) )
      || ( ( result[0] == 1 ) && ( result[1] == 0 ) ) );
  ASSERT_EQ( isExpectedSolution, true );
}
//=============================================================================
} //! Tests
} //! Optimization
//=============================================================================