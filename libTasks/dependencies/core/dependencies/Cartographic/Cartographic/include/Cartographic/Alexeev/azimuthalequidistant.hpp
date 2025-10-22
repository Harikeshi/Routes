#ifndef CARTOGRAPHIC_ALEXEEV_AZIMUTHALEQUIDISTANT_HPP
#define CARTOGRAPHIC_ALEXEEV_AZIMUTHALEQUIDISTANT_HPP
//==============================================================================
#include <Cartographic/iprojection.hpp>
// std
#include <memory>
//==============================================================================
namespace Cartographic
{
//==============================================================================
class Sphere;
//==============================================================================
namespace Alexeev
{
//==============================================================================
//!
//! \brief Азимутальная нормальная равнопромежуточная проекция
//!
//! Азимутальная нормальная равнопромежуточная проекция на сфере
//! по версии В. Н. Алексеева
//!
class AzimuthalEquidistant: public IForwardProjection
{
public:
  struct BaseParameters
  {
    // Шар
    std::shared_ptr<const Sphere> sphere;
    // Широта главной параллели
    Degrees b0;
    // Долгота начального меридиана
    Degrees l0;
  };

  explicit AzimuthalEquidistant(BaseParameters const &base_parameters);

  void Forward(Degrees b, Degrees l, Meters &x, Meters &y,
               Units *d1 = 0, Units *d2 = 0) const;

private:
  Units mP1, mP2;
};
//==============================================================================
} // !Alexeev
//==============================================================================
} // !Cartographic
//==============================================================================
#endif //! CARTOGRAPHIC_ALEXEEV_AZIMUTHALEQUIDISTANT_HPP
