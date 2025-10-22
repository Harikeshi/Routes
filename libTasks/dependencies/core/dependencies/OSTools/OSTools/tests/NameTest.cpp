#include <OSTools/Name.hpp>
//! gtest
#include <gtest/gtest.h>
//==============================================================================
namespace OS
{
namespace Tests
{
//==============================================================================
/*!
 * \brief Тестирование получения имени сущности по её пути.
 */
TEST( NameTest, GetEntityName )
{
  ASSERT_EQ(
    std::string( "important" ),
    OS::getEntityName( "C:\\Users\\CrocodileGena\\Documents\\\\\\important\\" ) );
  ASSERT_EQ(
    std::string( "very_useless_module.txt" ),
    OS::getEntityName( "./bin//modules/useless_module/very_useless_module.txt" ) );
  ASSERT_EQ( std::string( "" ), OS::getEntityName( "////\\\\" ) );
  ASSERT_EQ( std::string( "name" ), OS::getEntityName( "name/" ) );
}
//==============================================================================
} //! Tests
} //! OS
//==============================================================================
