#ifndef CARTOGRAPHIC_ALEXEEV_GAUSSKRUGER_HPP
#define CARTOGRAPHIC_ALEXEEV_GAUSSKRUGER_HPP
//==============================================================================
#include <Cartographic/iprojection.hpp>
//==============================================================================
namespace Cartographic
{
namespace Alexeev
{
//==============================================================================
//!
//! \brief Проекция Гаусса-Крюгера
//!
//! Проекция Гаусса-Крюгера на эллипсоиде Красовского по версии В. Н. Алексеева
//!
class GaussKrueger: public IProjection
{
public:
  struct BaseParameters {};

  explicit GaussKrueger(BaseParameters const &);

  void Forward(Degrees b, Degrees l, Meters &x, Meters &y,
               Units *d1 = 0, Units *d2 = 0) const;

  void Inverse(Meters x, Meters y, Degrees &b, Degrees &l,
               Units *d1 = 0, Units *d2 = 0) const;
};
//==============================================================================
//!
//! \brief Проекция Гаусса-Крюгера с зоной
//!
//! Проекция Гаусса-Крюгера для конкретной зоны на эллипсоиде Красовского
//! по версии В. Н. Алексеева
//!
class GaussKruegerForZone: public IProjection
{
public:
  struct BaseParameters
  {
    Units zoneNo;
  };

  explicit GaussKruegerForZone(BaseParameters const &baseParameters);

  void Forward(Degrees b, Degrees l, Meters &x, Meters &y,
               Units *d1 = 0, Units *d2 = 0) const;

  void Inverse(Meters x, Meters y, Degrees &b, Degrees &l,
               Units *d1 = 0, Units *d2 = 0) const;

private:
  Units const mZoneNo;
};
//==============================================================================
} // !Alexeev
} // !Cartographic
//==============================================================================
#endif //! CARTOGRAPHIC_ALEXEEV_GAUSSKRUGER_HPP
