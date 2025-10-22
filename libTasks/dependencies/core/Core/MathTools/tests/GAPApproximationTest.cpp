#include <MathTools/Optimization/GAPApproximation.hpp>
//! gtest
#include <gtest/gtest.h>
//=============================================================================
namespace Optimization
{
namespace Tests
{
//=============================================================================
/*!
 * \brief Тестирование решения обобщённой задачи о назначениях.
 */
 //=============================================================================
TEST( GAPApproximationTest, GeneralTest )
{
  std::vector<std::vector<int>> usersWeights( 3 );
  usersWeights[0] = { 100, 10, 1, 2 };
  usersWeights[1] = { 5, 5000, 50, 7 };
  usersWeights[2] = { 40, 4, 400, 4 };
  std::vector<int>                providersCapacities{ 30, 40, 50, 60 };
  std::vector<std::vector<float>> usersRates( 3 );
  usersRates[0] = { 0.f, 8610.f, 0.f, 0.f };
  usersRates[1] = { 2294.f, 0.f, 4596.f, 5832.f };
  usersRates[2] = { 1348.f, 1329.f, 1105.f, 0.f };
  std::vector<int> result(
    GAPApproximation( usersWeights, providersCapacities, usersRates ) );
  ASSERT_EQ( result.size( ), 3 );
  ASSERT_EQ( result[0], 1 );
  ASSERT_EQ( result[1], 3 );
  ASSERT_EQ( result[2], 1 );
}
//=============================================================================
} //! Tests
} //! Optimization
//=============================================================================