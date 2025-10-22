#ifndef CARTOGRAPHIC_ALEXEEV_SIMPLEPOLYCONIC_HPP
#define CARTOGRAPHIC_ALEXEEV_SIMPLEPOLYCONIC_HPP
//==============================================================================
#include <Cartographic/iprojection.hpp>
// std
#include <memory>
//==============================================================================
namespace Cartographic
{
namespace Alexeev
{
//==============================================================================
//!
//! \brief Простая поликоническая проекция
//!
//!  Простая поликоническая проекция по версии В. Н. Алексеева
//!
class SimplePolyconic: public IForwardProjection
{
public:
  struct BaseParameters
  {
    // Эллипсоид
    std::shared_ptr<const IEllipsoid> ellipsoid;
    // Широта главной параллели
    Degrees b0;
    // Долгота осевого меридиана
    Degrees l0;
  };

  explicit SimplePolyconic(BaseParameters const &base_parameters);

  void Forward(Degrees b, Degrees l, Meters &x, Meters &y,
               Units *d1 = 0, Units *d2 = 0) const;

private:
  Units mP1, mP2, mP3, mP4;
};
//==============================================================================
} // !Alexeev
} // !Cartographic
//==============================================================================
#endif //! CARTOGRAPHIC_ALEXEEV_SIMPLEPOLYCONIC_HPP
