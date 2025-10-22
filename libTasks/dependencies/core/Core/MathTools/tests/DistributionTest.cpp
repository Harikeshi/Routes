#include <MathTools/Optimization/Distribution.hpp>
//! gtest
#include <gtest/gtest.h>
//==============================================================================
namespace Optimization
{
namespace Tests
{
//==============================================================================
  /*!
   * \brief Тестирование оценки возможности равноценного распределения объектов.
   */
   //==============================================================================
  TEST( DistributionTest, EstimateEquivalentDistribution )
{
  std::vector<unsigned int> multilinkedUser = { 2U, 4U, 3U, 4U };         //!< 13
  std::vector<unsigned int> simplyConnectedUsers{ 5U, 2U, 4U, 3U, 9U };  //!< 23
  std::vector<unsigned int> providers = { 8U, 8U, 8U, 4U };               //!< 28


  //! Не хватает даже на односвязных потребителей по количеству поставщиков
  ASSERT_EQ( -22, estimateEquivalentDistribution( multilinkedUser,
    simplyConnectedUsers,
    providers ) );

  //! Всё ещё не хватает объектов на потребителей
  providers.emplace_back( 8U );                                           //!< 36 
  ASSERT_EQ( -14, estimateEquivalentDistribution( multilinkedUser,
    simplyConnectedUsers,
    providers ) );

  //! Казалось бы хватает, но нет поставщика способного охватить в одиночку 
  //! потребителя
  providers.emplace_back( 6U );                                           //!< 42 
  providers.emplace_back( 8U );                                           //!< 50 
  ASSERT_EQ( -1, estimateEquivalentDistribution( multilinkedUser,
    simplyConnectedUsers,
    providers ) );

  //! А вот теперь хватает 
  //! (и даже излишки на носителях есть)
  providers.pop_back( );
  providers.emplace_back( 10U );                                          //!< 52 
  ASSERT_EQ( 3, estimateEquivalentDistribution( multilinkedUser,
    simplyConnectedUsers,
    providers ) );
}
//==============================================================================
} //! Tests
} //! Optimization
//==============================================================================
