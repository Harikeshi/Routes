#ifndef CARTOGRAPHIC_ALEXEEV_UTM_HPP
#define CARTOGRAPHIC_ALEXEEV_UTM_HPP
//==============================================================================
#include <Cartographic/iprojection.hpp>
//==============================================================================
namespace Cartographic
{
namespace Alexeev
{
//==============================================================================
//!
//! \brief UTM
//!
//! Проекция UTM на эллипсоиде WGS-84 по версии В. Н. Алексеева
//!
class UTM: public IProjection
{
public:
  struct BaseParameters {};

  explicit UTM(BaseParameters const &);

  void Forward(Degrees b, Degrees l, Meters &x, Meters &y,
               Units *d1 = 0, Units *d2 = 0) const;

  void Inverse(Meters x, Meters y, Degrees &b, Degrees &l,
               Units *d1 = 0, Units *d2 = 0) const;
};
//==============================================================================
} // !Alexeev
} // !Cartographic
//==============================================================================
#endif //! CARTOGRAPHIC_ALEXEEV_UTM_HPP
